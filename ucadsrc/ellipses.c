/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	ellipsemode.c - Ellipse/Circle User Interface
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
 *	2	 9-May-87 SH	Setup new 3d icons
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*--------------------------------Macros-------------------------------------*/

#define TOP_USERWINDOW (YSsize-BOTTOMAREA)

#define ELLIPSE_CLASS	20

#define GAD_CENTERON	30
#define GAD_RADIUSON	31
#define GAD_STARTON	32
#define GAD_ENDON	33
#define GAD_ROTATEON	34

#define GAD_CIRCLE	20
#define GAD_RENDER	21
#define GAD_SKETCH	22

/*-----------------------------Forward References----------------------------*/

VOID PerformEllipse();

/*-------------------------External Symbol Definitions-----------------------*/

extern struct drawing_node *CurrentRoller;
extern WORD EllipseClip;

extern BOOL radialpoint;
extern STDFLT radialx,radialy;
extern SHORT iradialx,iradialy;

extern struct Image ellipse_1s_image;
extern struct Image ellipse_1u_image;
extern struct Image ellipse_2s_image;
extern struct Image ellipse_2u_image;
extern struct Image ellipse_3s_image;
extern struct Image ellipse_3u_image;
extern struct Image ellipse_4s_image;
extern struct Image ellipse_4u_image;
extern struct Image ellipse_5s_image;
extern struct Image ellipse_5u_image;
extern struct Image ellipse_6s_image;
extern struct Image ellipse_6u_image;
extern struct Image ellipse_7s_image;
extern struct Image ellipse_7u_image;
extern struct Image ellipse_8s_image;
extern struct Image ellipse_8u_image;

/*-------------------------Global Variable Definitions-----------------------*/

WORD	Activator=GAD_CENTERON;
BOOL	ellipsegadgetssetup=FALSE;

STDFLT	CentX,CentY,RadX,RadY,StartA,EndA,Rotation;
BOOL	CenterPoint,RadiusPoint,CircleMode,StartPoint,EndPoint;

STDFLT 	ShadCentX,ShadCentY,ShadRadX,ShadRadY,ShadBeg,ShadEnd,ShadRot;
BOOL	ShadowOn;
BOOL 	EllipseSetup = FALSE;
extern	BOOL pacman;

/*---------------------------------Code Section------------------------------*/

EllipseClear()
{

	DeEllipseShadow();
}
	
VOID GetRidOfEllipseMode()
{

	DeEllipseShadow();

	GI_DeactivateBGadget(ELLIPSE_CLASS,0,NULL);	

	zapwindow();

	EllipseSetup = FALSE;
}

VOID HandleCircle(ingad)
struct GI_bgadget *ingad;
{

	CircleMode=GI_GetBGadget(0,0,ingad); 

	return;
	
}

VOID HandleActivator(ingad)
struct GI_bgadget *ingad;
{
	radialpoint = FALSE;
	iradialx = iradialy = 0;

	if (ingad->GadgetID==Activator) {
		GI_SetBGadget(ELLIPSE_CLASS,0,ingad,FALSE);
		GI_RefreshGadgets();
		Activator=999;
		if (!(CenterPoint&&RadiusPoint))
			MainMess(ELLIP_MESS_FINISH);
		else	MainMess(ELLIP_MESS_REND);
		radialpoint = FALSE;
		iradialx = iradialy = 0;
		return;
	}

	GI_SetBGadget(ELLIPSE_CLASS,Activator,NULL,FALSE);


	if ((!CenterPoint)||ingad->GadgetID==GAD_CENTERON) {

		if (ingad->GadgetID!=GAD_CENTERON)
			GI_SetBGadget(ELLIPSE_CLASS,0,ingad,FALSE);

		Activator=GAD_CENTERON;
	
		GI_SetBGadget(ELLIPSE_CLASS,Activator,NULL,TRUE);

		GI_RefreshGadgets();

		MainMess(ELLIP_MESS_CENT);

		radialpoint = FALSE;
		iradialx = iradialy = 0;
		
		return;
	}

	if ((!RadiusPoint)||(ingad->GadgetID==GAD_RADIUSON)) {
			
		if (ingad->GadgetID!=GAD_RADIUSON)
			GI_SetBGadget(ELLIPSE_CLASS,0,ingad,FALSE);

		Activator=GAD_RADIUSON;
	
		GI_SetBGadget(ELLIPSE_CLASS,Activator,NULL,TRUE);

		MainMess(ELLIP_MESS_RAD);

		radialx = CentX;
		radialy = CentY;
		radialpoint = TRUE;
		iradialx = iradialy = 0;
		
		GI_RefreshGadgets();

		return;
	}


	Activator=ingad->GadgetID;

	switch (Activator) {
	case	GAD_STARTON:
		MainMess(ELLIP_MESS_STP);
		radialx = CentX;
		radialy = CentY;
		radialpoint = TRUE;
		iradialx = iradialy = 0;
		break;
	case	GAD_ENDON:
		MainMess(ELLIP_MESS_ENP);
		radialx = CentX;
		radialy = CentY;
		radialpoint = TRUE;
		iradialx = iradialy = 0;
		break;
	case	GAD_ROTATEON:
		MainMess(ELLIP_MESS_ROT);
		radialx = CentX;
		radialy = CentY;
		radialpoint = TRUE;
		iradialx = iradialy = 0;
		break;
	}

	GI_SetBGadget(ELLIPSE_CLASS,Activator,NULL,TRUE);

	GI_RefreshGadgets();

	return;
}

VOID RestartEllipse()
{

	CenterPoint=RadiusPoint=StartPoint=EndPoint=FALSE;
	radialpoint = FALSE;
	iradialx = iradialy = 0;
	
	sprintf(pbuffer,"  ");
	paintinfo();

	GI_SetBGadget(ELLIPSE_CLASS,Activator,NULL,FALSE);

	Activator=GAD_CENTERON;
	
	GI_SetBGadget(ELLIPSE_CLASS,Activator,NULL,TRUE);
	MainMess(ELLIP_MESS_CENT);

	GI_RefreshGadgets();

	StartA=0.0;
	Rotation=0.0;
	EndA=TWOPI;

}

VOID EllipProc(xr,yr)
STDFLT xr,yr;
{

	D_NODE *pickem;
	struct ellip_mode *ellip;
	
	pickem = PickSelect(xr,yr,NODE_ELLIP);
	if (pickem==NULL) return;
	
	if (pickem==CurrentRoller) CurrentRoller = NULL;
	
	ellip = GetEllipData(pickem);
	
	CentX = ellip->x1;
	CentY = ellip->y1;
	RadX = ellip->xradius;
	RadY = ellip->yradius;
	Rotation = ellip->rotation;
	StartA = ellip->fromang;
	EndA = ellip->toangle;
			
	CenterPoint=RadiusPoint=StartPoint=EndPoint=TRUE;
	
	sprintf(pbuffer,"  ");
	paintinfo();

	if (Activator!=999) GI_SetBGadget(ELLIPSE_CLASS,Activator,NULL,FALSE);

	Activator=999;
	
	GI_RefreshGadgets();

	pickem->node_flags |= NODE_DELETED;
	GenericRender(pickem);
	
	DeleteNode(pickem);

	AttemptEllipseRender();		/* Render our new ellipse */
			

}

VOID EllipScan(sx,sy,action,xr,yr)
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
		EllipProc(xr,yr);	/* Search All types */
		SExitSub(NULL);
		return;

	}
}

VOID HandleSketch(ingad)
struct GI_bgadget *ingad;
{

	radialpoint = FALSE;
	iradialx = iradialy = 0;
	DeEllipseShadow();

	if (!EnterSub(SEARCH_MODE,SEARCH_MESS_EXIT,EllipScan)) return;
	MainMess(SEARCH_MESS_FIND);

}

VOID HandleRender(ingad)
struct GI_bgadget *ingad;
{

	DeEllipseShadow();

	if (!(CenterPoint&&RadiusPoint)) {
		InfoMess(ELLIP_MODE,ELLIP_MESS_NOTSET);
		return;
	}
	
	CreateEllipse(CentX,CentY,RadX,RadY,Rotation,StartA,EndA);
	
	RestartEllipse();
}

VOID Draw_Ellipse()
{

	int bl;

	MainMode(ELLIP_MODE);

	if (EllipseSetup) return;

	if (WrapupDrawMode!=NULL &&
	    WrapupDrawMode != GetRidOfEllipseMode) 
			(*WrapupDrawMode)();

	mouseaction=PerformEllipse;
	WrapupDrawMode=GetRidOfEllipseMode;
	CleanupShadows=EllipseClear;

	zapwindow();

	EllipseSetup = TRUE;
	if (ellipsegadgetssetup) {

		GI_ActivateBGadget(ELLIPSE_CLASS,0,NULL,w);

		GI_RefreshGadgets();
	
		return;
	}

	bl = (YSsize - 223)/2;

	GI_BGadgetImage(3,bl,HandleActivator,
		TRUE,ELLIPSE_CLASS,GAD_CENTERON,FALSE,&ellipse_1u_image,
						      &ellipse_1s_image);

	GI_BGadgetImage(3,bl+28,HandleActivator,
		TRUE,ELLIPSE_CLASS,GAD_RADIUSON,FALSE,&ellipse_2u_image,
						      &ellipse_2s_image);

	GI_BGadgetImage(3,bl+56,HandleActivator,
		TRUE,ELLIPSE_CLASS,GAD_STARTON,FALSE,&ellipse_3u_image,
						      &ellipse_3s_image);

	GI_BGadgetImage(3,bl+84,HandleActivator,
		TRUE,ELLIPSE_CLASS,GAD_ENDON,FALSE,&ellipse_4u_image,
						      &ellipse_4s_image);

	GI_BGadgetImage(3,bl+112,HandleActivator,
		TRUE,ELLIPSE_CLASS,GAD_ROTATEON,FALSE,&ellipse_5u_image,
						      &ellipse_5s_image);

	GI_BGadgetImage(3,bl+140,HandleCircle,TRUE,ELLIPSE_CLASS,
		GAD_CIRCLE,FALSE,&ellipse_6s_image,&ellipse_6u_image);

	GI_BGadgetImage(3,bl+168,HandleRender,TRUE,ELLIPSE_CLASS,
		GAD_RENDER,TRUE,&ellipse_7u_image,&ellipse_7s_image);
		
	GI_BGadgetImage(3,bl+196,HandleSketch,TRUE,ELLIPSE_CLASS,
		GAD_SKETCH,TRUE,&ellipse_8u_image,&ellipse_8s_image);

	GI_SetBGadget(ELLIPSE_CLASS,GAD_CENTERON,NULL,TRUE);
	MainMess(ELLIP_MESS_CENT);

	GI_SetBGadget(ELLIPSE_CLASS,GAD_RADIUSON,NULL,FALSE);
	GI_SetBGadget(ELLIPSE_CLASS,GAD_STARTON,NULL,FALSE);
	GI_SetBGadget(ELLIPSE_CLASS,GAD_ENDON,NULL,FALSE);
	GI_SetBGadget(ELLIPSE_CLASS,GAD_ROTATEON,NULL,FALSE);

	GI_ActivateBGadget(ELLIPSE_CLASS,0,NULL,w);

	CircleMode=FALSE;
	GI_SetBGadget(ELLIPSE_CLASS,GAD_CIRCLE,NULL,CircleMode);

	GI_RefreshGadgets();

	ellipsegadgetssetup=TRUE;

	StartA=0.0;
	Rotation=0.0;
	EndA=TWOPI;
	
}

VOID PerformEllipse(sx,sy,action,xr,yr)
WORD sx,sy,action;
STDFLT xr,yr;
{

	STDFLT theta;

	switch (action) {
	case MOVEMENT:
	case UMOVEMENT:
		switch (Activator) {

		case GAD_CENTERON:

			CentX=xr;
			CentY=yr;

			AttemptEllipseRender();
			radialpoint=FALSE;
			iradialx = iradialy = 0;

			return;

		case GAD_RADIUSON:

			RadX=abs(xr-CentX);
			RadY=abs(yr-CentY);
			if (CircleMode) RadY=RadX;

			sprintf(pbuffer,"Radii [%s,%s]",
				DispLin(RadX),
				DispLin(RadY));
			paintinfo();

			AttemptEllipseRender();
			
			return;

		case GAD_STARTON:

			theta = atan2(yr-CentY,xr-CentX);

			normalize(theta);

			StartA=theta;

			sprintf(pbuffer,"Start Ang %s",DispAng(theta));
			paintinfo();
			
			return;

		case GAD_ENDON:

			theta = atan2(yr-CentY,xr-CentX);

			normalize(theta);

			EndA=theta;

			sprintf(pbuffer,"End Ang %s",DispAng(theta));
			paintinfo();
			
			return;

		case GAD_ROTATEON:

			theta = atan2(yr-CentY,xr-CentX);

			normalize(theta);

			Rotation=theta;

			sprintf(pbuffer,"Ellipse rot %s",DispAng(theta));
			paintinfo();
			
			AttemptEllipseRender();

			return;

		}

		break;

	case LBUTTONDOWN:
		switch (Activator) {

		case GAD_CENTERON:

			CentX=xr;
			CentY=yr;

			CenterPoint=TRUE;

			Activator=999;
			if (!RadiusPoint)
				Activator=GAD_RADIUSON;
	
			GI_SetBGadget(ELLIPSE_CLASS,GAD_CENTERON,NULL,FALSE);

			if (Activator!=999) {
			    GI_SetBGadget(ELLIPSE_CLASS,Activator,NULL,TRUE);
			    MainMess(ELLIP_MESS_RAD);
				radialx = CentX;
				radialy = CentY;
				radialpoint = TRUE;
				iradialx = iradialy = 0;
			}

			GI_RefreshGadgets();

			AttemptEllipseRender();
			
			return;

		case GAD_RADIUSON:

			RadX=abs(xr-CentX);
			RadY=abs(yr-CentY);
			if (CircleMode) RadY=RadX;

			if (RadX==0.0||RadY==0.0) {
				DisplayBeep(screen);
				break;
			}

			RadiusPoint=TRUE;

			Activator=999;
	
			GI_SetBGadget(ELLIPSE_CLASS,GAD_RADIUSON,NULL,FALSE);
			MainMess(ELLIP_MESS_REND);

			radialpoint = FALSE;
			iradialx = iradialy = 0;

			GI_RefreshGadgets();

			AttemptEllipseRender();
			
			return;
		
		case GAD_STARTON:

			theta = atan2(yr-CentY,xr-CentX);

			normalize(theta);

			StartA=theta;

			theta = theta * 180.0;
			theta /= PI;
			if (theta<0) theta+=360.0;

			StartPoint=TRUE;
			
			Activator=999;
			if (!EndPoint) Activator=GAD_ENDON;
	
			GI_SetBGadget(ELLIPSE_CLASS,GAD_STARTON,NULL,FALSE);

			if (Activator!=999) { 
			  GI_SetBGadget(ELLIPSE_CLASS,Activator,NULL,TRUE);
			  MainMess(ELLIP_MESS_ENP);
			}

			GI_RefreshGadgets();

			AttemptEllipseRender();
			
			return;

		case GAD_ENDON:

			theta = atan2(yr-CentY,xr-CentX);

			normalize(theta);

			EndA=theta;

			theta = theta * 180.0;
			theta /= PI;
			if (theta<0) theta+=360.0;

			EndPoint=TRUE;
			
			Activator=999;
	
			GI_SetBGadget(ELLIPSE_CLASS,GAD_ENDON,NULL,FALSE);
			MainMess(ELLIP_MESS_REND);
			GI_RefreshGadgets();

			radialpoint = FALSE;
			iradialx = iradialy = 0;

			AttemptEllipseRender();
			
			return;

		case GAD_ROTATEON:

			theta = atan2(yr-CentY,xr-CentX);

			normalize(theta);

			Rotation=theta;

			theta = theta * 180.0;
			theta /= PI;
			if (theta<0) theta+=360.0;

			EndPoint=TRUE;
			
			Activator=999;
	
			GI_SetBGadget(ELLIPSE_CLASS,GAD_ROTATEON,NULL,FALSE);
			MainMess(ELLIP_MESS_REND);

			radialpoint = FALSE;
			iradialx = iradialy = 0;

			GI_RefreshGadgets();

			AttemptEllipseRender();
			
			return;

		}
		break;
	case RBUTTONDOWN:
		RestartEllipse();

		break;


	}
}
		
AttemptEllipseRender()
{

	DeEllipseShadow();

	ShadCentX=CentX;
	ShadCentY=CentY;
	ShadRadX=RadX;
	ShadRadY=RadY;
	ShadBeg=StartA;
	ShadEnd=EndA;
	ShadRot=Rotation;

	if (!(CenterPoint||RadiusPoint)) return;

	GI_DrawSetup(1,1);

	pacman = TRUE;
	FancyEllipseClipper(ShadCentX,ShadCentY,ShadRadX,ShadRadY,
		ShadBeg,ShadEnd,ShadRot,TRUE);

	ShadowOn=TRUE;

}


DeEllipseShadow()
{

	if (!ShadowOn) return;

	GI_DrawSetup(1,1);

	pacman = TRUE;
	FancyEllipseClipper(ShadCentX,ShadCentY,ShadRadX,ShadRadY,
			ShadBeg,ShadEnd,ShadRot,TRUE);

	ShadowOn=FALSE;

}

