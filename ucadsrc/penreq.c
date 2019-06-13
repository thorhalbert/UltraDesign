/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	penreq.c - Pen requesters
 *
 */

/*
 *	Copyright (C) 1986, 1987 by WURBsoft Inc., and Scott Halbert.
 *
 *	No portion of this product in any form (this includes but is not
 *	limited to code sources, binaries, documentation, data or other), 
 *	may be used, transferred, or kept by any parties other than the 
 *	owners (WURBsoft Inc. and Scott Halbert),  unless the proper 
 *	licenses have been executed between the user and WURBsoft Inc.
 *
 *	Any use not covered by a legal software license is considered 
 *	by us to be Software Piracy, and constitutes a violation of our 
 *	rights.  Don't get us mad.
 *
 */

/*
 *	Modification History
 *
 *	0	11-Jan-87 SH	Create new template
 *	1	11-May-87 SH	Generate new program from scratch
 *	2	13-Jun-87 SH	Add pen requesters
 *	3	28-Jun-87 SH	Layout tree requester code
 *	4	12-Sep-87 SH	Add Instant Response to prop slider
 *	5	13-Sep-87 SH	Break attributes.c into pieces
 *	6	11-May-88 SH	Add pen information to scroller display
 *	7	10-Nov-88 SH	Break out stuff to penhandle.c
 *	8	 7-May-89 SH	Copy report code from layerreq
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include "attributes.h"

/*----------------------------------Macros-----------------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*------------------------Set up scroller for pens------------------------*/

LOCAL BOOL exflag;
LOCAL int postaction;

PEN_A *hold_pen=NULL;

VOID *Pen_First();
VOID *Pen_Next();
VOID *Pen_Prev();
VOID *Pen_Search();
VOID *Pen_Parent();
VOID *Pen_Descend();
VOID *Pen_Form();
VOID *Pen_Name();

BOOL ReportPen()
{

	PEN_A *looper;

	FILE *ofil;

	ofil = openprt("PenReport");
	if (ofil==NULL) return(FALSE);
	
	fprintf(ofil,"%s Pen Listing ",ucadname);
	if (Savnam) fprintf(ofil,"(File %s)",Savefile);
	fprintf(ofil,"\n\n\r");
	
	fprintf(ofil,"Name of Drawing Pem-----------Counts\n\n\r");
	
	for (looper=Root_Pen; looper!=NULL; looper=looper->pen_next) 
		fprintf(ofil,"   %-32.32s %4d\n\r",
			looper->Pen_Name,looper->Use_Count);

	fclose(ofil);
	
}

BOOL Pen_Sc(start,title,new)
PEN_A *start;
UBYTE *title;
BOOL new;
{

	return(Scroller(Pen_First,
		Pen_Next,
		Pen_Prev,
		Pen_Search,
		Pen_Parent,
		Pen_Descend,
		Pen_Form,
		Pen_Name,
		FALSE,
		TRUE,
		new,
		start,
		"    OK    ",
		"  Cancel  ",
		title,
		"Name of Drawing Pen--------------Counts-Flag--[O/C/A]-----",
		&hold_pen,
		PenMask,
		"(Pen Table)"));
}

BOOL Pen_NewCurrent()
{

	if (Current_Pen==NULL) Current_Pen=Root_Pen;

	return(Pen_Sc(Current_Pen,"Select New Current Pen",FALSE));

}
Pen_Add()
{

	BOOL getback;
	PEN_A *newpen;

	if (Current_Pen==NULL) Current_Pen=Root_Pen;

	getback=Pen_Sc(NULL,"Enter New Pen Name",TRUE);
	
	CleanOutArgText();
	
	if (!getback) return;

	newpen = (PEN_A *) 
		GI_RemAlloc(sizeof (PEN_A));
	if (newpen==NULL) {
		DispMess("Insufficient memory to create new pen - sorry");
		return;
	}

	if (Current_Pen==NULL) *newpen = default_pen;
	else	*newpen = *Current_Pen;
	ArgText[30] = '\0';

	sprintf(newpen->Pen_Name,"%s",ArgText);
	newpen->Use_Count = 0;

	Link_Pen(newpen);

	Current_Pen = newpen;

	Pen_Current();		/* Fix up the new pen */

}

/*---------------------Drawing Pen Edit Requester---------------------*/


LOCAL PEN_A cancelpen;
LOCAL struct Requester PenEditRequester;

VOID save_curpen();

VOID PEHandler(ingad)
struct Gadget *ingad;
{

	CloseRequest(&PenEditRequester,w);

	switch (ingad->GadgetID) {
	case 41:
		save_curpen();
		postaction = 4;
		Current_Pen = Current_Pen->pen_prev;
		if (Current_Pen==NULL) Current_Pen = Last_Pen;
		return;
	case 42:
		save_curpen();
		postaction = 4;
		Current_Pen = Current_Pen->pen_next;
		if (Current_Pen==NULL) Current_Pen = Root_Pen;
		return;
	case 43:
		postaction = 10;
		return;
	case 44:
		postaction = 11;
		return;
	case 45:
		postaction = 12;
		return;
	case 46:
		postaction = 13;
		return;
	case 47:
		postaction = 14;
		return;
	case 20:
		*Current_Pen = cancelpen;
		exflag=FALSE;
		postaction = 0;
		return;
	case 21:
		exflag=FALSE;
		postaction = 3;
		return;
	case 22:
		postaction = 2;
		return;
	case 23:
		postaction = 1;
		return;
	case 24: 
		postaction = 5;
		return;
	}

	return;

}


LOCAL UBYTE PEComment[82],PEDats[3][11];
LOCAL struct StringInfo PEStringSpec[] = {
	{ &PEComment[0], undobuf, 0, 80,0,0,0,0,0,0,NULL,0,NULL },
	{ &PEDats[0][0], undobuf, 0, 10,0,0,0,0,0,0,NULL,0,NULL },
	{ &PEDats[1][0], undobuf, 0, 10,0,0,0,0,0,0,NULL,0,NULL },
	{ &PEDats[2][0], undobuf, 0, 10,0,0,0,0,0,0,NULL,0,NULL }};

UWORD Pen_Ord,Carousel_Num,Auto_Num;
ULONG Hard_Color;

VOID PEStringHandler();

extern struct Image attrib_1s_image;
extern struct Image attrib_1u_image;
extern struct Image attrib_2s_image;
extern struct Image attrib_2u_image;
extern struct Image attrib_3s_image;
extern struct Image attrib_3u_image;
extern struct Image attrib_4s_image;
extern struct Image attrib_4u_image;
extern struct Image add_s_image;		/* 59 * 15 */
extern struct Image add_u_image;
extern struct Image rename_s_image;		/* 59 * 15 */
extern struct Image rename_u_image;
extern struct Image kill_s_image;		/* 59 * 15 */
extern struct Image kill_u_image;
extern struct Image expunge_s_image;		/* 63 * 15 */
extern struct Image expunge_u_image;
extern struct Image merge_s_image;		/* 59 * 15 */
extern struct Image merge_u_image;		
extern struct Image uparrow_u_image;		/* 20 * 17 */
extern struct Image uparrow_s_image;
extern struct Image dnarrow_u_image;
extern struct Image dnarrow_s_image;

#define REQWID 450

VOID doline();
#define PEACTIVE GADGIMMEDIATE | RELVERIFY
#define PETACTIVE GADGIMMEDIATE | RELVERIFY | TOGGLESELECT
#define HILITE GADGIMAGE | GADGHIMAGE

LOCAL UBYTE PEText[2][32];
LOCAL struct IntuiText PEListText[] = {
	{ 2,3,JAM2,0,0,NULL,"  Cancel  ",NULL },
	{ 2,3,JAM2,0,0,NULL,"    OK    ",NULL },
	{ 3,2,JAM2,0,0,NULL,&PEText[0][0],NULL },
	{ 3,2,JAM2,0,0,NULL,"        ",NULL },
	{ 3,2,JAM2,0,0,NULL,"   ",NULL },
	{ 3,2,JAM2,0,0,NULL,"        ",NULL },
	};
ULONG victim_red,victim_green,victim_blue;
int victim_color=0;

VOID Save_Victim(ourcolor,makecolor)
int ourcolor;
ULONG makecolor;
{
	ULONG make_red,make_green,make_blue;

	victim_color = 7;
	if (ourcolor==victim_color) victim_color = 6;
	
	GI_GetColor(victim_color,&victim_red,&victim_green,&victim_blue);

	make_blue = (makecolor & 0xFF) << 8;
	makecolor >>= 8;
	make_green = (makecolor & 0xFF) << 8;
	makecolor >>= 8;
	make_red = (makecolor & 0xFF) << 8;

	PEListText[5].FrontPen = victim_color;
	PEListText[5].BackPen = victim_color;

	GI_SetColor(victim_color,make_red,make_green,make_blue);
}

ULONG Query_Victim()
{

	ULONG make_red,make_green,make_blue;
	ULONG makecolor;

	GI_GetColor(victim_color,&make_red,&make_green,&make_blue);
	
	makecolor = ((make_red >> 8) << 16) |
		    ((make_green >> 8) << 8) |
		    (make_blue >> 8);
		    
	return(makecolor);
	
}

VOID Restore_Victim()
{

	if (victim_color==0) return;
	GI_SetColor(victim_color,victim_red,victim_green,victim_blue);
	victim_color = 0;
	
}

LOCAL struct Gadget PEGadgets[] = {
	{ &PEGadgets[1], 80, 102, 350, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &PEStringSpec[0], 34, PEStringHandler  },
	{ &PEGadgets[2], 65, 27, 85, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &PEStringSpec[1], 35, PEStringHandler  },
	{ &PEGadgets[3], 232, 27, 64, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &PEStringSpec[2], 36, PEStringHandler  },
	{ NULL, 378, 27, 64, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &PEStringSpec[3], 37, PEStringHandler  }};

LOCAL struct Gadget PEList[] = {
	{ &PEList[1], 270,117,80,8,GADGHCOMP,PEACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PEListText[0],0,NULL,20,PEHandler},
	{ &PEList[2], 100,117,80,8,GADGHCOMP,PEACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PEListText[1],0,NULL,21,PEHandler},
	{ &PEList[3], 65,17,240,8,GADGHCOMP,PEACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PEListText[2],0,NULL,22,PEHandler},
	{ &PEList[4], 365,17,67,8,GADGHCOMP,PEACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PEListText[3],0,NULL,23,PEHandler},

#define OTLEF 30 /*(((REQWID-20-160)/2)+10)*/
	{ &PEList[5], OTLEF,57,31,27,HILITE,PETACTIVE,BOOLGADGET|REQGADGET,
		&attrib_1s_image,&attrib_1u_image,NULL,0,NULL,40,NULL},
	{ &PEList[7], OTLEF+40,57,31,27,HILITE,PETACTIVE,BOOLGADGET|REQGADGET,
		&attrib_2s_image,&attrib_2u_image,NULL,0,NULL,40,NULL},
		/* Skip around the lock button */
	{ &PEList[7], OTLEF+80,57,31,27,HILITE,PETACTIVE,BOOLGADGET|REQGADGET,
		&attrib_3s_image,&attrib_3u_image,NULL,0,NULL,40,NULL},
		
	{ &PEList[8], OTLEF+80,57,31,27,HILITE,PETACTIVE,BOOLGADGET|REQGADGET,
		&attrib_4u_image,&attrib_4s_image,NULL,0,NULL,40,NULL},

#define BUTBLK (((REQWID-20-(302+64))/2)+10)
	{ &PEList[9], BUTBLK,40,29,15,HILITE,PEACTIVE,BOOLGADGET|REQGADGET,
		&uparrow_u_image,&uparrow_s_image,NULL,0,NULL,41,PEHandler},
	{ &PEList[10], BUTBLK+30,40,29,15,HILITE,PEACTIVE,BOOLGADGET|REQGADGET,
		&dnarrow_u_image,&dnarrow_s_image,NULL,0,NULL,42,PEHandler},
	{ &PEList[11], BUTBLK+62,40,59,15,HILITE,PEACTIVE,BOOLGADGET|REQGADGET,
		&add_u_image,&add_s_image,NULL,0,NULL,43,PEHandler},
	{ &PEList[12], BUTBLK+122,40,59,15,HILITE,PEACTIVE,BOOLGADGET|REQGADGET,
		&rename_u_image,&rename_s_image,NULL,0,NULL,44,PEHandler},
	{ &PEList[13], BUTBLK+182,40,59,15,HILITE,PEACTIVE,BOOLGADGET|REQGADGET,
		&kill_u_image,&kill_s_image,NULL,0,NULL,45,PEHandler},
	{ &PEList[14], BUTBLK+242,40,59,15,HILITE,PEACTIVE,BOOLGADGET|REQGADGET,
		&merge_u_image,&merge_s_image,NULL,0,NULL,46,PEHandler},
	{ &PEList[15], BUTBLK+302,40,63,15,HILITE,PEACTIVE,BOOLGADGET|REQGADGET,
		&expunge_u_image,&expunge_s_image,NULL,0,NULL,47,PEHandler},

#define FSTART 15
#define BPT(a) (3+(28*a))
	{ &PEList[16], BPT(0),87,24,8,GADGHCOMP,PETACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PEListText[4],0,NULL,100,doline},
	{ &PEList[17], BPT(1),87,24,8,GADGHCOMP,PETACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PEListText[4],0,NULL,100,doline},
	{ &PEList[18], BPT(2),87,24,8,GADGHCOMP,PETACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PEListText[4],0,NULL,100,doline},
	{ &PEList[19], BPT(3),87,24,8,GADGHCOMP,PETACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PEListText[4],0,NULL,100,doline},
	{ &PEList[20], BPT(4),87,24,8,GADGHCOMP,PETACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PEListText[4],0,NULL,100,doline},
	{ &PEList[21], BPT(5),87,24,8,GADGHCOMP,PETACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PEListText[4],0,NULL,100,doline},
	{ &PEList[22], BPT(6),87,24,8,GADGHCOMP,PETACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PEListText[4],0,NULL,100,doline},
	{ &PEList[23], BPT(7),87,24,8,GADGHCOMP,PETACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PEListText[4],0,NULL,100,doline},
	{ &PEList[24], BPT(8),87,24,8,GADGHCOMP,PETACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PEListText[4],0,NULL,100,doline},
	{ &PEList[25], BPT(9),87,24,8,GADGHCOMP,PETACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PEListText[4],0,NULL,100,doline},
	{ &PEList[26], BPT(10),87,24,8,GADGHCOMP,PETACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PEListText[4],0,NULL,100,doline},
	{ &PEList[27], BPT(11),87,24,8,GADGHCOMP,PETACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PEListText[4],0,NULL,100,doline},
	{ &PEList[28], BPT(12),87,24,8,GADGHCOMP,PETACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PEListText[4],0,NULL,100,doline},
	{ &PEList[29], BPT(13),87,24,8,GADGHCOMP,PETACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PEListText[4],0,NULL,100,doline},
	{ &PEList[30], BPT(14),87,24,8,GADGHCOMP,PETACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PEListText[4],0,NULL,100,doline},
	{ &PEList[31], BPT(15),87,24,8,GADGHCOMP,PETACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PEListText[4],0,NULL,100,doline},

	{ &PEGadgets[0], 365,65,67,8,GADGHCOMP,PEACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PEListText[5],0,NULL,24,PEHandler},

};

LOCAL struct IntuiText PEHeadings[] = {
	{ 3,1,JAM2,8,6,NULL,"Current Drawing Pen Settings:",&PEHeadings[1]},
	{ 2,1,JAM2,8,17,NULL,"Pen:",&PEHeadings[2]},
	{ 2,1,JAM2,313,17,NULL,"Color:",&PEHeadings[3]},
	{ 2,1,JAM2,8,102,NULL,"Comment:",&PEHeadings[4]},
	{ 2,1,JAM2,8,27,NULL,"Order:",&PEHeadings[5]},
	{ 2,1,JAM2,158,27,NULL,"Carousel:",&PEHeadings[6]},
	{ 2,1,JAM2,313,27,NULL,"Autopen:",NULL} };

struct RastPort *penrp;

VOID doline()
{
	UWORD pat;
	int a;
	
	pat = 0;
	for (a=0; a<16; a++) {
		if (PEList[FSTART+a].Flags & SELECTED)
			pat |= 1<<a;
	}

		
	SetAPen(penrp, 1);
	Move(penrp, 3, 98);
	Draw(penrp, (REQWID-1)-3, 98);

	SetAPen(penrp, 0);
	SetBPen(penrp, 1);
	
	SetDrPt(penrp, pat); 
	Move(penrp, 3, 98);
	Draw(penrp, (REQWID-1)-3, 98);

	SetDrPt(penrp, 0xFFFF); 
}

VOID PEStringHandler(ingad)
struct Gadget *ingad;
{

	int button,a;
	
	Forbid();
	ingad->Flags &= ~SELECTED;	/* Make sure it is unselected */
	Permit();

	button = ingad->GadgetID;
	if (button==34) return;
	if (button==35) 
		if (sscanf(&PEDats[0][0],"%d",&a)==1) Pen_Ord = a;
	if (button==36) 
		if (sscanf(&PEDats[1][0],"%d",&a)==1) Carousel_Num = a;
	if (button==37) 
		if (sscanf(&PEDats[2][0],"%d",&a)==1) Auto_Num = a;

	sprintf(&PEDats[0][0],"%5d",Pen_Ord);
	sprintf(&PEDats[1][0],"%5d",Carousel_Num);
	sprintf(&PEDats[2][0],"%5d",Auto_Num);

	RefreshGList(&PEList[0],w,&PenEditRequester,3);

	return;

}

VOID save_curpen()
{
	int a;
	
	Current_Pen->Pen_Flags &= ~(PEN_GHOSTED|PEN_DOFF|
			PEN_LOCKED|PEN_POFF); 
	if (PEList[4].Flags & SELECTED) 
		Current_Pen->Pen_Flags |= PEN_GHOSTED; 
	if (PEList[5].Flags & SELECTED) 
		Current_Pen->Pen_Flags |= PEN_DOFF; 
	if (PEList[6].Flags & SELECTED) 
		Current_Pen->Pen_Flags |= PEN_LOCKED; 
	if (PEList[7].Flags & SELECTED) 
		Current_Pen->Pen_Flags |= PEN_POFF; 

	Current_Pen->Line_Pattern = 0;	
	for (a=0; a<16; a++) {
		if (PEList[FSTART+a].Flags & SELECTED)
			Current_Pen->Line_Pattern |= 1<<a;
	}

	Current_Pen->Hard_Color = Query_Victim();
	Current_Pen->PenOrd = Pen_Ord;
	Current_Pen->Carousel = Carousel_Num;
	Current_Pen->AutoNum = Auto_Num;

	if (strcmp(PEComment,Current_Pen->Comment)!=0) 
		Current_Pen->Comment = StoreComment(PEComment);

}

PEN_A *beginpen;

picknew()
{
	beginpen = Current_Pen;
	cancelpen = *Current_Pen;

	Restore_Victim();
	Save_Victim(Current_Pen->Color_ID,Current_Pen->Hard_Color);
}

Pen_Current()
{

	PEN_A *tpen;
	UBYTE centerbuf[55],*dbuf,*incbuf;
	int numspa,a;
	beginpen = Current_Pen;
	cancelpen = *Current_Pen;
	exflag = TRUE;

	Save_Victim(Current_Pen->Color_ID,Current_Pen->Hard_Color);

	while (exflag) {

		InitRequester(&PenEditRequester);

		PenEditRequester.Width=REQWID;
		PenEditRequester.Height=130;
		PenEditRequester.LeftEdge=(XSsize-PenEditRequester.Width)/2;
		PenEditRequester.TopEdge=(YSsize-PenEditRequester.Height)/2;

		PenEditRequester.BackFill=1;

		PenEditRequester.ReqText = &PEHeadings[0];
		PenEditRequester.ReqGadget = &PEList[0];

		dbuf = Current_Pen->Pen_Name;
		numspa = (30 - strlen(dbuf))/2;
		for (incbuf = centerbuf; numspa>=0; numspa--,incbuf++) 
				*incbuf=' ';
		sprintf(incbuf,"%-30.30s",dbuf);
		sprintf(&PEText[0][0],"%-30.30s",centerbuf);
	
		sprintf(&PEText[1][0],"%30s"," ");
		PEListText[3].FrontPen = Current_Pen->Color_ID;
		PEListText[3].BackPen = Current_Pen->Color_ID;

		sprintf(PEComment,"%s",Current_Pen->Comment);

		PEList[4].Flags &= (~SELECTED);
		PEList[5].Flags &= (~SELECTED);
		PEList[6].Flags &= (~SELECTED);
		PEList[7].Flags &= (~SELECTED);
		if (Current_Pen->Pen_Flags & PEN_GHOSTED) 
			PEList[4].Flags |= SELECTED;
		if (Current_Pen->Pen_Flags & PEN_DOFF) 
			PEList[5].Flags |= SELECTED;
		if (Current_Pen->Pen_Flags & PEN_LOCKED) 
			PEList[6].Flags |= SELECTED;
		if (Current_Pen->Pen_Flags & PEN_POFF) 
			PEList[7].Flags |= SELECTED;

		Pen_Ord = Current_Pen->PenOrd;
		Carousel_Num = Current_Pen->Carousel;
		Auto_Num = Current_Pen->AutoNum;

		sprintf(&PEDats[0][0],"%5d",Pen_Ord);
		sprintf(&PEDats[1][0],"%5d",Carousel_Num);
		sprintf(&PEDats[2][0],"%5d",Auto_Num);

		for (a=0; a<16; a++) {
			PEList[FSTART+a].Flags &= (~SELECTED);
			if (Current_Pen->Line_Pattern&(1<<a))
				PEList[FSTART+a].Flags |= SELECTED;
		}

#ifdef _TURNSY
		PenEditRequester.Flags = NOISYREQ;
#endif

		SafeRequest(&PenEditRequester, w);

		postaction = 0;

		penrp = PenEditRequester.ReqLayer->rp;	/* Nasty */
		
		DoBorders(&PenEditRequester,15,37,85);

		SetAPen(penrp,2);
		SetBPen(penrp,3);
		SetDrMd(penrp,JAM2);
		dbuf = "Hardcopy Color:";
		Move(penrp,363-(strlen(dbuf)*8),71);
		Text(penrp,dbuf,strlen(dbuf));
		
		RefreshGList(&PEList[4],w,&PenEditRequester,11);
		RefreshGList(&PEList[31],w,&PenEditRequester,1);
		
		doline();
		
		sit_n_spin(&PenEditRequester,0,&PEList[0],&PEList[1]);
				/* Wait for requesters to exit */

		switch (postaction) {
		case 1:
			save_curpen();
			Hard_Color = Query_Victim();
			Restore_Victim();
			Current_Pen->Color_ID=
				Palette(Current_Pen->Color_ID);
			Save_Victim(Current_Pen->Color_ID,Hard_Color);
			break;
		case 5:
			save_curpen();
			Palette((-victim_color)-1);	/* Edit hardcolor */
			break;
		case 2:			/* Undo old changes, switch over */
			save_curpen();
			if (!Pen_NewCurrent()) break;
			if (scrollanswer==NULL) {
				oneliner("Pick a pen which exists!");
				break;
			}
			Current_Pen = (PEN_A *) scrollanswer;
			picknew();
			break;
		case 3:
			save_curpen();
			break;

		case 4:
			picknew();
			break;
		case 10:
			save_curpen();
			Pen_Add();
			picknew();
			break;

		case 11:
			save_curpen();
			Pen_Rename();
			picknew();
			break;
		case 12:
			save_curpen();
			Pen_Kill();
			picknew();
			break;
		case 13:
			save_curpen();
			Pen_Merge();
			picknew();
			break;
		case 14:
			save_curpen();
			Pen_Expunge();
			picknew();
			break;
		}
			
	}
	
	Restore_Victim();

}

Pen_Rename()
{

	BOOL getans;
	PEN_A *getpen;

	if (Current_Pen==NULL) Current_Pen=Root_Pen;
	if (Current_Pen==NULL) return;

	getans = Pen_Sc(Current_Pen,"Select Pen to Rename",FALSE);

	if (!getans) return;
	if (scrollanswer==NULL) {
		oneliner("Pick a pen that already exists!");
		return;
	}
	getpen = scrollanswer;

	strcpy(ArgText,getpen->Pen_Name);
	BoxReq("Enter New Pen Name",ArgText);
	CleanOutArgText();
	
	if (ArgText[0]=='\0') return;


	Unlink_Pen(getpen);
	sprintf(getpen->Pen_Name,"%s",ArgText);
	getpen->pen_next = getpen->pen_prev = NULL;
	getpen->pen_next = getpen->pen_prev = NULL;
	Link_Pen(getpen);

	return;		
	
}

Pen_Merge()
{

	PEN_A	*getpen,*topen;
	BOOL	getans;
	struct drawing_node *lnklst;

	if (Current_Pen==NULL) Current_Pen=Root_Pen;
	if (Current_Pen==NULL) return;

	getans=Pen_Sc(Current_Pen,"Merge Pens",FALSE);

	if (!getans) return;
	if (scrollanswer==NULL) {
		oneliner("Pick a pen that already exists!");
		return;
	}
	getpen = scrollanswer;

	getans = Pen_Sc(NULL,"Merge Into Which Pen",FALSE);
	if (!getans) return;
	if (scrollanswer==NULL) {
		oneliner("Pick a pen that already exists!");
		return;
	}
	topen = scrollanswer;

	if (getpen==topen) return;	/* Nothing done */

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
		if (lnklst->pen_attach==getpen) 
			lnklst->pen_attach = topen;
		if (lnklst->node_flags&NODE_ONSCREEN) GenericRender(lnklst);
	}

	if (Current_Pen==getpen) Current_Pen = topen;
	if (Default_Pen==getpen) Default_Pen = topen;
	if (Dim_Box_Pen==getpen) Dim_Box_Pen = topen;
	if (Wit_Pen==getpen) Wit_Pen = topen;
	if (Dim_Line_Pen==getpen) Dim_Line_Pen = topen; 

	Unlink_Pen(getpen);	/* Get rid of old pen */

	InventoryAll(FALSE);	/* Get a new inventory count */

}

UBYTE PenWar[2][60];
struct IntuiText Pen_text[] = {
 { 2, 1, JAM2, 6, 3, NULL, "ABORT", NULL },
 { 2, 1, JAM2, 6, 3, NULL, "PROCEED", NULL },
 { 5, 1, JAM2, 6, 5, NULL, "Carnage Warning:", &Pen_text[3] },
 { 2, 1, JAM2, 6, 15, NULL, &PenWar[0][0], &Pen_text[4] },
 { 2, 1, JAM2, 6, 25, NULL, &PenWar[1][0], NULL },
};

Pen_Kill()
{

	PEN_A	*getpen;
	BOOL	getans;
	struct drawing_node *lnklst;

	if (Current_Pen==NULL) Current_Pen=Root_Pen;
	if (Current_Pen==NULL) return;

	getans=Pen_Sc(Current_Pen,"Pen to Kill",FALSE);

	if (!getans) return;
	if (scrollanswer==NULL) {
		oneliner("Pick a pen that already exists!");
	}
	getpen = scrollanswer;

	sprintf(&PenWar[0][0],"Killing Pen: %s",getpen->Pen_Name);
	sprintf(&PenWar[1][0],"Deleting %d Objects",getpen->Use_Count);

	if (complain("ABORT","PROCEED",5,2,2,2,
		"Carnage Warning:",
		&PenWar[0][0],
		&PenWar[1][0],NULL)) return;

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
		if (lnklst->pen_attach!=getpen) continue;
		if (lnklst->node_flags&NODE_ONSCREEN) {
			lnklst->node_flags &= (~NODE_SELECTED);
			lnklst->node_flags |= NODE_DELETED;
			GenericRender(lnklst);		/* Erase from screen */
		}
		DeleteNode(lnklst);
	}

	Unlink_Pen(getpen);	/* Get rid of old pen */

	if (Current_Pen==getpen) Current_Pen = Root_Pen;
	if (Default_Pen==getpen) Default_Pen = Root_Pen;
	if (Dim_Box_Pen==getpen) Dim_Box_Pen = Root_Pen;
	if (Wit_Pen==getpen) Wit_Pen = Root_Pen;
	if (Dim_Line_Pen==getpen) Dim_Line_Pen = Root_Pen; 

	InventoryAll(FALSE);	/* Get a new inventory count */

}

Pen_Expunge()
{

	int needexp;
	PEN_A *looper;

	InventoryAll(TRUE);	/* Find out whats in use */

	needexp = 0;
	for (looper=Root_Pen; looper!=NULL; looper=looper->pen_next) 
		if (looper->Use_Count<0) needexp++;

	if (needexp>0) {
		sprintf(&PenWar[0][0],"Expunging %d Unused Pens",needexp);
		sprintf(&PenWar[1][0]," ");

		if (!complain("ABORT","PROCEED",5,2,2,2,
			"Carnage Warning:",
			&PenWar[0][0],
			&PenWar[1][0],NULL)) {

			for (looper=Root_Pen; looper!=NULL; 
				looper=looper->pen_next) 
					if (looper->Use_Count<0)
						Unlink_Pen(looper);
		}
	}

	InventoryAll(FALSE);
}

