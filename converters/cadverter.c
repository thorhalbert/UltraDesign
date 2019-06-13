/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	ucad1.c - amiga main() startup and os setup	
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
 *	2	11-Jan-87 SH	Begin portability rush
 *	3	 8-Jun-88 SH	Add code for stroke lib location
 *	4	17-Jun-89 SH	Add the aegis draw reader
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include <workbench/startup.h>
#include <workbench/workbench.h>
#include "iff.h"
#include "gio.h"
/* #include <proto/icon.h> */

#define rmalloc(s) AllocMem(s,0)
#define rmfree(x,s) FreeMem(x,s)

extern UBYTE	DxfMask[];
extern UBYTE	DxfPath[];
extern UBYTE	IntroMask[];
extern UBYTE	IntroPath[];
extern UBYTE	ILBMMask[];
extern UBYTE	ILBMPath[];
extern UBYTE	HatMask[];
extern UBYTE	AegisMask[];
extern UBYTE	AegisPath[];
extern UBYTE	AegisFont[];
extern UBYTE	DxyMask[];
extern UBYTE	DxyPath[];
extern UBYTE	HpglMask[];
extern UBYTE	HpglPath[];

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

VOID abort();

VOID *GfxBase,*IntuitionBase;
VOID *DiskfontBase;
VOID *MathTransBase;		/* This is for the IEEE-FFP cvt'ers */
extern VOID *IconBase;
extern BOOL allowparent;
VOID BaseSetup();

extern struct TextAttr UcadFont;	/* Desc of ucad font in screens.c */
struct TextFont *ufont;			/* The font when we open it */

extern BOOL allowparent;

extern BOOL PaintFullScreen;

struct ViewPort nv;

struct TextAttr UcadFont =
    {
    (UBYTE *) "pearl2.font", /* Standard system font */
    8,    0,    0
    };

struct NewScreen ns = {
   0, 0,                               	/* start position                */
   640, 200, 4,                        	/* width, height, depth          */
   1, 0,                               	/* detail pen, block pen         */
   HIRES,    				/* View Mode */
   CUSTOMSCREEN | SCREENBEHIND,		/* screen type                   */
   &UcadFont,                          	/* font to use                   */
   " CADVerter - UltraDesign -- File Exchange Utility V1.1(48)",
   NULL                                	/* pointer to additional gadgets */
};
		
struct NewWindow nw = {
   0, 0,                          	/* start position                */
   640, 200,                 		/* width, height                 */
   0, 1,                        	/* detail pen, block pen         */
   0,		                   	/* IDCMP flags                   */
   ACTIVATE | REPORTMOUSE | BACKDROP,
                                  	/* window flags                  */
   NULL,           		/* pointer to first user gadget  */
   NULL,                          	/* pointer to user checkmark     */
   NULL,           		  	/* window title                  */
   NULL,                          	/* pointer to screen (set below) */
   NULL,                          	/* pointer to superbitmap        */
   0, 0, 640, 200,			/* ignored since not sizeable    */
   CUSTOMSCREEN                   	/* type of screen desired        */
};

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

BOOL resolution=FALSE;

BOOL setscreen()
{
	XSsize=640;
	YSsize=400;
	Sdepth=4;

	resolution = TRUE;

	barsize=0;

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

	if (!resolution) aspect *= 4.0;*/
	
	/* Setup screen size dependancies */
	
	XBegin=0;
	XWidth=XSsize-XBegin;

	YBegin=TOPMARGIN;
	YWidth=YSsize-YBegin;

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
      		return(FALSE);
	}

   	rp = w->RPort;
	
	SetRGB4(vp,  0, 0, 0, 0);		/* Default colors */
	SetRGB4(vp,  1, 7, 7, 7);
	SetRGB4(vp,  2,12,12,12);
	SetRGB4(vp,  3, 0, 0,13);
	SetRGB4(vp,  4, 0, 0,15);
	SetRGB4(vp,  5,15, 0, 0);
	
	return(TRUE);
	
}

main(argc,argv) 	/* Main program section */
int argc;
char *argv;
{
	int i;
	struct WBStartup *argst;
	UBYTE *mat;
	struct WBArg *arg;

	GfxBase = OpenLibrary("graphics.library",0);
	if (GfxBase == NULL)
		abort("Can't open graphics.library");

	IntuitionBase = OpenLibrary("intuition.library",0);
	if (IntuitionBase == NULL)
		abort("Can't open intuition.library");

	IconBase = OpenLibrary("icon.library",0);
	if (IconBase == NULL)
		abort("Can't open icon.library");

	DiskfontBase = OpenLibrary("diskfont.library",0);
	if (DiskfontBase == NULL)
		abort("Can't open diskfont.library");

	MathTransBase = OpenLibrary("mathtrans.library",0);
	if (MathTransBase == NULL)
		abort("Can't open mathtrans.library");

	filereq_setup();
	sysinit();

	ufont = OpenDiskFont(&UcadFont);	/* Open if possible */

	strcpy(ClipMask,"*.uvw");
	strcpy(ViewMask,"*.uvw");
	strcpy(LibrMask,"*.ulb");
	strcpy(FontMask,"*.ufnt");

	strcpy(DxfMask,"*.dxf");
	strcpy(IntroMask,"*.cad");
	strcpy(ILBMMask,"*.pic");
	strcpy(HatMask,"*.uhat");
	strcpy(DxyMask,"*.dxy");
	strcpy(HpglMask,"*.hpgl");
	strcpy(AegisMask,"*");
	strcpy(AegisFont,"simplex2");

	strcpy(LayerMask,"*");
	strcpy(StyleMask,"*");
	strcpy(GroupMask,"*");
	strcpy(PenMask,"*");
	strcpy(FntMask,"*");
	strcpy(ItemMask,"*");

	strcpy(ViewPath,"UltraDesign:drawings");
	strcpy(DxfPath,"UltraDesign:dxf-files");
	strcpy(IntroPath,"UltraDesign:introcad-files");
	strcpy(ILBMPath,"UltraDesign:pix");
	strcpy(HatchPath,"UltraDesign:Hatch_lib");
	strcpy(AegisPath,"UltraDesign:aegis-files");
	strcpy(DxyPath,"UltraDesign:dxy-files");
	strcpy(HpglPath,"UltraDesign:hpgl-files");

	strcpy(StrokePath,"UltraDesign:Stroke_lib");
	strcpy(TemplatePath,"UltraDesign:templates");
	strcpy(HomePath,"UltraDesign:");

	allowparent = TRUE;

	if (argc==0) {
		HomePath[0] = '\0';
		path_setup(HomePath);
		argst = (struct WBStartup *) argv;
		arg = argst->sm_ArgList;
	/*	for (i=0; i<argst->sm_NumArgs; i++, arg++) procarg(i,arg); */

	}
	else {
		mat = (UBYTE *) argv[0];
		strcpy(HomePath,mat);
		path_setup(HomePath);
	}
	
	aspect=1.14285;		/* Hires aspect constant */

	parselist();
	
/*	if (!LoadStrokeFont("duplex1")) abort("Cannot load font duplex1\n"); */
	if (!LoadStrokeFont("simplex2")) abort("Cannot load font simplex2\n");
	
	BaseSetup();
	if (!setscreen()) abort("quit");
	PaintFullScreen = TRUE;
	Init_Tables();
	
	setreq();
	
	RepaintWindow();
	
/*	quietupdate = TRUE;
	dointro(argc,argv);
	quietupdate = FALSE;	*/
	
	fullzoomout();
	RepaintWindow();
	
	ScreenToFront(screen);
	
	wait_close();

}

VOID Exit_Quit()
{
	
	DeleteTimer();
	
	if (areabuffer)	FreeMem(areabuffer,areasize);
	if (TmpPlane)	FreeRaster(TmpPlane, XWidth+8, YWidth+8);

	if (rp) {
		rp->AreaInfo = NULL;
		rp->TmpRas = NULL;
	}
	
	if (w) {
		PurgeIDCMP(w->UserPort);
		ClearMenuStrip(w);
		CloseWindow(w);
	}
	
	if (screen) CloseScreen(screen); 
	
	w = NULL;
	rp = NULL;
	screen = NULL;
	
	abort("Done");

}

VOID abort(s)
char *s;
{


	FreeAegis();
	FreeAllPools();
	GI_FreeRem();
	GI_FreeDraw();

	CloseLibrary(GfxBase);
	CloseLibrary(IntuitionBase);
	CloseLibrary(IconBase);
	CloseLibrary(DiskfontBase);
	CloseLibrary(MathTransBase);

	exit(0);
}


VOID FinWindow() {}
VOID DoPanVert() {}
VOID DoPanHoriz() {}
VOID UpdateTitle() {}
VOID SExitSub() {}
VOID SInfoMess() {}
VOID freelibs() {}
VOID AdjustPots() {}
VOID UpdateCore() {}

BOOL drawing_dirty=TRUE;

VOID (*CleanupShadows)() = NULL;

struct IntuiText comptext[] = {
 { 5, 1, JAM2, 6, 5, NULL, NULL, NULL },
 { 5, 1, JAM2, 6, 15, NULL, NULL, NULL },
 { 5, 1, JAM2, 6, 25, NULL, NULL, NULL },
 { 5, 1, JAM2, 6, 35, NULL, NULL, NULL }};
struct IntuiText commes[] = {
 { 5, 3, JAM2, 6, 3, NULL, NULL, NULL },
 { 5, 3, JAM2, 6, 3, NULL, NULL, NULL }};

BOOL complain(nm,pm,c1,c2,c3,c4,l1,l2,l3,l4)
UBYTE *nm,*pm,*l1,*l2,*l3,*l4;
int c1,c2,c3,c4;
{

	int bot,i,wid;
	struct IntuiText *nmi,*pmi;
	struct Window *reqwin;
   	ULONG class,signals;
   	USHORT code;
	struct Gadget *GetGadget;
	BOOL breakout,answer;

    	ULONG kep_IDCMPFlags;   /* User-selected flags */
    	struct MsgPort *kep_UserPort, *kep_WindowPort;
    	struct IntuiMessage *kep_MessageKey;

	comptext[0].IText = l1;
	comptext[1].IText = l2;
	comptext[2].IText = l3;
	comptext[3].IText = l4;

	comptext[0].FrontPen = c1;
	comptext[1].FrontPen = c2;
	comptext[2].FrontPen = c3;
	comptext[3].FrontPen = c4;

	if (pm==NULL) {			/* Switch em around */
		pm = nm;
		pm = NULL;
	}	

	wid = 0;
	for (i=0; i<4; i++) {
		comptext[i].NextText=NULL;
		if (comptext[i].IText==NULL) break;
		wid = max(wid, IntuiTextLength(&comptext[i]));
		if (i>0) comptext[i-1].NextText = &comptext[i];
		bot = comptext[i].TopEdge;
	}

	if (wid>600) wid=600;

	commes[0].IText = nm;
	nmi = (nm==NULL)?NULL:&commes[0];
	commes[1].IText = pm;
	pmi = (pm==NULL)?NULL:&commes[1];

	if (w!=NULL) {
		Forbid();
    		kep_IDCMPFlags = w->IDCMPFlags;
		kep_UserPort = w->UserPort;
		kep_WindowPort = w->WindowPort;
		kep_MessageKey = w->MessageKey;
		Permit();
	}
	
	reqwin = BuildSysRequest(w,&comptext[0],nmi,pmi,
		GADGETUP | GADGETDOWN | RAWKEY,
		wid+36,bot+50);
		
	if (reqwin==FALSE) return(FALSE);
	if (reqwin==TRUE) return(TRUE);		/* Got an alert not a req */
	
/*	ClearPointer(w);
	PointerOn=FALSE; */

	breakout = FALSE;
	while (!breakout) {

		signals = Wait(1L << reqwin->UserPort->mp_SigBit);

		if (signals!=0) {

			while (message=GetMsg(reqwin->UserPort)) {

				class = message->Class;
				code  = message->Code;
	
				ReplyMsg(message);
	
				switch (class) {
	
				case GADGETDOWN:
					GetGadget=message->IAddress;
					answer = GetGadget->GadgetID;
					breakout=TRUE;
					break;
				case GADGETUP:
					GetGadget=message->IAddress;
					answer = GetGadget->GadgetID;
					breakout=TRUE;
					break;
				case RAWKEY:
					switch(code) {
					case 0x45:
						answer = TRUE;
						breakout = TRUE;
						break;
					case 0x44:
					case 0x43:
						answer = FALSE;
						breakout = TRUE;
						break;
					}
				}
			}
		}		
	}
	
	FreeSysRequest(reqwin);

	if (w!=NULL) {
		Forbid();
    		kep_IDCMPFlags = w->IDCMPFlags;
		kep_UserPort = w->UserPort;
		kep_WindowPort = w->WindowPort;
		kep_MessageKey = w->MessageKey;
		Permit();
	}

	return(answer);	

}

VOID oneliner(tex)
UBYTE *tex;
{

	(VOID) complain(NULL,"Recover",4,0,0,0,tex,NULL,NULL,NULL);
}

LOCAL UBYTE sbuf[255];
UBYTE *MakeFname(path,name,ext)
UBYTE *path,*name,*ext;
{
	int i;
	UBYTE *nb = "";
	
	if (ext==NULL) ext = nb;

	i = strlen(path);

	while (i>0&&path[i-1]==' ') {
		path[i-1] = '\0';
		i--;
	}
	
	if (path[i-1]=='/'||path[i-1]==':')
		sprintf(sbuf,"%s%s%s",path,name,ext);
	else	sprintf(sbuf,"%s/%s%s",path,name,ext);
	
	return(sbuf);
}

STYLE_A default_styles;
UBYTE *Default_Comment = "Default";

VOID BaseSetup()
{

	Color_Source = 2;		/* Use Layer Colors */
	
	mensetup(0x0F+Color_Source);	/* check Layer coloring */
	mensetup(0x50);			/* check Line mode */
	mensetup(0x90);			/* check roll on screen */
	mensetup(0x100);		/* degrees with decimal point */
	mensetup(0x114);		/* millimeters */
	
	Root_Node=NULL;
	Last_Node=NULL;
	
	sprintf(default_group.Group_Name,"Group_Root");
	default_group.Use_Count=0;
	default_group.Color_ID=Default_Color;
	default_group.Group_ID=0;
	default_group.Group_Flags=0;
	default_group.Libr_Flags=0;
	default_group.parent=NULL;
	default_group.next=NULL;
	default_group.previous=NULL;
	default_group.descendants=NULL;
	default_group.desc_last=NULL;
	default_group.Comment = Default_Comment;

	Root_Group = Current_Group = &default_group;

	sprintf(default_layer.Layer_Name,"Main_Layer");
	default_layer.Use_Count=0;
	default_layer.Color_ID=Default_Color;
	default_layer.Layer_Flags=0;
	default_layer.Layer_ID=0;
	default_layer.layer_next=NULL;
	default_layer.layer_prev=NULL;
	default_layer.Comment = Default_Comment;

	Root_Layer = Last_Layer = Current_Layer = &default_layer;

	sprintf(default_pen.Pen_Name,"Default_Pen");
	default_pen.Use_Count = 0;
	default_pen.Color_ID = Default_Color;
	default_pen.Line_Pattern = 0xFFFF;	/* Unbroken line */
	default_pen.Hard_Color = 0;		/* Black Black */
	default_pen.pen_next=NULL;
	default_pen.pen_prev=NULL;
	default_pen.Comment = Default_Comment;
	default_pen.PenOrd = 0;
	default_pen.Carousel = 0;
	default_pen.AutoNum = 0;
	default_pen.BitWidth = 1.0;
	default_pen.PenWidth = 0.5;

	Root_Pen = Last_Pen = Current_Pen = Default_Pen = &default_pen;

	sprintf(&default_styles.Letter_Style_ID,"%s","Template_Style");

	default_styles.draw_font = Root_Font;
	default_styles.Use_Count = 0;
	default_styles.Prime_Height = 5.0;
	default_styles.Aspect = 1.0;
	default_styles.Slant = 0.0;
	
	default_styles.prev_style = NULL;
	default_styles.next_style = NULL;

	Root_Style = &default_styles;
	Style_Last = Root_Style;
	Current_Style = Root_Style;
	Default_Style = Root_Style;

	Wit_Layer = Dim_Line_Layer = Dim_Box_Layer = Current_Layer;
	Wit_Group = Dim_Line_Group = Dim_Box_Group = Current_Group;
	Wit_Pen = Dim_Line_Pen = Dim_Box_Pen = Current_Pen;
	Dim_Box_Style = Current_Style;

}

VOID BaseVirSetup()
{ BaseSetup(); }

VOID CloseRequest(req,win)
struct Requester *req;
struct Window *win;
{

	PurgeIDCMP(win->UserPort);
	EndRequest(req, win);
}

#asm
* ----------------------------------------------------------------------
* PurgeIDCMP(iport)
*  register struct MsgPort *iport;
* Replies to all messages from a port, till no more messages pending
* ----------------------------------------------------------------------
MENUVERIFY	equ	$2000
MENUHOT		equ	$1
MENUCANCEL	equ	$2
im_Class	equ	$14
im_Code		equ	$18
	cseg
	public	_LVOGetMsg
	public	_LVOReplyMsg

	public	_PurgeIDCMP
_PurgeIDCMP:
	link	a5,#0
	move.l	a6,-(sp)

; while ( (imsg = (struct IntuiMessage *)GetMsg(iport)) != 0L )
	movea.l	4,a6
purg1:
	movea.l	8(a5),a0
	jsr	_LVOGetMsg(a6)
	tst.l	d0
	 beq.s	purgedone
	movea.l	d0,a1			;a1 = (struct IntuiMessage *)msg
	cmpi.l	#MENUVERIFY,im_Class(a1) ;Right Mouse button?
	 bne.s	1$			;No
	cmpi.w	#MENUHOT,im_Code(a1)	;Is our window active?
	 bne.s	1$			;No
	move.w	#MENUCANCEL,im_Code(a1)	;Otherwise cancel menu operations

1$
	jsr	_LVOReplyMsg(a6)	;ReplyMsg(imsg)
	bra.s	purg1

purgedone:
	move.l	(sp)+,a6
	unlk	a5
	rts
#endasm

VOID DoBorders(inreq,xlin,btop,bbot)
struct Requester *inreq;
int xlin,btop,bbot;
{

	struct RastPort *reqrp;

	reqrp = inreq->ReqLayer->rp;	/* Nasty */
		
	SetAPen(reqrp, 2);
	Move(reqrp, 0, 0);
	Draw(reqrp, (inreq->Width-1), 0);
	Draw(reqrp, (inreq->Width-1), 
		inreq->Height-1);
	Draw(reqrp, 0, inreq->Height-1);
	Draw(reqrp, 0, 0);

	SetAPen(reqrp, 0);
	Move(reqrp, 3, 2);
	Draw(reqrp, (inreq->Width-1)-3, 2);
	Draw(reqrp, (inreq->Width-1)-3, 
		inreq->Height-3);
	Draw(reqrp, 3, inreq->Height-3);
	Draw(reqrp, 3, 2);
		
	if (xlin>0) {
		Move(reqrp, 3, xlin);
		Draw(reqrp, (inreq->Width-1)-3, xlin);
	}
	
	if (btop>0) {	

		SetAPen(reqrp, 3);
		SetBPen(reqrp, 3);
		SetOPen(reqrp, 2);
		RectFill(reqrp, 8,btop,inreq->Width-8,bbot); 
	}
}

/*------------------------Write IFF ilbm file----------------------------*/

#define bufsiz (16*1024)
extern IFFP ifferror;

IFFP putilbm(path,file,iflnm,afln)
UBYTE *iflnm,*afln,*path,*file;
{

	BPTR	outfile;
	long	errret;
	UBYTE 	ptitle[80];
	UBYTE	ftmp[120],ftmp2[120];
	VOID	*stobuff;

	ifferror = IFF_OKAY;

	InfoMess(SAVE_MODE,SAVE_MESS_SAVE);

	sprintf(ftmp,"%s(ilbm-temporary).pic",iflnm);
	sprintf(ftmp2,"%s%s",iflnm,afln);
	outfile=GOpen(ftmp,MODE_NEWFILE);
	if (outfile==0) {
		errret=IoErr();

		printf("Cannot save %s - error %ld\n",afln,errret);
		InfoMess(SAVE_MODE,SAVE_MESS_ERROR);

		return(NO_FILE);
	}

	stobuff = rmalloc(bufsiz);
	if (stobuff==NULL) return(FALSE);	/* Oops */
	
	/* SetWindowTitles(w,-1,0); */
	ShowTitle(screen,FALSE);
	
	if (PutPict(outfile,rp->BitMap,w->Width,w->Height,
		vp->ColorMap->ColorTable,stobuff,bufsiz)) {
		rmfree(stobuff,bufsiz);
		GClose(outfile);
		fprintf(stdout,"Depth was %d\n",rp->BitMap->Depth);
		printf("Got error %d saving iff\n",ifferror);
		return(ifferror);
	}
	
	rmfree(stobuff,bufsiz);
	GClose(outfile);

	DeleteFile(ftmp2);	/* Delete if exists or not */
	if (!Rename(ftmp,ftmp2)) {
		printf("Couldn't rename temporary to your output name!\n");
		return(IFF_OKAY);
	}

	return(IFF_OKAY);
}

VOID ClearView()
{

	int loop;

	freelibs();		/* If loaded */
	FreeAllPools();		/* Free now that we're serious */

	Root_Style=Style_Last=NULL;
	Root_Layer=Last_Layer=NULL;
	Root_Pen=Last_Pen=NULL;

	Wit_Layer = Dim_Line_Layer = Dim_Box_Layer = NULL;
	Wit_Group = Dim_Line_Group = Dim_Box_Group = NULL;
	Wit_Pen = Dim_Line_Pen = Dim_Box_Pen = NULL;
	Dim_Box_Style = NULL;

	Default_Style = NULL;

	for (loop=0; loop<10; loop++) vset[loop] = FALSE;

	BaseVirSetup();

	Color_Source = 3;	/* Color by pen */	
	
}
