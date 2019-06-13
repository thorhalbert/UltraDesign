/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	searcher.c - Find and select objects
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
 *	4	 7-Feb-88 SH	Add the edit text function
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include <graphics/gfxmacros.h>

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

extern	BOOL	Point1Fixed;
extern	STDFLT	Line1X,Line1Y;
extern	struct	RastPort *rp;

/*-------------------------Global Variable Definitions-----------------------*/

STDFLT	perx,pery;

/*-------------------------------Code Section--------------------------------*/

STDFLT LineDistance(curline,ix,iy)
D_NODE *curline;
STDFLT ix,iy;
{

	O_LINE *drawline;
	STDFLT fx1,fx2,fy1,fy2,d1,d2,d3,m0,m1,m02,b0,b1,dy,dx;
	BOOL horiz;
	
	drawline=GetLineData(curline);

	fx1=drawline->x1;
	fx2=drawline->x2;
	fy1=drawline->y1;
	fy2=drawline->y2;

	dy = (fy1-fy2);
	dx = (fx1-fx2);

	horiz = FALSE;
	if (dx!=0.0&&(abs(dy/dx)<.001)) horiz = TRUE;  /* Close enough */

	if (horiz) {
		perx = ix;
		pery = fy1;
		if ((perx>=min(fx1,fx2))&&(perx<=max(fx1,fx2))) {
			d3=hypot(ix-perx,iy-pery);
			return(d3);
		}
	} else if (dx==0.0) {
		pery = iy;
		perx = fx1;
		if ((pery>=min(fy1,fy2))&&(pery<=max(fy1,fy2))) {
			d3=hypot(ix-perx,iy-pery);
			return(d3);
		}
	} else {
		m0 = dy / dx;
		m1 = -(1.0 / m0);
		b0 = fy1 - (m0 * fx1);
		b1 = iy - (m1 * ix);
		m02 = m0 * m0;
		pery = ((b1 * m02) + b0) / (1.0 + m02);
		perx = (pery - b0) / m0;
		dy = abs(dy);
		dx = abs(dx);
		if (dy>=dx&&(pery>=min(fy1,fy2))&&(pery<=max(fy1,fy2))) {
			d3=hypot(ix-perx,iy-pery);
			return(d3);
		}
		if (dx>=dy&&(perx>=min(fx1,fx2))&&(perx<=max(fx1,fx2))) {
			d3=hypot(ix-perx,iy-pery);
			return(d3);
		}
	}

	d1=hypot(ix-fx1,iy-fy1);
	d2=hypot(ix-fx2,iy-fy2);

	d3 = min(d1,d2);

	return(d3);			/* Find THE closest */

}

STDFLT EllipseDistance(curellip,ix,iy)
D_NODE *curellip;
STDFLT ix,iy;
{

	struct 	ellip_mode *drawellip;

	STDFLT 	fx1,fy1,bega,enda,rot,radx,rady,rotcos,rotsin;
	STDFLT	p1x,p1y,p2x,p2y,mpx,mpy,xn,yn,thet,thete;

	drawellip=GetEllipData(curellip);

	fx1 = drawellip->x1;
	fy1 = drawellip->y1;
	bega = drawellip->fromang;
	enda = drawellip->toangle;
	radx = drawellip->xradius;
	rady = drawellip->yradius;
	rot = drawellip->rotation;

	rotcos = cos(rot);
	rotsin = sin(rot);

	thet = atan2(iy-fy1,ix-fx1);
	thete = thet - rot;

	normalize(thete);
	normalize(thet);

	thete=ConvEllipNotation(thete,radx,rady);

	xn=cos(thete)*radx;		/* Calc start */
	yn=sin(thete)*rady;

	mpx = ((xn * rotcos) - (yn * rotsin)) + fx1; /* Rot ellipse */
	mpy = ((yn * rotcos) + (xn * rotsin)) + fy1;

	if (enda<=(bega+.001)) enda += TWOPI;	/* Equalize (min tolerance) */
	if (thet<bega) thet+=TWOPI;

	if (thet>=bega&&thet<=enda)
		return(hypot(ix-mpx,iy-mpy));

	bega -= rot;
	enda -= rot;
	normalize(bega);
	normalize(enda);

	bega=ConvEllipNotation(bega,radx,rady);
	enda=ConvEllipNotation(enda,radx,rady);

	xn=cos(bega)*radx;		/* Calc start */
	yn=sin(bega)*rady;

	p1x = ((xn * rotcos) - (yn * rotsin)) + fx1; /* Rot ellipse */
	p1y = ((yn * rotcos) + (xn * rotsin)) + fy1;

	xn=cos(enda)*radx;		/* Calc start */
	yn=sin(enda)*rady;

	p2x = ((xn * rotcos) - (yn * rotsin)) + fx1; /* Rot ellipse */
	p2y = ((yn * rotcos) + (xn * rotsin)) + fy1;

	return(min(hypot(ix-p1x,iy-p1y),hypot(ix-p2x,iy-p2y)));
		
}

STDFLT TextDistance(curtext,ix,iy)
D_NODE *curtext;
STDFLT ix,iy;
{

	UBYTE *inc;
	STDFLT *sizar;
	STDFLT x,y,ll,lr,wd,text_acos,text_asin;
	STDFLT fx1,fx2,fy1,fy2,d1,d2,d3,m0,m1,m02,b0,b1,t1,t2;
	O_TEXT *drawtext;

	drawtext=GetTextData(curtext);

	text_acos = cos(drawtext->ang * .01745329);
	text_asin = sin(drawtext->ang * .01745329);

	inc = drawtext->text;

	wd = 0.0;

	sizar = drawtext->letstyle->draw_font->font_link->Char_Widths;

	while (*inc) {
		
		wd += sizar[(int) *inc];
		inc++;
	}

	wd *= drawtext->letstyle->Prime_Height;
	wd *= Current_Style->Aspect;

	switch(drawtext->Handle_Flag) {

	case LEFT_HANDLE:
		ll = 0.0;
		lr = wd;
		break;
	case CENTER_HANDLE:
		ll = - wd/2.0;
		lr = - ll;
		break;
	case RIGHT_HANDLE:
		ll = - wd;
		lr = 0.0;
		break;
	}
	
	y = 0.0;
	x = ll;
	
	fx1 = x*text_acos-y*text_asin+drawtext->x1;
	fy1 = x*text_asin+y*text_acos+drawtext->y1;

	y = 0.0;
	x = lr;
	
	fx2 = x*text_acos-y*text_asin+drawtext->x1;
	fy2 = x*text_asin+y*text_acos+drawtext->y1;

	t1 = (fy1-fy2);
	t2 = (fx1-fx2);

	if (t1==0.0) {
		perx = ix;
		pery = fy1;
		if ((perx>=min(fx1,fx2))&&(perx<=max(fx1,fx2))) {
			d3=hypot(ix-perx,iy-pery);
			return(d3);
		}
	} else if (t2==0.0) {
		pery = iy;
		perx = fx1;
		if ((pery>=min(fy1,fy2))&&(pery<=max(fy1,fy2))) {
			d3=hypot(ix-perx,iy-pery);
			return(d3);
		}
	} else {
		m0 = t1 / t2;
		m1 = -(1.0 / m0);
		b0 = fy1 - (m0 * fx1);
		b1 = iy - (m1 * ix);
		m02 = m0 * m0;
		pery = ((b1 * m02) + b0) / (1.0 + m02);
		perx = (pery - b0) / m0;
		if ((pery>=min(fy1,fy2))&&(pery<=max(fy1,fy2))) {
			d3=hypot(ix-perx,iy-pery);
			return(d3);
		}
	}

	d1=hypot(ix-fx1,iy-fy1);
	d2=hypot(ix-fx2,iy-fy2);

	return(min(d1,d2));		/* Find THE closest */

}

STDFLT GenericDistance(innode,ix,iy)
D_NODE *innode;
STDFLT ix,iy;
{

	if (!(innode->node_flags&NODE_ONSCREEN)) return(MAXBIG);

	switch (innode->node_id) {

	case NODE_LINE: 	return(LineDistance(innode,ix,iy));
 	case NODE_ELLIP: 	return(EllipseDistance(innode,ix,iy));
	case NODE_TEXT: 	return(TextDistance(innode,ix,iy));

	}

	return(MAXBIG);		/* Impossibly far away */
}

extern D_NODE *CurrentRoller;

D_NODE *PickSelect(ix,iy,typeonly)
STDFLT ix,iy;
int typeonly;
{

	D_NODE *lnklst,*kepmin;
	STDFLT curmin=MAXBIG,rmin;

	kepmin = NULL;
	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
		if (typeonly!=0 && typeonly!=lnklst->node_id) continue;
	
		if (!SelectLegal(lnklst,FALSE,TRUE,TRUE)) continue;

		rmin = GenericDistance(lnklst,ix,iy);

		if (rmin>curmin) continue;
		curmin = rmin;
		kepmin = lnklst;
	}
	return(kepmin);

}

D_NODE *FindItem=NULL;
FindSelect(ix,iy,typeonly,quiet)
STDFLT ix,iy;
int typeonly;
BOOL quiet;
{

	FindItem = PickSelect(ix,iy,typeonly);
	if (FindItem==NULL) return;

	if (FindItem!=CurrentRoller) {
		if (CurrentRoller!=NULL) {
			CurrentRoller->node_flags &= (~NODE_SELECTED);
			GenericRender(CurrentRoller);
		}

		CurrentRoller = FindItem;

		CurrentRoller->node_flags |= NODE_SELECTED;
		if (!quiet) GenericRender(CurrentRoller);
	}

}

VOID FindItemScan(sx,sy,action,xr,yr)
WORD sx,sy,action;
STDFLT xr,yr;
{

	if (action==RBUTTONDOWN) {
			ExitSub(SEARCH_MESS_ABORT);
			return;
	}

	switch (action) {
	case LBUTTONDOWN:
		InfoMess(SEARCH_MODE,SEARCH_MESS_WAIT);
		FindSelect(xr,yr,0,FALSE);	/* Search All types */
		SExitSub(NULL);
		return;

	}
}

Searcher()
{

	if (!EnterSub(SEARCH_MODE,SEARCH_MESS_EXIT,FindItemScan)) return(FALSE);
	MainMess(SEARCH_MESS_FIND);

	return(FALSE);
}

#define SEEKMID		1
#define SEEKEND		2
#define SEEKPERP	3
#define SEEKCENT	4
#define SEEKTANG	5
#define SEEKTEXT	6

BOOL	SeekMid()	{ SeekItem(SEEKMID); return(FALSE); }
BOOL	SeekEnd()	{ SeekItem(SEEKEND); return(FALSE); }
BOOL	SeekPerp()	{ SeekItem(SEEKPERP); return(FALSE); }
BOOL	SeekCent()	{ SeekItem(SEEKCENT); return(FALSE); }
BOOL	SeekTang()	{ SeekItem(SEEKTANG); return(FALSE); }
BOOL	SeekText()	{ SeekItem(SEEKTEXT); return(FALSE); }

int seektype;

VOID SeekItemScan(sx,sy,action,xr,yr)
WORD sx,sy,action;
STDFLT xr,yr;
{

	if (action==RBUTTONDOWN) {
		ExitSide(SEEK_MESS_ABORT);
		return;
	}

	switch (action) {
	case LBUTTONDOWN:
		InfoMess(SEEK_MODE,SEEK_MESS_WAIT);
		SeekSelect(xr,yr,seektype);	/* Search All types */
		return;

	}
}

SeekItem(intype)
int intype;
{

	if (!EnterSide(SEEK_MODE,SEEK_MESS_EXIT,SeekItemScan)) return;
	MainMess(SEEK_MESS_FIND);

	seektype = intype;

}

SeekSelect(ix,iy)
STDFLT ix,iy;
{
	int typeonly;
	D_NODE *kepmin;
	O_LINE *drawline;
	O_ELLIP *drawellip;
	O_TEXT *drawtext;
	STDFLT x,y,d1,d2,rotsin,rotcos;
	STDFLT bega,enda,radx,rady,rot,fx1,fy1,p1x,p1y,p2x,p2y,xn,yn;

	typeonly = 0;
	switch (seektype) {
	case SEEKMID:
	case SEEKEND:
		typeonly = 0;
		break;	
	case SEEKPERP:	
		typeonly = NODE_LINE;
		break;
	case SEEKCENT:	
	case SEEKTANG:
		typeonly = NODE_ELLIP;
		break;	
	case SEEKTEXT:
		typeonly = NODE_TEXT;
		break;
	}

	kepmin = PickSelect(ix,iy,typeonly);

	SExitSide(NULL);	/* Exit so pointeractions will work */

	if (kepmin==NULL) return;

	switch (seektype) {
	case SEEKMID:
		switch(kepmin->node_id) {
		case NODE_LINE:
			drawline=GetLineData(kepmin);
			x = ((drawline->x1+drawline->x2)/2.0);
			y = ((drawline->y1+drawline->y2)/2.0);
			PointerAction(0,0,LBUTTONDOWN,x,y);
			return;
		case NODE_ELLIP:
			drawellip=GetEllipData(kepmin);

			fx1 = drawellip->x1;
			fy1 = drawellip->y1;
			bega = drawellip->fromang;
			enda = drawellip->toangle;
			radx = drawellip->xradius;
			rady = drawellip->yradius;
			rot = drawellip->rotation;

			rotcos = cos(rot);
			rotsin = sin(rot);

			bega -= rot;
			enda -= rot;

			if (enda<=(bega+.001)) enda += TWOPI;
			bega += (enda - bega)/2.0;
			normalize(bega);

			bega=ConvEllipNotation(bega,radx,rady);

			xn=cos(bega)*radx;		/* Calc start */
			yn=sin(bega)*rady;

			p1x = ((xn * rotcos) - (yn * rotsin)) + fx1; /* Rot ellipse */
			p1y = ((yn * rotcos) + (xn * rotsin)) + fy1;

			PointerAction(0,0,LBUTTONDOWN,p1x,p1y);
			return;
		}
		return;

	case SEEKEND:
		switch(kepmin->node_id) {
		case NODE_LINE:
			drawline=GetLineData(kepmin);
			d1=hypot(ix-drawline->x1,iy-drawline->y1);
			d2=hypot(ix-drawline->x2,iy-drawline->y2);

			if (d1<d2) 
			     PointerAction(0,0,LBUTTONDOWN,drawline->x1,drawline->y1);
			else
			     PointerAction(0,0,LBUTTONDOWN,drawline->x2,drawline->y2);
			return;
		case NODE_ELLIP:
			drawellip=GetEllipData(kepmin);

			fx1 = drawellip->x1;
			fy1 = drawellip->y1;
			bega = drawellip->fromang;
			enda = drawellip->toangle;
			radx = drawellip->xradius;
			rady = drawellip->yradius;
			rot = drawellip->rotation;

			rotcos = cos(rot);
			rotsin = sin(rot);

			bega -= rot;
			enda -= rot;
			normalize(bega);
			normalize(enda);

			bega=ConvEllipNotation(bega,radx,rady);
			enda=ConvEllipNotation(enda,radx,rady);

			xn=cos(bega)*radx;		/* Calc start */
			yn=sin(bega)*rady;

			p1x = ((xn * rotcos) - (yn * rotsin)) + fx1; /* Rot ellipse */
			p1y = ((yn * rotcos) + (xn * rotsin)) + fy1;

			xn=cos(enda)*radx;		/* Calc start */
			yn=sin(enda)*rady;

			p2x = ((xn * rotcos) - (yn * rotsin)) + fx1; /* Rot ellipse */
			p2y = ((yn * rotcos) + (xn * rotsin)) + fy1;

			d1 = hypot(ix-p1x,iy-p1y);
			d2 = hypot(ix-p2x,iy-p2y);

			if (d1<d2)   PointerAction(0,0,LBUTTONDOWN,p1x,p1y);
			else	     PointerAction(0,0,LBUTTONDOWN,p2x,p2y);
			return;
		}
		return;
	case SEEKPERP:	
		drawline=GetLineData(kepmin);
		if (Point1Fixed) {
			(VOID) LineDistance(kepmin,Line1X,Line1Y);
			PointerAction(0,0,LBUTTONDOWN,perx,pery);
		}
		return;
	case SEEKCENT:	
		drawellip=GetEllipData(kepmin);
		PointerAction(0,0,LBUTTONDOWN,drawellip->x1,drawellip->y1);
		return;
	case SEEKTANG:
		drawellip=GetEllipData(kepmin);
		return;
	case SEEKTEXT:
		drawtext=GetTextData(kepmin);
		PointerAction(0,0,LBUTTONDOWN,drawtext->x1,drawtext->y1);
		return;
	}

}

/*
 *     +++
 *     +++
 *  +++++++++		Line Hook
 *  +++++++++
 *     +++
 *     +++
 */

int 	hookstage=0;
struct 	drawing_node *hookitem;
struct	line_mode *hookline;
STDFLT	hookx1,hooky1,hookx2,hooky2,hookx3,hooky3;
STDFLT	shookx2,shooky2;
BOOL	shookset=FALSE;

VOID FindHookSelect(ix,iy,typeonly)
STDFLT ix,iy;
int typeonly;
{

	D_NODE *lnklst,*kepmin;
	STDFLT curmin=MAXBIG,rmin;

	kepmin = NULL;
	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
		if (typeonly!=0 && typeonly!=lnklst->node_id) continue;

		if (!SelectLegal(lnklst,FALSE,TRUE,TRUE)) continue;

		rmin = GenericDistance(lnklst,ix,iy);

		if (rmin>curmin) continue;
		curmin = rmin;
		kepmin = lnklst;
	}
	hookitem = kepmin;
	if (kepmin!=NULL) {
		hookline = GetLineData(hookitem);
		hookx1 = hookline->x1;
		hooky1 = hookline->y1;
		hookx3 = hookline->x2;
		hooky3 = hookline->y2;
		
		hookitem->node_flags |= NODE_DELETED;
		GenericRender(hookitem);
		hookitem->node_flags &= (~NODE_DELETED);
		
	}

}

HookShadows()
{

	GI_DrawSetup(1,1);

	if (shookset) {
		v_move(hookx1,hooky1);
		v_draw(shookx2,shooky2);
		v_draw(hookx3,hooky3);
	}

	shookset = FALSE;
}
	
VOID LineHookScan(sx,sy,action,xr,yr)
WORD sx,sy,action;
STDFLT xr,yr;
{

	struct	Group_Attrib	*curgrpsave;
	struct	Layer_Attrib	*curlaysave;
	struct	Pen_Attrib	*curpensave;
	int	fg;

	HookShadows();

	if (action==RBUTTONDOWN) {
		switch (hookstage) {
		case 0:
			ExitSub(LINHOOK_MESS_ABORT);
			return;
		case 1:
			ExitSub(LINHOOK_MESS_ABORT);
			if (hookitem!=NULL) GenericRender(hookitem);
			return;
		}
		return;
	}

	switch (action) {
	case LBUTTONDOWN:
		switch (hookstage) {
		case 0:
			InfoMess(LINHOOK_MODE,LINHOOK_MESS_WAIT);
			FindHookSelect(xr,yr,NODE_LINE);
			if (hookitem==NULL) {
				SExitSub(NULL);
				return;
			}
			hookstage = 1;
			MainMess(LINHOOK_MESS_PLACE);
			return;
		case 1:
			fg = hookline->line_flags;

			hookline->x1 = hookx1;
			hookline->y1 = hooky1;
			hookline->x2 = xr;
			hookline->y2 = yr;
			hookline->line_flags = (fg & (~LINE_P2ARROW));
			GenericRender(hookitem);

			curgrpsave = Current_Group;
			curlaysave = Current_Layer;
			curpensave = Current_Pen;
	
			Current_Group = hookitem->above_group;
			Current_Layer = hookitem->node_layer;
			Current_Pen = hookitem->pen_attach;

			AttachLine(hookitem,xr,yr,hookx3,hooky3,
				fg & (~LINE_P1ARROW));

			Current_Group = curgrpsave;
			Current_Layer = curlaysave;
			Current_Pen = curpensave;

			SExitSub(NULL);
			
			return;
		}
	case UMOVEMENT:
	case MOVEMENT:
		if (hookstage!=1) return;
		hookx2 = xr;
		hooky2 = yr;
		shookx2 = hookx2;
		shooky2 = hooky2;
		
		shookset = TRUE;
		HookShadows();
		shookset = TRUE;
		return;
	}
}

LineHook()
{

	if (!EnterSub(LINHOOK_MODE,LINHOOK_MESS_EXIT,LineHookScan)) return(FALSE);
	MainMess(LINHOOK_MESS_FIND);

	hookstage = 0;
	hookitem = NULL;

	return(FALSE);
}

/*
 *     +++
 *     +++
 *  +++++++++		Move Line Endpoint
 *  +++++++++
 *     +++
 *     +++
 */


int 	unhendstage=0;
struct 	drawing_node *unhenditem,*unhenditem2;
struct	line_mode *unhendline,*unhendline2;
STDFLT	unhendx1,unhendy1,unhendx2,unhendy2,unhendx3,unhendy3;
STDFLT	sunhendx2,sunhendy2;
int	unhend;
BOOL	sunhendset=FALSE,unhend2;

LOCAL	BOOL	ThruLock = FALSE,
		singul;
	
LOCAL	STDFLT	LockAngle,
		LockTan,
		Lockdx,
		Lockdy,
		LockX,
		LockY;

VOID UnhendDrawThru()
{
	
	STDFLT dx,dy;
	
	dx = unhendx2 - unhendx1;
	dy = unhendy2 - unhendy1;
	
	if (abs(Lockdx)>abs(Lockdy)) {	/* More mouse resolution in x */
	
		if (dx==0.0) {
			unhendy2 = unhendy1;
			return;
		}
		
		unhendy2 = unhendy1 + (LockTan * dx);
		return;
	}
	
	if (singul) {	/* Vertical line */
	
		unhendx2 = unhendx1;
		return;
	}
	
	unhendx2 = unhendx1 + (dy / LockTan);
}


VOID FindUnhendSelect(ix,iy,typeonly)
STDFLT ix,iy;
int typeonly;
{

	D_NODE *lnklst,*kepmin;
	STDFLT curmin=MAXBIG,rmin;

	kepmin = NULL;
	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
		if (typeonly!=0 && typeonly!=lnklst->node_id) continue;

		if (!SelectLegal(lnklst,FALSE,TRUE,TRUE)) continue;

		rmin = GenericDistance(lnklst,ix,iy);

		if (rmin>curmin) continue;
		curmin = rmin;
		kepmin = lnklst;
	}
	unhenditem = kepmin;
	if (kepmin!=NULL) {
		unhendline = GetLineData(unhenditem);
		
		unhendx1 = unhendline->x1;
		unhendy1 = unhendline->y1;
		unhendx2 = unhendline->x2;
		unhendy2 = unhendline->y2;
		
		if (ThruLock) {
			Lockdx = unhendx2 - unhendx1;
			Lockdy = unhendy2 - unhendy1;

			LockAngle = atan2(Lockdy,Lockdx);
	
			singul = FALSE;
			if (Lockdx!=0.0) LockTan = tan(LockAngle);
			else singul = TRUE;
	
		}
		
		unhend = 1;
		if (hypot(ix-unhendx1,iy-unhendy1)<
		    hypot(ix-unhendx2,iy-unhendy2)) unhend = 0;
		    
		switch (unhend) {
		case 0:	
			unhendx2 = unhendline->x1;
			unhendy2 = unhendline->y1;
			unhendx1 = unhendline->x2;
			unhendy1 = unhendline->y2; 
			break;
		case 1:
			unhendx1 = unhendline->x1;
			unhendy1 = unhendline->y1;
			unhendx2 = unhendline->x2;
			unhendy2 = unhendline->y2;
			break;
		}
		    
		LockX = unhendx1;
		LockY = unhendx2;
		
		unhend2 = FALSE;
		switch (unhend) {
		case 0:
			unhenditem2 = unhenditem->node_previous;
			if (unhenditem2!=NULL) 
			    if (unhenditem2->node_id==NODE_LINE) {
				
				unhendline2 = GetLineData(unhenditem2);
				unhendx3 = unhendline2->x1;
				unhendy3 = unhendline2->y1;
				if (unhendline2->x2==unhendx2 &&
				    unhendline2->y2==unhendy2) 
				    	unhend2 = TRUE;
			}
			break;
		case 1:
			unhenditem2 = unhenditem->node_subsequent;
			if (unhenditem2!=NULL) 
			    if (unhenditem2->node_id==NODE_LINE) {
				
				unhendline2 = GetLineData(unhenditem2);
				unhendx3 = unhendline2->x2;
				unhendy3 = unhendline2->y2;
				if (unhendline2->x1==unhendx2 &&
				    unhendline2->y1==unhendy2) 
				    	unhend2 = TRUE;
			}
			break;
		}
			
		unhenditem->node_flags |= NODE_DELETED;
		GenericRender(unhenditem);
		unhenditem->node_flags &= (~NODE_DELETED);
		
		if (unhend2) {
			unhenditem2->node_flags |= NODE_DELETED;
			GenericRender(unhenditem2);
			unhenditem2->node_flags &= (~NODE_DELETED);
		}
	}

}

UnhendShadows()
{

	GI_DrawSetup(1,1);

	if (sunhendset) {
		v_move(unhendx1,unhendy1);
		v_draw(sunhendx2,sunhendy2);
		if (unhend2) v_draw(unhendx3,unhendy3);
	}

	sunhendset = FALSE;
}
	
VOID LineUnhendScan(sx,sy,action,xr,yr)
WORD sx,sy,action;
STDFLT xr,yr;
{

	UnhendShadows();

	if (action==RBUTTONDOWN) {
		switch (unhendstage) {
		case 0:
			ExitSub(UNHEND_MESS_ABORT);
			return;
		case 1:
			ExitSub(UNHEND_MESS_ABORT);
			if (unhenditem!=NULL) 	GenericRender(unhenditem);
			if (unhend2) 		GenericRender(unhenditem2);
			return;
		}
		return;
	}

	switch (action) {
	case LBUTTONDOWN:
		switch (unhendstage) {
		case 0:
			InfoMess(UNHEND_MODE,UNHEND_MESS_WAIT);
			FindUnhendSelect(xr,yr,NODE_LINE);
			if (unhenditem==NULL) {
				SExitSub(NULL);
				return;
			}
			unhendstage = 1;
			MainMess(UNHEND_MESS_PLACE);
			return;
		case 1:
			unhendx2 = xr;
			unhendy2 = yr;		
			if (ThruLock) UnhendDrawThru();
			
			switch (unhend) {
			
			case 0:
				unhendline->x1 = unhendx2;
				unhendline->y1 = unhendy2;
				if (unhend2) {
					unhendline2->x2 = unhendx2;
					unhendline2->y2 = unhendy2;
				}
				break;
			case 1:
				unhendline->x2 = unhendx2;
				unhendline->y2 = unhendy2;
				if (unhend2) {
					unhendline2->x1 = unhendx2;
					unhendline2->y1 = unhendy2;
				}
			}
	
			v_reset();		
			GenericRender(unhenditem);
			v_reset();
			if (unhend2) GenericRender(unhenditem2);

			SExitSub(NULL);
			
			return;
		}
	case UMOVEMENT:
	case MOVEMENT:
		if (unhendstage!=1) return;
		unhendx2 = xr;
		unhendy2 = yr;
		if (ThruLock) UnhendDrawThru();
		sunhendx2 = unhendx2;
		sunhendy2 = unhendy2;
		
		sunhendset = TRUE;
		UnhendShadows();
		sunhendset = TRUE;
		return;
	}
}

LineUnhend()
{

	ThruLock = FALSE;

	if (!EnterSub(UNHEND_MODE,UNHEND_MESS_EXIT,LineUnhendScan)) return(FALSE);
	MainMess(UNHEND_MESS_FIND);

	unhendstage = 0;
	unhenditem = NULL;

	return(FALSE);
}

LineUnhendC()
{

	ThruLock = TRUE;

	if (!EnterSub(UNHEND_MODE,UNHEND_MESS_EXIT,LineUnhendScan)) return(FALSE);
	MainMess(UNHEND_MESS_FIND);

	unhendstage = 0;
	unhenditem = NULL;

	return(FALSE);
}

/*
 *     +++
 *     +++
 *  +++++++++		Line Unhook
 *  +++++++++
 *     +++
 *     +++
 */


VOID FindUnHookSelect(ix,iy,typeonly)
STDFLT ix,iy;
int typeonly;
{

	D_NODE *lnklst,*kepmin1,*kepmin2;
	STDFLT curmin1=MAXBIG,curmin2=MAXBIG,rmin;
	O_LINE *hook1,*hook2;
	int 	action;
	ULONG	setflags;

	kepmin1 = NULL;
	kepmin2 = NULL;
	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
		if (typeonly!=0 && typeonly!=lnklst->node_id) continue;

		if (!SelectLegal(lnklst,FALSE,TRUE,TRUE)) continue;

		rmin = GenericDistance(lnklst,ix,iy);

		if (rmin<curmin1) {
			kepmin2 = kepmin1;
			curmin2 = curmin1;
			kepmin1 = lnklst;
			curmin1 = rmin;
			continue;
		}

		if (rmin<curmin2) {
			kepmin2 = lnklst;
			curmin2 = rmin;
		}
	}

	if (kepmin1==NULL || kepmin2==NULL) return;

	hook1 = GetLineData(kepmin1);		
	hook2 = GetLineData(kepmin2);		

	setflags = hook1->line_flags | hook2->line_flags;

	action = -1;
	if (hook1->x1==hook2->x1&&hook1->y1==hook2->y1) action = 1;
	else if (hook1->x1==hook2->x2&&hook1->y1==hook2->y2) {
		lnklst = kepmin1;
		kepmin1 = kepmin2;
		kepmin2 = lnklst;
		hook1 = GetLineData(kepmin1);		
		hook2 = GetLineData(kepmin2);		
		action=0;
	} else if (hook1->x2==hook2->x2&&hook1->y2==hook2->y2) action = 2;
	else if (hook1->x2==hook2->x1&&hook1->y2==hook2->y1) action = 0;

	if (action<0) return;

	if (action&1) {
		rmin = hook1->x1;
		hook1->x1 = hook1->x2;
		hook1->x2 = rmin;
		rmin = hook1->y1;
		hook1->y1 = hook1->y2;
		hook1->y2 = rmin;		/* Re-Order hook1 */
	}
	if (action&2) {
		rmin = hook2->x1;
		hook2->x1 = hook2->x2;
		hook2->x2 = rmin;
		rmin = hook2->y1;
		hook2->y1 = hook2->y2;
		hook2->y2 = rmin;		/* Re-Order hook2 */
	}

	kepmin1->node_flags |= NODE_DELETED;	/* Erase previous lines */
	GenericRender(kepmin1);
	kepmin1->node_flags &= (~NODE_DELETED);

	kepmin2->node_flags |= NODE_DELETED;
	GenericRender(kepmin2);
		
	hook1->x2 = hook2->x2;			/* Unhook */
	hook1->y2 = hook2->y2;

	hook1->line_flags = setflags;	/* Reconnect arrow flags */

	DeleteNode(kepmin2);
	GenericRender(kepmin1);

	return;

}

VOID LineUnHookScan(sx,sy,action,xr,yr)
WORD sx,sy,action;
STDFLT xr,yr;
{

	if (action==RBUTTONDOWN) {
			ExitSub(UNHOOK_MESS_ABORT);
			return;
	}

	switch (action) {
	case LBUTTONDOWN:
		InfoMess(UNHOOK_MODE,UNHOOK_MESS_WAIT);
		FindUnHookSelect(xr,yr,NODE_LINE);
		SExitSub(NULL);
		return;

	}
}

LineUnhook()
{

	if (!EnterSub(UNHOOK_MODE,UNHOOK_MESS_EXIT,LineUnHookScan)) return(FALSE);
	MainMess(UNHOOK_MESS_FIND);

	return(FALSE);
}

/*
 *     +++
 *     +++
 *  +++++++++		Snap to Intercept
 *  +++++++++
 *     +++
 *     +++
 */


D_NODE *interfind[2];
int intermode=0;
STDFLT interx,intery;

VOID FindInterSelect(ix,iy,typeonly,level)
STDFLT ix,iy;
int typeonly;
int level;
{

	D_NODE *lnklst,*kepmin;
	STDFLT curmin=MAXBIG,rmin;

	kepmin = NULL;
	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
		if (typeonly!=0 && typeonly!=lnklst->node_id) continue;

		if (!SelectLegal(lnklst,FALSE,TRUE,TRUE)) continue;

		rmin = GenericDistance(lnklst,ix,iy);

		if (rmin>curmin) continue;
		curmin = rmin;
		kepmin = lnklst;
	}
	
	interfind[level] = kepmin;

	return;

}

BOOL DoIntersection()
{

	STDFLT rx,ry;

	if (!CalcIntercept(
		GetLineData(interfind[0])->x1,
		GetLineData(interfind[0])->y1,
		GetLineData(interfind[0])->x2,
		GetLineData(interfind[0])->y2,
		GetLineData(interfind[1])->x1,
		GetLineData(interfind[1])->y1,
		GetLineData(interfind[1])->x2,
		GetLineData(interfind[1])->y2,
			&interx,&intery)) return(FALSE);
		
	return(TRUE);
	
}

VOID LineInterScan(sx,sy,action,xr,yr)
WORD sx,sy,action;
STDFLT xr,yr;
{

	if (action==RBUTTONDOWN) {
		ExitSide(INTER_MESS_ABORT);
		return;
	}

	if (action!=LBUTTONDOWN) return;

	switch (intermode) {
	case 0:
		InfoMess(INTER_MODE,INTER_MESS_WAIT);
		FindInterSelect(xr,yr,NODE_LINE,0);
		if (interfind[0]!=NULL) {
			intermode++;
			MainMess(INTER_MESS_FINDI);
			return;
		}
		InfoMess(INTER_MODE,INTER_MESS_BAD);
		return;
	case 1:
		FindInterSelect(xr,yr,NODE_LINE,1);
		if (interfind[0]==interfind[1]) {
			InfoMess(INTER_MODE,INTER_MESS_SAME);
			return;
		}
		if (interfind[1]!=NULL) {
			if (!DoIntersection()) ExitSide(INTER_MESS_NONE);
			else	{
				SExitSide(NULL);	/* Exit so pointeractions will work */
				PointerAction(0,0,LBUTTONDOWN,interx,intery);
			}
			return;
		}
		InfoMess(INTER_MODE,INTER_MESS_BAD);
		return;
	}
	
}

LineIntersection()
{

	if (!EnterSide(INTER_MODE,INTER_MESS_EXIT,LineInterScan)) return(FALSE);
	MainMess(INTER_MESS_FIND);
	
	interfind[0] = NULL;
	interfind[1] = NULL;
	
	intermode = NULL;
	
	return(FALSE);
}

/*
 *     +++
 *     +++
 *  +++++++++		Edit Text
 *  +++++++++
 *     +++
 *     +++
 */

LOCAL int exmod;
LOCAL struct Requester GetTextFromScreen;

VOID EditHandler(ingad)
struct Gadget *ingad;
{

	int gnum;

	if (ingad==NULL) return;
	gnum = ingad->GadgetID;

	CloseRequest(&GetTextFromScreen,w);	/* Close manually */

	switch (gnum) {
	case 40:
		exmod = 0;
		return;
	case 20:
		exmod = 1;
		return;
	case 21:
		exmod = 0;
		return;
	}
}

LOCAL struct StringInfo BoxStringSpec = 
	{ NULL, undobuf, 0, 254,0,0,0,0,0,0,NULL,0,NULL };
LOCAL struct IntuiText TextEnders[] = {
	{ 2,3,JAM2,0,0,NULL,"  Cancel  ",NULL },
	{ 2,3,JAM2,0,0,NULL,"    OK    ",NULL }};
#define TXACTIVE GADGIMMEDIATE | RELVERIFY 
LOCAL struct Gadget TextBoxGadget[] = {
 	{ &TextBoxGadget[1],5,50,340,9,GADGHCOMP,TXACTIVE,STRGADGET|REQGADGET, 
	NULL, NULL, NULL, 0, &BoxStringSpec, 40, EditHandler },
	{ &TextBoxGadget[2], 50,70,80,8,GADGHCOMP,TXACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&TextEnders[0],0,NULL,20,EditHandler},
	{ NULL, 220,70,80,8,GADGHCOMP,TXACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&TextEnders[1],0,NULL,21,EditHandler}};
LOCAL struct IntuiText BoxQuery = { 2,1,JAM2,20,20,NULL,
	"Enter Text to Draw (1-255 chars)",NULL};

VOID EdBoxReq(intext)
O_TEXT *intext;
{

	UBYTE DoText[256],*newtex;
	int i;

	sprintf(DoText,"%s",intext->text);

	InitRequester(&GetTextFromScreen);
	
	GetTextFromScreen.Width=350;
	GetTextFromScreen.Height=85;
	GetTextFromScreen.LeftEdge=(XSsize-GetTextFromScreen.Width)/2;
	GetTextFromScreen.TopEdge=(YSsize-GetTextFromScreen.Height)/2;

	GetTextFromScreen.BackFill=1;
	BoxStringSpec.Buffer = DoText;
	BoxStringSpec.BufferPos = strlen(DoText);

	GetTextFromScreen.ReqGadget = &TextBoxGadget[0];
	GetTextFromScreen.ReqText = &BoxQuery;

#ifdef _TURNSY
	GetTextFromScreen.Flags = NOISYREQ;
#endif

	SafeRequest(&GetTextFromScreen, w);

	for (i=0; i<4; i++) {
		if (ActivateGadget(&TextBoxGadget[0],w,&GetTextFromScreen)) {
			printf("Text requester activated\n");
			break;
		}
		Delay(2);
	}

	DoBorders(&GetTextFromScreen,0,0,0);

	sit_n_spin(&GetTextFromScreen,0,&TextBoxGadget[1],&TextBoxGadget[2]);	
		/* Wait for requesters to exit */

	if (exmod==0) {
		newtex = GI_DrawAlloc(strlen(DoText)+2);
		if (newtex==NULL) return;
		sprintf(newtex,"%s",DoText);
		intext->text = newtex;
	}

	Text_Box_Dirty = TRUE;
	/* text_shadow(); */

	RidSpurious();
	UpdateTitle();		/* Get the normal header back */

	return;

}

VOID DoEditItem()
{

	D_NODE *innode;
	O_LINE *drawline;
	O_ELLIP *drawellip;
	O_TEXT *drawtext;

	innode = FindItem;

	switch (innode->node_id) {

	case NODE_LINE: 	return;
 	case NODE_ELLIP: 	return;
	case NODE_TEXT: 	
		drawtext=GetTextData(innode);
		innode->node_flags &= (~NODE_SELECTED);
		innode->node_flags |= NODE_DELETED;
		GenericRender(innode);
		EdBoxReq(drawtext);
		innode->node_flags &= (~NODE_DELETED);
		innode->node_flags |= NODE_SELECTED;
		GenericRender(innode);
		return;
	}

}	
VOID EditItemScan(sx,sy,action,xr,yr)
WORD sx,sy,action;
STDFLT xr,yr;
{

	if (action==RBUTTONDOWN) {
		ExitSub(EDIT_MESS_ABORT);
		return;
	}

	switch (action) {
	case LBUTTONDOWN:
		InfoMess(EDIT_MODE,EDIT_MESS_WAIT);
		FindSelect(xr,yr,NODE_TEXT,TRUE);	/* Search Text Now */
		if (FindItem!=NULL) DoEditItem();
		SExitSub(NULL);
		return;

	}
}

BOOL EditItem()
{

	if (!EnterSub(EDIT_MODE,EDIT_MESS_EXIT,EditItemScan)) return(FALSE);
	MainMess(EDIT_MESS_FIND);

	return(FALSE);
}

/*
 *     +++
 *     +++
 *  +++++++++		Snap to Filet
 *  +++++++++
 *     +++
 *     +++
 */


D_NODE *filetfind[2];
int filetmode=0;
STDFLT filetx,filety,ffindx[2],ffindy[2],fperx[2],fpery[2];

VOID FindFiletSelect(ix,iy,typeonly,level)
STDFLT ix,iy;
int typeonly;
int level;
{

	D_NODE *lnklst,*kepmin;
	STDFLT curmin=MAXBIG,rmin;

	kepmin = NULL;
	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
		if (typeonly!=0 && typeonly!=lnklst->node_id) continue;

		if (!SelectLegal(lnklst,FALSE,TRUE,TRUE)) continue;

		rmin = GenericDistance(lnklst,ix,iy);

		if (rmin>curmin) continue;
		fperx[filetmode] = perx;
		fpery[filetmode] = pery;
		curmin = rmin;
		kepmin = lnklst;
	}
	
	filetfind[level] = kepmin;

	return;

}

BOOL neara(val,std)
STDFLT val,std;
{

	STDFLT mar,a;

	mar = std * .001;

	a = abs(val - std);
	normalize(a);
	
	if (a>=(TWOPI-.001)) return(TRUE);
	if (a<.001) return(TRUE);
	
	return(FALSE);
}

BOOL DoFilet()
{

	int i;
	STDFLT rx[2][2],ry[2][2],bx[2],by[2],sa,ea,s,lx[2],ly[2];
	STDFLT theta,phi,hyp,a,b,c,nx,ny,cl,ang[2],p1x,p2x,p1y,p2y;
	int te[2][2];
	struct	Group_Attrib	*curgrpsave;
	struct	Layer_Attrib	*curlaysave;
	struct	Pen_Attrib	*curpensave;

	if (!CalcIntercept(
		rx[0][0]=GetLineData(filetfind[0])->x1,
		ry[0][0]=GetLineData(filetfind[0])->y1,
		rx[0][1]=GetLineData(filetfind[0])->x2,
		ry[0][1]=GetLineData(filetfind[0])->y2,
		rx[1][0]=GetLineData(filetfind[1])->x1,
		ry[1][0]=GetLineData(filetfind[1])->y1,
		rx[1][1]=GetLineData(filetfind[1])->x2,
		ry[1][1]=GetLineData(filetfind[1])->y2,
			&filetx,&filety)) return(FALSE);

	a = atan2(fpery[0]-filety,fperx[0]-filetx);
	normalize(a);
	b = atan2(fpery[1]-filety,fperx[1]-filetx);
	normalize(b);
	
	theta = a-b;
	if (theta<0.0) theta= -theta;
	if (theta>PI) theta = TWOPI - theta;
			
	phi = PID2 - (theta/2.0);

	if (phi==0.0) return(FALSE);
	if (phi==PID2) return(FALSE);
	if (phi==PI) return(FALSE);
	
	hyp = filletrad / cos(phi);
	
	if (neara(a+theta,b)) cl = a + (theta/2.0);
	if (neara(b+theta,a)) cl = b + (theta/2.0);
	
	normalize(cl);

	nx = (cos(cl) * hyp) + filetx;
	ny = (sin(cl) * hyp) + filety;
			
	s = (TWOPI - (2.0 * theta)) / 4.0;
	
	c = atan2(filety-ny,filetx-nx);
	sa = c-s;
	ea = c+s;

	p1x = (cos(sa) * filletrad) + nx;
	p1y = (sin(sa) * filletrad) + ny;
	p2x = (cos(ea) * filletrad) + nx;
	p2y = (sin(ea) * filletrad) + ny;

	if (GenericDistance(filetfind[0],p1x,p1y)<
	    GenericDistance(filetfind[1],p1x,p1y)) {
	    	lx[0] = p1x;
		ly[0] = p1y;
		lx[1] = p2x;
		ly[1] = p2y;
	} else {
		lx[1] = p1x;
		ly[1] = p1y;
		lx[0] = p2x;
		ly[0] = p2y;
	}
	

	for (i=0; i<2; i++) {
		filetfind[i]->node_flags |= NODE_DELETED;
		GenericRender(filetfind[i]);
		filetfind[i]->node_flags &= (~NODE_DELETED);
	}

	for (i=0; i<2; i++) {
	
		if (hypot(rx[i][0]-lx[i],ry[i][0]-ly[i])<
		    hypot(rx[i][0]-filetx,ry[i][0]-filety)) {
	
		    	    
			GetLineData(filetfind[i])->x2 = lx[i];
			GetLineData(filetfind[i])->y2 = ly[i];
		}
		else {
			
			GetLineData(filetfind[i])->x1 = lx[i];
			GetLineData(filetfind[i])->y1 = ly[i];
			
		}
		
		GenericRender(filetfind[i]);
	}
	
	curgrpsave = Current_Group;
	curlaysave = Current_Layer;
	curpensave = Current_Pen;
	
	Current_Group = filetfind[0]->above_group;
	Current_Layer = filetfind[0]->node_layer;
	Current_Pen = filetfind[0]->pen_attach;

	CreateEllipse(nx,ny,filletrad,filletrad,0.0,sa,ea);

	Current_Group = curgrpsave;
	Current_Layer = curlaysave;
	Current_Pen = curpensave;
	
	return(TRUE);
	
}

VOID LineFiletScan(sx,sy,action,xr,yr)
WORD sx,sy,action;
STDFLT xr,yr;
{

	if (action==RBUTTONDOWN) {
		ExitSide(FILET_MESS_ABORT);
		return;
	}

	if (action!=LBUTTONDOWN) return;

	switch (filetmode) {
	case 0:
		InfoMess(FILET_MODE,FILET_MESS_WAIT);
		ffindx[0] = xr;
		ffindy[0] = yr;
		FindFiletSelect(xr,yr,NODE_LINE,0);
		if (filetfind[0]!=NULL) {
			filetmode++;
			MainMess(FILET_MESS_FINDI);
			return;
		}
		InfoMess(FILET_MODE,FILET_MESS_BAD);
		return;
	case 1:   
		ffindx[1] = xr;
		ffindy[1] = yr;
		FindFiletSelect(xr,yr,NODE_LINE,1);
		if (filetfind[0]==filetfind[1]) {
			InfoMess(FILET_MODE,FILET_MESS_SAME);
			return;
		}
		if (filetfind[1]!=NULL) {
			if (!DoFilet()) ExitSide(FILET_MESS_NONE);
			else	{
				SExitSide(NULL);
					/* Exit so pointeractions will work */
			}
			return;
		}
		InfoMess(FILET_MODE,FILET_MESS_BAD);
		return;
	}
	
}

LineFilet()
{

	if (!EnterSide(FILET_MODE,FILET_MESS_EXIT,LineFiletScan)) return(FALSE);
	MainMess(FILET_MESS_FIND);
	
	filetfind[0] = NULL;
	filetfind[1] = NULL;
	
	filetmode = NULL;
	
	return(FALSE);
}
