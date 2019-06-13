/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	penhandle.c - handle pen structure mechanics
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
 *	1	10-Nov-88 SH	Pull in stuff from penreq.c
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

PEN_A *Pen_Parent(current_in,inmask,descend)
PEN_A *current_in;
UBYTE *inmask;
BOOL descend;
{

	return(NULL);

}

PEN_A *Pen_Next(current_in,inmask)
PEN_A *current_in;
UBYTE *inmask;
{

	FOREVER {
		current_in=current_in->pen_next;
		if (current_in==NULL) return(NULL);
	
		if (match(inmask,current_in->Pen_Name))
			return(current_in);

	}
}

PEN_A *Pen_Prev(current_in,inmask)
PEN_A *current_in;
UBYTE *inmask;
{

	FOREVER {
		current_in=current_in->pen_prev;
		if (current_in==NULL) return(NULL);
	
		if (match(inmask,current_in->Pen_Name))
			return(current_in);

	}
}

PEN_A *Pen_First(current_in,inmask,descend)
PEN_A *current_in;
UBYTE *inmask;
BOOL descend;
{
	current_in = Root_Pen;

	if (match(inmask,current_in->Pen_Name))
		return(current_in);

	return(Pen_Next(Root_Pen,inmask));

}

PEN_A *Pen_Search(insearch)
UBYTE *insearch;
{

	PEN_A *looper;

	for (looper=Root_Pen; looper!=NULL; looper=looper->pen_next) 
		if (match(insearch,looper->Pen_Name)) return(looper);

	return(NULL);

}

PEN_A *Pen_Descend(current_in,inmask,descend)
PEN_A *current_in;
UBYTE *inmask;
BOOL descend;
{

	return(NULL);

}

VOID Pen_Form(current_in,outform)
PEN_A *current_in;
UBYTE *outform;
{
	char odis[5];
	sprintf(odis,"    ");
	if (current_in->Pen_Flags&PEN_GHOSTED) odis[0]='G';
	if (current_in->Pen_Flags&PEN_DOFF) odis[1]='D';
	if (current_in->Pen_Flags&PEN_LOCKED) odis[3]='L';
	if (current_in->Pen_Flags&PEN_POFF) odis[2]='P';
	sprintf(outform,"%-32.32s %4d %s  [%d/%d/%d]",
		current_in->Pen_Name,current_in->Use_Count,odis,
		current_in->PenOrd,current_in->Carousel,
		current_in->AutoNum);
	Scr_Color = current_in->Color_ID;
}


VOID Pen_Name(current_in,outform)
PEN_A *current_in;
UBYTE *outform;
{

	sprintf(outform,"%s",current_in->Pen_Name);

}

Unlink_Pen(inpen)
PEN_A *inpen;
{

	if (inpen->pen_next!=NULL) {
		if (inpen->pen_prev!=NULL) {
			inpen->pen_prev->pen_next=
				inpen->pen_next;
			inpen->pen_next->pen_prev=
				inpen->pen_prev;
		} else {
			Root_Pen=inpen->pen_next;
			inpen->pen_next->pen_prev=NULL;
		}
	} else {
		if (inpen->pen_prev!=NULL) {
			inpen->pen_prev->pen_next=NULL;
			Last_Pen=inpen->pen_prev;
		} else {
			Root_Pen = Last_Pen = NULL;
		}
	}
}

