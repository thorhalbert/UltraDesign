/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	pasteup.c - paste up main program
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
 *	4	13-Oct-87 SH	Make pasteup program
 *
 */

/*-------------------------------Include Files-------------------------------*/

int Dprintf(mask,p1,p2,p3,p4,p5,p6)
{}

#include "plotsetup.h"
#include <exec/memory.h>
#include <graphics/gfxmacros.h>
#include <workbench/startup.h>
#include <workbench/workbench.h>
#include <libraries/dos.h>
#include <libraries/dosextens.h>

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

BOOL complain();

struct TextAttr UcadFont =
    {
    (UBYTE *) "pearl2.font", /* Standard system font */
    8,    0,    0
    };


struct NewScreen ns = {
   0, 0,                               	/* start position                */
   640, 200, 4,                        	/* width, height, depth          */
   3, 2,                               	/* detail pen, block pen         */
   HIRES,    				/* View Mode */
   CUSTOMSCREEN|SCREENBEHIND,		/* screen type                   */
   &UcadFont,                          	/* font to use                   */
   VERDIS,				/* default title for screen      */
   NULL                                	/* pointer to additional gadgets */
};

extern struct Gadget MAGadgets[];
extern struct IntuiText MAHeadings[];
		
struct NewWindow nw = {
   0, 0,                          	/* start position                */
   640, 200,                 		/* width, height                 */
   3, 2,                        	/* detail pen, block pen         */
   0,		                   	/* IDCMP flags                   */
   ACTIVATE | REPORTMOUSE | BACKDROP,
                                  	/* window flags                  */
   &MAGadgets[0],	           		/* pointer to first user gadget  */
   NULL,                          	/* pointer to user checkmark     */
   NULL,           		  	/* window title                  */
   NULL,                          	/* pointer to screen (set below) */
   NULL,                          	/* pointer to superbitmap        */
   0, 0, 640, 200,			/* ignored since not sizeable    */
   CUSTOMSCREEN                   	/* type of screen desired        */
};

/*-------------------------------Code Section--------------------------------*/

BOOL GotProj = FALSE;
UBYTE LoadFile[255];

VOID procarg(anm,arg)
int anm;
struct WBArg *arg;
{

	struct DiskObject *diskobj=NULL;
	LONG olddir;
	char **toolarray;
	char *value;

/*	printf("Arg %s, lock=%d\n",arg->wa_Name,arg->wa_Lock);	*/
	if (arg->wa_Lock==NULL&&anm>0) return;	
	if (arg->wa_Name[0]==' ') return;	/* A drawer... */

	if (arg->wa_Lock!=NULL) olddir = CurrentDir(arg->wa_Lock);

	diskobj = GetDiskObject(arg->wa_Name);	

	if (olddir!=NULL) CurrentDir(olddir);	/* Get back */

	if (diskobj==NULL) return;		/* Nothing done */

	toolarray = diskobj->do_ToolTypes;

/*	while (*toolarray!=NULL) printf("tool=%s\n",*toolarray++);	*/

	toolarray = diskobj->do_ToolTypes;

	if (!GotProj&&diskobj->do_Type==WBPROJECT) {
		value = FindToolType(toolarray,"FILETYPE");
		if (value) {
			if (MatchToolValue(value,"2DOF_PASTEUP")) {
				strcpy(LoadFile,arg->wa_Name);
				if (arg->wa_Lock!=NULL) 
					olddir = CurrentDir(arg->wa_Lock);
				GotProj=TRUE;
			}
		}
	}

	FreeDiskObject(diskobj);		/* Give memory back */
	
}

struct Remember *DrawStore;
UBYTE *DrawMem(size)
int size;
{

	UBYTE *outloc;

	outloc = AllocRemember(&DrawStore, size, MEMF_FAST);

	if (outloc!=NULL) return(outloc);	/* We got fast memory */

	outloc = AllocRemember(&DrawStore, size, MEMF_CHIP);

	if (outloc!=NULL) return(outloc);	/* Need to use chip memory */

/*	printf("Unable to allocate %d mainline bytes\n",size);	*/

	return(NULL);

}

VOID GI_RectFill(color,x1,y1,x2,y2)
int color,x1,y1,x2,y2;
{

	SetAPen(rp,color);
	SetOPen(rp,color);

	SetDrMd(rp, JAM1);

	RectFill(rp, x1,y1,x2,y2);

}

BOOL resolution=FALSE;

BOOL create_screens()
{

	XSsize=640;
	YSsize=200;
	Sdepth=3;
	resolution = FALSE;

	aspect=2.2857;		/* Aspect constant */

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

#define ASPECT_FACTOR (1.14285/400.0)

	aspect = ASPECT_FACTOR;
	aspect *= ((STDFLT) YSsize);

	if (!resolution) aspect *= 4.0;
	
	/*---------------------Setup Window-------------------*/

   	vp = &screen->ViewPort;

	SetRGB4(vp,  0, 0, 0,12);	/* Blue out screen for setup */
	SetRGB4(vp,  1, 0, 0,12);
	SetRGB4(vp,  2, 0, 0,12);
	SetRGB4(vp,  3, 0, 0,12);
	SetRGB4(vp,  4, 0, 0,12);
	SetRGB4(vp,  5, 0, 0,12);
	SetRGB4(vp,  6, 0, 0,12);
	SetRGB4(vp,  7, 0, 0,12);

/*--------------Turn the screen blue during setup for effect----------------*/

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

   	SetDrMd(rp,JAM1);
	SetAPen(rp, 1);
   	SetBPen(rp, 0);

	Init_Tables();

	GI_RectFill(3,3,11,634,197);

	sprintf(PlotName,"SER:");

	PrintIText(rp, &MAHeadings[0], 0, 0);
	Set_Current();		/* Paint all of the gadgets */

	SetRGB4(vp,  0, 0, 0, 0);
	SetRGB4(vp,  1,12,12,12);
	SetRGB4(vp,  2,15,15,15);
	SetRGB4(vp,  3, 0, 0,13);
	SetRGB4(vp,  4, 0, 0,15);
	SetRGB4(vp,  5,15, 0, 0);
	SetRGB4(vp,  6,10, 0,10);
	SetRGB4(vp,  7, 8, 0, 0);

   	return(TRUE);
}


VOID CloseDisplay()
{

	if (w) {
		ClearMenuStrip(w);
		CloseWindow(w);
	}
	if (screen) CloseScreen(screen); 
	w = NULL;
	screen = NULL;
	return;

}

#include <graphics/gfxbase.h>
#include <intuition/intuitionbase.h>

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

ULONG begfree,endfree;

int cur_resource = 0;

/*-------------------------------Code Section--------------------------------*/

VOID *GfxBase=NULL, *IntuitionBase=NULL;
extern VOID *IconBase;

extern BOOL allowparent;

struct Process *CurProc;		/* current process */
struct CommandLineInterface *CurCLI;	/* current CLI info */
static APTR savewindow;			/* saved process WindowPtr */
static APTR pushwindow;			/* pushed process WindowPtr */

sysinit()
{

	CurProc = (struct Process *)FindTask((char *)NULL);
	CurCLI = (struct CommandLineInterface *)BADDR(CurProc->pr_CLI);
	savewindow = CurProc->pr_WindowPtr;

}

reqoff()
{
	pushwindow = CurProc->pr_WindowPtr;
	CurProc->pr_WindowPtr = (APTR)-1;
}

setreq()
{
	CurProc->pr_WindowPtr = w;
}

/*
 * reqpop -- restore requesters to action at last reqoff
 */
reqpop()
{
	CurProc->pr_WindowPtr = pushwindow;
}

char *getcwd(buf, len)
register char *buf;
int len;
{
	register UBYTE *dirname;

	if (CurCLI == NULL) return(NULL);
	dirname = (UBYTE *)BADDR(CurCLI->cli_SetName);
	if (len < *dirname + 1) return(NULL);
	strncpy(buf, dirname + 1, *dirname);
	buf[*dirname] = 0;
	return(buf);
}

BPTR safe_read(name)
UBYTE *name;
{

	BPTR nfile;

	reqoff();	/* Turn off requester */

	nfile = Open(name, (LONG) MODE_NEWFILE);
	
	reqpop();

	return(nfile);
	
}

main(argc,argv) 	/* Main program section */
int argc;
char **argv;
{

	void wait_close();
	int i;
	struct WBArg *arg;
	struct WBStartup *argst;
	UBYTE *mat,*lc,*ls;

	GfxBase = OpenLibrary("graphics.library",0);
	if (GfxBase == NULL)
		getout("Can't open graphics.library");

	IntuitionBase = OpenLibrary("intuition.library",0);
	if (IntuitionBase == NULL)
		getout("Can't open intuition.library");

	IconBase = OpenLibrary("icon.library",0);
	if (IconBase == NULL)
		getout("Can't open icon.library");

	setupmasks();
	sysinit();
	
	mainbase.MNI = &maininter;
	mainbase.MPI = &maininfo;

	allowparent=TRUE;
	if (argc==0) {
		HomePath[0]='\0';
		path_setup(HomePath);	/* Get path of current id */

		argst = (struct WBStartup *) argv;
		arg = argst->sm_ArgList;
		for (i=0; i<argst->sm_NumArgs; i++, arg++) procarg(i,arg);

	} 
	else {
		mat = (UBYTE *) argv[0];
		strcpy(HomePath,mat);		
		path_setup(HomePath);
	}

	if (!create_screens()) getout("Insufficient space for screens\n");

	setreq();

	parselist();
	
	if (!LoadStrokeFont("simplex2")) getout("Cannot load font simplex2\n");

	if (GotProj) loadsets(LoadFile);		
	else if (DefName[0]!='\0') loaddefs(DefName);
		
	ScreenToFront(screen);		/* Face the music */

	wait_close();

}

BOOL Exit_Quit()
{

	getout("quit");
}

extern struct Remember *LibrMem,*LoadMem;

getout(s)
char *s;
{
	CloseDisplay();

	CloseLibrary(GfxBase);
	CloseLibrary(IntuitionBase);

	FreeRemember(&DrawStore, TRUE);
	FreePolyBuffer();
	if (DriverSegHook!=0) {
		(*mainbase.PLI->expungedriver)();    /* Let driver clean up */
		UnLoadSeg(DriverSegHook);
	}

	puts(s);
	exit(0);
}

/*
 *     +++
 *     +++
 *  +++++++++	complain and one-liner code
 *  +++++++++
 *     +++
 *     +++
 */

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
	nmi = (nm==NULL)?&commes[1]:&commes[0];
	commes[1].IText = pm;
	pmi = (pm==NULL)?&commes[0]:&commes[1];

	if (w!=NULL) {
		Forbid();
    		kep_IDCMPFlags = w->IDCMPFlags;
		kep_UserPort = w->UserPort;
		kep_WindowPort = w->WindowPort;
		kep_MessageKey = w->MessageKey;
		Permit();
	}
	
	reqwin = BuildSysRequest(w,&comptext[0],pmi,nmi,
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
					answer = !(GetGadget->GadgetID);
					breakout=TRUE;
					break;
				case GADGETUP:
					GetGadget=message->IAddress;
					answer = !(GetGadget->GadgetID);
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

