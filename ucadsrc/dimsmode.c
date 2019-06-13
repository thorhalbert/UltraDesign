/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	dimsmode - Dimensioning User Interface
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
 *	1	11-Jan-87 SH	Beginning of history
 *	2	 9-May-87 SH	Setup new 3d icons
 *	3	29-Nov-87 SH	Convert over to dimensioning mode
 *	4	30-Jan-88 SH	Add the dimension requester call
 *	5	 8-Feb-88 SH	Double click on requester causes crash,
 *				 put in sempophore to keep from being
 *				 multiply called
 *	6	19-Apr-90 SH	Abort after first witness point would
 *				 not clear properly 
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*--------------------------------Macros-------------------------------------*/

#define TOP_USERWINDOW (YSsize-BOTTOMAREA)

#define DIMS_CLASS	40

#define GAD_WITTOP	30
#define GAD_DIMLIN	31
#define GAD_WITBOT	32
#define GAD_BOXINL	33
#define GAD_BOXOUT	34
#define GAD_VLOCK	35
#define GAD_HLOCK	36
#define GAD_ELOCK	37
#define GAD_RENDER	40

#define GADS_TOPWIT	50
#define GADS_TOPARROW	51
#define GADS_BOX	52
#define GADS_BOTARROW	53
#define GADS_BOTWIT	54

/*-----------------------------Forward References----------------------------*/

VOID PerformDims();

/*-------------------------External Symbol Definitions-----------------------*/

extern struct Image dims_1s_image;
extern struct Image dims_1u_image;
extern struct Image dims_2s_image;
extern struct Image dims_2u_image;
extern struct Image dims_3s_image;
extern struct Image dims_3u_image;
extern struct Image dims_4s_image;
extern struct Image dims_4u_image;
extern struct Image dims_5s_image;
extern struct Image dims_5u_image;
extern struct Image dims_6s_image;
extern struct Image dims_6u_image;
extern struct Image dims_7s_image;
extern struct Image dims_7u_image;
extern struct Image dims_8s_image;
extern struct Image dims_8u_image;
extern struct Image dims_9s_image;
extern struct Image dims_9u_image;

extern struct Image dimsb_1s_image;
extern struct Image dimsb_1u_image;
extern struct Image dimsb_2s_image;
extern struct Image dimsb_2u_image;
extern struct Image dimsb_3s_image;
extern struct Image dimsb_3u_image;
extern struct Image dimsb_4s_image;
extern struct Image dimsb_4u_image;
extern struct Image dimsb_5s_image;
extern struct Image dimsb_5u_image;

/*-------------------------Global Variable Definitions-----------------------*/

WORD	DimActivator=GAD_WITTOP;
BOOL	dimsgadgetssetup=FALSE;
BOOL 	DimsSetup = FALSE;
STDFLT	dim_box_ang;

UBYTE	DimText[30];

LOCAL STDFLT wittopx,wittopy,witbotx,witboty,linx1,liny1,linx2,liny2,boxx,boxy;
LOCAL STDFLT tboxx,tboxy,witlen;
LOCAL STDFLT swittopx,swittopy,switbotx,switboty,slinx1,sliny1,slinx2,sliny2,
		sboxx,sboxy;
LOCAL	BOOL	topwitset=FALSE,botwitset=FALSE,linset=FALSE,boxfree=FALSE;
LOCAL	BOOL	gotlin1=FALSE,gotlin2=FALSE;
LOCAL	BOOL	vlock=FALSE,hlock=FALSE,elock=TRUE;
LOCAL 	STDFLT	witang,wittan,witperp;
LOCAL	BOOL	witsingul;

extern BOOL radialpoint;
extern STDFLT radialx,radialy;
extern SHORT iradialx,iradialy;

/*---------------------------------Code Section------------------------------*/

VOID DeDimsShadow();
VOID AttemptDimsRender();
VOID DoBox();
VOID AttemptBoxRender();
BOOL FindIntercept();

VOID DimsClear()
{

	DeDimsShadow();
}
	
VOID GetRidOfDimsMode()
{

	DeDimsShadow();

	GI_DeactivateBGadget(DIMS_CLASS,0,NULL);	

	zapwindow();

	DimsSetup = FALSE;
}

VOID CalDimPerp()
{

	STDFLT fx1,fx2,fy1,fy2,fx3,fy3,m0,m1,m02,b0,b1,t1,t2;

	fx1 = wittopx;
	fy1 = wittopy;

	fx2 = linx1;
	fy2 = liny1;

	fx3 = witbotx;
	fy3 = witboty;

	t1 = (fy1-fy2);
	t2 = (fx1-fx2);

	if (t1==0.0&&t2==0.0) return;

	if (t1==0.0) {
		linx2 = fx2;
		liny2 = fy3;
		gotlin2=TRUE;
		return;
	}
	if (t2==0.0) {
		linx2 = fx3;
		liny2 = fy2;
		gotlin2=TRUE;
		return;
	}

	m0 = t1 / t2;
	m0 = -(1.0 / m0);	/* Perp to other line */
	m1 = -(1.0 / m0);
	b0 = fy2 - (m0 * fx2);
	b1 = fy3 - (m1 * fx3);
	m02 = m0 * m0;
	liny2 = ((b1 * m02) + b0) / (1.0 + m02);
	linx2 = (liny2 - b0) / m0;
	gotlin2=TRUE;

}

VOID CalWitAng()
{
	
	STDFLT fang;
	STDFLT dx,dy;
	
	dx = linx1 - wittopx;
	dy = liny1 - wittopy;
	
	if (abs(wittan)<0.5) {	/* More mouse resolution in x */
	
		if (dx==0.0) {
			liny1 = wittopy;
			return;
		}
		
		liny1 = wittopy + (wittan * dx);
		return;
	}
	
	if (witsingul) {	/* Vertical line */
	
		linx1 = wittopx;
		return;
	}
	
	linx1 = wittopx + (dy / wittan);
}


VOID HandleDimDummy(ingad)
struct GI_bgadget *ingad;
{

	return;
	
}
VOID HandleVLock(ingad)
struct GI_bgadget *ingad;
{

	if (!GI_GetBGadget(0,0,ingad)) {
		vlock = FALSE;
		return;
	}

	vlock = TRUE;
	hlock = FALSE;
	elock = FALSE;
	GI_SetBGadget(DIMS_CLASS,GAD_VLOCK,NULL,vlock);
	GI_SetBGadget(DIMS_CLASS,GAD_HLOCK,NULL,hlock);
	GI_SetBGadget(DIMS_CLASS,GAD_ELOCK,NULL,elock);
	return;
	
}

VOID HandleHLock(ingad)
struct GI_bgadget *ingad;
{

	if (!GI_GetBGadget(0,0,ingad)) {
		hlock = FALSE;
		return;
	}

	hlock = TRUE;
	vlock = FALSE;
	elock = FALSE;
	GI_SetBGadget(DIMS_CLASS,GAD_VLOCK,NULL,vlock);
	GI_SetBGadget(DIMS_CLASS,GAD_HLOCK,NULL,hlock);
	GI_SetBGadget(DIMS_CLASS,GAD_ELOCK,NULL,elock);
	return;
	
}

VOID HandleELock(ingad)
struct GI_bgadget *ingad;
{

	if (!GI_GetBGadget(0,0,ingad)) {
		elock = FALSE;
		return;
	}

	elock = TRUE;
	vlock = FALSE;
	hlock = FALSE;
	GI_SetBGadget(DIMS_CLASS,GAD_VLOCK,NULL,vlock);
	GI_SetBGadget(DIMS_CLASS,GAD_HLOCK,NULL,hlock);
	GI_SetBGadget(DIMS_CLASS,GAD_ELOCK,NULL,elock);
	return;
	
}

BOOL InReq=FALSE;
VOID HandleDefs(ingad)
struct GI_bgadget *ingad;
{

	if (InReq) return;
	if (ingad==NULL) return;
	InReq = TRUE;

	Dims_Requester();

	InReq = FALSE;	
	return;
	
}

VOID dimemp()
{
	if (!(topwitset&&botwitset&&linset)) 
		MainMess(DIM_MESS_FINISH);
	else	MainMess(DIM_MESS_REND);
}

VOID HandleDimAct(ingad)
struct GI_bgadget *ingad;
{

	radialpoint = FALSE;
	iradialx = iradialy = 0;

	if (ingad->GadgetID==DimActivator) {
		GI_SetBGadget(DIMS_CLASS,0,ingad,FALSE);
		GI_RefreshGadgets();
		dimemp();
		DimActivator=999;
		return;
	}

	GI_SetBGadget(DIMS_CLASS,DimActivator,NULL,FALSE);

	if ((!topwitset)||(ingad->GadgetID==GAD_WITTOP)) {

		if (ingad->GadgetID!=GAD_WITTOP)
			GI_SetBGadget(DIMS_CLASS,0,ingad,FALSE);

		DimActivator=GAD_WITTOP;
	
		GI_SetBGadget(DIMS_CLASS,DimActivator,NULL,TRUE);

		GI_RefreshGadgets();

		MainMess(DIM_MESS_WTOP);

		return;
	}

	if ((!botwitset)||(ingad->GadgetID==GAD_WITBOT)) {
			
		if (ingad->GadgetID!=GAD_WITBOT)
			GI_SetBGadget(DIMS_CLASS,0,ingad,FALSE);

		DimActivator=GAD_WITBOT;
	
		GI_SetBGadget(DIMS_CLASS,DimActivator,NULL,TRUE);

		MainMess(DIM_MESS_WBOT);

		GI_RefreshGadgets();

		radialx = wittopx;
		radialy = wittopy;
		radialpoint = TRUE;
		iradialx = iradialy = 0;
		
		return;
	}

	if ((!linset)||(ingad->GadgetID==GAD_DIMLIN)) {
			
		if (ingad->GadgetID!=GAD_DIMLIN)
			GI_SetBGadget(DIMS_CLASS,0,ingad,FALSE);

		DimActivator=GAD_DIMLIN;
	
		GI_SetBGadget(DIMS_CLASS,DimActivator,NULL,TRUE);

		MainMess(DIM_MESS_DIML);

		GI_RefreshGadgets();

		return;
	}

	DimActivator=ingad->GadgetID;

	switch (DimActivator) {
	case	GAD_BOXINL:
		boxfree = FALSE;
		MainMess(DIM_MESS_PBOX);
		break;
	case	GAD_BOXOUT:
		boxfree = TRUE;
		MainMess(DIM_MESS_PBOX);
		break;
	}

	GI_SetBGadget(DIMS_CLASS,DimActivator,NULL,TRUE);

	GI_RefreshGadgets();

	return;
}

STDFLT boxc[2][4],sboxc[2][4];

VOID caltbox()
{

	UBYTE *inc;
	STDFLT *sizar,height,temp_acos,temp_asin,boxw;
	STDFLT x,y,ll,lr;
	STYLE_A *boxstyle;
	BOOL invert;

	if (Use_Fixed_Angle) dim_box_ang = Fixed_Angle;
	else {
		invert = FALSE;
		dim_box_ang = atan2(liny1-liny2,linx1-linx2)+PI;
		if (Free_Flip_To<Free_Flip_From) {
			if (dim_box_ang<=Free_Flip_To||
			    dim_box_ang>=Free_Flip_From) invert=TRUE;
		} else {
			if (dim_box_ang>=Free_Flip_From&&
			    dim_box_ang<=Free_Flip_To) invert=TRUE;
		}
		if (invert) dim_box_ang += PI;
		normalize(dim_box_ang);
	}


	boxstyle = Dim_Box_Style;
	if (boxstyle==NULL) boxstyle = Current_Style;

	boxstyle->Slant = boxstyle->Slant;
	height = boxstyle->Prime_Height;

	temp_acos = cos(dim_box_ang);
	temp_asin = sin(dim_box_ang);

	inc = DimText;

	boxw = 0.0;

	sizar = boxstyle->draw_font->font_link->Char_Widths;

	while (*inc) {
		
		boxw += sizar[(int) *inc];
		inc++;
	}

	boxw *= height;
	boxw *= boxstyle->Aspect;

	ll = - boxw/2.0;
	lr = - ll;
	
	y = -height/2.0;		/* Left bottom */
	x = ll + (boxstyle->Slant*y);

	boxc[0][0] = x*temp_acos-y*temp_asin;
	boxc[1][0] = x*temp_asin+y*temp_acos;

	y = height/2.0;			/* Left top */
	x = ll + (boxstyle->Slant*y);
	
	boxc[0][1] = x*temp_acos-y*temp_asin;
	boxc[1][1] = x*temp_asin+y*temp_acos;

	y = height/2.0;			/* Right top */
	x = lr + (boxstyle->Slant*y);
	
	boxc[0][2] = x*temp_acos-y*temp_asin;
	boxc[1][2] = x*temp_asin+y*temp_acos;

	y = -height/2.0;		/* Right bottom */
	x = lr + (boxstyle->Slant*y);
	
	boxc[0][3] = x*temp_acos-y*temp_asin;
	boxc[1][3] = x*temp_asin+y*temp_acos;

	y = -height/2.0;		/* Bottom of text box */
	x = 0.0;
	
	tboxx = x*temp_acos-y*temp_asin+boxx;
	tboxy = x*temp_asin+y*temp_acos+boxy;

	return;
}

int inter=0;
STDFLT	cepts[2][2];

VOID tryline(l1,l2)
int l1,l2;
{

	STDFLT gex,gey;

	if (!FindIntercept(boxc[0][l1]+boxx,boxc[1][l1]+boxy,
			   boxc[0][l2]+boxx,boxc[1][l2]+boxy,
		linx1,liny1,linx2,liny2,&gex,&gey)) return;

	if (inter>=1) 
		if (cepts[0][0]==gex&&cepts[1][0]==gey) return;

	if (inter>=2) return;		/* No can do */

	cepts[0][inter] = gex;
	cepts[1][inter] = gey;

	inter++;
}

VOID trailline(xi,yi)
STDFLT xi,yi;
{

	int minset,i;
	STDFLT boxm[2][4],mindis,tdis;

#define cdefs(i,k) 	boxm[0][i] = boxx+(boxc[0][i] + boxc[0][k])/2; \
			boxm[1][i] = boxy+(boxc[1][i] + boxc[1][k])/2

	cdefs(0,1);
	cdefs(1,2);
	cdefs(2,3);
	cdefs(3,0);

	mindis = MAXBIG;
	for (i=0; i<4; i++) {
		tdis = hypot(boxm[0][i]-xi,boxm[1][i]-yi);
		if (tdis<mindis) {
			mindis = tdis;
			minset = i;
		}
	}

	CreateLine(xi,yi,boxm[0][minset],boxm[1][minset],0);

	return;
}

VOID DimLine(inlayer,ingroup,inpen,x1,y1,x2,y2,p1off,p2off,linflg,boxrdr)
LAYER_A *inlayer;
GROUP_A *ingroup;
PEN_A *inpen;
STDFLT x1,y1,x2,y2,p1off,p2off;
int linflg;
BOOL boxrdr;
{

	LAYER_A *save_layer;
	GROUP_A *save_group;
	PEN_A *save_pen;

	STDFLT	dangle,dx1,dy1,dx2,dy2,df1,df2,dln;

	save_layer = Current_Layer;
	save_group = Current_Group;
	save_pen = Current_Pen;

	df1 = y2 - y1;
	df2 = x2 - x1;

	if (df1==0.0&&df2==0.0) return;		/* No line at all */

	dangle = atan2(df1,df2);
	dln = hypot(df2,df1);
	
	dx1 = cos(dangle) * p1off;
	dy1 = sin(dangle) * p1off;

	dx2 = cos(dangle) * (dln + p2off);
	dy2 = sin(dangle) * (dln + p2off);

	if (inlayer!=NULL) Current_Layer = inlayer;
	if (ingroup!=NULL) Current_Group = ingroup;
	if (inpen!=NULL) Current_Pen = inpen;

	CreateLine(dx1+x1,dy1+y1,dx2+x1,dy2+y1,linflg);
	if (boxrdr) trailline(dx2+x1,dy2+y1); 

	Current_Layer = save_layer;
	Current_Group = save_group;
	Current_Pen = save_pen;
}

VOID RestartDims()
{

	boxfree = topwitset = botwitset = linset = FALSE;
	radialpoint = FALSE;	/* edit 6 */
	gotlin1=gotlin2=FALSE;
	
	sprintf(pbuffer,"  ");
	paintinfo();

	GI_SetBGadget(DIMS_CLASS,DimActivator,NULL,FALSE);

	DimActivator=GAD_WITTOP;
	
	GI_SetBGadget(DIMS_CLASS,DimActivator,NULL,TRUE);
	MainMess(DIM_MESS_WTOP);

	GI_RefreshGadgets();

}
		
VOID DimHandleRender(ingad)
struct GI_bgadget *ingad;
{

	UBYTE *inc;
	int topclos,botclos;

	GROUP_A *save_group;
	LAYER_A *save_layer;
	PEN_A *save_pen;
	STYLE_A *save_style;

	save_group = Current_Group;
	save_layer = Current_Layer;
	save_pen = Current_Pen;
	save_style = Current_Style;

	DeDimsShadow();

	if (!(topwitset&&botwitset&&linset)) {
		InfoMess(DIM_MODE,DIM_MESS_NOTSET);
		return;
	}

	/* Render the thing */

	/* Calculate & draw top witness line */

	if (GI_GetBGadget(DIMS_CLASS,GADS_TOPWIT,NULL)) 
		DimLine(B_Wit_Layer?Wit_Layer:NULL,
			B_Wit_Group?Wit_Group:NULL,
			B_Wit_Pen?Wit_Pen:NULL,
			wittopx,wittopy,linx1,liny1,Wit_Pick,Wit_Over,0,FALSE);

	/* Calculate & draw bottom witness line */

	if (GI_GetBGadget(DIMS_CLASS,GADS_BOTWIT,NULL)) 
		DimLine(B_Wit_Layer?Wit_Layer:NULL,
			B_Wit_Group?Wit_Group:NULL,
			B_Wit_Pen?Wit_Pen:NULL,
			witbotx,witboty,linx2,liny2,Wit_Pick,Wit_Over,0,FALSE);

	/* Calculate box boundaries */

	caltbox();

	/* Box is fixed in line */

	if (!boxfree) {

		inter = 0;
		tryline(0,1);
		tryline(1,2);
		tryline(2,3);
		tryline(3,0);

		if (inter>=2) {		/* Otherwise - no-chance */

			if (hypot(linx1-cepts[0][0],liny1-cepts[1][0])<
			    hypot(linx1-cepts[0][1],liny1-cepts[1][1])) {
				topclos = 0;
				botclos = 1;
			} else {
				topclos = 1;
				botclos = 0;
			}
	
			/* Calculate & draw top dim line */

			if (GI_GetBGadget(DIMS_CLASS,GADS_TOPARROW,NULL)) 
			   DimLine(B_Dim_Line_Layer?Dim_Line_Layer:NULL,
	 			   B_Dim_Line_Group?Dim_Line_Group:NULL,
				   B_Dim_Line_Pen?Dim_Line_Pen:NULL,
		 	  	   linx1,liny1,cepts[0][topclos],
				   cepts[1][topclos],
			  	   Dim_Line_Offset,-Dim_Box_Offset,
				   LINE_P1ARROW,FALSE);

			/* Calculate & draw bottom dim line */	

			if (GI_GetBGadget(DIMS_CLASS,GADS_BOTARROW,NULL)) 
			   DimLine(B_Dim_Line_Layer?Dim_Line_Layer:NULL,
	 			   B_Dim_Line_Group?Dim_Line_Group:NULL,
				   B_Dim_Line_Pen?Dim_Line_Pen:NULL,
			  linx2,liny2,cepts[0][botclos],cepts[1][botclos],
			  Dim_Line_Offset,-Dim_Box_Offset,LINE_P1ARROW,FALSE);

		}

	/* Box is free from the line */
	} else {

		/* Calculate & draw top dim line */

		if (GI_GetBGadget(DIMS_CLASS,GADS_TOPARROW,NULL)) 
		   DimLine(B_Dim_Line_Layer?Dim_Line_Layer:NULL,
			   B_Dim_Line_Group?Dim_Line_Group:NULL,
			   B_Dim_Line_Pen?Dim_Line_Pen:NULL,
	 	  linx1,liny1,linx2,liny2,
		  -Dim_Line_Offset,-witlen-Arrow_Len*2.0,LINE_P1ARROW,TRUE);

		/* Calculate & draw bottom dim line */	

		if (GI_GetBGadget(DIMS_CLASS,GADS_BOTARROW,NULL)) 
		   DimLine(B_Dim_Line_Layer?Dim_Line_Layer:NULL,
			   B_Dim_Line_Group?Dim_Line_Group:NULL,
			   B_Dim_Line_Pen?Dim_Line_Pen:NULL,
	 	  linx2,liny2,linx1,liny1,
		  -Dim_Line_Offset,-witlen-Arrow_Len*2.0,LINE_P1ARROW,FALSE);
	}

	/* Calculate & draw text */

	if (GI_GetBGadget(DIMS_CLASS,GADS_BOX,NULL)) {
		if (B_Dim_Box_Layer) 	Current_Layer = Dim_Box_Layer;
		if (B_Dim_Box_Group) 	Current_Group = Dim_Box_Group;
		if (B_Dim_Box_Pen) 	Current_Pen = Dim_Box_Pen;
		if (B_Dim_Box_Style) 	Current_Style = Dim_Box_Style;

		CreateText(DimText,tboxx,tboxy,
			dim_box_ang/.01745329,CENTER_HANDLE,0,Current_Style);
	}

	Current_Group = save_group;
	Current_Layer = save_layer;
	Current_Pen = save_pen;
	Current_Style = save_style;

	RestartDims();
}

VOID Draw_Dims()
{

	int bl;

	MainMode(DIM_MODE);

	if (DimsSetup) return;

	if (WrapupDrawMode!=NULL &&
	    WrapupDrawMode != GetRidOfDimsMode) 
			(*WrapupDrawMode)();

	mouseaction=PerformDims;
	WrapupDrawMode=GetRidOfDimsMode;
	CleanupShadows=DimsClear;

	zapwindow();

	DimsSetup = TRUE;
	if (dimsgadgetssetup) {

		GI_ActivateBGadget(DIMS_CLASS,0,NULL,w);

		GI_RefreshGadgets();
	
		return;
	}

	bl = (YSsize - 305)/2;
	bl += 10;

	GI_BGadgetImage(3,bl,HandleDimAct,
		TRUE,DIMS_CLASS,GAD_WITTOP,FALSE,&dims_1u_image,
						      &dims_1s_image);

	GI_BGadgetImage(3,bl+28,HandleDimAct,
		TRUE,DIMS_CLASS,GAD_WITBOT,FALSE,&dims_3u_image,
						      &dims_3s_image);

	GI_BGadgetImage(3,bl+56,HandleDimAct,
		TRUE,DIMS_CLASS,GAD_DIMLIN,FALSE,&dims_2u_image,
						      &dims_2s_image);

	GI_BGadgetImage(3,bl+84,HandleDimAct,
		TRUE,DIMS_CLASS,GAD_BOXINL,FALSE,&dims_4u_image,
						      &dims_4s_image);

	GI_BGadgetImage(3,bl+112,HandleDimAct,
		TRUE,DIMS_CLASS,GAD_BOXOUT,FALSE,&dims_5u_image,
						      &dims_5s_image);

	GI_BGadgetImage(3,bl+140,HandleVLock,
		TRUE,DIMS_CLASS,GAD_VLOCK,FALSE,&dims_6u_image,
						      &dims_6s_image);

	GI_BGadgetImage(3,bl+168,HandleHLock,
		TRUE,DIMS_CLASS,GAD_HLOCK,FALSE,&dims_7u_image,
						      &dims_7s_image);

	GI_BGadgetImage(3,bl+196,HandleELock,
		TRUE,DIMS_CLASS,GAD_ELOCK,FALSE,&dims_8u_image,
						      &dims_8s_image);

	GI_BGadgetImage(3,bl+224,DimHandleRender,TRUE,DIMS_CLASS,
		GAD_RENDER,TRUE,&dims_9u_image,&dims_9s_image);

	GI_BGadgetImage(3,bl+256,HandleDimDummy,TRUE,DIMS_CLASS,
		GADS_TOPWIT,FALSE,&dimsb_1s_image,&dimsb_1u_image);
	GI_BGadgetImage(3,bl+263,HandleDimDummy,TRUE,DIMS_CLASS,
		GADS_TOPARROW,FALSE,&dimsb_2s_image,&dimsb_2u_image);
	GI_BGadgetImage(3,bl+276,HandleDimDummy,TRUE,DIMS_CLASS,
		GADS_BOX,FALSE,&dimsb_3s_image,&dimsb_3u_image);
	GI_BGadgetImage(3,bl+287,HandleDimDummy,TRUE,DIMS_CLASS,
		GADS_BOTARROW,FALSE,&dimsb_4s_image,&dimsb_4u_image);
	GI_BGadgetImage(3,bl+300,HandleDimDummy,TRUE,DIMS_CLASS,
		GADS_BOTWIT,FALSE,&dimsb_5s_image,&dimsb_5u_image);

	GI_SetBGadget(DIMS_CLASS,GAD_WITTOP,NULL,TRUE);
	MainMess(DIM_MESS_WTOP);

	GI_SetBGadget(DIMS_CLASS,GAD_DIMLIN,NULL,FALSE);
	GI_SetBGadget(DIMS_CLASS,GAD_WITBOT,NULL,FALSE);
	GI_SetBGadget(DIMS_CLASS,GAD_BOXINL,NULL,FALSE);
	GI_SetBGadget(DIMS_CLASS,GAD_BOXOUT,NULL,FALSE);

	GI_SetBGadget(DIMS_CLASS,GADS_TOPWIT,NULL,TRUE);
	GI_SetBGadget(DIMS_CLASS,GADS_TOPARROW,NULL,TRUE);
	GI_SetBGadget(DIMS_CLASS,GADS_BOX,NULL,TRUE);
	GI_SetBGadget(DIMS_CLASS,GADS_BOTARROW,NULL,TRUE);
	GI_SetBGadget(DIMS_CLASS,GADS_BOTWIT,NULL,TRUE);

	GI_SetBGadget(DIMS_CLASS,GAD_VLOCK,NULL,vlock);
	GI_SetBGadget(DIMS_CLASS,GAD_HLOCK,NULL,hlock);
	GI_SetBGadget(DIMS_CLASS,GAD_ELOCK,NULL,elock);
	
	GI_ActivateBGadget(DIMS_CLASS,0,NULL,w);

	GI_RefreshGadgets();

	dimsgadgetssetup=TRUE;

}

VOID PerformDims(sx,sy,action,xr,yr)
WORD sx,sy,action;
STDFLT xr,yr;
{

	switch (action) {
	case MOVEMENT:
	case UMOVEMENT:
		switch (DimActivator) {
 
		case GAD_WITTOP:
			wittopx = xr;
			wittopy = yr;

			AttemptDimsRender();

			return;

		case GAD_WITBOT:

			witbotx = xr;
			witboty = yr;

			AttemptDimsRender();
			
			return;

		case GAD_DIMLIN:

			linx1 = xr;
			liny1 = yr;
			if (vlock) liny1 = wittopy;
			if (hlock) linx1 = wittopx;

			gotlin1 = TRUE;

			AttemptDimsRender();
			
			return;

		case GAD_BOXINL:
			DoBox(xr,yr);
			AttemptBoxRender();
			return;

		case GAD_BOXOUT:

			boxx = xr;
			boxy = yr;

			AttemptBoxRender();

			return;

		}
		break;

	case LBUTTONDOWN:
		switch (DimActivator) {

		case GAD_WITTOP:

			wittopx=xr;
			wittopy=yr;

			topwitset=TRUE;

			DimActivator=999;
			if (!botwitset)	DimActivator=GAD_WITBOT;
	
			GI_SetBGadget(DIMS_CLASS,GAD_WITTOP,NULL,FALSE);

			if (DimActivator!=999) {
			    GI_SetBGadget(DIMS_CLASS,DimActivator,NULL,TRUE);
			    MainMess(DIM_MESS_WBOT);
				radialx = wittopx;
				radialy = wittopy;
				radialpoint = TRUE;
				iradialx = iradialy = 0;
			} else dimemp();

			GI_RefreshGadgets();

			AttemptDimsRender();
			
			return;

		case GAD_WITBOT:

			witbotx = xr;
			witboty = yr;
			
			witang = atan2(wittopy-witboty, wittopx-witbotx);
			witperp = witang + (PI/2.0);
				
			witsingul = FALSE;
	
			normalize(witperp);
	
			if (witperp!=(0.5*PI)||witperp!=(1.5*PI)) 
				wittan = tan(witperp);
			else witsingul = TRUE;
	
			if (witbotx==wittopx&&witboty==wittopy) {
				DisplayBeep(screen);
				break;
			}

			botwitset=TRUE;

			DimActivator=999;
			if (!linset)	DimActivator=GAD_DIMLIN;
	
			GI_SetBGadget(DIMS_CLASS,GAD_WITBOT,NULL,FALSE);

			if (DimActivator!=999) {
			    GI_SetBGadget(DIMS_CLASS,DimActivator,NULL,TRUE);
			    MainMess(DIM_MESS_DIML);
			} else dimemp();
			
			radialpoint = FALSE;
			iradialx = iradialy = 0;
			
			GI_RefreshGadgets();

			AttemptDimsRender();
			
			return;
		
		case GAD_DIMLIN:

			linx1 = xr;
			liny1 = yr;
			if (vlock) liny1 = wittopy;
			if (hlock) linx1 = wittopx;
			gotlin1 = TRUE;

			if (elock) CalWitAng();
			CalDimPerp();

			if (!gotlin2) { 
				DisplayBeep(screen);
				break;
			}

			linset = TRUE;
			
			GI_SetBGadget(DIMS_CLASS,DimActivator,NULL,FALSE);
			DimActivator = 999;
			MainMess(DIM_MESS_REND);

			GI_RefreshGadgets();

			AttemptDimsRender();
			caltbox();
			
			return;

		case GAD_BOXINL:
			DoBox(xr,yr);
			AttemptBoxRender();

			GI_SetBGadget(DIMS_CLASS,DimActivator,NULL,FALSE);
			DimActivator = 999;
			MainMess(DIM_MESS_REND);

			return;

		case GAD_BOXOUT:
			boxx = xr;
			boxy = yr;
			AttemptBoxRender();

			GI_SetBGadget(DIMS_CLASS,DimActivator,NULL,FALSE);
			DimActivator = 999;
			MainMess(DIM_MESS_REND);

			return;

		}
		break;

	case RBUTTONDOWN:
		RestartDims();
		break;


	}
}

VOID DoBox(xi,yi)
STDFLT xi,yi;
{
	
	STDFLT xd,yd,b,m,xmn,xmx,ymn,ymx;

	xmn = min(linx2,linx1);
	xmx = max(linx2,linx1);

	ymn = min(liny2,liny1);
	ymx = max(liny2,liny1);

	yd = liny1 - liny2;
	xd = linx1 - linx2;

	boxx = xi;
	boxy = yi;

	if (xd==0.0&&yd==0.0) return;

	if (xd==0.0) {
		boxx = linx1;
		if (boxy<ymn) boxy=ymn;
		if (boxy>ymx) boxy=ymx;
		return;
	}
	
	if (yd==0.0) {
		boxy = liny1;
		if (boxx<xmn) boxx=xmn;
		if (boxx>xmx) boxx=xmx;
		return;
	}
		
	m = yd/xd;
	b = liny1 - (m*linx1);

	if (abs(xd)>abs(yd)) 	boxy = (m*boxx)+b;
	else 			boxx = (boxy-b)/m;

	if (boxy<ymn) boxy=ymn;
	if (boxy>ymx) boxy=ymx;

	if (boxx<xmn) boxx=xmn;
	if (boxx>xmx) boxx=xmx;

	return;
}

BOOL 	boxshad = FALSE,
	dimshad = FALSE;

VOID DeBoxShadow() 
{
	if (!boxshad) return;

	GI_DrawSetup(1,1);

	v_move(sboxc[0][0]+sboxx,sboxc[1][0]+sboxy);
	v_draw(sboxc[0][1]+sboxx,sboxc[1][1]+sboxy);
	v_draw(sboxc[0][2]+sboxx,sboxc[1][2]+sboxy);
	v_draw(sboxc[0][3]+sboxx,sboxc[1][3]+sboxy);
	v_draw(sboxc[0][0]+sboxx,sboxc[1][0]+sboxy);

	boxshad = FALSE;
}

VOID DeDimsShadow() 
{
	DeBoxShadow();

	if (!dimshad) return;

	GI_DrawSetup(1,1);

	v_move(swittopx,swittopy);
	v_draw(slinx1,sliny1);
	v_draw(slinx2,sliny2);
	v_draw(switbotx,switboty);

	dimshad = FALSE;
}

VOID AttemptBoxRender()
{

	int i,j;

	DeBoxShadow();

	sboxx = boxx;
	sboxy = boxy;

	for (i=0; i<2; i++) 
		for (j=0; j<4; j++)	
			sboxc[i][j]=boxc[i][j];

	boxshad = TRUE;
	DeBoxShadow();		/* Trick shadow routine into rendering */
	boxshad = TRUE;
}

VOID AttemptDimsRender() 
{

	DeDimsShadow();

	if (!(topwitset&&botwitset)) return;

	if (!gotlin1) return;
	if (elock) CalWitAng();
	CalDimPerp();
	if (!gotlin2) return;

	boxx = (linx1 + linx2)/2.0;
	boxy = (liny1 + liny2)/2.0;

	swittopx = wittopx;
	swittopy = wittopy;
	slinx1 = linx1;
	sliny1 = liny1;
	slinx2 = linx2;
	sliny2 = liny2;
	switbotx = witbotx;
	switboty = witboty;

	witlen = hypot(linx2-linx1,liny2-liny1);
	sprintf(DimText,"%s",DispLin(witlen));
	sprintf(pbuffer,"Dim Text [%s]",DimText);

	paintinfo();

	dimshad = TRUE;
	DeDimsShadow();		/* Trick shadow routine into rendering */
	dimshad = TRUE;
	
}

LOCAL STDFLT tsp;
#define swp(x,y) tsp = x; x = y; y = tsp

BOOL FindIntercept(fx1,fy1,fx2,fy2,ix1,iy1,ix2,iy2,irx,iry)
STDFLT ix1,ix2,iy1,iy2,*irx,*iry,fx1,fy1,fx2,fy2;
{

	STDFLT 	ox1,oy1,ox2,oy2,fyd,fxd,iyd,ixd,
		oxd,oyd,oix,oiy,m0,b0,m1,b1,intx,inty;

	ox1=fx1;
	ox2=fx2;
	oy1=fy1;
	oy2=fy2;

	oyd=fyd = (fy1-fy2);
	oxd=fxd = (fx1-fx2);
	oiy = iyd = (iy1-iy2);
	oix = ixd = (ix1-ix2);

	if ((fxd==0.0&&ixd==0.0)||(fyd==0.0&&iyd==0.0)) return(FALSE);

	if (ixd==0.0||iyd==0.0) {
		swp(fy1,iy1);
		swp(fx1,ix1);
		swp(fy2,iy2);
		swp(fx2,ix2);
		swp(fyd,iyd);
		swp(fxd,ixd);
	} 

	if (fxd==0.0) {			/* Vertical */
		intx = fx1;
		if (ixd==0.0) return(FALSE);
		m0 = iyd/ixd;		/* Cal slope */
		b0 = iy1 - (m0 * ix1);
		inty = (m0 * intx) + b0;
		if (((oyd!=0.0&&inty>=min(oy1,oy2)&&inty<=max(oy1,oy2))||
		     (oxd!=0.0&&intx>=min(ox1,ox2)&&intx<=max(ox1,ox2)))&&
		    ((oiy!=0.0&&inty>=min(iy1,iy2)&&inty<=max(iy1,iy2))||
		     (oix!=0.0&&intx>=min(ix1,ix2)&&intx<=max(ix1,ix2)))) {
			tagint(intx,inty);
			*irx = intx;
			*iry = inty;
			return(TRUE);
		}	else return(FALSE);
	}

	if (fyd==0.0) {
		inty = fy1;
		if (ixd==0.0) {
			intx = ix1;
		if (((oyd!=0.0&&inty>=min(oy1,oy2)&&inty<=max(oy1,oy2))||
		     (oxd!=0.0&&intx>=min(ox1,ox2)&&intx<=max(ox1,ox2)))&&
		    ((oiy!=0.0&&inty>=min(iy1,iy2)&&inty<=max(iy1,iy2))||
		     (oix!=0.0&&intx>=min(ix1,ix2)&&intx<=max(ix1,ix2)))) {
				tagint(intx,inty);
				*irx = intx;
				*iry = inty;
				return(TRUE);
			}
		}
		m0 = iyd/ixd;		/* Cal slope */
		b0 = iy1 - (m0 * ix1);
		intx = (inty - b0) / m0;
		if (((oyd!=0.0&&inty>=min(oy1,oy2)&&inty<=max(oy1,oy2))||
		     (oxd!=0.0&&intx>=min(ox1,ox2)&&intx<=max(ox1,ox2)))&&
		    ((oiy!=0.0&&inty>=min(iy1,iy2)&&inty<=max(iy1,iy2))||
		     (oix!=0.0&&intx>=min(ix1,ix2)&&intx<=max(ix1,ix2)))) {
			tagint(intx,inty);
			*irx = intx;
			*iry = inty;
			return(TRUE);
		} else return(FALSE);
	}

	m0 = iyd/ixd;		/* Cal slope */
	b0 = iy1 - (m0 * ix1);

	m1 = fyd/fxd;		/* Cal slope */
	b1 = fy1 - (m1 * fx1);

	if ((m0-m1)==0.0&&(m1-m0)==0.0) return(FALSE);	/* Huh? */

	if ((m0-m1)!=0.0) 	intx = (b1 - b0) / (m0 - m1);
	else			intx = (b0 - b1) / (m1 - m0);

	inty = (m0 * intx) + b0;

	if (((oyd!=0.0&&inty>=min(oy1,oy2)&&inty<=max(oy1,oy2))||
	     (oxd!=0.0&&intx>=min(ox1,ox2)&&intx<=max(ox1,ox2)))&&
	    ((oiy!=0.0&&inty>=min(iy1,iy2)&&inty<=max(iy1,iy2))||
	     (oix!=0.0&&intx>=min(ix1,ix2)&&intx<=max(ix1,ix2)))) {
		tagint(intx,inty);
		*irx = intx;
		*iry = inty;
		return(TRUE);
	}	else return(FALSE);

}
