/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	groupreq.c - group requester
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
 *	1	23-Jul-87 SH	Begin writing
 *	2	 8-Aug-87 SH	Add the assembly icon
 *	3	10-Nov-88 SH	Extract stuff to grouphandle.c
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include "attributes.h"

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/
	
/*-------------------------Global Variable Definitions-----------------------*/

extern GROUP_A *savelast,*lastassy;
extern UBYTE GroupPath[300];
extern BOOL boxoff;

LOCAL GROUP_A cancelgroup;

BOOL Group_Add();
BOOL Group_Rename();
BOOL Group_Merge();
BOOL Group_Prune();
BOOL Group_Graft();
BOOL Group_Expunge();

/*-------------------------------Code Section--------------------------------*/

extern UBYTE	GroupMask[];
	
VOID *Group_First();
VOID *Group_Next();
VOID *Group_Prev();
VOID *Group_Search();
VOID *Group_Parent();
VOID *Group_Descend();
VOID *Group_Form();
VOID *Group_Name();
extern UBYTE GroupTitle[80];

BOOL Group_Sc(title,load,header,Holf,exist,setbox)
UBYTE *title,*load,*header;
GROUP_A **Holf;
BOOL exist;
BOOL setbox;
{

	BOOL ret;
	
	boxoff = setbox;
		
	ret=Scroller(Group_First,
		Group_Next,
		Group_Prev,
		Group_Search,
		Group_Parent,
		Group_Descend,
		Group_Form,
		Group_Name,
		TRUE,
		TRUE,
		exist,
		*Holf,
		load,
		"  Cancel  ",
		title,
		GroupTitle,
		Holf,
		GroupMask,
		GroupPath);

	return(ret);

}

GROUP_A *curgrp=NULL;

BOOL Group_GetCurrent()
{

	GROUP_A *savecurrent;
	savecurrent = Current_Group;

 	curgrp = (GROUP_A *) 2;

	if (!Group_Sc("Select Current Drawing Group","  Select  ","Group Name",
			&curgrp,FALSE,TRUE)) {
		Current_Group = savecurrent;
		setpath();

		return(FALSE);
	}

	if (ArgText[0]=='\0') curgrp = 2;

	if ((int) curgrp!=2) {
		oneliner("You must select a Drawing_Objects item!");
		Current_Group = savecurrent;
		setpath();
		return(FALSE);
	}

	setpath();
	return(TRUE);
}

BOOL Group_Add()
{

	BOOL	ret;
	struct	Group_Attrib NewGrp,*sgrp;
	UBYTE 	*sc;

	curgrp = NULL;
	ret=Group_Sc("Enter New Group to Add","    Add   ","Group Name",
		&curgrp,TRUE,FALSE);

	CleanOutArgText();

	if (!ret) return(FALSE);

	ArgText[30] = '\0';
	sc = ArgText;
	while (*sc==' '||*sc=='\t') sc++;

	sgrp = Group_Search(sc);

	if (sgrp!=NULL) {
		oneliner("Group cannot already exist at this level!");
		return(FALSE);
	}

	if (*sc=='\0') {
		oneliner("Enter a group name to add.");
		return(FALSE);
	}

	sprintf(NewGrp.Group_Name,"%s",sc);
	NewGrp.Use_Count = 0;
	NewGrp.Color_ID = Current_Group->Color_ID;
	NewGrp.Group_ID = 0;
	NewGrp.Group_Flags = 0;
	NewGrp.Libr_Flags = 0;
	NewGrp.Comment = " ";		/* Dummy comment */

	Current_Group = groupinsert(NewGrp,Current_Group,TRUE);

	setpath();

	Group_Current();
	return(FALSE);
}

/*---------------------Drawing Group Edit Requester---------------------*/

LOCAL GROUP_A *begingroup;

LOCAL BOOL exflag;
LOCAL int postaction;
extern UBYTE undobuf[];
LOCAL struct Requester GroupEditRequester;

VOID save_curgroup();

VOID GEHandler(ingad)
struct Gadget *ingad;
{
	GROUP_A *sav;
	
	sav = Current_Group;

	CloseRequest(&GroupEditRequester,w);

	switch (ingad->GadgetID) {
	case 20:
		Current_Group = begingroup;
		*begingroup = cancelgroup;
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
	case 41:
		if (Current_Group->parent==NULL) break;
		save_curgroup();
		Current_Group = Current_Group->previous;
		if (Current_Group==NULL) 
			Current_Group = sav->parent->desc_last;
		postaction = 4;
		break;
	case 42:
		if (Current_Group->parent==NULL) break;
		save_curgroup();
		Current_Group = Current_Group->next;
		if (Current_Group==NULL) 
			Current_Group = sav->parent->descendants;
		postaction = 4;
		break;
	case 43:
		save_curgroup();
		postaction = 10;
		break;
	case 44:
		save_curgroup();
		postaction = 11;
		break;
	case 45:
		save_curgroup();
		postaction = 12;
		break;
	case 46:
		save_curgroup();
		postaction = 13;
		break;
	case 47:
		save_curgroup();
		postaction = 14;
		break;
	case 48:
		save_curgroup();
		postaction = 15;
		break;
	case 51:
		save_curgroup();
		Current_Group = Root_Group;
		setpath();
		postaction = 4;
		break;
	case 52:
		save_curgroup();
		if (Current_Group->parent!=NULL) 
			Current_Group = Current_Group->parent;
		setpath();
		postaction = 4;
		break;
	case 53:
		save_curgroup();
		if (Current_Group->descendants!=NULL)
			Current_Group = Current_Group->descendants;
		setpath();
		postaction = 4;
		break;
	}

	return;

}

LOCAL UBYTE GEComment[82];
LOCAL struct StringInfo GEStringSpec[] = {
	{ &GEComment[0], undobuf, 0, 80,0,0,0,0,0,0,NULL,0,NULL }};

VOID GEStringHandler(ingad)
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
extern struct Image attrib_5s_image;
extern struct Image attrib_5u_image;
extern struct Image add_s_image;		/* 59 * 15 */
extern struct Image add_u_image;
extern struct Image rename_s_image;		/* 59 * 15 */
extern struct Image rename_u_image;
extern struct Image kill_s_image;		/* 59 * 15 */
extern struct Image kill_u_image;
extern struct Image expunge_s_image;		/* 63 * 15 */
extern struct Image expunge_u_image;
extern struct Image prune_s_image;		/* 59 * 15 */
extern struct Image prune_u_image;		
extern struct Image graft_s_image;		/* 59 * 15 */
extern struct Image graft_u_image;
extern struct Image merge_s_image;
extern struct Image merge_u_image;
extern struct Image uparrow_u_image;		/* 20 * 17 */
extern struct Image uparrow_s_image;
extern struct Image dnarrow_u_image;
extern struct Image dnarrow_s_image;
extern struct Image root_u_image;		/* 58 * 15 */
extern struct Image root_s_image;		/* 58 * 15 */
extern struct Image parent_u_image;		/* 58 * 15 */
extern struct Image parent_s_image;		/* 58 * 15 */
extern struct Image desc_u_image;		/* 85 * 14 */
extern struct Image desc_s_image;		/* 85 * 14 */

LOCAL UBYTE GEText[2][32];
LOCAL struct IntuiText GEListText[] = {
	{ 2,3,JAM2,0,0,NULL,"  Cancel  ",NULL },
	{ 2,3,JAM2,0,0,NULL,"    OK    ",NULL },
	{ 3,2,JAM2,0,0,NULL,&GEText[0][0],NULL },
	{ 3,2,JAM2,0,0,NULL,"        ",NULL }};
LOCAL struct Gadget GEGadgets[] = {
	{ NULL, 80, 102, 350, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &GEStringSpec[0], 34, GEStringHandler  }};
#define GEACTIVE GADGIMMEDIATE | RELVERIFY 
#define GETACTIVE GADGIMMEDIATE | RELVERIFY | TOGGLESELECT
#define HILITE GADGIMAGE | GADGHIMAGE
LOCAL struct Gadget GEList[] = {
	{ &GEList[1], 270,117,80,8,GADGHCOMP,GEACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&GEListText[0],0,NULL,20,GEHandler},
	{ &GEList[2], 100,117,80,8,GADGHCOMP,GEACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&GEListText[1],0,NULL,21,GEHandler},
	{ &GEList[3], 65,27,240,8,GADGHCOMP,GEACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&GEListText[2],0,NULL,22,GEHandler},
	{ &GEList[4], 365,27,67,8,GADGHCOMP,GEACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&GEListText[3],0,NULL,23,GEHandler},

#define REQWID 450

#define OTLEF (((REQWID-20-(4*40))/2)+10)
	{ &GEList[5], OTLEF,72,31,27,HILITE,GETACTIVE,BOOLGADGET|REQGADGET,
		&attrib_1s_image,&attrib_1u_image,NULL,0,NULL,40,NULL},
	{ &GEList[7], OTLEF+40,72,31,27,HILITE,GETACTIVE,BOOLGADGET|REQGADGET,
		&attrib_2s_image,&attrib_2u_image,NULL,0,NULL,40,NULL},
		/* Skip around the lock button */
	{ &GEList[7], OTLEF+80,72,31,27,HILITE,GETACTIVE,BOOLGADGET|REQGADGET,
		&attrib_3s_image,&attrib_3u_image,NULL,0,NULL,40,NULL},
	{ &GEList[8], OTLEF+80,72,31,27,HILITE,GETACTIVE,BOOLGADGET|REQGADGET,
		&attrib_4u_image,&attrib_4s_image,NULL,0,NULL,40,NULL},
	{ &GEList[9], OTLEF+120,72,31,27,HILITE,GETACTIVE,BOOLGADGET|REQGADGET,
		&attrib_5u_image,&attrib_5s_image,NULL,0,NULL,40,NULL},

#define BUTBLK (((REQWID-20-(362+64))/2)+10)
	{ &GEList[10], BUTBLK,54,29,15,HILITE,GEACTIVE,BOOLGADGET|REQGADGET,
		&uparrow_u_image,&uparrow_s_image,NULL,0,NULL,41,GEHandler},
	{ &GEList[11], BUTBLK+30,54,29,15,HILITE,GEACTIVE,BOOLGADGET|REQGADGET,
		&dnarrow_u_image,&dnarrow_s_image,NULL,0,NULL,42,GEHandler},
	{ &GEList[12], BUTBLK+62,54,59,15,HILITE,GEACTIVE,BOOLGADGET|REQGADGET,
		&add_u_image,&add_s_image,NULL,0,NULL,43,GEHandler},
	{ &GEList[13], BUTBLK+122,54,59,15,HILITE,GEACTIVE,BOOLGADGET|REQGADGET,
		&rename_u_image,&rename_s_image,NULL,0,NULL,44,GEHandler},
	{ &GEList[14], BUTBLK+182,54,59,15,HILITE,GEACTIVE,BOOLGADGET|REQGADGET,
		&merge_u_image,&merge_s_image,NULL,0,NULL,45,GEHandler},
	{ &GEList[15], BUTBLK+242,54,59,15,HILITE,GEACTIVE,BOOLGADGET|REQGADGET,
		&prune_u_image,&prune_s_image,NULL,0,NULL,46,GEHandler},
	{ &GEList[16], BUTBLK+302,54,59,15,HILITE,GEACTIVE,BOOLGADGET|REQGADGET,
		&graft_u_image,&graft_s_image,NULL,0,NULL,47,GEHandler},
	{ &GEList[17], BUTBLK+362,54,63,15,HILITE,GEACTIVE,BOOLGADGET|REQGADGET,
		&expunge_u_image,&expunge_s_image,NULL,0,NULL,48,GEHandler},

#define BUTBL1 (((REQWID-20-(120+85))/2)+10)
	{ &GEList[18], BUTBL1,39,59,15,HILITE,GEACTIVE,BOOLGADGET|REQGADGET,
		&root_u_image,&root_s_image,NULL,0,NULL,51,GEHandler},
	{ &GEList[19], BUTBL1+60,39,59,15,HILITE,GEACTIVE,BOOLGADGET|REQGADGET,
		&parent_u_image,&parent_s_image,NULL,0,NULL,52,GEHandler},
	{ &GEGadgets[0], BUTBL1+120,39,85,15,HILITE,GEACTIVE,BOOLGADGET|REQGADGET,
		&desc_u_image,&desc_s_image,NULL,0,NULL,53,GEHandler},
};

#define MAXP 44
UBYTE pathstr[MAXP+1];
LOCAL struct IntuiText GEHeadings[] = {
	{ 3,1,JAM2,7,5,NULL,"Current Drawing Group Settings:",&GEHeadings[1]},
	{ 2,1,JAM2,7,17,NULL," Path:",&GEHeadings[2]},
	{ 2,1,JAM2,65,17,NULL,pathstr,&GEHeadings[3]},
	{ 2,1,JAM2,7,27,NULL,"Group:",&GEHeadings[4]},
	{ 2,1,JAM2,310,27,NULL,"Color:",&GEHeadings[5]},
	{ 2,1,JAM2,7,102,NULL,"Comment:",NULL }};

VOID save_curgroup()
{
	begingroup->Libr_Flags = 0;
	begingroup->Group_Flags = 0;

	if (GEList[4].Flags & SELECTED) 
		begingroup->Group_Flags |= GROUP_GHOSTED; 
	if (GEList[5].Flags & SELECTED) 
		begingroup->Group_Flags |= GROUP_DOFF; 
	if (GEList[6].Flags & SELECTED) 
		begingroup->Group_Flags |= GROUP_LOCKED; 
	if (GEList[7].Flags & SELECTED) 
		begingroup->Group_Flags |= GROUP_POFF; 
	if (GEList[8].Flags & SELECTED) 
		begingroup->Libr_Flags |= LIBR_ASSY; 

	if (strcmp(begingroup->Comment,GEComment)!=0) 
		begingroup->Comment = StoreComment(GEComment);

	Current_Group = begingroup;
	setpath();
}
Group_Current()
{

	UBYTE centerbuf[55],*dbuf,*incbuf;
	int numspa;
	struct RastPort *reqrp;
	UBYTE *a,*b;

	begingroup = Current_Group;
	cancelgroup = *Current_Group;
	setpath();

	exflag = TRUE;

	while (exflag) {

		InitRequester(&GroupEditRequester);

		GroupEditRequester.Width=450;
		GroupEditRequester.Height=130;
		GroupEditRequester.LeftEdge=(XSsize-GroupEditRequester.Width)/2;
		GroupEditRequester.TopEdge=(YSsize-GroupEditRequester.Height)/2;

		GroupEditRequester.BackFill=1;

		GroupEditRequester.ReqText = &GEHeadings[0];
		GroupEditRequester.ReqGadget = &GEList[0];

		dbuf = begingroup->Group_Name;
		numspa = (30 - strlen(dbuf))/2;
		for (incbuf = centerbuf; numspa>=0; numspa--,incbuf++) 
				*incbuf=' ';
		sprintf(incbuf,"%-30.30s",dbuf);
		sprintf(&GEText[0][0],"%-30.30s",centerbuf);
	
		sprintf(&GEText[1][0],"%30s"," ");
		GEListText[3].FrontPen = begingroup->Color_ID;
		GEListText[3].BackPen = begingroup->Color_ID;

		sprintf(GEComment,"%s",begingroup->Comment);
		GEList[4].Flags &= (~SELECTED);
		GEList[5].Flags &= (~SELECTED);
		GEList[6].Flags &= (~SELECTED);
		GEList[7].Flags &= (~SELECTED);
		GEList[8].Flags &= (~SELECTED);
		if (begingroup->Group_Flags & GROUP_GHOSTED) 
			GEList[4].Flags |= SELECTED;
		if (begingroup->Group_Flags & GROUP_DOFF) 
			GEList[5].Flags |= SELECTED;
		if (begingroup->Group_Flags & GROUP_LOCKED) 
			GEList[6].Flags |= SELECTED;
		if (begingroup->Group_Flags & GROUP_POFF) 
			GEList[7].Flags |= SELECTED;
		if (begingroup->Libr_Flags & LIBR_ASSY) 
			GEList[8].Flags |= SELECTED;
			
		if (GroupPath[0]=='\0') strcpy(pathstr,"(Now at Root)");
		else {
			if (strlen(GroupPath)<MAXP) 
				strcpy(pathstr,GroupPath);	
			else	strcpy(pathstr,
					&GroupPath[strlen(GroupPath)-MAXP]);
		}
		
#ifdef _TURNSY
		GroupEditRequester.Flags = NOISYREQ;
#endif
	
		SafeRequest(&GroupEditRequester, w);

		reqrp = GroupEditRequester.ReqLayer->rp;	/* Nasty */
		
		postaction = 0;

		DoBorders(&GroupEditRequester,15,37,100);

		RefreshGList(&GEList[4],w,&GroupEditRequester,16);

		sit_n_spin(&GroupEditRequester,0,&GEList[0],&GEList[1]);	
			/* Wait for requesters to exit */

		switch (postaction) {
		case 1:
			save_curgroup();
			begingroup->Color_ID=
				Palette(begingroup->Color_ID);
			break;
		case 2:			/* Undo old changes, switch over */
			save_curgroup();
			Group_GetCurrent();
			begingroup = Current_Group;	/* Set new */
			cancelgroup = *begingroup;
			setpath();
			break;
		case 3:
			save_curgroup();
			if (!Library_Mode) break;	/* Only in lib mode */
			if (savelast!=lastassy) RepaintWindow();
			savelast = lastassy;
			break;		/* Assembly changed, so repaint */
		case 4:
			begingroup = Current_Group;
			cancelgroup = *begingroup;
			setpath();
			break;
		case 10:
			Group_Add();
			begingroup = Current_Group;
			cancelgroup = *begingroup;
			setpath();
			break;
		case 11:
			Group_Rename();
			begingroup = Current_Group;
			cancelgroup = *begingroup;
			setpath();
			break;
		case 12:
			Group_Merge();
			begingroup = Current_Group;
			cancelgroup = *begingroup;
			setpath();
			break;
		case 13:
			Group_Prune();
			begingroup = Current_Group;
			cancelgroup = *begingroup;
			setpath();
			break;
		case 14:
			Group_Graft();
			begingroup = Current_Group;
			cancelgroup = *begingroup;
			setpath();
			break;
		case 15:
			Group_Expunge();
			begingroup = Current_Group;
			cancelgroup = *begingroup;
			setpath();
			break;
		}
				
	}

}

BOOL Group_Rename()
{

	GROUP_A *savecurrent,*torename,*groupmother;
	savecurrent = Current_Group;

 	curgrp = (GROUP_A *) 2;

	if (!Group_Sc("Select Group To Rename","  Select  ","Group Name",
			&curgrp,FALSE,TRUE)) {
		Current_Group = savecurrent;
		setpath();

		return(FALSE);
	}

	if (ArgText[0]=='\0') curgrp = 2;


	if ((int) curgrp!=2) {
		oneliner("You must select a Drawing_Objects item!");
		Current_Group = savecurrent;
		setpath();
		return(FALSE);
	}

	if (Current_Group==Root_Group) {
		oneliner("You may not rename the root group!");
		Current_Group = savecurrent;
		setpath();
		return(FALSE);
	}

	torename = Current_Group;
 	curgrp = NULL;

	strcpy(ArgText,torename->Group_Name);
	BoxReq("Enter New Group Name",ArgText);
	CleanOutArgText();
	
	if (ArgText[0]=='\0') return;

	groupmother = torename->parent;

	Unlink_Group(torename);
	sprintf(torename->Group_Name,"%s",ArgText);
	Link_Group(torename,groupmother,FALSE);		/* Don't loose desc */

	Current_Group = savecurrent;
	setpath();
		
	return(TRUE);
}

BOOL Group_Merge()
{

	GROUP_A	*getgroup,*togroup,*savecurrent;
	BOOL	getans;
	struct drawing_node *lnklst;

	if (Current_Group==NULL) Current_Group = Root_Group;
	if (Current_Group==NULL) return;

	savecurrent = Current_Group;

 	curgrp = (GROUP_A *) 2;

	if (!Group_Sc("Select Group To Merge From","  Select  ","Group Name",
			&curgrp,FALSE,TRUE)) {
		Current_Group = savecurrent;
		setpath();

		return(FALSE);
	}

	if (ArgText[0]=='\0') curgrp = 2;


	if ((int) curgrp!=2) {
		oneliner("You must select a Drawing_Objects item!");
		Current_Group = savecurrent;
		setpath();
		return(FALSE);
	}

	getgroup = Current_Group;

 	curgrp = (GROUP_A *) 2;

	if (!Group_Sc("Select Group To Merge Into","  Select  ","Group Name",
			&curgrp,FALSE,TRUE)) {
		Current_Group = savecurrent;
		setpath();

		return(FALSE);
	}

	if ((int) curgrp!=2) {
		oneliner("You must select a Drawing_Objects item!");
		Current_Group = savecurrent;
		setpath();
		return(FALSE);
	}

	togroup = Current_Group;

	Current_Group = savecurrent;
	setpath();

	if (getgroup==togroup) return;	/* Nothing done */

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
		if (lnklst->above_group==getgroup) 
			lnklst->above_group = togroup;
		if (lnklst->node_flags&NODE_ONSCREEN) GenericRender(lnklst);
	}

	InventoryAll(TRUE);	/* Is it expendable ? */

	if (getgroup->Use_Count<0&&getgroup->descendants==NULL) {

		if (Current_Group==getgroup) Current_Group = togroup;
		if (Dim_Box_Group==getgroup) Dim_Box_Group = togroup;
		if (Wit_Group==getgroup) Wit_Group = togroup;
		if (Dim_Line_Group==getgroup) Dim_Line_Group = togroup;
		setpath();

		Unlink_Group(getgroup);	/* Get rid of old group */
	}

	InventoryAll(FALSE);	/* Get a new inventory count */

}

UBYTE GroupWar[2][60];
struct IntuiText Group_text[] = {
 { 2, 1, JAM2, 6, 3, NULL, "ABORT", NULL },
 { 2, 1, JAM2, 6, 3, NULL, "PROCEED", NULL },
 { 5, 1, JAM2, 6, 5, NULL, "Carnage Warning:", &Group_text[3] },
 { 2, 1, JAM2, 6, 15, NULL, &GroupWar[0][0], &Group_text[4] },
 { 2, 1, JAM2, 6, 25, NULL, &GroupWar[1][0], NULL },
};


KillBranch(ingroup)
GROUP_A *ingroup;
{

	GROUP_A *looper;
	struct drawing_node *lnklst;

	if (ingroup==NULL) return;

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
		if (lnklst->above_group!=ingroup) continue;
		if (lnklst->node_flags&NODE_ONSCREEN) {
			lnklst->node_flags &= (~NODE_SELECTED);
			lnklst->node_flags |= NODE_DELETED;
			GenericRender(lnklst);		/* Erase from screen */
			if (CurrentRoller==lnklst) 
				CurrentRoller=lnklst->node_subsequent;
		}
		DeleteNode(lnklst);
	}

	for (looper=ingroup->descendants; looper!=NULL; looper=looper->next)
		KillBranch(looper);

	if (ingroup!=Root_Group) Unlink_Group(ingroup);	/* Cannot kill root */
}
	

BOOL Group_Prune()
{

	GROUP_A	*getgroup,*togroup,*mother,*savecurrent;
	BOOL	getans;
	struct drawing_node *lnklst;
	int killgrp,killobj;

	if (Current_Group==NULL) Current_Group=Root_Group;
	if (Current_Group==NULL) return;

	savecurrent = Current_Group;

 	curgrp = (GROUP_A *) 2;

	if (!Group_Sc("Select Group to Prune","  Select  ","Group Name",
			&curgrp,FALSE,TRUE)) {
		Current_Group = savecurrent;
		setpath();

		return(FALSE);
	}

	if (ArgText[0]=='\0') curgrp = 2;


	if ((int) curgrp!=2) {
		oneliner("You must select a Drawing_Objects item!");
		Current_Group = savecurrent;
		setpath();
		return(FALSE);
	}

	getgroup = Current_Group;

	killgrp = 0;
	killobj = 0;
	CountBranch(getgroup,&killgrp,&killobj);

	sprintf(&GroupWar[0][0],"Pruning at Group: %s",getgroup->Group_Name);
	sprintf(&GroupWar[1][0],"Deleting %d Groups, %d Objects",
			killgrp,killobj);

	if (complain("ABORT","PROCEED",5,2,2,2,
		"Carnage Warning:",
		&GroupWar[0][0],
		&GroupWar[1][0])) return;
	
	mother = getgroup->parent;
	if (mother==NULL) mother = Root_Group;

	KillBranch(getgroup);

	if (Current_Group==getgroup) Current_Group = mother;
	if (Dim_Box_Group==getgroup) Dim_Box_Group = mother;
	if (Wit_Group==getgroup) Wit_Group = mother;
	if (Dim_Line_Group==getgroup) Dim_Line_Group = mother; 

	setpath();

	InventoryAll(FALSE);	/* Get a new inventory count */

}

BOOL CountExpunge(ingrp,killcnt)
GROUP_A *ingrp;
int *killcnt;
{

	GROUP_A *looper;
	BOOL retgrp=FALSE;

	for (looper=ingrp->descendants; looper!=NULL; looper=looper->next)
		retgrp |= CountExpunge(looper,killcnt);

	if (retgrp) return(FALSE);	/* A desc is occupied */

	if (ingrp->Use_Count>0) return(FALSE);	/* We are occupied */

	if (ingrp==Root_Group) return(FALSE);	/* Cannot be deleted */

	if (Current_Group==ingrp) return(FALSE);
	if (Dim_Box_Group==ingrp) return(FALSE);
	if (Wit_Group==ingrp) return(FALSE);
	if (Dim_Line_Group==ingrp) return(FALSE);

	*killcnt += 1;		/* We can be deleted */

	return(TRUE);
}

VOID ExpungeBranch(ingrp)
GROUP_A *ingrp;
{
	GROUP_A *looper;

	for (looper=ingrp->descendants; looper!=NULL; looper=looper->next)
		ExpungeBranch(looper);

	if (ingrp->descendants) return;		/* No... */
	if (ingrp==Root_Group) return;		/* No... */
	if (ingrp->Use_Count>0) return;		/* No... */

	if (Current_Group==ingrp) return;
	if (Dim_Box_Group==ingrp) return;
	if (Wit_Group==ingrp) return;
	if (Dim_Line_Group==ingrp) return;

	Unlink_Group(ingrp);			/* Yes!!! */

	return;
}

BOOL Group_Expunge()
{

	int getcount;

	getcount = 0;
	(VOID) CountExpunge(Root_Group,&getcount);
	
	if (getcount==0) return(FALSE);		/* No Action */

	sprintf(&GroupWar[0][0],"Expunging Unused Group Branches");
	sprintf(&GroupWar[1][0],"Deleting %d Groups",getcount);

	if (complain("ABORT","PROCEED",5,2,2,2,
		"Carnage Warning:",
		&GroupWar[0][0],
		&GroupWar[1][0])) return;
	
	ExpungeBranch(Root_Group);

	return(TRUE);
}

BOOL Group_Graft()
{

	GROUP_A	*getgroup,*togroup,*savecurrent,*grouploop,*savdsc,*savdscl;
	BOOL	getans;

	if (Current_Group==NULL) Current_Group = Root_Group;
	if (Current_Group==NULL) return;

	savecurrent = Current_Group;

 	curgrp = (GROUP_A *) 2;

	if (!Group_Sc("Select Group Branch to Move","  Select  ","Group Name",
			&curgrp,FALSE,TRUE)) {
		Current_Group = savecurrent;
		setpath();

		return(FALSE);
	}

	if (ArgText[0]=='\0') curgrp = 2;


	if ((int) curgrp!=2) {
		oneliner("You must select a Drawing_Objects item!");
		Current_Group = savecurrent;
		setpath();
		return(FALSE);
	}

	getgroup = Current_Group;

 	curgrp = (GROUP_A *) 2;

	if (!Group_Sc("Select Group To Place Branch In","  Select  ","Group Name",
			&curgrp,FALSE,TRUE)) {
		Current_Group = savecurrent;
		setpath();

		return(FALSE);
	}

	if (ArgText[0]=='\0') curgrp = 2;

	if ((int) curgrp!=2) {
		oneliner("You must select a Drawing_Objects item!");
		Current_Group = savecurrent;
		setpath();
		return(FALSE);
	}

	togroup = Current_Group;

	Current_Group = savecurrent;
	setpath();

	for (grouploop=togroup; grouploop!=NULL; grouploop=grouploop->parent)
		if (grouploop==getgroup)
			oneliner("Branch cannot be moved below its root\n");
	
	savdsc = getgroup->descendants;
	savdscl = getgroup->desc_last;
	getgroup->descendants = NULL;
	getgroup->desc_last = NULL;

	Unlink_Group(getgroup);			/* Snip out old group */
	Link_Group(getgroup,togroup,FALSE);	/* Put into new place */

	getgroup->descendants = savdsc;
	getgroup->desc_last = savdscl;	/* Replace its descendants */

	InventoryAll(FALSE);	/* Get a new inventory count */

}

VOID printgroup(f,group,level)
FILE *f;
GROUP_A *group;
int level;
{

	UBYTE nambuf[40];

	int i;
	
	for (; group!=NULL; group=group->next) {
	
		for (i=0; i<level*2&&i<38; i++) nambuf[i] = ' ';
		nambuf[i] = '\0';
	
		strcpy(&nambuf[i],group->Group_Name);
	
		fprintf(f,"%-40.40s %5d\n\r",nambuf,group->Use_Count);
	
		if (group->descendants!=NULL) 
			printgroup(f,group->descendants,level+1);
	}

}

BOOL ReportGroup()
{

	GROUP_A *looper;

	FILE *ofil;

	ofil = openprt("GroupReport");
	if (ofil==NULL) return(FALSE);
	
	fprintf(ofil,"%s Group Listing ",ucadname);
	if (Savnam) fprintf(ofil,"(File %s)",Savefile);
	fprintf(ofil,"\n\n\r");
	
	fprintf(ofil,"Name of Drawing Group---------Counts\n\n\r");

	printgroup(ofil,Root_Group,0);
	
	fclose(ofil);
}
