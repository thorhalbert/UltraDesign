/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	renderers - line drawing algorithms
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
 *	1	 5-Nov-87 SH	New Composition
 *	2	29-Nov-87 SH	Use amiga ellipse drawer if we can 
 *	3	 6-Jan-89 SH	Move polygon renderers over here
 *	4	 3-Mar-90 SH	Ellipse Degree convert tends to overflow
 *				 FFP value when doing calcs -- since
 *				 its the ratio and not the actual radius
 *				 that is required, we will calculate a
 *				 ratio instead
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

VOID setbits();

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

LOCAL UWORD *dispbuffer[8];
LOCAL UWORD wordcol;
LOCAL UBYTE drawpen,drawmode;

LOCAL STDFLT right,top,xoset,yoset;

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(a) ((a)<0?(-(a)):(a))

/*-------------------------------Code Section--------------------------------*/

#define COHEN_SUTHERLAND
/* #define LIANG_BARSKY */
#define LEFT_S	0x0001
#define RIGH_S	0x0002
#define BOTM_S	0x0004
#define TOPO_S	0x0008

LOCAL	SHORT 	sx1,sy1,ox1= -1,oy1= -1;

VOID Line_Normal()
{

	xoset = xviewbegin;
	yoset = yviewbegin;
	top = yviewsize;
	right = xviewsize;

}

VOID Line_Border(l,r,t,b)
STDFLT l,r,t,b;
{

	yoset = b;
	xoset = l;
	right = r-l;
	top = t-b;

}

VOID v_reset()
{

	ox1 = -1;
	oy1 = -1;

}


#ifdef COHEN_SUTHERLAND
/* The Cohen-Sutherland line clipping algorithm with some extra code to 
   keep points which were already considered causing a loop due to minor
   rounding errors */


VOID cdrawline(ix1,iy1,ix2,iy2,fg)
STDFLT ix1,iy1,ix2,iy2;
BOOL fg;
{

	register 	UBYTE	flag1,flag2;
	register	UBYTE	cflag,rflag,mask1,mask2;
	register	STDFLT	tx,ty;

	clip_sret = WIND_IN;

	flag1=flag2=mask1=mask2=0;

	ix1 -= xoset;
	iy1 -= yoset;
	ix2 -= xoset;
	iy2 -= yoset;

	if (ix1 < 0.0)			flag1 |= LEFT_S;
	else	if (ix1 > right)	flag1 |= RIGH_S;
	if (iy1 < 0.0)			flag1 |= BOTM_S;
	else	if (iy1 > top)		flag1 |= TOPO_S;

	if (ix2 < 0.0)			flag2 |= LEFT_S;
	else	if (ix2 > right)	flag2 |= RIGH_S;
	if (iy2 < 0.0)			flag2 |= BOTM_S;
	else	if (iy2 > top)		flag2 |= TOPO_S;

	if (flag1&flag2) {
		clip_sret=WIND_OFF;
		return;		/* Lines off screen */
	}		

	while (flag1|flag2) {
	
		if (flag1&flag2) {
			clip_sret=WIND_OFF;
			return;		/* Lines off screen */
		}		

		cflag = flag2?flag2:flag1;

		if (cflag & LEFT_S) {
			ty = iy1+((iy2-iy1)*(-ix1)/(ix2-ix1));
			tx = 0.0;
			rflag = LEFT_S;
		} else
		if (cflag & RIGH_S) {
			ty = iy1+((iy2-iy1)*(right-ix1)/(ix2-ix1));
			tx = right;
			rflag = RIGH_S;
		} else
		if (cflag & BOTM_S) {
			tx = ix1+((ix2-ix1)*(-iy1)/(iy2-iy1));
			ty = 0.0;
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
			if (ix1 < 0.0)			flag1 |= LEFT_S;
			else	if (ix1 > right)	flag1 |= RIGH_S;
			if (iy1 < 0.0)			flag1 |= BOTM_S;
			else	if (iy1 > top)		flag1 |= TOPO_S;

			flag1 &= ~mask1;
		} else {	
			ix2 = tx;
			iy2 = ty;
			mask2 |= rflag;

			flag2=0;
			if (ix2 < 0.0)			flag2 |= LEFT_S;
			else	if (ix2 > right)	flag2 |= RIGH_S;
			if (iy2 < 0.0)			flag2 |= BOTM_S;
			else	if (iy2 > top)		flag2 |= TOPO_S;

			flag2 &= ~mask2;
		}
	}

	if (fg) {
	 	sx1 = ix1/xpixsize;
	  	sy1 = iy1/ypixsize;
	  	if (sx1!=ox1||sy1!=oy1)
		  	Move(rp,XBegin+sx1,YBegin+YWidth-sy1);
	  	ox1 = ix2/xpixsize;
	  	oy1 = iy2/ypixsize;
	  	Draw(rp,XBegin+ox1,YBegin+YWidth-oy1);
	}

	return;

}
#endif

/*------------------------Text Renderers------------------------------*/

extern BOOL PaintFullScreen;

VOID PutText(curtext,draw)
struct drawing_node *curtext;
BOOL draw;
{

	register	WORD pen,tx,ty,l;
	STDFLT	rx,ry;
	STDFLT	_sclv,_sclh,_rno,_trno;
	UBYTE 	*text;
	STDFLT 	hx,hy,ang;
	UWORD 	flipflags;
	UBYTE 	*inc;
	STDFLT 	*sizar;
	STDFLT 	x,y,ll,lr,xr[4],yr[4];
	STDFLT 	slant,height,width,tcos,tsin,csz,asp;
	WORD	*off;
	WORD	*strx;
	int	cr,o,i;
	UBYTE	j;
	int	handle;
	WORD 	clipstat;
	BOOL 	backwards;
	STYLE_A	*instyle;
	struct 	text_mode 	*drawtext;
	BOOL	drawbox;

	drawtext = GetTextData(curtext);

	instyle=drawtext->letstyle;
	text=drawtext->text;
	hx=drawtext->x1;
	hy=drawtext->y1;
	ang=drawtext->ang;
	flipflags=drawtext->Text_Bits;
	handle=drawtext->Handle_Flag;
	asp = instyle->Aspect;

	clipstat = 0;
	
	Setup_Render(curtext);
	v_reset();

	slant = instyle->Slant;
	height = instyle->Prime_Height;

	tcos = cos(ang * .01745329);
	tsin = sin(ang * .01745329);

	inc = text;

	width = 0.0;

	sizar = instyle->draw_font->font_link->Char_Widths;

	while (*inc) {
		
		width += sizar[(int) *inc];
		inc++;
	}

	width *= height * asp;

	switch(handle) {

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

	drawbox = FALSE;
	if (height<(qtextlim*xpixsize)) drawbox=TRUE;
	if (PaintFullScreen) drawbox = FALSE;
	if (!draw) drawbox = FALSE;

	clipstat = 0;

	y = 0.0;
	x = ll + (slant*y);
	
	xr[0] = x*tcos-y*tsin+hx;
	yr[0] = x*tsin+y*tcos+hy;

	y = height;
	x = ll + (slant*y);

	xr[1] = x*tcos-y*tsin+hx;
	yr[1] = x*tsin+y*tcos+hy;

	y = height;
	x = lr + (slant*y);
	
	xr[2] = x*tcos-y*tsin+hx;
	yr[2] = x*tsin+y*tcos+hy;

	y = 0.0;
	x = lr + (slant*y);
	
	xr[3] = x*tcos-y*tsin+hx;
	yr[3] = x*tsin+y*tcos+hy;

	v_reset();
	v_move(xr[0],yr[0]);
	for (i=1; i<4; i++) {
		if (drawbox) v_draw(xr[i],yr[i]);
		             v_drawn(xr[i],yr[i]);
		clipstat |= clip_sret;
	}
	if (drawbox) v_draw(xr[0],yr[0]);
	             v_drawn(xr[0],yr[0]);

	clipstat |= clip_sret;

	x = (xr[1]+xr[2])/2.0;
	y = (yr[1]+yr[2])/2.0;
	
	if (drawbox) {
		v_draw(x,y);
		v_draw(xr[3],yr[3]);
	} else {
		v_drawn(x,y);
		v_drawn(xr[3],yr[3]);
	}
	
	drawbox=FALSE;
	if (draw) drawbox=TRUE;
	if (height<(qtextlim*xpixsize)) drawbox=FALSE;
	if (PaintFullScreen) drawbox = TRUE;
	if (clipstat==WIND_OFF) drawbox=FALSE;
	if (drawbox) {
    
		if (flipflags&TEXT_CHRFL&&flipflags&TEXT_FLIPX) {
			flipflags &= ~(TEXT_CHRFL|TEXT_FLIPX);
			backwards=TRUE;
		} else 	backwards=FALSE;
		if (flipflags&(TEXT_FLIPX|TEXT_CHRFL)) slant = -slant;

		y = 0.0;
		x = ll + (slant*y);
	
		if (flipflags&TEXT_FLIPY) y = height;
		if (flipflags&TEXT_FLIPX) x = lr*asp;

		v_move(x*tcos-y*tsin+hx,
		       x*tsin+y*tcos+hy);

		_rno = instyle->draw_font->font_link->Font_Width * height;
		_sclh = instyle->draw_font->font_link->Font_Horiz * height;
		_sclv = instyle->draw_font->font_link->Font_Vertical * height;

		off = instyle->draw_font->font_link->Char_Xtable;
		strx = instyle->draw_font->font_link->Word_Table;

		if (backwards) inc = &text[strlen(text)-1];
		else	inc = text;

		for (cr=0; cr<strlen(text); cr++) {

			j = *inc;
			if (!backwards) inc++;
			else		inc--;

			if (flipflags&TEXT_CHRFL) {
				csz = sizar[j];
				csz *= height;
				csz /= instyle->Aspect;

				rx = csz*asp;
				ry = 0.0;
				v_move(rx*tcos-ry*tsin+lastx,
				       rx*tsin+ry*tcos+lasty);
			}	

			l = off[j]-1;
			o = strx[l];
			pen = -o;

			for (;;pen=strx[++l]) {

				if (pen<=0) break;

				ty = pen & 0x7F;
				pen>>=7;

				tx = pen & 0x7F;
				pen>>=7;

				pen=pen;

				ry = _sclv * ((STDFLT) (ty-64));
				if (flipflags&TEXT_FLIPY) ry = -ry;
				rx = _sclh * ((STDFLT) (tx-64)) + slant*ry;
				if (flipflags&(TEXT_FLIPX|TEXT_CHRFL)) rx = -rx;
				rx *= asp;

				clip_sret = WIND_IN;

				if ((3-pen)==3)		
					v_move(rx*tcos-ry*tsin+lastx,
				       		rx*tsin+ry*tcos+lasty);
				else	v_draw(rx*tcos-ry*tsin+lastx,
				       		rx*tsin+ry*tcos+lasty);
				if (pen==1) clipstat |= clip_sret;
			
			}

			_trno = _rno * asp;
			if (flipflags&(TEXT_FLIPX|TEXT_CHRFL)) _trno = - _trno;
	
			v_move(_trno*tcos+lastx,_trno*tsin+lasty);
	
			if (flipflags&TEXT_CHRFL) {
				rx = csz*asp;
				ry = 0.0;
				v_move(rx*tcos-ry*tsin+lastx,
	 			       rx*tsin+ry*tcos+lasty);
			}
		}
	}

	clip_sret = clipstat;

	if (draw) {		/* Only update if actually drawing full */

		curtext->node_flags &= (~(NODE_ONSCREEN|NODE_FULLWINDOW));
		if (clipstat==WIND_OFF) return;

		curtext->node_flags |= NODE_ONSCREEN;
		if (clipstat!=WIND_CLIP) curtext->node_flags |= NODE_FULLWINDOW;
	}

	return;

}

#ifdef LIANG_BARSKY

/* The Liang-Barsky Line clipping algorithm */

STDFLT deltax,deltay,t0,t1;

BOOL clipt(p,q)
STDFLT p,q;
{

	STDFLT r;
	BOOL accept;

	accept = TRUE;

	if (p<0.0) {
		r = q/p;
		if (r>t1) accept = FALSE;
		else if (r>t0) t0 = r;
	} else if (p>0.0) {
		r = q/p;
		if (r<t0) accept = FALSE;
		else if (r<t1) t1 = r;
	} else if (q<0.0) accept = FALSE;
	return(accept);
}

SUBDEF(VOID,cdrawline) 
ARGDEF(	ARGS(STDFLT,x0)
	ARGS(STDFLT,y0)
	ARGS(STDFLT,x1)
	ARGS(STDFLT,y1)
	LARG(BOOL,fg) )
SUBEND	
{

	t0 = 0.0;
	t1 = 1.0;

	clip_sret = WIND_OFF;

	deltax = x1 - x0;
	if (clipt(-deltax, x0 - 0.0)) 
		if (clipt(deltax,right - x0)) {
			deltay = y1 - y0;
			if (clipt(-deltay,y0 - 0.0))
				if (clipt(deltay,top - y0)) {
					clip_sret = WIND_IN;
					if (t1<1.0) {
						x1 = x0 + t1 * deltax;
						y1 = y0 + t1 * deltay;
						clip_sret = WIND_CLIP;
					}
					if (t0>0.0) {
						x0 = x0 + t0 * deltax;
						y0 = y0 + t0 * deltay;
						clip_sret = WIND_CLIP;
					}

					if (fg) {
					  sx1 = ((x0-xviewbegin)/xpixsize);
					  sy1 = ((y0-yviewbegin)/ypixsize);
					  if (sx1!=ox1||sy1!=oy1) 
					  Move(rp,XBegin+sx1,YBegin+YWidth-sy1);
			 		  ox1 = ((x1-xviewbegin)/xpixsize);
					  oy1 = ((y1-yviewbegin)/ypixsize);
					  Draw(rp,XBegin+ox1,YBegin+YWidth-oy1);
					}

				}
		}

	return;
}
#endif

/*---------------------Ellipse Clipper Code-----------------------*/

STDFLT degcvt(theta)
STDFLT theta;
{
	
	theta = theta * 180.0;
	theta /= PI;
	if (theta<0) theta+=360.0;

	return(theta);
}

BOOL near(val,std)
STDFLT val,std;
{

	STDFLT mar;
	
	mar = std * .001;

	if (val<(std-mar)) return(FALSE);
	if (val>(std+mar)) return(FALSE);

	return(TRUE);
}

STDFLT ConvEllipNotation(penng,radiusx,radiusy)
STDFLT penng,radiusx,radiusy;
{

	STDFLT inslope,PID34,inter,denom;

	/* We don't need real radii, since the equation will overflow FFP */
	
	inslope = radiusx;
	radiusx = radiusx/inslope;
	radiusy = radiusy/inslope;	/* Make nice ratio */

	PID34=PI;
	PID34+=PID2;

	if (near(penng,PID2)) return(PID2);
	if (near(penng,PID34)) return(PID34);

	if (radiusx==0.0||radiusy==0.0) return(penng);

	inslope=tan(penng);

	denom = (radiusy*radiusy)+(radiusx*radiusx*inslope*inslope);
	
	inter=sqrt((radiusx*radiusx*radiusy*radiusy)/denom)/radiusx;

	if (inter>1.0) inter = 1.0;
	if (inter<(-1.0)) inter = -1.0;

	if (penng<=PID2) return(acos(inter));
	if (penng<=PI) return(acos(-inter));
	if (penng<=PID34) return(TWOPI-acos(-inter));
	return(TWOPI-acos(inter));
		
}

VOID DrawScreenEllipse(centx,centy,radx,rady)
STDFLT centx,centy,radx,rady;
{

	SHORT cx1,cy1,rdx,rdy;

	cx1 = ((centx-xviewbegin)/xpixsize);
	cy1 = ((centy-yviewbegin)/ypixsize);
	rdx = radx / xpixsize;
	rdy = rady / ypixsize;

	DrawEllipse(rp, XBegin+cx1,YBegin+YWidth-cy1,rdx,rdy);

}

#ifdef DAMNFAST
SUBDEF(VOID,DamnFastEllipses)
ARGDEF(	ARGS(int,px)
	ARGS(int,py)
	ARGS(int,rx)
	LARG(int,ry) )
SUBEND
{

	FAST WORD tx,ty,i;
	WORD fxp,fyp,fxm,fym;
	int d1;
	int t1,t2,t3,t4,t5,t6,t7;

	if (rx==0||ry==0) return;

	tx = rx;
	ty = 0;
	
	t1 = rx * rx;	t2 = t1 << 1;
	t3 = ry * ry;	t4 = t3 << 1;
	t5 = rx * t3;	t6 = t5 << 1;
	t7 = 0;		d1 = ((t1 + t3)>>2) - t5;

	for (i=0; i<= rx+ry; i++) {
		fxp = px+tx;
		fyp = py+ty;
		fxm = px-tx;
		fym = py-ty;
		setbits(fxp,fyp);
		setbits(fxm,fyp);
		setbits(fxp,fym);
		setbits(fxm,fym);

		if (d1<0) {
			ty++;
			t7 += t2;
			d1 += t7;
		} else {
			tx--;
			t6 -= t4;
			d1 -= t6;
		}
	}

}
#endif

BOOL pacman=FALSE;

VOID FancyEllipseClipper(centx,centy,radx,rady,bega,enda,rotation,fg)
STDFLT centx,centy,radx,rady,bega,enda,rotation;
BOOL fg;
{

	STDFLT xn,yn,xn1,yn1,tx1,ty1;
	FAST	WORD qp,ctr,i;
	STDFLT sinth,costh,theta;
	STDFLT csx,rotcos,rotsin,thet1;
	STDFLT deltang;

	EllipseClip = WIND_OFF;

	if (radx<=0.0) return;
	if (rady<=0.0) return;		/* Don't let this happen to you! */

	csx = max(radx,rady);		/* Simple implemention */
	if (centx+csx<xoset) return;
	if (centx-csx>xoset+right) return;
	if (centy+csx<yoset) return;
	if (centy-csx>yoset+top) return;

	v_reset();

 	if (Center_On&&fg) {
		csx = xpixsize * 2.0;
		v_move(centx-csx,centy);
		v_draw(centx+csx,centy);
		v_move(centx,centy-csx);
		v_draw(centx,centy+csx);
	}

	deltang=enda-bega;
	while (deltang<0.02) deltang+=TWOPI;
	if (deltang>TWOPI) deltang=TWOPI;
	
	if (pacman&&deltang>(TWOPI-.01)) pacman = FALSE;

	if (radx==rady&&deltang>=(TWOPI-.01)) {		/* Its a circle */
		bega = 0.0;
		enda = 0.0;
		rotation = 0.0;		
	}

#ifdef USEELLIP

	if (deltang>=(TWOPI-.01)&&abs(rotation)<=0.01&&
		centx-radx>=0.0&&
		centx+radx<=right&&
		centy-rady>=0.0&&
		centy+rady<=top) {

			EllipseClip = WIND_IN;
			if (fg) DrawScreenEllipse(centx,centy,radx,rady);
			return;		/* All on screen */
	}
#endif /* USEELLIP */

	if (abs(rotation)<=0.01) {	/* Not rotated */
		rotcos = 1.0;
		rotsin = 0.0;
	} else {
		rotcos = cos(rotation);
		rotsin = sin(rotation);
	}

	if (deltang<(TWOPI-.01)) {		/* Drawing an arc, recalc */

		bega -= rotation;
		enda -= rotation;
		if (bega<0.0) bega+=TWOPI;
		if (enda<0.0) enda+=TWOPI;

		bega=ConvEllipNotation(bega,radx,rady);
		
		enda=ConvEllipNotation(enda,radx,rady);

		deltang=enda-bega;
		while (deltang<0.02) deltang+=TWOPI;
		if (deltang>TWOPI) deltang=TWOPI;
	}

	xn = (deltang * max(radx,rady)) / ypixsize;
	
	qp=xn;

	if (qp>8000)		ctr = 110;
	else if (qp>3000)	ctr = 85;
	else if (qp>2000)	ctr = 75;
	else if (qp>1000)	ctr = 70;
	else if (qp>500)	ctr = 50;
	else if (qp>100)	ctr = 30;
	else if (qp>50)		ctr = 10;
	else if (qp>20)		ctr = 8;
	else if (qp>10)		ctr = 5;
	else ctr=4;

	qp = ctr;
	xn=qp;
	
	theta=deltang/xn;

	sinth=sin(theta);
	costh=cos(theta);

	thet1 = bega;
	xn=cos(thet1)*radx;		/* Calc start */
	yn=sin(thet1)*rady;
	tx1 = ((xn * rotcos) - (yn * rotsin)) + centx; /* Rot ellipse */
	ty1 = ((yn * rotcos) + (xn * rotsin)) + centy;

	EllipseClip=0;

	if (pacman&&fg) {
		v_move(centx,centy);		
		v_draw(tx1,ty1);	
	}
	else	v_move(tx1,ty1);	

	xn=cos(thet1);
	yn=sin(thet1);
		
	for (ctr=0; ctr<qp; ctr++) {
		xn1=(xn*costh)-(yn*sinth);
		yn1=(yn*costh)+(xn*sinth);

		xn=xn1;
		yn=yn1;

		xn1 *= radx;
		yn1 *= rady;

		tx1 = ((xn1 * rotcos) - (yn1 * rotsin)) + centx; 
		ty1 = ((yn1 * rotcos) + (xn1 * rotsin)) + centy;

		if (fg) v_draw(tx1,ty1);
		else	v_drawn(tx1,ty1);

		EllipseClip |= clip_sret;

	}

	if (pacman&&fg) v_draw(centx,centy);

	v_reset();
	
	pacman = FALSE;


}

#ifdef DAMNFAST
VOID setbits(x,y)
FAST UWORD x,y;
{

	FAST ULONG 	ad;
	FAST UWORD outword;
	FAST UBYTE i;

	if (x<0||
	    y<0||
	    x>XWidth||
	    y>YWidth) {
		clip_sret |= WIND_OFF;
		return;
	}

	clip_sret |= WIND_IN;

	if (drawmode==255) return;

	x = XBegin + x;
	y = YBegin + YWidth - y;

	ad = (wordcol * y) + (x>>4);
	outword = 1<<(15-(x&15));		/* What bit is it */

	if (drawmode&COMPLEMENT) {
		for (i=0; i<Sdepth; i++)
			dispbuffer[i][ad] ^= outword;
	} else {
		for (i=0; i<Sdepth; i++)
			if (drawpen&(1<<i))	dispbuffer[i][ad] |= outword;
			else			dispbuffer[i][ad] &= ~outword;
	}

}
#endif

/*	C Implementation of the Sutherland-Hodgman Polygon Clipping Algorithm
		See Principles of Interactive Computer Graphics,
			By Newman & Sproull, Pages 69-72	*/

LOCAL STDFLT clipx[2],clipy[2];
LOCAL STDFLT fx[4],fy[4],sx[4],sy[4];
LOCAL int levct[5];
BOOL drawhatch=FALSE;

BOOL PolySetup(drawit)
BOOL drawit;
{
	FAST int i;

	drawhatch = drawit;

	clipx[0] = xoset;
	clipx[1] = xoset + right;
	
	clipy[0] = yoset;
	clipy[1] = yoset + top;
	
	levct[4] = 0;
	for (i=0; i<4; i++) {
		levct[i] = 0;
		fx[i] = fy[i] = sx[i] = sy[i] = 0.0;
	}
	
	clip_sret = WIND_IN;

	if (!SetupRas()) return(FALSE);
	return(TRUE);

}

VOID PolygonClip(px,py,it)
STDFLT px,py;
int it;
{
	WORD wx,wy;
	BOOL sg1,sg2;
	int i;

	STDFLT ix,iy;
	
	levct[it] ++;

	if (it>3) {
		if (!drawhatch) return;
		MapWorldToScreen(&wx,&wy,px,py);
		if (levct[it]==1) 	AreaMove(rp, wx+XBegin,wy+YBegin);
		else			AreaDraw(rp, wx+XBegin,wy+YBegin); 
		
		return;
	}
	
	switch (levct[it]) {
	case 1:
		fx[it] = px;
		fy[it] = py;
		break;
	default:		/* Does SP intersect boundary */

		switch (it) {
		case 0:		/* Bottom clipy[0] */
			sg1 = (sy[it] > clipy[0])?TRUE:FALSE;
			sg2 = (py > clipy[0])?TRUE:FALSE;
			break;
		case 1:		/* Top clipy[1] */
			sg1 = (sy[it] > clipy[1])?TRUE:FALSE;
			sg2 = (py > clipy[1])?TRUE:FALSE;
			break;
		case 2:		/* Left clipx[0] */
			sg1 = (sx[it] > clipx[0])?TRUE:FALSE;
			sg2 = (px > clipx[0])?TRUE:FALSE;
			break;
		case 3:		/* Right clipx[1] */
			sg1 = (sx[it] > clipx[1])?TRUE:FALSE;
			sg2 = (px > clipx[1])?TRUE:FALSE;
			break;
		}
		if (sg1!=sg2) {		/* Calculate intersection and out */
			clip_sret = WIND_CLIP;
			switch (it) {
			case 0:
				ix = px+((sx[it]-px)*(clipy[0]-py)/(sy[it]-py));
				iy = clipy[0];
				break;
			case 1:
				ix = px+((sx[it]-px)*(clipy[1]-py)/(sy[it]-py));
				iy = clipy[1];
				break;
			case 2:
				iy = py+((sy[it]-py)*(clipx[0]-px)/(sx[it]-px));
				ix = clipx[0];
				break;
			case 3:
				iy = py+((sy[it]-py)*(clipx[1]-px)/(sx[it]-px));
				ix = clipx[1];
				break;
			}

			PolygonClip(ix,iy,it+1);	

		}
		
		break;
	}
	
	sx[it] = px;
	sy[it] = py;
	
	sg1 = FALSE;	
	switch (it) {		/* Is s on visible side of line */
	case 0:
		sg1 = (sy[it]>clipy[0])?TRUE:FALSE;
		break;
	case 1:
		sg1 = (sy[it]<clipy[1])?TRUE:FALSE;
		break;
	case 2:
		sg1 = (sx[it]>clipx[0])?TRUE:FALSE;
		break;
	case 3:
		sg1 = (sx[it]<clipx[1])?TRUE:FALSE;
		break;
	}
	
	if (sg1) PolygonClip(sx[it],sy[it],it+1);
	
}

VOID DoPolyClosure(it)	/* Forces last vertex out */
int it;
{
	STDFLT ix,iy;
	BOOL sg1,sg2;

	/* Does SF intersect boundary */

	switch (it) {
	case 0:		/* Bottom clipy[0] */
		sg1 = (sy[it] > clipy[0])?TRUE:FALSE;
		sg2 = (fy[it] > clipy[0])?TRUE:FALSE;
		break;
	case 1:		/* Top clipy[1] */
		sg1 = (sy[it] > clipy[1])?TRUE:FALSE;
		sg2 = (fy[it] > clipy[1])?TRUE:FALSE;
		break;
	case 2:		/* Left clipx[0] */
		sg1 = (sx[it] > clipx[0])?TRUE:FALSE;
		sg2 = (fx[it] > clipx[0])?TRUE:FALSE;
		break;
	case 3:		/* Right clipx[1] */
		sg1 = (sx[it] > clipx[1])?TRUE:FALSE;
		sg2 = (fx[it] > clipx[1])?TRUE:FALSE;
		break;
	}
	if (sg1!=sg2) {		/* Calculate intersection and out */
		clip_sret = WIND_CLIP;
		switch (it) {
		case 0:
			ix = fx[it]+((sx[it]-fx[it])*(clipy[0]-fy[it])/(sy[it]-fy[it]));
			iy = clipy[0];
			break;
		case 1:
			ix = fx[it]+((sx[it]-fx[it])*(clipy[1]-fy[it])/(sy[it]-fy[it]));
			iy = clipy[1];
			break;
		case 2:
			iy = fy[it]+((sy[it]-fy[it])*(clipx[0]-fx[it])/(sx[it]-fx[it]));
			ix = clipx[0];
			break;
		case 3:
			iy = fy[it]+((sy[it]-fy[it])*(clipx[1]-fx[it])/(sx[it]-fx[it]));
			ix = clipx[1];
			break;
		}
		PolygonClip(ix,iy,it+1);	
	}
}

VOID PolyClosure()
{
	int i;
	
	for (i=0; i<4; i++) DoPolyClosure(i);
}

VOID PolyRender()
{

	if (levct[4]>0) {
		if (drawhatch) AreaEnd(rp);
	}
	else	clip_sret = WIND_OFF;

}

VOID PolyVertex(px,py)
STDFLT px,py;
{

	PolygonClip(px,py,0);	/* Get it going */
}

VOID HatchedEllipse(centx,centy,radx,rady,bega,enda,rotation,fg)
STDFLT centx,centy,radx,rady,bega,enda,rotation;
BOOL fg;
{

	STDFLT xn,yn,xn1,yn1,tx1,ty1;
	FAST	WORD qp,ctr,i;
	STDFLT sinth,costh,theta;
	STDFLT csx,rotcos,rotsin,thet1;
	STDFLT deltang;

	clip_sret = WIND_OFF;

	if (radx<=0.0) return;
	if (rady<=0.0) return;		/* Don't let this happen to you! */

	csx = max(radx,rady);		/* Simple implemention */
	if (centx+csx<xoset) return;
	if (centx-csx>xoset+right) return;
	if (centy+csx<yoset) return;
	if (centy-csx>yoset+top) return;

	if (!PolySetup(fg)) return;

	deltang=enda-bega;
	while (deltang<0.02) deltang+=TWOPI;
	if (deltang>TWOPI) deltang=TWOPI;

	if (radx==rady&&deltang>=(TWOPI-.01)) {		/* Its a circle */
		bega = 0.0;
		enda = 0.0;
		rotation = 0.0;		
	}

	if (abs(rotation)<=0.01) {	/* Not rotated */
		rotcos = 1.0;
		rotsin = 0.0;
	} else {
		rotcos = cos(rotation);
		rotsin = sin(rotation);
	}

	if (deltang<(TWOPI-.01)) {		/* Drawing an arc, recalc */

		bega -= rotation;
		enda -= rotation;
		if (bega<0.0) bega+=TWOPI;
		if (enda<0.0) enda+=TWOPI;

		bega=ConvEllipNotation(bega,radx,rady);
		enda=ConvEllipNotation(enda,radx,rady);

		deltang=enda-bega;
		while (deltang<0.02) deltang+=TWOPI;
		if (deltang>TWOPI) deltang=TWOPI;
	}

	xn = (deltang * max(radx,rady)) / ypixsize;
	
	qp=xn;

	if (qp>8000)		ctr = 110;
	else if (qp>3000)	ctr = 85;
	else if (qp>2000)	ctr = 75;
	else if (qp>1000)	ctr = 70;
	else if (qp>500)	ctr = 50;
	else if (qp>100)	ctr = 30;
	else if (qp>50)		ctr = 10;
	else if (qp>20)		ctr = 8;
	else if (qp>10)		ctr = 5;
	else ctr=4;

	qp = ctr;
	xn=qp;
	
	theta=deltang/xn;

	sinth=sin(theta);
	costh=cos(theta);

	thet1 = bega;
	xn=cos(thet1)*radx;		/* Calc start */
	yn=sin(thet1)*rady;
	tx1 = ((xn * rotcos) - (yn * rotsin)) + centx; /* Rot ellipse */
	ty1 = ((yn * rotcos) + (xn * rotsin)) + centy;

	PolyVertex(tx1,ty1);	

	xn=cos(thet1);
	yn=sin(thet1);
		
	for (ctr=0; ctr<qp; ctr++) {
		xn1=(xn*costh)-(yn*sinth);
		yn1=(yn*costh)+(xn*sinth);

		xn=xn1;
		yn=yn1;

		xn1 *= radx;
		yn1 *= rady;

		tx1 = ((xn1 * rotcos) - (yn1 * rotsin)) + centx; 
		ty1 = ((yn1 * rotcos) + (xn1 * rotsin)) + centy;

		PolyVertex(tx1,ty1);

	}

	PolyClosure();
	PolyRender();

}

#define POLYPIECE 512
struct polysto {
	struct	polysto	*next,*prev;
	UWORD	inhabitants;
	UBYTE	flags[POLYPIECE];
	/* Flags belong to the line which this vector completes */
	/* The flag with POL_SEG is the line which closes the polygon */
#define POL_SEG 0x01		/* Begin Segment */
#define POL_INT 0x02		/* There is an intersection */
#define POL_UP  0x04		/* Line goes up */
#define POL_DWN	0x08		/* Line goes down (if UP&DWN==0, horizontal) */
#define POL_HORIZ (POL_UP|POL_DWN)
	STDFLT	x[POLYPIECE];
	STDFLT	y[POLYPIECE];
} *rootpoly=NULL, *tailpoly=NULL, *currentpoly=NULL;

struct Remember *PolyBuffer;

VOID NewPolyBuffer()
{

	struct polysto *newbuff;
	
	newbuff = AllocRemember(&PolyBuffer, sizeof (struct polysto), 0);
	
	if (newbuff==NULL) return;
	
	newbuff->inhabitants = 0;
	newbuff->next = NULL;
	newbuff->prev = NULL;
	
	if (tailpoly==NULL) rootpoly = newbuff;
	else {
		tailpoly->next = newbuff;
		newbuff->prev = tailpoly;
	}
	
	tailpoly = newbuff;

}

VOID FreePolyBuffer()
{

	if (rootpoly==NULL) return;
	
	FreeRemember(&PolyBuffer,TRUE);
	
	rootpoly = tailpoly = NULL;
	
}

VOID ClearPolyBuffer()
{

	struct polysto *looper;
	
	for (looper=rootpoly; looper!=NULL; looper=looper->next) 
		looper->inhabitants = 0;
		
}

UBYTE Clip_Hatch = WIND_IN;

STDFLT boundxmin,boundxmax,boundymin,boundymax;

VOID ClipBoundarySetup()
{

	if (rootpoly==NULL) NewPolyBuffer();
	currentpoly = rootpoly;

	ClearPolyBuffer();
	
	Clip_Hatch = WIND_IN;
	
	boundxmin = boundymin = MAXBIG;
	boundxmax = boundymax = -MAXBIG;
	
}

VOID ClipBoundaryMove(x,y)
STDFLT x,y;
{

	FAST UWORD i;

	if (currentpoly==NULL) return;		/* Just not enough memory */

	i = currentpoly->inhabitants;
	if (i>=POLYPIECE) {
		if (currentpoly->next==NULL) NewPolyBuffer();
		if (currentpoly->next==NULL) return;
		currentpoly = currentpoly->next;
		i = currentpoly->inhabitants;
	}
	
	if (x<boundxmin) boundxmin = x;
	if (y<boundymin) boundymin = y;
	if (x>boundxmax) boundxmax = x;
	if (y>boundymax) boundymax = y;
	
	currentpoly->flags[i] = POL_SEG;	/* Begin segment */
	currentpoly->x[i] = x;
	currentpoly->y[i] = y;
	
	currentpoly->inhabitants = i+1;
}

VOID ClipBoundaryDraw(x,y)
STDFLT x,y;
{

	FAST UWORD i;
	STDFLT ox,oy;

	if (currentpoly==NULL) return;		/* Just not enough memory */

	i = currentpoly->inhabitants;
	ox = currentpoly->x[i-1];
	oy = currentpoly->y[i-1];
	
	if (i>=POLYPIECE) {
		if (currentpoly->next==NULL) NewPolyBuffer();
		if (currentpoly->next==NULL) return;
		currentpoly = currentpoly->next;
		i = currentpoly->inhabitants;
	}
	
	if (x<boundxmin) boundxmin = x;
	if (y<boundymin) boundymin = y;
	if (x>boundxmax) boundxmax = x;
	if (y>boundymax) boundymax = y;
	
	currentpoly->flags[i] = 0x00;	/* clear all flags */
	currentpoly->x[i] = x;
	currentpoly->y[i] = y;
	
	/* What direction is the line */
	
	if (oy!=y) {
		if (y>oy) currentpoly->flags[i] |= POL_UP;
		else 	currentpoly->flags[i] |= POL_DWN;
	}
	
	currentpoly->inhabitants = i+1;
}

LOCAL STDFLT tsp;
#define swp(x,y) tsp = x; x = y; y = tsp

ltagint(intx,inty,bla)
STDFLT intx,inty;
int bla;
{
	STDFLT t;

	t = xpixsize * 3.0;

	SetAPen(rp,bla);
	v_move(intx-t,inty-t);
	v_draw(intx+t,inty+t);
	v_move(intx+t,inty-t);
	v_draw(intx-t,inty+t);
}

BOOL LineSegment(vx,vy,ax1,ay1,ax2,ay2,bx1,by1,bx2,by2)
STDFLT vx,vy,ax1,ay1,ax2,ay2,bx1,by1,bx2,by2;
{

	int ck;

	ck = 0;

	if (ax1==ax2) ck |= 0x01;
	else if ((vx>min(ax1,ax2))&&(vx<max(ax1,ax2))) ck |= 0x01;

	if (ay1==ay2) ck |= 0x02;
	else if ((vy>min(ay1,ay2))&&(vy<max(ay1,ay2))) ck |= 0x02;

	if (bx1==bx2) ck |= 0x04;
	else if ((vx>min(bx1,bx2))&&(vx<max(bx1,bx2))) ck |= 0x04;

	if (by1==by2) ck |= 0x08;
	else if ((vy>min(by1,by2))&&(vy<max(by1,by2))) ck |= 0x08;

	return(ck==0xF);
}

BOOL CalcIntercept(lineAx1,lineAy1,lineAx2,lineAy2,
		   lineBx1,lineBy1,lineBx2,lineBy2,	outX,outY)
		   
STDFLT lineAx1,lineAy1,lineAx2,lineAy2,lineBx1,lineBy1,
	lineBx2,lineBy2,*outX,*outY;

{

	FAST STDFLT	m0,b0,m1,b1;
		
	FAST STDFLT	deltaAx,deltaAy,deltaBx,deltaBy;

	deltaAx = lineAx1-lineAx2;	/* Calc rise/runs for lines */
	deltaAy = lineAy1-lineAy2;
	deltaBx = lineBx1-lineBx2;
	deltaBy = lineBy1-lineBy2;

	/* Switch line B with A if B is horiz or vertical */

	if (deltaBx==0.0||deltaBy==0.0) {
		swp(lineAx1,lineBx1);
		swp(lineAx2,lineBx2);
		swp(lineAy1,lineBy1);
		swp(lineAy2,lineBy2);
		swp(deltaAx,deltaBx);
		swp(deltaAy,deltaBy);
	}

	/* Is line A vertical?  Special case */
	
	if (deltaAx==0.0) {
		if (deltaBx==0.0) return(FALSE);/* Both are vertical */
		*outX = lineAx1;
		if (deltaBy==0.0) {		/* 90 degree intersection */
			*outY = lineBy1;
			return(TRUE);
		}
		m0 = deltaBy/deltaBx;		/* Cal slope */
		b0 = lineBy1 - (m0 * lineBx1);
		*outY = (m0 * (*outX)) + b0;	/* Cal intercept */
 		return(TRUE);
	}

	/* Is line A horizontal? Special case */

	if (deltaAy==0.0) {			
		if (deltaBy==0.0) return(FALSE);/* Lines both horizontal */
		*outY = lineAy1;
		if (deltaBx==0.0) {		/* 90 degree intercept */
			*outX = lineBx1;
			return(TRUE);
		}
		m0 = deltaBy/deltaBx;			/* Cal slope */
		b0 = lineBy1 - (m0 * lineBx1);
		if (m0==0.0) return(FALSE);	/* How did this happen? */
		*outX = ((*outY) - b0) / m0;	/* Calculate intercept */
		return(TRUE);
	}

	m0 = deltaBy/deltaBx;			/* Cal slope of B */
	b0 = lineBy1 - (m0 * lineBx1);		/* Y intercept of B */

	m1 = deltaAy/deltaAx;			/* Cal slope of A */
	b1 = lineAy1 - (m1 * lineAx1);		/* Y intercept of A */

	if ((m0-m1)==0.0&&(m1-m0)==0.0) return(FALSE);	/* Huh? /0 check */

	if ((m0-m1)!=0.0) 	*outX = (b1 - b0) / (m0 - m1); /* Calc Int */
	else			*outX = (b0 - b1) / (m1 - m0); /* Alt Calc */

	*outY = (m0 * (*outX)) + b0;		/* Set value */

	return(TRUE);

}

VOID DrawBegin(x1,y1,x2,y2,begin,start,drawit,cx1,cy1)
STDFLT x1,y1,x2,y2;
BOOL drawit;
int start;
struct polysto *begin;
STDFLT cx1,cy1;
{

	struct polysto *looper;
	int i,count;
	UBYTE *fl1,*fl0,fl;
	BOOL ft=FALSE;
	struct polysto *backloop;
	int backcount;
	
	STDFLT cx2,cy2,bx1,by1,bx2,by2,gx,gy,cx0,cy0;
	
	for (looper=begin; looper!=NULL; looper=looper->next,start=0) {
	    for (i=start; i<looper->inhabitants; i++,cx1=cx2,cy1=cy2) {
	    
		cx2 = looper->x[i];
		cy2 = looper->y[i];

		if (looper->flags[i]&POL_SEG) continue;
		
		if (min(y1,y2)>max(cy1,cy2)) continue;	/* Line above seg */
		if (max(y1,y2)<min(cy1,cy2)) continue;	/* Line below seg */
		if (min(x1,x2)>max(cx1,cx2)) continue;	/* Line right seg */
		if (max(x1,x2)<min(cx1,cx2)) continue;	/* Line left seg */

		if (!CalcIntercept(x1,y1,x2,y2,cx1,cy1,cx2,cy2,&gx,&gy))
				continue; 	/* Does not intercept */

		if (!LineSegment(gx,gy,x1,y1,x2,y2,cx1,cy1,cx2,cy2)) continue;

		/* Break up segments and go recursive */

		DrawBegin(x1,y1,gx,gy,looper,i+1,drawit,cx2,cy2);
		DrawBegin(gx,gy,x2,y2,looper,i+1,drawit,cx2,cy2);
		
		return;
	    }
	}	/* If we managed to exit here, there were no breaks */
	
	/* Now need to see if segment is visible */
	/* Using thor's optimized inside algorithm */
		
	for (looper=rootpoly; looper!=NULL; looper=looper->next) 
	    for (i=0; i<looper->inhabitants; i++) 
	    	looper->flags[i] &= (~POL_INT);	    /* Clear intersect flag */
	    
	bx1 = x1; by1 = y1; bx2 = x2; by2 = y2;
	
	cx1 = (x2+x1) / 2.0;	/* Calculate line midpoint */
	cy1 = (y2+y1) / 2.0;

	x1 = cx1;		/* Make a ray from [cx1,cy1] on left */
	y1 = cy1;
	x2 = xviewbegin-xviewsize;
	y2 = cy1;

	/* Go dig for the last non-horiz line before first */
	
	/*******We need more work here for multiple polygon tables********/

	fl0 = NULL;
	i = 2;	/* We must go back 2 */
	for (backloop=tailpoly; backloop!=NULL; backloop=backloop->prev) {
		for (backcount=backloop->inhabitants-1; 
				backcount>=0; backcount--) {
			
			fl = backloop->flags[backcount];
			
			if (fl&POL_SEG) continue;	/* Cannot use this */
			if ((fl&POL_HORIZ)==0) continue; /* It's horizontal */
			
			/* We have to get the starting x,y */
			if (i==2) {	/* First pass */
				cx1=backloop->x[backcount];
				cy1=backloop->y[backcount];
				fl0 = &(backloop->flags[backcount]);
			}
			if (i==1) {
				cx0=backloop->x[backcount];
				cy0=backloop->y[backcount];
			}
			i--;
			if (i<=0) break;		/* We found them */
			
		}
		if (backcount>=0) break;	/* Get out */
	}
			
	if (backloop==NULL) return;	/* No polygon, or it's flat */
	if (fl0==NULL) return;

	ft=TRUE;			
	for (looper=rootpoly; looper!=NULL; looper=looper->next) {
	    for (i=0; i<looper->inhabitants; i++) {
	    
		/******** It calculates this each time!  Got to get
		   this out of the loop --- very inefficient ********/

		if (looper->flags[i]&POL_SEG) {	
			/* Didn't set the trend flags originally */
	
			looper->flags[i] &= (~POL_HORIZ);	/* Clear */
			if (looper->y[i]!=cy0) {
				if (looper->y[i]>cy0) 
					looper->flags[i] |= POL_UP;
				else 	looper->flags[i] |= POL_DWN;
			}
		}
		
		/* Let the horizontal lines drop out of processing, 
		   the X factors have to be repaired, cx0 is never really
		   used except for debugging */
			    
		if ((looper->flags[i]&POL_HORIZ)==0) {
			if (ft) {
				ft=FALSE;
				continue;	/* Skip anyway */
			}
			
			/*ltagint(looper->x[i],looper->y[i],4);*/

			cx0=cx1;
			cx1=cx2;
			cx2 = looper->x[i];
			
			continue;		/* Skip it */
		
		} else {
		    	if (!ft) {
				cx0=cx1;
				cy0=cy1;
				
				fl0=fl1;
			
			    	cx1=cx2;
				cy1=cy2;
			}
		}
						
		ft = FALSE;
	
		/* Get factors for next vertex */
		
		fl1 = &(looper->flags[i]);  
		cx2 = looper->x[i];
		cy2 = looper->y[i];

		/*v_move(cx1,cy1);
		v_draw(cx2,cy2);*/

		/* If the point is inside the box of the boundary
		   line, we need to calculate the intersection, otherwise
		   we can cheat.  We can just do range comparisons -- by the
		   nature of the beast, there can only be one intersection
		   per line.   If the line directions are different and there
		   is a line between the common vertical segments, there must
		   be two intersections.  If the lines are the same direction
		   there can only be one.  Any number of intervening 
		   horizonals (should be only one, but who can tell) 
		   are ignored.	*/

		/* Try for some quick tests for simple elimination */

		if (y1<min(cy1,cy2)) continue;	/* Line below point - skip */
	
		if (y1>max(cy1,cy2)) continue;	/* Line above point - skip */
					
		if (min(cx1,cx2)>x1) continue;	/* Line is right of point */
					
		if (max(cx1,cx2)>=x1) {		/* It's in the box */
		
			CalcIntercept(x1,y1,x2,y2,cx1,cy1,cx2,cy2,&gx,&gy);
			
			/* We could have singularities in the box */
		
			if ((*fl1&POL_HORIZ)==
			    (*fl0&POL_HORIZ)) {
				/* if left and both lines trend same way 
				   the ray must intersect, otherwise the 
				   box select wouldn't have caught it. */
			
				if (gx<=x1) {	/* It's left of point */
					*fl1 |= POL_INT;
				
					/* The prev cannot also have an 
					   intercept since the lines go the 
					   same way */
				   
					*fl0 &= (~POL_INT);
				}
				
			} else {

				/* Line reverses direction -- if gx<x1 there
				   must be an intercept.  Check for singul */
				   			
				if (gx<=x1) 	/* It's left of point */
					*fl1 |= POL_INT;
			}
			
			
		} else {   /* Outside box, inline and left of the point */
			
			if ((*fl1&POL_HORIZ)==
			    (*fl0&POL_HORIZ)) {
			    	/* Lines trend same way */
			    
				/* Tag current, untag prev to clear
				   possible singularity */

				*fl1 |= POL_INT;
				*fl0 &= (~POL_INT);
				
			} else {
				/* Lines reverse direction */
					
				*fl1 |= POL_INT;
#ifdef wrongjunk			
				if (*fl1&POL_UP) {
					/* Down-up transition - find min */
						
					gy = min(cy2,cy0);/*min*/
					*fl1 |= POL_INT;
					/*if (y1<=gy)
						*fl0 |= POL_INT;*/
				} else {
					/* Up-Down transition - find max */

					gy = max(cy2,cy0);/*max*/
					*fl1 |= POL_INT;
					/*if (y1>=gy)
						*fl0 |= POL_INT;*/
				}
#endif
			}
		}
		
	    }
	}

	/* Go harvest the intersection counts */

	count = 0;
	for (looper=rootpoly; looper!=NULL; looper=looper->next) 
	    for (i=0; i<looper->inhabitants; i++) 
	    	if (looper->flags[i] & POL_INT) count++;

	if (count&0x01) cdrawline(bx1,by1,bx2,by2,drawit);
	
}

VOID DrawClip(x1,y1,x2,y2,drawit)
STDFLT x1,y1,x2,y2;
BOOL drawit;
{

	if (max(x1,x2)<boundxmin) return;	/* Eliminate if not in field */
	if (min(x1,x2)>boundxmax) return;
	if (max(y1,y2)<boundymin) return;
	if (min(y1,y2)>boundymax) return;

	if (max(x1,x2)<xoset) return;		/* Eliminate if not on screen */
	if (min(x1,x2)>xoset+right) return;
	if (max(y1,y2)<yoset) return;
	if (min(y1,y2)>yoset+top) return;

	DrawBegin(x1,y1,x2,y2,rootpoly,0,drawit,0.0,0.0);

}

VOID FilledEllipse(centx,centy,radx,rady,bega,enda,rotation,fg)
STDFLT centx,centy,radx,rady,bega,enda,rotation;
BOOL fg;
{

	STDFLT xn,yn,xn1,yn1,tx1,ty1,txf,tyf;
	FAST	WORD qp,ctr,i;
	STDFLT sinth,costh,theta;
	STDFLT csx,rotcos,rotsin,thet1;
	STDFLT deltang;

	ClipBoundarySetup();

	if (radx<=0.0) return;
	if (rady<=0.0) return;		/* Don't let this happen to you! */

	csx = max(radx,rady);		/* Simple implemention */
	if (centx+csx<xoset) return;
	if (centx-csx>xoset+right) return;
	if (centy+csx<yoset) return;
	if (centy-csx>yoset+top) return;

	deltang=enda-bega;
	while (deltang<0.02) deltang+=TWOPI;
	if (deltang>TWOPI) deltang=TWOPI;

	if (radx==rady&&deltang>=(TWOPI-.01)) {		/* Its a circle */
		bega = 0.0;
		enda = 0.0;
		rotation = 0.0;		
	}

	if (abs(rotation)<=0.01) {	/* Not rotated */
		rotcos = 1.0;
		rotsin = 0.0;
	} else {
		rotcos = cos(rotation);
		rotsin = sin(rotation);
	}

	if (deltang<(TWOPI-.01)) {		/* Drawing an arc, recalc */

		bega -= rotation;
		enda -= rotation;
		if (bega<0.0) bega+=TWOPI;
		if (enda<0.0) enda+=TWOPI;

		bega=ConvEllipNotation(bega,radx,rady);
		enda=ConvEllipNotation(enda,radx,rady);

		deltang=enda-bega;
		while (deltang<0.02) deltang+=TWOPI;
		if (deltang>TWOPI) deltang=TWOPI;
	}

	xn = (deltang * max(radx,rady)) / ypixsize;
	
	qp=xn;

	if (qp>8000)		ctr = 110;
	else if (qp>3000)	ctr = 85;
	else if (qp>2000)	ctr = 75;
	else if (qp>1000)	ctr = 70;
	else if (qp>500)	ctr = 50;
	else if (qp>100)	ctr = 30;
	else if (qp>50)		ctr = 10;
	else if (qp>20)		ctr = 8;
	else if (qp>10)		ctr = 5;
	else ctr=4;

	qp = ctr;
	xn=qp;
	
	theta=deltang/xn;

	sinth=sin(theta);
	costh=cos(theta);

	thet1 = bega;
	xn=cos(thet1)*radx;		/* Calc start */
	yn=sin(thet1)*rady;
	tx1 = ((xn * rotcos) - (yn * rotsin)) + centx; /* Rot ellipse */
	ty1 = ((yn * rotcos) + (xn * rotsin)) + centy;

	EllipseClip=0;

	ClipBoundaryMove(tx1,ty1);		
	txf = tx1;
	tyf = ty1;

	xn=cos(thet1);
	yn=sin(thet1);
		
	for (ctr=0; ctr<qp; ctr++) {
		xn1=(xn*costh)-(yn*sinth);
		yn1=(yn*costh)+(xn*sinth);

		xn=xn1;
		yn=yn1;

		xn1 *= radx;
		yn1 *= rady;

		tx1 = ((xn1 * rotcos) - (yn1 * rotsin)) + centx; 
		ty1 = ((yn1 * rotcos) + (xn1 * rotsin)) + centy;

		ClipBoundaryDraw(tx1,ty1);

	}

	ClipBoundaryDraw(txf,tyf);
	
}
