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
 *	3	 4-Oct-87 SH	Convert to screen saver
 *
 */

#ifdef SCREEN_SAVER

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

struct NewScreen savscr = {
   0, 0,                               	/* start position                */
   640, STDSCREENHEIGHT, 1,             /* width, height, depth          */
   0, 0,                               	/* detail pen, block pen         */
   LACE|HIRES,				/* View Mode */
   CUSTOMSCREEN,			/* screen type                   */
   NULL,                          	/* font to use                   */
   "UCAD", 				/* default title for screen      */
   NULL                                	/* pointer to additional gadgets */
};
		
struct NewWindow savwin = {
   0, 0,                          	/* start position                */
   640, 400,                 		/* width, height                 */
   0, 0,                        	/* detail pen, block pen         */
   MOUSEBUTTONS|RAWKEY|INTUITICKS,	/* IDCMP flags                   */
   BORDERLESS | BACKDROP | ACTIVATE | RMBTRAP,
                                  	/* window flags                  */
   NULL,           			/* pointer to first user gadget  */
   NULL,                          	/* pointer to user checkmark     */
   NULL,           		  	/* window title                  */
   NULL,                          	/* pointer to screen (set below) */
   NULL,                          	/* pointer to superbitmap        */
   0, 0, 640, 400,			/* ignored since not sizeable    */
   CUSTOMSCREEN                   	/* type of screen desired        */
};


#define FDraw(x,y) Draw(rp,(int)(x),(int)((y)/spct))
#define FMove(x,y) Move(rp,(int)(x),(int)((y)/spct))

extern struct Image uueaerbbrush_image;
extern USHORT uueaerbbrush_imageData[];
#define BRUSHSET 90

extern struct IntuiMessage *message;

LOCAL	STDFLT	ftop,fbot,flef,frig,frise,frun;
LOCAL	STDFLT	fnx,fny,fox=320,foy=200;

struct Window *wf;
struct Screen *screenf;
struct RastPort *rpf;
struct ViewPort *vpf;

#define MAXMOVE 3.0

ULONG lastbunch=0;

double ran();
#define grand() ran()

VOID setran()
{

	frise = (grand()*2.0*MAXMOVE) - MAXMOVE;
	frun = (grand()*2.0*MAXMOVE) - MAXMOVE;

}

VOID wiggle()
{

	STDFLT	tentwig;

	do {

		do tentwig = frise + ((grand()*.4)-.2);
		while (tentwig<-MAXMOVE||tentwig>MAXMOVE);
		frise = tentwig;

		do tentwig = frun + ((grand()*.4)-.2);
		while (tentwig<-MAXMOVE||tentwig>MAXMOVE);
		frun = tentwig;
	
		if (abs(frise+frun)<.4) setran();	/* Jump */

		fnx = fox + frun;
		fny = foy + frise;

		if (fny<ftop||
		    fny>fbot||
		    fnx<flef||
		    fnx>frig) setran();

	} while (fny<ftop||
		 fny>fbot||
		 fnx<flef||
		 fnx>frig);

	WaitTOF();	/* Wait for frame so we don't get flicker */

	uueaerbbrush_image.PlanePick = 0;
	uueaerbbrush_image.PlaneOnOff = 0;	

	DrawImage(rpf,&uueaerbbrush_image,(int) fox-20,(int) foy-15);

	uueaerbbrush_image.PlanePick = 1;
	uueaerbbrush_image.PlaneOnOff = 0;

	DrawImage(rpf,&uueaerbbrush_image,(int) fnx-20,(int) fny-15);

	fox = fnx;
	foy = fny;
}
		
screensaver()
{

	STDFLT x,y,t,ta,xn,yn;
	int tc;
	int class;


  	screenf = OpenScreen(&savscr);
   	if (screenf == NULL) {
      		fputs("Can't open new screen!\n",stdout);
      		return(1);
   	}

	ShowTitle(screenf,FALSE);

   	vpf = &screenf->ViewPort;

	SetRGB4(vpf, 0, 0, 0, 0);
	SetRGB4(vpf, 1, 14, 14, 13);

	savwin.Width = screenf->Width;
	savwin.Height = screenf->Height;

   	savwin.Screen = screenf;
   	wf = OpenWindow(&savwin);
   	if (wf == NULL) {
      		CloseScreen(screenf);
      		fputs("Can't open new window!\n",stdout);
      		return (1);
   	}

	SetPointer(wf, &BlankPointer, 1, 1, 0, 0);

   	rpf = wf->RPort;

	for (tc=0; tc<BRUSHSET; tc++) 
		uueaerbbrush_imageData[tc] = ~uueaerbbrush_imageData[tc];

	ftop = 20;
	fbot = screenf->Height - 20;
	flef = 24;
	frig = screenf->Width-24;

	setran();

	FOREVER {

      		Wait((1 << wf->UserPort->mp_SigBit));

      		while (message=GetMsg(wf->UserPort)) {

			class = message->Class;

      			ReplyMsg(message);
			if (class==MOUSEBUTTONS||
			    class==RAWKEY) {
				if (wf) CloseWindow(wf);
				if (screenf) CloseScreen(screenf);
				uueaerbbrush_image.Depth = 2;
				uueaerbbrush_image.PlanePick = 3;
				uueaerbbrush_image.PlaneOnOff = 0;
				for (tc=0; tc<BRUSHSET; tc++) 
					uueaerbbrush_imageData[tc] = 
						~uueaerbbrush_imageData[tc];
				return;
			}
		wiggle();
		}
	}
}

#endif
