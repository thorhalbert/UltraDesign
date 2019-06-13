/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	clonedrag.c - implement clone and drag functions
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
 *	2	 6-May-87 SH	Add Andy's multiclone operation
 *	3	18-Jul-87 SH	Clean up some dispatch status returns
 *	4	 1-Aug-87 SH	Update dirty flags
 *	5	 3-Sep-87 SH	Add flip modes and make a universal
 *				sub for drag, rotate, scale, and flips
 *	6	15-Jan-89 SH	Add make string routines
 *	7	19-Apr-90 SH	String routine leaves bogus current selected
 *				 items in list
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include <graphics/gfxmacros.h>

/*----------------------------------Macros-----------------------------------*/

#define TOP_USERWINDOW (YSsize-BOTTOMAREA)

/*-----------------------------Forward References----------------------------*/

VOID PerformCopy();
VOID clone_clip();
VOID drag_clip();
VOID ClonePieces();
VOID PerformScale();

D_NODE *gethatch();

/*-------------------------External Symbol Definitions-----------------------*/

extern struct RastPort *rp;

/*-------------------------Global Variable Definitions-----------------------*/

BOOL orgset,cloneflag;

STDFLT OrgX,ToX,OrgY,ToY;

/*-------------------------------Code Section--------------------------------*/

/*
 *     +++
 *     +++
 *  +++++++++	Generic Flip, Rotate, Scale and Drag Routine
 *  +++++++++
 *     +++
 *     +++
 */

VOID FlipAbout(xh,yh,xc,yc,rotang,doscale,pxflip,pyflip,waitmess)
struct	mesdef *waitmess;
STDFLT 	xc,yc,xh,yh,rotang,doscale;
BOOL 	pxflip,pyflip;
{
	BYTEBITS	holdflag;
	struct 	drawing_node *lnklst,*dohatch,*hatchd;
	struct	line_mode	*linemode;
	struct	ellip_mode	*ellipmode;
	struct	text_mode	*textmode;
	STDFLT	x,y,tsin,tcos;
	BOOL 	doit;

	SInfoMess(Main_Mode,waitmess);

	tsin = sin(rotang);
	tcos = cos(rotang);
	
	dohatch = NULL;

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {

		doit = FALSE;
		holdflag=lnklst->node_flags;		
		if (holdflag&NODE_INCLIP) doit = TRUE;
		
		if (ishatched(lnklst)) {
			hatchd = gethatch(lnklst);
			if (hatchd->node_flags&NODE_INCLIP) doit = TRUE;
			else doit = FALSE;
		}
		
		if (!doit) continue;

		drawingdirty = TRUE;	/* Probably dirty now */

		lnklst->node_flags=NODE_DELETED;
		GenericRender(lnklst);

		lnklst->node_flags=holdflag;

		switch(lnklst->node_id) {
		case NODE_LINE:
			linemode = GetLineData(lnklst);

			linemode->x1 -= xh;
			linemode->y1 -= yh;
			linemode->x2 -= xh;
			linemode->y2 -= yh;

			if (pyflip) {
				linemode->y1 = - linemode->y1;
				linemode->y2 = - linemode->y2;
			}
			if (pxflip) {
				linemode->x1 = - linemode->x1;
				linemode->x2 = - linemode->x2;
			}
			linemode->x1 *= doscale;
			linemode->y1 *= doscale;
			linemode->x2 *= doscale;
			linemode->y2 *= doscale;
			
			x = linemode->x1;
			y = linemode->y1;
			linemode->x1 = x*tcos-y*tsin;
			linemode->y1 = x*tsin+y*tcos;

			x = linemode->x2;
			y = linemode->y2;
			linemode->x2 = x*tcos-y*tsin;
			linemode->y2 = x*tsin+y*tcos;

			linemode->x1 += xc;
			linemode->y1 += yc;
			linemode->x2 += xc;
			linemode->y2 += yc;
			break;

		case NODE_ELLIP:
			ellipmode = GetEllipData(lnklst);
			ellipmode->x1 -= xh;
			ellipmode->y1 -= yh;

			ellipmode->xradius *= doscale;
			ellipmode->yradius *= doscale;
			ellipmode->x1 *= doscale;
			ellipmode->y1 *= doscale;
			if (pxflip) {
				ellipmode->x1 = - ellipmode->x1;

				ellipmode->rotation = PI - ellipmode->rotation;
				x = ellipmode->fromang;
				y = ellipmode->toangle;
				ellipmode->fromang = PI - y;
				ellipmode->toangle = PI - x;
			}
			if (pyflip) {
				ellipmode->y1 = - ellipmode->y1;

				ellipmode->rotation = TWOPI - ellipmode->rotation;
				x = ellipmode->fromang;
				y = ellipmode->toangle;
				ellipmode->fromang = TWOPI - y;
				ellipmode->toangle = TWOPI - x;
			}

			x = ellipmode->x1;
			y = ellipmode->y1;
			ellipmode->x1 = x*tcos-y*tsin;
			ellipmode->y1 = x*tsin+y*tcos;

			ellipmode->rotation += rotang;
			ellipmode->fromang += rotang;
			ellipmode->toangle += rotang;

			normalize(ellipmode->rotation);
			normalize(ellipmode->fromang);
			normalize(ellipmode->toangle);

			ellipmode->x1 += xc;
			ellipmode->y1 += yc;
			break;

		case NODE_TEXT:
			if (dohatch) GenericRender(dohatch);
			dohatch = NULL;
			textmode = GetTextData(lnklst);

			{			
			
				STDFLT thx,thy,height,*sizar,width,slant;
				STDFLT ll,lr,pcos,psin;
				STYLE_A *instyle;
				UBYTE *inc;
				
				pcos = cos(textmode->ang * .01745329);
				psin = sin(textmode->ang * .01745329);
				
				/* Recalculate handle for center of box */
			
				instyle = textmode->letstyle;
				height = instyle->Prime_Height;
				slant = instyle->Slant;
				
				inc = textmode->text;

				width = 0.0;

				sizar = instyle->draw_font->font_link->Char_Widths;

				while (*inc) {
				
					width += sizar[(int) *inc];
					inc++;
				}

				width *= height * instyle->Aspect;

				switch(textmode->Handle_Flag) {

				case LEFT_HANDLE:
					ll = 0.0;
					lr = width;
					break;
				case CENTER_HANDLE:
					ll = - width/2.0;
					lr = - ll;
					break;
				case RIGHT_HANDLE:
					ll = - width;
					lr = 0.0;
					break;
				}
				
				thx = textmode->x1;
				thy = textmode->y1;
			
				y = height/2.0;
				x = ll + (slant*y) + width/2.0;
				
				textmode->x1 = x*pcos-y*psin+thx;
				textmode->y1 = x*psin+y*pcos+thy;

				textmode->x1 -= xh;
				textmode->y1 -= yh;

				textmode->x1 *= doscale;
				textmode->y1 *= doscale;

				if (pyflip) {
					textmode->ang = -textmode->ang;
					if (textmode->Text_Bits&TEXT_FLIPY)
						textmode->Text_Bits&=(~TEXT_FLIPY);
					else	textmode->Text_Bits|=TEXT_FLIPY;
				}
				if (pxflip) {
					textmode->ang = -(textmode->ang - 180.0);
					if (textmode->Text_Bits&TEXT_FLIPX)
						textmode->Text_Bits&=(~TEXT_FLIPX);
					else	textmode->Text_Bits|=TEXT_FLIPX;
				}

				x = textmode->x1;
				y = textmode->y1;
				
				textmode->x1 = x*tcos-y*tsin;
				textmode->y1 = x*tsin+y*tcos;

				textmode->x1 += xc;
				textmode->y1 += yc;

				thx = textmode->x1;
				thy = textmode->y1;
			
				textmode->ang += rotang / .01745329;
				while (textmode->ang>360.0) textmode->ang -= 360.0;
				while (textmode->ang<0.0)   textmode->ang += 360.0;
				
				y = - (height/2.0);
				x = -(ll + (slant*y) + width/2.0);
				
				/* Angle changed, so recalc */
				pcos = cos(textmode->ang * .01745329);
				psin = sin(textmode->ang * .01745329);
				
				textmode->x1 = x*pcos-y*psin+thx;
				textmode->y1 = x*psin+y*pcos+thy;

			}
			
			break;
		case NODE_HATCH:
			if (dohatch) GenericRender(dohatch);
			dohatch = lnklst;
			
		}

		if (lnklst->node_id!=NODE_HATCH) GenericRender(lnklst);
	}
	
	if (dohatch) GenericRender(dohatch);

}

/*
 *     +++
 *     +++
 *  +++++++++	Clip Clone Routine
 *  +++++++++
 *     +++
 *     +++
 */


VOID ClonePieces()
{

	BYTEBITS holdflag;
	D_NODE *dohatch;
	D_NODE *lnklst,*newlnk,*lastlink;
	BOOL useit;
	
	dohatch = NULL;
	lastlink = Last_Node;

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
	
		if (lnklst->node_previous==lastlink) break;
		
		useit = FALSE;
		holdflag=lnklst->node_flags;
		
		if (holdflag&NODE_INCLIP) {
			useit = TRUE;
		}
		
		if (ishatched(lnklst)) {
			newlnk = gethatch(lnklst);
			if (newlnk->node_flags&NODE_INCLIP) {
				useit=TRUE;
			}
			else useit=FALSE;
		}
		if (useit) {

			drawingdirty = TRUE;	/* Probably dirty now */

			switch(lnklst->node_id) {
			case NODE_LINE:
				newlnk = StoStrLine(GetLineData(lnklst));

				GetLineData(newlnk)->x1 += (ToX-OrgX);
				GetLineData(newlnk)->x2 += (ToX-OrgX);
				GetLineData(newlnk)->y1 += (ToY-OrgY);
				GetLineData(newlnk)->y2 += (ToY-OrgY);
				break;
	
			case NODE_ELLIP:
				newlnk = StoStrEllip(GetEllipData(lnklst));

				GetEllipData(newlnk)->x1 += (ToX-OrgX);
				GetEllipData(newlnk)->y1 += (ToY-OrgY);
				break;

			case NODE_TEXT:
				if (dohatch!=NULL) GenericRender(dohatch);
				dohatch = NULL;
				newlnk = StoStrText(GetTextData(lnklst));
			
				GetTextData(newlnk)->x1 += (ToX-OrgX);
				GetTextData(newlnk)->y1 += (ToY-OrgY);
				break;

			case NODE_HATCH:
				if (dohatch!=NULL) GenericRender(dohatch);
				dohatch = NULL;
				newlnk = StoStrHatch(GetHatchData(lnklst),NULL);
				dohatch = newlnk;
				break;
			}
	
/*  Insure that new drawing element retains old attributes - repair counts */

			newlnk->above_group->Use_Count--;
			newlnk->node_layer->Use_Count--;
			newlnk->pen_attach->Use_Count--;

			newlnk->above_group = lnklst->above_group;
			newlnk->node_layer = lnklst->node_layer;
			newlnk->pen_attach = lnklst->pen_attach;

			newlnk->above_group->Use_Count++;
			newlnk->node_layer->Use_Count++;
			newlnk->pen_attach->Use_Count++;
	
			newlnk->node_flags |= NODE_INCLIP;
			if (lnklst->node_id!=NODE_HATCH) GenericRender(newlnk);
			
		}
		
	}

	if (dohatch!=NULL) GenericRender(dohatch);
	dohatch = NULL;

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
		if (lnklst->node_previous==lastlink) break;
		if (!(lnklst->node_flags&NODE_INCLIP)) continue;
		
		lnklst->node_flags &= (~NODE_INCLIP);
		GenericRender(lnklst);	/* Unclip */
	}
		
	OrgX = ToX;
	OrgY = ToY;
	
}

/*
 *     +++
 *     +++
 *  +++++++++	Clone and Drag User Interface
 *  +++++++++
 *     +++
 *     +++
 */


BOOL Clip_Clone()
{

	if (!EnterSub(CLONE_MODE,CLONE_MESS_EXIT,PerformCopy)) return(FALSE);

	MainMess(CLONE_MESS_P1);

	cloneflag=TRUE;
	orgset=FALSE;

	return(FALSE);

}

BOOL Clip_Drag()
{

	if (!EnterSub(DRAG_MODE,DRAG_MESS_EXIT,PerformCopy)) return(FALSE);

	MainMess(DRAG_MESS_P1);

	cloneflag=FALSE;
	orgset=FALSE;

	return(FALSE);
	
}

VOID PerformCopy(sx,sy,action,xr,yr)
int sx,sy,action;
float xr,yr;
{

	if (action==RBUTTONDOWN) {
		SExitSub(cloneflag?(&CLONE_MESS_ABORT):(&DRAG_MESS_ABORT));
		return;
	}

	if (orgset&&action==LBUTTONDOWN) {
		ToX=xr;
		ToY=yr;
		if (cloneflag) ClonePieces();
		else FlipAbout(OrgX,OrgY,ToX,ToY,0.0,1.0,
				FALSE,FALSE,&DRAG_MESS_MOVE);
		if (!cloneflag) SExitSub(NULL);
		return;
	}

	if (action==LBUTTONDOWN) {
		OrgX=xr;
		OrgY=yr;
		SMainMess(cloneflag?(&CLONE_MESS_P2):(&DRAG_MESS_P2));
		orgset=TRUE;
		return;
	}

	return;
}

/*
 *     +++
 *     +++
 *  +++++++++	Even Scale User Interface
 *  +++++++++
 *     +++
 *     +++
 */


LOCAL int stapoint;
LOCAL STDFLT staX,staY;
LOCAL STDFLT fstX,fstY,fstlen;
LOCAL STDFLT secX,secY,seclen;
LOCAL STDFLT ratio;

VOID PerformScale(sx,sy,action,xr,yr)
int sx,sy,action;
float xr,yr;
{
	STDFLT tr;

	if (action==UMOVEMENT&&stapoint>1) {
		secX=xr;
		secY=yr;
		seclen = hypot(secX-staX, secY-staY);
		tr = seclen/fstlen*100.0;
		sprintf(pbuffer,"Scale %.2f%%",tr);

		paintinfo();
	}

	if (action==RBUTTONDOWN) {
		ExitSub(SCALE_MESS_ABORT);
		return;
	}

	if (action==LBUTTONDOWN) {
		switch (stapoint) {
		case 0:
			staX=xr;
			staY=yr;
			MainMess(SCALE_MESS_P2);
			stapoint=1;
			break;
		case 1:
			fstX=xr;
			fstY=yr;
			if (staX==xr&&staY==yr) {
				InfoMess(SCALE_MODE,SCALE_MESS_P1ERR);
				break;
			}
			fstlen = hypot(fstX-staX,fstY-staY);
			MainMess(SCALE_MESS_P3);
			stapoint=2;
			break;
		case 2:
			secX=xr;
			secY=yr;
			seclen = hypot(secX-staX, secY-staY);
			ratio = seclen/fstlen;
			FlipAbout(staX,staY,staX,staY,0.0,
				  ratio,FALSE,FALSE,&SCALE_MESS_SCALE);
			SExitSub(NULL);
			return;
		}
	}
}

BOOL Scale_Even()
{

	if (!EnterSub(SCALE_MODE,SCALE_MESS_EXIT,PerformScale)) return(FALSE);

	MainMess(SCALE_MESS_P1);

	stapoint=0;

	return(FALSE);

}

/*
 *     +++
 *     +++
 *  +++++++++	X & Y Flip User Interface
 *  +++++++++
 *     +++
 *     +++
 */


BOOL	doxflip;
BOOL	doyflip;

VOID PerformFlip(sx,sy,action,xr,yr)
int sx,sy,action;
float xr,yr;
{

	if (action==RBUTTONDOWN) {
		ExitSub(FLIP_MESS_ABORT);
		return;
	}

	if (action==LBUTTONDOWN) {
		FlipAbout(xr,yr,xr,yr,0.0,1.0,doxflip,doyflip,&FLIP_MESS_WAIT);
		SExitSub(NULL);
		return;
	}
}

BOOL x_flip()
{

	if (!EnterSub(XFLIP_MODE,FLIP_MESS_EXIT,PerformFlip)) return(FALSE);

	MainMess(FLIP_MESS_CENT);

	doxflip = TRUE;
	doyflip = FALSE;

	return(FALSE);

}

BOOL y_flip()
{

	if (!EnterSub(YFLIP_MODE,FLIP_MESS_EXIT,PerformFlip)) return(FALSE);

	MainMess(FLIP_MESS_CENT);

	doyflip = TRUE;
	doxflip = FALSE;

	return(FALSE);

}

STDFLT hypot(x,y)
STDFLT x,y;
{

	double a,b,c;
	STDFLT ans;

	a = x*x;
	b = y*y;

	c = sqrt(a+b);
	ans = c;
	return(ans);
}

/*
 *     +++
 *     +++
 *  +++++++++	Set Layer, Pen, Style, and Group to current
 *  +++++++++
 *     +++
 *     +++
 */


BOOL group_layer()
{

	D_NODE *lnklst;

	DispMess("Setting clipboard to current layer");

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {

		if (!(lnklst->node_flags&NODE_INCLIP)) continue;

		drawingdirty = TRUE;	/* Probably dirty now */

		lnklst->node_layer->Use_Count--;
		if (lnklst->node_layer->Use_Count<0) 
			lnklst->node_layer->Use_Count = 0;

		lnklst->node_layer = Current_Layer;
		lnklst->node_layer->Use_Count++;
	}

	return(TRUE);

}

BOOL group_group()
{

	D_NODE *lnklst;

	DispMess("Setting clipboard to current group");

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {

		if (!(lnklst->node_flags&NODE_INCLIP)) continue;
		if (lnklst->node_id==NODE_HATCH) 
			sethatchgroup(lnklst,Current_Group);
			
		if (ishatched(lnklst)) continue; /* Don't do lines */
		
		drawingdirty = TRUE;	/* Probably dirty now */

		lnklst->above_group->Use_Count--;
		if (lnklst->above_group->Use_Count<0) 
			lnklst->above_group->Use_Count = 0;

		lnklst->above_group = Current_Group;
		lnklst->above_group->Use_Count++;
	}

	return(TRUE);
}

BOOL group_pen()
{

	D_NODE *lnklst;

	DispMess("Setting clipboard to current pen");

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {

		if (!(lnklst->node_flags&NODE_INCLIP)) continue;
		
		drawingdirty = TRUE;	/* Probably dirty now */

		lnklst->pen_attach->Use_Count--;
		if (lnklst->pen_attach->Use_Count<0) 
			lnklst->pen_attach->Use_Count = 0;

		lnklst->pen_attach = Current_Pen;
		lnklst->pen_attach->Use_Count++;
	}

	return(TRUE);
}
	
BOOL group_lettering()
{

	D_NODE *lnklst;

	DispMess("Setting clipboard to current style");

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {

		if (!(lnklst->node_flags&NODE_INCLIP)) continue;
		if (lnklst->node_id!=NODE_TEXT) continue;
		
		drawingdirty = TRUE;	/* Probably dirty now */

		GetTextData(lnklst)->letstyle->Use_Count--;
		if (GetTextData(lnklst)->letstyle->Use_Count<0) 
			GetTextData(lnklst)->letstyle->Use_Count = 0;

		GetTextData(lnklst)->letstyle = Current_Style;
		GetTextData(lnklst)->letstyle->Use_Count++;
	}
	return(TRUE);
}

/*
 *     +++
 *     +++
 *  +++++++++	Rotate User Interface
 *  +++++++++
 *     +++
 *     +++
 */


BOOL rotshadow=FALSE;
STDFLT rotbeg,rotlen;

RotShadow()
{

	if (rotshadow) {
		SetDrPt(rp, 0xFFFF);
	
		SetDrMd(rp, COMPLEMENT | JAM1);
		SetAPen(rp,1);

		v_move(fstX,fstY);
		v_draw(staX,staY);
		v_draw(secX,secY);
		rotshadow = FALSE;
	}
}

VOID PerformRotate(sx,sy,action,xr,yr)
int sx,sy,action;
float xr,yr;
{
	STDFLT rotend,tr;

	RotShadow();

	if (action==RBUTTONDOWN) {
		ExitSub(ROTATE_MESS_ABORT);
		return;
	}

	if (action==UMOVEMENT&&stapoint==2) {
		rotend = atan2(yr-staY,xr-staX);
		secX = (cos(rotend)*rotlen)+staX;
		secY = (sin(rotend)*rotlen)+staY;
		rotshadow = TRUE;
		RotShadow();
		rotshadow = TRUE;
		tr = (rotend-rotbeg) / .01745329;
		while (tr>360.0) tr -= 360.0;
		while (tr<0.0) tr += 360.0;		/* Normalize */
		sprintf(pbuffer,"Rotate %.2f",tr);
		paintinfo();
		return;
	}
	if (action==UMOVEMENT&&stapoint==1) {
		fstX = xr;
		fstY = yr;
		rotshadow = TRUE;
		RotShadow();
		rotshadow = TRUE;
		return;
	}

	if (action==LBUTTONDOWN) {
		switch (stapoint) {
		case 0:
			staX=xr;
			staY=yr;
			secX=xr;
			secY=yr;
			fstX=xr;
			fstY=yr;
			MainMess(ROTATE_MESS_P2);
			stapoint=1;
			return;
		case 1:
			fstX=xr;
			fstY=yr;
			rotbeg = atan2(yr-staY,xr-staX);
			rotlen = hypot(fstX-staX,fstY-staY);
			MainMess(ROTATE_MESS_P3);
			stapoint=2;
			return;
		case 2:
			rotend = atan2(yr-staY,xr-staX);
			tr = rotend - rotbeg;
			FlipAbout(staX,staY,staX,staY,tr,
				  1.0,FALSE,FALSE,&ROTATE_MESS_ROTATE);
			SExitSub(NULL);
			return;
		}
	}
}

BOOL Clip_Rotate()
{

	if (!EnterSub(ROTATE_MODE,ROTATE_MESS_EXIT,PerformRotate)) 
		return(FALSE);

	MainMess(ROTATE_MESS_P1);

	stapoint=0;

	return(FALSE);

}

/*
 *     +++
 *     +++
 *  +++++++++	Sort, Make Open and Closed String
 *  +++++++++
 *     +++
 *     +++
 */


BOOL Make_String(inter,closed)
BOOL inter;
BOOL closed;
{

	D_NODE *lnklst,*temproot,*temptail,*sortroot,*sorttail,*sqfind[2],*newn;
	D_NODE *subs;
	STDFLT xend,yend,sqdist[2],traildist,a,b;
	UBYTE whichend[2];
	BOOL gotfirst;
	
	gotfirst = FALSE;

	temproot = temptail = NULL;	
	sortroot = sorttail = NULL;
	
	subs = NULL;
	for (lnklst=Last_Node; lnklst!=NULL; lnklst=subs) {
		subs = lnklst->node_previous;

		if (!(lnklst->node_flags&NODE_INCLIP)) continue;
		if (lnklst->node_id!=NODE_LINE) continue;
		if (ishatched(lnklst)) continue;
		
		drawingdirty = TRUE;	/* Probably dirty now */

		lnklst->node_flags|=NODE_DELETED;
		GenericRender(lnklst);	/* Unpaint this now */
		lnklst->node_flags&= ~NODE_DELETED;
	
		SnipObject(lnklst,&Root_Node,&Last_Node,FALSE);	
			
		if (!gotfirst) {
			AddToEnd(lnklst,&sortroot,&sorttail,FALSE);
			gotfirst = TRUE;
		} else 
			AddToEnd(lnklst,&temproot,&temptail,FALSE);
			
	}
	
	if (sortroot==NULL) return(TRUE);
	GenericRender(sortroot);
	
	while (temproot!=NULL) {
	    sqdist[0]=sqdist[1]= MAXBIG;
	    whichend[0]=whichend[1]=0;
	    sqfind[0]=sqfind[1]=NULL;
	    for (lnklst=temproot; lnklst!=NULL; lnklst=subs) {
		subs = lnklst->node_subsequent;

	    	a = GetLineData(sortroot)->x1 - GetLineData(lnklst)->x1;
	    	b = GetLineData(sortroot)->y1 - GetLineData(lnklst)->y1;
		traildist = (a*a) + (b*b);
		
		if (traildist<sqdist[0]) {
			sqdist[0] = traildist;
			sqfind[0] = lnklst;
			whichend[0] = 0;
		}
	
	    	a = GetLineData(sortroot)->x1 - GetLineData(lnklst)->x2;
	    	b = GetLineData(sortroot)->y1 - GetLineData(lnklst)->y2;
		traildist = (a*a) + (b*b);
		
		if (traildist<sqdist[0]) {
			sqdist[0] = traildist;
			sqfind[0] = lnklst;
			whichend[0] = 1;
		}
	
	    	a = GetLineData(sorttail)->x2 - GetLineData(lnklst)->x1;
	    	b = GetLineData(sorttail)->y2 - GetLineData(lnklst)->y1;
		traildist = (a*a) + (b*b);
		
		if (traildist<sqdist[1]) {
			sqdist[1] = traildist;
			sqfind[1] = lnklst;
			whichend[1] = 0;
		}
	
	    	a = GetLineData(sorttail)->x2 - GetLineData(lnklst)->x2;
	    	b = GetLineData(sorttail)->y2 - GetLineData(lnklst)->y2;
		traildist = (a*a) + (b*b);
		
		if (traildist<sqdist[1]) {
			sqdist[1] = traildist;
			sqfind[1] = lnklst;
			whichend[1] = 1;
		}
	    }
	    
	    if (sqdist[0]<sqdist[1]) { 	/* Put the node on the root */
	    
	    	if (sqfind[0]!=NULL) {
		
			if (whichend[0]==0) {	/* Do and end flip */
				a = GetLineData(sqfind[0])->x1;
				GetLineData(sqfind[0])->x1=
					GetLineData(sqfind[0])->x2;
				GetLineData(sqfind[0])->x2=a;
				a = GetLineData(sqfind[0])->y1;
				GetLineData(sqfind[0])->y1=
					GetLineData(sqfind[0])->y2;
				GetLineData(sqfind[0])->y2=a;
			}
		
			SnipObject(sqfind[0],&temproot,&temptail,FALSE);
			
			AddToRoot(sqfind[0],&sortroot,&sorttail,FALSE);
			GenericRender(sqfind[0]);
		}

	    } else {
		
	    	if (sqfind[1]!=NULL) {
		
			if (whichend[1]==1) {	/* Do and end flip */
				a = GetLineData(sqfind[1])->x1;
				GetLineData(sqfind[1])->x1=
					GetLineData(sqfind[1])->x2;
				GetLineData(sqfind[1])->x2=a;
				a = GetLineData(sqfind[1])->y1;
				GetLineData(sqfind[1])->y1=
					GetLineData(sqfind[1])->y2;
				GetLineData(sqfind[1])->y2=a;
			}
		
			SnipObject(sqfind[1],&temproot,&temptail,FALSE);
			
			AddToEnd(sqfind[1],&sortroot,&sorttail,FALSE);
			GenericRender(sqfind[1]);
		}
	    }
	}

	xend = GetLineData(sortroot)->x1;
	yend = GetLineData(sortroot)->y1;
	
	for (lnklst=sortroot; lnklst!=NULL; lnklst=subs) {
		subs = lnklst->node_subsequent;
		newn = NULL;
		if (lnklst->node_subsequent==NULL) {
			if (closed) {
			    newn = CopyNode(lnklst);
			    if (newn!=NULL) {
			     GetLineData(newn)->x1=GetLineData(lnklst)->x2;
			     GetLineData(newn)->y1=GetLineData(lnklst)->y2;
			     GetLineData(newn)->x2=xend;
			     GetLineData(newn)->y2=yend;
			     
			    }
			}
		} else {
			if ((GetLineData(lnklst)->x2!=
			       GetLineData(lnklst->node_subsequent)->x1) ||
			    (GetLineData(lnklst)->y2!=
			       GetLineData(lnklst->node_subsequent)->y1)) {
			       
			    newn = CopyNode(lnklst);
			    if (newn!=NULL) {
			     GetLineData(newn)->x1=GetLineData(lnklst)->x2;
			     GetLineData(newn)->y1=GetLineData(lnklst)->y2;
			     GetLineData(newn)->x2=
			        GetLineData(lnklst->node_subsequent)->x1;
			     GetLineData(newn)->y2=
			        GetLineData(lnklst->node_subsequent)->y1;
			    }
			}
		}
		
		SnipObject(lnklst,&sortroot,&sorttail,FALSE);
		AddToEnd(lnklst,&Root_Node,&Last_Node,FALSE);
		if (newn!=NULL&&inter) {
			AddToEnd(newn,&Root_Node,&Last_Node,TRUE);
			if (CurrentRoller!=NULL) {	/* Edit 7 */
				CurrentRoller->node_flags &= (~NODE_SELECTED);
				v_reset();
				GenericRender(CurrentRoller);
			}

			CurrentRoller=newn;
			CurrentRoller->node_flags |= NODE_SELECTED;
			v_reset();
			GenericRender(newn);
		}
	}
	
	return(TRUE);
}

BOOL Clip_String_Open() 	{ return(Make_String(TRUE,FALSE)); }
BOOL Clip_String_Closed() 	{ return(Make_String(TRUE,TRUE)); }
BOOL Sort_Clip()		{ return(Make_String(FALSE,FALSE)); }

/*
 *     +++
 *     +++
 *  +++++++++	Copy clipped items to top and bottom
 *  +++++++++
 *     +++
 *     +++
 */


BOOL Copy_Top()
{

	D_NODE *lnklst,*temproot,*temptail,*subs,*hatched;
	BOOL	doit;

	temproot = temptail = NULL;	
	
	/* Copy items into temporary linked list */
	
	subs = NULL;
	for (lnklst=Last_Node; lnklst!=NULL; lnklst=subs) {
		subs = lnklst->node_previous;
		
		doit = FALSE;

		if (lnklst->node_flags&NODE_INCLIP) doit = TRUE;
		
		if (ishatched(lnklst)) {
			hatched = gethatch(lnklst);
			if (hatched->node_flags&NODE_INCLIP) doit = TRUE;
			else doit = FALSE;
		}

		if (!doit) continue;
		
		drawingdirty = TRUE;	/* Probably dirty now */
		
		SnipObject(lnklst,&Root_Node,&Last_Node,FALSE);	

		AddToRoot(lnklst,&temproot,&temptail,FALSE);
			
	}
	
	while (temptail) {
		lnklst = temptail;
		SnipObject(lnklst,&temproot,&temptail,FALSE);
		AddToRoot(lnklst,&Root_Node,&Last_Node,FALSE);
	}
	
	return(TRUE);
}

BOOL Copy_Bottom()
{

	D_NODE *lnklst,*temproot,*temptail,*subs,*hatched;
	BOOL doit;

	temproot = temptail = NULL;	
	
	/* Copy items into temporary linked list */
	
	subs = NULL;
	for (lnklst=Last_Node; lnklst!=NULL; lnklst=subs) {
		subs = lnklst->node_previous;

		doit = FALSE;
		if (lnklst->node_flags&NODE_INCLIP) doit = TRUE;
		
		if (ishatched(lnklst)) {
			hatched = gethatch(lnklst);
			if (hatched->node_flags&NODE_INCLIP) doit = TRUE;
			else doit = FALSE;
		}

		if (!doit) continue;
		
		drawingdirty = TRUE;	/* Probably dirty now */
		
		SnipObject(lnklst,&Root_Node,&Last_Node,FALSE);	

		AddToRoot(lnklst,&temproot,&temptail,FALSE);
			
	}
	
	while (temproot) {
		lnklst = temproot;
		SnipObject(lnklst,&temproot,&temptail,FALSE);
		AddToEnd(lnklst,&Root_Node,&Last_Node,FALSE);
	}
	
	return(TRUE);
}
