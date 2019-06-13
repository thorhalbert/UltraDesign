/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	pasteup.c - paste up main program
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
 *	2	 9-May-87 SH	Setup other 3D icon colors
 *	3	12-Jun-87 SH	Fill out rest of color defaults
 *	4	13-Oct-87 SH	Make pasteup program
 *	5	 7-Nov-87 SH	Make the paintdisplay routines
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "plotsetup.h"
#include <graphics/gfxmacros.h>

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/


#define LEFT_S	0x0001
#define RIGH_S	0x0002
#define BOTM_S	0x0004
#define TOPO_S	0x0008

STDFLT 	left,right,top,bottom;
LOCAL	WORD	flag1,flag2,cflag,rflag,mask1,mask2;
LOCAL	STDFLT	tx,ty;
LOCAL	SHORT 	sx1,sy1,sx2,sy2,ox1=-1,oy1=-1;

VOID Line_Border(l,r,t,b)
STDFLT l,r,t,b;
{

	left = l;
	right = r;
	top = t;
	bottom = b;

}

VOID v_reset()
{

	ox1 = 99999.9;
	oy1 = -99999.9;

}
	
VOID cdrawline(ix1,iy1,ix2,iy2,fg)
STDFLT ix1,iy1,ix2,iy2;
BOOL fg;
{


	clip_sret = WIND_IN;

	flag1=0;
	if (ix1 < left)			flag1 |= LEFT_S;
	else	if (ix1 > right)	flag1 |= RIGH_S;
	if (iy1 < bottom)		flag1 |= BOTM_S;
	else	if (iy1 > top)		flag1 |= TOPO_S;

	flag2=0;
	if (ix2 < left)			flag2 |= LEFT_S;
	else	if (ix2 > right)	flag2 |= RIGH_S;
	if (iy2 < bottom)		flag2 |= BOTM_S;
	else	if (iy2 > top)		flag2 |= TOPO_S;

	if (flag1&flag2) {
		clip_sret=WIND_OFF;
		return;		/* Lines off screen */
	}		

	mask1=mask2=0;

	while (flag1|flag2) {
	
		if (flag1&flag2) {
			clip_sret=WIND_OFF;
			return;		/* Lines off screen */
		}		

		if (flag1==0) 	cflag = flag2;
		else 		cflag = flag1;
	
		if (cflag & LEFT_S) {
			ty = iy1+((iy2-iy1)*(left-ix1)/(ix2-ix1));
			tx = left;
			rflag = LEFT_S;
		} else
		if (cflag & RIGH_S) {
			ty = iy1+((iy2-iy1)*(right-ix1)/(ix2-ix1));
			tx = right;
			rflag = RIGH_S;
		} else
		if (cflag & BOTM_S) {
			tx = ix1+((ix2-ix1)*(bottom-iy1)/(iy2-iy1));
			ty = bottom;
			rflag = BOTM_S;
		} else
		if (cflag & TOPO_S) {
			tx = ix1+((ix2-ix1)*(top-iy1)/(iy2-iy1));
			ty = top;
			rflag = TOPO_S;
		}

		clip_sret = WIND_CLIP;
	
		if (cflag==flag1) {	
			ix1 = tx;
			iy1 = ty;
			mask1 |= rflag;

			flag1=0;
			if (ix1 < left)			flag1 |= LEFT_S;
			else	if (ix1 > right)	flag1 |= RIGH_S;
			if (iy1 < bottom)		flag1 |= BOTM_S;
			else	if (iy1 > top)		flag1 |= TOPO_S;

			flag1 &= ~mask1;
		} else {	
			ix2 = tx;
			iy2 = ty;
			mask2 |= rflag;

			flag2=0;
			if (ix2 < left)			flag2 |= LEFT_S;
			else	if (ix2 > right)	flag2 |= RIGH_S;
			if (iy2 < bottom)		flag2 |= BOTM_S;
			else	if (iy2 > top)		flag2 |= TOPO_S;

			flag2 &= ~mask2;	
		}
	}

	if (fg) {
		sx1 = ((ix1-xviewbegin)/xpixsize);
		sy1 = ((iy1-yviewbegin)/ypixsize);
		if (sx1!=ox1||sy1!=oy1) 
			Move(rp,XBegin+sx1,YBegin+YWidth-sy1);
		ox1 = ((ix2-xviewbegin)/xpixsize);
		oy1 = ((iy2-yviewbegin)/ypixsize);
		Draw(rp,XBegin+ox1,YBegin+YWidth-oy1);
	}

	return;

}

VOID view_setup()
{

	xpixsize = xviewsize/((STDFLT) XWidth);
	ypixsize = xpixsize * aspect;

	yviewsize = ypixsize * ((STDFLT) YWidth);
}

fullzoomout()
{

	STDFLT	borderx,centerx,bordery,centery,scaler;
	STDFLT	screenx,screeny;

	borderx=SheetMaxIX-SheetMinIX;
	centerx=SheetMinIX+(borderx/2.0);

	bordery=SheetMaxIY-SheetMinIY;
	centery=SheetMinIY+(bordery/2.0);

	screenx=XWidth;
	screeny=YWidth;
	screeny=screeny*aspect;

	scaler=screenx/screeny;

	if ((borderx/bordery)<scaler) {		/* Narrower than screen */
		borderx=bordery*scaler;
	} else {
		bordery=borderx/scaler;
	}

	borderx*=1.05;
	bordery*=1.05;

	xviewbegin=centerx-(borderx/2.0);
	yviewbegin=centery-(bordery/2.0);

	xviewsize=borderx;
	yviewsize=bordery;

	view_setup();

}

STDFLT shad_tcos,shad_tsin,shad_hx,shad_hy,shad_xp,shad_yp,shad_x,shad_y;
BOOL rendup;

VOID shadview(viewptr)
struct ViewContext *viewptr;
{
	STDFLT tcos,tsin,hx,hy,xp,yp,x,y;
	
	if (!rendup) return;

	tcos = shad_tcos;
	tsin = shad_tsin;

	hx = shad_hx;
	hy = shad_hy;

	xp=shad_xp;
	yp=shad_yp;
	
	SetDrMd(rp, JAM1 | COMPLEMENT);
	SetAPen(rp,4);

	v_reset();

	x=0.0; y=0.0;	v_move(x*tcos-y*tsin+hx,x*tsin+y*tcos+hy);
	x=xp; y=0.0;	v_draw(x*tcos-y*tsin+hx,x*tsin+y*tcos+hy);
	x=xp; y=yp;	v_draw(x*tcos-y*tsin+hx,x*tsin+y*tcos+hy);
	x=0.0; y=yp;	v_draw(x*tcos-y*tsin+hx,x*tsin+y*tcos+hy);
	x=0.0; y=0.0;	v_draw(x*tcos-y*tsin+hx,x*tsin+y*tcos+hy);

	x=xp/2.0; y=yp;	v_draw(x*tcos-y*tsin+hx,x*tsin+y*tcos+hy);
	x=xp; y=0.0;	v_draw(x*tcos-y*tsin+hx,x*tsin+y*tcos+hy);
	
	rendup = FALSE;
}

VOID rendview(viewptr)
struct ViewContext *viewptr;
{

	shadview();
	shad_tcos = viewptr->RotCos;
	shad_tsin = viewptr->RotSin;

	shad_hx = viewptr->XNewOrigin;
	shad_hy = viewptr->YNewOrigin;

	shad_xp=(viewptr->XViewMAX - viewptr->XViewMIN) * viewptr->ViewScale;
	shad_yp=(viewptr->YViewMAX - viewptr->YViewMIN) * viewptr->ViewScale;
	
	rendup=TRUE;
	shadview();
	rendup=TRUE;
}

drawview(viewptr)
struct ViewContext *viewptr;
{
	STDFLT tcos,tsin,hx,hy,xp,yp,x,y;

	tcos = cos(viewptr->ViewRot);
	tsin = sin(viewptr->ViewRot);

	viewptr->RotCos = tcos;
	viewptr->RotSin = tsin;

	SetDrMd(rp, JAM1);
	if (viewptr==Current_View) SetAPen(rp,4);
	else	SetAPen(rp,5);

	hx = viewptr->XNewOrigin;
	hy = viewptr->YNewOrigin;

	xp=(viewptr->XViewMAX - viewptr->XViewMIN) * viewptr->ViewScale;
	yp=(viewptr->YViewMAX - viewptr->YViewMIN) * viewptr->ViewScale;

	v_reset();

	x=0.0; y=0.0;	v_move(x*tcos-y*tsin+hx,x*tsin+y*tcos+hy);
	x=xp; y=0.0;	v_draw(x*tcos-y*tsin+hx,x*tsin+y*tcos+hy);
	x=xp; y=yp;	v_draw(x*tcos-y*tsin+hx,x*tsin+y*tcos+hy);
	x=0.0; y=yp;	v_draw(x*tcos-y*tsin+hx,x*tsin+y*tcos+hy);
	x=0.0; y=0.0;	v_draw(x*tcos-y*tsin+hx,x*tsin+y*tcos+hy);

/*	x=0.0; y=0.0;	v_move(x*tcos-y*tsin+hx,x*tsin+y*tcos+hy); */
	x=xp/2.0; y=yp;	v_draw(x*tcos-y*tsin+hx,x*tsin+y*tcos+hy);
	x=xp; y=0.0;	v_draw(x*tcos-y*tsin+hx,x*tsin+y*tcos+hy);

	/* Find all trapezoidal boundaries */

	x=0.0; y=0.0; viewptr->XtrapMIN=x*tcos-y*tsin+hx;
	x=0.0; y=yp;  viewptr->XtrapMIN=min(viewptr->XtrapMIN,x*tcos-y*tsin+hx);
	x=xp; y=yp;   viewptr->XtrapMIN=min(viewptr->XtrapMIN,x*tcos-y*tsin+hx);
	x=xp; y=0.0;  viewptr->XtrapMIN=min(viewptr->XtrapMIN,x*tcos-y*tsin+hx);

	x=0.0; y=0.0; viewptr->YtrapMIN=x*tsin+y*tcos+hy;
	x=0.0; y=yp;  viewptr->YtrapMIN=min(viewptr->YtrapMIN,x*tsin+y*tcos+hy);
	x=xp; y=yp;   viewptr->YtrapMIN=min(viewptr->YtrapMIN,x*tsin+y*tcos+hy);
	x=xp; y=0.0;  viewptr->YtrapMIN=min(viewptr->YtrapMIN,x*tsin+y*tcos+hy);
 
	x=0.0; y=0.0; viewptr->XtrapMAX=x*tcos-y*tsin+hx;
	x=0.0; y=yp;  viewptr->XtrapMAX=max(viewptr->XtrapMAX,x*tcos-y*tsin+hx);
	x=xp; y=yp;   viewptr->XtrapMAX=max(viewptr->XtrapMAX,x*tcos-y*tsin+hx);
	x=xp; y=0.0;  viewptr->XtrapMAX=max(viewptr->XtrapMAX,x*tcos-y*tsin+hx);

	x=0.0; y=0.0; viewptr->YtrapMAX=x*tsin+y*tcos+hy;
	x=0.0; y=yp;  viewptr->YtrapMAX=max(viewptr->YtrapMAX,x*tsin+y*tcos+hy);
	x=xp; y=yp;   viewptr->YtrapMAX=max(viewptr->YtrapMAX,x*tsin+y*tcos+hy);
	x=xp; y=0.0;  viewptr->YtrapMAX=max(viewptr->YtrapMAX,x*tsin+y*tcos+hy);

}

VOID (*mouseaction)();
VOID Update_Disp()
{

	STDFLT xsiz,ysiz;
	int tx,ty;
	struct ViewContext *looper;
	
	rendup = FALSE;
	
	remshadows();

	GI_RectFill(0,XBegin,YBegin,XBegin+XWidth,YBegin+YWidth);

	if (DriverSegHook==0) return;

	SheetMinIX = MinIX;
	SheetMinIY = MinIY;

	xsiz = MaxIX-MinIX;
	ysiz = MaxIY-MinIY;

	SheetMaxIX = (xsiz * ((STDFLT) PosterX)) + MinIX;
	SheetMaxIY = (ysiz * ((STDFLT) PosterY)) + MinIY;

	fullzoomout();

	Line_Border(xviewbegin,xviewbegin+xviewsize,
		    yviewbegin+yviewsize,yviewbegin);

	sx1 = ((SheetMinIX-xviewbegin)/xpixsize);
	sy1 = ((SheetMinIY-yviewbegin)/ypixsize);

	sx2 = ((SheetMaxIX-xviewbegin)/xpixsize);
	sy2 = ((SheetMaxIY-yviewbegin)/ypixsize);

	GI_RectFill(6,sx1+XBegin,YBegin+YWidth-sy2,
		      sx2+XBegin,YBegin+YWidth-sy1);

	SetDrMd(rp, JAM1);
	SetAPen(rp, 2);

	for (tx=0; tx<=PosterX; tx++) {
		v_reset();
		v_move(xsiz*((STDFLT) tx)+SheetMinIX,SheetMinIY);
		v_draw(xsiz*((STDFLT) tx)+SheetMinIX,SheetMaxIY);
	}

	for (ty=0; ty<=PosterY; ty++) {
		v_reset();
		v_move(SheetMinIX,ysiz*((STDFLT) ty)+SheetMinIY);
		v_draw(SheetMaxIX,ysiz*((STDFLT) ty)+SheetMinIY);
	}

	for (looper=Root_View; looper!=NULL; looper=looper->next) 
		if (looper!=Current_View) 
			drawview(looper);

	if (mouseaction==NULL) drawview(Current_View);
	else	rendview(Current_View);
}

#ifdef LATTICEEXCEPT
matherr(in)
struct exception *in;
{

	printf("Received some math error %d %s(%e,%e)=%e\n",in->type,in->name,
		in->arg1,in->arg2,in->retval);

	return(0);
}

double except(type,name,arg1,arg2,retval)
int type;
char *name;
double arg1,arg2,retval;
{

	printf("Got error %d in %s(%e,%e)=%e\n",type,name,arg1,arg2,retval);
	return(retval);
}
#endif
