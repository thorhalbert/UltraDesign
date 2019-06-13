/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	layerreq.c - Layer requesters
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
 *	6	28-Sep-88 SH	Add new buttons
 *	7	10-Nov-88 SH	Remove handle code to layerhandle.c
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include "attributes.h"

/*----------------------------------Macros-----------------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*------------------------Set up scroller for layers------------------------*/

LOCAL BOOL exflag;
LOCAL int postaction;

LAYER_A *hold_layer=NULL;

VOID *Layer_First();
VOID *Layer_Next();
VOID *Layer_Prev();
VOID *Layer_Search();
VOID *Layer_Parent();
VOID *Layer_Descend();
VOID *Layer_Form();
VOID *Layer_Name();

BOOL ReportLayer()
{

	LAYER_A *looper;

	FILE *ofil;

	ofil = openprt("LayerReport");
	if (ofil==NULL) return(FALSE);
	
	fprintf(ofil,"%s Layer Listing ",ucadname);
	if (Savnam) fprintf(ofil,"(File %s)",Savefile);
	fprintf(ofil,"\n\n\r");
	
	fprintf(ofil,"Name of Drawing Layer---------Counts\n\n\r");
	
	for (looper=Root_Layer; looper!=NULL; looper=looper->layer_next) 
		fprintf(ofil,"   %-32.32s %4d\n\r",
			looper->Layer_Name,looper->Use_Count);

	fclose(ofil);
	
}

BOOL Layer_Sc(start,title,new)
LAYER_A *start;
UBYTE *title;
BOOL new;
{

	return(Scroller(Layer_First,
		Layer_Next,
		Layer_Prev,
		Layer_Search,
		Layer_Parent,
		Layer_Descend,
		Layer_Form,
		Layer_Name,
		FALSE,
		TRUE,
		new,
		start,
		"    OK    ",
		"  Cancel  ",
		title,
		"Name of Drawing Layer---------Counts",
		&hold_layer,
		LayerMask,
		"(Layer Table)"));
}

BOOL Layer_NewCurrent()
{

	if (Current_Layer==NULL) Current_Layer=Root_Layer;
	if (Current_Layer==NULL) return;

	return(Layer_Sc(Current_Layer,"Select New Current Layer",FALSE));

}


Layer_Add()
{

	BOOL getback;
	LAYER_A *newlayer;

	if (Current_Layer==NULL) Current_Layer=Root_Layer;

	getback=Layer_Sc(NULL,"Enter New Layer Name",TRUE);

	CleanOutArgText();
	
	if (!getback) return;

	newlayer = (LAYER_A *) 
		GI_RemAlloc(sizeof (LAYER_A));
	if (newlayer==NULL) {
		DispMess("Insufficient memory to create new layer - sorry");
		return;
	}

	if (Current_Layer==NULL) *newlayer = default_layer;
	else	*newlayer = *Current_Layer;

	ArgText[30] = '\0';

	sprintf(newlayer->Layer_Name,"%s",ArgText);
	newlayer->Use_Count = 0;

	Link_Layer(newlayer);

	Current_Layer = newlayer;

	Layer_Current();		/* Fix up the new layer */

}

/*---------------------Drawing Layer Edit Requester---------------------*/

LOCAL LAYER_A cancellayer;
LOCAL struct Requester LayerEditRequester;

VOID save_curlayer();

VOID LEHandler(ingad)
struct Gadget *ingad;
{

	CloseRequest(&LayerEditRequester,w);

	switch (ingad->GadgetID) {
	case 41:
		save_curlayer();
		postaction = 4;
		Current_Layer = Current_Layer->layer_prev;
		if (Current_Layer==NULL) Current_Layer = Last_Layer;
		return;
	case 42:
		save_curlayer();
		postaction = 4;
		Current_Layer = Current_Layer->layer_next;
		if (Current_Layer==NULL) Current_Layer = Root_Layer;
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
		*Current_Layer = cancellayer;
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
	}

	return;

}

LOCAL UBYTE LEComment[82];
LOCAL struct StringInfo LEStringSpec[] = {
	{ &LEComment[0], undobuf, 0, 80,0,0,0,0,0,0,NULL,0,NULL }};

VOID LEStringHandler(ingad)
struct Gadget *ingad;
{

	int button;

	Forbid();
	ingad->Flags &= ~SELECTED;	/* Make sure it is unselected */
	Permit();

	button = ingad->GadgetID-30;
	if (button==4) return;

	return;

}

extern struct Image attrib_1s_image;
extern struct Image attrib_1u_image;
extern struct Image attrib_2s_image;
extern struct Image attrib_2u_image;
extern struct Image attrib_3s_image;
extern struct Image attrib_3u_image;
extern struct Image attrib_4s_image;
extern struct Image attrib_4u_image;
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

LOCAL UBYTE LEText[2][32];
LOCAL struct IntuiText LEListText[] = {
	{ 2,3,JAM2,0,0,NULL,"  Cancel  ",NULL },
	{ 2,3,JAM2,0,0,NULL,"    OK    ",NULL },
	{ 3,2,JAM2,0,0,NULL,&LEText[0][0],NULL },
	{ 3,2,JAM2,0,0,NULL,"        ",NULL }};
LOCAL struct Gadget LEGadgets[] = {
	{ NULL, 80, 79, 350, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &LEStringSpec[0], 34, LEStringHandler  }};
#define LEACTIVE GADGIMMEDIATE | RELVERIFY
#define LETACTIVE GADGIMMEDIATE | RELVERIFY | TOGGLESELECT
#define HILITE GADGIMAGE | GADGHIMAGE
LOCAL struct Gadget LEList[] = {
	{ &LEList[1], 270,94,80,8,GADGHCOMP,LEACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&LEListText[0],0,NULL,20,LEHandler},
	{ &LEList[2], 100,94,80,8,GADGHCOMP,LEACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&LEListText[1],0,NULL,21,LEHandler},
	{ &LEList[3], 65,17,240,8,GADGHCOMP,LEACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&LEListText[2],0,NULL,22,LEHandler},
	{ &LEList[4], 365,17,67,8,GADGHCOMP,LEACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&LEListText[3],0,NULL,23,LEHandler},

#define REQWID 450

#define OTLEF (((REQWID-20-120)/2)+10)
	{ &LEList[5], OTLEF,47,31,27,HILITE,LETACTIVE,BOOLGADGET|REQGADGET,
		&attrib_1s_image,&attrib_1u_image,NULL,0,NULL,40,NULL},
	{ &LEList[7], OTLEF+40,47,31,27,HILITE,LETACTIVE,BOOLGADGET|REQGADGET,
		&attrib_2s_image,&attrib_2u_image,NULL,0,NULL,40,NULL},
		/* Skip around lock button */
	{ &LEList[7], OTLEF+40,47,31,27,HILITE,LETACTIVE,BOOLGADGET|REQGADGET,
		&attrib_3s_image,&attrib_3u_image,NULL,0,NULL,40,NULL},
	{ &LEList[8], OTLEF+80,47,31,27,HILITE,LETACTIVE,BOOLGADGET|REQGADGET,
		&attrib_4u_image,&attrib_4s_image,NULL,0,NULL,40,NULL},

#define BUTBLK (((REQWID-20-(302+64))/2)+10)
	{ &LEList[9], BUTBLK,28,29,15,HILITE,LEACTIVE,BOOLGADGET|REQGADGET,
		&uparrow_u_image,&uparrow_s_image,NULL,0,NULL,41,LEHandler},
	{ &LEList[10], BUTBLK+30,28,29,15,HILITE,LEACTIVE,BOOLGADGET|REQGADGET,
		&dnarrow_u_image,&dnarrow_s_image,NULL,0,NULL,42,LEHandler},
	{ &LEList[11], BUTBLK+62,28,59,15,HILITE,LEACTIVE,BOOLGADGET|REQGADGET,
		&add_u_image,&add_s_image,NULL,0,NULL,43,LEHandler},
	{ &LEList[12], BUTBLK+122,28,59,15,HILITE,LEACTIVE,BOOLGADGET|REQGADGET,
		&rename_u_image,&rename_s_image,NULL,0,NULL,44,LEHandler},
	{ &LEList[13], BUTBLK+182,28,59,15,HILITE,LEACTIVE,BOOLGADGET|REQGADGET,
		&kill_u_image,&kill_s_image,NULL,0,NULL,45,LEHandler},
	{ &LEList[14], BUTBLK+242,28,59,15,HILITE,LEACTIVE,BOOLGADGET|REQGADGET,
		&merge_u_image,&merge_s_image,NULL,0,NULL,46,LEHandler},
	{ &LEGadgets[0], BUTBLK+302,28,63,15,HILITE,LEACTIVE,BOOLGADGET|REQGADGET,
		&expunge_u_image,&expunge_s_image,NULL,0,NULL,47,LEHandler},
};

LOCAL struct IntuiText LEHeadings[] = {
	{ 3,1,JAM2,8,6,NULL,"Current Drawing Layer Settings:",&LEHeadings[1]},
	{ 2,1,JAM2,8,17,NULL,"Layer:",&LEHeadings[2]},
	{ 2,1,JAM2,310,17,NULL,"Color:",&LEHeadings[3]},
	{ 2,1,JAM2,8,79,NULL,"Comment:",NULL }};

VOID save_curlayer()
{
	Current_Layer->Layer_Flags &= ~(LAYER_GHOSTED|LAYER_DOFF|
		LAYER_LOCKED|LAYER_POFF); 
	if (LEList[4].Flags & SELECTED) 
		Current_Layer->Layer_Flags |= LAYER_GHOSTED; 
	if (LEList[5].Flags & SELECTED) 
		Current_Layer->Layer_Flags |= LAYER_DOFF; 
	if (LEList[6].Flags & SELECTED) 
		Current_Layer->Layer_Flags |= LAYER_LOCKED; 
	if (LEList[7].Flags & SELECTED) 
		Current_Layer->Layer_Flags |= LAYER_POFF; 

	if (strcmp(Current_Layer->Comment,LEComment)!=0) 
	   Current_Layer->Comment = StoreComment(LEComment);
}

Layer_Current()
{

	LAYER_A *beginlayer,*tlayer;
	UBYTE centerbuf[55],*dbuf,*incbuf;
	int numspa;
	struct RastPort *reqrp;
		
	beginlayer = Current_Layer;
	cancellayer = *Current_Layer;
	exflag = TRUE;

	while (exflag) {

		InitRequester(&LayerEditRequester);

		LayerEditRequester.Width=REQWID;
		LayerEditRequester.Height=109;
		LayerEditRequester.LeftEdge=(XSsize-LayerEditRequester.Width)/2;
		LayerEditRequester.TopEdge=(YSsize-LayerEditRequester.Height)/2;

		LayerEditRequester.BackFill=1;

		LayerEditRequester.ReqText = &LEHeadings[0];
		LayerEditRequester.ReqGadget = &LEList[0];

		dbuf = Current_Layer->Layer_Name;
		numspa = (30 - strlen(dbuf))/2;
		for (incbuf = centerbuf; numspa>=0; numspa--,incbuf++) 
				*incbuf=' ';
		sprintf(incbuf,"%-30.30s",dbuf);
		sprintf(&LEText[0][0],"%-30.30s",centerbuf);
	
		sprintf(&LEText[1][0],"%30s"," ");
		LEListText[3].FrontPen = Current_Layer->Color_ID;
		LEListText[3].BackPen = Current_Layer->Color_ID;

		sprintf(LEComment,"%s",Current_Layer->Comment);

		LEList[4].Flags &= (~SELECTED);
		LEList[5].Flags &= (~SELECTED);
		LEList[6].Flags &= (~SELECTED);
		LEList[7].Flags &= (~SELECTED);
		if (Current_Layer->Layer_Flags & LAYER_GHOSTED) 
			LEList[4].Flags |= SELECTED;
		if (Current_Layer->Layer_Flags & LAYER_DOFF) 
			LEList[5].Flags |= SELECTED;
		if (Current_Layer->Layer_Flags & LAYER_LOCKED) 
			LEList[6].Flags |= SELECTED;
		if (Current_Layer->Layer_Flags & LAYER_POFF) 
			LEList[7].Flags |= SELECTED;
			
#ifdef _TURNSY
		LayerEditRequester.Flags = NOISYREQ;
#endif
			
		SafeRequest(&LayerEditRequester, w);

		postaction = 0;

		reqrp = LayerEditRequester.ReqLayer->rp;	/* Nasty */

		DoBorders(&LayerEditRequester,15,26,75);

		RefreshGList(&LEList[4],w,&LayerEditRequester,11);
		
		sit_n_spin(&LayerEditRequester,0,&LEList[0],&LEList[1]);
			/* Wait for requesters to exit */

		switch (postaction) {
		case 1:
			save_curlayer();
			Current_Layer->Color_ID=
				Palette(Current_Layer->Color_ID);
			break;
		case 2:			/* Undo old changes, switch over */
			save_curlayer();
			if (!Layer_NewCurrent()) break;
			if (scrollanswer==NULL) {
				oneliner("Pick a layer that exists!");
				break;
			}
			Current_Layer = (LAYER_A *) scrollanswer;
			beginlayer = Current_Layer;
			cancellayer = *Current_Layer;
			break;
						/* No break -- fall into 3 */
		case 3:
			save_curlayer();
			beginlayer = Current_Layer;
			cancellayer = *Current_Layer;

			break;
		case 4:
			beginlayer = Current_Layer;
			cancellayer = *Current_Layer;

			break;
		case 10:
			save_curlayer();
			Layer_Add();
			beginlayer = Current_Layer;
			cancellayer = *Current_Layer;

			break;
		case 11:
			save_curlayer();
			Layer_Rename();
			beginlayer = Current_Layer;
			cancellayer = *Current_Layer;

			break;
		case 12:
			save_curlayer();
			Layer_Kill();
			beginlayer = Current_Layer;
			cancellayer = *Current_Layer;

			break;
		case 13:
			save_curlayer();
			Layer_Merge();
			beginlayer = Current_Layer;
			cancellayer = *Current_Layer;

			break;
		case 14:
			save_curlayer();
			Layer_Expunge();
			beginlayer = Current_Layer;
			cancellayer = *Current_Layer;

			break;

		}
				
	}

}

Layer_Rename()
{

	BOOL getans;
	LAYER_A *getlayer;

	if (Current_Layer==NULL) Current_Layer=Root_Layer;
	if (Current_Layer==NULL) return;

	getans = Layer_Sc(Current_Layer,"Select Layer to Rename",FALSE);

	if (!getans) return;
	if (scrollanswer==NULL) {
		oneliner("Pick a layer that already exists!");
		return;
	}
	getlayer = scrollanswer;

	strcpy(ArgText,getlayer->Layer_Name);
	BoxReq("Enter New Layer Name",ArgText);
	CleanOutArgText();
	
	if (ArgText[0]=='\0') return;
	
	Unlink_Layer(getlayer);
	sprintf(getlayer->Layer_Name,"%s",ArgText);
	getlayer->layer_next = getlayer->layer_prev = NULL;
	getlayer->layer_next = getlayer->layer_prev = NULL;
	Link_Layer(getlayer);

	return;		
	
}

Layer_Merge()
{

	LAYER_A	*getlayer,*tolayer;
	BOOL	getans;
	struct drawing_node *lnklst;

	if (Current_Layer==NULL) Current_Layer=Root_Layer;
	if (Current_Layer==NULL) return;

	getans=Layer_Sc(Current_Layer,"Merge Layers",FALSE);

	if (!getans) return;
	if (scrollanswer==NULL) {
		oneliner("Pick a layer that already exists!");
		return;
	}
	getlayer = scrollanswer;

	getans = Layer_Sc(NULL,"Merge Into Which Layer",FALSE);
	if (!getans) return;
	if (scrollanswer==NULL) {
		oneliner("Pick a layer that already exists!");
		return;
	}
	tolayer = scrollanswer;

	if (getlayer==tolayer) return;	/* Nothing done */

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
		if (lnklst->node_layer==getlayer) 
			lnklst->node_layer = tolayer;
		if (lnklst->node_flags&NODE_ONSCREEN) GenericRender(lnklst);
	}

	if (Current_Layer==getlayer) Current_Layer = tolayer;
	if (Dim_Box_Layer==getlayer) Dim_Box_Layer = tolayer;
	if (Wit_Layer==getlayer) Wit_Layer = tolayer;
	if (Dim_Line_Layer==getlayer) Dim_Line_Layer = tolayer;

	Unlink_Layer(getlayer);	/* Get rid of old layer */

	InventoryAll(FALSE);	/* Get a new inventory count */

}

UBYTE LayerWar[2][60];
struct IntuiText Layer_text[] = {
 { 2, 1, JAM2, 6, 3, NULL, "ABORT", NULL },
 { 2, 1, JAM2, 6, 3, NULL, "PROCEED", NULL },
 { 5, 1, JAM2, 6, 5, NULL, "Carnage Warning:", &Layer_text[3] },
 { 2, 1, JAM2, 6, 15, NULL, &LayerWar[0][0], &Layer_text[4] },
 { 2, 1, JAM2, 6, 25, NULL, &LayerWar[1][0], NULL },
};

Layer_Kill()
{

	LAYER_A	*getlayer,*tolayer;
	BOOL	getans;
	struct drawing_node *lnklst;

	if (Current_Layer==NULL) Current_Layer=Root_Layer;
	if (Current_Layer==NULL) return;

	getans=Layer_Sc(Current_Layer,"Layer to Kill",FALSE);

	if (!getans) return;
	if (scrollanswer==NULL) {
		oneliner("Pick a layer that already exists!");
		return;
	}
	getlayer = scrollanswer;

	sprintf(&LayerWar[0][0],"Killing Layer: %s",getlayer->Layer_Name);
	sprintf(&LayerWar[1][0],"Deleting %d Objects",getlayer->Use_Count);

	if (complain("ABORT","PROCEED",5,2,2,0,
		"Carnage Warning:",
		&LayerWar[0][0],
		&LayerWar[1][0])) return;

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
		if (lnklst->node_layer!=getlayer) continue;
		if (lnklst->node_flags&NODE_ONSCREEN) {
			lnklst->node_flags &= (~NODE_SELECTED);
			lnklst->node_flags |= NODE_DELETED;
			GenericRender(lnklst);		/* Erase from screen */
		}
		DeleteNode(lnklst);
	}

	Unlink_Layer(getlayer);	/* Get rid of old layer */

	if (Current_Layer==getlayer) Current_Layer = Root_Layer;
	if (Dim_Box_Layer==getlayer) Dim_Box_Layer = Root_Layer;
	if (Wit_Layer==getlayer) Wit_Layer = Root_Layer;
	if (Dim_Line_Layer==getlayer) Dim_Line_Layer = Root_Layer; 

	InventoryAll(FALSE);	/* Get a new inventory count */

}

Layer_Expunge()
{

	int needexp;
	LAYER_A *looper;

	InventoryAll(TRUE);	/* Find out whats in use */

	needexp = 0;
	for (looper=Root_Layer; looper!=NULL; looper=looper->layer_next) 
		if (looper->Use_Count<0) needexp++;

	if (needexp>0) {
		sprintf(&LayerWar[0][0],"Expunging %d Unused Layers",needexp);
		sprintf(&LayerWar[1][0]," ");

		if (!complain("ABORT","PROCEED",5,2,2,0,
			"Carnage Warning:",
			&LayerWar[0][0],
			&LayerWar[1][0])) {

			for (looper=Root_Layer; looper!=NULL; 
				looper=looper->layer_next) {
					/*fprintf(stdout,"%s count %d\n",
						looper->Layer_Name,
						looper->Use_Count);*/
					if (looper->Use_Count<0)
						Unlink_Layer(looper);
			}
		}
	}

	InventoryAll(FALSE);
}
