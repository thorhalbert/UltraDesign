/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	zooms.c - Zoom Control
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
 *	2	 6-May-87 SH	Don't make them hold button down
 *	3	23-Jun-87 SH	Include dispatch functions here now
 *	4	19-May-90 SH	Double clicking into zoom /0 crash
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

VOID PerformZoom();

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

BOOL ShadFirst;

STDFLT ZoomX1,ZoomY1,ZoomX2,ZoomY2;
STDFLT SZoomX1,SZoomY1,SZoomX2,SZoomY2;
BOOL ZoomShad=FALSE;

/*-------------------------------Code Section--------------------------------*/

BOOL zoomonin()
{

	if (!EnterSub(ZOOM_MODE,ZOOM_MESS_EXIT,PerformZoom)) return(FALSE);
	MainMess(ZOOM_MESS_FP);

	ShadFirst=FALSE;
	ZoomShad = FALSE;

	return(FALSE);

}
static BOOL shadowset=FALSE;

DoZoomShad()
{

	if (!ZoomShad) return;
	if (!ShadFirst) return;

	v_reset();
	GI_DrawSetup(1,1);
	v_move(SZoomX1,SZoomY1);
	v_draw(SZoomX2,SZoomY1);
	v_draw(SZoomX2,SZoomY2);
	v_draw(SZoomX1,SZoomY2);
	v_draw(SZoomX1,SZoomY1);

	ZoomShad = FALSE;
}

VOID PerformZoom(sx,sy,action,xr,yr)
WORD sx,sy,action;
STDFLT xr,yr;
{

	STDFLT a,b;

	DoZoomShad();

	if (action==RBUTTONDOWN) {
		ExitSub(ZOOM_MESS_ABORT);
		return;
	}

	switch (action) {
	case LBUTTONDOWN:
		if (!ShadFirst) {
		    	MainMess(ZOOM_MESS_SP);
			ShadFirst=TRUE;

			ZoomX1 = SZoomX1 = xr;
			ZoomY1 = SZoomY1 = yr;

			ShadFirst = TRUE;
			return;			
		} else {
		
			if (ZoomX1==xr) return;		/*4*/

			a = yviewsize/xviewsize;
			
			ZoomX2 = xr-ZoomX1;
			ZoomY2 = ZoomX2 * a;
			if (ZoomY2<0.0) ZoomY2 = -ZoomY2;
			if (yr<ZoomY1) ZoomY2 = -ZoomY2;
			ZoomX2 += ZoomX1;
			ZoomY2 += ZoomY1;

			xviewbegin = min(ZoomX1,ZoomX2);
			yviewbegin = min(ZoomY1,ZoomY2);
			xviewsize = abs(ZoomX2-ZoomX1);
			yviewsize = abs(ZoomY2-ZoomY1);

			RepaintWindow();
	
			SExitSub(NULL);

			return;
		}
	case MOVEMENT:
	case UMOVEMENT:
		if (!ShadFirst) return;

		a = yviewsize/xviewsize;
		SZoomX2 = xr-ZoomX1;
		SZoomY2 = SZoomX2 * a;
		if (SZoomY2<0) SZoomY2 = -SZoomY2;
		if (yr<SZoomY1) SZoomY2 = -SZoomY2;
		SZoomX2 += ZoomX1;
		SZoomY2 += ZoomY1;

		ZoomShad = TRUE;
		DoZoomShad();
		ZoomShad = TRUE;
		return;
	}
}

BOOL Zoom_In()
{

	zoomonin();
	return(TRUE);
}

BOOL Zoom_Full()
{

	fullzoomout();

	RepaintWindow();
	return(TRUE);
}

BOOL Zoom_All()
{

	return(FALSE);

}

BOOL Zoom_Out(fac)
STDFLT fac;
{

	STDFLT xm,ym;

	xm = xviewbegin + (xviewsize/2.0);
	ym = yviewbegin + (yviewsize/2.0);

	xviewbegin = xm - ((xviewsize * fac)/2.0);
	yviewbegin = ym - ((yviewsize * fac)/2.0);

	xviewsize *= fac;
	yviewsize *= fac;

	RepaintWindow();
	return(TRUE);
}
	
BOOL Zoom_O50()
{

	return(Zoom_Out(1.50));

}
BOOL Zoom_O100()
{

	return(Zoom_Out(2.0));

}
BOOL Zoom_O200()
{

	return(Zoom_Out(4.0));

}

BOOL RecenterView(x,y)
STDFLT x,y;
{

	STDFLT xm,ym;

	xviewbegin = x - (xviewsize/2.0);
	yviewbegin = y - (yviewsize/2.0);

	RepaintWindow();
	return(TRUE);

}

VOID CenterAct(sx,sy,action,xr,yr)
WORD sx,sy,action;
STDFLT xr,yr;
{

	if (action==RBUTTONDOWN) {
		ExitSub(CENTER_MESS_ABORT);
		return;
	}

	switch (action) {
	case LBUTTONDOWN:
		RecenterView(xr,yr);
		SExitSub(NULL);
		return;

	}
}


ReCenter()
{

	if (!EnterSub(CENTER_MODE,CENTER_MESS_EXIT,CenterAct)) return(FALSE);
	MainMess(CENTER_MESS_FIND);

	return(FALSE);
}
