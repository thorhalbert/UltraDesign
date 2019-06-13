/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	drivreq.c - driver parameter editing
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
 *	5	13-Sep-87 SH	Break attributes.c into pieces
 *	6	15-Sep-87 SH	Make stylereq into marginreq
 *	7	27-Oct-87 SH	make marginreq into drivreq for drivers
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "plotsetup.h"
#include <graphics/gfxmacros.h>

/*----------------------------------Macros-----------------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*---------------------Lettering Style Edit Requester---------------------*/

VOID togserv(),gridserv();
VOID Set_Current();
STDFLT ParLin();
STDFLT ParAng();
UBYTE *DispLin(/* STDFLT */);
UBYTE *DispAng(/* STDFLT */);

extern VOID (*mouseaction)();

extern struct Image top1_image,top1on_image;
extern struct Image top2_image,top2on_image;
extern struct Image top3_image,top3on_image;
extern struct Image bot1_image,bot1on_image;
extern struct Image bot2_image,bot2on_image;
extern struct Image grid1_image,grid1on_image;
extern struct Image grid2_image,grid2on_image;
extern struct Image grid3_image,grid3on_image;
extern struct Image grid4_image,grid4on_image;
extern struct Image grid5_image,grid5on_image;
extern struct Image grid6_image,grid6on_image;
extern struct Image grid7_image,grid7on_image;
extern struct Image grid8_image,grid8on_image;
extern struct Image grid9_image,grid9on_image;
extern struct Image sel1_image,sel1on_image;
extern struct Image sel2_image,sel2on_image;
extern struct Image sel3_image,sel3on_image;
extern struct Image sel4_image,sel4on_image;

#define grpgad(ngad,lef,top,wid,heig,pic1,pic2,gadid,tgs,serv) \
 { ngad,lef,top,wid,heig,GADGHIMAGE|GADGIMAGE, \
   GADGIMMEDIATE|RELVERIFY|tgs,BOOLGADGET, \
   (APTR) &pic1,(APTR) &pic2, NULL,0,NULL,gadid,(APTR) serv },

#define BSL 449
#define BST 166
#define TG TOGGLESELECT
#define SWT togserv
#define BUT gridserv

#define FR 25
#define LPS 115

#define FRG3 162
#define FRG4 162+9
#define FRG1 162+18
#define FRG2 162+27

#define LPSG 90
#define BSTRT LPSG+214+8

#define LCL1 90
#define LCL2 185
#define LCL3 355

struct Gadget BList[] = {
grpgad(&BList[ 1],BSL+  0,BST+  0,38,17,top1_image,top1on_image,101,TG,SWT)
grpgad(&BList[ 2],BSL+ 40,BST+  0,38,17,top2_image,top2on_image,102,TG,SWT)
grpgad(&BList[ 3],BSL+ 80,BST+  0,38,17,top3_image,top3on_image,103,TG,SWT)
grpgad(&BList[ 5],BSL+ 40,BST+ 18,38,13,bot1_image,bot1on_image,104,0,SWT)
grpgad(&BList[ 5],BSL+ 40,BST+ 18,80,13,bot2_image,bot2on_image,105,TG,SWT)
grpgad(&BList[ 6],BSL+120,BST+  0,23,11,grid1_image,grid1on_image,111,0,BUT)
grpgad(&BList[ 7],BSL+141,BST+  0,23,11,grid2_image,grid2on_image,112,0,BUT)
grpgad(&BList[ 8],BSL+162,BST+  0,23,11,grid3_image,grid3on_image,113,0,BUT)
grpgad(&BList[ 9],BSL+120,BST+ 10,23,11,grid4_image,grid4on_image,114,0,BUT)
grpgad(&BList[10],BSL+141,BST+ 10,23,11,grid5_image,grid5on_image,115,0,BUT)
grpgad(&BList[11],BSL+162,BST+ 10,23,11,grid6_image,grid6on_image,116,0,BUT)
grpgad(&BList[12],BSL+120,BST+ 20,23,11,grid7_image,grid7on_image,117,0,BUT)
grpgad(&BList[13],BSL+141,BST+ 20,23,11,grid8_image,grid8on_image,118,0,BUT)
grpgad(&BList[14],BSL+162,BST+ 20,23,11,grid9_image,grid9on_image,119,0,BUT)
grpgad(&BList[15],BSTRT,FRG4,15, 8,sel1_image,sel1on_image,130,0,BUT)
grpgad(&BList[16],BSTRT+17,FRG4,15, 8,sel2_image,sel2on_image,131,0,BUT)
grpgad(&BList[17],BSTRT+34,FRG4,31, 8,sel3_image,sel3on_image,132,0,BUT)
grpgad(NULL,BSTRT+67,FRG4,44, 8,sel4_image,sel4on_image,133,0,BUT)
};

VOID MAStringHandler(),MAHandler();

LOCAL UBYTE MAData[6][32];
LOCAL UBYTE Fname[255],Tname[40];

LOCAL struct StringInfo MAStringSpec[] = {
	{ &MAData[0][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[1][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[2][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[3][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ Fname,          undobuf, 0, 240,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[4][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[5][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL }};

#define GDST 	88,8,GADGHCOMP,RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, \
		STRGADGET,NULL, NULL, NULL, 0
#define SDST 	40,8,GADGHCOMP,RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, \
		STRGADGET,NULL, NULL, NULL, 0
struct Gadget MAGadgets[] = {
#undef GADUN
#define GADUN 0
	{ &MAGadgets[GADUN+1],LCL2,FRG1,GDST,
		(APTR) &MAStringSpec[GADUN],30+GADUN,(APTR) MAStringHandler  },
#undef GADUN
#define GADUN 1
	{ &MAGadgets[GADUN+1],LCL2,FRG2,GDST,
		(APTR) &MAStringSpec[GADUN],30+GADUN,(APTR) MAStringHandler  },
#undef GADUN
#define GADUN 2
	{ &MAGadgets[GADUN+1],LCL3,FRG1,GDST,
		(APTR) &MAStringSpec[GADUN],30+GADUN,(APTR) MAStringHandler  },
#undef GADUN
#define GADUN 3
	{ &MAGadgets[GADUN+1],LCL3,FRG2,GDST,
		(APTR) &MAStringSpec[GADUN],30+GADUN,(APTR) MAStringHandler  },
#undef GADUN
#define GADUN 4
	{ &MAGadgets[GADUN+1],LPSG,FRG3,258,8,GADGHCOMP,
		RELVERIFY|GADGIMMEDIATE|STRINGCENTER,
		STRGADGET,NULL, NULL, NULL, 0,
		(APTR) &MAStringSpec[GADUN],30+GADUN,(APTR) MAStringHandler  },
#undef GADUN
#define GADUN 5
	{ &MAGadgets[GADUN+1],LCL1,FRG1,SDST,
		(APTR) &MAStringSpec[GADUN],30+GADUN,(APTR) MAStringHandler  },
#undef GADUN
#define GADUN 6
	{ &BList[0],LCL1,FRG2,SDST,
		(APTR) &MAStringSpec[GADUN],30+GADUN,(APTR) MAStringHandler  },
};

#define BC 2,3,JAM2
#define BT 1,3,JAM2
struct IntuiText MAHeadings[] = {
	{ BC,LCL2-58,FRG1,NULL," X Org:",&MAHeadings[1] },
	{ BC,LCL2-58,FRG2,NULL," Y Org:",&MAHeadings[2] },
	{ BC,LCL3-82,FRG1,NULL," Rotation:",&MAHeadings[3] },
	{ BC,LCL3-82,FRG2,NULL,"    Scale:",&MAHeadings[4] },
	{ BC,LPSG-82,FRG3,NULL,"Plot File:",&MAHeadings[5] },
	{ BC,LCL1-82,FRG1,NULL,       " Poster X:",&MAHeadings[6] },
	{ BC,LCL1-82,FRG2,NULL,       " Poster Y:",&MAHeadings[7] },
	{ BT,LPSG-82,FRG4,NULL,   "  Drawing:",NULL }};


STDFLT minnum = -MAXBIG;
STDFLT maxnum = MAXBIG;
STDFLT zernum = 0.0;
STDFLT onenum = 1.0;
STDFLT divmax = 64.0;
STDFLT txtmax = 5;	

#define MQ 16
#define XQ 32
#define LNG 0
#define LIN 1
#define ANG 2
#define STR 3
#define WRD 4
#define SCI 5
#define NUM 6
#define TYPMSK 0x07

struct _reqtable {
	UBYTE	varflg;
	UBYTE	*varptr;	
	STDFLT	*minval;
	STDFLT	*maxval;
	struct	Gadget	*reqgad;
	union {
		STDFLT	savflt;
		LONG	savlng;
	} savvars;
} reqtab[] = {
{ LIN, 		(UBYTE *) NULL, &minnum, &maxnum, &MAGadgets[0] },
{ LIN, 		(UBYTE *) NULL, &minnum, &maxnum, &MAGadgets[1] },
{ ANG, 		(UBYTE *) NULL, &minnum, &maxnum, &MAGadgets[2] },
{ NUM, 		(UBYTE *) NULL, &minnum, &maxnum, &MAGadgets[3] },
{ STR, 		(UBYTE *) NULL, &minnum, &maxnum, &MAGadgets[4] },
{ WRD, 		(UBYTE *) NULL, &onenum, &maxnum, &MAGadgets[5] },
{ WRD, 		(UBYTE *) NULL, &onenum, &maxnum, &MAGadgets[6] },
};

#define TOTREQS 7

LOCAL BOOL exflag;
LOCAL int postaction=0;

UBYTE TempBuf[20];

UBYTE *FormMarReq(innum)
int innum;
{
	LONG	*wp;
	WORD	*op;
	STDFLT	*fp;
	UBYTE	*an;

	struct _reqtable *mpr;
	mpr = &reqtab[innum];

	if (mpr->varptr==NULL) return("**");
	switch (mpr->varflg&TYPMSK) {
	case LNG:
		wp = (LONG *) mpr->varptr;
		sprintf(TempBuf,"%d",(int) *wp);
		return(TempBuf);
	case WRD:
		op = (WORD *) mpr->varptr;
		sprintf(TempBuf,"%d",(int) *op);
		return(TempBuf);
	case NUM:
		fp = (STDFLT *) mpr->varptr;
		/* if ((*fp<.01)||(*fp>1000.0))  */
			sprintf(TempBuf,"%.4g",*fp);
		/* else	sprintf(TempBuf,"%.4f",*fp); */
		return(TempBuf);
	case SCI:
		fp = (STDFLT *) mpr->varptr;
		scinot = TRUE;
		an=DispLin(*fp);
		scinot=FALSE;
		return(an);		
	case LIN:
		fp = (STDFLT *) mpr->varptr;
		return(DispLin(*fp));
	case ANG:
		fp = (STDFLT *) mpr->varptr;
		return(DispAng(*fp));
	case STR:
		return(mpr->varptr);
	}
}

VOID MarBeep()
{

	int a;

	for (a=0; a<TOTREQS; a++) {

		strcpy(MAStringSpec[a].Buffer,FormMarReq(a));
		StgSet(&MAStringSpec[a]);
	}

	DisplayBeep(screen);

	RefreshGadgets(&MAGadgets[0],w,NULL);
	return;
}

STDFLT orgx,orgy;

VOID MAStringHandler(ingad)
struct Gadget *ingad;
{

	int button,a;
	STDFLT inquery,*fp;
	LONG	inlong;
	LONG	*wp;
	WORD	*op;
	UBYTE	*ob;
	BOOL	inchk;
	struct _reqtable *inbit;

	Forbid();
	ingad->Flags &= ~SELECTED;	/* Make sure it is unselected */
	Permit();

	button = ingad->GadgetID-30;

	inbit = &reqtab[button];
	
	switch (inbit->varflg&TYPMSK) {
	case STR:
		inchk = FALSE;
		break;
	case LNG:
		inchk = FALSE;
		if (sscanf(MAStringSpec[button].Buffer,"%d",&inlong)!=1) 
			inchk=TRUE;
		if (!inchk) inquery = (STDFLT) inlong;
		break;
	case NUM:
		inchk = FALSE;
		if (sscanf(MAStringSpec[button].Buffer,"%f",&inquery)!=1) 
			inchk=TRUE;
		break;
	case WRD:
		inchk = FALSE;
		if (sscanf(MAStringSpec[button].Buffer,"%d",&inlong)!=1) 
			inchk=TRUE;
		if (!inchk) inquery = (STDFLT) inlong;
		break;
	case SCI:
	case LIN:
		inquery = ParLin(MAStringSpec[button].Buffer,&inchk);
		if (inchk) break;
		break;
	case ANG:
		inquery = ParAng(MAStringSpec[button].Buffer,&inchk);
		break;
	}

	if (inbit->varflg&MQ) {
		if (inquery<=*(inbit->minval)) inchk=TRUE;
	}  else if (inquery<*(inbit->minval)) inchk=TRUE;

	if (inbit->varflg&XQ) {
		if (inquery>=*(inbit->maxval)) inchk=TRUE;
	}  else if (inquery>*(inbit->maxval)) inchk=TRUE;

	if (inchk) {
		MarBeep();
		return;
	}

	if (inbit->varptr!=NULL) {
		switch (inbit->varflg&TYPMSK) {
		case LNG:
			wp = (LONG *) inbit->varptr;
			*wp = inlong;
			break;
		case WRD:
			op = (WORD *) inbit->varptr;
			*op = inlong;
			break;
		case NUM:
		case SCI:
		case ANG:
		case LIN:
			fp = (STDFLT *) inbit->varptr;
			*fp = inquery;
			break;
		case STR:
			ob = (UBYTE *) inbit->varptr;
			if (button==4) strcpy(ob,MAStringSpec[button].Buffer);
		}
	}

	strcpy(MAStringSpec[button].Buffer,FormMarReq(button));
	StgSet(&MAStringSpec[button]);

	switch (button) {
	case 0:
		Current_View->XNewOrigin = orgx + SheetMinIX;
		break;
	case 1:
		Current_View->YNewOrigin = orgy + SheetMinIY;
		break;
	}
			

	Set_Current();
		
	return;

}

current_disp()
{

	int a;
	UBYTE	obuf[255];

	if (Current_View==NULL) {
		for (a=0; a<4; a++)
			MAGadgets[a].Flags |= GADGDISABLED;

		for (a=0; a<17; a++)
			BList[a].Flags |= GADGDISABLED;

		sprintf(obuf,"  << Load in a file >>");

		reqtab[0].varptr = NULL;
		reqtab[1].varptr = NULL;
		reqtab[2].varptr = NULL;
		reqtab[3].varptr = NULL;

	} else {

		for (a=0; a<4; a++) MAGadgets[a].Flags &= ~GADGDISABLED;

		for (a=0; a<17; a++) BList[a].Flags &= ~GADGDISABLED;

		strcpy(obuf,Current_View->ViewName);

		orgx = Current_View->XNewOrigin - SheetMinIX;
		orgy = Current_View->YNewOrigin - SheetMinIY;
		
		reqtab[0].varptr = (UBYTE *) &orgx;
		reqtab[1].varptr = (UBYTE *) &orgy;
		reqtab[2].varptr = (UBYTE *) &Current_View->ViewRot;
		reqtab[3].varptr = (UBYTE *) &Current_View->ViewScale;
	}

	SetDrMd(rp,JAM2);
	SetAPen(rp,0);
	SetBPen(rp,1);

	Move(rp,LPSG,FRG4+6);

	for (a=strlen(obuf); a<26; a++) obuf[a]=' ';
	obuf[26] = '\0';

	Text(rp,obuf,strlen(obuf));

	Update_Disp();
}

VOID undo_driv()
{

	int a;

	for (a=0; a<TOTREQS; a++) reqtab[a].varptr=NULL;
}

VOID Set_Current()
{

	int a;

	MinDX = SheetMinIX;
	MinDY = SheetMinIY;
	MaxDX = SheetMaxIX;
	MaxDY = SheetMaxIY;

	current_disp();

	for (a=0; a<TOTREQS; a++) {
		if (reqtab[a].varptr==NULL) 
			MAGadgets[a].Flags |= GADGDISABLED;
		else	MAGadgets[a].Flags &= ~GADGDISABLED;

		strcpy(MAStringSpec[a].Buffer,FormMarReq(a));
		StgSet(&MAStringSpec[a]);

	}

	if (DriverSegHook!=0) {
		sprintf(Tname,"PRT:");	/* Special case for preferences */

		if (mainbase.MPI->PlotFlags&PLFLAG_PRINTER) {
			MAStringSpec[4].Buffer = Tname;
			MAGadgets[4].Flags |= GADGDISABLED;
		} else	{
			strcpy(Fname,PlotName);
			MAStringSpec[4].Buffer = Fname;
			MAGadgets[4].Flags &= ~GADGDISABLED;
		}
	}

	RefreshGadgets(&MAGadgets[0],w,NULL);

}

BOOL drawing_dirty=TRUE;
disp_driv()
{

	drawing_dirty=TRUE;
	
}

refresh_driv()
{

	if (!drawing_dirty) return;
	drawing_dirty=FALSE;

	MinIX = mainbase.MPI->StepX * ((STDFLT) mainbase.MPI->MinCX);
	MaxIX = mainbase.MPI->StepX * ((STDFLT) mainbase.MPI->MaxCX);
	MinIY = mainbase.MPI->StepY * ((STDFLT) mainbase.MPI->MinCY);
	MaxIY = mainbase.MPI->StepY * ((STDFLT) mainbase.MPI->MaxCY);

	printf("MaxIX=%f,MaxIY=%f\n",MaxIX,MaxIY);

	reqtab[4].varptr = (UBYTE *) PlotName;
	reqtab[5].varptr = (UBYTE *) &PosterX;
	reqtab[6].varptr = (UBYTE *) &PosterY;


	Set_Current();
}

VOID ViewButton(inbut)
int inbut;
{

	if (inbut==133) {
		Load_View();
		return;
	}
	
	if (Current_View==NULL||Root_View==NULL) {
		DisplayBeep(screen);
		return;
	}

	switch (inbut) {

	case 130:	/* Get previous */
		if (Current_View->last==NULL) {
			DisplayBeep(screen);
			return;
		}
		Current_View = Current_View->last;
		break;
	case 131:	/* Get next */
		if (Current_View->next==NULL) {
			DisplayBeep(screen);
			return;
		}
		Current_View = Current_View->next;
		break;
	case 132:	/* Delete current */
		if (complain("No","Proceed",4,4,4,4,
			"Un-Paste Current View?",NULL,NULL,NULL)) return;
		if (Current_View->next==NULL&&Current_View->last==NULL) {
			Current_View = NULL;
			Root_View = Last_View = NULL;
			break;
		}
		if (Current_View->last==NULL) {
			Current_View->next->last = NULL;
			Root_View = Current_View->next;
			Current_View = Root_View;
			break;
		}
		if (Current_View->next==NULL) {
			Current_View->last->next = NULL;
			Last_View = Current_View->last;
			Current_View = Last_View;
			break;
		}
		Current_View->last->next = Current_View->next;
		Current_View->next->last = Current_View->last;
		Current_View=Current_View->last;
		break;
		
	}

	Set_Current();
}

dobutton(button)
int button;
{

	switch (button) {
	case 2:	Current_View->YNewOrigin += MaxDY - Current_View->YtrapMAX; 
		break;
	case 4:	Current_View->XNewOrigin += MinDX - Current_View->XtrapMIN;	
		break;
	case 6:	Current_View->XNewOrigin += MaxDX - Current_View->XtrapMAX;	
		break;
	case 8:	Current_View->YNewOrigin += MinDY - Current_View->YtrapMIN;	
		break;
	}

}

centerit()
{

	STDFLT cx,cy,ox,oy,tsin,tcos;

	tcos = cos(Current_View->ViewRot);
	tsin = sin(Current_View->ViewRot);

	cx = (MaxDX-MinDX)/2.0;
	cy = (MaxDY-MinDY)/2.0;

	ox = - (Current_View->XViewMAX-Current_View->XViewMIN)
			*Current_View->ViewScale*.5;
	oy = - (Current_View->YViewMAX-Current_View->YViewMIN)
			*Current_View->ViewScale*.5;

	Current_View->XNewOrigin = ox*tcos-oy*tsin+cx + MinDX;
	Current_View->YNewOrigin = ox*tsin+oy*tcos+cy + MinDY;

}

VOID serv_origin(ix,iy,action,sx,sy)
int ix,iy,action;
STDFLT sx,sy;
{
	switch (action) {
	
	case MOVEMENT:
	case UMOVEMENT:
		Current_View->XNewOrigin = sx;
		Current_View->YNewOrigin = sy;
		rendview(Current_View);
		break;
	case LBUTTONDOWN:
		Current_View->XNewOrigin = sx;
		Current_View->YNewOrigin = sy;
		mouseaction = NULL;
		BList[0].Flags &= ~SELECTED;
		Set_Current();
		break;
	}
}

VOID serv_rot(ix,iy,action,sx,sy)
int ix,iy,action;
STDFLT sx,sy;
{
	switch (action) {
	
	case MOVEMENT:
	case UMOVEMENT:
		Current_View->ViewRot = atan2(sy-Current_View->YNewOrigin,
					      sx-Current_View->XNewOrigin);
		Current_View->RotCos = cos(Current_View->ViewRot);
		Current_View->RotSin = sin(Current_View->ViewRot);
		rendview(Current_View);
		break;
	case LBUTTONDOWN:
		Current_View->ViewRot = atan2(sy-Current_View->YNewOrigin,
					      sx-Current_View->XNewOrigin);
		Current_View->RotCos = cos(Current_View->ViewRot);
		Current_View->RotSin = sin(Current_View->ViewRot);
		mouseaction = NULL;
		BList[2].Flags &= ~SELECTED;
		Set_Current();
		break;
	}
}

VOID serv_scale(ix,iy,action,sx,sy)
int ix,iy,action;
STDFLT sx,sy;
{

	BOOL usex;
	STDFLT bl,ts;
	
	usex = (abs(Current_View->RotCos)>abs(Current_View->RotSin));

	switch (action) {
	
	case MOVEMENT:
	case UMOVEMENT:
		if (usex)
			bl = (sx-Current_View->XNewOrigin)/Current_View->RotCos;
		else	bl = (sy-Current_View->YNewOrigin)/Current_View->RotSin;
		ts = bl / (Current_View->XViewMAX-Current_View->XViewMIN);
		if (ts>0.0) Current_View->ViewScale = ts;
		
		rendview(Current_View);
		break;
	case LBUTTONDOWN:
		if (usex)
			bl = (sx-Current_View->XNewOrigin)/Current_View->RotCos;
		else	bl = (sy-Current_View->YNewOrigin)/Current_View->RotSin;
		ts = bl / (Current_View->XViewMAX-Current_View->XViewMIN);
		if (ts>0.0) Current_View->ViewScale = ts;
		
		mouseaction = NULL;
		BList[1].Flags &= ~SELECTED;
		Set_Current();
		break;
	}
}

VOID togserv(ingad)
struct Gadget *ingad;
{

	BOOL NowOn;
	
	NowOn = ingad->Flags&SELECTED?TRUE:FALSE;

	switch (ingad->GadgetID) {
	case 104:	/* Flip object over 90 */
		if (Current_View==NULL) return;
		Current_View->ViewRot += PI/2.0;
		if (Current_View->ViewRot>PI*2.0)
			Current_View->ViewRot -= PI*2.0;
		Set_Current();
		break;
	case 101:	/* Move the origin of the box around */
		if (Current_View==NULL) return;
		if (NowOn) {
			mouseaction = serv_origin;	
			Update_Disp();
		} else 	{
			mouseaction = NULL;
			Set_Current();
		}
		break;
	case 102:	/* Move the origin of the box around */
		if (Current_View==NULL) return;
		if (NowOn) {
			mouseaction = serv_scale;	
			Update_Disp();
		} else 	{
			mouseaction = NULL;
			Set_Current();
		}
		break;
	case 103:	/* Change the rotation of the box */
		if (Current_View==NULL) return;
		if (NowOn) {
			mouseaction = serv_rot;	
			Update_Disp();
		} else 	{
			mouseaction = NULL;
			Set_Current();
		}
		break;
	case 105:	/* Sheet/View Lock */
		break;

	}
}

VOID gridserv(ingad)
struct Gadget *ingad;
{
	int button;

	if (ingad==NULL) return;
	if (ingad->GadgetID>=130) {
		ViewButton(ingad->GadgetID);
		return;
	}

	button = ingad->GadgetID - 110;

	switch(button) {
	case 1:		dobutton(2); dobutton(4);	break;
	case 3:		dobutton(2); dobutton(6);	break;
	case 7:		dobutton(4); dobutton(8);	break;
	case 9:		dobutton(6); dobutton(8);	break;
	case 5:		centerit();
	default:	dobutton(button);
	}

	Set_Current();
}
