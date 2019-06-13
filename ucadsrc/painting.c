/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	painting.c - routines oriented to painting and rendering
 *
 */

/*
 *	Copyright (C) 1986, 1987, 1988 by WURBsoft Inc., and Scott Halbert.
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
 *	1	15-Dec-88 SH	Invention
 *	2	19-May-90 SH	Grid divisions were not rendering properly
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

VOID RepaintWindow(),Setup_Render();
VOID view_setup();

LOCAL USHORT hatpat[] = { 0xCCCC, 0xCCCC, 0x3333, 0x3333 };

VOID SolidHatch(curhatch,drawit,qhat)
D_NODE *curhatch;
BOOL drawit,qhat;
{

	D_NODE *lineloop;
	O_HATCH	*drawhatch;
	O_LINE *drawline,*nextline;
	O_ELLIP *drawellipse;

	drawhatch = GetHatchData(curhatch);

	lineloop = curhatch->node_subsequent;
	if (lineloop==NULL) return;
	
	Setup_Render(curhatch);
	BNDRYOFF(rp);
	
	if (qhat) {
	
		SetBPen(rp,Erased_Color);		

		SetAfPt(rp, &hatpat, 2);

		SetDrMd(rp, JAM2);
	}

	
	if (lineloop->node_id==NODE_ELLIP) {
	
		drawellipse = GetEllipData(lineloop);
	
		HatchedEllipse(drawellipse->x1,drawellipse->y1,
			drawellipse->xradius,drawellipse->yradius,
			drawellipse->fromang,drawellipse->toangle,
			drawellipse->rotation,drawit);

	} else {
	
		if (lineloop->node_id!=NODE_LINE) return;	/* Not a polygon? */
							
		if (!PolySetup(drawit)) return;
	
		drawline = GetLineData(lineloop);
		PolyVertex(drawline->x1,drawline->y1);

		for (; lineloop!=NULL; lineloop=lineloop->node_subsequent) {
			if (lineloop->node_id!=NODE_LINE) break;  /* Not a polygon? */
			nextline = GetLineData(lineloop);
			if ((nextline!=drawline) &&
			    ((drawline->x2!=nextline->x1) ||
			     (drawline->y2!=nextline->y1))) break;
			    
			PolyVertex(nextline->x2,nextline->y2);
			drawline = nextline;
		}
	
		PolyClosure();
		PolyRender();
	}

	SetAfPt(rp,NULL,0);

	if (!drawit) return;	
	
	/* Determine the things onscreen status */
	
	curhatch->node_flags &= (~(NODE_ONSCREEN|NODE_FULLWINDOW));
	
	if (clip_sret==WIND_OFF) return;

	curhatch->node_flags |= NODE_ONSCREEN | NODE_FULLWINDOW;
	if (clip_sret==WIND_CLIP) 	
		curhatch->node_flags &= ~NODE_FULLWINDOW;
		
	return;
}

extern STDFLT boundxmin,boundxmax,boundymin,boundymax;
extern UBYTE Clip_Hatch;

BOOL aborthatch=FALSE;
BOOL PaintFullScreen = FALSE;
int cdown=0;

VOID RenderTile(centx,centy,ox,oy,sc,tsin,tcos,master,drawit)
STDFLT centx,centy,ox,oy,sc,tsin,tcos;
T_HATCH *master;
BOOL drawit;
{

	int numsteps,i,mvs;
	ULONG stage,xt,yt;
	STDFLT scale,orgx,orgy,xf,yf,tx,ty,oldx,oldy;
	ULONG class;
	USHORT code,qual;
	
	if (aborthatch) return;
	
	cdown--;
	if (cdown<0) {
		cdown = 10;
		
		if (!PaintFullScreen&&(message=GetMsg(w->UserPort))) {

			class = message->Class;
			code = message->Code;
			qual = message->Qualifier;
			ReplyMsg(message);
	
			switch (class) {
			case MOUSEBUTTONS:
				if (code==MENUDOWN) aborthatch=TRUE;
				break;
			case RAWKEY:
				if (code==0x45) aborthatch=TRUE;
				break;
			}

			if (aborthatch) {
				DisplayBeep(screen);
				return;
			}
	
		}	
	}
		
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

	aborthatch = FALSE;

	sc = hatcher->pattern_scale;
	rot = hatcher->pattern_rot;
	tcos = cos(rot);
	tsin = sin(rot);

	v_reset();

	master = hatcher->hatch_pattern;

	/* Sort the boundary box corners clockwise */

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
	
/*	v_reset();
	v_move(bx[0],by[0]);
	v_draw(bx[1],by[1]);
	v_draw(bx[2],by[2]);
	v_draw(bx[3],by[3]);
	v_draw(bx[0],by[0]);
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
			if (aborthatch) break;

/*#define DRAWBOUNDERS */

#ifdef DRAWBOUNDERS
			v_reset();
			v_move(centx+bx[0],centy+by[0]);
			v_draw(centx+bx[1],centy+by[1]);
			v_draw(centx+bx[2],centy+by[2]);
			v_draw(centx+bx[3],centy+by[3]);
			v_draw(centx+bx[0],centy+by[0]);
			ltagint(centx+bx[0],centy+by[0],5); 
#endif /* DRAWBOUNDERS */
			
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
				if (aborthatch) break;
#ifdef DRAWBOUNDERS
			v_reset();
			v_move(cx+bx[0],cy+by[0]);
			v_draw(cx+bx[1],cy+by[1]);
			v_draw(cx+bx[2],cy+by[2]);
			v_draw(cx+bx[3],cy+by[3]);
			v_draw(cx+bx[0],cy+by[0]);
			ltagint(cx+bx[0],cy+by[0],5); 
#endif /* DRAWBOUNDERS */
				
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
				if (aborthatch) break;
				
#ifdef DRAWBOUNDERS
			v_reset();
			v_move(cx+bx[0],cy+by[0]);
			v_draw(cx+bx[1],cy+by[1]);
			v_draw(cx+bx[2],cy+by[2]);
			v_draw(cx+bx[3],cy+by[3]);
			v_draw(cx+bx[0],cy+by[0]);
			ltagint(cx+bx[0],cy+by[0],5); 
#endif /* DRAWBOUNDERS */
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
				if (aborthatch) break;
#ifdef DRAWBOUNDERS
			v_reset();
			v_move(cx+bx[0],cy+by[0]);
			v_draw(cx+bx[1],cy+by[1]);
			v_draw(cx+bx[2],cy+by[2]);
			v_draw(cx+bx[3],cy+by[3]);
			v_draw(cx+bx[0],cy+by[0]);
			ltagint(cx+bx[0],cy+by[0],5); 
#endif /* DRAWBOUNDERS */
				
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
				if (aborthatch) break;
					
#ifdef DRAWBOUNDERS
			v_reset();
			v_move(cx+bx[0],cy+by[0]);
			v_draw(cx+bx[1],cy+by[1]);
			v_draw(cx+bx[2],cy+by[2]);
			v_draw(cx+bx[3],cy+by[3]);
			v_draw(cx+bx[0],cy+by[0]);
			ltagint(cx+bx[0],cy+by[0],5); 
#endif /* DRAWBOUNDERS */
				doany = TRUE;
			}

		}
		
		if ((i>=8)&&(doany==FALSE)) break;
		
		centx -= bx[2] - bx[0];
		centy -= by[2] - by[0];
		
	}

				

}

VOID PatternHatch(curhatch,drawit)
D_NODE *curhatch;
BOOL drawit;
{

	D_NODE *lineloop;
	O_HATCH	*drawhatch;
	O_LINE *drawline,*nextline;
	O_ELLIP *drawellipse;
	
	SolidHatch(curhatch,FALSE,FALSE);
	if (clip_sret==WIND_OFF) return;
	
	if (drawit) {
	
		/* Determine the things onscreen status */
	
		curhatch->node_flags &= (~(NODE_ONSCREEN|NODE_FULLWINDOW));
	
		if (clip_sret!=WIND_OFF) {

			curhatch->node_flags |= NODE_ONSCREEN | NODE_FULLWINDOW;
			if (clip_sret==WIND_CLIP) 	
				curhatch->node_flags &= ~NODE_FULLWINDOW;
		}
	}	
	
	drawhatch = GetHatchData(curhatch);

	lineloop = curhatch->node_subsequent;
	if (lineloop==NULL) return;
	
	Setup_Render(curhatch);
	
	if (lineloop->node_id==NODE_ELLIP) {
	
		drawellipse = GetEllipData(lineloop);
	
		FilledEllipse(drawellipse->x1,drawellipse->y1,
			drawellipse->xradius,drawellipse->yradius,
			drawellipse->fromang,drawellipse->toangle,
			drawellipse->rotation,drawit);

		TilePattern(drawhatch,drawit);

	} else {
	
		if (lineloop->node_id!=NODE_LINE) return;	/* Not a polygon? */
							
		ClipBoundarySetup();
	
		drawline = GetLineData(lineloop);
		ClipBoundaryMove(drawline->x1,drawline->y1);

		for (; lineloop!=NULL; lineloop=lineloop->node_subsequent) {
			if (lineloop->node_id!=NODE_LINE) break;  /* Not a polygon? */
			nextline = GetLineData(lineloop);
			if ((nextline!=drawline) &&
			    ((drawline->x2!=nextline->x1) ||
			     (drawline->y2!=nextline->y1))) break;
			    
			ClipBoundaryDraw(nextline->x2,nextline->y2);
			drawline = nextline;
		}

		/* Render the tile pattern across the boundary area */
		/* printf("PatternHatch %s\n",drawhatch->hatch_pattern->Hatch_Name); */

		TilePattern(drawhatch,drawit);

	}

	return;
}

VOID DrawHatch(curhatch,drawit)
D_NODE *curhatch;
BOOL drawit;
{

	O_HATCH	*drawhatch;

	drawhatch = GetHatchData(curhatch);

	switch (drawhatch->hatch_type) {
	
	case HATCH_SOLID:
		SolidHatch(curhatch,drawit,FALSE);
		break;
		
	case HATCH_PATTERN:
		if (Q_Hatch||(!drawit))	SolidHatch(curhatch,drawit,TRUE);
		else			PatternHatch(curhatch,drawit);
		break;
	}
}
		

DrwEllipse(curellip)
D_NODE *curellip;
{

	struct ellip_mode *drawellipse;

	int pen,clipresult;

	drawellipse=GetEllipData(curellip);

	Setup_Render(curellip);

	FancyEllipseClipper(drawellipse->x1,drawellipse->y1,
			drawellipse->xradius,drawellipse->yradius,
			drawellipse->fromang,drawellipse->toangle,
			drawellipse->rotation,TRUE);

	clipresult = EllipseClip;

	curellip->node_flags &= (~(NODE_ONSCREEN|NODE_FULLWINDOW));
	if (clipresult==WIND_OFF) {
		return;
	} 
	curellip->node_flags |= NODE_ONSCREEN;
	if (clipresult!=WIND_CLIP) 	
		curellip->node_flags |= NODE_FULLWINDOW;

}

DrawSegment(curline)
D_NODE *curline;
{

	FAST struct line_mode *drawline;

	FAST STDFLT fx1,fx2,fy1,fy2;

	Setup_Render(curline);

	drawline=GetLineData(curline);

	fx1=drawline->x1;
	fx2=drawline->x2;
	fy1=drawline->y1;
	fy2=drawline->y2;

	curline->node_flags &= (~(NODE_ONSCREEN|NODE_FULLWINDOW));
	
	cdrawline(fx1,fy1,fx2,fy2,TRUE);
	if (clip_sret==WIND_OFF) return;

	curline->node_flags |= NODE_ONSCREEN | NODE_FULLWINDOW;
	if (clip_sret==WIND_CLIP) 	
		curline->node_flags &= ~NODE_FULLWINDOW;

	if (drawline->line_flags&LINE_P1ARROW)
		DoArrow(fx1,fy1,fx2,fy2);
	if (drawline->line_flags&LINE_P2ARROW)
		DoArrow(fx2,fy2,fx1,fy1);

}

DoArrow(Pt1X,Pt1Y,Pt2X,Pt2Y)
FAST STDFLT Pt2X,Pt2Y,Pt1X,Pt1Y;
{

	FAST STDFLT	ang,hyp,ex,ang2,nx,ny,tc;

	v_reset();

	ang = atan2(Pt2Y-Pt1Y,Pt2X-Pt1X);

	tc = cos(Arrow_Angle);			/* 1(45) fix crash on /0 */
	if (tc!=0.0) hyp = Arrow_Len/tc;
	else hyp = 0.0;
	
	ex = sin(Arrow_Angle)*hyp;

	ang2 = ang + Arrow_Angle;

	nx = (cos(ang2)*hyp)+Pt1X;
	ny = (sin(ang2)*hyp)+Pt1Y;

	v_move(Pt1X,Pt1Y);
	v_draw(nx,ny);

	ang2 = ang2 - Arrow_Angle - PID2;

	nx = (cos(ang2)*ex)+nx;
	ny = (sin(ang2)*ex)+ny;

	v_draw(nx,ny);

	ang2 = ang - Arrow_Angle;

	nx = (cos(ang2)*hyp)+Pt1X;
	ny = (sin(ang2)*hyp)+Pt1Y;

	v_move(Pt1X,Pt1Y);
	v_draw(nx,ny);

	ang2 = ang2 + Arrow_Angle + PID2;

	nx = (cos(ang2)*ex)+nx;
	ny = (sin(ang2)*ex)+ny;

	v_draw(nx,ny);

}

LOCAL BOOL last_sel;

UBYTE cbuf[] = { 8, 9, 10, 11, 12, 13, 14, 15,
		 15, 14, 13, 12, 11, 10, 9, 8 };

BOOL selected_clip;

VOID CheckSel()
{

	int i,j;

	if (last_sel!=selected_clip) {
		j = selected_clip?1:0;
		for (i=0; i<16; i++) {
			Color_Buff[i][0][1] = cbuf[i];
			Color_Buff[i][1][1] = 0;
			Color_Buff[i][2][1] = cbuf[i]*j;
		}
		last_sel = selected_clip;
	}
}	
			
BOOL SelectLegal(inlink,inclip,onscreen,scan)
D_NODE *inlink;
BOOL inclip,onscreen,scan;
{

	if (inlink==NULL) return(FALSE);
	if (inclip&&(!(inlink->node_flags&NODE_INCLIP))) return(FALSE);
	if (onscreen&&(!(inlink->node_flags&NODE_ONSCREEN))) return(FALSE);
	if (Library_Mode&&(!inlink->above_group->cursel)) return(FALSE);
	if (inlink->above_group->Group_Flags&GROUP_DOFF) return(FALSE);
	if (inlink->node_layer->Layer_Flags&LAYER_DOFF) return(FALSE);
	if (inlink->pen_attach->Pen_Flags&PEN_DOFF) return(FALSE);
	if (scan) {
		if (Scan_Clip&&(!(inlink->node_flags&NODE_INCLIP))) return(FALSE);
		if (Scan_Group&&inlink->above_group!=Current_Group) return(FALSE);
		if (Scan_Layer&&inlink->node_layer!=Current_Layer) return(FALSE);
		if (Scan_Pen&&inlink->pen_attach!=Current_Pen) return(FALSE);
		if (Scan_Text) {
			if (inlink->node_id!=NODE_TEXT) return(FALSE);
			if (GetTextData(inlink)->letstyle!=Current_Style)
				return(FALSE);
		}
	}

	return(TRUE);
}

VOID GenericRender(innode)
D_NODE *innode;
{

	remshadows();

	if (innode->node_flags&NODE_SELECTED) {
	    	if (innode->node_flags&NODE_INCLIP) 
			selected_clip=TRUE;
		else 	selected_clip=FALSE;
	}

	if (innode->above_group->Group_Flags&GROUP_DOFF) return;
	if (innode->node_layer->Layer_Flags&LAYER_DOFF) return;
	if (innode->pen_attach->Pen_Flags&PEN_DOFF) return;

	v_reset();

	switch (innode->node_id) {

	case NODE_LINE: 
		DrawSegment(innode);
		break;
	case NODE_ELLIP: 
		DrwEllipse(innode);
		break;
	case NODE_TEXT:
		PutText(innode,TRUE);
		break;
	case NODE_HATCH:
		DrawHatch(innode,TRUE);
		break;
	}

	CheckSel();
	return;
}

BOOL MapWorldToScreen(outx,outy,fltinx,fltiny)
WORD *outx,*outy;
STDFLT fltinx,fltiny;
{
	STDFLT a;

	fltinx-=xviewbegin;
	fltiny-=yviewbegin;

	a=fltinx/xpixsize;
	*outx=(short) a;
	a=fltiny/ypixsize;
	*outy=YWidth - ((short) a);

}

VOID DoGrids()
{

	STDFLT 	x,y,t1,t2,xg,yg,yi,xi,tx,ty,yb,xb;
	WORD	xd,yd,sx,sy;
	int	i;

	if (!Grid_On) return;

	if (xgriddiv<1) xgriddiv = 1;
	if (ygriddiv<1) ygriddiv = 1;

	xd = xgriddiv;
	yd = ygriddiv;

	xg = caltsnap(xpixsize*5.0,xgrid);
	yg = caltsnap(ypixsize*5.0,ygrid);

	if (xg!=xgrid) xd = 1;
	if (yg!=ygrid) yd = 1;
	
	if ((xg/((STDFLT) xd))<xpixsize*3.0) xd = 1;
	if ((yg/((STDFLT) yd))<ypixsize*3.0) yd = 1;

	SetAPen(rp,Grid_Color);
	SetBPen(rp,Erased_Color);
	SetDrMd(rp,JAM1);
	SetDrPt(rp, 0x5555);		/* Dotted line */

	x = xviewbegin;
	xb = (floor((x/xg)-.5) * xg)+xgridoff;
	t1 = yviewbegin;
	t2 = yviewend;
	if (t1<yviewMIN) t1=yviewMIN;
	if (t2>yviewMAX) t2=yviewMAX;


	SetAPen(rp,1);
	for (i=0, x=xb; x<=xviewend; x+=xg/((STDFLT) xd), i++) {
		if (x<xviewMIN) continue;
		if (x>xviewMAX) break;
		if (i%xd!=0) {
			v_move(x,t1);
			v_draw(x,t2);
		}
	}

	SetAPen(rp,Grid_Color);
	for (x=xb; x<=xviewend; x+=xg) {
		if (x<xviewMIN) continue;
		if (x>xviewMAX) break;
		v_move(x,t1);
		v_draw(x,t2);
	}

	y = yviewbegin;
	yb = (floor((y/yg)-.5) * yg)+ygridoff;
	t1 = xviewbegin;
	t2 = xviewend;
	if (t1<xviewMIN) t1=xviewMIN;
	if (t2>xviewMAX) t2=xviewMAX;

	SetAPen(rp,1);
	i = 0;
	for (i=0, y=yb; y<=yviewend; y+=yg/((STDFLT) yd), i++) {
		if (y<yviewMIN) continue;
		if (y>yviewMAX) break;
		if (i%yd!=0) {
			v_move(t1,y);
			v_draw(t2,y);
		}
	}

	SetAPen(rp,Grid_Color);
	for (y=yb; y<=yviewend; y+=yg) {
		if (y<yviewMIN) continue;
		if (y>yviewMAX) break;
		v_move(t1,y);
		v_draw(t2,y);
	}
	
}	

/* LOCAL USHORT fillpat[] = { 0x8888, 0x0000, 0x2222, 0x0000 }; */

/* LOCAL USHORT fillpat[] = {
 0x0303, 0x0707, 0x0e0e, 0x0c0c, 
 0xc0c0, 0xe0e0, 0x7070, 0x3030 }; */
 
LOCAL USHORT fillpat[] = { 0x1111, 0x2222, 0x8888, 0x4444 };

extern WORD keepx,keepy;

VOID RepaintWindow()
{

	BOOL on;
	FAST D_NODE *lnklst;
	FAST struct line_mode *drawline;
	FAST struct ellip_mode *drawellip;
	ULONG class;
	USHORT code,qual;

	WORD x1,y1,x2,y2;
	STDFLT fx1,fx2,fy1,fy2;

	if (!Win_Setup) FinWindow();

	view_setup();
	Line_Normal();

	if (CleanupShadows!=NULL) (*CleanupShadows)();
	remshadows();
	AdjustPots();
 
	SetDrPt(rp, 0xFFFF);

	if (!PaintFullScreen) {

		SetAPen(rp,Mist_Color);
		SetBPen(rp,Erased_Color);		
		SetOPen(rp,Mist_Color);

		SetAfPt(rp, &fillpat, 2);

		SetDrMd(rp, JAM2);
		
	} else {
	
		SetAPen(rp,Erased_Color);
		SetBPen(rp,Erased_Color);
		SetOPen(rp,Erased_Color);
		
		SetDrMd(rp, JAM1);
		SetAfPt(rp,NULL,0);
	
	}

	RectFill(rp,XBegin,YBegin,XBegin+XWidth,YBegin+YWidth);

	on = TRUE;
	if (xviewMAX<xviewbegin) on = FALSE;
	if (yviewMAX<yviewbegin) on = FALSE;
	if (xviewMIN>xviewbegin+xviewsize) on = FALSE;
	if (yviewMIN>yviewbegin+yviewsize) on = FALSE;

	if (on) {

		fx1=xviewbegin;
		if (fx1<xviewMIN) fx1=xviewMIN;
		fx2=xviewbegin+xviewsize;
		if (fx2>xviewMAX) fx2=xviewMAX;

		fy1=yviewbegin;
		if (fy1<yviewMIN) fy1=yviewMIN;
		fy2=yviewbegin+yviewsize;
		if (fy2>yviewMAX) fy2=yviewMAX;

		MapWorldToScreen(&x1,&y1,fx1,fy1);
		MapWorldToScreen(&x2,&y2,fx2,fy2);
	
		SetAfPt(rp,NULL,0);
		GI_RectFill(Erased_Color,XBegin+x1,YBegin+y2,XBegin+x2,YBegin+y1);
	}

	if (!PaintFullScreen) DoGrids();	/* No grids except for ucad */

	SetAPen(rp,Bounds_Color);
	SetBPen(rp,Erased_Color);
	SetDrMd(rp,JAM1);
	SetDrPt(rp, 0xFFFF);

	if (!PaintFullScreen) {

		v_reset();
		v_move(xviewMIN,yviewMIN);
		v_draw(xviewMIN,yviewMAX);
		v_draw(xviewMAX,yviewMAX);
		v_draw(xviewMAX,yviewMIN);
		v_draw(xviewMIN,yviewMIN);
		
	}

#define DEB(a) 

	DEB("Repaint: ")
			

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
		if (Library_Mode&&(!lnklst->above_group->cursel)) continue;
		if (lnklst->above_group->Group_Flags&GROUP_DOFF) continue;
		if (lnklst->node_layer->Layer_Flags&LAYER_DOFF) continue;
		if (lnklst->pen_attach->Pen_Flags&PEN_DOFF) continue;

		if (!PaintFullScreen&&(message=GetMsg(w->UserPort))) {

			class = message->Class;
			code = message->Code;
			qual = message->Qualifier;
			ReplyMsg(message);
	
			switch (class) {
			case MOUSEBUTTONS:
				if (code==MENUDOWN) lnklst=NULL;
				break;
			case RAWKEY:
				if (code==0x45) lnklst=NULL;
				break;
			}
		}

		if (lnklst==NULL) {
			DisplayBeep(screen);
			break;
		}

		Setup_Render(lnklst);

		lnklst->node_flags &= (~(NODE_ONSCREEN|NODE_FULLWINDOW));

		switch (lnklst->node_id) {

		case NODE_LINE: 
			DEB("L ")
			drawline=GetLineData(lnklst);

			cdrawline(drawline->x1,drawline->y1,
				  drawline->x2,drawline->y2,TRUE);
			if (clip_sret!=WIND_OFF) {
				lnklst->node_flags |= 
					NODE_ONSCREEN | NODE_FULLWINDOW;
				if (clip_sret==WIND_CLIP) 	
					lnklst->node_flags &= ~NODE_FULLWINDOW;

				if (drawline->line_flags&LINE_P1ARROW)
					DoArrow(drawline->x1,drawline->y1,
						  drawline->x2,drawline->y2);
				if (drawline->line_flags&LINE_P2ARROW)
					DoArrow(drawline->x2,drawline->y2,
						  drawline->x1,drawline->y1);
			}
			break;
		case NODE_ELLIP: 
			DEB("E ")
			DrwEllipse(lnklst);
			break;
		case NODE_TEXT:
			DEB("T ")
			PutText(lnklst,TRUE);
			break;
		case NODE_HATCH:
			DEB("H ")
			DrawHatch(lnklst,TRUE);
		}
	
	}

	DEB("\n")

	UpdateTitle();
	CheckSel();

	GI_UpdateAllGadgets(w);
	v_reset();
	
	keepx = keepy = 0;	/* Make pointers.c redo crosshairs */

}

extern struct Image uueaerbbrush_image;

VOID zapwindow()
{

	GI_RectFill(3,0,TOPMARGIN,LEFTAREA,YSsize);
	DrawImage(rp, &uueaerbbrush_image, 1,15);

	UpdateCore();

}

UWORD lastpat=1243;

VOID Setup_Render(curitem)
FAST D_NODE *curitem;
{

	UWORD usepat;
	BOOL ghsted=FALSE;

	SetDrMd(rp, JAM1);
	usepat = curitem->pen_attach->Line_Pattern;
	
	if (curitem->above_group->Group_Flags&GROUP_GHOSTED||
	    curitem->node_layer->Layer_Flags&LAYER_GHOSTED||
	    curitem->pen_attach->Pen_Flags&PEN_GHOSTED) {
	    	usepat = 0x2452;
		ghsted = TRUE;
	}
	    
	if (usepat!=lastpat) {
		if (usepat==0) 
			usepat = curitem->pen_attach->Line_Pattern = 0xFFFF;
				/* They didn't really mean it - fix it */

		SetDrPt(rp, usepat); 
		
		lastpat = usepat;
	}

	if (curitem->node_flags&
		(NODE_DELETED|NODE_SELECTED|NODE_INCLIP)||ghsted) {

		if (curitem->node_flags&NODE_DELETED) {
			SetAPen(rp, Erased_Color);
			return;
		}

		if (curitem->node_flags&NODE_SELECTED) {
			selected_clip=FALSE;
			if (curitem->node_flags&NODE_INCLIP) 
				selected_clip = TRUE;
			SetAPen(rp, SelectedColor);
			return;
		}

		if (curitem->node_flags&NODE_INCLIP) {
			SetAPen(rp, ClippedColor);
			return;
		}
		
		if (ghsted) {
			SetAPen(rp, Ghost_Color);  /* Use the default grey */
			return;
		}
	}

	switch (Color_Source) {
	case 2:
		SetAPen(rp,curitem->node_layer->Color_ID);
		return;
	case 3:
		SetAPen(rp,curitem->pen_attach->Color_ID);
		return;
	case 1:
		SetAPen(rp,curitem->above_group->Color_ID);
		return;
	default:
		SetAPen(rp,curitem->node_layer->Color_ID);
		Color_Source = 2;	/* Set to layer */
		return;
	}

	return;

}

/* caltsnap - calculate a temporary snap value for a given axis such that
   the snap is greater than or equal to a pixel size, but not less, multiply
   by 2, 5 then 10 repeatedly until the snap fits */

STDFLT caltsnap(pixsize,insnap)
STDFLT pixsize,insnap;
{

	STDFLT newtsnap;

	newtsnap=insnap;
	
	while (pixsize>newtsnap) {
		if (newtsnap*2.0>=pixsize) {
			newtsnap*=2.0;
			break;
		}
		if (lintype==LIN_EDI||
		    lintype==LIN_EFI||
		    lintype==LIN_DF||
		    lintype==LIN_DI) {
			newtsnap*=2.0;
			continue;
		}

		if (newtsnap*5.0>=pixsize) {
			newtsnap*=5.0;
			break;
		}
		newtsnap*=10.0;
	}

	return(newtsnap);
}

view_defaults()
{

	fullzoomout();

}

/* view_setup - compute the criterion of how the user's coordinate grid
   will fit onto the pixels of the screen.  Calculates the snap arrays,
   so that grid conversions will be looked up from a table instead of
   calculated repeatedly so that the mouse will cook along without delay */

VOID view_setup()
{

	xpixsize = xviewsize/((STDFLT) XWidth);
	ypixsize = xpixsize * aspect / skew;

	yviewsize = ypixsize * ((STDFLT) YWidth);

	xtsnap=caltsnap(xpixsize,xsnap);

	ytsnap=caltsnap(ypixsize,ysnap);

}

fullzoomout()
{

	STDFLT	borderx,centerx,bordery,centery,scaler;
	STDFLT	screenx,screeny;

	borderx=xviewMAX-xviewMIN;
	centerx=xviewMIN+(borderx/2.0);

	bordery=yviewMAX-yviewMIN;
	centery=yviewMIN+(bordery/2.0);

	screenx=XWidth;
	screeny=YWidth;
	screeny=screeny*aspect/skew;

	scaler=screenx/screeny;

	if ((borderx/bordery)<scaler) {		/* Narrower than screen */
		borderx=bordery*scaler;
	} else {
		bordery=borderx/scaler;
	}

	borderx*=1.02;
	bordery*=1.02;

	xviewbegin=centerx-(borderx/2.0);
	yviewbegin=centery-(bordery/2.0);

	xviewsize=borderx;
	yviewsize=bordery;

	view_setup();

}

