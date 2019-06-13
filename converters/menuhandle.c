/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	menuhandle.c - menu handler
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
 *	2	16-May-87 SH	Add new attribute menu items
 *	3	13-Jun-87 SH	Add scale and pallette
 *	4	22-Jun-87 SH	Rewrite to be table driven 
 *	5	 1-Aug-87 SH	Use new menu items
 *	6	 3-Sep-87 SH	A new set of menus again
 *	7	14-Oct-87 SH	Make into menu handler
 *	8	 6-Dec-87 SH	Remove capslock from cap qualifiers
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include <libraries/dosextens.h>
#include <graphics/gels.h>
#include <graphics/gfxmacros.h>
#include <exec/errors.h>
#include <devices/timer.h>

/*-------------------definitions for menus-menus.h--------------------------*/

struct menu_access {
	USHORT level;			/* 1 is a menu level 2 is item 3 sub */
	BOOL	*access;		/* Access to bool toggle */
	BOOL	(*Implement)();		/* Execute function */
	BOOL	activate;		/* Do It? */
	UWORD	qualmask;		/* Qualifier Mask */
	UWORD	keycode;		/* Key Code Entered */
	UWORD	excludec;		/* Special exclude code */
	struct MenuItem *menptr;
	WORD	menu,item,sub;
};

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

BOOL FileRequest();
extern struct Screen *screen;
extern struct Window *w;
BOOL 	ConditionMouse();
VOID	UpdateMousePosition();

VOID	Load_View();
VOID	Load_Dxf();
VOID	Load_Intro();
VOID	Load_Aegis();
VOID	Load_Hpgl();
VOID	Load_Dxy();
VOID	Exit_Quit();

VOID	Margin_Current();
VOID	PalDo();
VOID	dorepaint();

VOID	N_Save_View();
VOID	Save_Dxf();
VOID	Save_ILBM();
VOID	Save_Hat();

VOID dopointer();

/*-------------------------Global Variable Definitions-----------------------*/

#define ALT_KEYS 	(IEQUALIFIER_LALT | IEQUALIFIER_RALT)
#define SHIFT_KEYS \
	(IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT) 
#define SHIFT_ONLY 	(IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT) 
#define CTRL_KEYS 	(IEQUALIFIER_CONTROL) 
#define ANY_ALT (ALT_KEYS | SHIFT_KEYS | CTRL_KEYS)

struct menu_access access_tables[] = {
{ 1, NULL, NULL, 	TRUE, 	0, 	0 },		/* MENU_PROJECT */

{ 2, NULL, Load_View, 	TRUE, 	0, 	0 },		/* load definition */
{ 2, NULL, Load_Dxf,	TRUE,	0,	0 },		/* save definition */
{ 2, NULL, Load_Intro,	TRUE, 	0,	0 },		/* change setup */
{ 2, NULL, Load_Aegis,	TRUE,	0,	0 },		/* Load Aegis file */
{ 2, NULL, Load_Hpgl,	TRUE,	0,	0 },		/* Load HPGL  */
{ 2, NULL, Load_Dxy,	TRUE,	0,	0 },		/* Load DXY File */
{ 2, NULL, Exit_Quit,	TRUE,	0,	0 },		/* quit out */

{ 1, NULL, NULL,	TRUE, 	0, 	0 },		/* view */

{ 2, NULL, PalDo	,TRUE,0, 	0 },		/* Load View */
{ 2, NULL, Margin_Current,TRUE,0, 	0 },		/* Load View */
{ 2, NULL, dorepaint,	TRUE,	0,	0 },		/* Repaint */

{ 1, NULL, NULL,	TRUE, 	0, 	0 },		/* Plot */

{ 2, NULL, N_Save_View,	TRUE, 	0, 	0 }, 		/* Save Setups */
{ 2, NULL, Save_Dxf,	TRUE,	0, 	0 },		/* Preview */
{ 2, NULL, Save_ILBM,	TRUE,	0, 	0 },		/* Preview */
{ 2, NULL, Save_Hat,	TRUE,	0,	0 },		/* Save hatch file */

{ 0, NULL, NULL, FALSE, 0, 0 }};

/*-------------------------------Code Section--------------------------------*/

VOID	Init_Tables()
{

	int a;
	struct menu_access *menidx;
	int menu,item,sub;

	ClearMenuStrip(w);

	menu = item = sub = -1;

	for (a=0; access_tables[a].level>0; a++) {
		menidx = &access_tables[a];
		switch(menidx->level) {
		case 1:
			menu ++;
			item = sub = -1;
			break;
		case 2:
			item ++;
			sub = -1;
			break;
		case 3:
			sub ++;
			break;
		}

		menidx->menu = menu;
		menidx->item = item;
		if (menidx->item<0) menidx->item = NOITEM;
		menidx->sub = sub;
		if (menidx->sub<0) menidx->sub = NOSUB;

		menidx->menptr = ItemAddress(&Titles[0], 
			SHIFTMENU(menu) | SHIFTITEM(item) | SHIFTSUB(sub));

		menidx->menptr->Flags &= ~(ITEMENABLED | CHECKED);
		if (menidx->activate) menidx->menptr->Flags |= ITEMENABLED;
		if (menidx->access!=NULL) {
			if (*(menidx->access)) 
				menidx->menptr->Flags |= CHECKED;
		}
	}

	SetMenuStrip(w,&Titles[0]);

}

VOID	mensetup(setcode)
int setcode;
{

	int a,setc;
	struct menu_access *menidx;

	if (setcode==0) setcode = 0xFFFF;	/* Extra special case */
	setc = setcode>>4;

	for (a=0; access_tables[a].level>0; a++) {
		menidx = &access_tables[a];
					
		if (menidx->excludec>0) menidx->menptr->Flags |= CHECKIT;

		if (menidx->excludec==setcode) {
			menidx->menptr->Flags |= CHECKED;
			continue;
		}
		if ((menidx->excludec>>4)==setc) {
			menidx->menptr->Flags &= ~CHECKED;
			continue;
		}
		if (menidx->access!=NULL) {
			menidx->menptr->Flags |= CHECKIT;
			menidx->menptr->Flags &= ~CHECKED;
			if (*(menidx->access)) 
				menidx->menptr->Flags |= CHECKED;
		}

	}

}

fullmenu(incode)
int incode;
{	

	ClearMenuStrip(w);

	mensetup(incode);

	SetMenuStrip(w,&Titles[0]);
}

VOID Perform_Menu(menidx)
struct menu_access *menidx;
{	

	BOOL result;

/*	RidSpurious(); */

	ClearMenuStrip(w);

	if (!menidx->activate) {

		DisplayBeep(screen);
		/* InfoMess(DISPATCH_MODE,DISPATCH_UNDERCONST); */
			
		return;
	}

	if (menidx->access!=NULL) {
		*(menidx->access) = 
			(*menidx->access) ? FALSE:TRUE;

		menidx->menptr->Flags &= ~CHECKED;
		if (*(menidx->access)) 
			menidx->menptr->Flags |= CHECKED;
	}

	if (menidx->excludec!=0) mensetup(menidx->excludec);

	if (menidx->Implement!=NULL) {
		result=(*menidx->Implement)();
	}

/*	UpdateTitle();	*/
	SetMenuStrip(w,&Titles[0]);
}

VOID	MenuActivated(incode)
int incode;
{

	int menu,item,sub;
	int a;
	struct menu_access *menidx;

	if (incode==MENUNULL) return;

	menu = MENUNUM(incode);
	item = ITEMNUM(incode);
	sub = SUBNUM(incode);

/*	printf("menu=%d, item=%d, sub=%d\n",menu,item,sub); */

	for (a=0; access_tables[a].level>0; a++) {
		menidx = &access_tables[a];
					
		if (menu!=menidx->menu) continue;
		if (item!=menidx->item) continue;
		if (sub!=menidx->sub) continue;

		Perform_Menu(menidx);
	}
}

VOID	KeyStroke(code,qual)
int code,qual;
{

	int a,p= -2;
	struct menu_access *menidx;

	if (code>=0x60) return;

/*	RidSpurious(); */

	switch(code){
	case 0x4F:	return;
	case 0x4E:	return;

	}

	qual &= ANY_ALT;

	for (a=0; access_tables[a].level>0; a++) {
		menidx = &access_tables[a];
					
		if (menidx->qualmask!=0) {
			if (!(menidx->qualmask & qual)) continue;
		} else if (qual!=0) continue;

		if (menidx->keycode!=code) continue;

		Perform_Menu(menidx);
		return;
	}

	/* InfoMess(DISPATCH_MODE,DISPATCH_ILLEGAL); */
	printf("What was key %x qual %x\n",code,qual);

}

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

VOID PointerAction();
VOID remshadows();

/*-------------------------External Symbol Definitions-----------------------*/

extern BOOL selected_clip;

/*-------------------------Global Variable Definitions-----------------------*/

VOID (*mouseaction)();
VOID (*actionsave)()=NULL;
VOID (*CleanupShadows)();

LOCAL BOOL PointerOn,MenusOn;
LOCAL BOOL Rbuttondown,Lbuttondown;
LOCAL WORD mx,my,ox,oy,omx,omy;
LOCAL STDFLT xreal,yreal;

extern struct SpriteImage BlankPointer;

LOCAL UWORD cstep=0;
LOCAL ULONG nclock=0;

LOCAL BOOL  active=TRUE;

LOCAL ULONG LastSec=0;
extern BOOL drawing_dirty;

ULONG 	IDCMP_Bit = 0,
	Timer_Bit = 0;

struct MsgPort *timerport = NULL;
struct timerequest timermsg;

#define TIME_CONST 10000L

/*-------------------------------Code Section--------------------------------*/

VOID ExecuteObject(ingad)
struct Gadget *ingad;
{

	VOID (*service)();

	if (ingad->UserData==NULL) return;

	service = ingad->UserData;
	(*service)(ingad);

}

VOID DoTime(in)
ULONG in;
{

	if (timerport==NULL) return;
	timermsg.tr_node.io_Command = TR_ADDREQUEST;
	timermsg.tr_time.tv_secs = 0;
	timermsg.tr_time.tv_micro = in;
	
	SendIO(&timermsg.tr_node);
}

VOID DeleteTimer()
{

	struct IORequest *trreq;

	if (timerport==NULL) return;

	trreq = &timermsg.tr_node;
	if (GetMsg(timerport)==NULL) {
		if (CheckIO(trreq)==NULL) {
			AbortIO(trreq);
			if ((int) trreq->io_Error==IOERR_ABORTED) {
				WaitIO(trreq);
				GetMsg(timerport);
			}
		}
	}

	CloseDevice(trreq);
	DeletePort(timerport);

	timerport = NULL;
	
}

BOOL CreateTimer()
{

	timerport = CreatePort("ucadport",0);
	if (timerport==NULL) return(FALSE);

	if (OpenDevice(TIMERNAME, UNIT_VBLANK, &timermsg.tr_node, 0)!=0) {
		DeleteTimer();
		return(FALSE);
	}

	timermsg.tr_node.io_Message.mn_ReplyPort = timerport;
	timermsg.tr_node.io_Flags = 0;
	timermsg.tr_node.io_Error = 0;

	Timer_Bit = (1L << timerport->mp_SigBit);
	DoTime(100L);

	return(TRUE);
}

VOID DoActivation(class,code,qual)
ULONG class;
USHORT code,qual;
{

	struct Gadget *GetGadget;

	if (MenusOn&&qual==IEQUALIFIER_RBUTTON) return;

	switch (class) {
	case ACTIVEWINDOW: 
		active=TRUE;
		break;
	case INACTIVEWINDOW:
		active=FALSE;
		break;
	case MOUSEBUTTONS:
		if (!ConditionMouse(mx,my,TRUE)) break;
		switch (code) {
		case SELECTDOWN:
			dotx = xreal;
			doty = yreal;
			dotset = TRUE;
			PointerAction(mx,my,LBUTTONDOWN,xreal,yreal);
			break;
		case MENUDOWN:
			PointerAction(mx,my,RBUTTONDOWN,xreal,yreal);
			break;
		}
		break;
	case GADGETUP:
		GetGadget=message->IAddress;
		ExecuteGadgetServer(GetGadget);
		break;
	case MENUPICK:
		MenuActivated(code);
		break;
	case RAWKEY:
		if (code==0x45) PointerAction(mx,my,RBUTTONDOWN,xreal,yreal);
		else	KeyStroke(code,qual);
		break;			
	}
}

VOID Loop_Always()
{

	ULONG class,signals;
	USHORT code,qual;

	Timer_Bit = (1L << timerport->mp_SigBit);
	IDCMP_Bit = (1L << w->UserPort->mp_SigBit);

	signals = Wait(IDCMP_Bit|Timer_Bit);

	if (signals & Timer_Bit) {
	
		while (message=GetMsg(timerport)) {
			mx = w->MouseX;
			my = w->MouseY;
			if (active) (VOID) ConditionMouse(mx,my,FALSE);
			DoTime(TIME_CONST);
		}
	}

	if (signals & IDCMP_Bit) {
		while (message=GetMsg(w->UserPort)) {

			StoSecs = message->Seconds;
			StoMics = message->Micros;
			mx = message->MouseX;
			my = message->MouseY;

			class = message->Class;
			code = message->Code;
			qual = message->Qualifier;
			ReplyMsg(message);
	
			DoActivation(class, code, qual);
		}
	}
	
}

VOID wait_close() 
{

   	ULONG a;

	for (a=0; a<10; a++) {
		keyx[a]=keyy[a]= 0.0;
		keyset[a]=FALSE;
		xvbeg[a]=yvbeg[a]=xvsiz[a]=yvsiz[a]=0.0;
		vset[a]=FALSE;
	}

	dotx=doty = 0.0;

	dotset=FALSE;

	ox=oy=omx=omy= -1;

	PointerOn=FALSE;
	MenusOn=TRUE;
	
	CreateTimer();

	Rbuttondown=Lbuttondown=FALSE;

	IDCMP_Bit = (1L << w->UserPort->mp_SigBit);

	turn_mouse_on();

	FOREVER Loop_Always();

}

BOOL ConditionMouse(amx,amy,upd)
int amx,amy;
BOOL upd;
{

	BOOL rcal;
	STDFLT sx,sy;
	int tx,ty;

	if (amx==keepx&&amy==keepy) {
		if (upd) UpdateMousePosition();
		return(TRUE);
	}

	mx = keepx = amx;
	my = keepy = amy;

	if (w->Flags&MENUSTATE) {
		/* ClearPointer(w); */
		PointerOn = FALSE;
		return(FALSE);
	}

	if (my>=YBegin&&MenusOn) {
		/* Forbid();
		w->Flags|= RMBTRAP;
		Permit(); */
		MenusOn=FALSE;
	}
	if (my<YBegin&&!MenusOn) {
		/* Forbid();
		w->Flags&= ~RMBTRAP;
		Permit(); */
		MenusOn=TRUE;
		return(FALSE);
	}
					
	if (my<YBegin||
	    my>YBegin+YWidth||
	    mx<XBegin||
	    mx>XBegin+XWidth) {
		/* if (PointerOn) ClearPointer(w); */
		remshadows();
		PointerOn=FALSE;
		return(FALSE);
	} else {
		/* SetPointer(w, &BlankPointer, 
			1, 1, 0, 0); */
		PointerOn=TRUE;
	}

	if (mx<XBegin) mx=XBegin;
	if (my<YBegin) my=YBegin;
			
	if (mx>XBegin+XWidth) mx=XBegin+XWidth-1;
	if (my>YBegin+YWidth) my=YBegin+YWidth-1;

/*-------Map the screen coordinate system to the virtual window------------*/

	rcal = FALSE;

	tx = mx - XBegin;
	ty = YWidth - (my - YBegin);

	sx = tx;
	sy = ty;

	xreal = xviewbegin + (sx * xpixsize);
	yreal = yviewbegin + (sy * ypixsize);		

/*	if (Snaps_On) {

		xreal = (floor((xreal/xtsnap)+.5) * xtsnap)+xoffset;
		yreal = (floor((yreal/ytsnap)+.5) * ytsnap)+yoffset;
		rcal = TRUE;
	}	*/

/*	if (Use_Bounds) {
		if (xreal<xviewMIN) {
			xreal = xviewMIN;
			rcal = TRUE;
		} else 	if (xreal>xviewMAX) {
			xreal = xviewMAX;
			rcal = TRUE;
		}

		if (yreal<yviewMIN) {
			yreal = yviewMIN;
			rcal = TRUE;
		} else 	if (yreal>yviewMAX) {
			yreal = yviewMAX;
			rcal = TRUE;
		}
		
	} */

	if (rcal) {

		sx = xreal - xviewbegin;
		sy = yreal - yviewbegin;
		sx /= xpixsize;
		sy /= ypixsize;
		tx = sx;
		ty = sy;
		mx = tx + XBegin;
		my = (YWidth - ty) + YBegin;



	}	

	dopointer();

	UpdateMousePosition();
	return(TRUE);

}

VOID UpdateMousePosition()
{
	WORD kmx,kmy,rpos,fd;
	BOOL vms;
	UBYTE	*absmask="[%s,%s]";
	UBYTE	*tmpmask="{%s,%s}";
	UBYTE	*usemask;

	if (Coord_On||Draw_Info) fd = 10;
	else	fd = 0;

/*
	if (Coord_On) {
	
		SetDrMd(rp, JAM2);
		SetAPen(rp,Coord_Color);
		SetBPen(rp,Erased_Color);

		if (x_origin!=0.0||y_origin!=0.0) 
			usemask = tmpmask;
		else	usemask = absmask;
		sprintf(pbuffer,usemask,
			DispLin(xreal-x_origin),
			DispLin(yreal-y_origin));
		centerpbuf(26);
		rpos = XBegin + XWidth - 26*8;

		Move(rp,rpos,TOPMARGIN+8);
		Text(rp,pbuffer,strlen(pbuffer));
		v_reset();
	}	*/

	if (Lbuttondown) 	PointerAction(omx,omy,MOVEMENT,xreal,yreal);
	else			PointerAction(omx,omy,UMOVEMENT,xreal,yreal);

	return;

}

VOID remshadows()
{

	int fd;

	SetDrPt(rp, 0xFFFF);
	
	SetDrMd(rp, COMPLEMENT | JAM1);
	SetAPen(rp,2);

	if (ox>=0&&oy>=0) {
		/* if (Coord_On||Draw_Info) fd = 10;
		else */	fd = 0;

		Move(rp,ox,YBegin+fd);
		Draw(rp,ox,YBegin+YWidth-1);
		Move(rp,XBegin,oy);
		Draw(rp,XBegin+XWidth-1,oy);
		ox=oy=-1;
	}

}

VOID dopointer()
{

	remshadows();
	
	if (mouseaction==NULL) return;	/* No pointer if no action */
	
	ox = mx;
	oy = my;
	remshadows();
	ox = mx;
	oy = my;
}

VOID PointerAction(ix,iy,buttonaction,fx,fy)
WORD ix,iy,buttonaction;
STDFLT fx,fy;
{

	if (mouseaction==NULL) return;

	(*mouseaction)(ix,iy,buttonaction,fx,fy);

}

/* While we are idling, make the selected and clipped colors pulsate */

Oscillate()
{
	struct DateStamp instamp;
	ULONG rintvl;

	DateStamp(&instamp);

	rintvl=(instamp.ds_Tick)>>3;

	if (rintvl==nclock) return;

	cstep+=2;

	if (cstep>15) cstep=7;
	if (cstep<1) cstep=7;

	/*SetRGB4(vp, 5, 0,0,cstep); */
	SetRGB4(vp, 4, 0,cstep,0);

	nclock=rintvl; 

	return;

}		

turn_basics()
{

	ModifyIDCMP(w,	ACTIVEWINDOW | INACTIVEWINDOW);

}

turn_mouse_on()
{

	if (!active) {
		turn_basics();
		return;
	}

	ModifyIDCMP(w,	MOUSEBUTTONS | MENUPICK | INTUITICKS |
			GADGETUP | RAWKEY | GADGETDOWN |  REQCLEAR |
			ACTIVEWINDOW | INACTIVEWINDOW);

}

turn_mouse_off()
{

	if (!active) {
		turn_basics();
		return;
	}

	ModifyIDCMP(w,	MOUSEBUTTONS | MENUPICK | GADGETUP | 
			RAWKEY | GADGETDOWN |  REQCLEAR | 
			ACTIVEWINDOW | INACTIVEWINDOW);
}

sit_n_spin(atype,escape,affirm)	/* Wait until the last requester clears the screen */
int atype;
struct Gadget *escape,*affirm;
{

   	ULONG class,signals;
   	USHORT code;
	struct Gadget *GetGadget,*RatMove=NULL;
	struct Gadget *SaveGadget=NULL;

	ClearPointer(w);
	PointerOn=FALSE;
	
	turn_mouse_on();

	Timer_Bit = (1L << timerport->mp_SigBit);
	IDCMP_Bit = (1L << w->UserPort->mp_SigBit);

	FOREVER {
		signals = Wait(IDCMP_Bit|Timer_Bit);

		if (signals & Timer_Bit) {
	
			while (message=GetMsg(timerport)) {
				if (atype==1&&RatMove!=NULL) 
					ExecuteGadgetServer(RatMove);
				DoTime(TIME_CONST);
			}
		}

		if (signals & IDCMP_Bit) {

			while (message=GetMsg(w->UserPort)) {

				class = message->Class;
				code  = message->Code;
				StoSecs = message->Seconds;
				StoMics = message->Micros;
	
				ReplyMsg(message);
	
				switch (class) {
	
				case GADGETDOWN:
					GetGadget=message->IAddress;
					if ((GetGadget->GadgetType&0x0F)==PROPGADGET)
						RatMove=message->IAddress;
					if (SaveGadget!=NULL&&SaveGadget!=GetGadget) {
						ExecuteGadgetServer(SaveGadget);
						SaveGadget = NULL;
					}
					if ((GetGadget->GadgetType&0x0F)==STRGADGET) {
						SaveGadget = GetGadget;
					} else 	SaveGadget = NULL;
					break;
				case GADGETUP:
					GetGadget=message->IAddress;
					RatMove = NULL;
					ExecuteGadgetServer(GetGadget);
					SaveGadget = NULL;
					break;
				case REQCLEAR:
					SaveGadget = NULL;
					RatMove = NULL;
					return;
				case RAWKEY:
					printf("Raw key %x\n",code);
					switch (code) {
					case 0x45:
					case 0x46:
 					  if (escape) 
					    ExecuteGadgetServer(escape);
					  break;
					case 0x43:
					case 0x44:
					  if (affirm)
					    ExecuteGadgetServer(affirm);
					  break;
					}
				}
			}
		}		
	}
}
