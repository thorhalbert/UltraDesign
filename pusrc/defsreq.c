/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	defsreq.c - driver parameter editing
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
 *	8	30-Aug-88 SH	split definitions from drivreq to defsreq
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "plotsetup.h"
#include <graphics/gfxmacros.h>

/*----------------------------------Macros-----------------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*---------------------Lettering Style Edit Requester---------------------*/

STDFLT ParLin();
STDFLT ParAng();
UBYTE *DispLin(/* STDFLT */);
UBYTE *DispAng(/* STDFLT */);
LOCAL struct RastPort *reqrp;

#define BST 166
#define TG TOGGLESELECT
#define SWT togserv
#define BUT gridserv

BOOL save_r[4] = {FALSE,FALSE,TRUE,FALSE};

VOID DFSTRINGHandler(),MAHandler(),MATogHandle();

struct Requester DefsRequester;

LOCAL struct IntuiText MAListText[] = {
	{ 2,3,JAM2,0,0,NULL,"  Cancel  ",NULL },
	{ 2,3,JAM2,0,0,NULL,"    OK    ",NULL },
	{ 2,3,JAM2,0,0,NULL,"  Select Driver Presets  ",NULL },
	{ 2,3,JAM2,0,0,NULL,"   Save   ",NULL },
	{ 2,3,JAM2,0,0,NULL,"   Save   ",NULL },
	{ 2,3,JAM2,0,0,NULL,"   Save   ",NULL },
	{ 2,3,JAM2,0,0,NULL,"Load Driver",NULL },
};

LOCAL UBYTE DFDATA[13][32];

LOCAL struct StringInfo DFSTRINGSpec[] = {
	{ &DFDATA[0][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &DFDATA[1][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &DFDATA[2][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &DFDATA[3][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &DFDATA[4][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &DFDATA[5][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &DFDATA[6][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &DFDATA[7][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &DFDATA[8][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &DFDATA[9][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &DFDATA[10][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &DFDATA[11][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &DFDATA[12][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL }
};

#define FR 16
#define LPS 115
#define LPSR (LPS-(152-88))
#define FRG 162
#define LPSG 350
#define GDST 	88,8,GADGHCOMP,RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, \
		STRGADGET|REQGADGET,NULL, NULL, NULL, 0
#define GDSC 	152,8,GADGHCOMP,RELVERIFY|GADGIMMEDIATE|STRINGCENTER, \
		STRGADGET|REQGADGET,NULL, NULL, NULL, 0

struct Gadget DFGADGETS[] = {
#undef GADUN
#define GADUN 0
	{ &DFGADGETS[GADUN+1],LPSR,FR+(GADUN*9),GDSC,
		(APTR) &DFSTRINGSpec[GADUN],30+GADUN,(APTR) DFSTRINGHandler  },
#undef GADUN
#define GADUN 1
	{ &DFGADGETS[GADUN+1],LPSR,FR+(GADUN*9),GDSC,
		(APTR) &DFSTRINGSpec[GADUN],30+GADUN,(APTR) DFSTRINGHandler  },
#undef GADUN
#define GADUN 2
	{ &DFGADGETS[GADUN+1],LPS,FR+(GADUN*9),GDST,
		(APTR) &DFSTRINGSpec[GADUN],30+GADUN,(APTR) DFSTRINGHandler  },
#undef GADUN
#define GADUN 3
	{ &DFGADGETS[GADUN+1],LPS,FR+(GADUN*9),GDST,
		(APTR) &DFSTRINGSpec[GADUN],30+GADUN,(APTR) DFSTRINGHandler  },
#undef GADUN
#define GADUN 4
	{ &DFGADGETS[GADUN+1],LPS,FR+(GADUN*9),GDST,
		(APTR) &DFSTRINGSpec[GADUN],30+GADUN,(APTR) DFSTRINGHandler  },
#undef GADUN
#define GADUN 5
	{ &DFGADGETS[GADUN+1],LPS,FR+(GADUN*9),GDST,
		(APTR) &DFSTRINGSpec[GADUN],30+GADUN,(APTR) DFSTRINGHandler  },
#undef GADUN
#define GADUN 6
	{ &DFGADGETS[GADUN+1],LPS,FR+(GADUN*9),GDST,
		(APTR) &DFSTRINGSpec[GADUN],30+GADUN,(APTR) DFSTRINGHandler  },
#undef GADUN
#define GADUN 7
	{ &DFGADGETS[GADUN+1],LPS,FR+(GADUN*9),GDST,
		(APTR) &DFSTRINGSpec[GADUN],30+GADUN,(APTR) DFSTRINGHandler  },
#undef GADUN
#define GADUN 8
	{ &DFGADGETS[GADUN+1],LPS,FR+(GADUN*9),GDST,
		(APTR) &DFSTRINGSpec[GADUN],30+GADUN,(APTR) DFSTRINGHandler  },
#undef GADUN
#define GADUN 9
	{ &DFGADGETS[GADUN+1],LPS,FR+(GADUN*9),GDST,
		(APTR) &DFSTRINGSpec[GADUN],30+GADUN,(APTR) DFSTRINGHandler  },
#undef GADUN
#define GADUN 10
	{ &DFGADGETS[GADUN+1],LPS,FR+(GADUN*9),GDST,
		(APTR) &DFSTRINGSpec[GADUN],30+GADUN,(APTR) DFSTRINGHandler  },
#undef GADUN
#define GADUN 11
	{ &DFGADGETS[GADUN+1],LPS,FR+(GADUN*9),GDST,
		(APTR) &DFSTRINGSpec[GADUN],30+GADUN,(APTR) DFSTRINGHandler  },
#undef GADUN
#define GADUN 12
	{ &DFGADGETS[GADUN+1],LPS,FR+(GADUN*9),GDST,
		(APTR) &DFSTRINGSpec[GADUN],30+GADUN,(APTR) DFSTRINGHandler  },
#define EBS (FR+(13*9))
#undef GADUN
#define GADUN 13
#define MAACTIVE GADGIMMEDIATE | RELVERIFY 
	{ &DFGADGETS[GADUN+1], 100,EBS,200,8,GADGHCOMP,MAACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&MAListText[2],0,NULL,22,MAHandler},
#undef GADUN
#define GADUN 14
	{ &DFGADGETS[GADUN+1], LPS+220-80-50,EBS+12,80,8,GADGHCOMP,MAACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&MAListText[0],0,NULL,20,MAHandler},
#undef GADUN
#define GADUN 15
	{ &DFGADGETS[GADUN+1], 50,EBS+12,80,8,GADGHCOMP,MAACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&MAListText[1],0,NULL,21,MAHandler},
#undef GADUN
#define GADUN 16
	{ &DFGADGETS[GADUN+1], LPS+100,FR+(3*9)+5,80,8,GADGHNONE,MAACTIVE|TOGGLESELECT,
		BOOLGADGET|REQGADGET,
		NULL,NULL,&MAListText[3],0,NULL,40,MATogHandle},
#undef GADUN
#define GADUN 17
	{ &DFGADGETS[GADUN+1], LPS+100,FR+(6*9)+5,80,8,GADGHNONE,MAACTIVE|TOGGLESELECT,
		BOOLGADGET|REQGADGET,
		NULL,NULL,&MAListText[4],0,NULL,41,MATogHandle},
#undef GADUN
#define GADUN 18
	{ &DFGADGETS[GADUN+1], LPS+100,FR+(9*9)+5,80,8,GADGHNONE,MAACTIVE|TOGGLESELECT,
		BOOLGADGET|REQGADGET,
		NULL,NULL,&MAListText[5],0,NULL,42,MATogHandle},
#undef GADUN
#define GADUN 19
	{ NULL, LPS+100,FR+(0*9)+5,80,8,GADGHNONE,MAACTIVE|TOGGLESELECT,
		BOOLGADGET|REQGADGET,
		NULL,NULL,&MAListText[6],0,NULL,43,MATogHandle},

};

#define BC 0,1,JAM2
#define BT 3,1,JAM2
struct IntuiText DFHEADings[] = {
	{ BC,8,FR+( 0)*9  ,NULL,"Name:",&DFHEADings[1]},
	{ BC,8,FR+( 1)*9  ,NULL,"Comm:",&DFHEADings[2]},
	{ BC,8,FR+( 2)*9  ,NULL," X Min Steps:",&DFHEADings[3]},
	{ BC,8,FR+( 3)*9  ,NULL," X Max Steps:",&DFHEADings[4]},
	{ BC,8,FR+( 4)*9  ,NULL," Y Min Steps:",&DFHEADings[5]},
	{ BC,8,FR+( 5)*9  ,NULL," Y Max Steps:",&DFHEADings[6]},
	{ BC,8,FR+( 6)*9  ,NULL," X Inch/Step:",&DFHEADings[7]},
	{ BC,8,FR+( 7)*9  ,NULL," Y Inch/Step:",&DFHEADings[8]},
	{ BC,8,FR+( 8)*9  ,NULL,"  X Min Inch:",&DFHEADings[9]},
	{ BC,8,FR+( 9)*9  ,NULL,"  X Max Inch:",&DFHEADings[10]},
	{ BC,8,FR+(10)*9  ,NULL,"  Y Min Inch:",&DFHEADings[11]},
	{ BC,8,FR+(11)*9  ,NULL,"  Y Max Inch:",&DFHEADings[12]},
	{ BC,8,FR+(12)*9  ,NULL, "  # Autopens:",&DFHEADings[13]},
	{ BT,8,4,NULL,"Printer Definitions Setup",NULL},
};

extern STDFLT minnum;
extern STDFLT maxnum;
extern STDFLT zernum;
extern STDFLT onenum;
extern STDFLT divmax;
extern STDFLT txtmax;

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
} dfreqtab[] = {
{ STR, 		(UBYTE *) NULL, &minnum, &maxnum, &DFGADGETS[0] },
{ STR,		(UBYTE *) NULL, &minnum, &maxnum, &DFGADGETS[1] },
{ LNG, 		(UBYTE *) NULL, &minnum, &maxnum, &DFGADGETS[2] },
{ LNG, 		(UBYTE *) NULL, &minnum, &maxnum, &DFGADGETS[3] },
{ LNG, 		(UBYTE *) NULL, &minnum, &maxnum, &DFGADGETS[4] },
{ LNG, 		(UBYTE *) NULL, &minnum, &maxnum, &DFGADGETS[5] },
{ SCI, 		(UBYTE *) NULL, &minnum, &maxnum, &DFGADGETS[6] },
{ SCI, 		(UBYTE *) NULL, &minnum, &maxnum, &DFGADGETS[7] },
{ LIN, 		(UBYTE *) NULL, &minnum, &maxnum, &DFGADGETS[8] },
{ LIN, 		(UBYTE *) NULL, &minnum, &maxnum, &DFGADGETS[9] },
{ LIN, 		(UBYTE *) NULL, &minnum, &maxnum, &DFGADGETS[10] },
{ LIN, 		(UBYTE *) NULL, &minnum, &maxnum, &DFGADGETS[11] },
{ WRD, 		(UBYTE *) NULL, &minnum, &maxnum, &DFGADGETS[12] },
};

#define TOTREQS 13

LOCAL BOOL exflag;
LOCAL int postaction=0;

UBYTE TempBuf[20];

UBYTE *FormDefReq(innum)
int innum;
{
	LONG	*wp;
	WORD	*op;
	STDFLT	*fp;
	UBYTE	*an;

	struct _reqtable *mpr;
	mpr = &dfreqtab[innum];

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
		sprintf(TempBuf,"%.4f",*fp);
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

VOID dispbutton(button)
int button;
{
	/*printf("Try button %d - ",button);*/
	sprintf(DFSTRINGSpec[button].Buffer,"%s",FormDefReq(button));
	/*printf("%s\n",DFSTRINGSpec[button].Buffer);*/
	StgSet(&DFSTRINGSpec[button]);
}

VOID DFMarBeep()
{

	int a;

	for (a=0; a<TOTREQS; a++) dispbutton(a);

	DisplayBeep(screen);

	RefreshGadgets(&DFGADGETS[0],w,NULL);
	return;
}

VOID MATogHandle(ingad)
struct Gadget *ingad;
{

	char *pt,*nt;
	int tog;
	BOOL gflag;
	
	tog = ingad->GadgetID-40;
	
	gflag = (ingad->Flags & SELECTED)?TRUE:FALSE;

	switch (tog) {
	case 3:		pt = "Load Driver";
			nt = "Don't Load ";
			break;
	default:
			pt = "   Save   ";
			nt = "Don't Save";
			break;
	}

	if (gflag) 
		MAListText[3+tog].IText=pt;
	else	MAListText[3+tog].IText=nt;

	PrintIText(reqrp,&MAListText[3+tog],ingad->LeftEdge,ingad->TopEdge);

}

static UBYTE Name[50],Comment[50],svr[4];
static LONG mincx,maxcx,mincy,maxcy,ap;
static STDFLT stx,sty,mnx,mxx,mny,mxy;

SaveDfs()
{
	int a;
	
	for (a=0; a<4; a++) svr[a] = save_r[a];

	sprintf(Name,"%s",mainbase.MPI->PlotName);
	sprintf(Comment,"%s",mainbase.MPI->Comment);
	mincx = mainbase.MPI->MinCX;
	maxcx = mainbase.MPI->MaxCX;
	mincy = mainbase.MPI->MinCY;
	maxcy = mainbase.MPI->MaxCY;
	stx = mainbase.MPI->StepX;
	sty = mainbase.MPI->StepY;
	mnx = MinIX;
	mxx = MaxIX;
	mny = MinIY;
	mxy = MaxIY;
	ap = mainbase.MPI->AutoPens;
}

RestoreDefs()
{

	int a;
	
	for (a=0; a<4; a++) save_r[a] = svr[a];

	sprintf(mainbase.MPI->PlotName,"%s",Name);
	sprintf(mainbase.MPI->Comment,"%s",Comment);
	mainbase.MPI->MinCX = mincx;
	mainbase.MPI->MaxCX = maxcx;
	mainbase.MPI->MinCY = mincy;
	mainbase.MPI->MaxCY = maxcy;
	mainbase.MPI->StepX = stx;
	mainbase.MPI->StepY = sty;
	MinIX = mnx;
	MaxIX = mxx;
	MinIY = mny;
	MaxIY = mxy;
	mainbase.MPI->AutoPens = ap;

}

#define CloseRequest(a,b) EndRequest(a,b)
VOID MAHandler(ingad)
struct Gadget *ingad;
{

	CloseRequest(&DefsRequester,w);	/* Close manually */

	switch (ingad->GadgetID) {
	case 20:
		RestoreDefs();
		exflag=FALSE;
		postaction = 0;
		return;
	case 21:
		exflag=FALSE;
		postaction = 0;
		return;
	case 22:
		exflag=TRUE;
		postaction = 1;
		return;
	}

	return;

}

VOID DFSTRINGHandler(ingad)
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

	inbit = &dfreqtab[button];
	
	switch (inbit->varflg&TYPMSK) {
	case STR:
		inchk = FALSE;
		break;
	case LNG:
		inchk = FALSE;
		if (sscanf(DFSTRINGSpec[button].Buffer,"%d",&inlong)!=1) 
			inchk=TRUE;
		if (!inchk) inquery = (STDFLT) inlong;
		break;
	case NUM:
		inchk = FALSE;
		if (sscanf(DFSTRINGSpec[button].Buffer,"%f",&inquery)!=1) 
			inchk=TRUE;
		break;
	case WRD:
		inchk = FALSE;
		if (sscanf(DFSTRINGSpec[button].Buffer,"%d",&inlong)!=1) 
			inchk=TRUE;
		if (!inchk) inquery = (STDFLT) inlong;
		break;
	case SCI:
	case LIN:
		inquery = ParLin(DFSTRINGSpec[button].Buffer,&inchk);
		if (inchk) break;
		break;
	case ANG:
		inquery = ParAng(DFSTRINGSpec[button].Buffer,&inchk);
		break;
	}

	if (inbit->varflg&MQ) {
		if (inquery<=*(inbit->minval)) inchk=TRUE;
	}  else if (inquery<*(inbit->minval)) inchk=TRUE;

	if (inbit->varflg&XQ) {
		if (inquery>=*(inbit->maxval)) inchk=TRUE;
	}  else if (inquery>*(inbit->maxval)) inchk=TRUE;

	if (inchk) {
		DFMarBeep();
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
		}

		switch(button) {
		case 2:
			MinIX = ((STDFLT) mainbase.MPI->MinCX) *
				mainbase.MPI->StepX;
			dispbutton(8);
			break;
		case 3:
			MaxIX = ((STDFLT) mainbase.MPI->MaxCX) *
				mainbase.MPI->StepX;
			dispbutton(9);
			break;
		case 4:
			MinIY = ((STDFLT) mainbase.MPI->MinCY) *
				mainbase.MPI->StepY;
			dispbutton(10);
			break;
		case 5:
			MaxIY = ((STDFLT) mainbase.MPI->MaxCY) *
				mainbase.MPI->StepY;
			dispbutton(11);
			break;
		case 6:
			MinIX = ((STDFLT) mainbase.MPI->MinCX) *
				mainbase.MPI->StepX;
			dispbutton(8);
			MaxIX = ((STDFLT) mainbase.MPI->MaxCX) *
				mainbase.MPI->StepX;
			dispbutton(9);
			break;
		case 7:
			MinIY = ((STDFLT) mainbase.MPI->MinCY) *
				mainbase.MPI->StepY;
			dispbutton(10);
			MaxIY = ((STDFLT) mainbase.MPI->MaxCY) *
				mainbase.MPI->StepY;
			dispbutton(11);
			break;
		case 8:
			mainbase.MPI->MinCX = MinIX / 
				mainbase.MPI->StepX;
			dispbutton(2);
			break;
		case 9:
			mainbase.MPI->MaxCX = MaxIX /
				mainbase.MPI->StepX;
			dispbutton(3);
			break;
		case 10:
			mainbase.MPI->MinCY = MinIY /
				mainbase.MPI->StepY;
			dispbutton(4);
			break;
		case 11:
			mainbase.MPI->MaxCY = MaxIY /
				mainbase.MPI->StepY;
			dispbutton(5);
			break;
		}
			
								
	}

	dispbutton(button);
	RefreshGadgets(&DFGADGETS[0],w,NULL);
	disp_driv();
			
	return;

}

defs_setup()
{


	int a;

	for (a=0; a<TOTREQS; a++) dfreqtab[a].varptr=NULL;

	dfreqtab[0].varptr = (UBYTE *) &mainbase.MPI->PlotName;
	dfreqtab[1].varptr = (UBYTE *) &mainbase.MPI->Comment;
	dfreqtab[2].varptr = (UBYTE *) &mainbase.MPI->MinCX;
	dfreqtab[3].varptr = (UBYTE *) &mainbase.MPI->MaxCX;
	dfreqtab[4].varptr = (UBYTE *) &mainbase.MPI->MinCY;
	dfreqtab[5].varptr = (UBYTE *) &mainbase.MPI->MaxCY;
	dfreqtab[6].varptr = (UBYTE *) &mainbase.MPI->StepX;
	dfreqtab[7].varptr = (UBYTE *) &mainbase.MPI->StepY;
	dfreqtab[8].varptr = (UBYTE *) &MinIX;
	dfreqtab[9].varptr = (UBYTE *) &MaxIX;
	dfreqtab[10].varptr = (UBYTE *) &MinIY;
	dfreqtab[11].varptr = (UBYTE *) &MaxIY;
	dfreqtab[12].varptr = (UBYTE *) &mainbase.MPI->AutoPens;
	/*if (mainbase.MPI->AutoPens<=0) dfreqtab[12].varptr = NULL;*/

	for (a=0; a<TOTREQS; a++) {
		if (dfreqtab[a].varptr==NULL) 
			DFGADGETS[a].Flags |= GADGDISABLED;
		else	DFGADGETS[a].Flags &= ~GADGDISABLED;

		dispbutton(a);

	}

	RefreshGadgets(&DFGADGETS[0],w,NULL);

}

WarnLoad()
{

	complain("Recover",NULL,1,0,0,0,
		"You must load a driver first!",NULL,NULL,NULL);

	return;
}

Defs_Req()
{

	struct Gadget *look;
	int a;

	if (DriverSegHook==NULL) return(WarnLoad());

	for (a=0; a<4; a++)
		if (save_r[a]) DFGADGETS[16+a].Flags |= SELECTED;
		else		DFGADGETS[16+a].Flags &= ~SELECTED;
		
	exflag = TRUE;
 	SaveDfs();

	while (exflag) {

		InitRequester(&DefsRequester);

		defs_setup();
		
	/*	for (look= &DFGADGETS[0]; look!=NULL; look=look->NextGadget) {
			printf("Left=%d Top=%d Height=%d Width=%d\n",
				look->LeftEdge,look->TopEdge,
				look->Height, look->Width);
			printf("ID=%d User=%x\n",look->GadgetID,look->UserData);
		} */
		
		DefsRequester.Width=LPS+220;
		DefsRequester.Height=EBS+24;
		DefsRequester.LeftEdge=(XSsize-DefsRequester.Width)/2;
		DefsRequester.TopEdge=(YSsize-DefsRequester.Height)/2;

		DefsRequester.BackFill=1;

		DefsRequester.ReqText = &DFHEADings[0];
		DefsRequester.ReqGadget = &DFGADGETS[0];

		DefsRequester.Flags = NOISYREQ;

		SafeRequest(&DefsRequester, w);
	
		reqrp = DefsRequester.ReqLayer->rp;	/* Nasty */
		SetAPen(reqrp, 5);
		
		Move(reqrp, 0, 0);
		Draw(reqrp, (DefsRequester.Width-1), 0);
		Draw(reqrp, (DefsRequester.Width-1), DefsRequester.Height);
		Draw(reqrp, 0, DefsRequester.Height);
		Draw(reqrp, 0, 0);

		Move(reqrp, 3, 2);
		Draw(reqrp, (DefsRequester.Width-1)-3, 2);
		Draw(reqrp, (DefsRequester.Width-1)-3, DefsRequester.Height-2);
		Draw(reqrp, 3, DefsRequester.Height-2);
		Draw(reqrp, 3, 2);
		
		Move(reqrp, 3, 14);
		Draw(reqrp, (DefsRequester.Width-1)-3, 14);

		Move(reqrp, 3, EBS+10);
		Draw(reqrp, (DefsRequester.Width-1)-3, EBS+10);

		Move(reqrp, LPS+88, FR+(2*9));
		Draw(reqrp, LPS+190, FR+(2*9));
		Draw(reqrp, LPS+190, FR+(6*9)-2);
		Draw(reqrp, LPS+88, FR+(6*9)-2);
				
		Move(reqrp, LPS+88, FR+(6*9));
		Draw(reqrp, LPS+190, FR+(6*9));
		Draw(reqrp, LPS+190, FR+(8*9)-2);
		Draw(reqrp, LPS+88, FR+(8*9)-2);
				
		Move(reqrp, LPS+88, FR+(8*9));
		Draw(reqrp, LPS+190, FR+(8*9));
		Draw(reqrp, LPS+190, FR+(12*9)-2);
		Draw(reqrp, LPS+88, FR+(12*9)-2);
				
		MATogHandle(&DFGADGETS[16]);
		MATogHandle(&DFGADGETS[17]);
		MATogHandle(&DFGADGETS[18]);
		MATogHandle(&DFGADGETS[19]);

		postaction = 0;

		sit_n_spin(&DefsRequester,0,&DFGADGETS[14],&DFGADGETS[15]);
				/* Wait for requesters to exit */

		for (a=0; a<4; a++) 
			save_r[a] = (DFGADGETS[a+16].Flags & SELECTED)
					?TRUE:FALSE;

		switch (postaction) {
		case 1:
			Get_Preset();
			break;
		}

	
	}
	
	disp_driv();

}
