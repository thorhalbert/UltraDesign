/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	virwindow.c - virtual window manager
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
 *	2	 6-May-87 SH 	Remove cursor shadows
 *	3	31-May-87 SH	NODEPTR goes bye-bye
 *	4	16-Aug-88 SH	Render used to blow up if the bounds box was
 *				 not visible on screen
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include <graphics/gfxmacros.h>

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

VOID UpdateTitle(),UpdateCore();
ULONG TotAvail();

/*-------------------------External Symbol Definitions-----------------------*/


/*-------------------------Global Variable Definitions-----------------------*/

BYTE titlearea[80];
UBYTE *Default_Comment = "Default Settings";

/*-------------------------------Code Section--------------------------------*/

VOID DispMess(message)
UBYTE *message;
{

	SetDrPt(rp, 0xFFFF);
	printf(&titlearea,"%s  * %s *\n",ucadname,message);

	/* GI_FillHeader(&titlearea); */
}

/*
 *     +++
 *     +++
 *  +++++++++	Set up initial defaults
 *  +++++++++
 *     +++
 *     +++
 */

STYLE_A default_styles;

default_setups()
{
	filereq_setup();

	strcpy(ClipMask,"*.uvw");
	strcpy(ViewMask,"*.uvw");
	strcpy(LibrMask,"*.ulb");
	strcpy(FontMask,"*.ufnt");
	strcpy(HatchMask,"*.uhat");

	strcpy(LayerMask,"*");
	strcpy(StyleMask,"*");
	strcpy(GroupMask,"*");
	strcpy(PenMask,"*");
	strcpy(FntMask,"*");
	strcpy(ItemMask,"*");

	strcpy(StrokePath,"UltraDesign:Stroke_lib");
	strcpy(TemplatePath,"UltraDesign:templates");
	strcpy(HomePath,"UltraDesign:");
	strcpy(HatchPath,"UltraDesign:Hatch_lib");
	strcpy(RexxPath,"UltraDesign:Rexx");
	strcpy(ReportDev,"PRT:");
		
}

VOID Style_Default()
{
	strcpy(default_styles.Letter_Style_ID,"Template_Style");

	default_styles.draw_font = Root_Font;
	default_styles.Use_Count = 0;
	default_styles.Prime_Height = 5.0;
	default_styles.Aspect = 1.0;
	default_styles.Slant = 0.0;
	
	default_styles.prev_style = NULL;
	default_styles.next_style = NULL;

	Root_Style = &default_styles;
	Style_Last = Root_Style;
	Current_Style = Root_Style;
	Default_Style = Root_Style;
}


VOID BaseVirSetup()
{

	Color_Source = 2;		/* Use Layer Colors */
	
	mensetup(0x0F+Color_Source);	/* check Layer coloring */
	mensetup(0x50);			/* check Line mode */
	mensetup(0x90);			/* check roll on screen */
	mensetup(0x100);		/* degrees with decimal point */
	mensetup(0x114);		/* millimeters */
	
	Root_Node=NULL;
	Last_Node=NULL;
	
	strcpy(default_group.Group_Name,"Group_Root");
	default_group.Use_Count=0;
	default_group.Color_ID=Default_Color;
	default_group.Group_ID=0;
	default_group.Group_Flags=0;
	default_group.Libr_Flags=0;
	default_group.parent=NULL;
	default_group.next=NULL;
	default_group.previous=NULL;
	default_group.descendants=NULL;
	default_group.desc_last=NULL;
	default_group.Comment = Default_Comment;

	Root_Group = Current_Group = &default_group;

	strcpy(default_layer.Layer_Name,"Main_Layer");
	default_layer.Use_Count=0;
	default_layer.Color_ID=Default_Color;
	default_layer.Layer_Flags=0;
	default_layer.Layer_ID=0;
	default_layer.layer_next=NULL;
	default_layer.layer_prev=NULL;
	default_layer.Comment = Default_Comment;

	Root_Layer = Last_Layer = Current_Layer = &default_layer;

	strcpy(default_pen.Pen_Name,"Default_Pen");
	default_pen.Use_Count = 0;
	default_pen.Color_ID = Default_Color;
	default_pen.Line_Pattern = 0xFFFF;	/* Unbroken line */
	default_pen.Hard_Color = 0;		/* Black Black */
	default_pen.pen_next=NULL;
	default_pen.pen_prev=NULL;
	default_pen.Comment = Default_Comment;
	default_pen.PenOrd = 0;
	default_pen.Carousel = 0;
	default_pen.AutoNum = 0;
	default_pen.BitWidth = 1.0;
	default_pen.PenWidth = 0.5;

	Root_Pen = Last_Pen = Current_Pen = Default_Pen = &default_pen;

	Wit_Layer = Dim_Line_Layer = Dim_Box_Layer = Current_Layer;
	Wit_Group = Dim_Line_Group = Dim_Box_Group = Current_Group;
	Wit_Pen = Dim_Line_Pen = Dim_Box_Pen = Current_Pen;
	Dim_Box_Style = Current_Style;

	quietupdate=FALSE;

}

/*
 *     +++
 *     +++
 *  +++++++++	Color by whatever 
 *  +++++++++
 *     +++
 *     +++
 */

BOOL Color_Layer()
{

	Color_Source = 2;
	RepaintWindow();
	return(TRUE);
}

BOOL Color_Group()
{

	Color_Source = 1;
	RepaintWindow();
	return(TRUE);

}

BOOL Color_Pen()
{

	Color_Source = 3;
	RepaintWindow();
	return(TRUE);

}

/*
 *     +++
 *     +++
 *  +++++++++	Screen message handling routines
 *  +++++++++
 *     +++
 *     +++
 */

VOID centerpbuf(inln)
int inln;
{

	UBYTE cbuff[90];
	int slen,ct,a,b;

	slen = strlen(pbuffer);

	ct = (inln-slen)/2;

	for (a=0; a<ct; a++) cbuff[a]=' ';
	for (a=ct,b=0; a<inln; a++,b++) {
		if (pbuffer[b]=='\0') break;
		cbuff[a]=pbuffer[b];
	}
	for (b=a; b<inln; b++) cbuff[b]=' ';
	cbuff[b]='\0';
	strcpy(pbuffer,cbuff);

}

VOID paintinfo()
{

	int rpos;

	if (!Draw_Info) return;

	SetDrMd(rp, JAM2);
	SetAPen(rp,Coord_Color);
	SetBPen(rp,Erased_Color);

	centerpbuf(46);
	rpos = XBegin +1;

	Move(rp,rpos,TOPMARGIN+8);
	Text(rp,pbuffer,strlen(pbuffer));
	v_reset();

}

VOID UpdateCore()
{

	LONG	memavail;
	UBYTE	obuf[10];

	memavail = (FASTAvail()+500)/1000;	/* Round to nearest K */

	if (memavail>999) sprintf(obuf,"%04d",memavail);
	else		sprintf(obuf,"%3dk",memavail);

	SetDrMd(rp, JAM2);
	SetAPen(rp,2);				/* Leave these alone */
	if (memavail<100) SetAPen(rp,5);	/* Blink */
	SetBPen(rp,3);

	Move(rp,4,YSsize-30);
	Text(rp,obuf,strlen(obuf));

	memavail = (CHIPAvail()+500)/1000;	/* Round to nearest K */

	if (memavail>999) sprintf(obuf,"%04d",memavail);
	else		sprintf(obuf,"%3dk",memavail);

	SetDrMd(rp, JAM2);
	SetAPen(rp,2);				/* Leave these alone */
	if (memavail<100) SetAPen(rp,5);	/* Blink */
	SetBPen(rp,3);

	Move(rp,4,YSsize-20);
	Text(rp,obuf,strlen(obuf));


}

/*
 *     +++
 *     +++
 *  +++++++++	Screen header message handling
 *  +++++++++
 *     +++
 *     +++
 */


VOID MainMode(inmode)
UBYTE *inmode;
{

	Main_Mode = inmode;
	Last_Mode = inmode;

	subaction = NULL;
	actionsave = NULL;
	Save_Mode = NULL;
	Save_Mess = NULL;


}

VOID SMainMess(inmess)
struct mesdef *inmess;
{

	Last_Mode = Main_Mode;
	Main_Mess = inmess;
	Last_Mess = inmess;
	UpdateTitle();

}

VOID SInfoMess(inmode,inmess)
UBYTE *inmode;
struct mesdef *inmess;
{

	Last_Mode = inmode;
	Last_Mess = inmess;
	if (inmess->mestype==ERR) DisplayBeep(screen);

	UpdateTitle();

}

VOID RidSpurious()
{

	Last_Mode = Main_Mode;
	Last_Mess = Main_Mess;
}

BOOL SEnterSub(mode,outfirst,actionroutine)
UBYTE *mode;
struct mesdef *outfirst;
VOID (*actionroutine)();
{

	if (subaction!=NULL) {
		SInfoMess(Side_Mode,Side_Out);
		return(FALSE);
	}

	if (actionsave!=NULL) {
		SInfoMess(Main_Mode,Main_Out);
		return(FALSE);
	}

	actionsave = mouseaction;
	mouseaction = actionroutine;
	Main_Out = outfirst;

	Save_Mode = Main_Mode;
	Save_Mess = Main_Mess;

	Main_Mode = mode;
	Last_Mode = mode;

	return(TRUE);
}

BOOL SEnterSide(mode,outfirst,actionroutine)
UBYTE *mode;
struct mesdef *outfirst;
VOID (*actionroutine)();
{

	if (subaction!=NULL) {
		SInfoMess(Side_Mode,Side_Out);
		return(FALSE);
	}

	subaction = mouseaction;
	mouseaction = actionroutine;
	Side_Out = outfirst;

	SaveSide_Mode = Main_Mode;
	SaveSide_Mess = Main_Mess;

	Side_Mode = mode;
	Last_Mode = mode;

	return(TRUE);
}

VOID SExitSub(posmes)
struct mesdef *posmes;
{

	if (actionsave==NULL) return;

	mouseaction = actionsave;
	Main_Mode = Save_Mode;
	Last_Mode = Save_Mode;

	Main_Mess = Save_Mess;

	if (posmes) {
		SInfoMess(Save_Mode,posmes);
		Last_Mess = Main_Mess;
	} else
		SMainMess(Main_Mess);

	actionsave = NULL;
	Save_Mode = NULL;
	Save_Mess = NULL;

	return;
}

VOID SExitSide(posmes)
struct mesdef *posmes;
{

	if (subaction==NULL) return;

	mouseaction = subaction;
	Main_Mode = SaveSide_Mode;
	Last_Mode = SaveSide_Mode;

	Main_Mess = SaveSide_Mess;

	if (posmes) {
		SInfoMess(SaveSide_Mode,posmes);
		Last_Mess = SaveSide_Mess;
	} else
		SInfoMess(SaveSide_Mode,SaveSide_Mess);

	subaction = NULL;
	SaveSide_Mode = NULL;
	SaveSide_Mess = NULL;

	return;
}

VOID UpdateTitle()
{

	SetDrPt(rp, 0xFFFF);
	switch (Last_Mess->mestype) {
	case ERR:
		sprintf(&titlearea,"%s [%s] ¿¿ %s ??",ucadname,
				Last_Mode,Last_Mess->mesloc);
		break;
	case WAR:
		sprintf(&titlearea,"%s [%s] ¡¡ %s !!",ucadname,
				Last_Mode,Last_Mess->mesloc);
		break;
	case ACT:
		sprintf(&titlearea,"%s [%s] ¤¤ %s ¤¤",ucadname,
				Last_Mode,Last_Mess->mesloc);
		break;
	case INF:
	default:
		sprintf(&titlearea,"%s [%s] ·· %s ··",ucadname,
				Last_Mode,Last_Mess->mesloc);
		break;
	}

	GI_FillHeader(&titlearea);
	UpdateCore();

}

VOID TitleMessage(inmess)
UBYTE *inmess;
{

	SetDrPt(rp, 0xFFFF);
	
	sprintf(&titlearea,"%s [dialog] %s",ucadname,inmess);
	
	GI_FillHeader(&titlearea);
	UpdateCore();
}
	
/*
 *     +++
 *     +++
 *  +++++++++	complain and one-liner code
 *  +++++++++
 *     +++
 *     +++
 */

struct IntuiText comptext[] = {
 { 5, 1, JAM2, 6, 5, NULL, NULL, NULL },
 { 5, 1, JAM2, 6, 15, NULL, NULL, NULL },
 { 5, 1, JAM2, 6, 25, NULL, NULL, NULL },
 { 5, 1, JAM2, 6, 35, NULL, NULL, NULL }};
struct IntuiText commes[] = {
 { 5, 3, JAM2, 6, 3, NULL, NULL, NULL },
 { 5, 3, JAM2, 6, 3, NULL, NULL, NULL }};

BOOL complain(nm,pm,c1,c2,c3,c4,l1,l2,l3,l4)
UBYTE *nm,*pm,*l1,*l2,*l3,*l4;
int c1,c2,c3,c4;
{

	int bot,i,wid;
	struct IntuiText *nmi,*pmi;
	struct Window *reqwin;
   	ULONG class,signals;
   	USHORT code;
	struct Gadget *GetGadget;
	BOOL breakout,answer;

    	ULONG kep_IDCMPFlags;   /* User-selected flags */
    	struct MsgPort *kep_UserPort, *kep_WindowPort;
    	struct IntuiMessage *kep_MessageKey;

	comptext[0].IText = l1;
	comptext[1].IText = l2;
	comptext[2].IText = l3;
	comptext[3].IText = l4;

	comptext[0].FrontPen = c1;
	comptext[1].FrontPen = c2;
	comptext[2].FrontPen = c3;
	comptext[3].FrontPen = c4;

	wid = 0;
	for (i=0; i<4; i++) {
		comptext[i].NextText=NULL;
		if (comptext[i].IText==NULL) break;
		wid = max(wid, IntuiTextLength(&comptext[i]));
		if (i>0) comptext[i-1].NextText = &comptext[i];
		bot = comptext[i].TopEdge;
	}

	if (wid>600) wid=600;

	commes[0].IText = nm;
	nmi = (nm==NULL)?&commes[1]:&commes[0];
	commes[1].IText = pm;
	pmi = (pm==NULL)?&commes[0]:&commes[1];

	if (w!=NULL) {
		Forbid();
    		kep_IDCMPFlags = w->IDCMPFlags;
		kep_UserPort = w->UserPort;
		kep_WindowPort = w->WindowPort;
		kep_MessageKey = w->MessageKey;
		Permit();
	}
	
	reqwin = BuildSysRequest(w,&comptext[0],pmi,nmi,
		GADGETUP | GADGETDOWN | RAWKEY,
		wid+36,bot+50);
		
	if (reqwin==FALSE) return(FALSE);
	if (reqwin==TRUE) return(TRUE);		/* Got an alert not a req */
	
/*	ClearPointer(w);
	PointerOn=FALSE; */

	breakout = FALSE;
	while (!breakout) {

		signals = Wait(1L << reqwin->UserPort->mp_SigBit);

		if (signals!=0) {

			while (message=GetMsg(reqwin->UserPort)) {

				class = message->Class;
				code  = message->Code;
	
				ReplyMsg(message);
	
				switch (class) {
	
				case GADGETDOWN:
					GetGadget=message->IAddress;
					answer = !(GetGadget->GadgetID);
					breakout=TRUE;
					break;
				case GADGETUP:
					GetGadget=message->IAddress;
					answer = !(GetGadget->GadgetID);
					breakout=TRUE;
					break;
				case RAWKEY:
					switch(code) {
					case 0x45:
						answer = TRUE;
						breakout = TRUE;
						break;
					case 0x44:
					case 0x43:
						answer = FALSE;
						breakout = TRUE;
						break;
					}
				}
			}
		}		
	}
	
	FreeSysRequest(reqwin);

	if (w!=NULL) {
		Forbid();
    		kep_IDCMPFlags = w->IDCMPFlags;
		kep_UserPort = w->UserPort;
		kep_WindowPort = w->WindowPort;
		kep_MessageKey = w->MessageKey;
		Permit();
	}

	return(answer);	

}

VOID oneliner(tex)
UBYTE *tex;
{

	(VOID) complain(NULL,"Recover",4,0,0,0,tex,NULL,NULL,NULL);
}

LOCAL UBYTE sbuf[255];
UBYTE *MakeFname(path,name,ext)
UBYTE *path,*name,*ext;
{
	int i;
	UBYTE *nb = "";
	
	if (ext==NULL) ext = nb;

	i = strlen(path);

	while (i>0&&path[i-1]==' ') {
		path[i-1] = '\0';
		i--;
	}
	
	if (path[i-1]=='/'||path[i-1]==':')
		sprintf(sbuf,"%s%s%s",path,name,ext);
	else	sprintf(sbuf,"%s/%s%s",path,name,ext);
	
	return(sbuf);
}

/*
 *     +++
 *     +++
 *  +++++++++	Make standard pretty borders for requesters
 *  +++++++++
 *     +++
 *     +++
 */


VOID DoBorders(inreq,xlin,btop,bbot)
struct Requester *inreq;
int xlin,btop,bbot;
{

	struct RastPort *reqrp;

	reqrp = inreq->ReqLayer->rp;	/* Nasty */
		
	SetAPen(reqrp, 2);
	Move(reqrp, 0, 0);
	Draw(reqrp, (inreq->Width-1), 0);
	Draw(reqrp, (inreq->Width-1), 
		inreq->Height-1);
	Draw(reqrp, 0, inreq->Height-1);
	Draw(reqrp, 0, 0);

	SetAPen(reqrp, 0);
	Move(reqrp, 3, 2);
	Draw(reqrp, (inreq->Width-1)-3, 2);
	Draw(reqrp, (inreq->Width-1)-3, 
		inreq->Height-3);
	Draw(reqrp, 3, inreq->Height-3);
	Draw(reqrp, 3, 2);
		
	if (xlin>0) {
		Move(reqrp, 3, xlin);
		Draw(reqrp, (inreq->Width-1)-3, xlin);
	}
	
	if (btop>0) {	

		SetAPen(reqrp, 3);
		SetBPen(reqrp, 3);
		SetOPen(reqrp, 2);
		RectFill(reqrp, 8,btop,inreq->Width-8,bbot); 
	}
}
