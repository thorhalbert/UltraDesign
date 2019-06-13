/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	clipboard.c - clipboard manager
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
 *	1	11-Jan-87 SH	Beginning of history
 *	2	27-Apr-87 SH	Don't let clipclear repaint unclipped stuff
 *	3	 8-Jun-87 SH	Write boxin and boxall clip
 *	4	18-Jul-87 SH	Fix some dispatch status returns
 *	5	28-Aug-87 SH	Install Message Facility
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

VOID BoxClipper();

/*-------------------------External Symbol Definitions-----------------------*/

extern struct drawing_node *CurrentRoller;
extern WORD clip_sret;
extern WORD EllipseClip;

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

/*
 *     +++
 *     +++
 *  +++++++++	Empty Clipboard
 *  +++++++++
 *     +++
 *     +++
 */


BOOL Clip_Clear()
{

	struct drawing_node *lnklst;

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
		if (!(lnklst->node_flags&NODE_INCLIP)) continue;
		lnklst->node_flags &= ~NODE_INCLIP;
		GenericRender(lnklst);
	}

	return(TRUE);
}

/*
 *     +++
 *     +++
 *  +++++++++	Display all clipped items on screen
 *  +++++++++
 *     +++
 *     +++
 */


BOOL Display_Clip()
{

	struct drawing_node *lnklst;

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
		if (!(lnklst->node_flags&NODE_INCLIP)) continue;
		GenericRender(lnklst);
	}

	return(TRUE);
}

/*
 *     +++
 *     +++
 *  +++++++++	Kill all items in clipboard after asking user
 *  +++++++++
 *     +++
 *     +++
 */


BOOL Clip_Kill()
{

	struct drawing_node *lnklst,*hatchd;
	int killcount=0;	
	UBYTE ClpKilWar[2][60];
	BOOL doit;

	RollUnpaint();

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
		doit = FALSE;
		if (lnklst->node_flags&NODE_INCLIP) doit = TRUE;
		if (ishatched(lnklst)) {
			hatchd = gethatch(lnklst);
			if (hatchd->node_flags&NODE_INCLIP) doit = TRUE;
			else doit=FALSE;
		}
		if (!doit) continue;

		killcount++;
	}

	if (killcount<=0) return;
	sprintf(&ClpKilWar[0][0],"Killing ALL Clipped Objects");
	sprintf(&ClpKilWar[1][0],"Deleting %d Objects",killcount);

	if (complain("ABORT","PROCEED",5,2,2,0,
		"Carnage Warning:",
		&ClpKilWar[0][0],
		&ClpKilWar[1][0],NULL)) return;

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
		doit = FALSE;
		if (lnklst->node_flags&NODE_INCLIP) doit = TRUE;
		if (ishatched(lnklst)) {
			hatchd = gethatch(lnklst);
			if (hatchd->node_flags&NODE_INCLIP) doit = TRUE;
			else doit=FALSE;
		}
			
		if (!doit) continue;
				
		lnklst->node_flags &= (~(NODE_INCLIP|NODE_SELECTED));
		lnklst->node_flags |= NODE_DELETED;
		GenericRender(lnklst);
		DeleteNode(lnklst);
	}

	RollPaint();
	return(TRUE);
}

/*
 *     +++
 *     +++
 *  +++++++++	Box In, Box All, and Box Out interface
 *  +++++++++
 *     +++
 *     +++
 */


LOCAL BOOL buttonyet;
LOCAL	int cliptype;

LOCAL WORD 	BoxIX1,BoxIY1,BoxIX2,BoxIY2;
LOCAL STDFLT 	BoxSX1,BoxSY1,BoxSX2,BoxSY2;

BOOL BoxIn()
{

	if (!EnterSub(BOXIN_MODE,BOX_MESS_EXIT,BoxClipper)) return(FALSE);
	MainMess(BOX_MESS_FP);

	buttonyet=FALSE;
	cliptype = 1;

	return(FALSE);
}

BOOL BoxAll()
{

	if (!EnterSub(BOXALL_MODE,BOX_MESS_EXIT,BoxClipper)) return(FALSE);
	MainMess(BOX_MESS_FP);

	buttonyet=FALSE;
	
	cliptype = 2;

	return(FALSE);
}

BOOL Box_Out()
{

	if (!EnterSub(BOXOUT_MODE,BOX_MESS_EXIT,BoxClipper)) return(FALSE);
	MainMess(BOX_MESS_FP);

	buttonyet=FALSE;
	
	cliptype = 3;

	return(FALSE);
}

static BOOL shadowset=FALSE;

boxclipshad()
{

	if (shadowset) {
		GI_DrawSetup(1,1);

		v_reset();
		v_move(BoxSX1,BoxSY1);
		v_draw(BoxSX2,BoxSY1);
		v_draw(BoxSX2,BoxSY2);
		v_draw(BoxSX1,BoxSY2);
		v_draw(BoxSX1,BoxSY1);

		shadowset=FALSE;
	}
	
}


VOID BoxClipper(sx,sy,action,xr,yr)
WORD sx,sy,action;
STDFLT xr,yr;
{
	boxclipshad();

	if (action==RBUTTONDOWN) {
			ExitSub(BOX_MESS_ABORT);
			return;
	}

	switch (action) {
		case LBUTTONDOWN:
			if (!buttonyet) {
				MainMess(BOX_MESS_SP);

				buttonyet=TRUE;

				BoxIX1 = sx;
				BoxIY1 = sy;
				BoxSX1 = xr;
				BoxSY1 = yr;

				return;
			} else {

				BoxIX2 = sx;
				BoxIY2 = sy;
				BoxSX2 = xr;
				BoxSY2 = yr;

				MainMess(BOX_MESS_PICK);
				ClipAll();

				SExitSub(NULL);

				return;
			}
		case MOVEMENT:
		case UMOVEMENT:
			if (!buttonyet) return;

			BoxIX2 = sx;
			BoxIY2 = sy;
			BoxSX2 = xr;
			BoxSY2 = yr;
			
			shadowset=TRUE;
			boxclipshad();
			shadowset=TRUE;
			return;
	}
}

/*
 *     +++
 *     +++
 *  +++++++++	Actual Bounding Box Clip Routine
 *  +++++++++
 *     +++
 *     +++
 */


ClipAll()
{

	struct drawing_node *lnklst;
	struct line_mode *lneptr;
	struct ellip_mode *drawellipse;
	STDFLT left,right,top,bottom;
	BOOL clipit;

	left = 	min(BoxSX1,BoxSX2);
	right = max(BoxSX1,BoxSX2);
	top = 	max(BoxSY1,BoxSY2);
	bottom =min(BoxSY1,BoxSY2);

	Line_Border(left,right,top,bottom);

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {

		if (lnklst->node_flags&NODE_INCLIP) continue;
		if (!SelectLegal(lnklst,FALSE,TRUE,TRUE)) continue;

		clip_sret = WIND_OFF;	/* Default */
		switch (lnklst->node_id) {

		case NODE_LINE: 
			lneptr = GetLineData(lnklst);
			cdrawline(lneptr->x1,lneptr->y1,lneptr->x2,lneptr->y2,
				FALSE);
			break;
		case NODE_ELLIP:
			drawellipse = GetEllipData(lnklst);
			FancyEllipseClipper(drawellipse->x1,drawellipse->y1,
				drawellipse->xradius,drawellipse->yradius,
				drawellipse->fromang,drawellipse->toangle,
				drawellipse->rotation,FALSE);
			clip_sret = EllipseClip;
			break;
		case NODE_TEXT:
			PutText(lnklst,FALSE);
			break;
		case NODE_HATCH:
			DrawHatch(lnklst,FALSE);
			break;
		}

		clipit=FALSE;

		switch(cliptype) {
		case 1:
			if (clip_sret==WIND_IN) clipit = TRUE;
			break;
		case 2:
			if (clip_sret!=WIND_OFF) clipit = TRUE;
			break;
		case 3:
			if (clip_sret==WIND_OFF) clipit = TRUE;
			break;
		}

		if (!clipit) continue;

		lnklst->node_flags |= NODE_INCLIP;

	}

	Line_Normal();
	Display_Clip();		/* Display the clipboard */
}

/*
 *     +++
 *     +++
 *  +++++++++	Dialog Hook Box Clip Subroutine
 *  +++++++++
 *     +++
 *     +++
 */

VOID DialogBoxClip(x1,y1,x2,y2,type)
{

	cliptype = type;
	
	BoxSX1 = x1;
	BoxSY1 = y1;
	BoxSX2 = x2;
	BoxSY2 = y2;
	
	ClipAll();
	
}
