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
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "plotsetup.h"
#include <exec/memory.h>
#include <graphics/gfxmacros.h>

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/


BOOL stopath();
BOOL stokey();
BOOL stobool();
BOOL stolong();
BOOL stostr();
BOOL stofp();

#define PA stopath
#define KY stokey
#define NB stobool
#define NL stolong
#define PS stostr
#define FP stofp

BOOL defon=TRUE,defoff=FALSE;
STDFLT defone=1.0;

struct startlist {
	BOOL	(*setval)();
	VOID	*accesskey;
	VOID	*defdata;
	VOID	*stoloc;	
} startups[] = {
	{ PA,"DRAWING_DRAWER",	"$Drawings",	ViewPath },
	{ PA,"VIEW_DRAWER",	"$Drawings",	ViewPath },
	{ PA,"FONT_DRAWER",	"$Stroke_Lib",	StrokePath },
	{ PA,"HATCH_DRAWER",	"$Hatch_Lib",	HatchPath },
	{ PA,"TEMPLATE_DRAWER",	"$Templates",	TemplatePath },
	{ PA,"DRIVER_DRAWER",	"$Drivers",	DrivPath },
	{ PA,"DEFS_DRAWER",	"$Definitions",	DefsPath },
	{ PA,"PASTEUP_DEF",	"$Definitions/Prefdefault.upd", DefName },
	{ NB,"PAUSE_PAGE",	&defoff,	&Pause_Page },
	{ NB,"PAUSE_PLOT",	&defon,		&Pause_Plot },	
	{ PS,"FILL_HATCH_FILE",	"Hatch_File_One", Fill_Hatch_File },
	{ PS,"FILL_HATCH_NAME", "Grid",		Fill_Hatch_Name },
	{ FP,"FILL_HATCH_SCALE", &defone,	&Fill_Hatch_Scale },
	{ FP,"PLOT_DOT_SIZE",   &defone,	&dot_size},
	{ 0,0,NULL},
};

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
			"We must live with canned defaults!",NULL);
		return(FALSE);
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
		
		for (i=0; startups[i].setval!=0; i++) {
			if (strcmp(parslf,startups[i].accesskey)!=0) continue;
			*(startups[i].setval)(&startups[i],parpar,parbod);
			break;
		}
	}
	
	fclose(infile);
	
}

BOOL stopath(instart,arg,body)
struct startlist *instart;
UBYTE *arg,*body;
{
	if (body[0]=='$')
		strcpy(instart->stoloc,MakeFname(HomePath,&body[1],NULL));
	else	strcpy(instart->stoloc,body);
	
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
	
	/* printf("%s/%d\n",instart->accesskey,getans); */
			
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

	/* printf("%s/%ld\n",instart->accesskey,*polong); */
			
}

BOOL stofp(instart,arg,body)
struct startlist *instart;
UBYTE *arg,*body;
{
	STDFLT *pofp,getans;
	int scanans;
	
	pofp = (STDFLT *) instart->stoloc;
	
	scanans = sscanf(body,"%f",&getans);
	if (scanans!=1) return;
	
	*pofp = getans;

/*	printf("%s/%f\n",instart->accesskey,*pofp); */
			
}

BOOL stostr(instart,arg,body)
struct startlist *instart;
UBYTE *arg,*body;
{

	strcpy(instart->stoloc,body);
	
}
