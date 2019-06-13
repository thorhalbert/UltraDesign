/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	hatchmode.c - hatching user interface
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
 *	7	27-Dec-88 SH	Create hatch mode from text mode
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*----------------------------------Macros-----------------------------------*/

#define HATCH_CLASS	50

#define GAD_FINDPOLY	20
#define GAD_SETTYPE	30
#define GAD_GETDEFS	22
#define GAD_RENDER	55

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

extern struct Image hatch_1s_image;
extern struct Image hatch_1u_image;
extern struct Image hatch_2s_image;
extern struct Image hatch_2u_image;
extern struct Image hatch_3s_image;
extern struct Image hatch_3u_image;
extern struct Image hatch_4s_image;
extern struct Image hatch_4u_image;
extern struct Image hatch_5s_image;
extern struct Image hatch_5u_image;

/*-------------------------Global Variable Definitions-----------------------*/

VOID DefHandler();

VOID Hatch_shadow_rem();
VOID PerformHatch();
extern D_NODE *PickSelect();

WORD	H_Activator=GAD_FINDPOLY;
BOOL 	HatchSetup = FALSE;
BOOL	Poly_Set = FALSE;
BOOL	Hatchgadgetssetup = FALSE;
BOOL	circlemode=FALSE;
O_ELLIP	shadowcircle;
BOOL	cshad;
D_NODE	*Start_Node=NULL;

/* Requester variables */

STDFLT Hatch_Scale = 1.0;
STDFLT Hatch_Rotation = 0.0;

UBYTE Hatch_Mode = HATCH_SOLID;

/*-------------------------------Code Section--------------------------------*/

VOID GetRidOfHatchMode()
{

	GI_DrawSetup(1,1);

	Hatch_shadow_rem();

	GI_DeactivateBGadget(HATCH_CLASS,0,NULL);	

	CleanupShadows=NULL;

	zapwindow();

	HatchSetup = FALSE;

}

VOID HandleHatchMouse(ingad)
struct GI_bgadget *ingad;
{

	if (ingad->GadgetID==H_Activator) {
		GI_SetBGadget(HATCH_CLASS,0,ingad,FALSE);
		H_Activator=999;
		if (Poly_Set)	MainMess(HATCH_MESS_REND);
		else		MainMess(HATCH_MESS_POLY);
		return;
	}

	if ((!Poly_Set)&&ingad->GadgetID!=GAD_FINDPOLY) {
		MainMess(HATCH_MESS_POLY);
		InfoMess(HATCH_MODE,HATCH_MESS_NOPOLY);
		GI_SetBGadget(HATCH_CLASS,0,ingad,FALSE);
		H_Activator = 999;
		return;
	}

	GI_SetBGadget(HATCH_CLASS,H_Activator,NULL,FALSE);

	GI_SetBGadget(HATCH_CLASS,0,ingad,TRUE);
	H_Activator = ingad->GadgetID;

	switch (H_Activator) {
	case GAD_FINDPOLY:
		MainMess(HATCH_MESS_REND);
		break;
	}

	return;
}
	
VOID FlagHandler(ingad) 
struct GI_bgadget *ingad;
{

	BOOL answer;

	answer = GI_GetBGadget(0,0,ingad);
	
	if (answer==FALSE)	Hatch_Mode = HATCH_SOLID;
	else			Hatch_Mode = HATCH_PATTERN;


}

VOID RestartHatch()
{

	Poly_Set=FALSE;

	Hatch_shadow_rem();

	GI_SetBGadget(HATCH_CLASS,GAD_FINDPOLY,NULL,TRUE);
	H_Activator = GAD_FINDPOLY;
	MainMess(HATCH_MESS_POLY);

	return;
	
}

VOID RenderHatch(ingad)
struct GI_bgadget *ingad;
{

	D_NODE	*wander;
	O_LINE	*lineobj,*thisobj;
	O_HATCH newhatch;

	if (!Poly_Set) {
		InfoMess(HATCH_MODE,HATCH_MESS_NOPOLY);
		return;
	}

	if (Hatch_Mode==HATCH_PATTERN&&Current_Hatch==NULL) {
		complain(NULL,"PROCEED",0,0,0,0,
			"To use a patterned hatch you",
			"must first select a pattern in",
			"the setup requester.",NULL);
		return;
	}

	Hatch_shadow_rem();

	newhatch.hatch_type = Hatch_Mode;
	newhatch.hatch_flags = 0;
	newhatch.number_polys = 1;
	newhatch.pattern_rot = Hatch_Rotation;
	newhatch.pattern_scale = Hatch_Scale;
	newhatch.hatch_pattern = NULL;
	if (Hatch_Mode==HATCH_PATTERN) newhatch.hatch_pattern = Current_Hatch;
/*	printf("Link=%x\n",Current_Hatch);
	if (Current_Hatch!=NULL) 
		printf("Hatching with %s\n",Current_Hatch->Hatch_Name); */
	
	
	CreateHatch(&newhatch,Start_Node); 

	v_reset();
	thisobj = GetLineData(Start_Node);
	for (wander = Start_Node; wander!=NULL; wander=wander->node_subsequent){
		if (wander->node_id!=NODE_LINE) break;
		lineobj = GetLineData(wander);
		if ((thisobj!=lineobj) &&
		    ((lineobj->x1!=thisobj->x2) ||
		     (lineobj->y1!=thisobj->y2))) break;
		wander->above_group = Current_Group;
		GenericRender(wander);
	} 
	
	RestartHatch();
	
}

STDFLT	*Shadow_Array=NULL;
/* STDFLT	Shadow_Array[4096]; */
int	Shadow_Count;

BOOL ItsAnEllipse()
{

	O_ELLIP *drawellipse;

	circlemode = TRUE;

	SetDrMd(rp, JAM1 | COMPLEMENT);
	SetAPen(rp,1);
	BNDRYOFF(rp);
	
	drawellipse = GetEllipData(Start_Node);
	
	HatchedEllipse(drawellipse->x1,drawellipse->y1,
			drawellipse->xradius,drawellipse->yradius,
			drawellipse->fromang,drawellipse->toangle,
			drawellipse->rotation,TRUE);

	shadowcircle = *drawellipse;
	cshad = TRUE;

	return(TRUE);
	
}
	
BOOL findpoly(x,y)
STDFLT x,y;
{

	D_NODE	*find_node,*wander;
	O_LINE	*lineobj,*thisobj;
	WORD	sx,sy;
	int	counts;

	Hatch_shadow_rem();
						
	find_node = PickSelect(x,y,0);

	if (find_node==NULL) {
		complain(NULL,"PROCEED",0,2,2,0,
			"Please select a line or an ellipse",
			NULL,NULL,NULL);
		return(FALSE);
	}
	
	switch(find_node->node_id) {
	case NODE_LINE:
	case NODE_ELLIP:
		break;
	default:
		complain(NULL,"PROCEED",0,2,2,0,
			"Please select a line or an ellipse",
			NULL,NULL,NULL);
		return(FALSE);
	}

	if (find_node->node_id==NODE_ELLIP) {
		Start_Node = find_node;
		return(ItsAnEllipse());
	}
	
	circlemode = FALSE;

	/* Wander backwards to beginning of the list */
	
	thisobj = GetLineData(find_node);
	for (wander = find_node->node_previous; wander!=NULL; 
	    wander=wander->node_previous) {
		if (wander->node_id!=NODE_LINE) break;
		lineobj = GetLineData(wander);
		if ((lineobj->x2==thisobj->x1) &&
		    (lineobj->y2==thisobj->y1)) {
		    	find_node = wander;	/* Walk back */
		    	thisobj = lineobj;
		} else break;
	}
	
	wander = find_node->node_subsequent;
	if (wander!=NULL) {
		if (wander->node_id!=NODE_LINE) {
		   	DisplayBeep(screen);	/* One line is not a poly */
			return(FALSE);
		}
		lineobj = GetLineData(wander);
		if ((lineobj->x1!=thisobj->x2) ||
		    (lineobj->y1!=thisobj->y2)) {
		    
		   	DisplayBeep(screen);	/* One line is not a poly */
			return(FALSE);
		}
	} else {
		DisplayBeep(screen);	/* End of item list */
		return(FALSE);
	}
	
	counts = 1;
	thisobj = GetLineData(find_node);	
	
	for (wander = find_node; wander!=NULL; wander=wander->node_subsequent) {
		if (wander->node_id!=NODE_LINE) break;
		lineobj = GetLineData(wander);
		if ((thisobj!=lineobj) &&
		   ((lineobj->x1!=thisobj->x2) ||
		    (lineobj->y1!=thisobj->y2))) break;

		counts++;
	
		thisobj = lineobj;
	}
	
	if (counts<=1) return(FALSE);	/* What? No items */
	
	Shadow_Count = counts;

	if (Shadow_Array!=NULL)
		FreeMem(Shadow_Array,Shadow_Count * 2 * (sizeof (STDFLT)));
	Shadow_Array = NULL;
	Shadow_Array = AllocMem(counts * 2 * (sizeof (STDFLT)), 0);
	if (Shadow_Array==NULL) return(FALSE);
		
	SetDrMd(rp, JAM1 | COMPLEMENT);
	SetAPen(rp,1);
	BNDRYOFF(rp);
	
	if (!PolySetup(TRUE)) return(FALSE);

	counts = 0;
	
	thisobj = GetLineData(find_node);
		
	Shadow_Array[counts*2+0] = thisobj->x1;
	Shadow_Array[counts*2+1] = thisobj->y1;
	
	counts++;
	
	PolyVertex(thisobj->x1,thisobj->y1);	
	
	for (wander = find_node; wander!=NULL; wander=wander->node_subsequent) {
		if (wander->node_id!=NODE_LINE) break;
		lineobj = GetLineData(wander);
		if ((thisobj!=lineobj) &&
		    ((lineobj->x1!=thisobj->x2) ||
		    (lineobj->y1!=thisobj->y2))) break;
	
			Shadow_Array[counts*2+0] = lineobj->x2;
			Shadow_Array[counts*2+1] = lineobj->y2;
			counts++;
	
		PolyVertex(lineobj->x2, lineobj->y2);
	
		thisobj = lineobj;
	}
	
	PolyClosure();
	PolyRender();

	Start_Node = find_node;
			
	return(TRUE);
}

VOID Draw_Hatch()
{

	int bl;

	MainMode(HATCH_MODE);

	if (HatchSetup) return;

	if (WrapupDrawMode!=NULL &&
	    WrapupDrawMode != GetRidOfHatchMode) 
			(*WrapupDrawMode)();

	mouseaction=PerformHatch;
	WrapupDrawMode=GetRidOfHatchMode;

	zapwindow();

	HatchSetup = TRUE;

	CleanupShadows = Hatch_shadow_rem;

	if (Hatchgadgetssetup) {

		GI_ActivateBGadget(HATCH_CLASS,0,NULL,w);

		GI_RefreshGadgets();

		return;
	}

	bl = (YSsize - 28*4)/2;

	GI_BGadgetImage(3,bl,HandleHatchMouse,
		TRUE,HATCH_CLASS,GAD_FINDPOLY,TRUE,&hatch_1u_image,
						      &hatch_1s_image);
	GI_SetBGadget(HATCH_CLASS,GAD_FINDPOLY,NULL,TRUE);
	MainMess(HATCH_MESS_POLY);

	GI_BGadgetImage(3,bl+28,FlagHandler,TRUE,HATCH_CLASS,
		GAD_SETTYPE,FALSE,&hatch_2u_image,&hatch_2s_image);
	
	GI_BGadgetImage(3,bl+28*2,DefHandler,TRUE,HATCH_CLASS,
		GAD_GETDEFS,TRUE,&hatch_4u_image,&hatch_4s_image);
	
	GI_BGadgetImage(3,bl+28*3,RenderHatch,TRUE,HATCH_CLASS,
		GAD_RENDER,TRUE,&hatch_5u_image,&hatch_5s_image);
	
	GI_ActivateBGadget(HATCH_CLASS,0,NULL,w);

	GI_RefreshGadgets();

	Hatchgadgetssetup=TRUE;
	
}
	
VOID PerformHatch(sx,sy,action,xr,yr)
WORD sx,sy,action;
STDFLT xr,yr;
{

	STDFLT theta;

	GI_DrawSetup(1,1);

	switch (action) {
	case MOVEMENT:
	case UMOVEMENT:
		switch (H_Activator) {
		case GAD_FINDPOLY:
			return;
		}
		return;
	case LBUTTONDOWN:
		switch (H_Activator) {
		case GAD_FINDPOLY:
			if (findpoly(xr,yr)) {
				Poly_Set=TRUE;
				GI_SetBGadget(HATCH_CLASS,GAD_FINDPOLY,NULL,FALSE);
				MainMess(HATCH_MESS_REND);
				H_Activator=999;
			}
			return;

		}
		return;
	case RBUTTONDOWN:
		RestartHatch();
		break;
	}
}

VOID UndoCircle()
{

	O_ELLIP *drawellipse;

	drawellipse = &shadowcircle;

	SetDrMd(rp, JAM1 | COMPLEMENT);
	SetAPen(rp,1);
	BNDRYOFF(rp);
	
	drawellipse = GetEllipData(Start_Node);
	
	HatchedEllipse(drawellipse->x1,drawellipse->y1,
			drawellipse->xradius,drawellipse->yradius,
			drawellipse->fromang,drawellipse->toangle,
			drawellipse->rotation,TRUE);

	cshad = FALSE;

	return;
}
	
VOID Hatch_shadow_rem() 
{

	int counts;

	if (circlemode) {
		if (cshad) UndoCircle();
		return;
	}

	if (Shadow_Count<=0) return;
	if (Shadow_Array==NULL) return;

	SetDrMd(rp, JAM1 | COMPLEMENT);
	SetAPen(rp,1);
	BNDRYOFF(rp);
	
	if (!PolySetup(TRUE)) return;
	
	PolyVertex(Shadow_Array[0],Shadow_Array[1]);

	for (counts=2; counts<(Shadow_Count*2); counts+=2)
		PolyVertex(Shadow_Array[counts+0],Shadow_Array[counts+1]);
	
	PolyClosure();
	PolyRender();

	FreeMem(Shadow_Array,Shadow_Count * 2 * (sizeof (STDFLT)));
	Shadow_Array = NULL; 
	Shadow_Count = 0;

}

/*-------------------------Defaults Requester-----------------------------*/


LOCAL T_HATCH *hold_hatch=NULL;

VOID *Hatch_First();
VOID *Hatch_Next();
VOID *Hatch_Prev();
VOID *Hatch_Search();
VOID *Hatch_Parent();
VOID *Hatch_Descend();
VOID *Hatch_Form();
VOID *Hatch_Name();

BOOL Hatch_Get()
{
	UBYTE HatchTmpMask[100];
	
	strcpy(HatchTmpMask,"*");

	return(Scroller(Hatch_First,
		Hatch_Next,
		Hatch_Prev,
		Hatch_Search,
		Hatch_Parent,
		Hatch_Descend,
		Hatch_Form,
		Hatch_Name,
		FALSE,
		TRUE,
		FALSE,
		NULL,
		"    OK    ",
		"  Cancel  ",
		"Select New Current Pattern",
		"Hatch Pattern File           Hatch Pattern Name",
		&hold_hatch,
		HatchTmpMask,
		"(Hatch Table)"));
}

BOOL InDef=FALSE;
LOCAL BOOL exflag;
LOCAL int postaction;
LOCAL struct Requester HatchRequester;

VOID HMHandler();
VOID HMStringHandler();
UBYTE HMText[2][40];


LOCAL struct IntuiText HMEndTx[] = {
	{ 2,3,JAM2,0,0,NULL,"  Cancel  ",NULL },
	{ 2,3,JAM2,0,0,NULL,"    OK    ",NULL },
};

#define HMACTIVE GADGIMMEDIATE | RELVERIFY 
#define HMBOOL BOOLGADGET|REQGADGET
#define HMLEFT 135
#define HMINFO 235
#define HMBASE 20
#define HMBOTTOM HMBASE+73
#define HMRIGHT 390

LOCAL UBYTE HMData[2][32];

LOCAL struct StringInfo HMStringSpec[] = {
	{ &HMData[0][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &HMData[1][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
};

extern struct Image hreq_1s_image;
extern struct Image hreq_1u_image;
extern struct Image hreq_2s_image;
extern struct Image hreq_2u_image;

LOCAL struct Gadget HMEnders[] = {
	{ &HMEnders[1],HMRIGHT-70-80,HMBOTTOM-15,80,8,GADGHCOMP,HMACTIVE,HMBOOL,
		NULL,NULL,&HMEndTx[0],0,NULL,20,HMHandler},
	{ &HMEnders[2], 70,HMBOTTOM-15,80,8,GADGHCOMP,HMACTIVE,HMBOOL,
		NULL,NULL,&HMEndTx[1],0,NULL,21,HMHandler},
		
	{ &HMEnders[3], HMINFO-40, HMBASE+20, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &HMStringSpec[0], 30, HMStringHandler  },
	{ &HMEnders[4], HMINFO-40, HMBASE+30, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &HMStringSpec[1], 31, HMStringHandler  },
	  
	{ &HMEnders[5],  55,HMBOTTOM-33,134,14,GADGIMAGE|GADGHIMAGE,
		HMACTIVE,HMBOOL,&hreq_1u_image,&hreq_1s_image,
		NULL,0,NULL,22,HMHandler},
	{ NULL,         200,HMBOTTOM-33,134,14,GADGIMAGE|GADGHIMAGE,
		HMACTIVE,HMBOOL,&hreq_2u_image,&hreq_2s_image,
		NULL,0,NULL,30,HMHandler},
};

UBYTE Hfile[40],Hpatt[40];
LOCAL struct IntuiText HMHeadings[] = {
 { 3,1,JAM2,5,4,NULL,"Patterned Hatching Setup:",&HMHeadings[1]},
 { 2,1,JAM2,10,HMBASE    ,NULL,"   Hatch File:",&HMHeadings[2] },
 { 2,1,JAM2,10,HMBASE+  8,NULL,"      Pattern:",&HMHeadings[3] },
 { 2,1,JAM2,80,HMBASE+ 20,NULL,"        Scale:",&HMHeadings[4] },
 { 2,1,JAM2,80,HMBASE+ 30,NULL,"     Rotation:",&HMHeadings[5] },
 { 0,1,JAM2,HMLEFT, HMBASE, NULL, &Hfile[0], &HMHeadings[6] },
 { 0,1,JAM2,HMLEFT, HMBASE+8, NULL, &Hpatt[0], NULL },
};

VOID FixStringH()
{

	if (Hatch_Scale<1e-10) Hatch_Scale = 1.0;

	sprintf(&HMData[0][0],"%.5f",Hatch_Scale);
	strcpy(&HMData[1][0],DispAng(Hatch_Rotation));

	RefreshGList(&HMEnders[2],w,&HatchRequester,2);
	
}

VOID HMStringHandler(ingad)
struct Gadget *ingad;
{
	STDFLT 	inans;
	BOOL	inchk;
	int 	i;

	switch (ingad->GadgetID) {
	
	inchk = FALSE;
	case 30:	/* Scale */
		i = sscanf(&HMData[0][0],"%f",&inans);
		if (i==1) Hatch_Scale = inans;
		break;
	case 31:	/* Rotation */
		inans = ParAng(&HMData[1][0],&inchk);
		if (!inchk) Hatch_Rotation = inans;
		break;
	
	}
	
	FixStringH();
}

VOID HMHandler(ingad)
struct Gadget *ingad;
{

	int gnum;
	
	gnum = ingad->GadgetID;
	
	CloseRequest(&HatchRequester,w);
	
	switch (gnum) {
	
	case 20:
		exflag = FALSE;
		postaction = 0;
		return;
	case 21:
		exflag = FALSE;
		postaction = 1;
		return;
	case 22:
		exflag = TRUE;
		postaction =  3;
		return;
	case 30:
	case 31:
		exflag = TRUE;
		postaction = 2;
		return;
	}
	
	return;
}

VOID DoHatchRequester()
{

	UBYTE centerbuf[55],*dbuf,*incbuf;
	int numspa;
	T_HATCH *save_hatch;
	STDFLT save_scale;
	STDFLT save_rot;
	
	save_hatch = Current_Hatch;
	save_scale = Hatch_Scale;
	save_rot = Hatch_Rotation;
			
	exflag = TRUE;

	while (exflag) {

		if (Current_Hatch==NULL) 
			dbuf = "[No Hatch File Selected]";
		else	dbuf = Current_Hatch->File_Name;
		
		numspa = (30 - strlen(dbuf))/2;
		for (incbuf = centerbuf; numspa>=0; numspa--,incbuf++) 
				*incbuf=' ';
		sprintf(incbuf,"%-30.30s",dbuf);
		sprintf(Hfile,"%-30.30s",centerbuf);
	
		if (Current_Hatch==NULL)
			dbuf = "[No Hatch Selected]";
		else	dbuf = Current_Hatch->Hatch_Name;
		
		numspa = (30 - strlen(dbuf))/2;
		for (incbuf = centerbuf; numspa>=0; numspa--,incbuf++) 
				*incbuf=' ';
		sprintf(incbuf,"%-30.30s",dbuf);
		sprintf(Hpatt,"%-30.30s",centerbuf);
		
		InitRequester(&HatchRequester);

		HatchRequester.Width=HMRIGHT;
		HatchRequester.Height=HMBOTTOM;
		HatchRequester.LeftEdge=(XSsize-HatchRequester.Width)/2;
		HatchRequester.TopEdge=(YSsize-HatchRequester.Height)/2;

		HatchRequester.BackFill=1;

		HatchRequester.ReqText = &HMHeadings[0];
		HatchRequester.ReqGadget = &HMEnders[0];


#ifdef _TURNSY
		HatchRequester.Flags = NOISYREQ;
#endif

		SafeRequest(&HatchRequester, w);
		FixStringH();

		postaction = 0;

		DoBorders(&HatchRequester,15,0,0);

		sit_n_spin(&HatchRequester,0,&HMEnders[0],&HMEnders[1]);
			/* Wait for requesters to exit */

		switch (postaction) {
		case 0:		/* Cancel */
			Current_Hatch = save_hatch;
			Hatch_Scale = save_scale;
			Hatch_Rotation = save_rot;
			break;
		case 1:		/* OK */
			break;
		case 2:		/* Change hatch */
			if (Hatch_Get()&&scrollanswer!=NULL) 
				Current_Hatch = scrollanswer;
			break;
		case 3:		/* Load hatch */
			Load_Hatch();
			break;
		}
	}
		
}

VOID DefHandler(ingad)
struct GI_bgadget *ingad;
{

	if (InDef) return;
	
	if (ingad==NULL) return;
	
	InDef = TRUE;
	
	DoHatchRequester();
	
	InDef = FALSE;
	
	return;

}

