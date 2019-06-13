/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	palette.c - color map maintenenace requester	
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
 *	1	13-Jun-87 SH	Create palette program from scratch
 *	2	16-Jul-87 SH	Make more better
 *	3	11-Sep-87 SH	Add proportional icons
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include <graphics/gfxmacros.h>

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

VOID Palhandler(),PLHandler();

/*-------------------------External Symbol Definitions-----------------------*/

extern struct Window *w;
extern struct ViewPort *vp;

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

VOID ColorSlide();

#define PALGAD RELVERIFY | GADGIMMEDIATE

struct Image RedImage;

struct PropInfo	RedSlide =
{ AUTOKNOB | FREEVERT,0,0,0xFFFF, 0x1000,0,0,0,0,0,0};

struct Gadget	RedProp =
{ NULL,20,20,20,105,GADGIMAGE,PALGAD,PROPGADGET,(APTR) &RedImage,
  NULL,NULL,0,(APTR) &RedSlide,0,ColorSlide }; 

struct Image GreenImage;

struct PropInfo	GreenSlide =
{ AUTOKNOB | FREEVERT,0,0,0xFFFF, 0x1000,0,0,0,0,0,0};

struct Gadget	GreenProp =
{ &RedProp,50,20,20,105,GADGIMAGE,PALGAD,PROPGADGET,(APTR) &GreenImage,
  NULL,NULL,0,(APTR) &GreenSlide,0,ColorSlide }; 

struct Image BlueImage;

struct PropInfo	BlueSlide =
{ AUTOKNOB | FREEVERT,0,0,0xFFFF, 0x1000,0,0,0,0,0,0};

struct Gadget	BlueProp =
{ &GreenProp,80,20,20,105,GADGIMAGE,PALGAD,PROPGADGET,(APTR) &BlueImage,
  NULL,NULL,0,(APTR) &BlueSlide,0,ColorSlide }; 

LOCAL struct IntuiText DispColor = 
	{ 0,0,JAM2,0,0,NULL,"                   ",NULL};

LOCAL struct IntuiText PalBox[] = {
	{ 0,0,JAM2,0,0,NULL,"     ",NULL },
	{ 1,1,JAM2,0,0,NULL,"     ",NULL },
	{ 2,2,JAM2,0,0,NULL,"     ",NULL },
	{ 3,3,JAM2,0,0,NULL,"     ",NULL },
	{ 6,6,JAM2,0,0,NULL,"     ",NULL },
	{ 7,7,JAM2,0,0,NULL,"     ",NULL },
	{ 8,8,JAM2,0,0,NULL,"     ",NULL },
	{ 9,9,JAM2,0,0,NULL,"     ",NULL },
	{ 10,10,JAM2,0,0,NULL,"     ",NULL },
	{ 11,11,JAM2,0,0,NULL,"     ",NULL },
	{ 12,12,JAM2,0,0,NULL,"     ",NULL },
	{ 13,13,JAM2,0,0,NULL,"     ",NULL },
	{ 14,14,JAM2,0,0,NULL,"     ",NULL },
	{ 15,15,JAM2,0,0,NULL,"     ",NULL }};

#define PALACTIVE RELVERIFY
LOCAL struct Gadget Pallist[] = {
	{ &Pallist[1], 200,20,40,8,GADGHNONE,PALACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PalBox[0],0,NULL,20,Palhandler},
	{ &Pallist[2], 200,30,40,8,GADGHNONE,PALACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PalBox[1],0,NULL,21,Palhandler},
	{ &Pallist[3], 200,40,40,8,GADGHNONE,PALACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PalBox[2],0,NULL,22,Palhandler},
	{ &Pallist[4], 200,50,40,8,GADGHNONE,PALACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PalBox[3],0,NULL,23,Palhandler},
	{ &Pallist[5], 200,60,40,8,GADGHNONE,PALACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PalBox[4],0,NULL,24,Palhandler},
	{ &Pallist[6], 200,70,40,8,GADGHNONE,PALACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PalBox[5],0,NULL,25,Palhandler},
	{ &Pallist[7], 200,80,40,8,GADGHNONE,PALACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PalBox[6],0,NULL,26,Palhandler},
	{ &Pallist[8], 200,90,40,8,GADGHNONE,PALACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PalBox[7],0,NULL,27,Palhandler},
	{ &Pallist[9], 200,100,40,8,GADGHNONE,PALACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PalBox[8],0,NULL,28,Palhandler},
	{ &Pallist[10], 200,110,40,8,GADGHNONE,PALACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PalBox[9],0,NULL,29,Palhandler},
	{ &Pallist[11], 200,120,40,8,GADGHNONE,PALACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PalBox[10],0,NULL,30,Palhandler},
	{ &Pallist[12], 200,130,40,8,GADGHNONE,PALACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PalBox[11],0,NULL,31,Palhandler},
	{ &Pallist[13], 200,140,40,8,GADGHNONE,PALACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PalBox[12],0,NULL,32,Palhandler},
	{ &BlueProp, 200,150,40,8,GADGHNONE,PALACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PalBox[13],0,NULL,33,Palhandler}};

LOCAL struct IntuiText PLListText[] = {
	{ 2,3,JAM2,0,0,NULL,"  Cancel  ",NULL },
	{ 2,3,JAM2,0,0,NULL,"    OK    ",NULL },
	{ 3,1,JAM2,5,5,NULL,"Palette Setup",NULL },
};

#define PLACTIVE GADGIMMEDIATE | RELVERIFY
LOCAL struct Gadget PLList[] = {
	{ &PLList[1], 150,165,80,8,GADGHCOMP,PLACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PLListText[0],0,NULL,50,PLHandler},
	{ &Pallist[0], 20,165,80,8,GADGHCOMP,PLACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&PLListText[1],0,NULL,51,PLHandler}};

LOCAL struct Requester PalRequester;

LOCAL USHORT xtab1[] = { 0,1,2,3,6,7,8,9,10,11,12,13,14,15 };
LOCAL USHORT xtab2[] = { 0,1,2,3,3,3,4,5,6,7,8,9,10,11,12,13 };
LOCAL int selout;
LOCAL int bakcolor,selcolor;
LOCAL int redback[16],greenback[16],blueback[16];
LOCAL int reddef[16],greendef[16],bluedef[16];

int oldpal = -1;

struct RastPort *PalRp;

VOID ColorSlide(ingad)
struct Gadget *ingad;
{ 	

	ULONG ourcolor;
	UBYTE disout[8];

	ourcolor = xtab1[selcolor];

	GI_SetColor(ourcolor,
		~(RedSlide.VertPot),
		~(GreenSlide.VertPot),
		~(BlueSlide.VertPot));

	SetAPen(PalRp, ourcolor);
	SetOPen(PalRp, ourcolor);		/* Put a box around it */

	RectFill(PalRp, 110,20,150,125);	/* Update color */
	GI_ColorString(ourcolor,disout);
	SetAPen(PalRp,0);
	SetBPen(PalRp,1);
	SetDrPt(PalRp, 0xFFFF);

	SetDrMd(PalRp, JAM2);

	Move(PalRp,110,136);

	Text(PalRp, disout, strlen(disout));

}

VOID Palhandler(ingad)
struct Gadget *ingad;
{ 

	int a,pos;
	ULONG red,blue,green;

	if (ingad==NULL) return;	/* Stray mouse move? */

	selcolor = ingad->GadgetID - 20;
	printf("Selected color %d (%d)\n",xtab1[selcolor],selcolor);
	for (a=0; a<14; a++) {
		pos = RemoveGList(w,&Pallist[a],1);
		if (pos<0) {
			printf("didn't find item %d\n",a);
			continue;
		}
		Pallist[a].Flags &= (~SELECTED);
		AddGList(w,&Pallist[a],-1,1,&PalRequester);
	}
	pos = RemoveGList(w,&Pallist[selcolor],1);
	Pallist[selcolor].Flags |= SELECTED;

	GI_GetColor(xtab1[selcolor],&red,&green,&blue);

	ModifyProp(&RedProp,w,&PalRequester,
		AUTOKNOB | FREEVERT, 0, ~red, 0xFFFF, 0x1000);
	ModifyProp(&GreenProp,w,&PalRequester,
		AUTOKNOB | FREEVERT, 0, ~green, 0xFFFF, 0x1000);
	ModifyProp(&BlueProp,w,&PalRequester,
		AUTOKNOB | FREEVERT, 0, ~blue, 0xFFFF, 0x1000);

	AddGList(w,&Pallist[selcolor],-1,1,&PalRequester);

	RefreshGadgets(PalRequester.ReqGadget,w,&PalRequester);

	ColorSlide(NULL);
	
}

VOID PLHandler(ingad)
struct Gadget *ingad;
{ 

	if (ingad==NULL) return;	/* Stray mouse moves? */

	CloseRequest(&PalRequester, w);
	
	switch (ingad->GadgetID) {
	case 51:
		selout=2;
		break;
	default:
		selout=1;
		break;
	}
}

BOOL PalDo()
{

	if (oldpal<0) oldpal = 0;
	Palette(oldpal);
	return(TRUE);
}

int Palette(inpal)
int inpal;
{
	int a;
	BOOL spec;
	ULONG red,blue,green;

	if (inpal<0) {
		inpal=(-inpal)-1;;
		spec=TRUE;
	} else spec=FALSE;
	
	bakcolor = inpal;
	selcolor = xtab2[inpal];
	selout = 0;

	for (a=0; a<16; a++) {
		GI_GetColor(a, &redback[a],&greenback[a],&blueback[a]);
	}

	for (a=0; a<14; a++) Pallist[a].Flags &= ~(SELECTED|GADGDISABLED);
	Pallist[selcolor].Flags |= SELECTED;
	if (spec) for (a=0; a<14; a++) Pallist[a].Flags |= GADGDISABLED;
	
	InitRequester(&PalRequester);
	
	PalRequester.Width=250;
	PalRequester.Height=190;
	PalRequester.LeftEdge=(XSsize-PalRequester.Width)/2;
	PalRequester.TopEdge=(YSsize-PalRequester.Height)/2;

	PalRequester.BackFill=1;

	PalRequester.ReqGadget = &PLList[0];
	PalRequester.ReqBorder = NULL;
	PalRequester.ReqText = &PLListText[2];
	
	GI_GetColor(inpal,&red,&green,&blue);

	RedSlide.VertPot = ~red;
	GreenSlide.VertPot = ~green;
	BlueSlide.VertPot = ~blue;
	
#ifdef _TURNSY
		PalRequester.Flags = NOISYREQ;
#endif

	SafeRequest(&PalRequester, w);

	PalRp = PalRequester.ReqLayer->rp;	/* Nasty */

	SetAPen(PalRp,0);
	SetBPen(PalRp,1);
	SetDrPt(PalRp, 0xFFFF);

	SetDrMd(PalRp, JAM2);

	Move(PalRp,28,136);
	Text(PalRp, "R", 1);

	Move(PalRp,58,136);
	Text(PalRp, "G", 1);

	Move(PalRp,88,136);
	Text(PalRp, "B", 1);

	ColorSlide(NULL);

	SetAPen(PalRp,2);
	Move(PalRp,109,19);
	Draw(PalRp,151,19);
	Draw(PalRp,151,126);
	Draw(PalRp,109,126);
	Draw(PalRp,109,19);
		
	DoBorders(&PalRequester,15,0,0);

	sit_n_spin(&PalRequester,1,&PLList[0],&PLList[1]);
			/* Wait for requesters - follow mouse */

	if (selout!=2) {		/* Abort, put everything back */
		for (a=0; a<16; a++)
			GI_SetColor(a, redback[a],greenback[a],blueback[a]);
			oldpal = bakcolor;
		return(bakcolor);
	}

	oldpal = xtab1[selcolor];
	return(xtab1[selcolor]);
}

SavePalette()
{

	int a;

	for (a=0; a<16; a++) {
		GI_GetColor(a, &reddef[a],&greendef[a],&bluedef[a]);
	}
}

BOOL RestorePalette()
{
	int a;

	for (a=0; a<16; a++)
		GI_SetColor(a, reddef[a],greendef[a],bluedef[a]);

}
