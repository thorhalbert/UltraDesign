/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	presets - requester for driver presets
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
 *	1	28-Jun-87 SH	Begin writing
 *	2	22-Aug-87 SH	Insure that libraries get rid of their 
 *				 context on a save, so that the new file
 *				 will appear the next time requester used
 *	3	 1-Jan-88 SH	Create a driver preset requester
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "plotsetup.h"

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

BOOL Scroller();
extern LONG scrollanswer;

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

/*------------------------Set up scroller for styles------------------------*/

LONG Preset_Parent(current_in,inmask,descend)
LONG current_in;
UBYTE *inmask;
BOOL descend;
{

	/* printf("Parent entered\n"); */
	return(0);

}

LONG Preset_Next(current_in,inmask)
LONG current_in;
UBYTE *inmask;
{

	FOREVER {
		/* printf("Next entered with %d (max %d)\n",current_in,
			mainbase.MPI->NumPresets); */
		current_in++;
		if (((int)current_in)>=(mainbase.MPI->NumPresets+1)) return(0);
	
		(*mainbase.PLI->dopreset)(current_in-1);
		/* printf("Tried %d, name %s\n",current_in,mainbase.MPI->PlotName); */
		if (match(inmask,mainbase.MPI->PlotName))
			return(current_in);

	}

}

LONG Preset_Prev(current_in,inmask)
LONG current_in;
UBYTE *inmask;
{

	FOREVER {
		/* printf("Prev entered with %d\n",current_in); */
		current_in--;
		if (((int)current_in)<1) return(0);
	
		(*mainbase.PLI->dopreset)(current_in-1);
		if (match(inmask,mainbase.MPI->PlotName))
			return(current_in);

	}

}

LONG Preset_First(current_in,inmask,descend)
LONG current_in;
UBYTE *inmask;
BOOL descend;
{

	/* printf("First entered\n"); */

	if (descend) return(0);

	current_in = 0;		/* Start looking at 1 */
	return(Preset_Next(current_in,inmask));
}

LONG Preset_Search(insearch)
UBYTE *insearch;
{

	UWORD looper;

	/* printf("Search entered with %s\n",insearch); */

	for (looper=0; looper<mainbase.MPI->NumPresets; looper++) {
		(*mainbase.PLI->dopreset)(looper);
		/* printf("%d %s\n",looper,mainbase.MPI->PlotName); */
		if (match(insearch,mainbase.MPI->PlotName)) return(looper+1);
	}

	return(0);

}

LONG Preset_Descend(current_in,inmask,descend)
LONG current_in;
UBYTE *inmask;
BOOL descend;
{

	/* printf("Descend entered\n"); */
	return(0);
}

VOID Preset_Form(current_in,outform)
LONG current_in;
UBYTE *outform;
{

	if (((int)current_in)<1) return;

	/* printf("Form Entered with %d ",current_in); */

	current_in--;

	(*mainbase.PLI->dopreset)(current_in);
	sprintf(outform,"%-16.16s %-30.30s",
		mainbase.MPI->PlotName,
		mainbase.MPI->Comment);

	/* printf("%s\n",outform); */
}


VOID Preset_Name(current_in,outform)
LONG current_in;
UBYTE *outform;
{

	if (((int)current_in)<1) return;

	/* printf("Name entered with %d ",current_in); */

	current_in--;

	(*mainbase.PLI->dopreset)(current_in);
	sprintf(outform,"%s",
		mainbase.MPI->PlotName);

	/* printf("%s\n",outform); */

}

UBYTE PreMask[40];
extern ULONG CurrentPreset;

LOCAL LONG Holf;

BOOL Preset_Sc()	
{

	BOOL ret;

	/* printf("currentpreset = %d\n",CurrentPreset); */
	Holf = CurrentPreset+1;

	ret=Scroller(Preset_First,
		Preset_Next,
		Preset_Prev,
		Preset_Search,
		Preset_Parent,
		Preset_Descend,
		Preset_Form,
		Preset_Name,
		FALSE,
		TRUE,
		FALSE,
		Holf,
		"   Use    ",
		"  Cancel  ",
		"Select Driver Preset",
		"Preset Name-----Preset Description",
		&Holf,
		PreMask,
		" (Driver Presets)");

	return(ret);

}

BOOL Get_Preset()
{

	if (DriverSegHook==0) return(FALSE);	/* No driver loaded */
	if (mainbase.MPI->NumPresets<=1) return(FALSE);
				/* Driver has no presets */

	if(!Preset_Sc()) {
		(*mainbase.PLI->dopreset)(CurrentPreset);
		return(FALSE);
	}

	CurrentPreset = scrollanswer-1;
	(*mainbase.PLI->dopreset)(CurrentPreset);

	MinIX = ((STDFLT) mainbase.MPI->MinCX) *
		mainbase.MPI->StepX;

	MaxIX = ((STDFLT) mainbase.MPI->MaxCX) *
		mainbase.MPI->StepX;

	MinIY = ((STDFLT) mainbase.MPI->MinCY) *
		mainbase.MPI->StepY;

	MaxIY = ((STDFLT) mainbase.MPI->MaxCY) *
		mainbase.MPI->StepY;

	disp_driv();

	return(TRUE);

}
