/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	fancystartup.c - display nice intro page
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
 *	1	24-Jul-87 SH	Put template in
 *	2	 6-Sep-87 SH	Put Uueaerb Star In - Redo Text
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include "fonts.h"
#include "attributes.h"
#include <graphics/gfxmacros.h>

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

#define FDraw(x,y) Draw(rp,(int)(x),(int)((y)/spct))
#define FMove(x,y) Move(rp,(int)(x),(int)((y)/spct))

STDFLT spct = 1.14285;		/* Amiga Hires screen aspect magic number */
extern struct IntuiMessage *message;

VOID fancystartup()
{
	UBYTE outbuf[80];
	STDFLT x,y,t,ta,xn,yn;
	int tc;
	int class;

	SetDrMd(rp, JAM1);
	SetAPen(rp, 2);
	SetBPen(rp, 2);
	SetOPen(rp, 2);

	Move(rp, 0, 0);
	Draw(rp, 639,0);
	Draw(rp, 639,399);
	Draw(rp, 0,399);
	Draw(rp, 0,0);

	ScreenToFront(screen);

	for (tc=0; tc<7; tc++) {
		ta = PID2 + ((TWOPI/7.0)*((STDFLT) tc));
		x = (cos(ta) * 23.4);
		y = (sin(ta) * 23.4);
		FMove(x+90.0, y+100.0);
		t = ta + (TWOPI/14.0);
		x = (cos(t) * 66.5);
		y = (sin(t) * 66.5);
		FDraw(x+90.0, y+100.0);
		t += (TWOPI/14.0);
		x = (cos(t) * 23.4);
		y = (sin(t) * 23.4);
		FDraw(x+90.0, y+100.0);

		t = ta + (TWOPI/7.0);
		xn = (cos(t) * 53.7)+90.0;
		yn = (sin(t) * 53.7)+100.0;

		DoCirc(xn,yn,13.4,60.0);

	}

	DoCirc(90.0,100.0,75.0,120.0);
/*	DoCirc(90.0,100.0,77.0,120.0); 	*/

 	fnc_text(395.0,260.0,50.0,0.3,VERNAME);

#ifdef FLOODING
	for (tc=0; tc<7; tc++) {
		ta = PID2 + ((TWOPI/7.0)*((STDFLT) tc));

		t = ta + (TWOPI/7.0);
		xn = (cos(t) * 53.7)+90.0;
		yn = (sin(t) * 53.7)+100.0;
		yn /= spct;

		Flood(rp,0,(int) xn, (int) yn);	/* Flood the circles */
		
	}

	Flood(rp,0,90,(int)(100.0/spct));		/* Flood star */
#endif

	Current_Style->draw_font = Root_Font->next_font; /* Simplex1 */

#ifdef VERPRINT
#ifdef DEMO
	sprintf(&outbuf,"Demo Release %s",VERNUM);
#else
	sprintf(&outbuf,"Final Test Release %s",VERNUM);
#endif
	fnc_text(320.0,200.0,22.0,0.3,&outbuf);
#endif

	fnc_text(320.0,200.0,16.0,0.3,
		"\"Thought into Action... At the Speed of Light\"");

	fnc_text(320.0,160.0,18.0,0.0,
		"Copyright (C) 1990 - WURBsoft Inc");

	fnc_text(320.0,120.0,16.0,0.3,
		"\"A Program Worth Having is Worth Buying\"");

#ifdef TESTRELEASE
				
	fnc_text(320.0,80.0,18.0,0.0,
		"TEST VERSION ONLY -- DO NOT RELEASE!");

	fnc_text(320.0,50.0,18.0,0.0,
		"TEST VERSION ONLY -- DO NOT RELEASE!");

#else

	fnc_text(320.0,80.0,18.0,0.0,
		"Published by");

	fnc_text(320.0,50.0,18.0,0.0,
		"Progressive Peripherals and Software");
		
#endif

	fnc_text(320.0,10.0,9.0,0.0,
		"(Initializing Software)");

	Current_Style->draw_font = Root_Font->next_font;

#ifdef BUTTONWAIT
	FOREVER {

      		Wait((1 << w->UserPort->mp_SigBit));

      		while (message=GetMsg(w->UserPort)) {

			class = message->Class;

      			ReplyMsg(message);
			if (class==MOUSEBUTTONS) return;
		}
	}
#endif
}

DoCirc(xc,yc,rd,sp)
STDFLT xc,yc,rd,sp;
{

	STDFLT xt,yt,x,y,xt1,yt1,costh,sinth;
	int tc;

	DrawEllipse(rp,(int) xc,(int) (yc/spct),(int) rd,(int) (rd/spct));

#ifdef MANCIRC
	xt = cos(0.0);
	yt = sin(0.0);
	x = xt * rd;
	y = yt * rd;
	FMove(x+xc, y+yc);
	sinth = sin(TWOPI/sp);
	costh = cos(TWOPI/sp);

	for (tc=0; tc<=(int) sp; tc++) {
		xt1=(xt*costh)-(yt*sinth);
		yt1=(yt*costh)+(xt*sinth);
		xt = xt1;
		yt = yt1;

		x = xt * rd;
		y = yt * rd;

		FDraw(x+xc, y+yc);
	}
#endif
}

STDFLT _lx,_ly;

fnc_text(x,y,h,s,text)
STDFLT x,y,h,s;
UBYTE *text;
{

	UBYTE *inc;		
	STDFLT text_width;	
	STDFLT *sizar;
	WORD	*off;
	WORD	*strx;
	int	cr,l,o,ina,tx,ty,pen;
	STDFLT	rx,ry;
	STDFLT	_sclv,_sclh,_rno;
	UBYTE	j;
	
	inc = text;

	text_width = 0.0;

	sizar = Current_Style->draw_font->font_link->Char_Widths;

	while (*inc) {
		text_width += sizar[(int) *inc];
		inc++;
	}

	text_width *= h;

	tf_plot(x-(text_width/2.0),y,3);

	_rno = Current_Style->draw_font->font_link->Font_Width * h;
	_sclh = Current_Style->draw_font->font_link->Font_Horiz * h;
	_sclv = Current_Style->draw_font->font_link->Font_Vertical * h;

	off = Current_Style->draw_font->font_link->Char_Xtable;
	strx = Current_Style->draw_font->font_link->Word_Table;

	inc = text;

	for (cr=0; cr<strlen(text); cr++) {

		j = *inc;
		inc++;

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
			rx = _sclh * ((STDFLT) (tx-64)) + s*ry;
		
			tf_plot(rx+_lx,ry+_ly,3-pen);

		}

		tf_plot(_rno+_lx,_ly,3);

	}
}

tf_plot(x,y,pen)
STDFLT x,y;
int pen;
{

	STDFLT x1,y1,x2,y2;

	/* printf("plot(%.3f,%.3f,%d)\n",x,y,pen); */

	if (pen==3) {
		_lx=x;
		_ly=y;
		return;
	}

	x1 = _lx;
	y1 = _ly;
	x2 = x;
	y2 = y;
	
	_lx=x;
	_ly=y;

	if (x1<1.0) return;
	if (x1>640.0) return;
	if (y1<1.0) return;
	if (y1>400.0) return;
	if (x2<1.0) return;
	if (x2>640.0) return;
	if (y2<1.0) return;
	if (y2>400.0) return;

	Move(rp,(int) x1, 400 - (int) y1);
	Draw(rp,(int) x2, 400 - (int) y2);

	return;

}
