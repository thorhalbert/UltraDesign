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
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "plotsetup.h"
#include "mempools.h"
#include <libraries/dos.h>
#include <graphics/gfxmacros.h>

/*----------------------------------Macros-----------------------------------*/

#define vw_reset()
#define vw_move(x,y) lastx = x, lasty = y
#define vw_draw(x,y) viewline(lastx,lasty,x,y), lastx=x, lasty=y

#define FAST register

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/


#define POLYPIECE 512
struct polysto {
	struct	polysto	*next;
	UWORD	inhabitants;
	UBYTE	flags[POLYPIECE];
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
	
	if (rootpoly==NULL) rootpoly = newbuff;
	else tailpoly->next = newbuff;
	
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
	
	currentpoly->flags[i] = 0x01;	/* Begin segment */
	currentpoly->x[i] = x;
	currentpoly->y[i] = y;
	
	currentpoly->inhabitants = i+1;
}

VOID ClipBoundaryDraw(x,y)
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
	
	currentpoly->flags[i] = 0x00;	/* Continue segment */
	currentpoly->x[i] = x;
	currentpoly->y[i] = y;
	
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
	vw_move(intx-t,inty-t);
	vw_draw(intx+t,inty+t);
	vw_move(intx+t,inty-t);
	vw_draw(intx-t,inty+t);
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

	FAST struct polysto *looper;
	FAST UWORD i,count;
	
	STDFLT cx2,cy2,bx1,by1,bx2,by2,gx,gy;
	
	for (looper=begin; looper!=NULL; looper=looper->next,start=0) {
	    for (i=start; i<looper->inhabitants; i++,cx1=cx2,cy1=cy2) {
		if (looper->flags[i]==0x01) {
			cx2 = looper->x[i];
			cy2 = looper->y[i];
			continue;
		}
		
		cx2 = looper->x[i];
		cy2 = looper->y[i];

		if (min(y1,y2)>max(cy1,cy2)) continue;	/* Line above seg */
		if (max(y1,y2)<min(cy1,cy2)) continue;	/* Line below seg */
		if (min(x1,x2)>max(cx1,cx2)) continue;	/* Line right seg */
		if (max(x1,x2)<min(cx1,cx2)) continue;	/* Line left seg */

		if (!CalcIntercept(x1,y1,x2,y2,cx1,cy1,cx2,cy2,&gx,&gy))
				continue; 	/* Does not intercept */

		/* ltagint(gx,gy,5); */

		if (!LineSegment(gx,gy,x1,y1,x2,y2,cx1,cy1,cx2,cy2)) continue;
		
		/* ltagint(gx,gy,4); */

		/* SetAPen(rp,6);
		vw_reset(); vw_move(x1,y1); vw_draw(gx,gy); */
		DrawBegin(x1,y1,gx,gy,looper,i+1,drawit,cx2,cy2);

		/* SetAPen(rp,7);
		vw_reset(); vw_move(x2,y2); vw_draw(gx,gy); */
		DrawBegin(gx,gy,x2,y2,looper,i+1,drawit,cx2,cy2); /* Go Recursive */
		
		return;
	    }
	}	
	
	/* Now need to see if segment is visible */
	
	bx1 = x1; by1 = y1; bx2 = x2; by2 = y2;
	
	cx1 = (x2+x1) / 2.0;	/* Calculate line midpoint */
	cy1 = (y2+y1) / 2.0;

	x1 = cx1;
	y1 = cy1;
	x2 = cx1;
	y2 = boundymax*2.0;
	
	count = 0;
	
	for (looper=rootpoly; looper!=NULL; looper=looper->next) {
	    for (i=0; i<looper->inhabitants; i++,cx1=cx2,cy1=cy2) {
		if (looper->flags[i]==0x01) {
			cx2 = looper->x[i];
			cy2 = looper->y[i];
			continue;
		}
		
		cx2 = looper->x[i];
		cy2 = looper->y[i];
#ifdef LINECHECK		
		if (min(y1,y2)>max(cy1,cy2)) continue;	/* Line above seg */
		if (max(y1,y2)<min(cy1,cy2)) continue;	/* Line below seg */
		if (min(x1,x2)>max(cx1,cx2)) continue;	/* Line right seg */
		if (max(x1,x2)<min(cx1,cx2)) continue;	/* Line left seg */
#endif
		if (!CalcIntercept(x1,y1,x2,y2,cx1,cy1,cx2,cy2,&gx,&gy))
				continue;	/* Does not intercept */
		
		if (LineSegment(gx,gy,x1,y1,x2,y2,cx1,cy1,cx2,cy2)) count++;
				
	    }
	}

	/* SetAPen(rp,9); */
	
	if (count&0x01)				/* Even count */
		viewline(bx1,by1,bx2,by2,drawit);
	
}

VOID DrawClip(x1,y1,x2,y2,drawit)
STDFLT x1,y1,x2,y2;
BOOL drawit;
{

	if (max(x1,x2)<boundxmin) return;	/* Eliminate if not in field */
	if (min(x1,x2)>boundxmax) return;
	if (max(y1,y2)<boundymin) return;
	if (min(y1,y2)>boundymax) return;

	DrawBegin(x1,y1,x2,y2,rootpoly,0,drawit,0.0,0.0);

}

VOID RenderTile(centx,centy,ox,oy,sc,tsin,tcos,master,drawit)
STDFLT centx,centy,ox,oy,sc,tsin,tcos;
T_HATCH *master;
BOOL drawit;
{

	int numsteps,i,mvs;
	ULONG stage,xt,yt;
	STDFLT scale,orgx,orgy,xf,yf,tx,ty,oldx,oldy;
	
	scale = PORTtoF(&(master->Hatch_Scale));
	orgx = PORTtoF(&(master->BaseX));
	orgy = PORTtoF(&(master->BaseY));

	numsteps = master->Pattern_Size / (sizeof(ULONG));
	
	mvs = 0;
/*	printf("Steps=%d ",numsteps); */
	
	for (i=0; i<numsteps; i++) {
		
		stage = master->Pattern[i];
		/* printf("%08x ",stage); */ 
		
		xt = (stage>>15)&0x7FFF;
		yt = stage&0x7FFF;
		
		xf = xt;
		yf = yt;
	
		/* printf("%d %d ",xt,yt); */
				
		xf /= 32767.0;
		yf /= 32767.0;
		
		xf *= scale;
		yf *= scale;
		
		xf += orgx;		/* Cvt to org coord system */
		yf += orgy;
		
		xf -= ox;		/* Cvt to prime point coord system */
		yf -= oy;
		
		tx = xf*tcos-yf*tsin;	/* Rotate to tile coord system */
		ty = xf*tsin+yf*tcos;
		
		tx *= sc;		/* Scale to tile system */
		ty *= sc;
		
		tx += centx;		/* Position tile */
		ty += centy;
		
		if (!(stage&(1<<31))) DrawClip(oldx,oldy,tx,ty,drawit);
			
		oldx = tx;
		oldy = ty;
		
	}
	
	/* printf(" Moves=%d\n",mvs); */

}

T_HATCH *defhatch=NULL;

VOID loaddefhatch()
{

	UBYTE nameagg[100];
	
	if (Fill_Hatch_File[0]=='\0') strcpy(Fill_Hatch_File,"Hatch_File_One");
	if (Fill_Hatch_Name[0]=='\0') strcpy(Fill_Hatch_Name,"Grid");

	defhatch = Hatch_Search_File(Fill_Hatch_File);

	if (defhatch==NULL)
		(VOID) LoadHatch(Fill_Hatch_File);

	sprintf(nameagg,"%s&%s",Fill_Hatch_File,
				Fill_Hatch_Name);

	defhatch = Hatch_Search(nameagg);

}
		
VOID TilePattern(hatcher,drawit)
O_HATCH *hatcher;
BOOL drawit;
{

	STDFLT bx[4],by[4],tang[4],sc,rot,a1,a2;
	STDFLT tx[4],ty[4],ox,oy;
	STDFLT tcos,tsin,centx,centy,csx;
	int oorder[4],op,i,j,k;
	STDFLT cx,cy,a,b,m,c1,c2,im,mxx,mxy,mnx,mny;
	T_HATCH *master;
	BOOL doany;

	sc = hatcher->pattern_scale;
	rot = hatcher->pattern_rot;
	tcos = cos(rot);
	tsin = sin(rot);

	vw_reset();

	master = hatcher->hatch_pattern;
	if (master==NULL) {
		/*sc = 1.0/Current_View->ViewScale;
		sc *= Fill_Hatch_Scale;*/
		sc = Fill_Hatch_Scale;
		rot = 0.0;
		if (defhatch==NULL) loaddefhatch();
		master = defhatch;
	}
	
	if (master==NULL) return;		/* Cannot live with this */

	/* Sort the boundary box corners clockwise */

	tcos = cos(rot);
	tsin = sin(rot);
	
	op = 0;

#ifdef RIGHT_SIDE_UP
	cx = cy = 0.0;	
	for (i=0; i<4; i++) {
		tx[i] = PORTtoF(&(master->PGramX[i]));
		ty[i] = PORTtoF(&(master->PGramY[i]));

		bx[i] = tx[i]*tcos-ty[i]*tsin;
		by[i] = tx[i]*tsin+ty[i]*tcos;
		
		cx += bx[i];
		cy += by[i];
		oorder[i] = i;
	}
	
	cx /= 4.0;
	cy /= 4.0;		/* Calc center of p-gram */
	
	for (i=0; i<4; i++)
		tang[i] = atan2(by[i]-cy, bx[i]-cx);
	
	/* A quick bubble sort on these - yuck! but only for 4? */	
	
	for (i=0; i<4; i++) 
	  for (j=0; j<3; j++) {
	  	if (tang[j]<tang[j+1]) {
			k = oorder[j];			/* Twiddle orders */
			oorder[j] = oorder[j+1];
			oorder[j+1] = k;
			
			b = tang[j];
			tang[j] = tang[j+1];
			tang[j+1] = b;
		}
	  }
	
	/* Twiddle the original coordinates */
	
	for (i=0; i<4; i++)	tx[i] = bx[oorder[i]];
	for (i=0; i<4; i++)	ty[i] = by[oorder[i]];

	op = 0;
	ox = MAXBIG;
	for (i=0; i<4; i++) 	
		if (tx[i]<ox) {
			ox = tx[i];
			op = i;
		}
		
/*	i = op-1;
	if (i<0) i = 3;
	
	if (tx[i]<tx[op]) op = i;
	
	j = i-1;
	if (i<0) i = 3;
	
	if ((ty[j]==ty[i])||(tx[j]<tx[i])) op = i;	*/

#endif
			
	ox = PORTtoF(&(master->PGramX[op]));
	oy = PORTtoF(&(master->PGramY[op]));

	for (i=0; i<4; i++) {
		tx[i] = PORTtoF(&(master->PGramX[i])) - ox;
		ty[i] = PORTtoF(&(master->PGramY[i])) - oy;
		tx[i] *= sc;
		ty[i] *= sc;

		bx[i] = tx[i]*tcos-ty[i]*tsin;
		by[i] = tx[i]*tsin+ty[i]*tcos;
	}		
/*
	for (i=0; i<4; i++) {
		j = op+i;
		if (j>3) j-=4;
		tx[i] = bx[j];
		ty[i] = by[j];
	}
	
	for (i=0; i<4; i++) {
		bx[i] = tx[i];
		by[i] = ty[i];
	}
*/
	
/*	vw_reset();
	vw_move(bx[0],by[0]);
	vw_draw(bx[1],by[1]);
	vw_draw(bx[2],by[2]);
	vw_draw(bx[3],by[3]);
	vw_draw(bx[0],by[0]);
	ltagint(bx[0],by[0],3);	

	for (i=0; i<4; i++) printf("%d %f %f\n",i,bx[i],by[i]); */

	mxx = max(max(bx[0],bx[1]),max(bx[2],bx[3]));
	mnx = min(min(bx[0],bx[1]),min(bx[2],bx[3]));
	mxy = max(max(by[0],by[1]),max(by[2],by[3]));
	mny = min(min(by[0],by[1]),min(by[2],by[3]));

	centx = boundxmin;
	centy = boundymin;

	/* Find y step */
	
	a1 = a2 = MAXBIG;
	
	if ((bx[0]-bx[1])!=0.0) {	/* Not vertical */
	
		m = (by[0]-by[1])/(bx[0]-bx[1]);	/* Calc slope */
		b = by[0] - m*bx[0];			/* Cal Y intercept */
		
		c1 = b;
		
		b = by[2] - m*bx[2];			/* Cal 2nd Y int */
		
		c2 = b;
		
		b = fabs(c1 - c2);
		
		/* printf("Y 1 = %f\n",b); */
		
		if (b>0.0) a1 = b;
		
		
	}
	
	if ((bx[1]-bx[2])!=0.0) {	/* Not vertical */

		
		m = (by[1]-by[2])/(bx[1]-bx[2]);	/* Calc slope */
		b = by[1] - m*bx[1];			/* Cal Y intercept */
		
		c1 = b;
		
		b = by[3] - m*bx[3];			/* Cal 2nd Y int */
		
		c2 = b;
		
		b = fabs(c1 - c2);

		/* printf("Y 2 = %f\n",b); */
		
		if (b>0.0) a2 = b;
		
	}

	b = min(a1,a2);
	
	centy = floor(centy/b) * b;	/* Snap to b */
	
	/* Snap the prime point to a 0,0 aligned point */
			
	/* Find X Step */
	
	a1 = a2 = MAXBIG;
	
	if ((by[0]-by[1])!=0.0) {	/* Not horizontal */
	
		im = (bx[0]-bx[1])/(by[0]-by[1]);	/* Calc slope */
		a = bx[0] - im*by[0];			/* Cal Y intercept */
		
		c1 = a;
		
		a = bx[2] - im*by[2];			/* Cal 2nd Y int */
		
		c2 = a;
		
		b = fabs(c1 - c2);
		
		/* printf("X 1 = %f\n",b); */
		
		if (b>0.0) a1 = b;


	}
	
	if ((by[1]-by[2])!=0.0) {
				
		im = (bx[1]-bx[2])/(by[1]-by[2]);	/* Calc slope */
		a = bx[1] - im*by[1];			/* Cal Y intercept */
		
		c1 = a;
		
		a = bx[3] - im*by[3];			/* Cal 2nd Y int */
		
		c2 = a;
		
		b = fabs(c1 - c2);
		
		/* printf("X 2 = %f\n",b); */

		if (b>0.0) a2 = b;
		
	}

	b = min(a1,a2);
	
	centx = floor(centx/b) * b;	/* Snap to b */
		
	/* printf("%f %f\n",centx,centy);
	ltagint(centx,centy,4); */
			
	/* Time for the infinately radiating rectangle */
	
	for (i=0; ; i+=2) {

		doany = FALSE;
		
		if (centx+mxx>=boundxmin&&
		    centx+mnx<=boundxmax&&
		    centy+mxy>=boundymin&&
		    centy+mny<=boundymax) {

			RenderTile(centx,centy,ox,oy,sc,tsin,tcos,master,drawit);

			/* vw_reset();
			vw_move(centx+bx[0],centy+by[0]);
			vw_draw(centx+bx[1],centy+by[1]);
			vw_draw(centx+bx[2],centy+by[2]);
			vw_draw(centx+bx[3],centy+by[3]);
			vw_draw(centx+bx[0],centy+by[0]);
			ltagint(centx+bx[0],centy+by[0],5); */
			
			doany = TRUE;
		}
		
		cx = centx; cy = centy;
		
		for (j=0; j<i; j++) {		/* Walk up */
			cx += bx[1] - bx[0];
			cy += by[1] - by[0];
			
			if (cx+mxx>=boundxmin&&
			    cx+mnx<=boundxmax&&
			    cy+mxy>=boundymin&&
			    cy+mny<=boundymax) {
				
				RenderTile(cx,cy,ox,oy,sc,tsin,tcos,master,drawit);
				
				doany = TRUE;
			}
			
		}
		
		for (j=0; j<i; j++) {		/* Walk right */
			cx += bx[2] - bx[1];
			cy += by[2] - by[1];
			
			if (cx+mxx>=boundxmin&&
			    cx+mnx<=boundxmax&&
			    cy+mxy>=boundymin&&
			    cy+mny<=boundymax) {
			    
				RenderTile(cx,cy,ox,oy,sc,tsin,tcos,master,drawit);
				
				doany = TRUE;
			}
			
		}
		
		for (j=0; j<i; j++) {		/* Walk down */
			cx += bx[0] - bx[1];
			cy += by[0] - by[1];
			
			if (cx+mxx>=boundxmin&&
			    cx+mnx<=boundxmax&&
			    cy+mxy>=boundymin&&
			    cy+mny<=boundymax) {
				
				RenderTile(cx,cy,ox,oy,sc,tsin,tcos,master,drawit);
				
				doany = TRUE;
			}

		}
		
		for (j=0; j<i-1; j++) {		/* Walk left */
			cx += bx[1] - bx[2];
			cy += by[1] - by[2];
			
			if (cx+mxx>=boundxmin&&
			    cx+mnx<=boundxmax&&
			    cy+mxy>=boundymin&&
			    cy+mny<=boundymax) {
				
				RenderTile(cx,cy,ox,oy,sc,tsin,tcos,master,drawit);
				
				doany = TRUE;
			}

		}
		
		if ((i>=8)&&(doany==FALSE)) break;
		
		centx -= bx[2] - bx[0];
		centy -= by[2] - by[0];
		
	}

				

}
