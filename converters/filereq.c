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
#include <libraries/dos.h>
#include <libraries/dosextens.h>

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

BOOL CheckAbort();
/*-------------------------External Symbol Definitions-----------------------*/

UBYTE *GI_DoAlloc();
BOOL Scroller();

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

/*------------------------Set up scroller for styles------------------------*/


BOOL CheckAbort()
{

	if (!drawingdirty) return(FALSE);

	return(complain("ABORT","PROCEED",5,0,0,0,
		"Current Work not Saved!",NULL,NULL,NULL));

}

/*--------------------------------Load File--------------------------------*/

LOCAL struct File_Entry *FLoad_Root=NULL,
		  *FLoad_Tail=NULL,
		  *Load_Kep=NULL;
struct Remember *LoadMem=NULL;

BOOL Req_Load_View()
{


	return(File_Sc(TRUE,"   Load   ",&LoadMem,ViewMask,
		"Load UltraDesign Drawing File",
		&FLoad_Root,&FLoad_Tail,ViewPath,TRUE,&Load_Kep));

}

BOOL Req_Save_View()
{

	BOOL ret;

	ret=File_Sc(TRUE,"   Save   ",&LoadMem,ViewMask,
		"Save UltraDesign Drawing File",
		&FLoad_Root,&FLoad_Tail,ViewPath,TRUE,&Load_Kep);

	if (ret) {
		FLoad_Root=FLoad_Tail = NULL;
		GI_DoFree(&LoadMem);
		LoadMem=NULL;
		Load_Kep=NULL;
		return(TRUE);		/* The directory was changed */
	} else return(FALSE);
}

UBYTE	DxfMask[40];
UBYTE	DxfPath[256];
UBYTE	IntroMask[40];
UBYTE	IntroPath[256];
UBYTE	ILBMMask[40];
UBYTE	ILBMPath[256];
UBYTE	HatMask[40];
UBYTE	AegisMask[40];
UBYTE	AegisPath[255];
UBYTE	DxyMask[40];
UBYTE	DxyPath[256];
UBYTE	HpglMask[40];
UBYTE	HpglPath[256];


/*--------------------------------Load Dxf--------------------------------*/

LOCAL struct File_Entry *FDxf_Root=NULL,
		  *FDxf_Tail=NULL,
		  *Dxf_Kep=NULL;
struct Remember *DxfMem=NULL;

BOOL Req_Load_Dxf_View()
{


	return(File_Sc(TRUE,"   Load   ",&DxfMem,DxfMask,"Load DXF File",
		&FDxf_Root,&FDxf_Tail,DxfPath,TRUE,&Dxf_Kep));

}

BOOL Req_Save_Dxf()
{

	BOOL ret;

	ret=File_Sc(TRUE,"   Save   ",&DxfMem,DxfMask,"Save DXF File",
		&FDxf_Root,&FDxf_Tail,DxfPath,TRUE,&Dxf_Kep);

	if (ret) {
		FDxf_Root=FDxf_Tail = NULL;
		GI_DoFree(&DxfMem);
		DxfMem=NULL;
		Dxf_Kep=NULL;
		return(TRUE);		/* The directory was changed */
	} else return(FALSE);
}

/*--------------------------------Load Dxy--------------------------------*/

LOCAL struct File_Entry *FDxy_Root=NULL,
		  *FDxy_Tail=NULL,
		  *Dxy_Kep=NULL;
struct Remember *DxyMem=NULL;

BOOL Req_Load_Dxy_View()
{


	return(File_Sc(TRUE,"   Load   ",&DxyMem,DxyMask,"Load DXY File",
		&FDxy_Root,&FDxy_Tail,DxyPath,TRUE,&Dxy_Kep));

}

/*--------------------------------Load Hpgl--------------------------------*/

LOCAL struct File_Entry *FHpgl_Root=NULL,
		  *FHpgl_Tail=NULL,
		  *Hpgl_Kep=NULL;
struct Remember *HpglMem=NULL;

BOOL Req_Load_Hpgl_View()
{


	return(File_Sc(TRUE,"   Load   ",&HpglMem,HpglMask,"Load HPGL File",
		&FHpgl_Root,&FHpgl_Tail,HpglPath,TRUE,&Hpgl_Kep));

}

/*--------------------------------Load Intro-------------------------------*/

LOCAL struct File_Entry *FIntro_Root=NULL,
		  *FIntro_Tail=NULL,
		  *Intro_Kep=NULL;
struct Remember *IntroMem=NULL;

BOOL Req_Load_Intro_View()
{


	return(File_Sc(TRUE,"   Load   ",&IntroMem,IntroMask,"Load IntroCAD File",
		&FIntro_Root,&FIntro_Tail,IntroPath,TRUE,&Intro_Kep));

}

BOOL Req_Save_Intro_View()
{

	BOOL ret;

	ret=File_Sc(TRUE,"   Save   ",&IntroMem,IntroMask,"Save IntroCAD File",
		&FIntro_Root,&FIntro_Tail,IntroPath,TRUE,&Intro_Kep);

	if (ret) {
		FIntro_Root=FIntro_Tail = NULL;
		GI_DoFree(&IntroMem);
		IntroMem=NULL;
		Intro_Kep=NULL;
		return(TRUE);		/* The directory was changed */
	} else return(FALSE);
}

/*--------------------------------Load Aegis-------------------------------*/

LOCAL struct File_Entry *FAegis_Root=NULL,
		  *FAegis_Tail=NULL,
		  *Aegis_Kep=NULL;
struct Remember *AegisMem=NULL;

BOOL Req_Load_Aegis_View()
{


	return(File_Sc(TRUE,"   Load   ",&AegisMem,AegisMask,"Load Aegis Draw File",
		&FAegis_Root,&FAegis_Tail,AegisPath,TRUE,&Aegis_Kep));

}

BOOL Req_Save_Aegis_View()
{

	BOOL ret;

	ret=File_Sc(TRUE,"   Save   ",&AegisMem,AegisMask,"Save Aegis Draw File",
		&FAegis_Root,&FAegis_Tail,AegisPath,TRUE,&Aegis_Kep);

	if (ret) {
		FAegis_Root=FAegis_Tail = NULL;
		GI_DoFree(&AegisMem);
		AegisMem=NULL;
		Aegis_Kep=NULL;
		return(TRUE);		/* The directory was changed */
	} else return(FALSE);
}

/*--------------------------------Load ILBM-------------------------------*/

LOCAL struct File_Entry *FILBM_Root=NULL,
		  *FILBM_Tail=NULL,
		  *ILBM_Kep=NULL;
struct Remember *ILBMMem=NULL;

BOOL Req_Load_ILBM_View()
{


	return(File_Sc(TRUE,"   Load   ",&ILBMMem,ILBMMask,"Load ILBM Iff File",
		&FILBM_Root,&FILBM_Tail,ILBMPath,TRUE,&ILBM_Kep));

}

BOOL Req_Save_ILBM_View()
{

	BOOL ret;

	ret=File_Sc(TRUE,"   Save   ",&ILBMMem,ILBMMask,"Save ILBM Iff File",
		&FILBM_Root,&FILBM_Tail,ILBMPath,TRUE,&ILBM_Kep);

	if (ret) {
		FILBM_Root=FILBM_Tail = NULL;
		GI_DoFree(&ILBMMem);
		ILBMMem=NULL;
		ILBM_Kep=NULL;
		return(TRUE);		/* The directory was changed */
	} else return(FALSE);
}

BOOL Save_ILBM()
{

	UBYTE fname[255],*fs;
	int a;

#ifdef DEMO

	DispMess("Sorry - Demos Cannot Save Files");
	return(FALSE);
#else
	if (!Req_Save_ILBM_View()) return(FALSE);

	if ((fs=strrchr(ArgText,'.'))!=NULL)
		if (match(fs,".pic")) *fs='\0';
	
	if ((fs=strrchr(ArgText,'.'))==NULL) {
		if (strlen(ArgText)>24) ArgText[24]='\0';
		sprintf(fname,"%s.pic",ArgText);
		sprintf(ArgText,fname);
	}
		
	a=strlen(ILBMPath);
	fname[0]='\0';
	if (a>0&&ILBMPath[a-1]==':')sprintf(fname,"%s",ILBMPath);
	else sprintf(fname,"%s/",ILBMPath);

	putilbm(ILBMPath,ArgText,fname,ArgText);

	return(FALSE);
#endif
}

BOOL Load_View()
{

	UBYTE fname[255];
	int a;

	if (!Req_Load_View()) return(FALSE);

	if ((a=strlen(ViewPath))==0) sprintf(fname,"%s",ArgText);
	else if (a>0&&ViewPath[a-1]==':')sprintf(fname,"%s%s",ViewPath,ArgText);
	else sprintf(fname,"%s/%s",ViewPath,ArgText);

	getview(ViewPath,ArgText,fname,FALSE);

	return(FALSE);
}

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

BOOL	N_Save_View()	{ return(Save_View(FALSE)); }

BOOL Load_Dxf()
{

	UBYTE fname[255];
	int a;

	if (!Req_Load_Dxf_View()) return(FALSE);

	if ((a=strlen(DxfPath))==0) sprintf(fname,"%s",ArgText);
	else if (a>0&&DxfPath[a-1]==':')sprintf(fname,"%s%s",DxfPath,ArgText);
	else sprintf(fname,"%s/%s",DxfPath,ArgText);

	quietupdate = TRUE;
	dodxf(fname);
	quietupdate = FALSE;
	
	view_defaults();
	RepaintWindow();

	return(FALSE);
}

BOOL Save_Dxf(optim)
BOOL optim;
{

	UBYTE fname[255],*fs;
	int a;

#ifdef DEMO

	DispMess("Sorry - Demos Cannot Save Files");
	return(FALSE);
#else
	if (!Req_Save_Dxf()) return(FALSE);

	if ((fs=strrchr(ArgText,'.'))!=NULL)
		if (match(fs,".dxf")) *fs='\0';
	
	if ((fs=strrchr(ArgText,'.'))==NULL) {
		if (strlen(ArgText)>24) ArgText[24]='\0';
		sprintf(fname,"%s.dxf",ArgText);
		sprintf(ArgText,fname);
	}
		
	a=strlen(DxfPath);
	fname[0]='\0';
	if (a>0&&DxfPath[a-1]==':') sprintf(fname,"%s%s",DxfPath,ArgText);
	else sprintf(fname,"%s/%s",DxfPath,ArgText);

	doDXF(fname);

	return(FALSE);
#endif
}

BOOL Load_Intro()
{

	UBYTE fname[255];
	int a;

	if (!Req_Load_Intro_View()) return(FALSE);

	if ((a=strlen(IntroPath))==0) sprintf(fname,"%s",ArgText);
	else if (a>0&&IntroPath[a-1]==':')sprintf(fname,"%s%s",IntroPath,ArgText);
	else sprintf(fname,"%s/%s",IntroPath,ArgText);

	quietupdate = TRUE;
	dointro(fname);
	quietupdate = FALSE;

	view_defaults();
	RepaintWindow();

	return(FALSE);
}

BOOL Load_Dxy()
{

	UBYTE fname[255];
	int a;

	if (!Req_Load_Dxy_View()) return(FALSE);

	if ((a=strlen(DxyPath))==0) sprintf(fname,"%s",ArgText);
	else if (a>0&&DxyPath[a-1]==':')sprintf(fname,"%s%s",DxyPath,ArgText);
	else sprintf(fname,"%s/%s",DxyPath,ArgText);

	quietupdate = TRUE;
	dodxy(fname);
	quietupdate = FALSE;

	view_defaults();
	RepaintWindow();

	return(FALSE);
}

BOOL Load_Hpgl()
{

	UBYTE fname[255];
	int a;

	if (!Req_Load_Hpgl_View()) return(FALSE);

	if ((a=strlen(HpglPath))==0) sprintf(fname,"%s",ArgText);
	else if (a>0&&HpglPath[a-1]==':')sprintf(fname,"%s%s",HpglPath,ArgText);
	else sprintf(fname,"%s/%s",HpglPath,ArgText);

	quietupdate = TRUE;
	dohpgl(fname);
	quietupdate = FALSE;

	view_defaults();
	RepaintWindow();

	return(FALSE);
}


BOOL Load_Aegis()
{

	UBYTE fname[255];
	int a;

	if (!Req_Load_Aegis_View()) return(FALSE);

	if ((a=strlen(AegisPath))==0) sprintf(fname,"%s",ArgText);
	else if (a>0&&AegisPath[a-1]==':')sprintf(fname,"%s%s",AegisPath,ArgText);
	else sprintf(fname,"%s/%s",AegisPath,ArgText);

	quietupdate = TRUE;
	doaegis(fname);
	quietupdate = FALSE;

	view_defaults();
	RepaintWindow();

	return(FALSE);
}


/*--------------------------------Load Hatch-------------------------------*/

LOCAL struct File_Entry *FHat_Root=NULL,
		  *FHat_Tail=NULL,
		  *Hat_Kep=NULL;
struct Remember *HatMem=NULL;

BOOL Req_Load_Hat_View()
{


	return(File_Sc(TRUE,"   Load   ",&HatMem,HatMask,"Load Hatch File",
		&FHat_Root,&FHat_Tail,HatchPath,TRUE,&Hat_Kep));

}

BOOL Req_Save_Hat_View()
{

	BOOL ret;

	ret=File_Sc(TRUE,"   Save   ",&HatMem,HatMask,"Save Hatch File",
		&FHat_Root,&FHat_Tail,HatchPath,TRUE,&Hat_Kep);

	if (ret) {
		FHat_Root=FHat_Tail = NULL;
		GI_DoFree(&HatMem);
		HatMem=NULL;
		Hat_Kep=NULL;
		return(TRUE);		/* The directory was changed */
	} else return(FALSE);
}

BOOL Save_Hat()
{

	UBYTE fpart[255];
	UBYTE fname[255],*fs;
	int a;

#ifdef DEMO

	DispMess("Sorry - Demos Cannot Save Files");
	return(FALSE);
#else
	if (!Req_Save_Hat_View()) return(FALSE);

	if ((fs=strrchr(ArgText,'.'))!=NULL)
		if (match(fs,".uhat")) *fs='\0';
	
	strcpy(fpart,ArgText);
	if ((fs=strrchr(ArgText,'.'))==NULL) {
		if (strlen(ArgText)>24) ArgText[24]='\0';
		sprintf(fname,"%s.uhat",ArgText);
		sprintf(ArgText,fname);
	} else {
		*fs = '\0';
		strcpy(fpart,ArgText);
		*fs = '.';
	}
	
		
	a=strlen(HatchPath);
	fname[0]='\0';
	if (a>0&&HatchPath[a-1]==':')sprintf(fname,"%s",HatchPath);
	else sprintf(fname,"%s/",HatchPath);

	puthat(HatchPath,fpart,fname,ArgText);

	return(FALSE);
#endif
}

VOID dorepaint()
{

	view_defaults();
	RepaintWindow();

}

