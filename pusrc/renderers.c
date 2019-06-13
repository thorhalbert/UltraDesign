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
 *	2	10-Jul-89 SH	Add support for hatched ellipses
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "plotsetup.h"
#include "mempools.h"
#include <libraries/dos.h>
#include <graphics/gfxmacros.h>
#include <math.h>

int printf();

/*----------------------------------Macros-----------------------------------*/

#define vw_move(x,y) lastx = x, lasty = y
#define vw_draw(x,y) dotline(lastx,lasty,x,y), lastx=x, lasty=y

BOOL preview=FALSE;
#define INP if (!preview) 
#define IP if (preview) 

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

STDFLT MinTX,MaxTX,MinTY,MaxTY;
LOCAL STDFLT left,right,top,bottom;

#define LEFT_S	0x0001
#define RIGH_S	0x0002
#define BOTM_S	0x0004
#define TOPO_S	0x0008

LOCAL	WORD	flag1,flag2,cflag,rflag,mask1,mask2;
LOCAL	STDFLT	tx,ty,ypix,xpix;
LOCAL	LONG 	sx1,sy1,ox1=-1,oy1=-1;

VOID cvtview(ix,iy,nx,ny)
STDFLT ix,iy,*nx,*ny;
{

	ix -= Current_View->XViewMIN;
	iy -= Current_View->YViewMIN;
	ix *= Current_View->ViewScale;
	iy *= Current_View->ViewScale;

	*nx = ix*Current_View->RotCos-iy*Current_View->RotSin+
		Current_View->XNewOrigin;

	*ny = ix*Current_View->RotSin+iy*Current_View->RotCos+
		Current_View->YNewOrigin;
}

cvtsheet(ix,iy,sx,sy)
STDFLT ix,iy;
LONG *sx,*sy;
{

	ix -= MinTX;
	iy -= MinTY; 

	ix /= ((STDFLT) mainbase.MPI->StepX);
	iy /= ((STDFLT) mainbase.MPI->StepY);

	*sx = ix + mainbase.MPI->MinCX;
	*sy = iy + mainbase.MPI->MinCY;

}

VOID Line_Normal()	/* Slop over 1% on each side for edge fit */
{

	STDFLT tmp;

	left = Current_View->XViewMIN;
	right = Current_View->XViewMAX;
	tmp = right-left;
	tmp *= .01;
	left -= tmp;
	right += tmp;
	
	bottom = Current_View->YViewMIN;
	top = Current_View->YViewMAX;
	tmp = top-bottom;
	tmp *= .01;
	bottom -= tmp;
	top += tmp;
}

VOID vw_reset()
{

	ox1 = 99999;
	oy1 = -99999;

}
	
VOID rawdisplay(ix1,iy1,ix2,iy2)
STDFLT ix1,iy1,ix2,iy2;
{

	flag1=0;
	if (ix1 < MinTX)		flag1 |= LEFT_S;
	else	if (ix1 > MaxTX)	flag1 |= RIGH_S;
	if (iy1 < MinTY)		flag1 |= BOTM_S;
	else	if (iy1 > MaxTY)	flag1 |= TOPO_S;

	flag2=0;
	if (ix2 < MinTX)		flag2 |= LEFT_S;
	else	if (ix2 > MaxTX)	flag2 |= RIGH_S;
	if (iy2 < MinTY)		flag2 |= BOTM_S;
	else	if (iy2 > MaxTY)	flag2 |= TOPO_S;

	if (flag1&flag2) return;		/* Lines off screen */

	mask1=mask2=0;

	while (flag1|flag2) {
	
		if (flag1&flag2) return;		/* Lines off screen */

		if (flag1==0) 	cflag = flag2;
		else 		cflag = flag1;
	
		if (cflag & LEFT_S) {
			ty = iy1+((iy2-iy1)*(MinTX-ix1)/(ix2-ix1));
			tx = MinTX;
			rflag = LEFT_S;
		} else
		if (cflag & RIGH_S) {
			ty = iy1+((iy2-iy1)*(MaxTX-ix1)/(ix2-ix1));
			tx = MaxTX;
			rflag = RIGH_S;
		} else
		if (cflag & BOTM_S) {
			tx = ix1+((ix2-ix1)*(MinTY-iy1)/(iy2-iy1));
			ty = MinTY;
			rflag = BOTM_S;
		} else
		if (cflag & TOPO_S) {
			tx = ix1+((ix2-ix1)*(MaxTY-iy1)/(iy2-iy1));
			ty = MaxTY;
			rflag = TOPO_S;
		}

		if (cflag==flag1) {	
			ix1 = tx;
			iy1 = ty;
			mask1 |= rflag;

			flag1=0;
			if (ix1 < MinTX)		flag1 |= LEFT_S;
			else	if (ix1 > MaxTX)	flag1 |= RIGH_S;
			if (iy1 < MinTY)		flag1 |= BOTM_S;
			else	if (iy1 > MaxTY)	flag1 |= TOPO_S;

			flag1 &= ~mask1;
		} else {	
			ix2 = tx;
			iy2 = ty;
			mask2 |= rflag;

			flag2=0;
			if (ix2 < MinTX)		flag2 |= LEFT_S;
			else	if (ix2 > MaxTX)	flag2 |= RIGH_S;
			if (iy2 < MinTY)		flag2 |= BOTM_S;
			else	if (iy2 > MaxTY)	flag2 |= TOPO_S;

			flag2 &= ~mask2;	
		}
	}

/*	printf("rawout (%f,%f),(%f,%f)\n",ix1,iy1,ix2,iy2); 	*/

	cdrawline(ix1,iy1,ix2,iy2,TRUE); 	/* Draw on screen */

	cvtsheet(ix1,iy1,&sx1,&sy1);
	if (sx1!=ox1||sy1!=oy1)  INP (*mainbase.PLI->movetopoint)(sx1,sy1); 

	cvtsheet(ix2,iy2,&ox1,&oy1);
	if (sx1!=ox1||sy1!=oy1)	 INP (*mainbase.PLI->drawaline)(ox1,oy1); 

/*	printf("absout (%d,%d),(%d,%d)\n",sx1,sy1,ox1,oy1);	*/

	return;

}

VOID HomePen()
{

	INP (*mainbase.PLI->movetopoint)
		(mainbase.MPI->MinCX,mainbase.MPI->MinCY); 

	vw_reset();
}

VOID viewline(ix1,iy1,ix2,iy2)
STDFLT ix1,iy1,ix2,iy2;
{

	STDFLT nx1,ny1,nx2,ny2;

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

	if (flag1&flag2) return;		/* Lines off screen */

	mask1=mask2=0;

	while (flag1|flag2) {
	
		if (flag1&flag2) return;		/* Lines off screen */

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

	cvtview(ix1,iy1,&nx1,&ny1);
	cvtview(ix2,iy2,&nx2,&ny2);

/*	printf("ix1=%f,iy1=%f,ix2=%f,iy2=%f,nx1=%f,ny1=%f,nx2=%f,ny2=%f\n",
		ix1,iy1,ix2,iy2,nx1,ny1,nx2,ny2);	*/

	rawdisplay(nx1,ny1,nx2,ny2);

	return;

}

#define MAXPAT 18

STDFLT linpat[MAXPAT];		/* Dot dash pattern, pos is pen down,
				   neg is pen up, end with 0.0 */

STDFLT lineleft=0.0;		/* Distance left in this pattern */
int patnum=0;			/* Pattern currently in */

UWORD btab[16] = { 1, 1<<1, 1<<2, 1<<3, 1<<4, 1<<5, 1<<6, 1<<7, 1<<8,
		1<<9, 1<<10, 1<<11, 1<<12, 1<<13, 1<<14, 1<<15 };
UWORD oldpat=0x12e2;

VOID setdash(tpat)
register UWORD tpat;
{

	register UBYTE i,j,k;
	STDFLT use_dot;
	
	if (tpat==oldpat) return;	/* Already set up */
	
	oldpat = tpat;
	
	if (tpat==0||tpat==0xFFFF) {
		linpat[0] = 0.0;
		return;
	}
	
	use_dot = dot_size / Current_View->ViewScale;

/*	for (i=0,j=0; i<16; i++) {
		if (tpat&btab[i]) {
			if (k) 
				linpat[j] += use_dot;
			else	{
				j++;
				linpat[j] = use_dot;
				k=TRUE;
			}
		} else { 
			if (!k) 
				linpat[j] -= use_dot;
			else	{
				j++;
				linpat[j] = -use_dot;
				k=FALSE;
			}
		}
	}

	j++;
	linpat[j] = 0.0;	*/
	
	
	for (i=0; i<16; i++)
		if (tpat&btab[i]) linpat[i] = use_dot;
		else		linpat[i] = -use_dot;
		
	linpat[16] = 0.0; 
}

VOID dotline(x1,y1,x2,y2)
STDFLT x1,y1,x2,y2;
{

	STDFLT hypo,adj,opp,tx,ty,left,tcos,tsin;

	if (linpat[0]==0.0) {
		viewline(x1,y1,x2,y2);	/* Don't mess around */
		return;
	}

	opp = y2 - y1;
	adj = x2 - x1;

	hypo = sqrt(opp*opp+adj*adj);
	if (hypo==0.0) return;		/* No line here */

	tcos = adj/hypo;
	tsin = opp/hypo;

	FOREVER {

		if (lineleft>hypo) {
			lineleft -= hypo;
			if (linpat[patnum]<0.0) return;
			viewline(x1,y1,x2,y2);
			return;
		}

		tx = lineleft*tcos + x1;
		ty = lineleft*tsin + y1;

		if (linpat[patnum]>0.0) viewline(x1,y1,tx,ty);

		hypo -= lineleft;

		patnum++;
		if (linpat[patnum]==0.0) patnum=0;

		x1 = tx;
		y1 = ty;

		lineleft = linpat[patnum];
		if (lineleft<0) lineleft = -lineleft;
	}

}

PutText(instyle,text,hx,hy,ang,flipflags,handle)
struct Lettering_Styles *instyle;
UBYTE *text;
STDFLT hx,hy,ang;
UWORD flipflags;
int handle;
{

	UBYTE 	*inc;
	STDFLT 	*sizar;
	STDFLT 	x,y,ll,lr;
	STDFLT 	slant,height,width,tcos,tsin,csz,asp;
	WORD	*off;
	WORD	*strx;
	int	cr,l,o,ina,tx,ty,pen;
	STDFLT	rx,ry;
	STDFLT	_sclv,_sclh,_rno,_trno;
	UBYTE	j;
	BOOL 	backwards;

	asp = instyle->Aspect;

	vw_reset();

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

	if (flipflags&TEXT_CHRFL&&flipflags&TEXT_FLIPX) {
		flipflags &= ~(TEXT_CHRFL|TEXT_FLIPX);
		backwards=TRUE;
	} else 	backwards=FALSE;
	if (flipflags&(TEXT_FLIPX|TEXT_CHRFL)) slant = -slant;

	y = 0.0;
	x = ll + (slant*y);
	
	if (flipflags&TEXT_FLIPY) y = height;
	if (flipflags&TEXT_FLIPX) x = lr*asp;

	vw_move(x*tcos-y*tsin+hx,x*tsin+y*tcos+hy);

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
			vw_move(rx*tcos-ry*tsin+lastx,rx*tsin+ry*tcos+lasty);
		}

		l = off[j]-1;
		o = strx[l];
		ina = -o;

		for (;;ina=strx[++l]) {

			if (ina<=0) break;

			ty = ina & 0x7F;
			ina>>=7;

			tx = ina & 0x7F;
			ina>>=7;

			pen=ina;

			ry = _sclv * ((STDFLT) (ty-64));
			if (flipflags&TEXT_FLIPY) ry = -ry;
			rx = _sclh * ((STDFLT) (tx-64)) + slant*ry;
			if (flipflags&(TEXT_FLIPX|TEXT_CHRFL)) rx = -rx;
			rx *= asp;

			if ((3-pen)==3)		
				vw_move(rx*tcos-ry*tsin+lastx,
			       		rx*tsin+ry*tcos+lasty);
			else	vw_draw(rx*tcos-ry*tsin+lastx,
			       		rx*tsin+ry*tcos+lasty);
		
		}

		_trno = _rno * asp;
		if (flipflags&(TEXT_FLIPX|TEXT_CHRFL)) _trno = - _trno;
	
		vw_move(_trno*tcos+lastx,_trno*tsin+lasty);
	
		if (flipflags&TEXT_CHRFL) {
			rx = csz*asp;
			ry = 0.0;
			vw_move(rx*tcos-ry*tsin+lastx,
 			       rx*tsin+ry*tcos+lasty);
		}
	}

	return;

}

/*---------------------Ellipse Clipper Code-----------------------*/

STDFLT degcvt(theta)
STDFLT theta;
{
	
	theta = theta * 180.0;
	theta /= PI;
	if (theta<0) theta+=360.0;

	return(theta);
}

STDFLT ConvEllipNotation(inang,radiusx,radiusy)
STDFLT inang,radiusx,radiusy;
{

	STDFLT inslope,PID34,inter;

	/* We don't need real radii, since the equation will overflow FFP */
	
	inslope = radiusx;
	radiusx = radiusx/inslope;
	radiusy = radiusy/inslope;	/* Make nice ratio */

	PID34=PI;
	PID34+=PID2;

	if (inang==PID2) return(PID2);
	if (inang==PID34) return(PID34);

	inslope=tan(inang);

	inter=sqrt((radiusx*radiusx*radiusy*radiusy)/
		((radiusy*radiusy)+(radiusx*radiusx*inslope*inslope)))
			/radiusx;

	if (inter>1.0) inter = 1.0;		/* Precaution for FFP */
	if (inter<(-1.0)) inter = -1.0;
	
	if (inang<=PID2) return(acos(inter));
	if (inang<=PI) return(acos(-inter));
	if (inang<=PID34) return(TWOPI-acos(-inter));
	return(TWOPI-acos(inter));
		
}

VOID FancyEllipseClipper(centx,centy,radx,rady,bega,enda,rotation)
STDFLT centx,centy,radx,rady,bega,enda,rotation;
{

	STDFLT xn,yn,xn1,yn1,sinth,costh,theta,difang,tx1,ty1;
	STDFLT csx,rotcos,rotsin,thet1;
	LONG qp,ctr;

	csx = max(radx,rady);		/* Simple elimination */
	if (centx+csx<left) return;
	if (centx-csx>right) return;
	if (centy+csx<bottom) return;
	if (centy-csx>top) return;

	rotcos = cos(rotation);
	rotsin = sin(rotation);

	vw_reset();
	
	bega -= rotation;
	enda -= rotation;
	while (bega<0.0) bega+=TWOPI;
	while (enda<0.0) enda+=TWOPI;

	bega=ConvEllipNotation(bega,radx,rady);
	enda=ConvEllipNotation(enda,radx,rady);

	difang=enda-bega;
	while (difang<0.02) difang+=TWOPI;
	if (difang>TWOPI) difang=TWOPI;

	xn = max(radx,rady) * Current_View->ViewScale; /* Get in plot scale */
	xn /= xpix;
	if (xn<=0) return;		/* Don't divide by zero */

	yn = circstep/(2.0*xn);
	if (yn>1.0) yn=1.0;
	if (yn<-1.0) yn= -1.0;
	theta = 2.0 * asin(yn);

	if (theta>(PI/6.0)) theta = PI/6.0;	/* Min effort 12 steps */
	if (theta<(PI/100.0)) theta = PI/100.0;	/* Max effort 200 steps */

	xn = difang/theta;		/* Round to an integer */

	qp = xn;
	xn = qp;
	
	theta=difang/xn;

	/* printf("step=%f\n",theta/(TWOPI)); */

	sinth=sin(theta);
	costh=cos(theta);

	thet1 = bega;
	xn=cos(thet1)*radx;		/* Calc start */
	yn=sin(thet1)*rady;
	tx1 = ((xn * rotcos) - (yn * rotsin)) + centx; /* Rot ellipse */
	ty1 = ((yn * rotcos) + (xn * rotsin)) + centy;

	vw_move(tx1,ty1);	

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

		vw_draw(tx1,ty1);
	}

}

struct Text_Fonts *Stroke_Search(insearch)
UBYTE *insearch;
{

	struct Text_Fonts *looper;

	for (looper=Root_Font; looper!=NULL; looper=looper->next_font) 
		if (match(insearch,looper->Font_Name_ID)) return(looper);

	return(NULL);

}

BOOL DataErr = FALSE;
BPTR PlotFile=0;

VOID OutData(buffer,len)
UBYTE *buffer;
int len;
{

	int rlen;

	if (PlotFile==0) return;

	rlen = Write(PlotFile, buffer, len);
	if (rlen!=len) DataErr = TRUE;

	return;
}

BOOL unwindplot=FALSE;

LONG startord,startcar,startauto,startpens;
LONG lastord,lastcar,lastauto;

BOOL testgt(a1,a2,a3,b1,b2,b3)
LONG a1,a2,a3,b1,b2,b3;
{

	if (a1>b1) return(TRUE);
	if (a1<b1) return(FALSE);
	if (a2>b2) return(TRUE);
	if (a2<b2) return(FALSE);
	if (a3>b3) return(TRUE);
	if (a3<b3) return(FALSE);
	return(FALSE);			/* They're equal, so answer's false */
}

BOOL findnextpen()
{

	struct ViewContext *lpview;
	PEN_A *gpen;
	LONG	testord,testcar,testauto;
	int a;

	testord=testcar=testauto=65536;

	startpens++;
	if (!(mainbase.MPI->PlotFlags&PLFLAG_PHYPEN)) {
		if (startpens==1) return(TRUE);
		return(FALSE);		/* Only one pen */
	}

	for (lpview = Root_View; lpview!=NULL; lpview=lpview->next) {
		for (a=0; a<lpview->num_pens; a++) {
			gpen = lpview->Pen_Array[a];
			if (gpen==NULL) continue;
			
			if (testgt(testord,testcar,testauto,
				(LONG) gpen->PenOrd,
				(LONG) gpen->Carousel,
				(LONG) gpen->AutoNum)&&
			    testgt((LONG) gpen->PenOrd,
				(LONG) gpen->Carousel,
				(LONG) gpen->AutoNum,
				startord,startcar,startauto)) {
					testord = gpen->PenOrd;
					testcar = gpen->Carousel;
					testauto = gpen->AutoNum;
			}
		}
	}

	if (testauto==65536) return(FALSE);	/* No more found */

	startord = testord;
	startcar = testcar;
	startauto = testauto;

	return(TRUE);				/* Still another */
}

VOID penmount()
{

	UBYTE	Mess1[50],Mess2[50];

	if (!(mainbase.MPI->PlotFlags&PLFLAG_PHYPEN)) return;	/* No pens */

	HomePen();


	if (startauto>0&&mainbase.MPI->AutoPens>0
	    &&startauto<=mainbase.MPI->AutoPens) {
		if (lastauto>0) 
			INP (*mainbase.PLI->setuppen)(NULL,NULL,0,FALSE);
					/* Dismount last pen */
		if (lastcar!=startcar) {
			sprintf(Mess1,"Mount new pens");
			sprintf(Mess2,"Please load carousel %d",
				startcar);
				
			if (Pause_Plot) 
			INP if (complain("Stop","Proceed",1,0,0,0,
				Mess1,Mess2,NULL,NULL)) {
					unwindplot = TRUE;
					return;
			}
		}
		INP (*mainbase.PLI->setuppen)(NULL,NULL,startauto,FALSE);
		lastord = startord;
		lastcar = startcar;
		lastauto = startauto;
		return;
	}

	sprintf(Mess1,"Mount New Pens");
	sprintf(Mess2,"Please manually load pen %d/%d/%d",
		startord,startcar,startauto);
		
		
	INP if (complain("Stop","Proceed",1,0,0,0,
		Mess1,Mess2,NULL,NULL)) {
			unwindplot = TRUE;
			return;
	}

	lastord = startord;
	lastcar = startcar;
	lastauto = startauto;
	return;

}

VOID PutAPass(doview)
struct ViewContext *doview;
{

	printf("PutAPass Called\n");
	Current_View = doview;
	Line_Normal();

	SetDrMd(rp,JAM1);
	SetAPen(rp,2);
	
	vw_reset();
	loadview();

}

VOID FirstPage()
{

	UBYTE	Mess1[50],Mess2[50];

	startord = startcar = startauto = -1;
	lastord = lastcar = lastauto = -1;		/* Nothing mounted */
	startpens = 0;

	if (PosterX==1&&PosterY==1) {
		sprintf(Mess1,"Begin Drawing");
		sprintf(Mess2,"Please Load Forms");
	} else {
		sprintf(Mess1,"Begin Drawing");
		sprintf(Mess2,"Load Form for Poster Page 1,1");
	}

	if (preview) return;

	if (((mainbase.MPI->PlotFlags&PLFLAG_PHYPEN)&Pause_Plot)||
	    (!(mainbase.MPI->PlotFlags&PLFLAG_PHYPEN)&Pause_Page)) 

	if (complain("Stop","Proceed",1,0,0,0,
		Mess1,Mess2,NULL,NULL)) {
			unwindplot = TRUE;
	}
	
	return;

}	

BOOL EndThePage(xp,yp)
int xp,yp;
{
	UBYTE	Mess1[50],Mess2[50];
	BOOL val;

	if ((PosterX==1&&PosterY==1)||((xp+1)==PosterX&&(yp+1)==PosterY)) {
		sprintf(Mess1,"Drawing Completed");
		sprintf(Mess2,"Please Unload Forms");
	} else {
		sprintf(Mess1,"Done With Poster Page %d,%d",xp+1,yp+1);
		yp++;
		if (yp>=PosterY) {
			xp++;
			yp = 0;
		}
		sprintf(Mess2,"Load Form for Page %d,%d",xp+1,yp+1);
	}

	if (preview) return(FALSE);

	if (((mainbase.MPI->PlotFlags&PLFLAG_PHYPEN)&Pause_Plot)||
	    (!(mainbase.MPI->PlotFlags&PLFLAG_PHYPEN)&Pause_Page)) 

	if (complain("Stop","Proceed",1,0,0,0,
		Mess1,Mess2,NULL,NULL)) {
			unwindplot = TRUE;
	}
	
	return(FALSE);
}

VOID SetupAllPens()
{

	struct ViewContext *lpview;
	int i;
	
	for (lpview=Root_View; lpview!=NULL; lpview=lpview->next)
	   for (i=0; i<lpview->num_pens; i++) 
	      if (lpview->Pen_Array[i]!=NULL)
		 INP (*mainbase.PLI->setuppen)
		    (NULL,&(lpview->Pen_Array[i]->pass_pen),0,TRUE);
}
	
VOID DoThePlot()
{

	int xpos,ypos,repas;
	struct ViewContext *lpview;
	BOOL breakout=FALSE;

	maininter.reporterr = printf;
	maininter.dataout = OutData;
	DataErr = FALSE;

	PlotFile = 0;
	if (!(mainbase.MPI->PlotFlags&PLFLAG_PRINTER)) {
		PlotFile = Open(PlotName,MODE_NEWFILE);
		if (PlotFile==0) {
			printf("Couldn't open output file %s\n",PlotName);
			DisplayBeep(screen);
			return;
		}
	}

	unwindplot = FALSE;

	for (xpos = 0; xpos<PosterX; xpos++) {
	  for (ypos = 0; ypos<PosterY; ypos++) {
	    if (unwindplot) break;
	    startord = startcar = startauto = -1;
	    lastord = lastcar = lastauto = -1;		/* Nothing mounted */
	    startpens = 0;

	    if (breakout) break;		/* Get all the way out */
	    if (xpos==0&&ypos==0) 
			FirstPage();
	    if (unwindplot) break;
	    if (breakout) break;		/* Get all the way out */

	    printf("Setup pens\n");
	    SetupAllPens();
	    
	    printf("initplot()\n");
	    INP (*mainbase.PLI->initplot)();
	    else mainbase.MPI->PassPerPage = 1;
	    
	    printf("PassPerPage=%d\n",mainbase.MPI->PassPerPage);
	    while (findnextpen()) {
	      penmount();
	      if (unwindplot) break;
	      for (repas = 0; repas<mainbase.MPI->PassPerPage; repas++) {
	      	if (unwindplot) break;
	        printf("beginpass()\n");
	        INP (*mainbase.PLI->beginpass)();
		else {
			mainbase.MPI->StepY = 1;
			mainbase.MPI->StepX = 1;
			mainbase.MPI->MinPX = 0.0;
			mainbase.MPI->MinPY = 0.0;
			mainbase.MPI->MaxPX = 1000.0;
			mainbase.MPI->MaxPY = 1000.0;
		}
		
	        ypix = mainbase.MPI->StepY;
	        xpix = mainbase.MPI->StepX;
		  /* Calculate the viewport boundaries for this pass */

	/*	fprintf(stdout,"MinCX = %d, MinPX = %d\n",
			mainbase.MPI->MinCX,mainbase.MPI->MinPX);
		fprintf(stdout,"MinCY = %d, MinPY = %d\n",
			mainbase.MPI->MinCY,mainbase.MPI->MinPY);
		fprintf(stdout,"MaxCX = %d, MaxPX = %d\n",
			mainbase.MPI->MaxCX,mainbase.MPI->MaxPX);
		fprintf(stdout,"MaxCY = %d, MaxPY = %d\n",
			mainbase.MPI->MaxCY,mainbase.MPI->MaxPY);
	*/
	
		MinTX = mainbase.MPI->MinPX * xpix;
		MinTX += ((mainbase.MPI->MaxCX-mainbase.MPI->MinCX) * xpix)
			* ((STDFLT) xpos);
		MaxTX = MinTX+((mainbase.MPI->MaxPX-mainbase.MPI->MinPX)*xpix);

		MinTY = mainbase.MPI->MinPY * ypix;
		MinTY += ((mainbase.MPI->MaxCY-mainbase.MPI->MinCY) * ypix)
			* ((STDFLT) ypos);
		MaxTY = MinTY+((mainbase.MPI->MaxPY-mainbase.MPI->MinPY)*ypix);

	/*	fprintf(stdout,"MinTX, MinTY = %g, %g\n",MinTX,MinTY);
		fprintf(stdout,"MaxTX, MaxTY = %g, %g\n",MaxTX,MaxTY);
					
		v_move(MinTX,MinTY);
		v_draw(MinTX,MaxTY);
		v_draw(MaxTX,MaxTY);
		v_draw(MaxTX,MinTY);
		v_draw(MinTX,MinTY);
		
		v_move(MinTX,MinTY);
		v_draw(MaxTX,MaxTX);
		v_move(MaxTX,MinTY);
		v_draw(MinTX,MaxTY);
	*/
		
		for (lpview=Root_View; lpview!=NULL; lpview=lpview->next)
 		  PutAPass(lpview);

	      }
	    }
	    INP (*mainbase.PLI->endplot)();
	    EndThePage(xpos,ypos);
	  }
	}

	if (PlotFile!=0) Close(PlotFile);
}

VOID DoPlot()
{

	preview = FALSE;
	
	DoThePlot();
	
}

VOID DoPreview()
{

	struct plot_info sav_info;
	
	preview = TRUE;
	
	sav_info = *mainbase.MPI;		/* Backup plotinfo */
	DoThePlot();
	*mainbase.MPI = sav_info;		/* Restore plotinfo */
	
}

DoPen(inpen,dashon)
PEN_A *inpen;
BOOL dashon;
{

	if (dashon) 	setdash(Current_Pen->Line_Pattern);
	else		setdash(0);
	
	if (mainbase.MPI->PlotFlags&PLFLAG_PHYPEN) return;

	INP (*mainbase.PLI->setuppen)(NULL,&(inpen->pass_pen),0,FALSE);
}

DoArrow(Pt1X,Pt1Y,Pt2X,Pt2Y)
STDFLT Pt2X,Pt2Y,Pt1X,Pt1Y;
{

	STDFLT	ang,hyp,ex,ang2,nx,ny;

	vw_reset();

	ang = atan2(Pt2Y-Pt1Y,Pt2X-Pt1X);

	hyp = Current_View->Arrowlen/cos(Current_View->ArrowAngle);
	ex = sin(Current_View->ArrowAngle)*hyp;

	ang2 = ang + Current_View->ArrowAngle;

	nx = (cos(ang2)*hyp)+Pt1X;
	ny = (sin(ang2)*hyp)+Pt1Y;

	vw_move(Pt1X,Pt1Y);
	vw_draw(nx,ny);

	ang2 = ang2 - Current_View->ArrowAngle - PID2;

	nx = (cos(ang2)*ex)+nx;
	ny = (sin(ang2)*ex)+ny;

	vw_draw(nx,ny);

	ang2 = ang - Current_View->ArrowAngle;

	nx = (cos(ang2)*hyp)+Pt1X;
	ny = (sin(ang2)*hyp)+Pt1Y;

	vw_move(Pt1X,Pt1Y);
	vw_draw(nx,ny);

	ang2 = ang2 + Current_View->ArrowAngle + PID2;

	nx = (cos(ang2)*ex)+nx;
	ny = (sin(ang2)*ex)+ny;

	vw_draw(nx,ny);

}

VOID dumphatch();
VOID checkline();
VOID checkellipse();

/* Hooks into master rendering routine from the iff reader */

UBYTE testhole[20];/********************/

#define HIP_NONE 0		/* No hatching yet */
#define HIP_HUH 1		/* We got something, but don't know what */
#define HIP_LINE 2		/* It's a line boundary */
#define HIP_ELLIP 3		/* It was an elliptical */

int HIP = HIP_NONE;

BOOL SeenItem = FALSE;		/* Have we seen first item? */
O_HATCH lasthatch;
STDFLT lasthx,lasthy;

VOID CreateText(text,hx,hy,ang,handle,flipflags,style) 
STDFLT hx,hy,ang;
UBYTE *text;
int handle;
UWORD flipflags;
struct Lettering_Styles *style;
{

	dumphatch();

	if (startord>0&&Current_Pen->PenOrd!=startord) return;
	if (startcar>0&&Current_Pen->Carousel!=startcar) return;
	if (startauto>0&&Current_Pen->AutoNum!=startauto) return;

	if (Current_Layer->Layer_Flags&LAYER_POFF) return;
	if (Current_Group->Group_Flags&GROUP_POFF) return;
	if (Current_Pen->Pen_Flags&PEN_POFF) return;

	DoPen(Current_Pen,FALSE);
	PutText(style,text,hx,hy,ang,flipflags,handle);
}

VOID CreateLine(ix1,iy1,ix2,iy2,fg)
STDFLT ix1,iy1,ix2,iy2;
WORD fg;
{

	checkline(ix1,iy1,ix2,iy2);

	if (startord>0&&Current_Pen->PenOrd!=startord) return;
	if (startcar>0&&Current_Pen->Carousel!=startcar) return;
	if (startauto>0&&Current_Pen->AutoNum!=startauto) return;

	if (Current_Layer->Layer_Flags&LAYER_POFF) return;
	if (Current_Group->Group_Flags&GROUP_POFF) return;
	if (Current_Pen->Pen_Flags&PEN_POFF) return;

	DoPen(Current_Pen,TRUE);

	if (fg&LINE_P1ARROW) DoArrow(ix1,iy1,ix2,iy2);

	dotline(ix1,iy1,ix2,iy2);

	if (fg&LINE_P2ARROW) DoArrow(ix2,iy2,ix1,iy1);

}

VOID CreateEllipse(centx,centy,radx,rady,rot,angs,ange)
STDFLT centx,centy,radx,rady,rot,angs,ange;
{

	checkellipse(centx,centy,radx,rady,rot,angs,ange);

	if (startord>0&&Current_Pen->PenOrd!=startord) return;
	if (startcar>0&&Current_Pen->Carousel!=startcar) return;
	if (startauto>0&&Current_Pen->AutoNum!=startauto) return;

	if (Current_Layer->Layer_Flags&LAYER_POFF) return;
	if (Current_Group->Group_Flags&GROUP_POFF) return;
	if (Current_Pen->Pen_Flags&PEN_POFF) return;

	DoPen(Current_Pen,TRUE);

	FancyEllipseClipper(centx,centy,radx,rady,angs,ange,rot);
}

VOID closehatch() 
{
	dumphatch();
}

VOID CreateHatch(inhatch,putpoint)
O_HATCH *inhatch;
D_NODE *putpoint;
{

	dumphatch();

	if (startord>0&&Current_Pen->PenOrd!=startord) return;
	if (startcar>0&&Current_Pen->Carousel!=startcar) return;
	if (startauto>0&&Current_Pen->AutoNum!=startauto) return;

	if (Current_Layer->Layer_Flags&LAYER_POFF) return;
	if (Current_Group->Group_Flags&GROUP_POFF) return;
	if (Current_Pen->Pen_Flags&PEN_POFF) return;

	DoPen(Current_Pen,FALSE);

	ClipBoundarySetup();
	SeenItem = FALSE;
	HIP = HIP_HUH;

	lasthatch = *inhatch;
	
}

VOID dumphatch()
{
	if (!HIP) return;

	TilePattern(&lasthatch,TRUE);

	HIP = HIP_NONE;
	SeenItem = FALSE;
	
}

VOID checkline(x1,y1,x2,y2)
STDFLT x1,y1,x2,y2;
{

	if (HIP==HIP_NONE) return;
	if (HIP==HIP_ELLIP) {
		dumphatch();
		return;
	}
	if (HIP==HIP_HUH) HIP = HIP_LINE;

	if (!SeenItem) {
		ClipBoundaryMove(x1,y1);
		lasthx = x1;
		lasthy = y1;
		SeenItem = TRUE;
	}
	
	if (lasthx!=x1||lasthy!=y1) {
		dumphatch();
		return;
	}
	
	ClipBoundaryDraw(x2,y2);
	
	lasthx = x2;
	lasthy = y2;
	
	return;
}

VOID checkellipse(centx,centy,radx,rady,bega,enda,rotation)
STDFLT centx,centy,radx,rady,bega,enda,rotation;
{

	STDFLT xn,yn,xn1,yn1,tx1,ty1,difang,txf,tyf;
	WORD qp,ctr,i;
	STDFLT sinth,costh,theta;
	STDFLT csx,rotcos,rotsin,thet1;
	STDFLT deltang;

	if (HIP==HIP_NONE) return;
	if (HIP==HIP_LINE) return;
	if (HIP==HIP_HUH) HIP = HIP_ELLIP;

	csx = max(radx,rady);		/* Simple elimination */
	if (centx+csx<left) return;
	if (centx-csx>right) return;
	if (centy+csx<bottom) return;
	if (centy-csx>top) return;

	rotcos = cos(rotation);
	rotsin = sin(rotation);
	
	bega -= rotation;
	enda -= rotation;
	while (bega<0.0) bega+=TWOPI;
	while (enda<0.0) enda+=TWOPI;

	bega=ConvEllipNotation(bega,radx,rady);
	enda=ConvEllipNotation(enda,radx,rady);

	difang=enda-bega;
	while (difang<0.02) difang+=TWOPI;
	if (difang>TWOPI) difang=TWOPI;

	xn = max(radx,rady) * Current_View->ViewScale; /* Get in plot scale */
	xn /= xpix;
	if (xn<=0) return;		/* Don't divide by zero */

	yn = circstep/(2*xn);
	if (yn>1.0) yn=1.0;
	if (yn<-1.0) yn=-1.0;
	theta = 2 * asin(yn);

	if (theta>(PI/6.0)) theta = PI/6.0;	/* Min effort 12 steps */
	if (theta<(PI/100.0)) theta = PI/100.0;	/* Max effort 400 steps */

	xn = difang/theta;		/* Round to an integer */

	qp = xn;
	xn = qp;
	
	theta=difang/xn;

	sinth=sin(theta);
	costh=cos(theta);

	thet1 = bega;
	xn=cos(thet1)*radx;		/* Calc start */
	yn=sin(thet1)*rady;
	tx1 = ((xn * rotcos) - (yn * rotsin)) + centx; /* Rot ellipse */
	ty1 = ((yn * rotcos) + (xn * rotsin)) + centy;

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

	dumphatch();
}
