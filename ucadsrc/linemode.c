
/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	linemode.c - line user interface and line draw/clipping
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
 *	2	 9-May-87 SH	Rearrange all of the icons!
 *	3	24-Aug-87 SH	Add integrated message facility
 *	4	12-May-89 SH	Add orthoswitch angle and multiline
 *	5	14-Nov-89 SH	Clean up bogus arrows in line strings
 *				 Box excludes multi and closure and vice-versa
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*----------------------------------Macros-----------------------------------*/

#define TOP_USERWINDOW (YSsize-BOTTOMAREA)

#define LINE_CLASS	10

#define GAD_BOX		20
#define GAD_LARROW	21
#define GAD_RARROW	22
#define GAD_XLOCK	23
#define GAD_YLOCK	24
#define GAD_MULTI	25
#define GAD_CLOSURE	26
#define GAD_LPERP	27
#define GAD_LPAR	28
#define GAD_THRU	29
#define GAD_FREE	30

#define GAD_PERSPECT	40
#define GAD_UNDO	41

/*-----------------------------Forward References----------------------------*/

VOID cdrawline();

/*-------------------------External Symbol Definitions-----------------------*/

extern struct Image line_1u_image;
extern struct Image line_1s_image;
extern struct Image line_2u_image;
extern struct Image line_2s_image;
extern struct Image line_3u_image;
extern struct Image line_3s_image;
extern struct Image line_4u_image;
extern struct Image line_4s_image;
extern struct Image line_5u_image;
extern struct Image line_5s_image;
extern struct Image line_6u_image;
extern struct Image line_6s_image;
extern struct Image line_7u_image;
extern struct Image line_7s_image;
extern struct Image line_8u_image;
extern struct Image line_8s_image;
extern struct Image line_9u_image;
extern struct Image line_9s_image;
extern struct Image line_10u_image;
extern struct Image line_10s_image;
extern struct Image line_11u_image;
extern struct Image line_11s_image;

/*-------------------------Global Variable Definitions-----------------------*/

BOOL	linegadgetssetup=FALSE;

STDFLT	Line1X,Line1Y,Line2X,Line2Y;
BOOL	Point1Fixed;
BOOL 	FreeHandOn = FALSE;

LOCAL	STDFLT	Shadow1X,Shadow1Y,Shadow2X,Shadow2Y;

LOCAL	BOOL 	ShadowUp,XLock,YLock;
LOCAL	BOOL	BoxMode=FALSE,LArrow=FALSE,RArrow=FALSE;
LOCAL	BOOL	ShadBox,ShadLA,ShadRA;
BOOL	LineSetup=FALSE;
BOOL	domulti=FALSE,
	closure=FALSE;

BOOL 	multi_display = FALSE;

struct VertList {
	STDFLT	x,y;			/* Vertex */
	STDFLT	chamX1,chamY1,
		chamX2,chamY2;		/* Chamfer line */
	STDFLT	shadX1,shadY1,
		shadX2,shadY2;		/* Store prev chamfer for shadow */
	UBYTE	cham_type;		/* Chamfer type */
	
#define CHAM_NONE 0 		/* No Chamfer calculated */
#define CHAM_PERP 1		/* Perpendicular cham calced */
#define CHAM_FULL 2		/* Full chamfer calced */

	struct	VertList *next;		/* Linked list */
} 	
	*root_vert=NULL,
  	*tail_vert=NULL;


/*-------------------------------Code Section--------------------------------*/

/*
 *     +++
 *     +++
 *  +++++++++		Vertex Linked List Management Code
 *  +++++++++
 *     +++
 *     +++
 */


struct Remember *VertBuffer;

/* Create a new vertex buffer and link it into the list */

VOID NewVert()
{

	struct VertList *newbuff;
	
	newbuff = AllocRemember(&VertBuffer, sizeof (struct VertList), 0);
	
	if (newbuff==NULL) return;
	
	newbuff->next = NULL;
	newbuff->cham_type = 0;		/* No chamfers calculated yet */
	
	if (root_vert==NULL) root_vert = newbuff;
	else tail_vert->next = newbuff;
	
	tail_vert = newbuff;

}

/* Free the entire list of vertex buffers and clear list */

VOID FreeVertList()
{

	if (root_vert==NULL) return;
	
	FreeRemember(&VertBuffer,TRUE);
	
	root_vert = tail_vert = NULL;
	
}

/* Free list if exists, place initial vertex onto list */

VOID BeginVert(x,y)
STDFLT x,y;
{

	if (root_vert!=NULL) FreeVertList();

	NewVert();		/* Initialize this piece */
	
	tail_vert->x = x;
	tail_vert->y = y;
			
}

VOID DoChamfers();
VOID VertexUpdate();

/* Place a vertex onto list */

VOID NextVert(x,y)
STDFLT x,y;
{

	NewVert();		/* Initialize this piece */
	
	tail_vert->x = x;
	tail_vert->y = y;

	DoChamfers();

	VertexUpdate();

}

/*
 *     +++
 *     +++		Chamfer control for multiline and eventually
 *  +++++++++		for wide lines
 *  +++++++++
 *     +++
 *     +++
 */

/* Calculate a line perpendicular to the first x and y coords */

VOID CalcPerpCham(vert,x1,y1,x2,y2)
struct VertList *vert;
STDFLT x1,y1,x2,y2;
{

	STDFLT	ang;
	
	if (vert->cham_type==CHAM_PERP) return; /* Why bother? */
	
	ang = atan2(y2-y1, x2-x1);	/* Calc line Angle */
	ang += (PI/2.0);		/* Add Ninety Deg for perp */
	
	/* Make a small perp line - it doesn't matter what size it is */
	
	vert->chamX1 = (cos(ang)*10.0)+x1;
	vert->chamY1 = (sin(ang)*10.0)+y1;
	vert->chamX2 = (cos(ang)*(-10.0))+x1;
	vert->chamY2 = (sin(ang)*(-10.0))+y1;
	
	vert->shadX1 = vert->chamX1;	/* Fill out initial cases */
	vert->shadY1 = vert->chamY1;
	vert->shadX2 = vert->chamX2;
	vert->shadY2 = vert->chamY2;
	
	vert->cham_type = CHAM_PERP;	/* It's a perp now */
	
	return;
}
		
/* Calculate a chamfer for the vertex - this line is one that bisects
   the original angle */
   
VOID CalcChamfer(vert,x1,y1,x2,y2,x3,y3)
struct VertList *vert;
STDFLT x1,y1,x2,y2,x3,y3;
{

	STDFLT	ang1,ang2,tang;
	
	if (vert->cham_type==CHAM_FULL) return; /* Why bother? */
	
	ang1 = atan2(y2-y1, x2-x1);	/* Calc line Angle */
	ang2 = atan2(y3-y2, x3-x2);	/* Calc line Angle */
	
	tang = ang1 + ((ang2 - ang1)/2.0);
	tang += (PI/2.0);		/* Add Ninety Deg for perp */
	
	/* Make a small bisecting line - it doesn't matter what size it is */
	
	vert->chamX1 = (cos(tang)*10.0)+x2;
	vert->chamY1 = (sin(tang)*10.0)+y2;
	vert->chamX2 = (cos(tang)*(-10.0))+x2;
	vert->chamY2 = (sin(tang)*(-10.0))+y2;
	
	vert->cham_type = CHAM_FULL;	/* It's a full cham now */

	return;
}
		
/* Calculate chamfers for multilines
   go through list to see if proper chamfers are calculated -- if not,
   set up properly.  If no multi, skip all this. */
   
VOID DoChamfers()
{

	struct VertList *looper,*prev;

	if (!domulti) return;	/* Don't bother with this */
	
	prev = NULL;
	for (looper=root_vert; looper!=NULL; prev=looper,looper=looper->next) {
		if (prev==NULL) {
			CalcPerpCham(looper,looper->x,looper->y,
				looper->next->x,looper->next->y);
			continue;
		}
		if (looper->next==NULL) {
			CalcPerpCham(looper,looper->x,looper->y,
				prev->x,prev->y);
			continue;
		}
		CalcChamfer(looper,prev->x,prev->y,
			looper->x,looper->y,
			looper->next->x,looper->next->y);
	}
}

/* Perform closure if required -- that is, close the polygon */

VOID DoClosure()
{

	struct VertList *tend;

	if (!closure) return;	/* No closure */
	
	if (root_vert->next==tail_vert) return;	/* Only one line */
	
	tend = tail_vert;
	
	NewVert();		/* Initialize closure piece */
	
	tail_vert->x = root_vert->x;
	tail_vert->y = root_vert->y;

	DoChamfers();

	tend->shadX1 = tend->chamX1;	/* Setup to repaint */
	tend->shadY1 = tend->chamY1;
	tend->shadX2 = tend->chamX2;
	tend->shadY2 = tend->chamY2;
		
	CalcChamfer(tail_vert, tend->x,tend->y,
		root_vert->x, root_vert->y,
		root_vert->next->x, root_vert->next->y);

	tail_vert->shadX1 = tail_vert->chamX1;	
	tail_vert->shadY1 = tail_vert->chamY1;
	tail_vert->shadX2 = tail_vert->chamX2;
	tail_vert->shadY2 = tail_vert->chamY2;
	
	root_vert->shadX1 = tail_vert->chamX1;	
	root_vert->shadY1 = tail_vert->chamY1;
	root_vert->shadX2 = tail_vert->chamX2;
	root_vert->shadY2 = tail_vert->chamY2;
	
}

/*
 *     +++
 *     +++		Shadow Rendering
 *  +++++++++
 *  +++++++++
 *     +++
 *     +++
 */

/* Perform work on vertex under construction */

VOID TossShadows()
{
	GI_DrawSetup(1,1);
	v_reset();

	if (ShadowUp) {
		if (ShadBox) {
			v_move(Shadow1X,Shadow1Y);
			v_draw(Shadow2X,Shadow1Y);
			v_draw(Shadow2X,Shadow2Y);
			v_draw(Shadow1X,Shadow2Y);
			v_draw(Shadow1X,Shadow1Y);
		} else {
			v_move(Shadow1X,Shadow1Y);
			v_draw(Shadow2X,Shadow2Y);
			/*
			if (ShadLA) DoArrow(Shadow1X,Shadow1Y,Shadow2X,Shadow2Y);
			if (ShadRA) DoArrow(Shadow2X,Shadow2Y,Shadow1X,Shadow1Y);
			*/
		}

		ShadowUp=FALSE;
	}

}

/* Calculate the multilines and render out */

VOID RemVertShad(beg,last)
struct VertList *beg,*last;
{

	STDFLT x1,x2,y1,y2,ang,oang;
	STDFLT nx1,nx2,ny1,ny2,stlen,a;
	STDFLT cx1,cx2,cy1,cy2;
	int i,j;
	
	x1 = beg->x;
	y1 = beg->y;
	x2 = last->x;
	y2 = last->y;
	
	oang = atan2(y2-y1,x2-x1);	/* Calc ang of line */
	ang = oang + (PI/2.0);		/* Calc perp to this */
	
	a = multi_line-1;
	stlen = -(multi_space * a) / 2.0;	/* Start for looping */
	
	for (i=0, a=stlen; i<multi_line; i++, a+=multi_space) {
		
		nx1 = (cos(ang)*a)+x1;
		ny1 = (sin(ang)*a)+y1;
		nx2 = (cos(oang)*10.0)+nx1;
		ny2 = (sin(oang)*10.0)+ny1;

		if (!CalcIntercept(nx1,ny1,nx2,ny2,
			beg->shadX1,beg->shadY1,beg->shadX2,beg->shadY2,
			&cx1,&cy1)) return;
		if (!CalcIntercept(nx1,ny1,nx2,ny2,
			last->shadX1,last->shadY1,last->shadX2,last->shadY2,
			&cx2,&cy2)) return;
		
		v_reset();	
		v_move(cx1,cy1);
		v_draw(cx2,cy2);
	}
}

/* Calculate the multilines and create line objects */

VOID CreateVertex(beg,last,flags,which)
struct VertList *beg,*last;
int flags,which;
{

	STDFLT x1,x2,y1,y2,ang,oang;
	STDFLT nx1,nx2,ny1,ny2,stlen,a,b;
	STDFLT cx1,cx2,cy1,cy2;
	int i,j;
	
	x1 = beg->x;
	y1 = beg->y;
	x2 = last->x;
	y2 = last->y;
	
	if (!domulti) {
		CreateLine(x1,y1,x2,y2,flags);
		return;
	}
	
	oang = atan2(y2-y1,x2-x1);	/* Calc ang of line */
	ang = oang + (PI/2.0);		/* Calc perp to this */
	
	a = multi_line-1;
	stlen = -(multi_space * a) / 2.0;	/* Start for looping */
	
	b = which;
	
	a = stlen + (b * multi_space);
	
	nx1 = (cos(ang)*a)+x1;
	ny1 = (sin(ang)*a)+y1;
	nx2 = (cos(oang)*10.0)+nx1;
	ny2 = (sin(oang)*10.0)+ny1;

	if (!CalcIntercept(nx1,ny1,nx2,ny2,
		beg->shadX1,beg->shadY1,beg->shadX2,beg->shadY2,
		&cx1,&cy1)) return;
	if (!CalcIntercept(nx1,ny1,nx2,ny2,
		last->shadX1,last->shadY1,last->shadX2,last->shadY2,
		&cx2,&cy2)) return;

	CreateLine(cx1,cy1,cx2,cy2,flags);
						
}

/* Depaint the last set and repaint */

VOID RenderVertex(opitem,pitem,item)
struct VertList *opitem,*pitem,*item;
{

	if (pitem==NULL) return;		/* Huh? Shouldn't happen */

	if (!domulti) {
		v_move(pitem->x,pitem->y);
		v_draw(item->x,item->y);
		return;
	}

	if (opitem!=NULL) {

		RemVertShad(opitem,pitem);	/* Depaint this set */
	
		pitem->shadX1 = pitem->chamX1;	/* Setup to repaint */
		pitem->shadY1 = pitem->chamY1;
		pitem->shadX2 = pitem->chamX2;
		pitem->shadY2 = pitem->chamY2;

		RemVertShad(opitem,pitem);	
				/* Paint prev line with new chamfer */
	}
	
	pitem->shadX1 = pitem->chamX1;	/* Setup new item to paint */
	pitem->shadY1 = pitem->chamY1;
	pitem->shadX2 = pitem->chamX2;
	pitem->shadY2 = pitem->chamY2;
	
	RemVertShad(pitem,item);	/* Paint last with perp chamfer */
	
}

/* Add the last vertex to the screen -- put in the multilines if on */

VOID VertexUpdate()
{

	struct VertList *loop,*prev,*oprev;
	
	GI_DrawSetup(1,1);
	v_reset();
	
	prev = oprev = NULL;
	
	for (loop=root_vert; loop!=NULL; oprev=prev,prev=loop,loop=loop->next) {
	
		if (loop->next!=NULL) continue;
		RenderVertex(oprev,prev,loop);
	}
}
	
/* Unpaint the previous ghost list */

VOID EliminateVertex()
{

	struct VertList *loop,*prev;

	prev = NULL;

	for (loop=root_vert; loop!=NULL; prev=loop,loop=loop->next) {
	
		if (prev!=NULL) {
			if (!domulti) {
				v_reset();
				v_move(prev->x,prev->y);
				v_draw(loop->x,loop->y);
				return;
			} else	RemVertShad(prev,loop);
		}
	}
}

/* Paint all of the vertexes out */

VOID StoreVertexes()
{

	int fg,fg1,fg2,ln,to;
	struct VertList *loop,*prev;

	EliminateVertex();

	DoClosure();

	fg1 = fg2 = 0;
	if (LArrow) fg1 |= LINE_P1ARROW;
	if (RArrow) fg2 |= LINE_P2ARROW;
	
	to = 1;
	if (domulti) to = multi_line;

	for (ln=0; ln<to; ln++) {
		prev = NULL;
		for (loop=root_vert; loop!=NULL; prev=loop,loop=loop->next) {
			fg = 0;
			if (prev==root_vert) fg = fg1;
			if (loop==tail_vert) fg = fg2;
			if (root_vert->next==tail_vert) fg = fg1 | fg2;
			
			if (prev!=NULL) CreateVertex(prev,loop,fg,ln);
		}
	}

	FreeVertList();
			
}				

/*
 *     +++
 *     +++		Freehand Mode control
 *  +++++++++	
 *  +++++++++
 *     +++
 *     +++
 */

VOID FreeHand(ingad)
struct GI_bgadget *ingad;
{

	FreeHandOn = FreeHandFB = GI_GetBGadget(0,0,ingad);

	if (FreeHandOn) {
		BoxMode=FALSE;
		GI_SetBGadget(LINE_CLASS,GAD_BOX,NULL,FALSE);
	}
}

/*
 *     +++
 *     +++		Line Mode Gadget Control
 *  +++++++++
 *  +++++++++
 *     +++
 *     +++
 */


VOID GetRidOfLineMode()
{

	EliminateVertex();
	FreeVertList();

	TossShadows();

	GI_DeactivateBGadget(LINE_CLASS,0,NULL);	

	zapwindow();

	LineSetup = FALSE;
	FreeHandFB = FALSE;
	
}

LOCAL	BOOL	GetDThru = FALSE,
		ThruLock = FALSE,
		singul;
	
LOCAL	STDFLT	LockAngle,
		LockTan,
		Lockdx,
		Lockdy,
		LockX,
		LockY;

LOCAL 	BOOL	doperp=FALSE;

VOID DrawMulti(ingad)
struct GI_bgadget *ingad;
{

	domulti = GI_GetBGadget(0,0,ingad);

	if (root_vert!=NULL) {
		EliminateVertex();
		FreeVertList();

		Point1Fixed=FALSE;
	}
	
	if (domulti) {
		BoxMode=FALSE;
		GI_SetBGadget(LINE_CLASS,GAD_BOX,NULL,FALSE);
	}
}

VOID DrawClosure(ingad)
struct GI_bgadget *ingad;
{

	closure = GI_GetBGadget(0,0,ingad);
	if (closure) {
		BoxMode=FALSE;
		GI_SetBGadget(LINE_CLASS,GAD_BOX,NULL,FALSE);
	}

}

VOID DrawThru(ingad)
struct GI_bgadget *ingad;
{

	if (!Point1Fixed) {
		InfoMess(LINE_MODE,LINE_MESS_FIRST);
		return;
	}

	TossShadows();
	
	ThruLock = FALSE;
	GetDThru = TRUE;
	
	MainMess(LINE_MESS_THRU);

}

STDFLT SetLockAngle(angle)
STDFLT angle;
{
	
	ThruLock = TRUE;
	
	singul = FALSE;
	
	normalize(angle);
	
	if (angle!=(0.5*PI)||angle!=(1.5*PI)) LockTan = tan(angle);
	else singul = TRUE;

}	

VOID GetDrawThru(x,y)
STDFLT x,y;
{

	STDFLT dx,dy;

	if (y==Line1Y&&x==Line1X) return;

	GetDThru = FALSE;

	Lockdx = x-Line1X;
	Lockdy = y-Line1Y;

	LockAngle = atan2(Lockdy,Lockdx);
	
	SetLockAngle(LockAngle);
	
	LockX = x;
	LockY = y;

	/* printf("Draw Through angle locked to %s\n",DispAng(LockAngle)); */

	MainMess(LINE_MESS_PT2);
	
}

VOID FixDrawThru()
{
	
	STDFLT dx,dy;
	
	dx = Line2X - Line1X;
	dy = Line2Y - Line1Y;
	
	if (abs(LockTan)<0.5) {	/* More mouse resolution in x */
	
		if (dx==0.0) {
			Line2Y = Line1Y;
			return;
		}
		
		Line2Y = Line1Y + (LockTan * dx);
		return;
	}
	
	if (singul) {	/* Vertical line */
	
		Line2X = Line1X;
		return;
	}
	
	Line2X = Line1X + (dy / LockTan);
}

VOID LineSearch(xr,yr)
STDFLT xr,yr;
{

	D_NODE *pickem;
	struct line_mode *line;
	STDFLT ang;
	
	pickem = PickSelect(xr,yr,NODE_LINE);
	if (pickem==NULL) return;
	
	line = GetLineData(pickem);

	ang = atan2(line->y2-line->y1, line->x2-line->x1);
	if (doperp) ang += (PI/2.0);

	if (Draw_Info&&!FreeHandOn) {			
		sprintf(pbuffer,"  ");
		paintinfo();
	}

	SetLockAngle(ang);

}

VOID WithScan(sx,sy,action,xr,yr)
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
		LineSearch(xr,yr);	/* Search All types */
		SExitSub(NULL);
		return;

	}
}

VOID DrawPerp(ingad)
struct GI_bgadget *ingad;
{
	doperp = TRUE;

	if (!EnterSub(SEARCH_MODE,SEARCH_MESS_EXIT,WithScan)) return;
	MainMess(SEARCH_MESS_FIND);
}

VOID DrawPar(ingad)
struct GI_bgadget *ingad;
{
	doperp = FALSE;
	
	if (!EnterSub(SEARCH_MODE,SEARCH_MESS_EXIT,WithScan)) return;
	MainMess(SEARCH_MESS_FIND);
}

VOID LineBreakHandler(ingad)
struct GI_bgadget *ingad;
{

	if (root_vert!=NULL) StoreVertexes();

	TossShadows();

	if (Draw_Info&&!FreeHandOn) {
		sprintf(pbuffer,"  ");
		paintinfo();
	}
	
	MainMess(LINE_MESS_PT1);

	Point1Fixed=FALSE;

}

VOID LArrowHandler(ingad)
struct GI_bgadget *ingad;
{

	LArrow = GI_GetBGadget(0,0,ingad);
	if (LArrow) {
		GI_SetBGadget(LINE_CLASS,GAD_BOX,NULL,FALSE);
		BoxMode = FALSE;
	}
}

VOID RArrowHandler(ingad)
struct GI_bgadget *ingad;
{

	RArrow = GI_GetBGadget(0,0,ingad);
	if (RArrow) {
		GI_SetBGadget(LINE_CLASS,GAD_BOX,NULL,FALSE);
		BoxMode = FALSE;
	}

}

VOID BoxHandler(ingad)
struct GI_bgadget *ingad;
{

	BoxMode = GI_GetBGadget(0,0,ingad);
	if (BoxMode) {
		LArrow = FALSE;
		RArrow = FALSE;
		GI_SetBGadget(LINE_CLASS,GAD_LARROW,NULL,FALSE);
		GI_SetBGadget(LINE_CLASS,GAD_RARROW,NULL,FALSE);
		XLock=FALSE;
		YLock=FALSE;
		GI_SetBGadget(LINE_CLASS,GAD_XLOCK,NULL,FALSE);
		GI_SetBGadget(LINE_CLASS,GAD_YLOCK,NULL,FALSE);
		domulti = FALSE;
		closure = FALSE;
		GI_SetBGadget(LINE_CLASS,GAD_MULTI,NULL,FALSE);
		GI_SetBGadget(LINE_CLASS,GAD_CLOSURE,NULL,FALSE);
		FreeHandOn = FreeHandFB = FALSE;
		GI_SetBGadget(LINE_CLASS,GAD_FREE,NULL,FALSE);
		
	}

}

VOID XLockHandler(ingad)
struct GI_bgadget *ingad;
{

	if (!GI_GetBGadget(0,0,ingad)) {
		XLock=FALSE;
		return;
	}
	XLock=TRUE;
	YLock=FALSE;
	BoxMode=FALSE;
	GI_SetBGadget(LINE_CLASS,GAD_YLOCK,NULL,YLock);
	GI_SetBGadget(LINE_CLASS,GAD_BOX,NULL,FALSE);

}

VOID YLockHandler(ingad)
struct GI_bgadget *ingad;
{

	if (!GI_GetBGadget(0,0,ingad)) {
		YLock=FALSE;
		return;
	}
	YLock=TRUE;
	XLock=FALSE;
	BoxMode=FALSE;
	GI_SetBGadget(LINE_CLASS,GAD_XLOCK,NULL,XLock);
	GI_SetBGadget(LINE_CLASS,GAD_BOX,NULL,FALSE);

}

/*
 *     +++
 *     +++		Line Mode Setup 
 *  +++++++++
 *  +++++++++
 *     +++
 *     +++
 */


VOID Draw_Line()
{

	int bl;

	MainMode(LINE_MODE);
	MainMess(LINE_MESS_PT1);
	
	if (LineSetup) return;

	if (WrapupDrawMode!=NULL &&
	    WrapupDrawMode != GetRidOfLineMode) 
			(*WrapupDrawMode)();

	mouseaction=PerformLine;
	WrapupDrawMode=GetRidOfLineMode;

	zapwindow();

	LineSetup = TRUE;
	
	FreeHandFB = FreeHandOn;

	if (linegadgetssetup) {
		GI_ActivateBGadget(LINE_CLASS,0,NULL,w);

		ShadowUp=Point1Fixed=FALSE;
		XLock=YLock=FALSE;

		GI_SetBGadget(LINE_CLASS,GAD_XLOCK,NULL,XLock);
		GI_SetBGadget(LINE_CLASS,GAD_YLOCK,NULL,YLock);

		GI_RefreshGadgets();

		return;
	}

	bl = (YSsize - (11*28))/2;
	bl += 5;			/* Don't let it touch logo */

	GI_BGadgetImage(3,bl+(0*28),LArrowHandler,TRUE,LINE_CLASS,
		GAD_LARROW,FALSE,&line_1u_image,&line_1s_image);

	GI_BGadgetImage(3,bl+(1*28),RArrowHandler,TRUE,LINE_CLASS,
		GAD_RARROW,FALSE,&line_2u_image,&line_2s_image);

	GI_BGadgetImage(3,bl+(2*28),BoxHandler,TRUE,LINE_CLASS,
		GAD_BOX,FALSE,&line_3u_image,&line_3s_image);

	GI_BGadgetImage(3,bl+(3*28),XLockHandler,TRUE,LINE_CLASS,
		GAD_XLOCK,FALSE,&line_4u_image,&line_4s_image);

	GI_BGadgetImage(3,bl+(4*28),YLockHandler,TRUE,LINE_CLASS,
		GAD_YLOCK,FALSE,&line_5u_image,&line_5s_image);

	GI_BGadgetImage(3,bl+(5*28),DrawMulti,TRUE,LINE_CLASS,
		GAD_MULTI,FALSE,&line_6u_image,&line_6s_image);

	GI_BGadgetImage(3,bl+(6*28),DrawClosure,TRUE,LINE_CLASS,
		GAD_CLOSURE,FALSE,&line_7u_image,&line_7s_image);

	GI_BGadgetImage(3,bl+(7*28),DrawPerp,TRUE,LINE_CLASS,
		GAD_LPERP,TRUE,&line_8u_image,&line_8s_image);

	GI_BGadgetImage(3,bl+(8*28),DrawPar,TRUE,LINE_CLASS,
		GAD_LPAR,TRUE,&line_9u_image,&line_9s_image);

	GI_BGadgetImage(3,bl+(9*28),DrawThru,TRUE,LINE_CLASS,
		GAD_THRU,TRUE,&line_10u_image,&line_10s_image);

	GI_BGadgetImage(3,bl+(10*28),FreeHand,TRUE,LINE_CLASS,
		GAD_FREE,FALSE,&line_11u_image,&line_11s_image);

	GI_ActivateBGadget(LINE_CLASS,0,NULL,w);

	ShadowUp=Point1Fixed=FALSE;
	XLock=YLock=FALSE;

	GI_SetBGadget(LINE_CLASS,GAD_XLOCK,NULL,XLock);
	GI_SetBGadget(LINE_CLASS,GAD_YLOCK,NULL,YLock);

	GI_RefreshGadgets();

	linegadgetssetup=TRUE;
	
}

/*
 *     +++
 *     +++		Line Mode Mouse/Pointer Control
 *  +++++++++
 *  +++++++++
 *     +++
 *     +++
 */
			
VOID PerformLine(sx,sy,action,xr,yr)
WORD sx,sy,action;
STDFLT xr,yr;
{

	STDFLT ang,theta;

	TossShadows();

	if (FreeHandOn&&action==MOVEMENT&&Point1Fixed) action = LBUTTONDOWN;

	switch (action) {
		case MOVEMENT:
		case UMOVEMENT:
			if (!Point1Fixed) return;

			Line2X=xr;
			Line2Y=yr;
			if (YLock) SetLockAngle(orthoangle);
			if (XLock) SetLockAngle(orthoangle+(0.5*PI));

			if (ThruLock) FixDrawThru();

			theta = 0.0;
			if (Line1X!=Line2X||Line1Y!=Line2Y) {
				theta = atan2(Line2Y-Line1Y,Line2X-Line1X);
			}

			if (Draw_Info&&!FreeHandOn) {
				sprintf(pbuffer,"[%s,%s] len %s Ang %s",
					DispLin(Line2X-Line1X),
					DispLin(Line2Y-Line1Y),
				   DispLin(hypot(Line2X-Line1X,Line2Y-Line1Y)),
					DispAng(theta));

				paintinfo();
			}

			Shadow1X=Line1X;
			Shadow1Y=Line1Y;
			Shadow2X=Line2X;
			Shadow2Y=Line2Y;

			ShadBox = BoxMode;
			ShadLA = LArrow;
			ShadRA = RArrow;

			ShadowUp=TRUE;
			TossShadows();		/* Draw the shadows */
			ShadowUp=TRUE;

			return;

		case RBUTTONDOWN:
			if (GetDThru) {
				GetDThru = FALSE;
				ThruLock = FALSE;
				return;
			}
			LineBreakHandler();
			return;

		case LBUTTONDOWN:
			if (GetDThru) {
				GetDrawThru(xr,yr);
				return;
			}
			
			if (Point1Fixed) {
				Line2X=xr;
				Line2Y=yr;

				if (YLock) SetLockAngle(orthoangle);
				if (XLock) SetLockAngle(orthoangle+(0.5*PI));
				
				if (ThruLock) FixDrawThru();
				ThruLock = FALSE;

				if (BoxMode) {
					CreateLine(Line1X,Line1Y,
						Line2X,Line1Y,0);
					CreateLine(Line2X,Line1Y,
						Line2X,Line2Y,0);
					CreateLine(Line2X,Line2Y,
						Line1X,Line2Y,0);
					CreateLine(Line1X,Line2Y,
						Line1X,Line1Y,0);
					LineBreakHandler(NULL);
					return;
				}

				NextVert(Line2X,Line2Y);
				
				Line1X=Line2X;
				Line1Y=Line2Y;

				if (YLock||XLock) {
					if (Ortho_Switch) {
					YLock=XLock;
					XLock=YLock?FALSE:TRUE;
						GI_SetBGadget(LINE_CLASS,
							GAD_XLOCK,NULL,XLock);
						GI_SetBGadget(LINE_CLASS,
							GAD_YLOCK,NULL,YLock);
					} else 	LineBreakHandler(NULL);

				}

				return;

			}

			GetDThru = FALSE;
			ThruLock = FALSE;

			Line1X=xr;
			Line1Y=yr;

			BeginVert(xr,yr);

			Point1Fixed=TRUE;

			MainMess(LINE_MESS_PT2);

	}
}
