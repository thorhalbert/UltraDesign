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

#include "plotsetup.h"
#include <libraries/dos.h>
#include <libraries/dosextens.h>

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

BOOL Scroller();
#define MemRem(size,rem) AllocRemember(rem,size,0)

/*--------------------------------Load File--------------------------------*/

UBYTE *BPTR_TO_CPTR(inptr)
APTR	inptr;
{

	ULONG	med;

	med = (ULONG) inptr;
	med <<= 2;
	inptr = (APTR) med;

	return((UBYTE *) med);
}

ULONG CurrentPreset = 0;
VOID MemFree();

VOID LoadDriver(fname)
UBYTE *fname;
{

	struct PlotterSegment *plothook;
	int i;

	if (DriverSegHook!=0) {
		(*mainbase.PLI->expungedriver)();
		UnLoadSeg(DriverSegHook); /* Free old driver */
	}

	DriverSegHook = LoadSeg(fname);

	if (DriverSegHook==0) {
		complain(NULL,"Recover",5,0,0,0,
			"Could not find driver:",
			fname,NULL,NULL,NULL);
		return;
	}

	strcpy(DriverName,fname);
	
	plothook = (struct PlotterSegment *) BPTR_TO_CPTR(DriverSegHook);	
	mainbase.PLI = &plothook->PLI;
	mainbase.MNI = &maininter;
	mainbase.MPI = &maininfo;
	mainbase.MinFree = 40000;		/* Make sure 40 k free */
	maininter.getfreemem = MemFree;

/*	maininter.dataout = writedata;	*/

	if (!(*mainbase.PLI->initdriver)(&mainbase)) {
		printf("Driver init error\n");
		undo_driv();
		UnLoadSeg(DriverSegHook); /* Free driver */
		DriverSegHook = 0;
		return;
	}

	CurrentPreset = 0;
	if (mainbase.MPI->NumPresets>0) 
		(VOID) (*mainbase.PLI->dopreset)(CurrentPreset); 
				/* Get initial defaults */

	disp_driv();

}

LOCAL struct File_Entry *FDriv_Root=NULL,
		  *FDriv_Tail=NULL,
		  *Driv_Kep=NULL;
struct Remember *DrivMem=NULL;

BOOL Req_Driver()
{

	int a;
	UBYTE fname[255];

	if(!File_Sc(TRUE,"   Load   ",&DrivMem,DrivMask,"Plot Driver File",
		&FDriv_Root,&FDriv_Tail,DrivPath,TRUE,&Driv_Kep)) return;
	
	if ((a=strlen(DrivPath))==0) strcpy(fname,ArgText);
	else if (a>0&&DrivPath[a-1]==':')
			sprintf(fname,"%s%s",DrivPath,ArgText);
	else sprintf(fname,"%s/%s",DrivPath,ArgText);

	LoadDriver(fname);
		
}

extern UBYTE PreMask[];

VOID setupmasks()
{

	filereq_setup();

	strcpy(PreMask,"*");
	strcpy(DrivPath,"");
	strcpy(DrivMask,"*.pdr");
	strcpy(ViewMask,"*.uvw");
	strcpy(DefsMask,"*.upd");
	strcpy(SetsMask,"*.upu");
	strcpy(HatchMask,"*.uhat");
	strcpy(ViewPath,"");
	strcpy(DefsPath,"");
	strcpy(HatchPath,"");

}

LOCAL struct File_Entry *FView_Root=NULL,
		  *FView_Tail=NULL,
		  *View_Kep=NULL;
struct Remember *ViewMem=NULL;

BOOL Req_Load_View()
{

	return(File_Sc(TRUE,"   Load   ",&ViewMem,ViewMask,
		"Load View File",
		&FView_Root,&FView_Tail,ViewPath,TRUE,&View_Kep));

}


BOOL Req_Save_Sets()
{

	BOOL ret;

	ret = File_Sc(TRUE,"   Save   ",&ViewMem,SetsMask,
		"Save Pasteups",
		&FView_Root,&FView_Tail,ViewPath,TRUE,&View_Kep);

	if (ret) {
		FView_Root=FView_Tail = NULL;
		FreeRemember(&ViewMem,TRUE);
		ViewMem=NULL;
		View_Kep=NULL;
		return(TRUE);		/* The directory was changed */
	} else return(FALSE);

	return(ret);
}

BOOL Req_Load_Sets()
{

	BOOL ret;

	ret = File_Sc(TRUE,"   Load   ",&ViewMem,SetsMask,
		"Load Pasteups",
		&FView_Root,&FView_Tail,ViewPath,TRUE,&View_Kep);

	return(ret);

}

LOCAL struct File_Entry *FDefs_Root=NULL,
		  *FDefs_Tail=NULL,
		  *Defs_Kep=NULL;
struct Remember *DefsMem=NULL;

BOOL Req_Save_Defs()
{

	BOOL ret;

	ret = File_Sc(TRUE,"   Save   ",&DefsMem,DefsMask,
		"Save Device Setup",
		&FDefs_Root,&FDefs_Tail,DefsPath,TRUE,&Defs_Kep);

	if (ret) {
		FDefs_Root=FDefs_Tail = NULL;
		FreeRemember(&DefsMem,TRUE);
		DefsMem=NULL;
		Defs_Kep=NULL;
		return(TRUE);		/* The directory was changed */
	} else return(FALSE);

	return(ret);
}
BOOL Req_Load_Defs()
{

	BOOL ret;

	ret = File_Sc(TRUE,"   Load   ",&DefsMem,DefsMask,
		"Load Device Setup",
		&FDefs_Root,&FDefs_Tail,DefsPath,TRUE,&Defs_Kep);

	return(ret);
}

BOOL CheckAbort()
{

	if (Root_View==NULL) return(FALSE);

	return(complain("Abort","Proceed",1,0,0,0,
		"JETTISON CURRENT PASTEUPS?",NULL,NULL,NULL));
}
