/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	marginreq.c - maintain the margins
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
 *	7	10-Jan-88 SH	Update string requester response with
 *				 new GList intuition routines
 *	8	12-May-89 SH	Add multiline support and orthoangle
 *	9	14-May-89 SH	Add margins report
 *	10	 8-Sep-89 SH	Add filet radius
 *	11	16-May-90 SH	Add freehand pixel jump size
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*----------------------------------Macros-----------------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*---------------------Lettering Style Edit Requester---------------------*/

struct Requester MarginRequester;

VOID MAStringHandler(),MAHandler();

LOCAL UBYTE MAData[28][32];

LOCAL struct StringInfo MAStringSpec[] = {
	{ &MAData[0][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[1][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[2][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[3][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[4][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[5][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[6][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[7][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[8][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[9][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[10][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[11][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[12][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[13][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[14][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[15][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[16][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[17][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[18][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[19][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[20][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[21][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[22][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[23][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[24][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[25][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[26][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &MAData[27][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
};
LOCAL struct IntuiText MAListText[] = {
	{ 2,3,JAM2,0,0,NULL,"  Cancel  ",NULL },
	{ 2,3,JAM2,0,0,NULL,"    OK    ",NULL }};
#define FR 20
#define LCP 136
#define RCP 353
LOCAL struct Gadget MAGadgets[] = {
	{ &MAGadgets[1], LCP, FR, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[0], 30, MAStringHandler  },
	{ &MAGadgets[2], RCP, FR, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[1], 31, MAStringHandler  },
	{ &MAGadgets[3], LCP, FR+10, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[2], 32, MAStringHandler  },
	{ &MAGadgets[4], RCP, FR+10, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[3], 33, MAStringHandler  },
	{ &MAGadgets[5], LCP, FR+20, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[4], 34, MAStringHandler  },
	{ &MAGadgets[6], RCP, FR+20, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[5], 35, MAStringHandler  },
	{ &MAGadgets[7], LCP, FR+30, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[6], 36, MAStringHandler  },
	{ &MAGadgets[8], RCP, FR+30, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[7], 37, MAStringHandler  },
	{ &MAGadgets[9], LCP, FR+40, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[8], 38, MAStringHandler  },
	{ &MAGadgets[10], RCP, FR+40, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[9], 39, MAStringHandler  },
	{ &MAGadgets[11], LCP, FR+50, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[10], 40, MAStringHandler  },
	{ &MAGadgets[12], RCP, FR+50, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[11], 41, MAStringHandler  },
	{ &MAGadgets[13], LCP, FR+60, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[12], 42, MAStringHandler  },
	{ &MAGadgets[14], RCP, FR+60, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[13], 43, MAStringHandler  },
	{ &MAGadgets[15], LCP, FR+70, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[14], 44, MAStringHandler  },
	{ &MAGadgets[16], RCP, FR+70, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[15], 45, MAStringHandler  },
	{ &MAGadgets[17], LCP, FR+80, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[16], 46, MAStringHandler  },
	{ &MAGadgets[18], RCP, FR+80, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[17], 47, MAStringHandler  },
	{ &MAGadgets[19], LCP, FR+90, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[18], 48, MAStringHandler  },
	{ &MAGadgets[20], RCP, FR+90, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[19], 49, MAStringHandler  },
	{ &MAGadgets[21], LCP, FR+100, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[20], 50, MAStringHandler  },
	{ &MAGadgets[22], RCP, FR+100, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[21], 51, MAStringHandler  },
	{ &MAGadgets[23], LCP, FR+110, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[22], 52, MAStringHandler  },
	{ &MAGadgets[24], RCP, FR+110, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[23], 53, MAStringHandler  },
	{ &MAGadgets[25], LCP, FR+120, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[24], 54, MAStringHandler  },
	{ &MAGadgets[26], RCP, FR+120, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[25], 55, MAStringHandler  },
	{ &MAGadgets[27], LCP, FR+130, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[26], 56, MAStringHandler  },
	{ NULL,           RCP, FR+130, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &MAStringSpec[27], 57, MAStringHandler  },
};

#define MAACTIVE GADGIMMEDIATE | RELVERIFY 
LOCAL struct Gadget MAList[] = {
	{ &MAList[1], 270,FR+145,80,8,GADGHCOMP,MAACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&MAListText[0],0,NULL,20,MAHandler},
	{ &MAGadgets[0], 100,FR+145,80,8,GADGHCOMP,MAACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&MAListText[1],0,NULL,21,MAHandler}};

LOCAL struct IntuiText MAHeadings[] = {
	{ 3,1,JAM2,5,4,NULL,"UltraDesign Margins and Configuration:",&MAHeadings[1]},
	{ 2,1,JAM2,8,FR,NULL,		"         X Snap:",&MAHeadings[2]},
	{ 2,1,JAM2,8,FR+10,NULL,	"       X Offset:",&MAHeadings[3]},
	{ 2,1,JAM2,8,FR+20,NULL,	"    X Grid Size:",&MAHeadings[4]},
	{ 2,1,JAM2,8,FR+30,NULL,	"    X Grid Offs:",&MAHeadings[5]},
	{ 2,1,JAM2,8,FR+40,NULL,	"   # X Grid Div:",&MAHeadings[6]},
	{ 2,1,JAM2,8,FR+50,NULL,	"  Left Boundary:",&MAHeadings[7]},
	{ 2,1,JAM2,8,FR+60,NULL,	" Right Boundary:",&MAHeadings[8]},
	{ 2,1,JAM2,8,FR+70,NULL,	"     Paper Unit:",&MAHeadings[9]},
	{ 2,1,JAM2,8,FR+80,NULL,	"    Arrow Angle:",&MAHeadings[10]},
	{ 2,1,JAM2,8,FR+90,NULL,	"  Lin Tolerance:",&MAHeadings[11]},
	{ 2,1,JAM2,8,FR+100,NULL,	"   # Linear Pts:",&MAHeadings[12]},
	{ 2,1,JAM2,8,FR+110,NULL,	"      Qtext Box:",&MAHeadings[13]},
	{ 2,1,JAM2,8,FR+120,NULL,	"   #Multi-Lines:",&MAHeadings[14]},
	{ 2,1,JAM2,8,FR+130,NULL,	"   Filet Radius:",&MAHeadings[15]},
	{ 2,1,JAM2,225,FR,NULL,		"         Y Snap:",&MAHeadings[16]},
	{ 2,1,JAM2,225,FR+10,NULL,	"       Y Offset:",&MAHeadings[17]},
	{ 2,1,JAM2,225,FR+20,NULL,	"    Y Grid Size:",&MAHeadings[18]},
	{ 2,1,JAM2,225,FR+30,NULL,	"    Y Grid Offs:",&MAHeadings[19]},
	{ 2,1,JAM2,225,FR+40,NULL,	"   # Y Grid Div:",&MAHeadings[20]},
	{ 2,1,JAM2,225,FR+50,NULL,	"Bottom Boundary:",&MAHeadings[21]},
	{ 2,1,JAM2,225,FR+60,NULL,	"   Top Boundary:",&MAHeadings[22]},
	{ 2,1,JAM2,225,FR+70,NULL,	"   Drawing Unit:",&MAHeadings[23]},
	{ 2,1,JAM2,225,FR+80,NULL,	"   Arrow Length:",&MAHeadings[24]},
	{ 2,1,JAM2,225,FR+90,NULL,	"  Ang Tolerance:",&MAHeadings[25]},
	{ 2,1,JAM2,225,FR+100,NULL,	"  # Angular Pts:",&MAHeadings[26]},
	{ 2,1,JAM2,225,FR+110,NULL,	"    Ortho Angle:",&MAHeadings[27]},
	{ 2,1,JAM2,225,FR+120,NULL,	"   Line Spacing:",&MAHeadings[28]},
	{ 2,1,JAM2,225,FR+130,NULL,	"Freehand pixels:",NULL}
};

STDFLT minnum = -MAXBIG;
STDFLT maxnum = MAXBIG;
STDFLT zernum = 0.0;
STDFLT onenum = 1.0;
STDFLT divmax = 64.0;
STDFLT txtmax = 5;	

#define MQ 4
#define XQ 8
#define WRD 0
#define LIN 1
#define ANG 2

struct _reqtable {
	UBYTE	varflg;
	APTR	varptr;
	STDFLT	*minval;
	STDFLT	*maxval;
	struct	Gadget	*reqgad;
	union {
		STDFLT	savflt;
		LONG	savlng;
	} savvars;
} reqtab[] = {
{ LIN, 		(APTR) &xsnap, 	&zernum, &maxnum, &MAGadgets[0] },
{ LIN, 		(APTR) &ysnap, 	&zernum, &maxnum, &MAGadgets[1] },
{ LIN, 		(APTR) &xoffset, &minnum, &maxnum, &MAGadgets[2] },
{ LIN, 		(APTR) &yoffset, &minnum, &maxnum, &MAGadgets[3] },
{ LIN, 		(APTR) &xgrid, 	&zernum, &maxnum, &MAGadgets[4] },
{ LIN, 		(APTR) &ygrid, 	&zernum, &maxnum, &MAGadgets[5] },
{ LIN, 		(APTR) &xgridoff, &minnum, &maxnum, &MAGadgets[6] },
{ LIN, 		(APTR) &ygridoff, &minnum, &maxnum, &MAGadgets[7] },
{ WRD, 		(APTR) &xgriddiv, &onenum, &divmax, &MAGadgets[8] },
{ WRD, 		(APTR) &ygriddiv, &onenum, &divmax, &MAGadgets[9] },
{ LIN, 		(APTR) &xviewMIN, &minnum, &xviewMAX, &MAGadgets[10] },
{ LIN, 		(APTR) &yviewMIN, &minnum, &yviewMAX, &MAGadgets[11] },
{ LIN, 		(APTR) &xviewMAX, &xviewMIN, &maxnum, &MAGadgets[12] },
{ LIN, 		(APTR) &yviewMAX, &yviewMIN, &maxnum, &MAGadgets[13] },
{ LIN, 		(APTR) &paperunit, &zernum, &maxnum, &MAGadgets[14] },
{ LIN, 		(APTR) &viewunit, &zernum, &maxnum, &MAGadgets[15] },
{ ANG, 		(APTR) &Arrow_Angle, &minnum, &maxnum, &MAGadgets[16] },
{ LIN, 		(APTR) &Arrow_Len, &zernum, &maxnum, &MAGadgets[17] },
{ LIN, 		(APTR) &lintolerance, &zernum, &maxnum, &MAGadgets[18] },
{ ANG, 		(APTR) &angtolerance, &zernum, &maxnum, &MAGadgets[19] },
{ MQ|WRD,	(APTR) &linpoints, &zernum, &maxnum, &MAGadgets[20] },
{ MQ|WRD,	(APTR) &angpoints, &zernum, &maxnum, &MAGadgets[21] },
{ MQ|WRD,	(APTR) &qtextlim, &zernum, &maxnum, &MAGadgets[22] },
{ ANG,		(APTR) &orthoangle, &zernum, &maxnum, &MAGadgets[23] },
{ MQ|WRD,	(APTR) &multi_line, &zernum, &maxnum, &MAGadgets[24] },
{ LIN,		(APTR) &multi_space, &zernum, &maxnum, &MAGadgets[25] },
{ LIN,		(APTR) &filletrad, &zernum, &maxnum, &MAGadgets[26] },
{ WRD,		(APTR) &free_pixels, &zernum, &maxnum, &MAGadgets[27] },
};	
#define TOTREQS 28

LOCAL BOOL exflag;
LOCAL int postaction=0;

VOID SaveSettings()
{

	struct _reqtable *mpr;
	WORD	*wp;
	STDFLT	*fp;
	int	a;

	for (a=0; a<TOTREQS; a++) {
		mpr = &reqtab[a];
		switch (mpr->varflg&0x03) {
		case WRD:
			wp = (WORD *) mpr->varptr;
			mpr->savvars.savlng = *wp;
			break;
		case LIN:
		case ANG:
			fp = (STDFLT *) mpr->varptr;
			mpr->savvars.savflt = *fp;
			break;
		}
	}
	printf("SaveSettings finished\n");
}

VOID RestoreSettings()
{

	struct _reqtable *mpr;
	WORD	*wp;
	STDFLT	*fp;
	int	a;

	for (a=0; a<TOTREQS; a++) {
		mpr = &reqtab[a];
		switch (mpr->varflg&0x03) {
		case WRD:
			wp = (WORD *) mpr->varptr;
			*wp = mpr->savvars.savlng;
			break;
		case LIN:
		case ANG:
			fp = (STDFLT *) mpr->varptr;
			*fp = mpr->savvars.savflt;
			break;
		}
	}
}

UBYTE TempBuf[20];
UBYTE *FormMarReq(innum)
int innum;
{
	WORD	*wp;
	STDFLT	*fp;

	struct _reqtable *mpr;
	mpr = &reqtab[innum];

	switch (mpr->varflg&0x3) {
	case WRD:
		wp = (WORD *) mpr->varptr;
		sprintf(TempBuf,"%d",(int) *wp);
		return(TempBuf);
		break;
	case LIN:
		fp = (STDFLT *) mpr->varptr;
		return(DispLin(*fp));
	case ANG:
		fp = (STDFLT *) mpr->varptr;
		return(DispAng(*fp));
	}
}

VOID MAHandler(ingad)
struct Gadget *ingad;
{

	CloseRequest(&MarginRequester,w);	/* Close manually */

	switch (ingad->GadgetID) {
	case 20:
		RestoreSettings();
		exflag=FALSE;
		postaction = 0;
		return;
	case 21:
		exflag=FALSE;
		postaction = 0;
		return;
	}

	return;

}

VOID MarBeep()
{

	int a;

	for (a=0; a<TOTREQS; a++) {

		sprintf(&MAData[a][0],"%s",FormMarReq(a));
		StgSet(&MAStringSpec[a]);
	}

	DisplayBeep(screen);

	RefreshGList(MarginRequester.ReqGadget,w,&MarginRequester,-1);
	return;
}

VOID MAStringHandler(ingad)
struct Gadget *ingad;
{

	int button,a,gpos;
	STDFLT inquery,*fp;
	LONG	inlong;
	WORD	*wp;
	BOOL	inchk;
	struct _reqtable *inbit;

	/* Forbid();
	ingad->Flags &= ~SELECTED;
	Permit();	*/

	gpos = RemoveGList(w,ingad,1);

	button = ingad->GadgetID-30;

	inbit = &reqtab[button];

	switch (inbit->varflg&0x3) {
	case WRD:
		inchk = FALSE;
		if (sscanf(&MAData[button][0],"%d",&inlong)!=1) inchk=TRUE;
		if (!inchk) inquery = (STDFLT) inlong;
		break;
	case LIN:
		inquery = ParLin(&MAData[button][0],&inchk);
		if (inchk) break;
	
		break;
	case ANG:
		inquery = ParAng(&MAData[button][0],&inchk);
		break;
	}

	if (inbit->varflg&MQ) {
		if (inquery<=*(inbit->minval)) inchk=TRUE;
	}  else if (inquery<*(inbit->minval)) inchk=TRUE;

	if (inbit->varflg&XQ) {
		if (inquery>=*(inbit->maxval)) inchk=TRUE;
	}  else if (inquery>*(inbit->maxval)) inchk=TRUE;

	if (inchk) {
		if (gpos>=0) AddGList(w,ingad,gpos,1,&MarginRequester);
		MarBeep();
		return;
	}

	switch (inbit->varflg&0x3) {
	case WRD:
		wp = (WORD *) inbit->varptr;
		*wp = inlong;
		break;
	case ANG:
	case LIN:
		fp = (STDFLT *) inbit->varptr;
		*fp = inquery;
		break;
	}

	sprintf(&MAData[button][0],"%s",FormMarReq(button));
	StgSet(&MAStringSpec[button]);

	if (gpos>=0) AddGList(w,ingad,gpos,1,&MarginRequester);
 		
	return;

}

Margin_Current()
{

	int a;

	exflag = TRUE;
 	SaveSettings(); 

	while (exflag) {

		InitRequester(&MarginRequester);

		for (a=0; a<TOTREQS; a++) {

			sprintf(&MAData[a][0],"%s",FormMarReq(a));
			StgSet(&MAStringSpec[a]);
		} 
	
		MarginRequester.Width=450;
		MarginRequester.Height=180;
		MarginRequester.LeftEdge=(XSsize-MarginRequester.Width)/2;
		MarginRequester.TopEdge=(YSsize-MarginRequester.Height)/2;

		MarginRequester.BackFill=1;

		MarginRequester.ReqText = &MAHeadings[0];
		MarginRequester.ReqGadget = &MAList[0];

#ifdef _TURNSY
		MarginRequester.Flags = NOISYREQ;
#endif

		SafeRequest(&MarginRequester, w);

		postaction = 0;

		DoBorders(&MarginRequester,15,0,0);
		sit_n_spin(&MarginRequester,0,&MAList[0],&MAList[1]);
				/* Wait for requesters to exit */

	}

	viewscale = paperunit/viewunit;
	view_setup();
	AdjustPots();

}

VOID MarginsReport()
{

	FILE *op;
	UBYTE *p;
	
	
	op = openprt("MarginReport");
	if (op==NULL) return;
	
	fprintf(op, "%s Margins, Defaults and Current Settings\n\r",ucadname);
	if (Savnam) {
		fprintf(op,"Current file: %s",Savefile);
		if (Library_Mode) fprintf(op, " (Library)");
		fprintf(op,"\n\r");
	}
	fprintf(op, "\n\r");
	
	fprintf(op, "   Current Layer: %s\n\r",Current_Layer->Layer_Name);
	fprintf(op, "   Current Pen:   %s\n\r",Current_Pen->Pen_Name);
	fprintf(op, "   Current Style: %s (Font %s)\n\r",
		Current_Style->Letter_Style_ID,
		Current_Style->draw_font->Font_Name_ID);
	fprintf(op, "   Default Pen:   %s\n\r",Default_Pen->Pen_Name);
	fprintf(op, "   Default Style: %s (Font %s)\n\r",
		Default_Style->Letter_Style_ID,
		Default_Style->draw_font->Font_Name_ID);
	if (Current_Hatch!=NULL) 
		fprintf(op, "   Current Hatch File: %s Hatch: %s\n\r",
			Current_Hatch->File_Name,Current_Hatch->Hatch_Name);

	fprintf(op, "\n\r");

	fprintf(op, "   X Snap:        %s\n\r",DispLin(xsnap));
	fprintf(op, "   Y Snap:        %s\n\r",DispLin(ysnap));
	fprintf(op, "   X Offset:      %s\n\r",DispLin(xoffset));
	fprintf(op, "   Y Offset:      %s\n\r",DispLin(yoffset));
	fprintf(op, "   X Grid:        %s\n\r",DispLin(xgrid));
	fprintf(op, "   Y Grid:        %s\n\r",DispLin(ygrid));
	fprintf(op, "   X Grid Offset: %s\n\r",DispLin(xgrid));
	fprintf(op, "   Y Grid Offset: %s\n\r",DispLin(ygrid));
	fprintf(op, "   X Grid Div:    %d\n\r",xgriddiv);
	fprintf(op, "   Y Grid Div:    %d\n\r",ygriddiv);
	fprintf(op, "   X Draw Min:    %s\n\r",DispLin(xviewMIN));
	fprintf(op, "   X Draw Max:    %s\n\r",DispLin(xviewMAX));
	fprintf(op, "   Y Draw Min:    %s\n\r",DispLin(yviewMIN));
	fprintf(op, "   Y Draw Max:    %s\n\r",DispLin(yviewMAX));
	fprintf(op, "   Qtext Limit:   %d\n\r",qtextlim);
	fprintf(op, "   Paper Unit:    %s\n\r",DispLin(paperunit));
	fprintf(op, "   Drawing Unit:  %s\n\r",DispLin(viewunit));
	if (viewunit!=0.0) 
		fprintf(op, "   View Scale:    %.4f\n\r",paperunit/viewunit);
	fprintf(op, "   Arrow Angle:   %s\n\r",DispAng(Arrow_Angle));
	fprintf(op, "   Arrow Length:  %s\n\r",DispLin(Arrow_Len));
	fprintf(op, "   Ortho Angle:   %s\n\r",DispAng(orthoangle));
	fprintf(op, "   Linear Toler:  %s\n\r",DispLin(lintolerance));
	fprintf(op, "   Linear Points: %d\n\r",linpoints);
	fprintf(op, "   Angular Toler: %s\n\r",DispAng(angtolerance));
	fprintf(op, "   Angular Pts:   %d\n\r",angpoints);
	fprintf(op, "   # Multi-lines: %d\n\r",multi_line);
	fprintf(op, "   Multi Spacing: %s\n\r",DispLin(multi_space));
	fprintf(op, "    Filet Radius: %s\n\r",DispLin(filletrad));
	fprintf(op, " Freehand Pixels: %d\n\r",free_pixels);
	
	fprintf(op, "\n\r");
	
	fprintf(op, "   Circle Centers: %s\n\r",Center_On?"On":"Off");
	fprintf(op, "   Coord Display:  %s\n\r",Coord_On?"On":"Off");
	fprintf(op, "   Info Display:   %s\n\r",Draw_Info?"On":"Off");
	fprintf(op, "   Cursor Snap:    %s\n\r",Snaps_On?"On":"Off");
	fprintf(op, "   Grid Display:   %s\n\r",Grid_On?"On":"Off");
	fprintf(op, "   Boundary Lock:  %s\n\r",Use_Bounds?"On":"Off");
	fprintf(op, "   Ortho Switch:   %s\n\r",Ortho_Switch?"On":"Off");
	fprintf(op, "   Quick Hatching: %s\n\r",Q_Hatch?"On":"Off");

	fprintf(op,"\n\r");
	
	if (Scan_Clip |
	    Scan_Group |
	    Scan_Layer |
	    Scan_Pen |
	    Scan_Text) {
	    
	    	if (Scan_Clip) 	fprintf(op, "   Scan in Clipboard On\n\r");
	    	if (Scan_Group) fprintf(op, "   Scan Same Group On\n\r");
	    	if (Scan_Layer) fprintf(op, "   Scan Same Layer On\n\r");
	    	if (Scan_Pen) 	fprintf(op, "   Scan Same Pen On\n\r");
	    	if (Scan_Text) 	fprintf(op, "   Scan Same Text Style On\n\r");
	}

	switch (lintype) {
	case LIN_EDI:	p = "English Decimal/Inches";	break;
	case LIN_EFI:	p = "Architectural English Feet/Inches"; break;
	case LIN_DF:	p = "English Decimal Feet";	break;
	case LIN_DI:	p = "English Decimal Inches";	break;
	case LIN_MM:	p = "Millimeters";		break;
	case LIN_CM:	p = "Centimeters";		break;
	case LIN_M:	p = "Meters";			break;
	case LIN_MILE:	p = "Miles";			break;
	case LIN_KM:	p = "Kilometers";		break;
	}
	
	fprintf(op, "   Linear Units:  %s\n\r",p);

	switch (angtype) {
	case ANG_DEGD:	p = "Degrees";	break;
	case ANG_DEGM:	p = "Degrees/Minutes"; break;
	case ANG_DEGMS:	p = "Degress/Minutes/Seconds"; break;
	case ANG_rad:	p = "Radians"; break;
	case ANG_Grad:	p = "Gradians (400 per circle)"; break;
	}
	
	fprintf(op, "   Angular Units  %s\n\r",p);

	fprintf(op,"\n\r");
	
	fprintf(op,"   Witness Pick Offset:    %s\n\r",DispLin(Wit_Pick));
	fprintf(op,"   Witness Line Overshoot: %s\n\r",DispLin(Wit_Over));
	fprintf(op,"   Dimension Line Offset:  %s\n\r",DispLin(Dim_Line_Offset));
	fprintf(op,"   Dimension Box Offset:   %s\n\r",DispLin(Dim_Box_Offset));
	if (Use_Fixed_Angle) 
		fprintf(op,"   Use Fixed Angle Dimension Box\n\r");
	else	fprintf(op,"   Align Dimension Box with Dimension Line\n\r");
	fprintf(op,"   Flip Box Over From:     %s\n\r",DispAng(Free_Flip_From));
	fprintf(op,"   Flip Box Over To:       %s\n\r",DispAng(Free_Flip_To));

	if (Wit_Layer!=NULL)
		fprintf(op,"   Witness Layer:  %s\n\r",Wit_Layer->Layer_Name);
	if (Wit_Pen!=NULL)
		fprintf(op,"   Witness Pen:    %s\n\r",Wit_Pen->Pen_Name);
	if (Dim_Line_Layer!=NULL)
		fprintf(op,"   Dim Line Layer: %s\n\r",Dim_Line_Layer->Layer_Name);
	if (Dim_Line_Pen!=NULL)
		fprintf(op,"   Dim Line Pen:   %s\n\r",Dim_Line_Pen->Pen_Name);
	if (Dim_Box_Layer!=NULL)
		fprintf(op,"   Dim Box Layer:  %s\n\r",Dim_Box_Layer->Layer_Name);
	if (Dim_Box_Pen!=NULL)
		fprintf(op,"   Dim Box Pen:    %s\n\r",Dim_Box_Pen->Pen_Name);
	if (Dim_Box_Style!=NULL)
		fprintf(op,"   Dim Box Style:  %s\n\r",
			Dim_Box_Style->Letter_Style_ID);

	fprintf(op,"\n\r[End of Report]\n\r\f");
	
	fclose(op);

}
