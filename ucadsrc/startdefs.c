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
 *	1	18-Sep-88 SH	Invention
 *	2	13-Jun-89 SH	Add the report device field
 *	3	 6-Jun-90 SH	Add the screen aspect field
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include "errors.h"
#include <exec/memory.h>
#include <graphics/gfxmacros.h>

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

extern UBYTE defload[];

BOOL stostr();
BOOL stopath();
BOOL stokey();
BOOL stobool();
BOOL stolong();
BOOL stoflt();

#define PA stopath
#define PS stostr
#define KY stokey
#define NB stobool
#define NL stolong
#define NF stoflt

BOOL defon=TRUE;
int deftime=600;
						
struct startlist {
	BOOL	(*setval)();
	UBYTE	*accesskey;
	VOID	*defdata;
	VOID	*stoloc;

} startups[] = {
	{ PA,"DRAWING_DRAWER",	"$Drawings",	ViewPath },
	{ PA,"VIEW_DRAWER",	"$Drawings",	ViewPath },
	{ PA,"FONT_DRAWER",	"$Stroke_Lib",	StrokePath },
	{ PA,"HATCH_DRAWER",	"$Hatch_Lib",	HatchPath },
	{ PA,"TEMPLATE_DRAWER",	"$Templates",	TemplatePath },
	{ PA,"LIBRARY_DRAWER",	"$Libraries",	LibrPath },
	{ PA,"REXX_DRAWER",	"$Rexx",	RexxPath },
	{ PA,"DIALOG_DRAWER",	"$Dialog",	DialogPath },
	{ PA,"SCRIPT_DRAWER",	"$Dialog",	DialogPath },
	{ PA,"LOAD_DRAWING",	"$Defaults.uvw", defload },
	{ PA,"LOAD_VIEW",	"$Defaults.uvw", defload },
	{ PA,"LOAD_DIALOG",	"$Ultradesign.uucm", defstart },
	{ NF,"ASPECT",		&defon,		&aspect },
	{ PA,"REPORT_DEV",	"PRT:",		ReportDev },
	{ NB,"SCREEN_SAVER",	&defon,		&Screen_Save_On },
	{ NL,"SAVER_SECONDS",	&deftime,	&Screen_Save_Time },
	{ PS,"DIALOG_DEF",	NULL,		&dialog_def },
	{ PS,"DIALOG_WB1",	NULL,		&dialog_wb1 },
	{ PS,"DIALOG_WB2",	NULL,		&dialog_wb2 },
	{ PS,"DIALOG_MN1",	NULL,		&dialog_mn1 },
	{ PS,"DIALOG_MN2",	NULL,		&dialog_mn2 },
	{ KY,"VP_KEY" },	/* Anacronism */
	{ KY,"UD_KEY" },
	{ 0,0,NULL},
};

BOOL parselist()
{

	FILE *infile;
	UBYTE parsl[257],parslf[30],parpar[30],parbod[256],*a,*b,*c,*d;
	int i;
	
	strcpy(parbod,MakeFname(HomePath,"UltraDesign",".config"));
	infile = fopen(parbod,"r");
	if (infile==NULL) {
		complain(NULL,"Recover",4,3,0,0,
			"Cannot Find Configuration File:",
			parbod,
			NULL,NULL);
		abort(DIALOG_NOFILE);
	}
	
	FOREVER {
		if (fgets(parsl,256,infile)==NULL) break;
		
		i=strlen(parsl);
		if (parsl[i-1]=='\n') parsl[i-1]='\0';
		
		if (parsl[0]=='\0') continue;		/* Blank line */
		if (parsl[0]=='#') continue;		/* Comment */
		
		a = index(parsl,'@');			/* Find delimeter */
		if (a==NULL) a = index(parsl,'/');
		
		for (b = parslf,c = parsl; c<a; c++,b++) *b = *c;
		*b++ = '\0';
		
		parpar[0] = '\0';
		if (*a=='@') {
			b = index(parsl,'/');
			for (c=a+1,d=parpar;c<b;c++,d++) *d = *c;
			*d++ = '\0';
			a = b;
		}
		
		parbod[0] = '\0';
		
		for (b=a+1,c=parbod;*b != '\0'; c++,b++) *c = *b;
		*c++ = '\0';
		
	/*	printf("Key=%s Arg=%s Body=%s\n",parslf,parpar,parbod); */

		for (i=0; startups[i].setval!=0; i++) {
			if (strcmp(parslf,startups[i].accesskey)!=0) continue;
			*(startups[i].setval)(&startups[i],parpar,parbod);
			break;
		}
						
	}
	
	fclose(infile);
	
}

BOOL stokey(instart,arg,body) 
struct startlist *instart;
UBYTE *arg,*body;
{
	/* StoKeyVal(arg,body); use ultradesign.menus now */
}

BOOL stopath(instart,arg,body)
struct startlist *instart;
UBYTE *arg,*body;
{
	if (body[0]=='$')
		strcpy(instart->stoloc,MakeFname(HomePath,&body[1],NULL));
	else	strcpy(instart->stoloc,body);
	
}

BOOL stostr(instart,arg,body)
struct startlist *instart;
UBYTE *arg,*body;
{

	strcpy(instart->stoloc,body);
	
}

BOOL stobool(instart,arg,body)
struct startlist *instart;
UBYTE *arg,*body;
{
	BOOL *pobool;
	int getans;
	int scanans;
	
	pobool = (BOOL *) instart->stoloc;
	
	scanans = sscanf(body,"%d",&getans);
	if (scanans!=1) return;
	
	*pobool = getans;
	
	printf("%s/%d\n",instart->accesskey,getans);
			
}

BOOL stolong(instart,arg,body)
struct startlist *instart;
UBYTE *arg,*body;
{
	LONG *polong,getans;
	int scanans;
	
	polong = (LONG *) instart->stoloc;
	
	scanans = sscanf(body,"%ld",&getans);
	if (scanans!=1) return;
	
	*polong = getans;

	printf("%s/%ld\n",instart->accesskey,*polong);
			
}

BOOL stoflt(instart,arg,body)
struct startlist *instart;
UBYTE *arg,*body;
{
	STDFLT *poflt,getans;
	int scanans;
	
	poflt = (STDFLT *) instart->stoloc;
	
	scanans = sscanf(body,"%f",&getans);
	if (scanans!=1) return;
	
	*poflt = getans;

	printf("%s/%g\n",instart->accesskey,*poflt);
			
}
