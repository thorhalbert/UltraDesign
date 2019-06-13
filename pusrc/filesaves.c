/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	filesaves.c - save definitions and pasteup files
 *
 */

/*
 *	Copyright (C) 1986, 1987, 1988 by WURBsoft Inc., and Scott Halbert.
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
 *	1	 5-Nov-87 SH	New Composition
 *	2	 2-Aug-88 SH	Create new
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "plotsetup.h"
#include "mempools.h"
#include <libraries/dos.h>
#include <graphics/gfxmacros.h>
#include <workbench/workbench.h>
#include "iff.h"
#include <ctype.h>

IFFP getview();

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

extern UBYTE DriverName[];
extern BOOL save_r[4];
UBYTE obuf[255];

UBYTE *RootAble(fname)
UBYTE *fname;
{
	BOOL theymatch;
	int a,b;
	
	theymatch = TRUE;
	
	for (a=0,b=0; a<strlen(HomePath); a++,b++) {
		while (fname[b]==' ') b++;
		if (toupper(fname[b])==toupper(HomePath[a])) continue;
		theymatch = FALSE;
	}
	
	if (!theymatch) return(fname);

	while (fname[b]=='/') b++;	
	sprintf(obuf,"$%s",&fname[b]);
	
	return(obuf);
}
		
VOID DumpDefinitions(ofile,fsave)
BOOL fsave;
FILE *ofile;
{

	fprintf(ofile,"*BEGIN SETUP\n");
	if (fsave||save_r[3]) {
		fprintf(ofile,"DRIVER/%s\n",RootAble(DriverName));
		fprintf(ofile,"PLOTTER/%s\n",	mainbase.MPI->PlotName);
		fprintf(ofile,"PLOTDEV/%s\n",	PlotName);
		fprintf(ofile,"AUTOPENS/%d\n",	(int) mainbase.MPI->AutoPens);
		fprintf(ofile,"TYPE/%d\n",	mainbase.MPI->PlotType);
		fprintf(ofile,"FLAGS/%x\n",	mainbase.MPI->PlotFlags);
	}
	fprintf(ofile,"COMMENT/%s\n",	mainbase.MPI->Comment);
	if (fsave||save_r[0]) fprintf(ofile,"BED/%d,%d,%d,%d\n",
		mainbase.MPI->MinCX,
		mainbase.MPI->MaxCX,
		mainbase.MPI->MinCY,
		mainbase.MPI->MaxCY);
	if (fsave||save_r[1]) fprintf(ofile,"STEPSIZES/%.10g,%.10g\n",
		mainbase.MPI->StepX,
		mainbase.MPI->StepY);
	if (fsave||save_r[2]) fprintf(ofile,"SHEET/%.10g,%.10g,%.10g,%.10g\n",
		MinIX,MaxIX,MinIY,MaxIY);
	fprintf(ofile,"*END SETUP\n");

}

VOID DumpPasteup(ofile)
FILE *ofile;
{

	BOOL any=FALSE;
	struct ViewContext *looper;

	for (looper=Root_View; looper!=NULL; looper=looper->next) {
		if (!any) fprintf(ofile,"*BEGIN PASTEUP\n");
		any = TRUE;

		fprintf(ofile,"VIEWFILE/%s\n",RootAble(looper->FullPath));
		fprintf(ofile,"ORIGIN/%.10g,%.10g\n",
			looper->XNewOrigin - SheetMinIX,
			looper->YNewOrigin - SheetMinIY);
		fprintf(ofile,"ROTATION/%.10g\n",looper->ViewRot);
		fprintf(ofile,"SCALE/%.10g,1.0\n",looper->ViewScale);
		fprintf(ofile,"FLAGS/%x\n",looper->ViewFlags);
	}

	if (any) fprintf(ofile,"*END PASTEUP\n");

}

VOID SaveDefs()
{

	int a;
	UBYTE fname[255];
	FILE *ofile;
	UBYTE *fs;

	if (DriverSegHook==NULL) return;
	if (!Req_Save_Defs()) return;

	if ((fs=strrchr(ArgText,'.'))!=NULL)
		if (match(fs,".upd")) *fs='\0';
	
	if ((fs=strrchr(ArgText,'.'))==NULL) {
		if (strlen(ArgText)>24) ArgText[24]='\0';
		sprintf(fname,"%s.upd",ArgText);
		sprintf(ArgText,fname);
	}
		
	if ((a=strlen(DefsPath))==0) sprintf(fname,"%s",ArgText);
	else if (a>0&&DefsPath[a-1]==':')sprintf(fname,"%s%s",DefsPath,ArgText);
	else sprintf(fname,"%s/%s",DefsPath,ArgText);

	ofile = fopen(fname,"w");
	if (ofile==NULL) {
		DisplayBeep(screen);
		return;
	}

	DumpDefinitions(ofile,FALSE);

	fclose(ofile);

	return;

}

VOID SaveSets()
{

	int a;
	UBYTE fname[255],fname2[255],*savvec;
	FILE *ofile;
	UBYTE *fs;
	struct	DiskObject *newicon;

	if (DriverSegHook==NULL) return;
	if (!Req_Save_Sets()) return;

	if ((fs=strrchr(ArgText,'.'))!=NULL)
		if (match(fs,".upu")) *fs='\0';
	
	if ((fs=strrchr(ArgText,'.'))==NULL) {
		if (strlen(ArgText)>24) ArgText[24]='\0';
		sprintf(fname,"%s.upu",ArgText);
		sprintf(ArgText,fname);
	}
		
	if ((a=strlen(ViewPath))==0) sprintf(fname,"%s",ArgText);
	else if (a>0&&ViewPath[a-1]==':')sprintf(fname,"%s%s",ViewPath,ArgText);
	else sprintf(fname,"%s/%s",ViewPath,ArgText);

	ofile = fopen(fname,"w");
	if (ofile==NULL) {
		DisplayBeep(screen);
		return;
	}

	DumpDefinitions(ofile,TRUE);
	DumpPasteup(ofile);

	fclose(ofile);

	newicon = GetDiskObject(fname);		/* See if already an icon */
	if (newicon==NULL) {
		newicon = GetDiskObject(MakeFname(TemplatePath,
			"pasteup-data",NULL));
				/* Get our template */
		if (newicon!=NULL) {
			strcpy(fname2,MakeFname(HomePath,"PasteUp",NULL));
			savvec = newicon->do_DefaultTool;
			newicon->do_DefaultTool = fname2;
			newicon->do_CurrentX = NO_ICON_POSITION;
			newicon->do_CurrentY = NO_ICON_POSITION;
			PutDiskObject(fname,newicon);	/* Make icon */
			newicon->do_DefaultTool = savvec;
		}
	}
	if (newicon!=NULL) FreeDiskObject(newicon);	/* Toss this */
	
	return;

}

UBYTE rbuf[255];

BOOL getline(iov)
FILE *iov;
{

	if (feof(iov)) return(FALSE);
	
	if (fgets(rbuf, 250, iov)==NULL) return(FALSE);
	rbuf[strlen(rbuf)-1] = '\0';
	
	return(TRUE);
}

BOOL parsedefs(ifile)
UBYTE *ifile;
{
	UBYTE *usefil;
	int rv;

	if (strncmp(rbuf,"*END SETUP",10)==0) return(FALSE);
	
	if (strncmp(rbuf,"*",1)==0) return(TRUE);
	if (strncmp(rbuf,"#",1)==0) return(TRUE);
	
	if (strncmp(rbuf,"DRIVER/",7)==0) {
		usefil = &rbuf[7];
		if (*usefil=='$') usefil=MakeFname(HomePath,usefil+1,NULL);
		LoadDriver(usefil);
		if (DriverSegHook==NULL) {
			complain(NULL,"Recover",5,0,0,0,
				 "Unable to find driver from defs file",
				 ifile,usefil,NULL);
			return(FALSE);
		}
		return(TRUE);
	}
	
	if (strncmp(rbuf,"PLOTTER/",8)==0) {
		sprintf(mainbase.MPI->PlotName,"%s",&rbuf[8]);
		return(TRUE);
	}

	if (strncmp(rbuf,"PLOTDEV/",8)==0) {
		sprintf(PlotName,"%s",&rbuf[8]);
		return(TRUE);
	}

	if (strncmp(rbuf,"COMMENT/",8)==0) {
		sprintf(mainbase.MPI->Comment,"%s",&rbuf[8]);
		return(TRUE);
	}
	
	if (strncmp(rbuf,"BED/",4)==0) {
		sscanf(&rbuf[4],"%d,%d,%d,%d",
			&mainbase.MPI->MinCX,
			&mainbase.MPI->MaxCX,
			&mainbase.MPI->MinCY,
			&mainbase.MPI->MaxCY);
		MinIX = mainbase.MPI->StepX * ((STDFLT) mainbase.MPI->MinCX);
		MaxIX = mainbase.MPI->StepX * ((STDFLT) mainbase.MPI->MaxCX);
		MinIY = mainbase.MPI->StepY * ((STDFLT) mainbase.MPI->MinCY);
		MaxIY = mainbase.MPI->StepY * ((STDFLT) mainbase.MPI->MaxCY);
		return(TRUE);
	}
	
	if (strncmp(rbuf,"STEPSIZES/",10)==0) {
		sscanf(&rbuf[10],"%f,%f",
			&mainbase.MPI->StepX,
			&mainbase.MPI->StepY);
		MinIX = mainbase.MPI->StepX * ((STDFLT) mainbase.MPI->MinCX);
		MaxIX = mainbase.MPI->StepX * ((STDFLT) mainbase.MPI->MaxCX);
		MinIY = mainbase.MPI->StepY * ((STDFLT) mainbase.MPI->MinCY);
		MaxIY = mainbase.MPI->StepY * ((STDFLT) mainbase.MPI->MaxCY);
		return(TRUE);
	}
	
	if (strncmp(rbuf,"SHEET/",6)==0) {
		sscanf(&rbuf[6],"%f,%f,%f,%f",
			&MinIX,&MaxIX,&MinIY,&MaxIY);
		mainbase.MPI->MinCX = (MinIX / mainbase.MPI->StepX) + .5;
		mainbase.MPI->MaxCX = (MaxIX / mainbase.MPI->StepX) + .5;
		mainbase.MPI->MinCY = (MinIY / mainbase.MPI->StepY) + .5;
		mainbase.MPI->MaxCY = (MaxIY / mainbase.MPI->StepY) + .5;
		return(TRUE);
	}
		
	if (strncmp(rbuf,"AUTOPENS/",9)==0) {
		sscanf(&rbuf[9],"%d",&rv);
		mainbase.MPI->AutoPens=rv;
		return(TRUE);
	}
	
	if (strncmp(rbuf,"TYPE/",5)==0) {
		sscanf(&rbuf[5],"%d",&rv);
		mainbase.MPI->PlotType = rv;
		return(TRUE);
	}
	
	if (strncmp(rbuf,"FLAGS/",6)==0) {
		sscanf(&rbuf[6],"%x",&rv);
		mainbase.MPI->PlotFlags = rv;
		return(TRUE);
	}

	printf("Unparsed %s\n",rbuf);

	return(TRUE);
}

BOOL anyset=FALSE;

IFFP LoadAView(inname)
UBYTE *inname;
{

	UBYTE fname[255];
	UBYTE *lc,*ls;

	lc = rindex(inname,':');
	ls = rindex(inname,'/');
	
	if (lc==ls) sprintf(fname,"%s",inname);
	else 
		sprintf(fname,"%s",max(lc,ls)+1);

	return(getview(fname,inname));
}

BOOL parsesets(ifile)
UBYTE *ifile;
{

	STDFLT a,b;
	UBYTE *usefil;
	
	if (strncmp(rbuf,"*END PASTEUP",12)==0) return(FALSE);
	
	if (strncmp(rbuf,"*",1)==0) return(TRUE);
	if (strncmp(rbuf,"#",1)==0) return(TRUE);
	
	if (strncmp(rbuf,"VIEWFILE/",9)==0) {
		if (DriverSegHook==NULL) {
			complain(NULL,"Recover",5,0,0,0,
				 "Pasteup file did not load a driver",
				 NULL,NULL,NULL);
			return(FALSE);
		}
		anyset=FALSE;
		usefil = &rbuf[9];
		if (*usefil=='$') usefil=MakeFname(HomePath,usefil+1,NULL);
		if (LoadAView(usefil)!=IFF_OKAY) return(TRUE);
		anyset=TRUE;
		return(TRUE);
	}
		
	if (strncmp(rbuf,"ORIGIN/",7)==0) {
		if (!anyset) return(TRUE);	/* We're skipping this one */
		sscanf(&rbuf[7],"%f,%f",
			&Current_View->XNewOrigin,
			&Current_View->YNewOrigin);
		Current_View->XNewOrigin += SheetMinIX;
		Current_View->YNewOrigin += SheetMinIY;
		return(TRUE);
	}
	
	if (strncmp(rbuf,"ROTATION/",9)==0) {
		if (!anyset) return(TRUE);	/* We're skipping this one */
		sscanf(&rbuf[9],"%f",
			&Current_View->ViewRot);
		return(TRUE);
	}
	
	if (strncmp(rbuf,"SCALE/",6)==0) {
		if (!anyset) return(TRUE);	/* We're skipping this one */
		sscanf(&rbuf[6],"%f,%f",&a,&b);
		Current_View->ViewScale = a/b;
		return(TRUE);
	}
	
	
	if (strncmp(rbuf,"FLAGS/",6)==0) {
		if (!anyset) return(TRUE);	/* We're skipping this one */
		sscanf(&rbuf[6],"%x",&Current_View->ViewFlags);
		return(TRUE);
	}

	printf("Unparsed %s\n",rbuf);

	return(TRUE);
}

VOID loaddefs(ifile)
UBYTE *ifile;
{

	FILE *fiov;
	
	fiov = fopen(ifile,"r");
	if (fiov==NULL) {
		complain(NULL,"Recover",5,0,0,0,
			 "Could not load defs file:",
			 ifile,NULL,NULL);
		return;
	}
	
	FOREVER {
		if (!getline(fiov)) break;
		if (!parsedefs(ifile)) break;
	}
	
	MinIX = ((STDFLT) mainbase.MPI->MinCX) *
	mainbase.MPI->StepX;

	MaxIX = ((STDFLT) mainbase.MPI->MaxCX) *
		mainbase.MPI->StepX;

	MinIY = ((STDFLT) mainbase.MPI->MinCY) *
		mainbase.MPI->StepY;

	MaxIY = ((STDFLT) mainbase.MPI->MaxCY) *
		mainbase.MPI->StepY;

	disp_driv();


	
	fclose(fiov);
}

VOID loadsets(ifile)
UBYTE *ifile;
{

	FILE *fiov;
	
	fiov = fopen(ifile,"r");
	if (fiov==NULL) {
		complain(NULL,"Recover",5,0,0,0,
			 "Could not load pasteup file:",
			 ifile,NULL,NULL);
		return;
	}
	
	Root_View = Last_View = Current_View = NULL;	/* Throw away old */
	
	FOREVER {
		if (!getline(fiov)) break;
		if (!parsedefs(ifile)) break;
	}

	FOREVER {
		if (!getline(fiov)) break;
		if (!parsesets(ifile)) break;
	}
	
	disp_driv();

	fclose(fiov);
}

BOOL Req_Load_Defs();

BOOL Load_Defs()
{

	UBYTE fname[255],fpath[255];
	int a;

	if (!Req_Load_Defs()) return(FALSE);

	if ((a=strlen(DefsPath))==0) sprintf(fname,"%s",ArgText);
	else if (a>0&&DefsPath[a-1]==':')sprintf(fname,"%s%s",DefsPath,ArgText);
	else sprintf(fname,"%s/%s",DefsPath,ArgText);

	loaddefs(fname);
	
	disp_driv();
	refresh_driv();
	
	return(FALSE);
}

BOOL Req_Load_Sets();

BOOL Load_Sets()
{

	UBYTE fname[255],fpath[255];
	int a;

	if (!Req_Load_Sets()) return(FALSE);

	if ((a=strlen(ViewPath))==0) sprintf(fname,"%s",ArgText);
	else if (a>0&&ViewPath[a-1]==':')sprintf(fname,"%s%s",ViewPath,ArgText);
	else sprintf(fname,"%s/%s",ViewPath,ArgText);

	loadsets(fname);
	
	disp_driv();
	refresh_driv();
	
	return(FALSE);
}
