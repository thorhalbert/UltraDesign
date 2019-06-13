/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	snaps.c - snap handling code
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
 *	1	 5-May-87 SH	Get sliders working
 *	2	 5-Aug-87 SH	Clean out old junk
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

extern struct Gadget HorizPanProp,VertPanProp;

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/


/*------------------------snap handling code-----------------------------*/

extern struct PropInfo HorizPan,VertPan;

DoPanVert(ingad)
struct Gadget *ingad;
{
	UWORD	inpos;
	LONG	scaler;
	STDFLT	equate,tcons,newstart,width,sbegin;
	
	tcons=0xFFFF;
	inpos = VertPan.VertPot;

	equate=0xFFFF-inpos;
	equate = (equate/tcons);

	width = (yviewMAX-yviewMIN);
	sbegin = yviewMIN + (width/2.0) - (width*1.05/2.0);

	newstart = sbegin + equate*((width*1.05)-yviewsize);

	newstart /= ysnap;
	scaler=newstart;
	newstart=scaler;
	newstart *= ysnap;
	yviewbegin = newstart;

	RepaintWindow();
	
}

DoPanHoriz(ingad)
struct Gadget *ingad;
{
	UWORD	inpos;
	LONG	scaler;
	STDFLT	equate,tcons,newstart,width,sbegin;
	
	tcons=0xFFFF;
	inpos = HorizPan.HorizPot;

	equate=inpos;
	equate = (equate/tcons);

	width = (xviewMAX-xviewMIN);
	sbegin = xviewMIN + (width/2.0) - (width*1.05/2.0);

	newstart = sbegin + equate*((width*1.05)-xviewsize);

	newstart /= xsnap;
	scaler=newstart;
	newstart=scaler;
	newstart *= xsnap;

	xviewbegin = newstart;

	RepaintWindow();
	
}

AdjustPots()
{

	STDFLT pct,pcvalue,width,sbegin;
	UWORD potvalue,bodyvalue,fconst;

	fconst=0xFFFF;

	pct=1.0;

	if (xviewMAX-xviewMIN>0.0) {
		pct=xviewsize/((xviewMAX-xviewMIN));
	}

	if (pct<0.0) pct=0.0;
	if (pct>1.0) pct=1.0;

	pcvalue = pct*fconst;
	if (pcvalue>fconst) pcvalue=fconst;

	bodyvalue=pcvalue;

	pct=0.0;

	width = (xviewMAX-xviewMIN);
	sbegin = xviewMIN + (width/2.0) - (width*1.05/2.0);

	if (xviewMAX-xviewMIN>0.0) {
		pct=(xviewbegin-sbegin)/((width*1.05)-xviewsize);
	}

	if (pct<0.0) pct=0.0;
	if (pct>1.0) pct=1.0;

	pcvalue = pct*fconst;
	if (pcvalue>fconst) pcvalue=fconst;
	
	potvalue=pcvalue;
	if (potvalue<0) potvalue=0;

	ModifyProp(&HorizPanProp,w,NULL,AUTOKNOB | FREEHORIZ,
		potvalue,MAXPOT,bodyvalue,MAXBODY);

	pct=1.0;

	if (yviewMAX-yviewMIN>0.0) {
		pct=yviewsize/((yviewMAX-yviewMIN)*1.05);
	}

	if (pct<0.0) pct=0.0;
	if (pct>1.0) pct=1.0;

	pcvalue = pct*fconst;
	if (pcvalue>fconst) pcvalue=fconst;

	bodyvalue=pcvalue;

	width = (yviewMAX-yviewMIN);
	sbegin = yviewMIN + (width/2.0) - (width*1.05/2.0);

	if (xviewMAX-xviewMIN>0.0) {
		pct=(yviewbegin-sbegin)/((width*1.05)-yviewsize);
	}

	if (pct<0.0) pct=0.0;
	if (pct>1.0) pct=1.0;

	pcvalue = pct*fconst;
	if (pcvalue>fconst) pcvalue=fconst;
	
	potvalue=pcvalue;
	potvalue = 0xFFFF - potvalue;
	if (potvalue<0) potvalue=0;

	ModifyProp(&VertPanProp,w,NULL,AUTOKNOB | FREEVERT,
		MAXPOT,potvalue,MAXBODY,bodyvalue);

}
