/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	stylereq.c - Lettering style requesters
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
 *	1	11-May-87 SH	Generate new program from scratch
 *	2	13-Jun-87 SH	Add layer requesters
 *	3	28-Jun-87 SH	Layout tree requester code
 *	4	12-Sep-87 SH	Add Instant Response to prop slider
 *	5	13-Sep-87 SH	Break attributes.c into pieces
 *	6	 8-Feb-88 SH	Add the expunge command
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include "attributes.h"

/*----------------------------------Macros-----------------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*------------------------Set up scroller for styles------------------------*/

STYLE_A *hold_style=NULL;

VOID *Style_First();
VOID *Style_Next();
VOID *Style_Prev();
VOID *Style_Search();
VOID *Style_Parent();
VOID *Style_Descend();
VOID *Style_Form();
VOID *Style_Name();

BOOL Style_Sc(stpos,title,new)
UBYTE *title;
STYLE_A *stpos;
BOOL new;
{
	return(Scroller(Style_First,
		Style_Next,
		Style_Prev,
		Style_Search,
		Style_Parent,
		Style_Descend,
		Style_Form,
		Style_Name,
		FALSE,
		TRUE,
		new,
		stpos,
		"    OK    ",
		"  Cancel  ",
		title,
		"Name of Lettering Style--------Counts-Font",
		&hold_style,
		StyleMask,
		"(Style Table)"));
}

BOOL Style_NewCurrent()
{

	if (Current_Style==NULL) Current_Style=Root_Style;
	if (Current_Style==NULL) return(FALSE);

	return(Style_Sc(Current_Style,"Select Current Lettering Style",FALSE));

}

BOOL ReportStyle()
{

	STYLE_A *looper;

	FILE *ofil;

	ofil = openprt("StyleReport");
	if (ofil==NULL) return(FALSE);
	
	fprintf(ofil,"%s Style Listing ",ucadname);
	if (Savnam) fprintf(ofil,"(File %s)",Savefile);
	fprintf(ofil,"\n\n\r");
	
	fprintf(ofil,"   Name of Lettering Style--------Counts-Font\n\n\r");
	
	for (looper=Root_Style; looper!=NULL; looper=looper->next_style) 
		fprintf(ofil,"   %-32.32s %4d %s\n\r",
			looper->Letter_Style_ID,looper->Use_Count,
			looper->draw_font->Font_Name_ID);

	fclose(ofil);
	
}

extern STYLE_A default_styles;

Style_Add()
{

	BOOL getback;
	STYLE_A *newstyle;

	if (Current_Style==NULL) Current_Style=Root_Style;

	getback=Style_Sc(NULL,"Enter New Lettering Style",TRUE);
	
	CleanOutArgText();
	
	if (!getback) return;

	newstyle = (STYLE_A *) 
		GI_RemAlloc(sizeof (STYLE_A));
	if (newstyle==NULL) {
		DispMess("Insufficient memory to create new style - sorry");
		return;
	}

	if (Current_Style==NULL) *newstyle = default_styles;
	else	*newstyle = *Current_Style;

	ArgText[30] = '\0';	/* Limit to 30 chars */
	sprintf(newstyle->Letter_Style_ID,"%s",ArgText);
	newstyle->Use_Count = 0;

	Link_Style(newstyle);

	Current_Style = newstyle;

	Style_Current();		/* Fix up the new style */

}
Style_Rename()
{

	BOOL getans;
	STYLE_A *getstyle;

	if (Current_Style==NULL) Current_Style=Root_Style;
	if (Current_Style==NULL) return;

	getans = Style_Sc(Current_Style,"Select Style to Rename",FALSE);

	if (!getans) return;
	if (scrollanswer==NULL) {
		oneliner("Pick a style that already exists!");
		return;
	}
	getstyle = scrollanswer;

	strcpy(ArgText,getstyle->Letter_Style_ID);
	BoxReq("Enter New Style Name",ArgText);
	CleanOutArgText();
	
	if (ArgText[0]=='\0') return;

	Unlink_Style(getstyle);
	sprintf(getstyle->Letter_Style_ID,"%s",ArgText);
	getstyle->next_style = getstyle->prev_style = NULL;

	Link_Style(getstyle);

	return;		
	
}

Style_Merge()
{

	STYLE_A	*getstyle,*tostyle;
	BOOL	getans;
	struct drawing_node *lnklst;

	if (Current_Style==NULL) Current_Style=Root_Style;
	if (Current_Style==NULL) return;

	getans=Style_Sc(Current_Style,"Merge Lettering Styles",FALSE);

	if (!getans) return;
	if (scrollanswer==NULL) {
		oneliner("Pick a style that already exists!");
		return;
	}
	getstyle = scrollanswer;

	getans = Style_Sc(NULL,"Merge Into Which Style",FALSE);
	if (!getans) return;
	if (scrollanswer==NULL) {
		oneliner("Pick a style that already exists!");
		return;
	}
	tostyle = scrollanswer;

	if (getstyle==tostyle) return;	/* Nothing done */

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
		if (lnklst->node_id!=NODE_TEXT) continue;
		if (GetTextData(lnklst)->letstyle==getstyle) 
			GetTextData(lnklst)->letstyle=tostyle;
	}

	if (Current_Style==getstyle) Current_Style = tostyle;
	if (Default_Style==getstyle) Default_Style = tostyle;
	if (Dim_Box_Style==getstyle) Dim_Box_Style = tostyle;

	Unlink_Style(getstyle);	/* Get rid of old style */

	InventoryAll(FALSE);	/* Get a new inventory count */

}

UBYTE StyleWar[2][60];
struct IntuiText Style_text[] = {
 { 2, 1, JAM2, 6, 3, NULL, "ABORT", NULL },
 { 2, 1, JAM2, 6, 3, NULL, "PROCEED", NULL },
 { 5, 1, JAM2, 6, 5, NULL, "Carnage Warning:", &Style_text[3] },
 { 2, 1, JAM2, 6, 15, NULL, &StyleWar[0][0], &Style_text[4] },
 { 2, 1, JAM2, 6, 25, NULL, &StyleWar[1][0], NULL },
};

Style_Kill()
{

	STYLE_A	*getstyle,*tostyle;
	BOOL	getans;
	struct drawing_node *lnklst;

	if (Current_Style==NULL) Current_Style=Root_Style;
	if (Current_Style==NULL) return;

	getans=Style_Sc(Current_Style,"Lettering Style to Kill",FALSE);

	if (!getans) return;
	if (scrollanswer==NULL) {
		oneliner("Pick a style that already exists!");
		return;
	}
	getstyle = scrollanswer;

	sprintf(&StyleWar[0][0],"Killing Style: %s",getstyle->Letter_Style_ID);
	sprintf(&StyleWar[1][0],"Deleting %d Objects",getstyle->Use_Count);

	if (complain("ABORT","PROCEED",5,2,2,2,
		"Carnage Warning:",
		&StyleWar[0][0],
		&StyleWar[1][0],NULL)) return;

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
		if (lnklst->node_id!=NODE_TEXT) continue;
		if (GetTextData(lnklst)->letstyle!=getstyle) continue;
		if (lnklst->node_flags&NODE_ONSCREEN) {
			lnklst->node_flags &= (~NODE_SELECTED);
			lnklst->node_flags |= NODE_DELETED;
			GenericRender(lnklst);		/* Erase from screen */
		}
		DeleteNode(lnklst);
	}

	Unlink_Style(getstyle);	/* Get rid of old style */

	if (Current_Style==getstyle) Current_Style = Root_Style;
	if (Default_Style==getstyle) Default_Style = Root_Style;
	if (Dim_Box_Style==getstyle) Dim_Box_Style = Root_Style;

	InventoryAll(FALSE);	/* Get a new inventory count */

}

Style_Expunge()
{

	int needexp;
	STYLE_A *looper;

	InventoryAll(TRUE);	/* Find out whats in use */

	needexp = 0;
	for (looper=Root_Style; looper!=NULL; looper=looper->next_style) 
		if (looper->Use_Count<0) needexp++;

	if (needexp>0) {
		sprintf(&StyleWar[0][0],"Expunging %d Unused Styles",needexp);
		sprintf(&StyleWar[1][0]," ");

		if (!complain("ABORT","PROCEED",5,2,2,2,
			"Carnage Warning:",
			&StyleWar[0][0],
			&StyleWar[1][0],NULL)) {

			for (looper=Root_Style; looper!=NULL; 
				looper=looper->next_style) 
					if (looper->Use_Count<0)
						Unlink_Style(looper);
		}
	}

	InventoryAll(FALSE);
}
	
LOCAL struct Text_Fonts *hold_font=NULL;

VOID *Stroke_First();
VOID *Stroke_Next();
VOID *Stroke_Prev();
VOID *Stroke_Search();
VOID *Stroke_Parent();
VOID *Stroke_Descend();
VOID *Stroke_Form();
VOID *Stroke_Name();

BOOL Stroke_Get()
{

	return(Scroller(Stroke_First,
		Stroke_Next,
		Stroke_Prev,
		Stroke_Search,
		Stroke_Parent,
		Stroke_Descend,
		Stroke_Form,
		Stroke_Name,
		FALSE,
		TRUE,
		FALSE,
		Current_Style->draw_font,
		"    OK    ",
		"  Cancel  ",
		"Select New Current Font",
		"Name of Stroke Font-------------Counts--Size",
		&hold_font,
		FntMask,
		"(Font Table)"));
}

/*---------------------Lettering Style Edit Requester---------------------*/

LOCAL BOOL exflag;
LOCAL int postaction=0;
LOCAL STYLE_A cancelstyle;
LOCAL struct Requester StyleEditRequester;
VOID save_curstyle();

VOID SEHandler(ingad)
struct Gadget *ingad;
{

	CloseRequest(&StyleEditRequester,w);	/* Close manually */

	switch (ingad->GadgetID) {
	case 41:
		save_curstyle();
		postaction = 4;
		Current_Style = Current_Style->prev_style;
		if (Current_Style==NULL) Current_Style = Style_Last;
		return;
	case 42:
		save_curstyle();
		postaction = 4;
		Current_Style = Current_Style->next_style;
		if (Current_Style==NULL) Current_Style = Root_Style;
		return;
	case 43:
		postaction = 10;
		return;
	case 44:
		postaction = 11;
		return;
	case 45:
		postaction = 12;
		return;
	case 46:
		postaction = 13;
		return;
	case 47:
		postaction = 14;
		return;
	case 20:
		*Current_Style = cancelstyle;
		exflag=FALSE;
		postaction = 0;
		return;
	case 21:
		exflag=FALSE;
		postaction = 3;
		return;
	case 22:
		postaction = 2;
		return;
	case 23:
		postaction = 1;
		return;
	case 31:
		postaction = 40;
		return;
	}

	return;

}

LOCAL UBYTE SEData[4][32];
LOCAL UBYTE SEComment[82];
LOCAL struct StringInfo SEStringSpec[] = {
	{ &SEData[0][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &SEData[1][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &SEData[2][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &SEData[3][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &SEComment[0], undobuf, 0, 80,0,0,0,0,0,0,NULL,0,NULL }};

VOID EditBeepRestore()
{

	int a;

	sprintf(&SEData[0][0],"%s",DispLin(Current_Style->Prime_Height));
	sprintf(&SEData[1][0],"%s",DispAng(Current_Style->Slant*PID4));
	sprintf(&SEData[2][0],"%s",DispLin(Current_Style->Prime_Height));
	sprintf(&SEData[3][0],"%.3f",Current_Style->Aspect);
	
	for (a=0; a<5; a++) StgSet(&SEStringSpec[a]);

	DisplayBeep(screen);

	RefreshGadgets(StyleEditRequester.ReqGadget,w,&StyleEditRequester);
	return;
}

VOID SEStringHandler(ingad)
struct Gadget *ingad;
{

	int button,a,gpos;
	STDFLT inquery;
	BOOL	inchk;

	/* Forbid();
	ingad->Flags &= ~SELECTED;
	Permit();	*/

	button = ingad->GadgetID-30;
	if (button==4) return;

	gpos = RemoveGList(w,ingad,1);		/* Take out */

	switch(button) {
	case 0:
	case 2:
		inquery = ParLin(&SEData[button][0],&inchk);
		break;
	case 1:
		inquery = ParAng(&SEData[button][0],&inchk);
		break;
	case 3:
		inchk = FALSE;
		if (sscanf(&SEData[button][0],"%f",&inquery)!=1) inchk=TRUE;
		break;
	}

	if (inchk) {
		EditBeepRestore();
		if (gpos>=0) AddGList(w,ingad,gpos,1,&StyleEditRequester);
		return;
	}

	switch (button) {
	case 0:
	case 2:
		if (inquery<=0.0) {
			EditBeepRestore();
			if (gpos>=0) AddGList(w,ingad,gpos,1,&StyleEditRequester);
			return;
		} 
		Current_Style->Prime_Height = inquery;
		break;
	case 1:	
		if (inquery>=(TWOPI-PID4)) inquery = inquery - TWOPI;
		if (inquery>PID4||inquery<-PID4) {
			EditBeepRestore();
			if (gpos>=0) AddGList(w,ingad,gpos,1,&StyleEditRequester);
			return;
		}
		Current_Style->Slant = inquery / PID4;
		break;
	case 3:	
		if (inquery<=0) {
			EditBeepRestore();
			if (gpos>=0) AddGList(w,ingad,gpos,1,&StyleEditRequester);
			return;
		}
		Current_Style->Aspect = inquery;
		break;
	}

	sprintf(&SEData[0][0],"%s",DispLin(Current_Style->Prime_Height));
	sprintf(&SEData[1][0],"%s",DispAng(Current_Style->Slant*PID4));
	sprintf(&SEData[2][0],"%s",DispLin(Current_Style->Prime_Height));
	sprintf(&SEData[3][0],"%.3f",Current_Style->Aspect);
	
	for (a=0; a<5; a++) StgSet(&SEStringSpec[a]);

	RefreshGadgets(StyleEditRequester.ReqGadget,w,&StyleEditRequester);

	if (gpos>=0) AddGList(w,ingad,gpos,1,&StyleEditRequester);
	
	return;

}

extern struct Image add_s_image;		/* 59 * 15 */
extern struct Image add_u_image;
extern struct Image rename_s_image;		/* 59 * 15 */
extern struct Image rename_u_image;
extern struct Image kill_s_image;		/* 59 * 15 */
extern struct Image kill_u_image;
extern struct Image expunge_s_image;		/* 63 * 15 */
extern struct Image expunge_u_image;
extern struct Image merge_s_image;		/* 59 * 15 */
extern struct Image merge_u_image;
extern struct Image uparrow_u_image;		/* 20 * 17 */
extern struct Image uparrow_s_image;
extern struct Image dnarrow_u_image;
extern struct Image dnarrow_s_image;
extern struct Image ldfont_u_image;		/* 85 * 14 */
extern struct Image ldfont_s_image;

#define REQWID 450

LOCAL UBYTE SEText[2][32];
LOCAL struct IntuiText SEListText[] = {
	{ 2,3,JAM2,0,0,NULL,"  Cancel  ",NULL },
	{ 2,3,JAM2,0,0,NULL,"    OK    ",NULL },
	{ 3,2,JAM2,0,0,NULL,&SEText[0][0],NULL },
	{ 3,2,JAM2,0,0,NULL,&SEText[1][0],NULL }};
LOCAL struct Gadget SEGadgets[] = {
	{ &SEGadgets[1], 235, 59, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &SEStringSpec[0], 30, SEStringHandler  },
	{ &SEGadgets[3], 125, 69, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &SEStringSpec[1], 31, SEStringHandler  },
	  /* Skip over the sheet height */
	{ &SEGadgets[3], 350, 59, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &SEStringSpec[2], 32, SEStringHandler  },
	{ &SEGadgets[4], 350, 69, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &SEStringSpec[3], 33, SEStringHandler  },
	{ NULL, 80, 79, 350, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &SEStringSpec[4], 34, SEStringHandler  }};

#define HILITE GADGIMAGE | GADGHIMAGE
#define SEACTIVE GADGIMMEDIATE | RELVERIFY 
LOCAL struct Gadget SEList[] = {
	{ &SEList[1], 270,92,80,8,GADGHCOMP,SEACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&SEListText[0],0,NULL,20,SEHandler},
	{ &SEList[2], 100,92,80,8,GADGHCOMP,SEACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&SEListText[1],0,NULL,21,SEHandler},
	{ &SEList[3], 85,17,240,8,GADGHCOMP,SEACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&SEListText[2],0,NULL,22,SEHandler},
	{ &SEList[4], 85,27,240,8,GADGHCOMP,SEACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&SEListText[3],0,NULL,23,SEHandler},

	{ &SEList[5], 335,19,85,14,HILITE,SEACTIVE,BOOLGADGET|REQGADGET,
		&ldfont_u_image,&ldfont_s_image,NULL,0,NULL,31,SEHandler},
	
#define TP 39
#define BUTBLK (((REQWID-20-(302+64))/2)+10)
	{ &SEList[6], BUTBLK,TP,29,15,HILITE,SEACTIVE,BOOLGADGET|REQGADGET,
		&uparrow_u_image,&uparrow_s_image,NULL,0,NULL,41,SEHandler},
	{ &SEList[7], BUTBLK+30,TP,29,15,HILITE,SEACTIVE,BOOLGADGET|REQGADGET,
		&dnarrow_u_image,&dnarrow_s_image,NULL,0,NULL,42,SEHandler},
	{ &SEList[8], BUTBLK+62,TP,59,15,HILITE,SEACTIVE,BOOLGADGET|REQGADGET,
		&add_u_image,&add_s_image,NULL,0,NULL,43,SEHandler},
	{ &SEList[9], BUTBLK+122,TP,59,15,HILITE,SEACTIVE,BOOLGADGET|REQGADGET,
		&rename_u_image,&rename_s_image,NULL,0,NULL,44,SEHandler},
	{ &SEList[10], BUTBLK+182,TP,59,15,HILITE,SEACTIVE,BOOLGADGET|REQGADGET,
		&kill_u_image,&kill_s_image,NULL,0,NULL,45,SEHandler},
	{ &SEList[11], BUTBLK+242,TP,59,15,HILITE,SEACTIVE,BOOLGADGET|REQGADGET,
		&merge_u_image,&merge_s_image,NULL,0,NULL,46,SEHandler},
	{ &SEGadgets[0], BUTBLK+302,TP,63,15,HILITE,SEACTIVE,BOOLGADGET|REQGADGET,
		&expunge_u_image,&expunge_s_image,NULL,0,NULL,47,SEHandler},
};

LOCAL struct IntuiText SEHeadings[] = {
	{ 3,1,JAM2,5,5,NULL,"Current Lettering Style Settings:",&SEHeadings[1]},
	{ 2,1,JAM2,25,17,NULL,"Style:",&SEHeadings[2]},
	{ 2,1,JAM2,25,27,NULL," Font:",&SEHeadings[3]},
	{ 2,1,JAM2,122,59,NULL," Font Height:",&SEHeadings[4]},
	{ 2,1,JAM2,12,69,NULL," Slant Angle:",&SEHeadings[5]},
	{ 2,1,JAM2,9,79,NULL,"Comment:",&SEHeadings[7]},
	{ 2,1,JAM2,238,59,NULL,"Sheet Height:",&SEHeadings[7]},
	{ 2,1,JAM2,238,69,NULL,"Aspect Ratio:",NULL }};

VOID save_curstyle()
{
	if (strcmp(SEComment,Current_Style->Comment)!=0) 
		Current_Style->Comment = StoreComment(SEComment);
}

Style_Current()
{

	STYLE_A *beginstyle,*tstyle;
	UBYTE centerbuf[55],*dbuf,*incbuf;
	int numspa,a;
	struct RastPort *reqrp;

	Text_Box_Dirty = TRUE;		/* Force box size reeval in textmode */
	
	beginstyle = Current_Style;
	cancelstyle = *Current_Style;
	exflag = TRUE;

	while (exflag) {

		InitRequester(&StyleEditRequester);

		sprintf(SEComment,"%s",Current_Style->Comment);

		sprintf(&SEData[0][0],"%s",DispLin(Current_Style->Prime_Height));
		sprintf(&SEData[1][0],"%s",DispAng(Current_Style->Slant*PID4));
		sprintf(&SEData[2][0],"%s",DispLin(Current_Style->Prime_Height));
		sprintf(&SEData[3][0],"%.3f",Current_Style->Aspect);
	
		StyleEditRequester.Width=REQWID;
		StyleEditRequester.Height=105;
		StyleEditRequester.LeftEdge=(XSsize-StyleEditRequester.Width)/2;
		StyleEditRequester.TopEdge=(YSsize-StyleEditRequester.Height)/2;

		StyleEditRequester.BackFill=1;

		StyleEditRequester.ReqText = &SEHeadings[0];
		StyleEditRequester.ReqGadget = &SEList[0];

		dbuf = Current_Style->Letter_Style_ID;
		numspa = (30 - strlen(dbuf))/2;
		for (incbuf = centerbuf; numspa>=0; numspa--,incbuf++) 
				*incbuf=' ';
		sprintf(incbuf,"%-30.30s",dbuf);
		sprintf(&SEText[0][0],"%-30.30s",centerbuf);
	
		dbuf = Current_Style->draw_font->Font_Name_ID;
		numspa = (30 - strlen(dbuf))/2;
		for (incbuf = centerbuf; numspa>=0; numspa--,incbuf++) 
				*incbuf=' ';
		sprintf(incbuf,"%-30.30s",dbuf);
		sprintf(&SEText[1][0],"%-30.30s",centerbuf);

		for (a=0; a<5; a++) StgSet(&SEStringSpec[a]);

#ifdef _TURNSY
		StyleEditRequester.Flags = NOISYREQ;
#endif
	
		SafeRequest(&StyleEditRequester, w);

		postaction = 0;

		reqrp = StyleEditRequester.ReqLayer->rp;	/* Nasty */
		
		DoBorders(&StyleEditRequester,14,37,56);

		RefreshGList(&SEList[5],w,&StyleEditRequester,7);
		
		sit_n_spin(&StyleEditRequester,0,&SEList[0],&SEList[1]);	
			/* Wait for requesters to exit */

		switch (postaction) {
		case 1:
			if (Stroke_Get()&&scrollanswer!=NULL) 
				Current_Style->draw_font = scrollanswer;
			break;
		case 2:			/* Undo old changes, switch over */
			save_curstyle();
			if (!Style_NewCurrent()) break;
			if (scrollanswer==NULL) {
				oneliner("Pick a style that already exists!");
				break;
			}
			Current_Style = (STYLE_A *) scrollanswer;
			beginstyle = Current_Style;
			cancelstyle = *Current_Style;
			break;
		case 3:
			save_curstyle();
			beginstyle = Current_Style;
			cancelstyle = *Current_Style;
			break;
		case 4:
			beginstyle = Current_Style;
			cancelstyle = *Current_Style;
			break;
		case 10:
			save_curstyle();
			Style_Add();
			beginstyle = Current_Style;
			cancelstyle = *Current_Style;
			break;
		case 11:
			save_curstyle();
			Style_Rename();
			beginstyle = Current_Style;
			cancelstyle = *Current_Style;
			break;
		case 12:
			save_curstyle();
			Style_Kill();
			beginstyle = Current_Style;
			cancelstyle = *Current_Style;
			break;
		case 13:
			save_curstyle();
			Style_Merge();
			beginstyle = Current_Style;
			cancelstyle = *Current_Style;
			break;
		case 14:
			save_curstyle();
			Style_Expunge();
			beginstyle = Current_Style;
			cancelstyle = *Current_Style;
			break;
		case 40:
			Load_Font();
			break;
		}
				
	}
}
