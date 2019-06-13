/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	dimsreq.c - dimensioning parameter requester
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
 *	6	30-Jan-88 SH	Use stylereq for dimsreq
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*----------------------------------Macros-----------------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*---------------------Lettering Style Edit Requester---------------------*/

LOCAL BOOL exflag;
LOCAL int postaction;
LOCAL struct Requester DimsRequester;
LOCAL UBYTE DMAttrs[10][32];
VOID DMHandler();
VOID DMStringHandler();

extern struct Image dimrq_1s_image;
extern struct Image dimrq_1u_image;

LOCAL struct IntuiText DMAttrText[] = {
	{ 3,2,JAM2,0,0,NULL,&DMAttrs[0][0],NULL },
	{ 3,2,JAM2,0,0,NULL,&DMAttrs[1][0],NULL },
	{ 3,2,JAM2,0,0,NULL,&DMAttrs[2][0],NULL },
	{ 3,2,JAM2,0,0,NULL,&DMAttrs[3][0],NULL },
	{ 3,2,JAM2,0,0,NULL,&DMAttrs[4][0],NULL },
	{ 3,2,JAM2,0,0,NULL,&DMAttrs[5][0],NULL },
	{ 3,2,JAM2,0,0,NULL,&DMAttrs[6][0],NULL },
	{ 3,2,JAM2,0,0,NULL,&DMAttrs[7][0],NULL },
	{ 3,2,JAM2,0,0,NULL,&DMAttrs[8][0],NULL },
	{ 3,2,JAM2,0,0,NULL,&DMAttrs[9][0],NULL }};

LOCAL struct IntuiText DMEndTx[] = {
	{ 2,3,JAM2,0,0,NULL,"  Cancel  ",NULL },
	{ 2,3,JAM2,0,0,NULL,"    OK    ",NULL },
	{ 2,3,JAM2,0,0,NULL," Current ",NULL },
	{ 2,3,JAM2,0,0,NULL," Default ",NULL },
};

#define DMACTIVE GADGIMMEDIATE | RELVERIFY 
#define DMBOOL BOOLGADGET|REQGADGET
#define DMLEFT 135
#define DMINFO 235
#define DMBASE 20
#define DMBOTTOM DMBASE+225

LOCAL UBYTE DMData[7][32];

LOCAL struct StringInfo DMStringSpec[] = {
	{ &DMData[0][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &DMData[1][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &DMData[2][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &DMData[3][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &DMData[4][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &DMData[5][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &DMData[6][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL }};

LOCAL struct Gadget DMEnders[] = {
	{ &DMEnders[1], 270,DMBOTTOM-15,80,8,GADGHCOMP,DMACTIVE,DMBOOL,
		NULL,NULL,&DMEndTx[0],0,NULL,20,DMHandler},
	{ &DMEnders[2], 100,DMBOTTOM-15,80,8,GADGHCOMP,DMACTIVE,DMBOOL,
		NULL,NULL,&DMEndTx[1],0,NULL,21,DMHandler},
	{ &DMEnders[3], 305,DMBASE+165,47,36,GADGIMAGE|GADGHIMAGE,
		TOGGLESELECT|DMACTIVE,DMBOOL,&dimrq_1u_image,&dimrq_1s_image,
		NULL,0,NULL,25,DMHandler},
	{ &DMEnders[4], DMINFO, DMBASE+115, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &DMStringSpec[0], 30, DMStringHandler  },
	{ &DMEnders[5], DMINFO, DMBASE+125, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &DMStringSpec[1], 31, DMStringHandler  },
	{ &DMEnders[6], DMINFO, DMBASE+140, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &DMStringSpec[2], 32, DMStringHandler  },
	{ &DMEnders[7], DMINFO, DMBASE+150, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &DMStringSpec[3], 33, DMStringHandler  },
	{ &DMEnders[8], DMINFO-50, DMBASE+170, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &DMStringSpec[4], 34, DMStringHandler  },
	{ &DMEnders[9], DMINFO-50, DMBASE+180, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &DMStringSpec[5], 35, DMStringHandler  },
	{ NULL, DMINFO-50, DMBASE+190, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &DMStringSpec[6], 36, DMStringHandler  }};

LOCAL struct Gadget DMCurr[] = {
	{ &DMCurr[1], DMLEFT+250,DMBASE+0,72,8,GADGHCOMP,DMACTIVE,DMBOOL,
		NULL,NULL,&DMEndTx[2],0,NULL,50,DMHandler},
	{ &DMCurr[2], DMLEFT+250,DMBASE+10,72,8,GADGHCOMP,DMACTIVE,DMBOOL,
		NULL,NULL,&DMEndTx[2],0,NULL,51,DMHandler},
	{ &DMCurr[3], DMLEFT+250,DMBASE+20,72,8,GADGHCOMP,DMACTIVE,DMBOOL,
		NULL,NULL,&DMEndTx[2],0,NULL,52,DMHandler},
	{ &DMCurr[4], DMLEFT+250,DMBASE+35,72,8,GADGHCOMP,DMACTIVE,DMBOOL,
		NULL,NULL,&DMEndTx[2],0,NULL,53,DMHandler},
	{ &DMCurr[5], DMLEFT+250,DMBASE+45,72,8,GADGHCOMP,DMACTIVE,DMBOOL,
		NULL,NULL,&DMEndTx[2],0,NULL,54,DMHandler},
	{ &DMCurr[6], DMLEFT+250,DMBASE+55,72,8,GADGHCOMP,DMACTIVE,DMBOOL,
		NULL,NULL,&DMEndTx[2],0,NULL,55,DMHandler},
	{ &DMCurr[7], DMLEFT+250,DMBASE+70,72,8,GADGHCOMP,DMACTIVE,DMBOOL,
		NULL,NULL,&DMEndTx[2],0,NULL,56,DMHandler},
	{ &DMCurr[8], DMLEFT+250,DMBASE+80,72,8,GADGHCOMP,DMACTIVE,DMBOOL,
		NULL,NULL,&DMEndTx[2],0,NULL,57,DMHandler},
	{ &DMCurr[9], DMLEFT+250,DMBASE+90,72,8,GADGHCOMP,DMACTIVE,DMBOOL,
		NULL,NULL,&DMEndTx[2],0,NULL,58,DMHandler},
	{ &DMEnders[0], DMLEFT+250,DMBASE+100,72,8,GADGHCOMP,DMACTIVE,DMBOOL,
		NULL,NULL,&DMEndTx[2],0,NULL,59,DMHandler}};

struct Gadget DMList[] = {
	{ &DMList[1], DMLEFT,DMBASE+0,240,8,GADGHCOMP,DMACTIVE,DMBOOL,
		NULL,NULL,&DMAttrText[0],0,NULL,30,DMHandler},
	{ &DMList[2], DMLEFT,DMBASE+10,240,8,GADGHCOMP,DMACTIVE,DMBOOL,
		NULL,NULL,&DMAttrText[1],0,NULL,31,DMHandler},
	{ &DMList[3], DMLEFT,DMBASE+20,240,8,GADGHCOMP,DMACTIVE,DMBOOL,
		NULL,NULL,&DMAttrText[2],0,NULL,32,DMHandler},
	{ &DMList[4], DMLEFT,DMBASE+35,240,8,GADGHCOMP,DMACTIVE,DMBOOL,
		NULL,NULL,&DMAttrText[3],0,NULL,33,DMHandler},
	{ &DMList[5], DMLEFT,DMBASE+45,240,8,GADGHCOMP,DMACTIVE,DMBOOL,
		NULL,NULL,&DMAttrText[4],0,NULL,34,DMHandler},
	{ &DMList[6], DMLEFT,DMBASE+55,240,8,GADGHCOMP,DMACTIVE,DMBOOL,
		NULL,NULL,&DMAttrText[5],0,NULL,35,DMHandler},
	{ &DMList[7], DMLEFT,DMBASE+70,240,8,GADGHCOMP,DMACTIVE,DMBOOL,
		NULL,NULL,&DMAttrText[6],0,NULL,36,DMHandler},
	{ &DMList[8], DMLEFT,DMBASE+80,240,8,GADGHCOMP,DMACTIVE,DMBOOL,
		NULL,NULL,&DMAttrText[7],0,NULL,37,DMHandler},
	{ &DMList[9], DMLEFT,DMBASE+90,240,8,GADGHCOMP,DMACTIVE,DMBOOL,
		NULL,NULL,&DMAttrText[8],0,NULL,38,DMHandler},
	{ &DMCurr[0], DMLEFT,DMBASE+100,240,8,GADGHCOMP,DMACTIVE,DMBOOL,
		NULL,NULL,&DMAttrText[9],0,NULL,39,DMHandler}};

LOCAL struct IntuiText DMHeadings[] = {
 { 3,1,JAM2,5,4,NULL,"Dimensioning Customization Settings:",&DMHeadings[1]},
 { 2,1,JAM2,10,DMBASE    ,NULL," Witness Layer:",&DMHeadings[2] },
 { 2,1,JAM2,10,DMBASE+ 10,NULL," Witness Group:",&DMHeadings[3] },
 { 2,1,JAM2,10,DMBASE+ 20,NULL,"   Witness Pen:",&DMHeadings[4] },
 { 2,1,JAM2,10,DMBASE+ 35,NULL,"Dim Line Layer:",&DMHeadings[5] },
 { 2,1,JAM2,10,DMBASE+ 45,NULL,"Dim Line Group:",&DMHeadings[6] },
 { 2,1,JAM2,10,DMBASE+ 55,NULL,"  Dim Line Pen:",&DMHeadings[7] },
 { 2,1,JAM2,10,DMBASE+ 70,NULL," Dim Box Style:",&DMHeadings[8] },
 { 2,1,JAM2,10,DMBASE+ 80,NULL," Dim Box Layer:",&DMHeadings[9] },
 { 2,1,JAM2,10,DMBASE+ 90,NULL," Dim Box Group:",&DMHeadings[10] },
 { 2,1,JAM2,10,DMBASE+100,NULL,"   Dim Box Pen:",&DMHeadings[11] },
 { 2,1,JAM2,70,DMBASE+115,NULL,"Witness Pick Offset:",&DMHeadings[12] },
 { 2,1,JAM2,70,DMBASE+125,NULL,"  Witness Overshoot:",&DMHeadings[13] },
 { 2,1,JAM2,70,DMBASE+140,NULL,"    Dim Line Offset:",&DMHeadings[14] },
 { 2,1,JAM2,70,DMBASE+150,NULL,"     Dim Box Offset:",&DMHeadings[15] },
 { 2,1,JAM2,20,DMBASE+170,NULL,"        Fixed Angle:",&DMHeadings[16] },
 { 2,1,JAM2,20,DMBASE+180,NULL,"    Flip From Angle:",&DMHeadings[17] },
 { 2,1,JAM2,20,DMBASE+190,NULL,"      Flip To Angle:",NULL }};

VOID MungText(donum)
int donum;
{
	UBYTE *dbuf,*outstr,*incbuf,db[55],centerbuf[55];
	int numspa;
	BOOL dv;

	switch (donum) {
	case 0:	
		strcpy(db,Wit_Layer->Layer_Name);
		dv = B_Wit_Layer;
		break;
	case 1:	
		strcpy(db,Wit_Group->Group_Name);
		dv = B_Wit_Group;
		break;
	case 2:	
		strcpy(db,Wit_Pen->Pen_Name);
		dv = B_Wit_Pen;
		break;
	case 3:	
		strcpy(db,Dim_Line_Layer->Layer_Name);
		dv = B_Dim_Line_Layer;
		break;
	case 4:	
		strcpy(db,Dim_Line_Group->Group_Name);
		dv = B_Dim_Line_Group;
		break;
	case 5:	
		strcpy(db,Dim_Line_Pen->Pen_Name);
		dv = B_Dim_Line_Pen;
		break;
	case 6:	
		strcpy(db,Dim_Box_Style->Letter_Style_ID);
		dv = B_Dim_Box_Style;
		break;
	case 7:	
		strcpy(db,Dim_Box_Layer->Layer_Name);
		dv = B_Dim_Box_Layer;
		break;
	case 8:	
		strcpy(db,Dim_Box_Group->Group_Name);
		dv = B_Dim_Box_Group;
		break;
	case 9:	
		strcpy(db,Dim_Box_Pen->Pen_Name);
		dv = B_Dim_Box_Pen;
		break;
	}		

	if (!dv) {
		DMCurr[donum].GadgetText = &DMEndTx[3];
		DMAttrText[donum].FrontPen = 2;
		DMAttrText[donum].BackPen = 3;
		strcpy(db,"<<<< Use Current >>>>");
	} else {
		DMCurr[donum].GadgetText = &DMEndTx[2];
		DMAttrText[donum].FrontPen = 3;
		DMAttrText[donum].BackPen = 2;
	}
	
	outstr = &DMAttrs[donum][0];

	dbuf = db;
	numspa = (30 - strlen(dbuf))/2;
	for (incbuf = centerbuf; numspa>=0; numspa--,incbuf++) 
			*incbuf=' ';
	sprintf(incbuf,"%-30.30s",dbuf);
	sprintf(outstr,"%-30.30s",centerbuf);

	return;
}

VOID DMDisp(gad)
int gad;
{

	int gpos;

	switch (gad) {
	case 0: sprintf(&DMData[0][0],"%s",DispLin(Wit_Pick));	break;
	case 1: sprintf(&DMData[1][0],"%s",DispLin(Wit_Over));	break;
	case 2: sprintf(&DMData[2][0],"%s",DispLin(Dim_Line_Offset));	break;
	case 3: sprintf(&DMData[3][0],"%s",DispLin(Dim_Box_Offset));	break;
	case 4: sprintf(&DMData[4][0],"%s",DispAng(Fixed_Angle));	break;
	case 5: sprintf(&DMData[5][0],"%s",DispAng(Free_Flip_From));	break;
	case 6: sprintf(&DMData[6][0],"%s",DispAng(Free_Flip_To));	break;
	}

	gpos = RemoveGList(w,&DMEnders[3+gad],1);
	if (gpos>=0) AddGList(w,&DMEnders[3+gad],gpos,1,&DimsRequester);
	RefreshGList(&DMEnders[3+gad],w,&DimsRequester,1);
}

VOID DMStringHandler(ingad)
struct Gadget *ingad;
{

	STDFLT inans;
	BOOL   inchk;
	int gpos,gnum;
	gnum = ingad->GadgetID;

	switch (gnum) {
	case 30:
	case 31:
	case 32:
	case 33:
		inans = ParLin(&DMData[gnum-30],&inchk);
		break;
	case 34:
	case 35:
	case 36:
		inans = ParAng(&DMData[gnum-30],&inchk);
		break;
	}

	if (inchk) {
		DMDisp(gnum-30);
		return;
	}

	switch (gnum) {
	case 30: Wit_Pick = inans;	break;
	case 31: Wit_Over = inans;	break;
	case 32: Dim_Line_Offset = inans;	break;
	case 33: Dim_Box_Offset = inans;	break;
	case 34: Fixed_Angle = inans;	break;
	case 35: Free_Flip_From = inans;	break;
	case 36: Free_Flip_To = inans;	break;
	}
	
	DMDisp(gnum-30);

}
	
VOID SetButton()
{
	int gpos[3];

	gpos[0] = RemoveGList(w,&DMEnders[7],1);
	gpos[1] = RemoveGList(w,&DMEnders[8],1);
	gpos[2] = RemoveGList(w,&DMEnders[9],1);

	if (DMEnders[2].Flags&SELECTED) {
		Use_Fixed_Angle = FALSE;
		DMEnders[7].Flags |= GADGDISABLED;
		DMEnders[8].Flags &= ~GADGDISABLED;
		DMEnders[9].Flags &= ~GADGDISABLED;
	} else	{
		Use_Fixed_Angle = TRUE;
		DMEnders[7].Flags &= ~GADGDISABLED;
		DMEnders[8].Flags |= GADGDISABLED;
		DMEnders[9].Flags |= GADGDISABLED;
	}

	if (gpos[0]>=0) AddGList(w,&DMEnders[7],gpos[0],1,&DimsRequester);
	if (gpos[1]>=0) AddGList(w,&DMEnders[8],gpos[1],1,&DimsRequester);
	if (gpos[2]>=0) AddGList(w,&DMEnders[9],gpos[2],1,&DimsRequester);

	RefreshGList(&DMEnders[2],w,&DimsRequester,1);
	RefreshGList(&DMEnders[7],w,&DimsRequester,1);
	RefreshGList(&DMEnders[8],w,&DimsRequester,1);
	RefreshGList(&DMEnders[9],w,&DimsRequester,1);
}

VOID DMHandler(ingad)
struct Gadget *ingad;
{

	int gpos,gnum;
	gnum = ingad->GadgetID;

	if (gnum==25) {
		SetButton();
		return;
	}

	if (gnum>=50) {

#define tog(a) a = ! a

		switch (gnum) {
		case 50:	tog(B_Wit_Layer);	break;
		case 51:	tog(B_Wit_Group);	break;
		case 52:	tog(B_Wit_Pen);		break;
		case 53:	tog(B_Dim_Line_Layer);	break;
		case 54:	tog(B_Dim_Line_Group);	break;
		case 55:	tog(B_Dim_Line_Pen);	break;
		case 56:	tog(B_Dim_Box_Style);	break;
		case 57:	tog(B_Dim_Box_Layer);	break;
		case 58:	tog(B_Dim_Box_Group);	break;
		case 59:	tog(B_Dim_Box_Pen);	break;
		}

		MungText(gnum-50);
		RefreshGList(&DMList[gnum-50],w,&DimsRequester,1);
		RefreshGList(&DMCurr[gnum-50],w,&DimsRequester,1);

		return;
	}

	CloseRequest(&DimsRequester,w);	/* Close manually */

	if (gnum>=30) {
		postaction = gnum;
		return;
	}

	switch (gnum) {
	case 20:
		exflag=FALSE;
		postaction = 0;
		return;
	case 21:
		exflag=FALSE;
		postaction = 1;
		return;
	}

	return;
 
}

Dims_Requester()
{

	LAYER_A *tlayer;
	GROUP_A *tgroup,*bgroup;
	PEN_A *tpen;
	STYLE_A *tstyle;
	int a;

	LAYER_A	*S_Wit_Layer;	
	GROUP_A	*S_Wit_Group;
	PEN_A	*S_Wit_Pen;
	LAYER_A	*S_Dim_Line_Layer;
	GROUP_A	*S_Dim_Line_Group;
	PEN_A	*S_Dim_Line_Pen;
	STYLE_A	*S_Dim_Box_Style;
	LAYER_A	*S_Dim_Box_Layer;
	GROUP_A	*S_Dim_Box_Group;
	PEN_A	*S_Dim_Box_Pen;

	BOOL	S_Use_Fixed_Angle;
	STDFLT	S_Wit_Pick;
	STDFLT	S_Wit_Over;
	STDFLT	S_Dim_Line_Offset;
	STDFLT	S_Dim_Box_Offset;
	STDFLT	S_Fixed_Angle;
	STDFLT	S_Free_Flip_From;
	STDFLT	S_Free_Flip_To;
	
	struct RastPort *reqrp;

	S_Use_Fixed_Angle = Use_Fixed_Angle;
	S_Wit_Pick = Wit_Pick;
	S_Wit_Over = Wit_Over;
	S_Dim_Line_Offset = Dim_Line_Offset;
	S_Dim_Box_Offset = Dim_Box_Offset;
	S_Fixed_Angle = Fixed_Angle;
	S_Free_Flip_From = Free_Flip_From;
	S_Free_Flip_To = Free_Flip_To;

	S_Wit_Layer = Wit_Layer;
	S_Wit_Group = Wit_Group;
	S_Wit_Pen = Wit_Pen;
	S_Dim_Line_Layer = Dim_Line_Layer;
	S_Dim_Line_Group = Dim_Line_Group;
	S_Dim_Line_Pen = Dim_Line_Pen;
	S_Dim_Box_Style = Dim_Box_Style;
	S_Dim_Box_Layer = Dim_Box_Layer;
	S_Dim_Box_Group = Dim_Box_Group;
	S_Dim_Box_Pen = Dim_Box_Pen;
	exflag = TRUE;

	while (exflag) {

		InitRequester(&DimsRequester);

		DimsRequester.Width=465;
		DimsRequester.Height=DMBOTTOM;
		DimsRequester.LeftEdge=(XSsize-DimsRequester.Width)/2;
		DimsRequester.TopEdge=(YSsize-DimsRequester.Height)/2;

		DimsRequester.BackFill=1;

		DimsRequester.ReqText = &DMHeadings[0];
		DimsRequester.ReqGadget = &DMList[0];

		for (a=0; a<10; a++)
			MungText(a);

		DMEnders[2].Flags &= ~SELECTED;
		if (!Use_Fixed_Angle) DMEnders[2].Flags |= SELECTED;

#ifdef _TURNSY
		DimsRequester.Flags = NOISYREQ;
#endif

		SafeRequest(&DimsRequester, w);

		for (a=0; a<7; a++) DMDisp(a);
		SetButton();

		postaction = 0;

		reqrp = DimsRequester.ReqLayer->rp;	/* Nasty */

		DoBorders(&DimsRequester,15,0,0);

		SetAPen(reqrp, 3);
		SetBPen(reqrp, 3);
		SetOPen(reqrp, 2);
		RectFill(reqrp, 280,DMBASE+160,280+47+50,DMBASE+165+36+5); 

		RefreshGList(&DMEnders[2],w,&DimsRequester,1);

		sit_n_spin(&DimsRequester,0,&DMEnders[0],&DMEnders[1]);
			/* Wait for requesters to exit */

		switch (postaction) {
		case 0:
			Use_Fixed_Angle = S_Use_Fixed_Angle;
			Wit_Pick = S_Wit_Pick;
			Wit_Over = S_Wit_Over;
			Dim_Line_Offset = S_Dim_Line_Offset;
			Dim_Box_Offset = S_Dim_Box_Offset;
			Fixed_Angle = S_Fixed_Angle;
			Free_Flip_From = S_Free_Flip_From;
			Free_Flip_To = S_Free_Flip_To;

			Wit_Layer = S_Wit_Layer;
			Wit_Group = S_Wit_Group;
			Wit_Pen = S_Wit_Pen;
			Dim_Line_Layer = S_Dim_Line_Layer;
			Dim_Line_Group = S_Dim_Line_Group;
			Dim_Line_Pen = S_Dim_Line_Pen;
			Dim_Box_Style = S_Dim_Box_Style;
			Dim_Box_Layer = S_Dim_Box_Layer;
			Dim_Box_Group = S_Dim_Box_Group;
			Dim_Box_Pen = S_Dim_Box_Pen;
			break;
		case 1:
			break;
		case 30:
		case 33:
		case 37:
			if (!Layer_NewCurrent()) break;
			tlayer = (LAYER_A *) scrollanswer;
			if (tlayer!=NULL) {
			    if (postaction==30) Wit_Layer = tlayer;
			    if (postaction==33) Dim_Line_Layer = tlayer;
			    if (postaction==37) Dim_Box_Layer = tlayer;
			}
			break;
		case 31:
		case 34:
		case 38:
			bgroup = Current_Group;
			if (!Group_GetCurrent()) {
				Current_Group = bgroup;
				setpath();
				break;
			}
			tgroup = Current_Group;
			if (tgroup!=NULL) {
			    if (postaction==31) Wit_Group = tgroup;
			    if (postaction==34) Dim_Line_Group = tgroup;
			    if (postaction==38) Dim_Box_Group = tgroup;
			}
			Current_Group = bgroup;
			setpath();
			break;
		case 32:
		case 35:
		case 39:
			if (!Pen_NewCurrent()) break;
			tpen = (PEN_A *) scrollanswer;
			if (tpen!=NULL) {
			    if (postaction==32) Wit_Pen = tpen;
			    if (postaction==35) Dim_Line_Pen = tpen;
			    if (postaction==39) Dim_Box_Pen = tpen;
			}
			break;
		case 36:
			if (!Style_NewCurrent()) break;
			tstyle = (STYLE_A *) scrollanswer;
			if (tstyle!=NULL) Dim_Box_Style = tstyle;
			break;

		default:
			break;
		}
				
	}

}
