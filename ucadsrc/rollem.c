/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	rollem.c - roller/selector control
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
 *	1	11-Jan-87 SH	Begin History
 *	2	28-Apr-87 SH	Provision to make the selected element
 *				 purple when clipped also
 *	3	16-Aug-87 SH	Actually delete deleted items
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

BOOL rolldir;

/*-------------------------------Code Section--------------------------------*/

VOID RollUnpaint()
{

	if (CurrentRoller==NULL) return;

	if (CurrentRoller->node_flags & NODE_SELECTED) {
		CurrentRoller->node_flags &= (~NODE_SELECTED);
		GenericRender(CurrentRoller);
	}
}

VOID RollPaint()
{

	if (CurrentRoller==NULL) return;

	if (!(CurrentRoller->node_flags & NODE_SELECTED)) {
		CurrentRoller->node_flags |= NODE_SELECTED;
		GenericRender(CurrentRoller);
	}
}

/*
 *	Select roll down stack
 *
 *	Don't forget to implement recursive stack descent of tree groups
 *	Now acts like there is just one group 
 *
 */

VOID Rollem()
{

	struct drawing_node *oldroller;

	if (CurrentRoller==NULL) CurrentRoller=Root_Node;
	
	if (CurrentRoller==NULL) return;

	oldroller=CurrentRoller;	/* Don't clear until we
					   are sure we moved */

	FOREVER {

		if (CurrentRoller==NULL) return;
	
		CurrentRoller->node_flags &= (~NODE_SELECTED);

		if (rolldir) {
			CurrentRoller=CurrentRoller->node_subsequent;
			if (CurrentRoller==NULL) CurrentRoller=Root_Node;
		} else {
			CurrentRoller=CurrentRoller->node_previous;
			if (CurrentRoller==NULL) CurrentRoller=Last_Node;
		}

		if (CurrentRoller==oldroller) return;

		if (!SelectLegal(CurrentRoller,FALSE,TRUE,TRUE)) continue;

		return;
	}
}

VOID Roll_Left()
{

	RollUnpaint();	
	rolldir = FALSE;
	Rollem();
	RollPaint();
}
VOID Roll_Right()
{
	RollUnpaint();
	rolldir = TRUE;
	Rollem();
	RollPaint();
}
	
VOID DeleteNode(object)
struct drawing_node *object;
{

/*------------------Snip object out of linked list------------------------*/

	if (object==CurrentRoller) {	/* Roll us back */
		CurrentRoller = object->node_previous;
		if (CurrentRoller==NULL) CurrentRoller = Last_Node;
	}

	if (object==CurrentRoller) Rollem();
	if (object==CurrentRoller) CurrentRoller = NULL;

	drawingdirty = TRUE;		/* Drawing now dirty */
	
	SnipObjectMain(object,TRUE);
	
	FreeMem(object, object->node_length);		/* Free item */

}


VOID DelCurrent()
{

	struct drawing_node *object;

	if (CurrentRoller==NULL) return;		/* Don't assume */
	if (CurrentRoller->node_id==NODE_LINE) 
		if (ishatched(CurrentRoller)) return;
	
	CurrentRoller->node_flags &= (~NODE_SELECTED);
	CurrentRoller->node_flags |= NODE_DELETED;
	GenericRender(CurrentRoller);		/* Erase from screen */

	object = CurrentRoller;

	DeleteNode(object);

	RollPaint();
}

VOID ClipCurrent()
{

	if (CurrentRoller==NULL) return;

	CurrentRoller->node_flags |= NODE_INCLIP;
	RollUnpaint();
		
	Rollem();
	RollPaint();
}

VOID UnClipCurrent()
{

	if (CurrentRoller==NULL) return;

	CurrentRoller->node_flags &= (~NODE_INCLIP);
	
	RollUnpaint();	
	Rollem();
	RollPaint();
}
