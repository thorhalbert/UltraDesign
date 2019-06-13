/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	scroller.c - Main scrolling requester
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
 *	2	13-Jun-87 SH	Add layer requesters
 *	3	28-Jun-87 SH	Layout tree requester code
 *	4	12-Sep-87 SH	Add Instant Response to prop slider
 *	5 	13-Sep-87 SH	Break attributes.c into little pieces
 *	6	 6-Dec-87 SH	Add color to bar 
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include "attributes.h"
#include <ctype.h>

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

BOOL match();
VOID PBoxInput(),MBoxInput(),TBoxInput(),SelHandle(),OKHandler(),SlideHandle();
VOID Link_Style();
VOID Link_Layer();

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

#define SCR_PARENT 0x0001	/* Entry accesses parent */
#define SCR_SUBDIR 0x0002	/* Subdirectory name block */
#define SCR_HEADER 0x0004	/* Header entry - not selectable */
#define SCR_SELENT 0x0010	/* Selectable entry */
struct RastPort *ScrRp;

LOCAL short Bord1[] = { 0,0, 500,0, 500,2, 0,2, 0,0  };
LOCAL short Bord2[] = 
	{ 16,40, 16,28, 497,28, 497,40, 16,40, 16,162, 497,162, 497,40 };
LOCAL struct Border BorderArray[] = {
 	{ 0,14, 0,0,JAM1, 5, &Bord1[0], &BorderArray[1] },
	{ 0,0,  0,0,JAM1, 8, &Bord2[0], NULL }};

LOCAL struct StringInfo PathStringSpec = 
	{ NULL, undobuf, 1, 100,0,0,0,0,0,0,NULL,0,NULL };
LOCAL struct Gadget PathBoxGadget = { NULL, 55, 18, 340, 9, GADGHCOMP, 
	RELVERIFY|GADGIMMEDIATE, STRGADGET|REQGADGET, NULL, NULL,
	NULL, 0, &PathStringSpec, 0, PBoxInput };

LOCAL struct StringInfo MaskStringSpec = 
	{ NULL, undobuf, 1, MASKMAX,0,0,0,0,0,0,NULL,0,NULL };
LOCAL struct Gadget MaskBoxGadget = { &PathBoxGadget, 300, 4, 180, 9, 
	GADGHCOMP, RELVERIFY|GADGIMMEDIATE, STRGADGET|REQGADGET, NULL, NULL,
	NULL, 0, &MaskStringSpec, 0, MBoxInput };

LOCAL struct StringInfo BoxStringSpec = 
	{ ArgText, undobuf, 1, 100,0,0,0,0,0,0,NULL,0,NULL };
LOCAL struct Gadget TextBoxGadget = { &MaskBoxGadget, 55, 170, 340, 9, 
	GADGHCOMP, RELVERIFY|GADGIMMEDIATE, STRGADGET|REQGADGET, NULL, NULL,
	NULL, 0, &BoxStringSpec, 0, TBoxInput };

LOCAL struct Requester ScrollerRequester;
LOCAL struct IntuiText TitleTexts[] = {
	{ 3,1,JAM2,5,4,NULL,NULL,	&TitleTexts[1] },
	{ 2,1,JAM2,18,30,NULL,NULL,	&TitleTexts[2] },
	{ 2,1,JAM2,5,18,NULL," Path:",	&TitleTexts[3] },
	{ 2,1,JAM2,5,170,NULL,"Entry:",	&TitleTexts[4] },
	{ 2,1,JAM2,255,4,NULL,"Mask:",	NULL },
	{ 2,1,JAM2,55,18,NULL, NULL, NULL }};		/* Path display */

LOCAL APTR accessdata[13],oldaccess[13];
LOCAL WORD accessflags[13];
LOCAL struct IntuiText OKListText[2] = {
	{ 2,3,JAM2,0,0,NULL,NULL,NULL },
	{ 2,3,JAM2,0,0,NULL,NULL,NULL }};
#define OKACTIVE GADGIMMEDIATE | RELVERIFY
LOCAL struct Gadget OKList[] = {
	{ &OKList[1], 320,185,80,8,GADGHCOMP,OKACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&OKListText[0],0,NULL,20,OKHandler},
	{ &TextBoxGadget, 100,185,80,8,GADGHCOMP,OKACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&OKListText[1],0,NULL,21,OKHandler}};

LOCAL struct Image SlideImage;

LOCAL struct PropInfo	Slider =
{ AUTOKNOB | FREEVERT,0,0,0xFFFF, 0xFFFF,0,0,0,0,0,0};

LOCAL struct Gadget	SlideProp =
{ &OKList[0],0,40,16,122,GADGIMAGE,RELVERIFY|GADGIMMEDIATE,PROPGADGET,
	(APTR) &SlideImage,NULL,NULL,0,(APTR) &Slider,0,SlideHandle }; 

#define DFL 58
LOCAL UBYTE DataFields[12][DFL+1];
LOCAL WORD FieldColor[12];
LOCAL struct IntuiText ScrollList[] = {
	{ 0,1,JAM2,0,0,NULL,&DataFields[0][0],NULL },
	{ 0,1,JAM2,0,0,NULL,&DataFields[1][0],NULL },
	{ 0,1,JAM2,0,0,NULL,&DataFields[2][0],NULL },
	{ 0,1,JAM2,0,0,NULL,&DataFields[3][0],NULL },
	{ 0,1,JAM2,0,0,NULL,&DataFields[4][0],NULL },
	{ 0,1,JAM2,0,0,NULL,&DataFields[5][0],NULL },
	{ 0,1,JAM2,0,0,NULL,&DataFields[6][0],NULL },
	{ 0,1,JAM2,0,0,NULL,&DataFields[7][0],NULL },
	{ 0,1,JAM2,0,0,NULL,&DataFields[8][0],NULL },
	{ 0,1,JAM2,0,0,NULL,&DataFields[9][0],NULL },
	{ 0,1,JAM2,0,0,NULL,&DataFields[10][0],NULL },
	{ 0,1,JAM2,0,0,NULL,&DataFields[11][0],NULL },
	{ 0,1,JAM2,0,0,NULL,"",NULL };
#define GBASE 42
#define GADACTIVE GADGIMMEDIATE | RELVERIFY
#define GWID 462
#define GLF 30
LOCAL struct Gadget GadList[12] = {
	{ &GadList[1],GLF,GBASE,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[12],0,NULL,20,SelHandle},
	{ &GadList[2],GLF,GBASE+10,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[12],0,NULL,21,SelHandle},
	{ &GadList[3],GLF,GBASE+20,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[12],0,NULL,22,SelHandle},
	{ &GadList[4],GLF,GBASE+30,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[12],0,NULL,23,SelHandle},
	{ &GadList[5],GLF,GBASE+40,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[12],0,NULL,24,SelHandle},
	{ &GadList[6],GLF,GBASE+50,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[12],0,NULL,25,SelHandle},
	{ &GadList[7],GLF,GBASE+60,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[12],0,NULL,26,SelHandle},
	{ &GadList[8],GLF,GBASE+70,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[12],0,NULL,27,SelHandle},
	{ &GadList[9],GLF,GBASE+80,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[12],0,NULL,28,SelHandle},
	{ &GadList[10],GLF,GBASE+90,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[12],0,NULL,29,SelHandle},
	{ &GadList[11],GLF,GBASE+100,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[12],0,NULL,30,SelHandle},
	{ &SlideProp,GLF,GBASE+110,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[12],0,NULL,31,SelHandle}};

/*-------------------------------Code Section--------------------------------*/

LOCAL VOID (*get_formname)(),(*get_formdata)();
LOCAL APTR 	(*get_retfirst)(),(*get_retnext)(),(*get_retprev)(),
		(*get_srchrec)(),(*get_getparent)(),(*get_godescend)();
APTR cur_entry;
LOCAL ULONG KepSecs,KepMics;
LOCAL int KepButton;
LOCAL BOOL scrollokcancel;

VOID StgSet(instr)
struct StringInfo *instr;
{

	Forbid();
	instr->DispPos = 0;
	instr->NumChars = strlen(instr->Buffer);
	Permit();
}

advstr()
{
	int a,b;

	for(a=0; a<11; a++) {
		accessdata[a]=accessdata[a+1];
		accessflags[a]=accessflags[a+1];
		ScrollList[a].FrontPen = ScrollList[a+1].FrontPen;
		ScrollList[a].BackPen = ScrollList[a+1].BackPen;
		FieldColor[a] = FieldColor[a+1];
		/* sprintf(&DataFields[a][0],"%s",&DataFields[a+1][0]); */
	}
	GadList[11].UserData=SelHandle;
	GadList[11].GadgetID=20+11;
	accessdata[11]=NULL;
	accessflags[11]=NULL;
	DataFields[11][0]='\0';
	ScrollList[11].FrontPen=0;
	ScrollList[11].BackPen=1;
	FieldColor[11] = -1;

}

UpdatePaint(sp)
int sp;
{

	APTR begptr,othptr,tptr,tmpaccess[13];
	int a,b,dlin,plin,tbody;
	STDFLT r;
	BOOL scrollok;

	/* Clear out the current display */

	for (a=0; a<12; a++) {
		GadList[a].UserData=SelHandle;
		GadList[a].GadgetID=20+a;
		accessdata[a]=NULL;
		accessflags[a]=NULL;
		DataFields[a][0]='\0';
		ScrollList[a].FrontPen=0;
		ScrollList[a].BackPen=1;
		FieldColor[a] = -1;
	}

	/* Find the root of the list */

	if (cur_entry==NULL) 
		cur_entry=(*get_retfirst)(NULL,MaskStringSpec.Buffer,FALSE);	

	/* Count up the number of entries in the table for scroller body */

	tbody = 0;
	
	begptr = (*get_retfirst)(NULL,MaskStringSpec.Buffer,FALSE);	
	
	for (othptr=begptr; othptr!=NULL; 
		   othptr=(*get_retnext)(othptr,MaskStringSpec.Buffer)) tbody++;

	/* See if there is a parent to see if we need a /Parent entry */

	tptr=(*get_getparent)(begptr,MaskStringSpec.Buffer,FALSE);
	if (tptr!=NULL) tbody ++;

	/* Try to position sp where the knob is on the prop gadget */

	if (sp>=0) {
		a = tbody - 12;
		if (a<1) a=1;
		sp = (((Slider.VertPot+1) * a) >> 16) + 12;
		if (sp==tbody-1) sp=tbody;
		if (sp>tbody) sp = tbody;
		if (sp<12) sp=12;
	}

	/* Put the window at sp */

	plin = 9999;		/* Count down timer within list */
	dlin = 0;		/* Count up within list */
	a = 0;			/* Count up within window */
	
	/* Put the /Parent on the top if there is one */
	
	tptr=(*get_getparent)(begptr,MaskStringSpec.Buffer,FALSE);
	if (tptr!=NULL) {
		accessdata[a]=tptr;
		accessflags[a]|=SCR_PARENT;
		ScrollList[a].FrontPen=3;
		dlin++;
		plin--;
		a++;
	}
	
	/* Loop through the items and stop when position ideal */
	
	for (othptr=begptr; othptr!=NULL; 
		   othptr=(*get_retnext)(othptr,MaskStringSpec.Buffer)) {

		if (a>=12) {		/* If we loop off bottom, scroll */
			advstr();
			a = 11;
		}

		if (sp<0&&othptr==cur_entry) plin = 5; 
					/* Put the current item in the middle
					   of the window */

		accessdata[a]=othptr;
		
		/* See if this is a directory */
		
		if ((*get_godescend)
			(othptr,MaskStringSpec.Buffer,FALSE)!=NULL) {
			ScrollList[a].FrontPen=3;
			accessflags[a]|=SCR_SUBDIR;
		} else 	accessflags[a]|=SCR_SELENT;
		dlin++;
		plin--;
		a++;
		
		/* Have we gone past the scroller -- if so, stop */
		
		if (sp>=0&&dlin>=sp) break;
		
		/* If we have counted down to zero or counted up to 12, stop */
		
		if (plin<0&&dlin>=12) break;
		
		/* Continue */
		
	}

	/* Calculate a new pot position */
	
	if (sp<0) {

		a = dlin - 12;
		if (a<0) a=0;
		b = tbody - 12;
		if (b<1) b=1;

		r = ((STDFLT) a) / ((STDFLT) b);
		r *= ((STDFLT) MAXPOT);

		a = r;
		if (dlin>=tbody) a = MAXPOT;
		if (a>MAXPOT) a=MAXPOT;
		if (a<0) a=0;

		if (dlin>12) dlin=12;
		if (dlin>tbody) dlin=tbody;
		if (tbody<12) tbody=dlin = 1;

		r = ((STDFLT) dlin) / ((STDFLT) tbody);
		r *= ((STDFLT) MAXBODY);

		b = r;
		if (b>MAXBODY) b = MAXBODY;
		if (b<0) b=0;

		ModifyProp(&SlideProp,w,&ScrollerRequester,
			AUTOKNOB | FREEVERT, 0, a, 0, b);
			
	}

	/* Try to scroll display up */
	
	scrollok = TRUE;
	for (a=1; a<12; a++) 
		if (accessdata[0]==oldaccess[a]) {
			if (ScrRp) {
				SetBPen(ScrRp, 1);	/* Fill hole with grey */
				ScrollRaster(ScrRp,0,(a*10),GLF-13,GBASE,
						    GLF+GWID,GBASE+(12*10)-1);
			}
			scrollok = FALSE;
			for (b=a; b<12; b++) oldaccess[b-a] = oldaccess[b];
			for (b=12-a; b<12; b++) oldaccess[b] = NULL;
			break;
		}
	
	
	/* Try to scroll down */
	
	if (scrollok) {
		for (a=1; a<12; a++) 
		   if (accessdata[a]==oldaccess[0]) {
		   	if (ScrRp) {
			       SetBPen(ScrRp, 1);
			       ScrollRaster(ScrRp,0,-(a*10),GLF-13,GBASE,
		       				      GLF+GWID,GBASE+(12*10)-1);
			}
		        for (b=11; b>=a; b--) oldaccess[b] = oldaccess[b-a];
		        for (b=0; b<a; b++) oldaccess[b] = NULL;
		        break;
		   }
	} 

	for (a=0; a<12; a++) {
		if (oldaccess[a]==accessdata[a]) continue;
		if (ScrRp) {
			SetAPen(ScrRp, 1);
			SetOPen(ScrRp, 1);
	
			RectFill(ScrRp, GLF-13,GBASE+(a*10),
					GLF+GWID,GBASE+9+(a*10));
		}
		
		if (accessdata[a]==NULL) continue;

		Scr_Color = -1;
		(*get_formdata)(accessdata[a],&DataFields[a][0]);
		FieldColor[a] = Scr_Color;
		if (ScrRp) {
			SetAPen(ScrRp, 0);
			SetBPen(ScrRp, 1);
			PrintIText(ScrRp, &ScrollList[a], GLF, GBASE+(a*10));
		}
		if (FieldColor[a]<0) continue;

		if (ScrRp) {

			SetAPen(ScrRp, FieldColor[a]);
			SetOPen(ScrRp, 0);		/* Outline in black */
	
			RectFill(ScrRp, GLF-13,GBASE+(a*10),
					GLF-3,GBASE+6+(a*10));
		}
	}
	
	for (a=0; a<12; a++) oldaccess[a]=accessdata[a];
	if (ScrRp)	PrintIText(ScrRp,&TitleTexts[0],0,0);

}

VOID SelHandle(ingad)
struct Gadget *ingad;
{

	APTR ptrnam;
	int gadnum;

	if (ingad==NULL) return;

	gadnum = ingad->GadgetID-20;

	if (gadnum<0||gadnum>11) return;
	ptrnam = accessdata[gadnum];
	if (ptrnam==NULL) return;
	scrollanswer = ptrnam;
	if (accessflags[gadnum]&SCR_PARENT) {
		(*get_getparent)(ptrnam,MaskStringSpec.Buffer,TRUE);
		cur_entry = NULL;
		ArgText[0]='\0';			/* Blank out */
		StgSet(&BoxStringSpec);
		UpdatePaint(-1);
		return;
	}
	if (accessflags[gadnum]&SCR_SUBDIR) {
		(*get_godescend)(ptrnam,MaskStringSpec.Buffer,TRUE);
		cur_entry = NULL;
		ArgText[0]='\0';			/* Blank out */
		StgSet(&BoxStringSpec);
		UpdatePaint(-1);
		return;
	}
	(*get_formname)(ptrnam,ArgText);
	StgSet(&BoxStringSpec);
	RefreshGList(ingad,w,&ScrollerRequester,1);
	RefreshGList(&TextBoxGadget,w,&ScrollerRequester,1);
	if (ScrRp) PrintIText(ScrRp,&TitleTexts[0],0,0);
	if (KepButton==gadnum) {
		if (DoubleClick(KepSecs,KepMics,StoSecs,StoMics)) {
			CloseRequest(&ScrollerRequester,w);
			scrollokcancel = TRUE;
		}
	} 
	KepButton = gadnum;
	KepSecs = StoSecs;
	KepMics = StoMics;

}
	
VOID TBoxInput(ingad)
struct Gadget *ingad;
{

	return;

}

VOID SlideHandle(ingad)
struct Gadget *ingad;
{
	UpdatePaint(1);
	return;

}

VOID MBoxInput(ingad)
struct Gadget *ingad;
{

	if (ingad==NULL) return;

	cur_entry = NULL;
	ArgText[0]='\0';			/* Blank out */
	StgSet(&BoxStringSpec);
	UpdatePaint(-1);
	return;

}

VOID PBoxInput(ingad)
struct Gadget *ingad;
{

	if (ingad==NULL) return;

	(*get_retfirst)(NULL,MaskStringSpec.Buffer,TRUE);
	cur_entry = NULL;
	ArgText[0]='\0';			/* Blank out */
	StgSet(&BoxStringSpec);
	UpdatePaint(-1);
	return;

}

	
VOID OKHandler(ingad)
struct Gadget *ingad;
{

	if (ingad==NULL) return;

	CloseRequest(&ScrollerRequester,w);	/* Do manually */
	
	switch (ingad->GadgetID) {
	case 20:
		scrollokcancel = FALSE;
		break;
	case 21:
		scrollokcancel = TRUE;
		break;
	}

	return;

}

UBYTE _c_data[60];

UBYTE *CleanAttribs(indata)
UBYTE *indata;
{

	UBYTE *d;
	
	for (d = _c_data; *indata!='\0'; indata++) {
	
		if (*indata==NULL) break;
		if (*indata>' ') 
	
			switch (*indata) {
			case '*':
			case '?':
			case '/':
			case ':':
			case '\\':
				break;
			default:
				*d++ = *indata;
			}
		
	}
	
	*d++ = '\0';
	
	return(_c_data);
}

VOID CleanOutArgText()		/* Need to mod for extended ascii */
{

	UBYTE *inpass,*outpass;

	inpass = ArgText;
	outpass = ArgText;

	for (; *inpass!='\0'; inpass++) {
		if (iscntrl(*inpass)||isspace(*inpass)) continue;
		if (*inpass=='/'||*inpass=='\\'||
		    *inpass=='*'||*inpass=='?'||*inpass==':') continue;
		*outpass++=*inpass;
	}

	*outpass='\0';

	StgSet(&BoxStringSpec);
}

BOOL boxoff=FALSE;

BOOL Scroller(retfirst,retnext,retprev,srchrec,getparent,godescend,formdata,
	formname,pathflag,maskflag,mustbenew,defaultitem,affirm,cancel,
	title,header,startp,inmask,inpath)
APTR (*retfirst)();	/* Routine to get root of current list */
APTR (*retnext)();	/* Return successor item */
APTR (*retprev)();	/* Return predecessor item */
APTR (*srchrec)();	/* Search table for given key */
APTR (*getparent)();	/* Get parent node for any level */
APTR (*godescend)();	/* If a branch, go to top of descendant node */
VOID (*formdata)();	/* Convert a given pointer to display format */
VOID (*formname)();	/* Get the name part only */
BOOL pathflag;		/* Can the path be changed */
BOOL maskflag;		/* Is a mask allowed? */
BOOL mustbenew;		/* Old items cannot be accessed */
APTR defaultitem;	/* Begin item */
UBYTE *affirm;		/* Affirmation icon string */
UBYTE *cancel;		/* Negative icon string */
UBYTE *title;		/* Title bar information */
UBYTE *header;		/* Header bar information */
APTR *startp;		/* Context location */
UBYTE *inmask;		/* Incoming mask */
UBYTE *inpath;		/* Incoming path spec */
{

	int a,b;
	struct RastPort *reqrp;

	get_retfirst = retfirst;
	get_retnext = retnext;
	get_retprev = retprev;
	get_srchrec = srchrec;
	get_getparent = getparent;
	get_godescend = godescend;
	get_formdata = formdata;
	get_formname = formname;

	InitRequester(&ScrollerRequester);

	for (a=0; a<12; a++) {
		GadList[a].UserData=SelHandle;
		GadList[a].GadgetID=20+a;
		oldaccess[a]=accessdata[a]=NULL;
		accessflags[a]=NULL;
		for (b=0; b<DFL; b++) DataFields[a][b]=' ';
		DataFields[a][DFL]='\0';
		ScrollList[a].FrontPen=0;
		ScrollList[a].BackPen=1;
	}

	ScrollerRequester.Width=500;
	ScrollerRequester.Height=200;
	ScrollerRequester.LeftEdge=(XSsize-ScrollerRequester.Width)/2;
	ScrollerRequester.TopEdge=(YSsize-ScrollerRequester.Height)/2;

	ScrollerRequester.BackFill=1;

	ScrollerRequester.ReqGadget = &GadList[0];
	ScrollerRequester.ReqBorder = &BorderArray[0];
	TitleTexts[0].IText = title;
	TitleTexts[1].IText = header;
	ScrollerRequester.ReqText = &TitleTexts[0];

	MaskStringSpec.Buffer = inmask;
	StgSet(&MaskStringSpec);
	PathStringSpec.Buffer = inpath;
	StgSet(&PathStringSpec);

	MaskBoxGadget.NextGadget = &PathBoxGadget;
	TitleTexts[4].NextText = NULL;

	if (!pathflag) {
		MaskBoxGadget.NextGadget = NULL;
		TitleTexts[4].NextText = &TitleTexts[5];
		TitleTexts[5].IText = inpath;
	}

	OKListText[0].IText = cancel;
	OKListText[1].IText = affirm;

	cur_entry = *startp;
	if (cur_entry==NULL) cur_entry=(*get_retfirst)(NULL,inmask,FALSE);	

	scrollanswer = NULL;

	ArgText[0]='\0';			/* Blank out */
	if (defaultitem!=NULL) {		/* Set to default, if any */
		/* (*get_formname)(defaultitem,ArgText);
		scrollanswer = defaultitem; */
	}

	StgSet(&BoxStringSpec);

	if (boxoff) TextBoxGadget.Flags |= GADGDISABLED;
	else	    TextBoxGadget.Flags &= ~GADGDISABLED;

#ifdef _TURNSY
	ScrollerRequester.Flags = NOISYREQ;
#endif

	SafeRequest(&ScrollerRequester, w);

	ScrRp = ScrollerRequester.ReqLayer->rp;	/* Nasty */

	SetAPen(ScrRp, 2);
	Move(ScrRp, 0, 0);
	Draw(ScrRp, (ScrollerRequester.Width-1), 0);
	Draw(ScrRp, (ScrollerRequester.Width-1), 
		ScrollerRequester.Height-1);
	Draw(ScrRp, 0, ScrollerRequester.Height-1);
	Draw(ScrRp, 0, 0);

	UpdatePaint(-1);	
	
	if (!boxoff) for (a=0; a<4; a++) {
		if (ActivateGadget(&TextBoxGadget,w,&ScrollerRequester)) 
			break;
		Delay(2);
	}
	
	sit_n_spin(&ScrollerRequester,1,&OKList[0],&OKList[1]); 
		/* Wait for requesters to exit */

	if (cur_entry!=NULL) *startp = cur_entry;

	/* CleanOutArgText(); */
	scrollanswer = (*get_srchrec)(ArgText);
	if (scrollanswer!=NULL) *startp = scrollanswer;

	boxoff = FALSE;
	ScrRp = NULL;

	/* if (ArgText[0]=='\0') return(FALSE); */

	if (mustbenew&&scrollanswer!=NULL) {
		oneliner("Entry cannot exist already!");
		return(FALSE);
	}

	return(scrollokcancel);

}
