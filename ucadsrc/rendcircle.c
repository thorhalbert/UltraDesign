/*---------------------Ellipse Clipper Code-----------------------*/

STDFLT degcvt(theta)
STDFLT theta;
{
	
	theta = theta * 180.0;
	theta /= PI;
	if (theta<0) theta+=360.0;

	return(theta);
}

STDFLT ConvEllipNotation(penng,radiusx,radiusy)
STDFLT penng,radiusx,radiusy;
{

	STDFLT inslope,PID34,inter;

	PID34=PI;
	PID34+=PID2;

	if (penng==PID2) return(PID2);
	if (penng==PID34) return(PID34);

	inslope=tan(penng);

	inter=sqrt((radiusx*radiusx*radiusy*radiusy)/
		((radiusy*radiusy)+(radiusx*radiusx*inslope*inslope)))
			/radiusx;

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

VOID FancyEllipseClipper(centx,centy,radx,rady,bega,enda,rotation,fg)
STDFLT centx,centy,radx,rady,bega,enda,rotation;
BOOL fg;
{

	STDFLT xn,yn,xn1,yn1,tx1,ty1;
	FAST	WORD qp,ctr,i;
	STDFLT sinth,costh,theta;
	STDFLT csx,rotcos,rotsin,thet1;
	STDFLT deltang;
#ifdef DAMNFAST
	int cx,cy,rx,ry;
#endif

	/* printf("Ellipse(%f,%f,%f,%f,%f,%f,%f)\n",
		centx,centy,radx,rady,bega,enda,rotation); */

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

	if (radx==rady&&deltang>=(TWOPI-.01)) {		/* Its a circle */
		bega = 0.0;
		enda = 0.0;
		rotation = 0.0;		
	}

	if (deltang>=(TWOPI-.01)&&abs(rotation)<=0.01&&
		centx-radx>=0.0&&
		centx+radx<=right&&
		centy-rady>=0.0&&
		centy+rady<=top) {

			EllipseClip = WIND_IN;
			if (fg) DrawScreenEllipse(centx,centy,radx,rady);
			return;		/* All on screen */
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

	xn = deltang;
	xn *= max(radx,rady);
	xn /= ypixsize;
	
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
		
	v_move(tx1,ty1);	

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

	v_reset();

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
