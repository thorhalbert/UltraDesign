/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	loadtext - load in text structure
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
 *
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

BOOL LoadStrokeFont(infile)
UBYTE *infile;
{

	int i,j;
	char fname[100];
	FILE *loadin;
	T_FONT *newfont;
	struct Stroke_Font *newstroke;
	struct Load_Font_20 *loadstroke;

	strcpy(fname,MakeFname(StrokePath,infile,STROKE_EXT));

	loadin = fopen(fname,"r");

	if (loadin==NULL) {
		complain(NULL,"Recover",4,3,0,0,
			"Cannot open font file - why?",
			fname,NULL,NULL);
		return(FALSE);
	}

	newfont = (T_FONT *) GI_RemAlloc(sizeof (T_FONT));
	if (newfont==NULL) {
		fclose(loadin);
		printf("Cannot malloc font header\n");
		return(FALSE);
	}
	newstroke = (struct Stroke_Font *) 
			GI_RemAlloc(sizeof (struct Stroke_Font));
	if (newstroke==NULL) {
		fclose(loadin);
		printf("Cannot malloc stroke header\n");
		return(FALSE);
	}
	loadstroke = (struct Load_Font_20 *) 
			AllocMem(sizeof (struct Load_Font_20),0);
	if (loadstroke==NULL) {
		fclose(loadin);
		printf("Cannot malloc stroke load header\n");
		return(FALSE);
	}

	j = sizeof (struct Load_Font_20);
	i=fread(loadstroke, j, 1, loadin);
	if (i!=1) {
		fclose(loadin);
		complain(NULL,"Recover",4,3,0,0,
			"Data read problem when loading font",
			fname,NULL,NULL);
		printf("fread return is %d, not 1 (size %d)\n",i,j);
		return(FALSE);
	}

	if (loadstroke->Font_Magic!=CUR_MAGIC_NUM) {
		fclose(loadin);
		complain(NULL,"Recover",4,3,4,0,
			"Font Magic Number Mismatch:",
			fname,
			"Is this really a font?",NULL);
		return(FALSE);
	}

	if (loadstroke->Font_Version!=CUR_STROKE_VER) {
		fclose(loadin);
		complain(NULL,"Recover",4,3,4,0,
			"Font version number incompatibility",
			fname,
			"Find newer fonts or convert",NULL);
		return(FALSE);
	}

	newstroke->Font_Magic = loadstroke->Font_Magic;
	newstroke->Font_Version = loadstroke->Font_Version;
	newstroke->Font_Width = PORTtoF(&loadstroke->Font_Width);
	newstroke->Font_Vertical = PORTtoF(&loadstroke->Font_Vertical);
	newstroke->Font_Horiz = PORTtoF(&loadstroke->Font_Horiz);

	for (i=0; i<256; i++) {
		newstroke->Char_Xtable[i] = loadstroke->Char_Xtable[i];
		newstroke->Char_Widths[i] = PORTtoF(&loadstroke->Char_Widths[i]);
	}

	newstroke->Num_Chars = loadstroke->Num_Chars;

	FreeMem(loadstroke,sizeof (struct Load_Font_20));

	newfont->Font_Size=newstroke->Num_Chars*(sizeof (WORD));

	newstroke->Word_Table = (WORD *)
		GI_RemAlloc(newstroke->Num_Chars * (sizeof (WORD)));

	if (newstroke->Word_Table==NULL) {
		fclose(loadin);
	/*	printf("Cannot allocate the stroke table (size %d)\n",
			newstroke->Num_Chars*(sizeof (WORD)));	*/
		return(FALSE);
	}

	fread(newstroke->Word_Table, 2, newstroke->Num_Chars, loadin);
	fclose(loadin);
	
	sprintf(newfont->Font_Name_ID,"%s",infile);

	newfont->font_link = newstroke;
	newfont->Use_Count = 0;
	newfont->next_font = NULL;
	newfont->prev_font = NULL; 

	if (Font_Last==NULL) {
		Root_Font = newfont;
		Font_Last = newfont;
		newfont->prev_font=NULL;
	} else {
		Font_Last->next_font = newfont;
		newfont->prev_font=Font_Last;
	}

	Font_Last = newfont;

	return(TRUE);

}
