/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	loadhatch - load in a hatching pattern file
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

BOOL LoadHatch(infile)
UBYTE *infile;
{

	int i,j;
	char fname[100],text[100];
	FILE *loadin;
	
	struct Hatch_Header hatchheader;
	struct Hatch_Entry *newentry;

	/* Open the hatch file */

	strcpy(fname,MakeFname(HatchPath,infile,HATCH_EXT));

	loadin = fopen(fname,"r");

	if (loadin==NULL) {
		complain(NULL,"Recover",4,3,0,0,
			"Cannot open hatch file - why?",
			fname,NULL,NULL);
		return(FALSE);
	}

	/* Read in the header and authenticate */

	j = sizeof (struct Hatch_Header);
	i=fread((void *) &hatchheader, j, 1, loadin);
	if (i!=1) {
		fclose(loadin);
		sprintf(text,"header return is %d, not 1 (size %d)",i,j);
		complain(NULL,"Recover",4,3,0,0,
			"Data read problem when loading hatch",
			fname,text,NULL);
		return(FALSE);
	}

	if (hatchheader.Hatch_Magic!=HAT_MAGIC_NUM) {
		fclose(loadin);
		i = hatchheader.Hatch_Magic;
		sprintf(text,"Read magic = %d, should be %d",i,HAT_MAGIC_NUM);
		complain(NULL,"Recover",4,3,4,0,
			"Hatch Magic Number Mismatch:",
			fname,
			"Is this really a hatch file?",text);
		return(FALSE);
	}

	if (hatchheader.Hatch_Version!=CUR_HATCH_VER) {
		fclose(loadin);
		i = hatchheader.Hatch_Version;
		sprintf(text,"Read version = %d, current is %d",i,CUR_HATCH_VER);
		complain(NULL,"Recover",4,3,4,0,
			"Hatch version number incompatibility",
			fname,
			"Find newer patterns or convert",text);
		return(FALSE);
	}
	
	/* Do the hatch entry loop */

	FOREVER {

		newentry = (struct Hatch_Entry *) 	/* Alloc new piece */
			GI_RemAlloc(sizeof (struct Hatch_Entry));
			
		/* Read in entry and authenticate */
		
		j = sizeof (struct Hatch_Entry);
		i=fread((void *) newentry, j, 1, loadin);
		
		if (i!=1) {
			fclose(loadin);
			sprintf(text,"entry return is %d, not 1 (size %d)",i,j);
			complain(NULL,"Recover",4,3,0,0,
				"Data read problem when loading hatch",
				fname,text,NULL);
			return(FALSE);
		}

		if ((int) newentry->next==0) break;	/* End of file */
		
		/* for (i=0; i<4; i++)
			printf("%d - %f %f\n",i,
				PORTtoF(&(newentry->PGramX[i])),
				PORTtoF(&(newentry->PGramY[i]))); */
		
		newentry->next = NULL;
		newentry->prev = NULL;
		newentry->Use_Count = 0;
		newentry->Hatch_ID = 0;
		
		strcpy(newentry->File_Name,infile);	/* Redo file name */

		/* Prep pattern memory and read in */
		
		newentry->Pattern = (ULONG *) 
			GI_RemAlloc(newentry->Pattern_Size);
		if (newentry->Pattern==NULL) return(FALSE);	
							
		j = newentry->Pattern_Size;
		i=fread((void *) newentry->Pattern, j, 1, loadin);
		
		if (i!=1) {
			fclose(loadin);
			sprintf(text,"data return is %d, not 1 (size %d)",i,j);
			complain(NULL,"Recover",4,3,0,0,
				"Data read problem when loading hatch",
				fname,text,NULL);
			return(FALSE);
		}
		
		/* printf("Pattern for %s ",newentry->Hatch_Name);
		for (i=0; i<newentry->Pattern_Size/4; i++) {
			printf("%08x ",newentry->Pattern[i]);
		}
		printf("\n"); */

		Link_Hatch_In(newentry);
	}

	fclose(loadin);		/* Done with the file now */

	return(TRUE);

}
