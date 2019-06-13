/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	origin.c - Set permanent and temporary origins
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
 *	1	17-Jul-87 SH	Begin coding origin
 *	2	28-Aug-87 SH	Install new message facility
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

BOOL	ResetOrigin()
{

	x_origin = y_origin = 0.0;

	return(FALSE);
}

VOID MoveCenter(innode,ix,iy)
struct drawing_node *innode;
STDFLT ix,iy;
{
	struct line_mode *lineitem;
	struct ellip_mode *ellipitem;
	struct text_mode *textitem;

	if (Library_Mode&&(!innode->above_group->cursel)) return;

	switch (innode->node_id) {

	case NODE_LINE: 
		lineitem = GetLineData(innode);
		lineitem->x1 -=	ix;
		lineitem->y1 -=	iy;
		lineitem->x2 -=	ix;
		lineitem->y2 -=	iy;
		break;
 	case NODE_ELLIP: 
		ellipitem = GetEllipData(innode);
		ellipitem->x1 -= ix;
		ellipitem->y1 -= iy;
		break;
	case NODE_TEXT: 
		textitem = GetTextData(innode);
		textitem->x1 -= ix;
		textitem->y1 -= iy;
		break;
	}

}

MoveOver(ix,iy)
STDFLT ix,iy;
{

	struct drawing_node *lnklst;

	InfoMess(PERMORG_MODE,PERMORG_MESS_WAIT);

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) 
		MoveCenter(lnklst,ix,iy);

	x_origin = y_origin = 0.0;	/* Extra feature */

	if (Library_Mode)
		RepaintWindow();
	else {
		xviewbegin -= ix;
		yviewbegin -= iy;
		xviewMIN -= ix;
		yviewMIN -= iy;
		xviewMAX -= ix;
		yviewMAX -= iy;

	}

}

VOID PermOrgAct(sx,sy,action,xr,yr)
WORD sx,sy,action;
STDFLT xr,yr;
{

	if (action==RBUTTONDOWN) {
		ExitSub(PERMORG_MESS_ABORT);
		return;
	}

	switch (action) {
	case LBUTTONDOWN:
		MoveOver(xr,yr);
		SExitSub(NULL);
		return;

	}
}

PermOrigin()
{

	if (!EnterSub(PERMORG_MODE,PERMORG_MESS_EXIT,PermOrgAct)) return(FALSE);
	MainMess(PERMORG_MESS_FIND);

	return(FALSE);
}

VOID TempOrgAct(sx,sy,action,xr,yr)
WORD sx,sy,action;
STDFLT xr,yr;
{

	if (action==RBUTTONDOWN) {
		ExitSub(TEMPORG_MESS_ABORT);
		return;
	}

	switch (action) {
	case LBUTTONDOWN:
		x_origin = xr;
		y_origin = yr;
		SExitSub(NULL);
		return;

	}
}

TempOrigin()
{

	if (!EnterSub(TEMPORG_MODE,TEMPORG_MESS_EXIT,TempOrgAct)) return(FALSE);
	MainMess(TEMPORG_MESS_FIND);

	return(FALSE);
}
