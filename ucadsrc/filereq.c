/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	filereq - File requester
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
 *	1	28-Jun-87 SH	Begin writing
 *	2	22-Aug-87 SH	Insure that libraries get rid of their 
 *				 context on a save, so that the new file
 *				 will appear the next time requester used
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*-------------------------Load Font-------------------------------------*/

LOCAL struct File_Entry *FFont_Root=NULL,
		  *FFont_Tail=NULL;

LOCAL struct Remember *FntMem=NULL;
LOCAL struct File_Entry *Font_File=NULL;
extern UBYTE FontMask[];

BOOL Load_Font()
{

	UBYTE *fs;
	struct Text_Fonts *infont;

	if (!File_Sc(FALSE,"   Load   ",&FntMem,FontMask,"Load Stroke Font File",
		&FFont_Root,&FFont_Tail,StrokePath,FALSE,&Font_File)) return(FALSE);

	if ((fs=strrchr(ArgText,'.'))!=NULL) *fs='\0';	/* Get rid of .ufnt */
	
	infont = Stroke_Search(ArgText);
	if (infont!=NULL) {
		complain(NULL,"Recover",4,3,0,0,
			"That font was already loaded!",
			ArgText,NULL,NULL);
		return(FALSE);
	}
	
	if (!LoadStrokeFont(ArgText)) 	return(FALSE);

	return(TRUE);

}

/*-------------------------Load Hatch-------------------------------------*/

LOCAL struct File_Entry *FHatch_Root=NULL,
		  *FHatch_Tail=NULL;

LOCAL struct Remember *HatchMem=NULL;
LOCAL struct File_Entry *Hatch_File=NULL;
extern UBYTE HatchMask[];

BOOL Load_Hatch()
{

	UBYTE *fs;
	struct Text_Hatchs *inhatch;

	if (!File_Sc(FALSE,"   Load   ",&HatchMem,HatchMask,"Load Hatch Pattern File",
		&FHatch_Root,&FHatch_Tail,HatchPath,FALSE,&Hatch_File)) return(FALSE);

	if ((fs=strrchr(ArgText,'.'))!=NULL) *fs='\0';	/* Get rid of .uhat */
	
	inhatch = Hatch_Search_File(ArgText);
	if (inhatch!=NULL) {
		complain(NULL,"Recover",4,3,0,0,
			"That hatch file was already loaded!",
			ArgText,NULL,NULL);
		return(FALSE);
	}
	
	if (!LoadHatch(ArgText)) 	return(FALSE);

	return(TRUE);

}

/*--------------------------------Load File--------------------------------*/

LOCAL struct File_Entry *FLoad_Root=NULL,
		  *FLoad_Tail=NULL,
		  *Load_Kep=NULL;
struct Remember *LoadMem=NULL;

BOOL Req_Load_View()
{


	return(File_Sc(TRUE,"   Load   ",&LoadMem,ViewMask,"Load Drawing File",
		&FLoad_Root,&FLoad_Tail,ViewPath,TRUE,&Load_Kep));

}

BOOL Req_Mrg_View()
{

	return(File_Sc(TRUE,"  Merge   ",&LoadMem,ViewMask,"Merge Drawing File",
		&FLoad_Root,&FLoad_Tail,ViewPath,TRUE,&Load_Kep));

}

BOOL Req_Save_View()
{

	BOOL ret;

	ret=File_Sc(TRUE,"   Save   ",&LoadMem,ViewMask,"Save Drawing File",
		&FLoad_Root,&FLoad_Tail,ViewPath,TRUE,&Load_Kep);

	CleanOutArgText();

	if (ret) {
		FLoad_Root=FLoad_Tail = NULL;
		GI_DoFree(&LoadMem);
		LoadMem=NULL;
		Load_Kep=NULL;
		return(TRUE);		/* The directory was changed */
	} else return(FALSE);
}

BOOL Req_Clip_View()
{

	BOOL ret;

	ret=File_Sc(TRUE,"   Save   ",&LoadMem,ViewMask,"Save Clip File",
		&FLoad_Root,&FLoad_Tail,ViewPath,TRUE,&Load_Kep);

	CleanOutArgText();

	if (ret) {
		FLoad_Root=FLoad_Tail = NULL;
		GI_DoFree(&LoadMem);
		LoadMem=NULL;
		Load_Kep=NULL;
		return(TRUE);		/* The directory was changed */
	} else return(FALSE);
}

BOOL CheckAbort()
{

	if (!drawingdirty) return(FALSE);

	return(complain("ABORT","PROCEED",5,0,0,0,
		"Current Work not Saved!",NULL,NULL,NULL));

}

LOCAL struct File_Entry *FLibr_Root=NULL,
		  *FLibr_Tail=NULL,
		  *Libr_Kep=NULL;
struct Remember *LibrMem=NULL;

BOOL Req_Use_Libr()
{

	return(File_Sc(TRUE,"   Use    ",&LibrMem,LibrMask,
		"Use Drawing Library",
		&FLibr_Root,&FLibr_Tail,LibrPath,TRUE,&Libr_Kep));

}

BOOL Req_Load_Libr()
{

	return(File_Sc(TRUE,"   Load   ",&LibrMem,LibrMask,
		"Load Drawing Library",
		&FLibr_Root,&FLibr_Tail,LibrPath,TRUE,&Libr_Kep));

}

BOOL Req_Save_Libr()
{

	BOOL ret;

	ret = File_Sc(TRUE,"   Save   ",&LibrMem,LibrMask,
		"Save Drawing Library",
		&FLibr_Root,&FLibr_Tail,LibrPath,TRUE,&Libr_Kep);

	CleanOutArgText();

	if (ret) {
		FLibr_Root=FLibr_Tail = NULL;
		GI_DoFree(&LibrMem);
		LibrMem=NULL;
		Libr_Kep=NULL;
		return(TRUE);		/* The directory was changed */
	} else return(FALSE);

}

BOOL Load_View()
{

	UBYTE fname[255];
	int a;

	if (CheckAbort()) return(FALSE);

	if (!Req_Load_View()) return(FALSE);

	if ((a=strlen(ViewPath))==0) sprintf(fname,"%s",ArgText);
	else if (a>0&&ViewPath[a-1]==':')sprintf(fname,"%s%s",ViewPath,ArgText);
	else sprintf(fname,"%s/%s",ViewPath,ArgText);

	getview(ViewPath,ArgText,fname,FALSE);

	return(FALSE);
}

BOOL Merge_View(ingrp)
GROUP_A *ingrp;
{

	UBYTE fname[255];
	int a;

	if (!Req_Mrg_View()) return(FALSE);

	if ((a=strlen(ViewPath))==0) sprintf(fname,"%s",ArgText);
	else if (a>0&&ViewPath[a-1]==':')sprintf(fname,"%s%s",ViewPath,ArgText);
	else sprintf(fname,"%s/%s",ViewPath,ArgText);

	mrgview(fname,ArgText,ingrp);

	return(FALSE);
}

BOOL Root_Merge()
{ 	return(Merge_View(Root_Group)); }

BOOL Curr_Merge()
{ 	return(Merge_View(Current_Group)); }

BOOL Load_Libr()
{

	UBYTE fname[255];
	int a;

	if (CheckAbort()) return(FALSE);

	if (!Req_Load_Libr()) return(FALSE);

	if ((a=strlen(LibrPath))==0) sprintf(fname,"%s",ArgText);
	else if (a>0&&LibrPath[a-1]==':')sprintf(fname,"%s%s",LibrPath,ArgText);
	else sprintf(fname,"%s/%s",LibrPath,ArgText);

	getview(LibrPath,ArgText,fname,TRUE);

	return(FALSE);
}

BOOL Clear_Work(libmode)
BOOL libmode;
{

	if (!indialog) 
		if (CheckAbort()) return(FALSE);

	SExitSub(NULL);		/* Clear any secondary modes */

	FreeOnlyDraw();		/* Cut out the drawing elements */

	Root_Node=NULL;			/* Clear lists now */
	Last_Node=NULL;
	CurrentRoller = NULL;		/* Clear the roller */

	drawingdirty=FALSE;

	Library_Mode = libmode;

	setpath();		/* Fix up the current group information */

	fullzoomout();
	RepaintWindow();

	InventoryAll(FALSE);	/* Fix up the counts */

	Savnam = FALSE;

}

BOOL Clear_View()
{ 	return(Clear_Work(FALSE)); }

BOOL Clear_Libr()
{	 return(Clear_Work(TRUE)); }

BOOL Save_View(optim)
BOOL optim;
{

	UBYTE fname[255],*fs;
	int a;

#ifdef DEMO

	DispMess("Sorry - Demos Cannot Save Files");
	return(FALSE);
#else
	if (!Req_Save_View()) return(FALSE);

	if ((fs=strrchr(ArgText,'.'))!=NULL)
		if (match(fs,".uvw")) *fs='\0';
	
	if ((fs=strrchr(ArgText,'.'))==NULL) {
		if (strlen(ArgText)>24) ArgText[24]='\0';
		sprintf(fname,"%s.uvw",ArgText);
		sprintf(ArgText,fname);
	}
		
	a=strlen(ViewPath);
	fname[0]='\0';
	if (a>0&&ViewPath[a-1]==':')sprintf(fname,"%s",ViewPath);
	else sprintf(fname,"%s/",ViewPath);

	putview(ViewPath,ArgText,fname,ArgText,FALSE,optim,FALSE);

	return(FALSE);
#endif
}

BOOL Clip_View()
{

	UBYTE fname[255],*fs;
	int a;

#ifdef DEMO

	DispMess("Sorry - Demos Cannot Save Files");
	return(FALSE);
#else
	if (!Req_Clip_View()) return(FALSE);

	if ((fs=strrchr(ArgText,'.'))!=NULL)
		if (match(fs,".uvw")) *fs='\0';
	
	if ((fs=strrchr(ArgText,'.'))==NULL) {
		if (strlen(ArgText)>24) ArgText[24]='\0';
		sprintf(fname,"%s.uvw",ArgText);
		sprintf(ArgText,fname);
	}
		
	a=strlen(ViewPath);
	fname[0]='\0';
	if (a>0&&ViewPath[a-1]==':')sprintf(fname,"%s",ViewPath);
	else sprintf(fname,"%s/",ViewPath);

	putview(ViewPath,ArgText,fname,ArgText,TRUE,TRUE,FALSE);

	return(FALSE);
#endif
}

BOOL Save_Libr(optim)
BOOL optim;
{

	UBYTE fname[255],*fs;
	int a;

#ifdef DEMO

	DispMess("Sorry - Demos Cannot Save Files");
	return(FALSE);
#else
	if (!Req_Save_Libr()) return(FALSE);

	if ((fs=strrchr(ArgText,'.'))!=NULL)
		if (match(fs,".ulb")) *fs='\0';
	
	if ((fs=strrchr(ArgText,'.'))==NULL) {
		if (strlen(ArgText)>24) ArgText[24]='\0';
		sprintf(fname,"%s.ulb",ArgText);
		sprintf(ArgText,fname);
	}
		
	a=strlen(LibrPath);
	fname[0]='\0';
	if (a>0&&LibrPath[a-1]==':')sprintf(fname,"%s",LibrPath);
	else sprintf(fname,"%s/",LibrPath);

	putview(LibrPath,ArgText,fname,ArgText,FALSE,optim,TRUE);

	return(FALSE);
#endif
}

BOOL	O_Save_Libr()	{ return(Save_Libr(TRUE)); }
BOOL	N_Save_Libr()	{ return(Save_Libr(FALSE)); }

BOOL	O_Save_View()	{ return(Save_View(TRUE)); }
BOOL	N_Save_View()	{ return(Save_View(FALSE)); }

BOOL	Do_Update(optim)
BOOL 	optim;
{

	UBYTE fname[300];
	int a;

	if (!Savnam) return(FALSE);		/* No filename */

	a=strlen(Savepath);
	fname[0]='\0';
	if (a>0&&Savepath[a-1]==':')sprintf(fname,"%s",Savepath);
	else sprintf(fname,"%s/",Savepath);

	putview(Savepath,Savefile,fname,Savefile,FALSE,optim,Savlib);

	return(TRUE);
	
}

BOOL 	O_Update()	{ return(Do_Update(TRUE)); }
BOOL 	N_Update()	{ return(Do_Update(FALSE)); }

VOID CleanFileReq()
{

	if (FntMem!=NULL) 	GI_DoFree(&FntMem);
	if (HatchMem!=NULL)	GI_DoFree(&HatchMem);
	if (LoadMem!=NULL)	GI_DoFree(&LoadMem);
	if (LibrMem!=NULL)	GI_DoFree(&LibrMem);
	
}
