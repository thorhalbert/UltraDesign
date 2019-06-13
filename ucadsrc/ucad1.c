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
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include "errors.h"
#include <workbench/startup.h>
#include <workbench/workbench.h>
/* #include <proto/icon.h> */

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

extern BOOL CheckAbort();
VOID abort();

/*-------------------------Global Variable Definitions-----------------------*/

ULONG begfree,endfree;
int cur_resource = 0;
ULONG defload[100];

/*-------------------------------Code Section--------------------------------*/

extern struct TextAttr UcadFont;	/* Desc of ucad font in screens.c */
struct TextFont *ufont;			/* The font when we open it */

extern BOOL menushere;
BOOL GotProj = FALSE,loadlib = FALSE;
UBYTE	LoadFile[120];

VOID procarg(anm,arg)
int anm;
struct WBArg *arg;
{

	struct DiskObject *diskobj=NULL;
	LONG olddir;
	char **toolarray;
	char *value;

	if (arg->wa_Lock==NULL&&anm>0) return;	
	if (arg->wa_Name[0]==' ') return;	/* A drawer... */

	if (arg->wa_Lock!=NULL) olddir = CurrentDir(arg->wa_Lock);

	diskobj = GetDiskObject(arg->wa_Name);	

	if (olddir!=NULL) CurrentDir(olddir);	/* Get back */

	if (diskobj==NULL) return;		/* Nothing done */

	toolarray = diskobj->do_ToolTypes;

	while (*toolarray!=NULL) printf("tool=%s\n",*toolarray++);

	toolarray = diskobj->do_ToolTypes;

	if (!GotProj&&diskobj->do_Type==WBPROJECT) {
		value = FindToolType(toolarray,"FILETYPE");
		if (value) {
			if (MatchToolValue(value,"2DOF_VIEW")||
			    (loadlib=MatchToolValue(value,"2DOF_LIBR"))) {
				sprintf(LoadFile,"%s",arg->wa_Name);
				if (arg->wa_Lock!=NULL) 
					olddir = CurrentDir(arg->wa_Lock);
				GotProj=TRUE;
				printf("load drawing %s, value %s %d\n",
					LoadFile,value,loadlib);
			}
		}
	}

		

	FreeDiskObject(diskobj);		/* Give memory back */
	
}

VOID *GfxBase,*IntuitionBase;
VOID *DiskfontBase;
extern VOID *IconBase;
extern BOOL allowparent;

main(argc,argv) 	/* Main program section */
int argc;
struct WBStartup **argv;
{
	FILE *fopen();
	char *fgets(),*stpblk();
	void wait_close();
	struct WBStartup *argst;
	UBYTE *mat;
	int i;
	struct WBArg *arg;

	sysinit();
	
	dialog_in = Input();
	dialog_out = Output();
	
	strcpy(dialog_def,"D");
	strcpy(dialog_wb1,"CNC:10/10/300/50/Dialog/");
	strcpy(dialog_wb2,"CON:10/10/300/50/Dialog");
	strcpy(dialog_mn1,"CNC:S%x/10/10/300/50/Dialog/");
	strcpy(dialog_mn2,"CON:10/10/300/50/Dialog");

	ViewPath[0]='\0';	/* Default to current directory */
	LibrPath[0]='\0';
	defload[0]='\0';
	
	begfree = TotAvail();

	GfxBase = OpenLibrary("graphics.library",0);
	if (GfxBase == NULL)
		abort(DIALOG_NOGRAPH);

	IntuitionBase = OpenLibrary("intuition.library",0);
	if (IntuitionBase == NULL)
		abort(DIALOG_NOINTUI);

	IconBase = OpenLibrary("icon.library",0);
	if (IconBase == NULL)
		abort(DIALOG_NOICON);

	DiskfontBase = OpenLibrary("diskfont.library",0);
	if (DiskfontBase == NULL)
		abort(DIALOG_NOFONTS);

	OpenRexxLib();
	OpenRexxPort();
	MakeUcadPort();

	ufont = OpenDiskFont(&UcadFont);	/* Open if possible */

	default_setups();
	allowparent = TRUE;

	getcwd(HomePath,PATHMAX);

	if (argc==0) {
		HomePath[0]='\0';
		path_setup(HomePath);	/* Get path of current id */

		argst = (struct WBStartup *) argv;
		arg = argst->sm_ArgList;
		for (i=0; i<argst->sm_NumArgs; i++, arg++) procarg(i,arg);

	} else {
		mat = (UBYTE *) argv[0];
		strcpy(HomePath,mat);		
		path_setup(HomePath);
	}
	
	set_hiscn();	

	parselist();
	
	Screen_Save_Time *= 60;

	if (!CreateTimer()) abort(DIALOG_NOTIMER);

	if (!open_winds()) abort(DIALOG_NOWINDOWS);

	setreq();

	if (!LoadStrokeFont("duplex1")) abort(DIALOG_NOSTROKE);
	
	if (!LoadStrokeFont("simplex2")) abort(DIALOG_NOSTROKE);
	
	Style_Default();

	fancystartup();

	getview(" "," ",defload,FALSE);
	Savnam = FALSE;

	if (GotProj) {
		if (!loadlib) ViewPath[0]='\0';	/* Default to current directory */
		else		LibrPath[0]='\0';
		
		getview(" "," ",LoadFile,loadlib);
		Savnam = FALSE;		/* Doesn't work yet */
	}

	SavePalette();		/* Whatever they are now */

	for (i=1; i<argc; i++) DM_Dofile(argv[i]);
	if (i<=1) DM_Dofile(defstart);

	if (!Win_Setup) RepaintWindow();

	InfoMess(Main_Mode,LINE_MESS_COPY);

	ActivateWindow(w);
	
	wait_close();
	/*if (menushere)	wait_close();
	else abort(DIALOG_NOFILE);*/


}

BOOL Quit_Out()
{

	if (CheckAbort()) return(FALSE);

	abort(DIALOG_OK);
}

VOID abort(code)
int code;
{


	DeleteTimer();

	CloseDisplay();

	FreeAllPools();
	FreeMenus();			/* Clear text of menus */
	FreeVertList();			/* Free line mode tables */

	GI_FreeRem();
	CleanFileReq();
	freelibs();			/* Get rid of everything! */
	FreePolyBuffer();
	
	/*endfree=TotAvail();
	printf("\nBegin memory %ld End memory %ld\n",begfree,endfree); */

	if (ufont) CloseFont(ufont);

	FreeRexx();		/* Close this out */

	CloseLibrary(GfxBase);
	CloseLibrary(IntuitionBase);
	CloseLibrary(IconBase);
	CloseLibrary(DiskfontBase);

	exit(code);
}
