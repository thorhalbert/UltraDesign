/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	layerhandle.c - basic layer support
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
 *	1	10-Nov-88 SH	Split off stuff from layerreq
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/


LAYER_A *Layer_Parent(current_in,inmask,descend)
LAYER_A *current_in;
UBYTE *inmask;
BOOL descend;
{

	return(NULL);

}

LAYER_A *Layer_Next(current_in,inmask)
LAYER_A *current_in;
UBYTE *inmask;
{

	FOREVER {
		current_in=current_in->layer_next;
		if (current_in==NULL) return(NULL);
	
		if (match(inmask,current_in->Layer_Name))
			return(current_in);

	}
}

LAYER_A *Layer_Prev(current_in,inmask)
LAYER_A *current_in;
UBYTE *inmask;
{

	FOREVER {
		current_in=current_in->layer_prev;
		if (current_in==NULL) return(NULL);
	
		if (match(inmask,current_in->Layer_Name))
			return(current_in);

	}
}

LAYER_A *Layer_First(current_in,inmask,descend)
LAYER_A *current_in;
UBYTE *inmask;
BOOL descend;
{
	current_in = Root_Layer;

	if (match(inmask,current_in->Layer_Name))
		return(current_in);

	return(Layer_Next(Root_Layer,inmask));

}

LAYER_A *Layer_Search(insearch)
UBYTE *insearch;
{

	LAYER_A *looper;

	for (looper=Root_Layer; looper!=NULL; looper=looper->layer_next) 
		if (match(insearch,looper->Layer_Name)) return(looper);

	return(NULL);

}

LAYER_A *Layer_Descend(current_in,inmask,descend)
LAYER_A *current_in;
UBYTE *inmask;
BOOL descend;
{

	return(NULL);

}

VOID Layer_Form(current_in,outform)
LAYER_A *current_in;
UBYTE *outform;
{
	char odis[5];
	sprintf(odis,"    ");
	if (current_in->Layer_Flags&LAYER_GHOSTED) odis[0]='G';
	if (current_in->Layer_Flags&LAYER_DOFF) odis[1]='D';
	if (current_in->Layer_Flags&LAYER_LOCKED) odis[3]='L';
	if (current_in->Layer_Flags&LAYER_POFF) odis[2]='P';
	sprintf(outform,"%-32.32s %4d %s",
		current_in->Layer_Name,current_in->Use_Count,odis);

	Scr_Color = current_in->Color_ID;
}


VOID Layer_Name(current_in,outform)
LAYER_A *current_in;
UBYTE *outform;
{

	sprintf(outform,"%s",current_in->Layer_Name);

}

Unlink_Layer(inlayer)
LAYER_A *inlayer;
{

	if (inlayer->layer_next!=NULL) {
		if (inlayer->layer_prev!=NULL) {
			inlayer->layer_prev->layer_next=
				inlayer->layer_next;
			inlayer->layer_next->layer_prev=
				inlayer->layer_prev;
		} else {
			Root_Layer=inlayer->layer_next;
			inlayer->layer_next->layer_prev=NULL;
		}
	} else {
		if (inlayer->layer_prev!=NULL) {
			inlayer->layer_prev->layer_next=NULL;
			Last_Layer=inlayer->layer_prev;
		} else {
			Root_Layer = Last_Layer = NULL;
		}
	}
}

