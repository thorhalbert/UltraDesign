/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	amigahooks.c - standard hooks into amiga OS
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
 *	1	11-Jan-87 SH	Begin History
 *	2	 1-Jun-87 SH	S gadgets not used, ifdef out
 *	3	11-Sep-87 SH	Add the 16-bit color function
 *
 */

/*-------------------------------Include Files-------------------------------*/

int Dprintf(mask,p1,p2,p3,p4,p5,p6)
{

}

#include "ucad.h"
#include <exec/memory.h>
#include <graphics/gfxmacros.h>
#include <libraries/dos.h>
#include <libraries/dosextens.h>

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

extern struct Window *w;
extern struct RastPort *rp;
extern struct ViewPort *vp;

/*-------------------------Global Variable Definitions-----------------------*/

struct Remember *MainStore,*DrawStore;

/*-------------------------------Code Section--------------------------------*/

static struct Border initbox = {-1,-1,6,7,JAM1,5,NULL,NULL};
#ifdef DEF_S_GADGETS

/*
 *	struct GI_sgadget *GI_sgadgetsetup(x,y,#char,#charmax,serviceroutine,
 *			activateflag,enableflag)
 *
 */

struct GI_sgadget *rootsgadget=NULL,*lastsgadget=NULL;
static struct IntuiText inittext = {0,1,JAM2,0,0,NULL,NULL,NULL};
static struct StringInfo initstring = 
	{NULL,NULL,0,0,0,0,0,0,0,0,NULL,0,NULL};
static struct Gadget initgadget = 
	{NULL,0,0,0,8,GADGHCOMP,RELVERIFY|GADGIMMEDIATE,STRGADGET,NULL,
	 NULL,NULL,0,NULL,0,NULL };

struct GI_sgadget *GI_SGadgetSetup(X,Y,CharsInWindow,TotalChars,ServiceRoutine,
		EnableFlag,Class,ID,prefixtext,initialvalue,justify)
int X,Y,CharsInWindow,TotalChars,Class,ID,justify;
BOOL (*ServiceRoutine)(),EnableFlag;
{
	
	struct GI_sgadget *newsgadget;
	WORD i,j;

/*	printf("Created gadget class %d id %d\n",(int) Class, (int) ID); */

	newsgadget=(struct GI_sgadget *) GI_RemAlloc(sizeof (struct GI_sgadget));
	if (newsgadget==NULL) {
		printf("GI_sgadgetsetup() couldn't alloc initial array\n");
		return(NULL);
	}

	newsgadget->amigagadget=initgadget;
	newsgadget->amigastrings=initstring;
	newsgadget->dispbox=initbox;

	newsgadget->maxsize=TotalChars;
	newsgadget->nextsgadget=NULL;
	newsgadget->GadgetClass=Class;
	newsgadget->GadgetID=ID;
	newsgadget->UndoBuffer=NULL;
	newsgadget->StorageBuffer=NULL;
	newsgadget->prefixtext=prefixtext;
	newsgadget->servicefun=ServiceRoutine;
	newsgadget->activated=FALSE;
	newsgadget->enabled=EnableFlag;
	newsgadget->thiswindow=NULL;

	if ((newsgadget->UndoBuffer=GI_RemAlloc(TotalChars+1))==NULL) {
		printf("GI_sgadgetsetup() no room\n");
		/* mfree(newsgadget); */
		return(NULL);
	}
		
	if ((newsgadget->StorageBuffer=	GI_RemAlloc(TotalChars+1))==NULL) {
		printf("GI_sgadgetsetup() no room\n");
		/* mfree(newsgadget->UndoBuffer); */
		/* mfree(newsgadget); */		
		return(NULL);
	}

/*-----------------------Set up box and border--------------------------------*/

	for (j=0; j<10; j++) 
		newsgadget->BorderArray[j]=0;
	
	if (CharsInWindow<=0) CharsInWindow=TotalChars;

	i=(CharsInWindow*8)+2;
	newsgadget->BorderArray[5]=9;
	newsgadget->BorderArray[7]=9;
	newsgadget->BorderArray[2]=i;
	newsgadget->BorderArray[4]=i;

	newsgadget->dispbox=initbox;
	newsgadget->dispbox.XY=newsgadget->BorderArray;

/*-----------------------Set up prefix text (if any)--------------------------*/

	if (prefixtext!=NULL) {
		newsgadget->preftextintui=inittext;

		newsgadget->preftextintui.LeftEdge= -((strlen(prefixtext)*8)+5);
		newsgadget->preftextintui.IText=prefixtext;
	}

/*-----------------------Set up string info part of gadget--------------------*/

	newsgadget->amigastrings.Buffer=newsgadget->StorageBuffer;
	newsgadget->amigastrings.UndoBuffer=newsgadget->UndoBuffer;

	newsgadget->amigastrings.MaxChars=TotalChars;
	newsgadget->amigastrings.NumChars=CharsInWindow;

/*--------------------Set up default string-----------------------------------*/

	if (initialvalue!=NULL) 
		strncpy(newsgadget->StorageBuffer,initialvalue,
			newsgadget->maxsize);

/*------------------Set up the final gadget structure-------------------------*/

	newsgadget->amigagadget.LeftEdge=X-1;
	newsgadget->amigagadget.TopEdge=Y;

	newsgadget->amigagadget.Width=CharsInWindow*8;

	if (justify==GI_GADGETCENTER) 
		newsgadget->amigagadget.Activation |= STRINGCENTER;
	if (justify==GI_GADGETRIGHT)
		newsgadget->amigagadget.Activation |= STRINGRIGHT;
	
	newsgadget->amigagadget.GadgetRender=NULL;
		/* (APTR) &(newsgadget->dispbox); */

	newsgadget->amigagadget.SpecialInfo=
		(APTR) &(newsgadget->amigastrings);

	if (prefixtext!=NULL)
		newsgadget->amigagadget.GadgetText=
			&(newsgadget->preftextintui);

	newsgadget->amigagadget.GadgetID=GI_STRINGGADGET;
	newsgadget->amigagadget.UserData=(APTR) newsgadget;

	if (!EnableFlag) newsgadget->amigagadget.Flags |= GADGDISABLED;

/*-------------------Link into the storage list-------------------------------*/

	if (lastsgadget==NULL) {
		rootsgadget=newsgadget;
		lastsgadget=newsgadget;
	} else {
		lastsgadget->nextsgadget=newsgadget;
		lastsgadget=newsgadget;
	}

/*	printf("Created gadget class %d id %d\n",(int) newsgadget->GadgetClass,
		(int) newsgadget->GadgetID); */

	return(newsgadget);

}

VOID GI_EnableSGadget(class,id,gadptr)
int class,id;
struct GI_sgadget *gadptr;
{

	if (gadptr!=NULL) {
		gadptr->enabled=TRUE;
		if (gadptr->activated) {
		    OnGadget(&(gadptr->amigagadget),gadptr->thiswindow,NULL);
		    return;
		}
		gadptr->amigagadget.Flags &= (~GADGDISABLED);
		return;
	}

	for (gadptr=rootsgadget; gadptr!=NULL; gadptr=gadptr->nextsgadget) {
		if (class!=0&&class!=gadptr->GadgetClass) continue;
		if (id!=0&&id!=gadptr->GadgetID) continue;
		GI_EnableSGadget(0,0,gadptr);
	}

	return;

}

VOID GI_DisableSGadget(class,id,gadptr)
int class,id;
struct GI_sgadget *gadptr;
{

	if (gadptr!=NULL) {
		gadptr->enabled=FALSE;
		if (gadptr->activated) {
		    OffGadget(&(gadptr->amigagadget),gadptr->thiswindow,NULL);
		    return;
		}
		gadptr->amigagadget.Flags |= GADGDISABLED;
		return;
	}

	for (gadptr=rootsgadget; gadptr!=NULL; gadptr=gadptr->nextsgadget) {
		if (class!=0&&class!=gadptr->GadgetClass) continue;
		if (id!=0&&id!=gadptr->GadgetID) continue;
		GI_DisableSGadget(0,0,gadptr);
	}

	return;

}

VOID GI_ActivateSGadget(class,id,gadptr,window)
int class,id;
struct GI_sgadget *gadptr;
struct Window *window;
{

	if (gadptr!=NULL) {
		if (gadptr->activated) return;
		gadptr->activated=TRUE;
		gadptr->thiswindow=window;
		
		AddGadget(gadptr->thiswindow,&gadptr->amigagadget,-1);		

/*	printf("Activated gadget class %d id %d\n",(int) gadptr->GadgetClass,
		(int) gadptr->GadgetID); */


		return;
	}

	for (gadptr=rootsgadget; gadptr!=NULL; gadptr=gadptr->nextsgadget) {
		if (class!=0&&class!=gadptr->GadgetClass) continue;
		if (id!=0&&id!=gadptr->GadgetID) continue;
		GI_ActivateSGadget(0,0,gadptr,window);
	}

	return;

}

VOID GI_DeactivateSGadget(class,id,gadptr)
int class,id;
struct GI_sgadget *gadptr;
{

	if (gadptr!=NULL) {
		if (!gadptr->activated) return;
		gadptr->activated=FALSE;

		RemoveGadget(gadptr->thiswindow,&gadptr->amigagadget);
		gadptr->thiswindow=NULL;

		return;
	}

	for (gadptr=rootsgadget; gadptr!=NULL; gadptr=gadptr->nextsgadget) {
		if (class!=0&&class!=gadptr->GadgetClass) continue;
		if (id!=0&&id!=gadptr->GadgetID) continue;
		GI_DeactivateSGadget(0,0,gadptr);
	}

	return;

}

VOID GI_SetSGadget(class,id,gadptr,stringvalue)
int class,id;
struct GI_sgadget *gadptr;
UBYTE *stringvalue;
{

	if (gadptr!=NULL) {
		strncpy(gadptr->StorageBuffer,stringvalue,gadptr->maxsize);
		return;
	}

	for (gadptr=rootsgadget; gadptr!=NULL; gadptr=gadptr->nextsgadget) {
		if (class!=0&&class!=gadptr->GadgetClass) continue;
		if (id!=0&&id!=gadptr->GadgetID) continue;
		GI_SetSGadget(0,0,gadptr,stringvalue);
	}

	return;

}

UBYTE *GI_GetSGadget(class,id,gadptr)
int class,id;
struct GI_sgadget *gadptr;
{

	if (gadptr!=NULL) return(gadptr->StorageBuffer);

	for (gadptr=rootsgadget; gadptr!=NULL; gadptr=gadptr->nextsgadget) {
		if (class!=0&&class!=gadptr->GadgetClass) continue;
		if (id!=0&&id!=gadptr->GadgetID) continue;
		return(gadptr->StorageBuffer);		
	}

	return(NULL);

}

#endif /* DEF_S_GADGETS */

/*
 *
 *	struct GI_bgadget *GI_bgadgetsetup(x,y,#char,serviceroutine,
 *		EnableFlag,Class,ID,mestext,buttonflag)
 *
 */

struct GI_bgadget *rootbgadget=NULL,*lastbgadget=NULL;

#ifdef DEF_BT_GADGETS	/* Not using the text type button gadgets */
	
struct GI_bgadget *GI_BGadgetSetup(X,Y,CharsInWindow,ServiceRoutine,
		EnableFlag,Class,ID,mestext,buttonflag)
int X,Y,CharsInWindow,Class,ID;
BOOL (*ServiceRoutine)(),EnableFlag,buttonflag;
{
	
	struct GI_bgadget *newbgadget;
	WORD i,j;

	static struct Gadget initgadget = 
		{NULL,0,0,0,8,GADGHCOMP,RELVERIFY|GADGIMMEDIATE,BOOLGADGET,NULL,
		 NULL,NULL,0,NULL,0,NULL };

/* 	printf("Created B gadget class %d id %d\n",(int) Class, (int) ID); */

	newbgadget=(struct GI_bgadget *) GI_RemAlloc(sizeof (struct GI_bgadget));
	if (newbgadget==NULL) {
		printf("GI_bgadgetsetup() couldn't alloc initial array\n");
		return(NULL);
	}

	newbgadget->amigagadget=initgadget;
	newbgadget->dispbox=initbox;

	newbgadget->maxsize=CharsInWindow;
	newbgadget->nextbgadget=NULL;
	newbgadget->GadgetClass=Class;
	newbgadget->GadgetID=ID;
	newbgadget->servicefun=ServiceRoutine;
	newbgadget->activated=FALSE;
	newbgadget->enabled=EnableFlag;
	newbgadget->thiswindow=NULL;

/*-----------------------Set up box and border--------------------------------*/

	for (j=0; j<10; j++) 
		newbgadget->BorderArray[j]=0;
	
	i=(CharsInWindow*8)+1;
	newbgadget->BorderArray[5]=9;
	newbgadget->BorderArray[7]=9;
	newbgadget->BorderArray[2]=i;
	newbgadget->BorderArray[4]=i;

	newbgadget->dispbox=initbox;
	newbgadget->dispbox.XY=newbgadget->BorderArray;

/*-----------------------Set up message text (if any)--------------------------*/

	newbgadget->bodytextintui=inittext;

	newbgadget->bodytextintui.LeftEdge= 
		((CharsInWindow-strlen(mestext))*8)/2;
	newbgadget->bodytextintui.IText=mestext;

/*------------------Set up the final gadget structure-------------------------*/

	newbgadget->amigagadget.LeftEdge=X-1;
	newbgadget->amigagadget.TopEdge=Y;

	newbgadget->amigagadget.Width=CharsInWindow*8;

	newbgadget->amigagadget.GadgetRender=NULL;
		/* (APTR) &(newbgadget->dispbox); */

	newbgadget->amigagadget.GadgetText=
		&(newbgadget->bodytextintui);

	newbgadget->amigagadget.GadgetID=
		(buttonflag?GI_BOOLSGADGET:GI_BUTTONGADGET);

	newbgadget->amigagadget.UserData=(APTR) newbgadget;

	if (!buttonflag) newbgadget->amigagadget.Activation |= TOGGLESELECT;
	if (!EnableFlag) newbgadget->amigagadget.Flags |= GADGDISABLED;

/*-------------------Link into the storage list-------------------------------*/

	if (lastbgadget==NULL) {
		rootbgadget=newbgadget;
		lastbgadget=newbgadget;
	} else {
		lastbgadget->nextbgadget=newbgadget;
		lastbgadget=newbgadget;
	}

/*	printf("Created gadget class %d id %d\n",(int) newbgadget->GadgetClass, 
		(int) newbgadget->GadgetID);	*/

	return(newbgadget);

}

#endif

struct GI_bgadget *GI_BGadgetImage(X,Y,ServiceRoutine,
		EnableFlag,Class,ID,buttonflag,renderimage,selectimage)
int X,Y,Class,ID;
BOOL (*ServiceRoutine)(),EnableFlag,buttonflag;
struct Image *renderimage,*selectimage;
{
	
	struct GI_bgadget *newbgadget;
	WORD i,j;

	static struct Gadget initgadget = 
		{NULL,0,0,0,8,GADGIMAGE|GADGHIMAGE,RELVERIFY|GADGIMMEDIATE,
		 BOOLGADGET,NULL,
		 NULL,NULL,0,NULL,0,NULL };

/*	printf("Created B gadget class %d id %d\n",(int) Class, (int) ID); */

	newbgadget=(struct GI_bgadget *) GI_RemAlloc(sizeof (struct GI_bgadget));
	if (newbgadget==NULL) {
		printf("GI_bgadgetsetup() couldn't alloc initial array\n");
		return(NULL);
	}

	newbgadget->amigagadget=initgadget;
	newbgadget->dispbox=initbox;

	newbgadget->nextbgadget=NULL;
	newbgadget->GadgetClass=Class;
	newbgadget->GadgetID=ID;
	newbgadget->servicefun=ServiceRoutine;
	newbgadget->activated=FALSE;
	newbgadget->enabled=EnableFlag;
	newbgadget->thiswindow=NULL;

/*------------------Set up the final gadget structure-------------------------*/

	newbgadget->amigagadget.LeftEdge=X;
	newbgadget->amigagadget.TopEdge=Y;

	newbgadget->amigagadget.Width=renderimage->Width;
	newbgadget->amigagadget.Height=renderimage->Height;

	newbgadget->amigagadget.GadgetRender=renderimage;
	newbgadget->amigagadget.SelectRender=selectimage;

	newbgadget->amigagadget.GadgetID=
		(buttonflag?GI_BOOLSGADGET:GI_BUTTONGADGET);

	newbgadget->amigagadget.UserData=(APTR) newbgadget;

	if (!buttonflag) newbgadget->amigagadget.Activation |= TOGGLESELECT;
	if (!EnableFlag) newbgadget->amigagadget.Flags |= GADGDISABLED;

/*-------------------Link into the storage list-------------------------------*/

	if (lastbgadget==NULL) {
		rootbgadget=newbgadget;
		lastbgadget=newbgadget;
	} else {
		lastbgadget->nextbgadget=newbgadget;
		lastbgadget=newbgadget;
	}

/* 	printf("Created gadget class %d id %d\n",(int) newbgadget->GadgetClass, 
		(int) newbgadget->GadgetID); */

	return(newbgadget);

}

VOID GI_EnableBGadget(class,id,gadptr)
int class,id;
struct GI_bgadget *gadptr;
{

	if (gadptr!=NULL) {
		gadptr->enabled=TRUE;
		if (gadptr->activated) {
		    OnGadget(&(gadptr->amigagadget),gadptr->thiswindow,NULL);
		    return;
		}
		gadptr->amigagadget.Flags &= (~GADGDISABLED);
		return;
	}

	for (gadptr=rootbgadget; gadptr!=NULL; gadptr=gadptr->nextbgadget) {
		if (class!=0&&class!=gadptr->GadgetClass) continue;
		if (id!=0&&id!=gadptr->GadgetID) continue;
		GI_EnableBGadget(0,0,gadptr);
	}

	return;

}

VOID GI_DisableBGadget(class,id,gadptr)
int class,id;
struct GI_bgadget *gadptr;
{

	if (gadptr!=NULL) {
		gadptr->enabled=FALSE;
		if (gadptr->activated) {
		    OffGadget(&(gadptr->amigagadget),gadptr->thiswindow,NULL);
		    return;
		}
		gadptr->amigagadget.Flags |= GADGDISABLED;
		return;
	}

	for (gadptr=rootbgadget; gadptr!=NULL; gadptr=gadptr->nextbgadget) {
		if (class!=0&&class!=gadptr->GadgetClass) continue;
		if (id!=0&&id!=gadptr->GadgetID) continue;
		GI_DisableBGadget(0,0,gadptr);
	}

	return;

}

VOID GI_ActivateBGadget(class,id,gadptr,window)
int class,id;
struct GI_bgadget *gadptr;
struct Window *window;
{

	if (gadptr!=NULL) {
		if (gadptr->activated) return;
		gadptr->activated=TRUE;
		gadptr->thiswindow=window;
		
		AddGadget(gadptr->thiswindow,&gadptr->amigagadget,-1);		

/* 	printf("Activated gadget class %d id %d\n",(int) gadptr->GadgetClass,
		(int) gadptr->GadgetID); */


		return;
	}

	for (gadptr=rootbgadget; gadptr!=NULL; gadptr=gadptr->nextbgadget) {
		if (class!=0&&class!=gadptr->GadgetClass) continue;
		if (id!=0&&id!=gadptr->GadgetID) continue;
		GI_ActivateBGadget(0,0,gadptr,window);
	}

	return;

}

VOID GI_DeactivateBGadget(class,id,gadptr)
int class,id;
struct GI_bgadget *gadptr;
{

	if (gadptr!=NULL) {
		if (!gadptr->activated) return;
		gadptr->activated=FALSE;

		RemoveGadget(gadptr->thiswindow,&gadptr->amigagadget);
		gadptr->thiswindow=NULL;

		return;
	}

	for (gadptr=rootbgadget; gadptr!=NULL; gadptr=gadptr->nextbgadget) {
		if (class!=0&&class!=gadptr->GadgetClass) continue;
		if (id!=0&&id!=gadptr->GadgetID) continue;
		GI_DeactivateBGadget(0,0,gadptr);
	}

	return;

}

VOID GI_SetBGadget(class,id,gadptr,retvalue)
int class,id;
struct GI_bgadget *gadptr;
BOOL retvalue;
{

	BOOL activated,cursel;
	struct Window *curwin;

	if (gadptr!=NULL) {
		cursel=(gadptr->amigagadget.Flags&SELECTED)?TRUE:FALSE;
		if (cursel==retvalue) return;	/* Already set correctly */

		activated=gadptr->activated;
		curwin=gadptr->thiswindow;

		if (activated) GI_DeactivateBGadget(0,0,gadptr);

		if (retvalue) 	gadptr->amigagadget.Flags |= SELECTED;
		else		gadptr->amigagadget.Flags &= ~SELECTED;

		if (activated) GI_ActivateBGadget(0,0,gadptr,curwin);

		if (activated) RefreshGadgets(&gadptr->amigagadget,curwin,NULL);

		return;
	}

	for (gadptr=rootbgadget; gadptr!=NULL; gadptr=gadptr->nextbgadget) {
		if (class!=0&&class!=gadptr->GadgetClass) continue;
		if (id!=0&&id!=gadptr->GadgetID) continue;
		GI_SetBGadget(0,0,gadptr,retvalue);
	}

	return;

}

BOOL GI_GetBGadget(class,id,gadptr)
int class,id;
struct GI_bgadget *gadptr;
{

	if (gadptr!=NULL) 
		return((gadptr->amigagadget.Flags&SELECTED)?TRUE:FALSE);
		
	for (gadptr=rootbgadget; gadptr!=NULL; gadptr=gadptr->nextbgadget) {
		if (class!=0&&class!=gadptr->GadgetClass) continue;
		if (id!=0&&id!=gadptr->GadgetID) continue;
		return((gadptr->amigagadget.Flags&SELECTED)?TRUE:FALSE);
	}

	return(NULL);

}

VOID ExecuteStringGadget(inmaster)
struct GI_sgadget *inmaster;
{

	if (inmaster->servicefun==NULL) return;

	(*(inmaster->servicefun))(inmaster);

}

VOID ExecuteBoolGadget(inmaster)
struct GI_bgadget *inmaster;
{

	if (inmaster->servicefun==NULL) return;

	(*(inmaster->servicefun))(inmaster);

}

VOID ExecuteButtonGadget(inmaster)
struct GI_bgadget *inmaster;
{

	if (inmaster->servicefun==NULL) return;

	(*(inmaster->servicefun))(inmaster);

}
VOID ExecuteCustomGadget(ingad,inexec)
struct Gadget *ingad;
VOID (*inexec)();
{

	if (inexec==NULL) return;
	(*inexec)(ingad);
}

VOID ExecuteGadgetServer(ingadget)
struct Gadget *ingadget;
{

	if (ingadget->UserData==NULL) {
		switch(ingadget->GadgetID) {
		case V_GADGET:	DoPanVert(ingadget);
				return;
		case H_GADGET:	DoPanHoriz(ingadget);
				return;
		}
	
		return;
	}

	switch (ingadget->GadgetID) {

	case GI_STRINGGADGET:
		ExecuteStringGadget((struct GI_sgadget *) ingadget->UserData);
		break;
	case GI_BOOLSGADGET:
		ExecuteBoolGadget((struct GI_bgadget *) ingadget->UserData);
		break;
	case GI_BUTTONGADGET:
		ExecuteButtonGadget((struct GI_bgadget *) ingadget->UserData);
		break;
	default:
		ExecuteCustomGadget(ingadget,ingadget->UserData);
		break;
	}

}

VOID GI_UpdateAllGadgets(wind)
struct Window *wind;
{

	if (wind->FirstGadget==NULL) return;

	RefreshGadgets(wind->FirstGadget,wind,NULL);

	return;

}

VOID GI_DrawSetup(color,drawmode)
int color,drawmode;
{
	SetDrPt(rp, 0xFFFF);
	
	SetDrMd(rp, JAM1);
	if (drawmode==1) SetDrMd(rp, JAM1 | COMPLEMENT);

	SetAPen(rp, color);

}

VOID GI_LineMove(x,y)
int x,y;
{

	Move(rp,x,y);

}

VOID GI_LineDraw(x,y)
int x,y;
{

	Draw(rp,x,y);
}

VOID GI_RectFill(color,x1,y1,x2,y2)
int color,x1,y1,x2,y2;
{

	SetAPen(rp,color);
	SetOPen(rp,color);

	SetDrMd(rp, JAM1);

	RectFill(rp, x1,y1,x2,y2);

}

VOID GI_OutText(x,y,text,apen,bpen)
int x,y,apen,bpen;
UBYTE *text;
{

	SetAPen(rp,apen);
	SetBPen(rp,bpen);
	SetDrPt(rp, 0xFFFF);

	SetDrMd(rp, JAM2);

	Move(rp,x,y);

	Text(rp, text, strlen(text));
}

GI_FillHeader(text)
UBYTE *text;
{

	SetWindowTitles(w,NULL,text);

}

UBYTE *GI_RemAlloc(size)
int size;
{

	UBYTE *outloc;

	outloc = AllocRemember(&MainStore, size, MEMF_FAST);

	if (outloc!=NULL) return(outloc);	/* We got fast memory */

	outloc = AllocRemember(&MainStore, size, MEMF_CHIP);

	if (outloc!=NULL) return(outloc);	/* Need to use chip memory */

	printf("Unable to allocate %d mainline bytes\n",size);

	return(NULL);

}

VOID GI_FreeRem()
{

	FreeRemember(&MainStore, TRUE);

}

UBYTE *GI_DrawAlloc(size)
int size;
{

	UBYTE *outloc;

	outloc = AllocRemember(&DrawStore, size, MEMF_FAST);

	if (outloc!=NULL) return(outloc);	/* We got fast memory */

	outloc = AllocRemember(&DrawStore, size, MEMF_CHIP);

	if (outloc!=NULL) return(outloc);	/* Need to use chip memory */

	printf("Unable to allocate %d mainline bytes\n",size);

	return(NULL);

}

VOID GI_FreeDraw()
{

	FreeRemember(&DrawStore, TRUE);

}

UBYTE *GI_DoAlloc(size,remlist)
int size;
struct Remember **remlist;
{

	UBYTE *outloc;

	outloc = AllocRemember(remlist, size, MEMF_FAST);

	if (outloc!=NULL) return(outloc);	/* We got fast memory */

	outloc = AllocRemember(remlist, size, MEMF_CHIP);

	if (outloc!=NULL) return(outloc);	/* Need to use chip memory */

	printf("Unable to allocate %d mainline bytes\n",size);

	return(NULL);

}

VOID GI_DoFree(remlist)
struct Remember **remlist;
{

	FreeRemember(remlist, TRUE);

}

VOID GI_RefreshGadgets()
{

	RefreshGadgets(w->FirstGadget,w,NULL);

}

VOID GI_GetColor(findcol,red,green,blue)
ULONG findcol,*red,*green,*blue;
{

	ULONG incol,tred,tgreen,tblue;

	incol = GetRGB4(vp->ColorMap,findcol&0xF);
	tred = (incol>>8)&0x0F;
	tgreen = (incol>>4)&0xF;
	tblue = incol&0xF;

	*red = tred<<12;
	*green = tgreen<<12;
	*blue = tblue<<12;

}

VOID GI_ColorString(findcol,outstr)
ULONG findcol;
UBYTE *outstr;
{

	ULONG red,green,blue;

	GI_GetColor(findcol,&red,&green,&blue);
	
	sprintf(outstr," %01x%01x%01x ",red>>12,green>>12,blue>>12);
}

VOID GI_SetColor(colset,red,green,blue)
ULONG colset,red,green,blue;
{

	SetRGB4(vp, colset&0xF, red>>12, green>>12, blue>>12);

}

/*
        alloca -- (mostly) portable public-domain implementation -- D A Gwyn

        last edit:      86/05/30        rms
           include config.h, since on VMS it renames some symbols.
           Use malloc instead of malloc.

        This implementation of the PWB library alloca() function,
        which is used to allocate space off the run-time stack so
        that it is automatically reclaimed upon procedure exit,
        was inspired by discussions with J. Q. Johnson of Cornell.

        It should work under any C implementation that uses an
        actual procedure stack (as opposed to a linked list of
        frames).  There are some preprocessor constants that can
        be defined when compiling for your specific system, for
        improved efficiency; however, the defaults should be okay.

        The general concept of this implementation is to keep
        track of all alloca()-allocated blocks, and reclaim any
        that are found to be deeper in the stack than the current
        invocation.  This heuristic does not reclaim storage as
        soon as it becomes invalid, but it will do so eventually.

        As a special case, alloca(0) reclaims storage without
        allocating any.  It is a good idea to use alloca(0) in
        your main control loop, etc. to force garbage collection.
*/

typedef char    *pointer;               /* generic pointer type */

extern void     free();
extern pointer  malloc();

/*
        Define STACK_DIRECTION if you know the direction of stack
        growth for your system; otherwise it will be automatically
        deduced at run-time.

        STACK_DIRECTION > 0 => grows toward higher addresses
        STACK_DIRECTION < 0 => grows toward lower addresses
        STACK_DIRECTION = 0 => direction of growth unknown
*/

#define STACK_DIRECTION -1

#define STACK_DIR       STACK_DIRECTION /* known at compile-time */

/*
        An "alloca header" is used to:
        (a) chain together all alloca()ed blocks;
        (b) keep track of stack depth.

        It is very important that sizeof(header) agree with malloc()
        alignment chunk size.  The following default should work okay.
*/

#ifndef ALIGN_SIZE
#define ALIGN_SIZE      sizeof(double)
#endif

typedef union hdr
{
  char  align[ALIGN_SIZE];      /* to force sizeof(header) */
  struct
    {
      union hdr *next;          /* for chaining headers */
      char *deep;               /* for stack depth measure */
    } h;
} header;

/*
        alloca( size ) returns a pointer to at least `size' bytes of
        storage which will be automatically reclaimed upon exit from
        the procedure that called alloca().  Originally, this space
        was supposed to be taken from the current stack frame of the
        caller, but that method cannot be made to work for some
        implementations of C, for example under Gould's UTX/32.
*/

static header *last_alloca_header = NULL; /* -> last alloca header */

pointer
alloca (size)                   /* returns pointer to storage */
     unsigned   size;           /* # bytes to allocate */
{
  auto char     probe;          /* probes stack depth: */
  register char *depth = &probe;

                                /* Reclaim garbage, defined as all alloca()ed storage that
                                   was allocated from deeper in the stack than currently. */

  {
    register header     *hp;    /* traverses linked list */

    for (hp = last_alloca_header; hp != NULL;)
      if (STACK_DIR > 0 && hp->h.deep > depth
          || STACK_DIR < 0 && hp->h.deep < depth)
        {
          register header       *np = hp->h.next;

          free ((pointer) hp);  /* collect garbage */

          hp = np;              /* -> next header */
        }
      else
        break;                  /* rest are not deeper */

    last_alloca_header = hp;    /* -> last valid storage */
  }

  if (size == 0)
    return NULL;                /* no allocation required */

  /* Allocate combined header + user data storage. */

  {
    register pointer    new = malloc (sizeof (header) + size);
    /* address of header */

    ((header *)new)->h.next = last_alloca_header;
    ((header *)new)->h.deep = depth;

    last_alloca_header = (header *)new;

    /* User storage begins just after header. */

    return (pointer)((char *)new + sizeof(header));
  }
}

void bcopy(a,b,l)
char *a,*b;
int l;
{

	register int i;
	
	for (i=0; i<l; i++) a[i] = b[i];
}

struct Process *CurProc;		/* current process */
struct CommandLineInterface *CurCLI;	/* current CLI info */
static APTR savewindow;			/* saved process WindowPtr */
static APTR pushwindow;			/* pushed process WindowPtr */

sysinit()
{

	CurProc = (struct Process *)FindTask((char *)NULL);
	CurCLI = (struct CommandLineInterface *)BADDR(CurProc->pr_CLI);
	savewindow = CurProc->pr_WindowPtr;

}

reqoff()
{
	pushwindow = CurProc->pr_WindowPtr;
	CurProc->pr_WindowPtr = (APTR)-1;
}

setreq()
{
	CurProc->pr_WindowPtr = w;
}

/*
 * reqpop -- restore requesters to action at last reqoff
 */
reqpop()
{
	CurProc->pr_WindowPtr = pushwindow;
}

char *getcwd(buf, len)
register char *buf;
int len;
{
	register UBYTE *dirname;

	if (CurCLI == NULL) return(NULL);
	dirname = (UBYTE *)BADDR(CurCLI->cli_SetName);
	if (len < *dirname + 1) return(NULL);
	strncpy(buf, dirname + 1, *dirname);
	buf[*dirname] = 0;
	return(buf);
}

BPTR safe_read(name)
UBYTE *name;
{

	BPTR nfile;

	reqoff();	/* Turn off requester */

	nfile = Open(name, (LONG) MODE_NEWFILE);
	
	reqpop();

	return(nfile);
	
}
