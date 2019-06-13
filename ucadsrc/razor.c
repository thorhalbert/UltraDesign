/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	razor.c - cut objects with etherial razor blade
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
 *	1	17-Jul-87 SH	Begin coding searcher
 *	2	28-Aug-87 SH	Install new message facility
 *	3	30-Aug-87 SH	Make real line distance calculation
 *	4	26-Sep-87 SH	Write razor via searcher
 *	5	15-May-88 SH	Try to clean up problems of razoring
 *				 which occurs off line.  Also make the
 *				 node which is created appear next to
 *				 other node, so lines are still in sequence
 *	6	 5-Jun-88 SH	Razor having difficulty with vert/horiz lines
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include <graphics/gfxmacros.h>

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

LOCAL STDFLT tsp;
#define swp(x,y) tsp = x; x = y; y = tsp
tagint(intx,inty)
STDFLT intx,inty;
{
	STDFLT t;

	t = xpixsize * 2.0;

	GI_DrawSetup(1,0);			
	v_move(intx-t,inty);
	v_draw(intx+t,inty);
	v_move(intx,inty-t);
	v_draw(intx,inty+t);
}

BOOL checkin(vx,vy,ax1,ay1,ax2,ay2,bx1,by1,bx2,by2)
STDFLT vx,vy,ax1,ay1,ax2,ay2,bx1,by1,bx2,by2;
{

	int ck;

	ck = 0;

	if (ax1==ax2) ck |= 0x01;
	else if (vx>min(ax1,ax2)&&vx<max(ax1,ax2)) ck |= 0x01;

	if (ay1==ay2) ck |= 0x02;
	else if (vy>min(ay1,ay2)&&vy<max(ay1,ay2)) ck |= 0x02;

	if (bx1==bx2) ck |= 0x04;
	else if (vx>min(bx1,bx2)&&vx<max(bx1,bx2)) ck |= 0x04;

	if (by1==by2) ck |= 0x08;
	else if (vy>min(by1,by2)&&vy<max(by1,by2)) ck |= 0x08;

	if (ck==0xF) tagint(vx,vy);
	return(ck==0xF);
}

BOOL LineIntercept(curline,ix1,iy1,ix2,iy2,irx,iry)
struct drawing_node *curline;
STDFLT ix1,ix2,iy1,iy2,*irx,*iry;
{

	struct line_mode *drawline;
	STDFLT 	ox1,oy1,ox2,oy2,fx1,fx2,fy1,fy2,fyd,fxd,iyd,ixd,
		oxd,oyd,oix,oiy,m0,b0,m1,b1,intx,inty,tx1,tx2,ty1,ty2;

	tx1 = ix1;
	ty1 = iy1;
	tx2 = ix2;
	ty2 = iy2;

	drawline=GetLineData(curline);

	ox1=fx1=drawline->x1;
	ox2=fx2=drawline->x2;
	oy1=fy1=drawline->y1;
	oy2=fy2=drawline->y2;

	oyd= fyd = (fy1-fy2);
	oxd= fxd = (fx1-fx2);
	oiy = iyd = (iy1-iy2);
	oix = ixd = (ix1-ix2);

	if ((fxd==0.0&&ixd==0.0)||(fyd==0.0&&iyd==0.0)) return(FALSE);

	if (ixd==0.0||iyd==0.0) {
		swp(fy1,iy1);
		swp(fx1,ix1);
		swp(fy2,iy2);
		swp(fx2,ix2);
		swp(fyd,iyd);
		swp(fxd,ixd);
	} 

	if (fxd==0.0) {			/* Vertical */
		intx = fx1;
		if (ixd==0.0) return(FALSE);	/* Lines are parallel */
		if (iyd==0.0) {
			inty = iy1;
			*irx = intx;
			*iry = inty;
		   return(checkin(intx,inty,ox1,oy1,ox2,oy2,tx1,ty1,tx2,ty2));
		}
		m0 = iyd/ixd;		/* Cal slope */
		b0 = iy1 - (m0 * ix1);
		inty = (m0 * intx) + b0;
		*irx = intx;
		*iry = inty;
		return(checkin(intx,inty,ox1,oy1,ox2,oy2,tx1,ty1,tx2,ty2));
	}

	if (fyd==0.0) {
		inty = fy1;
		if (iyd==0.0) return(FALSE);	/* Lines parallel */
		if (ixd==0.0) {
			intx = ix1;
			*irx = intx;
			*iry = inty;
		return(checkin(intx,inty,ox1,oy1,ox2,oy2,tx1,ty1,tx2,ty2));
		}
		if (ixd==0.0) return(FALSE);
		m0 = iyd/ixd;		/* Cal slope */
		b0 = iy1 - (m0 * ix1);
		if (m0==0.0) return(FALSE);	/* How did this happen? */
		intx = (inty - b0) / m0;
		*irx = intx;
		*iry = inty;
		return(checkin(intx,inty,ox1,oy1,ox2,oy2,tx1,ty1,tx2,ty2));
	}

	if (ixd==0.0) return(FALSE);
	m0 = iyd/ixd;		/* Cal slope */
	b0 = iy1 - (m0 * ix1);

	if (fxd==0.0) return(FALSE);
	m1 = fyd/fxd;		/* Cal slope */
	b1 = fy1 - (m1 * fx1);

	if ((m0-m1)==0.0&&(m1-m0)==0.0) return(FALSE);	/* Huh? */

	if ((m0-m1)!=0.0) 	intx = (b1 - b0) / (m0 - m1);
	else			intx = (b0 - b1) / (m1 - m0);

	inty = (m0 * intx) + b0;

	*irx = intx;
	*iry = inty;
	return(checkin(intx,inty,ox1,oy1,ox2,oy2,tx1,ty1,tx2,ty2));

}

LOCAL STDFLT rx1,ry1,rx2,ry2,srx1,sry1,srx2,sry2;
LOCAL Razact = 0;
LOCAL Razshad=FALSE;
VOID PerformRazor();

DoRazor()
{

	struct 	drawing_node *lnklst,*endlist;
	struct	line_mode	*linemode;
	struct	ellip_mode	*ellipmode;
	struct	text_mode	*textmode;
	struct	Group_Attrib	*curgrpsave;
	struct	Layer_Attrib	*curlaysave;
	struct	Pen_Attrib	*curpensave;
	STDFLT 	irx,iry;

	MainMess(RAZOR_MESS_WAIT);

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {

		if (!SelectLegal(lnklst,FALSE,TRUE,TRUE)) continue;	

		switch(lnklst->node_id) {
		case NODE_LINE:
			if (!LineIntercept(lnklst,rx1,ry1,rx2,ry2,&irx,&iry)) 
				continue;

			drawingdirty = TRUE;	/* Probably dirty now */

			linemode = GetLineData(lnklst);

			curgrpsave = Current_Group;
			curlaysave = Current_Layer;
			curpensave = Current_Pen;
	
			Current_Group = lnklst->above_group;
			Current_Layer = lnklst->node_layer;
			Current_Pen = lnklst->pen_attach;

			AttachLine(lnklst,irx,iry,linemode->x2,linemode->y2,
				linemode->line_flags & (~LINE_P1ARROW));
		
			linemode->x2 = irx;
			linemode->y2 = iry;
			linemode->line_flags &= (~LINE_P2ARROW);

			Current_Group = curgrpsave;
			Current_Layer = curlaysave;
			Current_Pen = curpensave;

			lnklst = lnklst->node_subsequent;
					/* Skip over newly created node */
			break;

		case NODE_ELLIP:
			ellipmode = GetEllipData(lnklst);
			break;

		case NODE_TEXT:
			textmode = GetTextData(lnklst);
			break;

		}
	}

	SExitSub(NULL);
}

BOOL Razor_Line()
{

	if (!EnterSub(RAZOR_MODE,RAZOR_MESS_EXIT,PerformRazor)) return(FALSE);

	MainMess(RAZOR_MESS_P1);

	Razact = 0;
	Razshad = FALSE;

	return(FALSE);

}

VOID DoRazShad()
{

	if (Razact<1) return;	
	if (!Razshad) return;

	GI_DrawSetup(1,1);
	v_move(srx1,sry1);
	v_draw(srx2,sry2);

	Razshad = FALSE;
}

VOID PerformRazor(sx,sy,action,xr,yr)
WORD sx,sy,action;
STDFLT xr,yr;
{

	DoRazShad();

	if (action==RBUTTONDOWN) {
		ExitSub(RAZOR_MESS_ABORT);
		return;
	}

	switch (action) {
	case LBUTTONDOWN:
		switch(Razact) {
		case 0:
			rx1 = srx1 = xr;
			ry1 = sry1 = yr;
			Razact = 1;
			MainMess(RAZOR_MESS_P2);
			return;
		case 1:
			rx2 = xr;
			ry2 = yr;
			DoRazor();
			return;
		}
		break;
	case MOVEMENT:
	case UMOVEMENT:
		switch(Razact) {
		case 0:
			return;
		case 1:
			srx2 = xr;
			sry2 = yr;
			Razshad = TRUE;
			DoRazShad();
			Razshad = TRUE;
		}
		break;
	}
}
