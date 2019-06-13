/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	textmode.c - text font user interface
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
 *	2	24-Mar-87 SH	Create textmode using linemode as template
 *	3	 6-May-87 SH	Reduce text box to 39 chars and compress
 *	4	18-Jun-87 SH	Fix the flip flags
 *	5	 7-Feb-88 SH	Add ok/cancel buttons to text req
 *	6	 8-Feb-88 SH	Double click on requester crashes machine
 *				 put in semophore interlock
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include "fonts.h"
#include "attributes.h"

/*----------------------------------Macros-----------------------------------*/

#define TOP_USERWINDOW (YSsize-BOTTOMAREA)

#define TEXT_CLASS	30

#define GAD_TEXTBOX	20
#define GAD_TEXREQ	21

#define GAD_HANDLEON	60
#define GAD_ANGLEON	65

#define GAD_LEFTH	30
#define GAD_CENTH	31
#define GAD_RIGHH	32

#define GAD_HORFLIP	40
#define GAD_CHRFLIP	41
#define GAD_VERFLIP	42

#define GAD_FONTDATA	50
#define GAD_RENDER	55
#define GAD_SKETCH	56

/*-----------------------------Forward References----------------------------*/

VOID PerformText();
VOID text_shadow_rem();

/*-------------------------External Symbol Definitions-----------------------*/

extern struct Image text_1s_image;
extern struct Image text_1u_image;
extern struct Image text_2s_image;
extern struct Image text_2u_image;
extern struct Image text_3s_image;
extern struct Image text_3u_image;
extern struct Image text_4s_image;
extern struct Image text_4u_image;
extern struct Image text_5s_image;
extern struct Image text_5u_image;
extern struct Image text_6s_image;
extern struct Image text_6u_image;
extern struct Image text_7s_image;
extern struct Image text_7u_image;
extern struct Image text_8s_image;
extern struct Image text_8u_image;
extern struct Image text_9s_image;
extern struct Image text_9u_image;
extern struct Image text_10s_image;
extern struct Image text_10u_image;
extern struct Image text_11s_image;
extern struct Image text_11u_image;

/*-------------------------Global Variable Definitions-----------------------*/

BOOL textgadgetssetup = FALSE;

UWORD	flipbits = 0;		/* Flipping bits */
UWORD	handle = CENTER_HANDLE;	/* Default handle */

STDFLT	text_handle_x,text_handle_y;
STDFLT	text_angle = 0.0;
STDFLT 	text_acos,text_asin;
BOOL	text_angle_dirty = TRUE;
STDFLT	text_slant = 0.0;
WORD	T_Activator=GAD_HANDLEON;
BOOL	Handle_Set = FALSE;
BOOL	TextSetup = FALSE;

extern BOOL radialpoint;
extern STDFLT radialx,radialy;
extern SHORT iradialx,iradialy;

/*-------------------------------Code Section--------------------------------*/


VOID GetRidOfTextMode()
{

	GI_DrawSetup(1,1);

	text_shadow_rem();

	GI_DeactivateBGadget(TEXT_CLASS,0,NULL);	

	CleanupShadows=NULL;

	zapwindow();

	TextSetup = FALSE;

}

VOID HandleTextMouse(ingad)
struct GI_bgadget *ingad;
{
	radialpoint = FALSE;
	iradialx = iradialy = 0;

	if (ingad->GadgetID==T_Activator) {
		GI_SetBGadget(TEXT_CLASS,0,ingad,FALSE);
		T_Activator=999;
		if (Handle_Set)	MainMess(TEXT_MESS_REND);
		else		MainMess(TEXT_MESS_SET);
		return;
	}

	if (!Handle_Set&&ingad->GadgetID!=GAD_HANDLEON) {
		MainMess(TEXT_MESS_SET);
		InfoMess(TEXT_MODE,TEXT_MESS_NOTSET);
		GI_SetBGadget(TEXT_CLASS,0,ingad,FALSE);
		T_Activator = 999;
		return;
	}

	GI_SetBGadget(TEXT_CLASS,T_Activator,NULL,FALSE);

	GI_SetBGadget(TEXT_CLASS,0,ingad,TRUE);
	T_Activator = ingad->GadgetID;

	switch (T_Activator) {
	case GAD_HANDLEON:
		MainMess(TEXT_MESS_HAND);
		break;
	case GAD_ANGLEON:
		MainMess(TEXT_MESS_ROT);
		radialx = text_handle_x;
		radialy = text_handle_y;
		radialpoint = TRUE;
		iradialx = iradialy = 0;
		
		break;
	}

	return;
}
	
UBYTE BoxText[255];
LOCAL int exmod;
LOCAL struct Requester GetTextFromScreen;
LOCAL BOOL InReq=FALSE;

VOID TextHandle(ingad)
struct Gadget *ingad;
{

	int gnum;

	if (ingad==NULL) return;

	gnum = ingad->GadgetID;

	CloseRequest(&GetTextFromScreen,w);		/* Close manually */

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

struct StringInfo BoxStringSpec = 
	{ BoxText, undobuf, 0, 254,0,0,0,0,0,0,NULL,0,NULL };
LOCAL struct IntuiText TextEnders[] = {
	{ 2,3,JAM2,0,0,NULL,"  Cancel  ",NULL },
	{ 2,3,JAM2,0,0,NULL,"    OK    ",NULL }};
#define TXACTIVE GADGIMMEDIATE | RELVERIFY 
struct Gadget TextBoxGadget[] = {
 	{ &TextBoxGadget[1],5,50,340,9,GADGHCOMP,TXACTIVE,
		STRGADGET|REQGADGET, 
		NULL, NULL, NULL, 0, &BoxStringSpec, 40, TextHandle },
	{ &TextBoxGadget[2], 220,70,80,8,GADGHCOMP,TXACTIVE,
		BOOLGADGET|REQGADGET,
		NULL,NULL,&TextEnders[0],0,NULL,20,TextHandle},
	{ NULL, 50,70,80,8,GADGHCOMP,TXACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&TextEnders[1],0,NULL,21,TextHandle}};

struct IntuiText BoxQuery = { 2,1,JAM2,20,20,NULL,
	"Enter Text to Draw (1-255 chars)",NULL};

VOID BoxRequester(ingad)
struct GI_bgadget *ingad;
{

	UBYTE SaveTex[256];
	USHORT repos,i;
	struct RastPort *reqrp;

	if (InReq) return;
	if (ingad==NULL) return;

	InReq = TRUE;

	sprintf(SaveTex,"%s",BoxText);

	InitRequester(&GetTextFromScreen);
	
	GetTextFromScreen.Width=350;
	GetTextFromScreen.Height=85;
	GetTextFromScreen.LeftEdge=(XSsize-GetTextFromScreen.Width)/2;
	GetTextFromScreen.TopEdge=(YSsize-GetTextFromScreen.Height)/2;

	GetTextFromScreen.BackFill=1;

	GetTextFromScreen.ReqGadget = &TextBoxGadget[0];
	GetTextFromScreen.ReqText = &BoxQuery;

#ifdef _TURNSY
	GetTextFromScreen.Flags = NOISYREQ;
#endif

	InfoMess(TEXT_MODE,TEXT_MESS_REQ);

	SafeRequest(&GetTextFromScreen, w);

	repos = RemoveGList(w, &TextBoxGadget[0],1);	/* Shuffle */
	AddGList(w, &TextBoxGadget[0], repos, 1, &GetTextFromScreen);
	
	for (i=0; i<4; i++) {
		if (ActivateGadget(&TextBoxGadget[0],w,&GetTextFromScreen)) {
			printf("Text requester activated\n");
			break;
		}
		Delay(2);
	}

	reqrp = GetTextFromScreen.ReqLayer->rp;	/* Nasty */
			
	DoBorders(&GetTextFromScreen,0,0,0);
		
	sit_n_spin(&GetTextFromScreen,0,&TextBoxGadget[1],&TextBoxGadget[2]);
		/* Wait for requesters to exit */

	if (exmod==1) 
		sprintf(BoxText,"%s",SaveTex);

	Text_Box_Dirty = TRUE;
	text_shadow();

	RidSpurious();
	UpdateTitle();		/* Get the normal header back */

	InReq = FALSE;
	return;

}

VOID FlipHandler(ingad)
struct GI_bgadget *ingad;
{

	BOOL answer;
	WORD setbit;

	answer = GI_GetBGadget(0,0,ingad);

	switch(ingad->GadgetID) {
	case GAD_HORFLIP:
		setbit = TEXT_FLIPX;
		break;
	case GAD_CHRFLIP:
		setbit = TEXT_CHRFL;
		break;
	case GAD_VERFLIP:
		setbit = TEXT_FLIPY;
		break;
	}

	flipbits &= (~setbit);
	if (answer) flipbits |= setbit;
	
}

VOID HandleHandler(ingad)
struct GI_bgadget *ingad;
{

	int cl1,cl2;

	switch (ingad->GadgetID) {
	case GAD_LEFTH:
		handle = LEFT_HANDLE;
		cl1 = GAD_CENTH;
		cl2 = GAD_RIGHH;
		break;
	case GAD_CENTH:
		handle = CENTER_HANDLE;
		cl1 = GAD_LEFTH;
		cl2 = GAD_RIGHH;
		break;
	case GAD_RIGHH:
		handle = RIGHT_HANDLE;
		cl1 = GAD_LEFTH;
		cl2 = GAD_CENTH;
	}	

	GI_SetBGadget(TEXT_CLASS,cl1,NULL,FALSE);
	GI_SetBGadget(TEXT_CLASS,cl2,NULL,FALSE);
	GI_SetBGadget(TEXT_CLASS,0,ingad,TRUE);

	text_shadow();
}

VOID RestartText()
{

	Handle_Set=FALSE;

	GI_SetBGadget(TEXT_CLASS,GAD_HANDLEON,NULL,TRUE);
	T_Activator = GAD_HANDLEON;
	MainMess(TEXT_MESS_HAND);

	radialpoint = FALSE;
	iradialx = iradialy = 0;

	text_angle_dirty=TRUE;

	return;
	
}


VOID TextProc(xr,yr)
STDFLT xr,yr;
{

	D_NODE *pickem;
	struct text_mode *textm;
	
	pickem = PickSelect(xr,yr,NODE_TEXT);
	if (pickem==NULL) return;
	
	if (pickem==CurrentRoller) CurrentRoller = NULL;
	
	textm = GetTextData(pickem);

/*	CreateText(BoxText,text_handle_x,text_handle_y,text_angle,
		handle,flipbits,Current_Style);		*/
	
	text_handle_x = textm->x1;
	text_handle_y = textm->y1;
	text_angle = textm->ang;
	handle = textm->Handle_Flag;
	flipbits = textm->Text_Bits;
	strcpy(BoxText,textm->text);
		
	Text_Box_Dirty = TRUE;
	Handle_Set = TRUE;
	text_angle_dirty = TRUE;

	sprintf(pbuffer,"  ");
	paintinfo();

	if (T_Activator!=999) GI_SetBGadget(TEXT_CLASS,T_Activator,NULL,FALSE);

	T_Activator=999;
	
	GI_SetBGadget(TEXT_CLASS,GAD_CENTH,NULL,handle==CENTER_HANDLE);
	GI_SetBGadget(TEXT_CLASS,GAD_LEFTH,NULL,handle==LEFT_HANDLE);
	GI_SetBGadget(TEXT_CLASS,GAD_LEFTH,NULL,handle==RIGHT_HANDLE);
	
	GI_SetBGadget(TEXT_CLASS,GAD_HORFLIP,NULL,flipbits&TEXT_FLIPX);
	GI_SetBGadget(TEXT_CLASS,GAD_CHRFLIP,NULL,flipbits&TEXT_CHRFL);
	GI_SetBGadget(TEXT_CLASS,GAD_VERFLIP,NULL,flipbits&TEXT_FLIPY);

	GI_RefreshGadgets();

	pickem->node_flags |= NODE_DELETED;
	GenericRender(pickem);
	
	DeleteNode(pickem);

	text_shadow();

}

VOID TextScan(sx,sy,action,xr,yr)
WORD sx,sy,action;
STDFLT xr,yr;
{

	if (action==RBUTTONDOWN) {
		ExitSub(SEARCH_MESS_ABORT);
		return;
	}

	switch (action) {
	case LBUTTONDOWN:
		InfoMess(SEARCH_MODE,SEARCH_MESS_WAIT);
		TextProc(xr,yr);	/* Search All types */
		SExitSub(NULL);
		return;

	}
}

VOID HandleTSketch(ingad)
struct GI_bgadget *ingad;
{

	text_shadow_rem();

	if (!EnterSub(SEARCH_MODE,SEARCH_MESS_EXIT,TextScan)) return;
	MainMess(SEARCH_MESS_FIND);

}

VOID RenderButton(ingad)
struct GI_bgadget *ingad;
{

	if (!Handle_Set) {
		InfoMess(TEXT_MODE,TEXT_MESS_NOTSET);
		return;
	}

	text_shadow_rem();

	CreateText(BoxText,text_handle_x,text_handle_y,text_angle,
		handle,flipbits,Current_Style);

	RestartText();
}

VOID Draw_Text()
{

	int bl;

	MainMode(TEXT_MODE);

	if (TextSetup) return;

	if (WrapupDrawMode!=NULL &&
	    WrapupDrawMode != GetRidOfTextMode) 
			(*WrapupDrawMode)();

	mouseaction=PerformText;
	WrapupDrawMode=GetRidOfTextMode;

	zapwindow();

	TextSetup = TRUE;

	CleanupShadows = text_shadow_rem;

	if (textgadgetssetup) {

		GI_ActivateBGadget(TEXT_CLASS,0,NULL,w);

		GI_RefreshGadgets();

		return;
	}

	bl = (YSsize - 306)/2;
	bl += 5;	/* Center within logo box */

	GI_BGadgetImage(3,bl,HandleTextMouse,
		TRUE,TEXT_CLASS,GAD_HANDLEON,TRUE,&text_1u_image,
						      &text_1s_image);
	GI_SetBGadget(TEXT_CLASS,GAD_HANDLEON,NULL,TRUE);
	MainMess(TEXT_MESS_HAND);

	GI_BGadgetImage(3,bl+28,HandleTextMouse,
		TRUE,TEXT_CLASS,GAD_ANGLEON,FALSE,&text_2u_image,
						      &text_2s_image);

	GI_BGadgetImage(3,bl+56,BoxRequester,TRUE,TEXT_CLASS,
		GAD_TEXREQ,TRUE,&text_3u_image,&text_3s_image);
	
	GI_BGadgetImage(3,bl+84,HandleHandler,TRUE,TEXT_CLASS,
		GAD_LEFTH,FALSE,&text_4u_image,&text_4s_image);
	
	GI_BGadgetImage(3,bl+112,HandleHandler,TRUE,TEXT_CLASS,
		GAD_CENTH,TRUE,&text_5u_image,&text_5s_image);
	
	GI_SetBGadget(TEXT_CLASS,GAD_CENTH,NULL,TRUE);

	GI_BGadgetImage(3,bl+140,HandleHandler,TRUE,TEXT_CLASS,
		GAD_RIGHH,FALSE,&text_6u_image,&text_6s_image);
	
	GI_BGadgetImage(3,bl+168,FlipHandler,TRUE,TEXT_CLASS,
		GAD_HORFLIP,FALSE,&text_7u_image,&text_7s_image);
	
	GI_BGadgetImage(3,bl+196,FlipHandler,TRUE,TEXT_CLASS,
		GAD_CHRFLIP,FALSE,&text_8u_image,&text_8s_image);
	
	GI_BGadgetImage(3,bl+224,FlipHandler,TRUE,TEXT_CLASS,
		GAD_VERFLIP,FALSE,&text_9u_image,&text_9s_image);
	
	GI_BGadgetImage(3,bl+252,RenderButton,TRUE,TEXT_CLASS,
		GAD_RENDER,TRUE,&text_10u_image,&text_10s_image);
	
	GI_BGadgetImage(3,bl+280,HandleTSketch,TRUE,TEXT_CLASS,
		GAD_SKETCH,TRUE,&text_11u_image,&text_11s_image);
	
	GI_ActivateBGadget(TEXT_CLASS,0,NULL,w);

	GI_RefreshGadgets();

	textgadgetssetup=TRUE;
	
}
	
VOID PerformText(sx,sy,action,xr,yr)
WORD sx,sy,action;
STDFLT xr,yr;
{

	STDFLT theta;

	GI_DrawSetup(1,1);

	switch (action) {
	case MOVEMENT:
	case UMOVEMENT:
		switch (T_Activator) {
		case GAD_HANDLEON:
			text_handle_x = xr;
			text_handle_y = yr;
			text_shadow();
			return;
		case GAD_ANGLEON:
			theta = atan2(yr-text_handle_y,xr-text_handle_x);
			normalize(theta);
			theta = theta * 180.0;
			theta /= PI;
			if (theta<0) theta+=360.0;
			text_angle=theta;
			text_angle_dirty=TRUE;
			text_shadow();
			return;
		}
	case LBUTTONDOWN:
		switch (T_Activator) {
		case GAD_HANDLEON:
			text_handle_x = xr;
			text_handle_y = yr;
			Handle_Set=TRUE;
			GI_SetBGadget(TEXT_CLASS,GAD_HANDLEON,NULL,FALSE);
			MainMess(TEXT_MESS_REND);
			T_Activator=999;
			text_shadow();
			radialpoint = FALSE;
			iradialx = iradialy = 0;
			return;
		case GAD_ANGLEON:
			theta = atan2(yr-text_handle_y,xr-text_handle_x);
			normalize(theta);
			theta = theta * 180.0;
			theta /= PI;
			if (theta<0) theta+=360.0;
			text_angle=theta;
			text_angle_dirty=TRUE;
			GI_SetBGadget(TEXT_CLASS,GAD_ANGLEON,NULL,FALSE);
			MainMess(TEXT_MESS_REND);
			T_Activator=999;
			text_shadow();
			radialpoint = FALSE;
			iradialx = iradialy = 0;
			return;
		}
		break;
	case RBUTTONDOWN:
		RestartText();
		break;
	}
}

BOOL text_shad_up = FALSE;
STDFLT 	text_shad_box,
	text_shad_h,
	text_shad_x,
	text_shad_y,
	text_shad_slant,
	text_shad_angle;
UWORD	text_shad_handle;

VOID render_shadow()
{

	STDFLT x,y,xr[4],yr[4],ll,lr;
	int i;

	GI_DrawSetup(1,1);

	switch(text_shad_handle) {

	case LEFT_HANDLE:
		ll = 0.0;
		lr = text_shad_box;
		break;
	case CENTER_HANDLE:
		ll = - text_shad_box/2.0;
		lr = - ll;
		break;
	case RIGHT_HANDLE:
		ll = - text_shad_box;
		lr = 0.0;
		break;
	}
	
	y = 0.0;
	x = ll + (text_shad_slant*y);

	xr[0] = x*text_acos-y*text_asin+text_shad_x;
	yr[0] = x*text_asin+y*text_acos+text_shad_y;

	y = text_shad_h;
	x = ll + (text_shad_slant*y);
	
	xr[1] = x*text_acos-y*text_asin+text_shad_x;
	yr[1] = x*text_asin+y*text_acos+text_shad_y;

	y = text_shad_h;
	x = lr + (text_shad_slant*y);
	
	xr[2] = x*text_acos-y*text_asin+text_shad_x;
	yr[2] = x*text_asin+y*text_acos+text_shad_y;

	y = 0.0;
	x = lr + (text_shad_slant*y);
	
	xr[3] = x*text_acos-y*text_asin+text_shad_x;
	yr[3] = x*text_asin+y*text_acos+text_shad_y;

	v_reset();
	v_move(xr[0],yr[0]);
	for (i=1; i<4; i++) v_draw(xr[i],yr[i]);
	v_draw(xr[0],yr[0]);

	x = (xr[1]+xr[2])/2.0;
	y = (yr[1]+yr[2])/2.0;
	
	v_draw(x,y);
	v_draw(xr[3],yr[3]);
	
	return;

}

text_shadow()
{

	UBYTE *inc;
	STDFLT *sizar;
	STDFLT x,y,ll,lr;

	if (text_shad_up) text_shadow_rem();

	text_shad_x = text_handle_x;
	text_shad_y = text_handle_y;
	text_shad_slant = Current_Style->Slant;
	text_shad_angle = text_angle;
	text_shad_h = Current_Style->Prime_Height;
	text_shad_handle = handle;

	if (text_angle_dirty) {
		text_acos = cos(text_shad_angle * .01745329);
		text_asin = sin(text_shad_angle * .01745329);
		text_angle_dirty = FALSE;
	}

	if (Text_Box_Dirty) {

		inc = BoxText;

		text_shad_box = 0.0;

		sizar = Current_Style->draw_font->font_link->Char_Widths;

		while (*inc) {
		
			text_shad_box += sizar[(int) *inc];
			inc++;
		}

		text_shad_box *= text_shad_h;
		text_shad_box *= Current_Style->Aspect;

		Text_Box_Dirty = FALSE;

	}
	
	render_shadow();

	text_shad_up = TRUE;

	return;

}

VOID text_shadow_rem()
{

	if (!text_shad_up) return;

	render_shadow();

	text_shad_up = FALSE;
}

