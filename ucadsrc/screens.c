/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	screens.c - amiga screen setups
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
 *	2	 9-May-87 SH	Setup other 3D icon colors
 *	3	12-Jun-87 SH	Fill out rest of color defaults
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include <graphics/gels.h>

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

void 	PerformLine(),
	view_setup(),
	CloseDisplay();

/*-------------------------External Symbol Definitions-----------------------*/

/* Vsprite setups */

UWORD spritecols[] = {0x00F, 0x0F0, 0xF00 };
extern UWORD PointerVSprite[];

struct VSprite TabHead,TabTail;
struct VSprite SpriteStruct = {
	0, 0, 0, 0, 0, 0, VSPRITE,
	0, 0,
	5, 0, 0, 0, 0,
	&PointerVSprite[0], 
	0, 0,
	&spritecols[0],
	0, 0, 0 };

struct GelsInfo SpriteInfo;
	
/*-------------------------Global Variable Definitions-----------------------*/

struct TextAttr UcadFont =
    {
    (UBYTE *) "pearl2.font", /* Standard system font */
    8,    0,    0
    };

struct Image H_PanImage;

struct PropInfo	HorizPan = {
	AUTOKNOB | FREEHORIZ,
	0, 0,
	0xFFFF, 0xFFFF,
	0,0,0,0,0,0
};
struct Gadget	HorizPanProp = {
	NULL,
	0,-(BOTTOMAREA+HORIZBAR-3),-VERTBAR,HORIZBAR,
	GADGIMAGE | GRELWIDTH,
	RELVERIFY,
	PROPGADGET,
	(APTR) &H_PanImage,
	NULL,
	NULL,
	0,
	(APTR) &HorizPan,
	H_GADGET,
	NULL
}; 

struct Image V_PanImage;

struct PropInfo	VertPan = {
	AUTOKNOB | FREEVERT,
	0, 0,
	0xFFFF, 0xFFFF,
	0,0,0,0,0,0
};
struct Gadget	VertPanProp = {
	NULL, 
	-(VERTBAR-1),TOPMARGIN,VERTBAR,-(TOPMARGIN+BOTTOMAREA+HORIZBAR),
	GADGIMAGE | GRELHEIGHT | GRELRIGHT,
	RELVERIFY,
	PROPGADGET,
	(APTR) &V_PanImage,
	NULL,
	NULL,
	0,
	(APTR) &VertPan,
	V_GADGET,
	NULL
}; 

struct NewScreen ns = {
   0, 0,                               	/* start position                */
   640, 200, 4,                        	/* width, height, depth          */
   3, 2,                               	/* detail pen, block pen         */
   HIRES,    				/* View Mode */
   CUSTOMSCREEN | SCREENBEHIND,		/* screen type                   */
   &UcadFont,                          	/* font to use                   */
   VERDIS,				/* default title for screen      */
   NULL                                	/* pointer to additional gadgets */
};
		
struct NewWindow nw = {
   0, 0,                          	/* start position                */
   640, 200,                 		/* width, height                 */
   3, 2,                        	/* detail pen, block pen         */
   0,		                   	/* IDCMP flags                   */
   WINDOWCLOSE | ACTIVATE | /*REPORTMOUSE|*/BACKDROP | BORDERLESS,
                                  	/* window flags                  */
   NULL,	           		/* pointer to first user gadget  */
   NULL,                          	/* pointer to user checkmark     */
   NULL,           		  	/* window title                  */
   NULL,                          	/* pointer to screen (set below) */
   NULL,                          	/* pointer to superbitmap        */
   0, 0, 640, 200,			/* ignored since not sizeable    */
   CUSTOMSCREEN                   	/* type of screen desired        */
};

/*-------------------------------Code Section--------------------------------*/

BOOL resolution=FALSE;

void set_loscn()
{

	XSsize=640;
	YSsize=200;
	Sdepth=3;

	resolution=FALSE;

	aspect=2.2857;		/* Aspect constant */

	barsize = HORIZBAR;

	return;

}
void set_hiscn()
{
	XSsize=640;
	YSsize=400;
	Sdepth=4;

	resolution=TRUE;

	aspect=1.14285;		/* Hires aspect constant */

	barsize=(HORIZBAR*2)-1;

}

extern UBYTE cbuf[];

struct AreaInfo UcadAreaInfo;
VOID *areabuffer=NULL;		/* Buffer for area fills in rastport */
LONG areasize;
PLANEPTR *TmpPlane=NULL;	/* Temporary plane for area fills - uggg */
struct TmpRas UcadTmpRas;
BOOL initras=FALSE;

BOOL SetupRas()
{

	if (initras) return(TRUE);	
	
	areabuffer = NULL;
	TmpPlane = NULL;

	if (rp) {
		rp->AreaInfo = NULL;
		rp->TmpRas = NULL;
	}
	
	areasize = (Max_Vectors * 5) + 4;
	areabuffer = (VOID *) AllocMem(areasize, 0);
			/* Get buffer from anywhere */
	if (areabuffer==NULL) return(FALSE);

	InitArea(&UcadAreaInfo, areabuffer, Max_Vectors);
	rp->AreaInfo = &UcadAreaInfo;

	TmpPlane = AllocRaster(XWidth+8,YWidth+8);
	if (TmpPlane == NULL) {
		if (areabuffer)	FreeMem(areabuffer,areasize);
		areabuffer = NULL;
		TmpPlane = NULL;

		if (rp) {
			rp->AreaInfo = NULL;
			rp->TmpRas = NULL;
		}
		
		return(FALSE);
	}
	
	rp->TmpRas = InitTmpRas(&UcadTmpRas, TmpPlane, 
		RASSIZE(XWidth+8,YWidth+8));
	
	initras = TRUE;
	
	return(TRUE);

}

extern struct Menu *Menu_Root;

BOOL open_winds()
{
	int i;

	ucadname = VERDIS;	/* Get from here so no recompiles needed */
	
	/*-----------------Configure screen-------------------*/
	
	ns.ViewModes=0;
	if (XSsize>320) ns.ViewModes|=HIRES;
	if (resolution) ns.ViewModes|=LACE;

	ns.Width=320;
	if (XSsize>320) ns.Width=640;
	
	ns.Height = STDSCREENHEIGHT;

	ns.Depth=Sdepth;

   	screen = SafeScreen(&ns);
   	if (screen == NULL) {
		sysalert("Cannot Open Screen");
      		return(FALSE);
   	}

	YSsize=screen->Height;

/*#define ASPECT_FACTOR (1.14285/400.0)

	aspect = ASPECT_FACTOR;
	aspect *= ((STDFLT) YSsize);

	if (!resolution) aspect *= 4.0;*/	/* Just override in startdefs */
	
	/* Setup screen size dependancies */
	
	VertPanProp.Height=-(TOPMARGIN+BOTTOMAREA+barsize);

	HorizPanProp.Height=barsize;
	HorizPanProp.TopEdge=YSsize-(BOTTOMAREA+barsize);

	XBegin=LEFTAREA+1;
	XWidth=XSsize-VERTBAR-XBegin;
	XWidth-=2;

	YBegin=TOPMARGIN;
	YWidth=HorizPanProp.TopEdge-YBegin;
	YWidth-=1;

	/*---------------------Setup Window-------------------*/

   	vp = &screen->ViewPort;

	nw.LeftEdge=0;
	nw.TopEdge=0;

	nw.Width=XSsize;
	nw.Height=YSsize;

   	nw.Screen = screen;
   	w = SafeWindow(&nw);
   	if (w == NULL) {
		sysalert("Cannot Open Window");
      		/* CloseScreen(screen); */
      		return(FALSE);
	}

	/* InitGels(&TabHead,&TabTail,&SpriteInfo); */
				/* We might do some vsprites */

   	rp = w->RPort;

	ShowTitle(screen,FALSE);

	SetRGB4(vp,  0, 0, 0, 0);
	SetRGB4(vp,  1, 13, 13, 13);
	SetRGB4(vp,  2, 13, 13, 13);
	SetRGB4(vp,  3, 13, 13, 13);
	SetRGB4(vp,  4, 13, 13, 13);
	
	SetAPen(rp,0);
	SetBPen(rp,0);
	SetOPen(rp,0);
		
	SetDrMd(rp, JAM1);
	SetAfPt(rp,NULL,0);
	RectFill(rp,0,0,XSsize,YSsize);
	
	areabuffer = NULL;
	TmpPlane = NULL;
	rp->AreaInfo = NULL;
	rp->TmpRas = NULL;
	
	Init_Tables();
	view_defaults();
	BaseVirSetup();

	for (i=0; i<16; i++) {
		Color_Buff[i][0][0] = 0;
		Color_Buff[i][1][0] = 0;
		Color_Buff[i][2][0] = cbuf[i];
		Color_Buff[i][0][1] = cbuf[i];
		Color_Buff[i][1][1] = 0;
		Color_Buff[i][2][1] = 0;
	}

#ifdef SCREEN_SAVER
	SetSaverWait();
#endif

   	SetDrMd(rp,JAM1);
	SetAPen(rp, 1);
   	SetBPen(rp, 0);

	return(TRUE);
	
}

FinWindow()
{

	int i;
	
	for (i=0; i<16; i++) SetRGB4(vp, i, 0,0,0);

	AddGadget(w,&VertPanProp,-1);
	AddGadget(w,&HorizPanProp,-1);

	SetAPen(rp,0);
	SetBPen(rp,0);
	SetOPen(rp,0);
		
	SetDrMd(rp, JAM1);
	SetAfPt(rp,NULL,0);
	RectFill(rp,0,0,XSsize,YSsize);
	
	Draw_Line();

	ClearMenuStrip(w);			/* It got added earlier */
   	SetMenuStrip(w, Menu_Root);

	StartInts();
	ShowTitle(screen,TRUE);
									
	for (i=0; i<16; i++) {

		SetRGB4(vp,  0,( 0*i)/15,( 0*i)/15,( 0*i)/15);
		SetRGB4(vp,  1,( 7*i)/15,( 7*i)/15,( 7*i)/15);
		SetRGB4(vp,  2,(12*i)/15,(12*i)/15,(12*i)/15);
		SetRGB4(vp,  3,( 0*i)/15,( 0*i)/15,(13*i)/15);
		SetRGB4(vp,  4,( 0*i)/15,( 0*i)/15,(15*i)/15);
		SetRGB4(vp,  5,(15*i)/15,( 0*i)/15,( 0*i)/15);
		SetRGB4(vp,  6,(10*i)/15,( 0*i)/15,(10*i)/15);
		SetRGB4(vp,  7,( 8*i)/15,( 0*i)/15,( 0*i)/15);

		if (Sdepth>3) {
			SetRGB4(vp,  8,( 0*i)/15,( 0*i)/15,(10*i)/15);
			SetRGB4(vp,  9,(11*i)/15,( 9*i)/15,( 0*i)/15);
			SetRGB4(vp, 10,(13*i)/15,( 0*i)/15,( 0*i)/15);
			SetRGB4(vp, 11,( 9*i)/15,( 9*i)/15,( 9*i)/15);
			SetRGB4(vp, 12,(11*i)/15,( 6*i)/15,( 0*i)/15);
			SetRGB4(vp, 13,( 1*i)/15,( 0*i)/15,(10*i)/15);
			SetRGB4(vp, 14,( 4*i)/15,( 0*i)/15,( 7*i)/15);
			SetRGB4(vp, 15,( 0*i)/15,(11*i)/15,( 6*i)/15);
		}
	}
	
	Win_Setup = TRUE;

}

void CloseDisplay()
{

	if (areabuffer)	FreeMem(areabuffer,areasize);
	if (TmpPlane)	FreeRaster(TmpPlane, XWidth+8, YWidth+8);
	areabuffer = NULL;
	TmpPlane = NULL;

	if (rp) {
		rp->AreaInfo = NULL;
		rp->TmpRas = NULL;
	}
	
#ifdef OLDCONSOLEHANDLER
	
	printf("3 ");
	if (dialwin) {
		closedial();
	}

#endif

	CloseDialog();	
	
	printf("4 ");
	if (w) {
		PurgeIDCMP(w->UserPort);
		StopInts();
		ClearMenuStrip(w);
		CloseWindow(w);
	}
	printf("5 ");
	if (screen) CloseScreen(screen); 

	rp = NULL;
	w = NULL;
	screen = NULL;
	printf("6\n");
	return;

}
