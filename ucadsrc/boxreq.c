/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	searcher.c - Find and select objects
 *
 */

/*
 *	Copyright (C) 1986-1989 by WURBsoft Inc., and Scott Halbert.
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
 *	1	16-Nov-89 SH	Make up text requester
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

/*
 *     +++
 *     +++
 *  +++++++++		Text box requester
 *  +++++++++
 *     +++
 *     +++
 */

LOCAL int exmod;
LOCAL struct Requester BoxRequester;

VOID BoxHandReq(ingad)
struct Gadget *ingad;
{

	int gnum;

	if (ingad==NULL) return;
	gnum = ingad->GadgetID;

	CloseRequest(&BoxRequester,w);	/* Close manually */

	switch (gnum) {
	case 40:
		exmod = 0;
		return;
	case 20:
		exmod = 1;
		return;
	case 21:
		exmod = 0;
		return;
	}
}

LOCAL struct StringInfo BoxStringSpec = 
	{ NULL, undobuf, 0, 254,0,0,0,0,0,0,NULL,0,NULL };
LOCAL struct IntuiText TextEnders[] = {
	{ 2,3,JAM2,0,0,NULL,"  Cancel  ",NULL },
	{ 2,3,JAM2,0,0,NULL,"    OK    ",NULL }};
#define TXACTIVE GADGIMMEDIATE | RELVERIFY 
LOCAL struct Gadget TextBoxGadget[] = {
 	{ &TextBoxGadget[1],5,50,340,9,GADGHCOMP,TXACTIVE,STRGADGET|REQGADGET, 
	NULL, NULL, NULL, 0, &BoxStringSpec, 40, BoxHandReq },
	{ &TextBoxGadget[2], 50,70,80,8,GADGHCOMP,TXACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&TextEnders[0],0,NULL,20,BoxHandReq},
	{ NULL, 220,70,80,8,GADGHCOMP,TXACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&TextEnders[1],0,NULL,21,BoxHandReq}};
LOCAL struct IntuiText BoxQuery = { 2,1,JAM2,20,20,NULL,NULL,NULL};

VOID BoxReq(header,intext)
UBYTE *header,*intext;
{

	UBYTE DoText[256],*newtex;
	int i;

	BoxQuery.IText = header;

	strcpy(DoText,intext);

	InitRequester(&BoxRequester);
	
	BoxRequester.Width=350;
	BoxRequester.Height=85;
	BoxRequester.LeftEdge=(XSsize-BoxRequester.Width)/2;
	BoxRequester.TopEdge=(YSsize-BoxRequester.Height)/2;

	BoxRequester.BackFill=1;
	BoxStringSpec.Buffer = DoText;
	BoxStringSpec.BufferPos = strlen(DoText);

	BoxRequester.ReqGadget = &TextBoxGadget[0];
	BoxRequester.ReqText = &BoxQuery;

#ifdef _TURNSY
	BoxRequester.Flags = NOISYREQ;
#endif

	SafeRequest(&BoxRequester, w);

	for (i=0; i<4; i++) {
		if (ActivateGadget(&TextBoxGadget[0],w,&BoxRequester)) {
			printf("Text requester activated\n");
			break;
		}
		Delay(2);
	}

	DoBorders(&BoxRequester,0,0,0);

	sit_n_spin(&BoxRequester,0,&TextBoxGadget[1],&TextBoxGadget[2]);	
		/* Wait for requesters to exit */

	if (exmod==0) {
		strcpy(intext,DoText);
	}

	return;

}
