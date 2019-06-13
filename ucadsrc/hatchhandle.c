/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	stylehandle - basic structure maint/functions
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
 *	1	 9-Feb-89 SH	Make hatchhandle out of stylehandle
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

/*------------------------Set up scroller for fonts------------------------*/

T_HATCH *Hatch_Parent(current_in,inmask,descend)
T_HATCH *current_in;
UBYTE *inmask;
BOOL descend;
{

	return(NULL);

}

T_HATCH *Hatch_Next(current_in,inmask)
T_HATCH *current_in;
UBYTE *inmask;
{

	UBYTE hatchcmp[90];

	FOREVER {
		current_in=current_in->next;
		if (current_in==NULL) return(NULL);
	
		sprintf(hatchcmp,"%s&%s",
			current_in->File_Name,
			current_in->Hatch_Name);
			
		if (match(inmask,hatchcmp))
			return(current_in);

	}
}

T_HATCH *Hatch_Prev(current_in,inmask)
T_HATCH *current_in;
UBYTE *inmask;
{

	UBYTE hatchcmp[90];

	FOREVER {
		current_in=current_in->prev;
		if (current_in==NULL) return(NULL);

		sprintf(hatchcmp,"%s&%s",
			current_in->File_Name,
			current_in->Hatch_Name);
			
		if (match(inmask,hatchcmp))
			return(current_in);

	}
}

T_HATCH *Hatch_First(current_in,inmask,descend)
T_HATCH *current_in;
UBYTE *inmask;
BOOL descend;
{
	UBYTE hatchcmp[90];

	current_in = Root_Hatch;

	sprintf(hatchcmp,"%s&%s",
		current_in->File_Name,
		current_in->Hatch_Name);
		
	if (match(inmask,hatchcmp))
		return(current_in);

	return(Hatch_Next(current_in,inmask));

}

T_HATCH *Hatch_Search(insearch)
UBYTE *insearch;
{

	T_HATCH *looper;
	UBYTE hatchcmp[90];

	for (looper=Root_Hatch; looper!=NULL; looper=looper->next) {
		sprintf(hatchcmp,"%s&%s",
			looper->File_Name,
			looper->Hatch_Name);
		
		if (match(insearch,hatchcmp)) return(looper);
	}

	return(NULL);

}

T_HATCH *Hatch_Search_File(insearch)
UBYTE *insearch;
{

	T_HATCH *looper;

	for (looper=Root_Hatch; looper!=NULL; looper=looper->next) {
		
		if (match(insearch,looper->File_Name)) return(looper);
	}

	return(NULL);

}

T_HATCH *Hatch_Descend(current_in,inmask,descend)
T_HATCH *current_in;
UBYTE *inmask;
BOOL descend;
{

	return(NULL);

}

VOID Hatch_Form(current_in,outform)
T_HATCH *current_in;
UBYTE *outform;
{

	sprintf(outform,"%-25.25s %s",
		current_in->File_Name,current_in->Hatch_Name);

}


VOID Hatch_Name(current_in,outform)
T_HATCH *current_in;
UBYTE *outform;
{

	sprintf(outform,"%s&%s",
		current_in->File_Name,
		current_in->Hatch_Name);

}



int cmphatname(in1,in2)
T_HATCH *in1,*in2;
{
	UBYTE name1[100];
	UBYTE name2[100];
	
	sprintf(name1,"%s&%s",in1->File_Name,in1->Hatch_Name);
	sprintf(name2,"%s&%s",in2->File_Name,in2->Hatch_Name);

	return(asccmp(name1,name2));

}

VOID Link_Hatch_In(instr)
T_HATCH *instr;
{

	T_HATCH *loop;

	instr->next = NULL;
	instr->prev = NULL;

	if (Root_Hatch==NULL) {
		Root_Hatch = Last_Hatch = instr;
		return;
	}

	for (loop=Root_Hatch; loop!=NULL; loop=loop->next)
		if (cmphatname(instr,loop)<0) break;

	if (loop==NULL) {
		Last_Hatch->next = instr;
		instr->prev = Last_Hatch;
		Last_Hatch = instr;
		return;
	}
	
	if (loop->prev==NULL) {
		Root_Hatch = instr;
		loop->prev = instr;
		instr->next = loop;
		return;
	}

	loop->prev->next = instr;
	instr->next = loop;
	instr->prev = loop->prev;
	loop->prev = instr;

	return;
}
