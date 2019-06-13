/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	pointers.c - digitizer input controller and main dispatch loop
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
 *	2	28-Apr-87 SH	Select can be purple if also in clip
 *	3	 8-May-87 SH	Make arrow keys the roller, make the
 *				right button passable to the servers
 *	4	19-Jun-87 SH	Update display of coords
 *	5	23-Jun-87 SH	Add table dispatch and selective coords
 *	6	20-Aug-87 SH	Add the viewport storage shorthand
 *	7 	30-Aug-87 SH	Ignore the MOUSEMOVEs now, and just use
 *				the last X and Y from INTUITICKS when
 *				it changes
 *	8	 4-Oct-87 SH	Add a screen saver function
 *	9	17-Jul-88 SH	Start Implementing timer, get rid of INTUITICKS
 *	10	 7-Aug-88 SH	Start implementing VSprite pointer
 *	11	16-May-90 SH	Alternate snap scheme for freehand
 *
 */

/*-------------------------------Include Files-------------------------------*/
#include "ucad.h"
#include "menus.h"
#include <libraries/dosextens.h>
#include <graphics/gels.h>
#include <graphics/gfxmacros.h>
#include <exec/errors.h>
#include <devices/timer.h>

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

VOID PointerAction();
VOID remshadows();
VOID dopointer();
VOID (*dialogcheck)();

/*-------------------------External Symbol Definitions-----------------------*/

extern BOOL selected_clip;
extern struct VSprite SpriteStruct;
struct View mv;

/*-------------------------Global Variable Definitions-----------------------*/

VOID (*mouseaction)();
VOID (*actionsave)()=NULL;
VOID (*subaction)()=NULL;
VOID (*WrapupDrawMode)();
VOID (*CleanupShadows)();
WORD CurrentDrawMode;

LOCAL BOOL PointerOn,MenusOn;
LOCAL BOOL Rbuttondown,Lbuttondown;
LOCAL WORD mx,my,ox,oy,omx,omy,orx,ory;
LOCAL BOOL orp;
LOCAL STDFLT xreal,yreal;

LOCAL UWORD cstep=0;
LOCAL ULONG nclock=0;

LOCAL BOOL  active=TRUE;

LOCAL ULONG LastSec=0;

ULONG 	IDCMP_Bit = 0,
	Timer_Bit = 0;
	
#ifdef OLDCONSOLEHANDLER	/* Yuck */	
	
ULONG	dialIDCMP_Bit = 0,
	dialCon_Bit = 0;

#endif

#ifdef USE_AREXX
ULONG	REXX_Bit = 0;
extern struct MsgPort UcadPort;
extern BOOL UcadSetup;
#endif

struct MsgPort *timerport = NULL;
struct timerequest timermsg;

#define TIME_CONST 10000L

/*-------------------------------Code Section--------------------------------*/

#ifdef SCREEN_SAVER
VOID SetSaverWait()
{

	Screen_Counter = Screen_Save_Time;

}
#endif

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

	if (OpenDevice(TIMERNAME, UNIT_MICROHZ, &timermsg.tr_node, 0)!=0) {
		DeleteTimer();
		return(FALSE);
	}

	timermsg.tr_node.io_Message.mn_ReplyPort = timerport;
	timermsg.tr_node.io_Flags = 0;
	timermsg.tr_node.io_Error = 0;

	Timer_Bit = (1L << timerport->mp_SigBit);
	DoTime(TIME_CONST);

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
		if (!ConditionMouse(mx,my,TRUE,TRUE)) break;
		switch (code) {
		case SELECTDOWN:
			dotx = xreal;
			doty = yreal;
			dotset = TRUE;
			PointerAction(mx,my,LBUTTONDOWN,xreal,yreal);
			Lbuttondown=TRUE;
			Rbuttondown=FALSE;
			break;
		case SELECTUP:
			Lbuttondown=FALSE;
			break;
		case MENUDOWN:
			PointerAction(mx,my,RBUTTONDOWN,xreal,yreal);
			Rbuttondown=TRUE;
			Lbuttondown=FALSE;
			break;
		case MENUUP:
			Rbuttondown=FALSE;
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

#ifdef OLDCONSOLEHANDLER	/* Yuck */	

VOID DoDialActivation(class,code,qual)
ULONG class;
USHORT code,qual;
{


	switch (class) {
	case CLOSEWINDOW:
		closedial();
		break;
	}		
}

extern struct MsgPort *dial_readport;

#endif

VOID Loop_Always()
{

	ULONG class,signals;
	USHORT code,qual;

	Timer_Bit = (1L << timerport->mp_SigBit);
	IDCMP_Bit = (1L << w->UserPort->mp_SigBit);
	
#ifdef USE_AREXX
	if (UcadSetup) REXX_Bit = (1L << UcadPort.mp_SigBit);
#endif
	
#ifdef OLDCONSOLEHANDLER	/* Yuck */	
	dialIDCMP_Bit = 0;
	dialCon_Bit = 0;
	if (dialwin) {
		dialIDCMP_Bit = (1L << dialwin->UserPort->mp_SigBit);
		dialCon_Bit = (1L << dial_readport->mp_SigBit);
	}
#endif


	signals = Wait(IDCMP_Bit|Timer_Bit
#ifdef OLDCONSOLEHANDLER	/* Yuck */
	|dialIDCMP_Bit|dialCon_Bit	
#endif
#ifdef USE_AREXX
	|REXX_Bit
#endif
		);

	if (signals & Timer_Bit) {
	
		while (message=GetMsg(timerport)) {
			mx = w->MouseX;
			my = w->MouseY;
#ifdef SCREEN_SAVER
			if (Screen_Counter<0) {
				if (Screen_Save_On&&active) screensaver();
				SetSaverWait();
			}
#endif
			if (active) (VOID) ConditionMouse(mx,my,FALSE,TRUE);
			DoTime(TIME_CONST);
			(*dialogcheck)();
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
#ifdef SCREEN_SAVER
			SetSaverWait();
#endif
		}
	}
	
#ifdef OLDCONSOLEHANDLER	/* Yuck */
	if (signals & dialCon_Bit) StuffChar();		

	if (signals & dialIDCMP_Bit) {
		if (message=GetMsg(dialwin->UserPort)) {

			class = message->Class;
			code = message->Code;
			qual = message->Qualifier;
			ReplyMsg(message);
	
			DoDialActivation(class, code, qual);
		}
	}
#endif
		
#ifdef USE_AREXX	
	if (signals & REXX_Bit) 
		if (message=GetMsg(&UcadPort)) 
			ProcessRexxPacket(message);
#endif
	
	return;
}

VOID wait_close() 
{

   	ULONG a;

/*	ON_DISPLAY;
	ON_SPRITE;	*/

	InitView(&mv);
	mv.ViewPort = vp;

/*	AddVSprite(&SpriteStruct, rp);	*/

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

	Rbuttondown=Lbuttondown=FALSE;

	IDCMP_Bit = (1L << w->UserPort->mp_SigBit);

	turn_mouse_on();

	FOREVER Loop_Always();

}

BOOL Catch_MouseClick(inx,iny)
STDFLT *inx,*iny;
{

	ULONG class,signals;
	USHORT code,qual;

	Timer_Bit = (1L << timerport->mp_SigBit);
	IDCMP_Bit = (1L << w->UserPort->mp_SigBit);

	FOREVER {
			
		signals = Wait(IDCMP_Bit|Timer_Bit);


		if (signals & Timer_Bit) {
	
			while (message=GetMsg(timerport)) {
				mx = w->MouseX;
				my = w->MouseY;

				(VOID) ConditionMouse(mx,my,FALSE,FALSE);
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
	
				switch (class) {
				case ACTIVEWINDOW: 
					active=TRUE;
					break;
				case INACTIVEWINDOW:
					active=FALSE;
					break;
				case MOUSEBUTTONS:
					if (!ConditionMouse(mx,my,TRUE,FALSE)) break;
					switch (code) {
					case SELECTDOWN:
						dotx = xreal;
						doty = yreal;
						dotset = TRUE;
						*inx = xreal;
						*iny = yreal;
						return(TRUE);
					case MENUDOWN:
						break;
				}
			}
			break;
			}
		}
	}
		
	return(FALSE);
}

BOOL radialpoint=FALSE;
STDFLT radialx,radialy;
SHORT iradialx=0,iradialy=0;

BOOL ConditionMouse(amx,amy,upd,try)
int amx,amy;
BOOL upd,try;
{

	BOOL rcal;
	STDFLT sx,sy;
	int tx,ty;

	if (FreeHandFB) {
		if (free_pixels<0) free_pixels = 0;
		if (free_pixels>0) {
			amx = (amx/free_pixels) * free_pixels;
			amy = (amy/free_pixels) * free_pixels;
		}
	}	
	
	if (amx==keepx&&amy==keepy) {
		if (upd) UpdateMousePosition(try);
		return(TRUE);
	}

	mx = keepx = amx;
	my = keepy = amy;

	if (w->Flags&MENUSTATE) {
		ClearPointer(w);
		PointerOn = FALSE;
		return(FALSE);
	}

	if (my>=YBegin&&MenusOn) {
		Forbid();
		w->Flags|= RMBTRAP;
		Permit();
		MenusOn=FALSE;
	}
	if (my<YBegin&&!MenusOn) {
		Forbid();
		w->Flags&= ~RMBTRAP;
		Permit();
		MenusOn=TRUE;
		return(FALSE);
	}

	if (FreeHandFB) {
		if (free_pixels<0) free_pixels = 0;
		if (free_pixels>0) {
			mx = (mx/free_pixels) * free_pixels;
			my = (my/free_pixels) * free_pixels;
		}
	}	
									
	if (my<YBegin||
	    my>YBegin+YWidth||
	    mx<XBegin||
	    mx>XBegin+XWidth) {
		if (PointerOn) ClearPointer(w);
		remshadows();
		PointerOn=FALSE;
		return(FALSE);
	} else {
		SetPointer(w, &BlankPointer, 
			1, 1, 0, 0);
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

	rcal = Snaps_On;
	if (FreeHandFB&&free_pixels>0) rcal = FALSE;
	
	if (rcal) {
		xreal = (floor((xreal/xtsnap)+.5) * xtsnap)+xoffset;
		yreal = (floor((yreal/ytsnap)+.5) * ytsnap)+yoffset;
		rcal = TRUE;
	}

	if (Use_Bounds) {
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
		
	}

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

	if (radialpoint&&iradialx==0) {
		sx = radialx - xviewbegin;
		sy = radialy - yviewbegin;
		sx /= xpixsize;
		sy /= ypixsize;
		tx = sx;
		ty = sy;
		iradialx = tx + XBegin;
		iradialy = (YWidth - ty) + YBegin;
	}

	dopointer();

	UpdateMousePosition(try);
	return(TRUE);

}

VOID UpdateMousePosition(upd)
BOOL upd;
{
	WORD kmx,kmy,rpos,fd;
	BOOL vms;
	UBYTE	*absmask="[%s,%s]";
	UBYTE	*tmpmask="{%s,%s}";
	UBYTE	*usemask;

	if (Coord_On||Draw_Info) fd = 10;
	else	fd = 0;

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
	}

	if (upd) {

	   if (Lbuttondown) 	PointerAction(omx,omy,MOVEMENT,xreal,yreal);
	   else			PointerAction(omx,omy,UMOVEMENT,xreal,yreal);
	}
	
	return;

}

VOID remshadows()
{

	int fd;

	SetDrPt(rp, 0xFFFF);
	
	SetDrMd(rp, COMPLEMENT | JAM1);
	SetAPen(rp,2);

	if (ox>=0&&oy>=0) {
		if (Coord_On||Draw_Info) fd = 10;
		else	fd = 0;

		if (orp) {
			Move(rp,ox,oy);
			Draw(rp,orx,ory);
		}
		
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
	ox = mx;
	oy = my;
	orx = iradialx;
	ory = iradialy;
	orp = radialpoint;
	remshadows();
	ox = mx;
	oy = my;
	orx = iradialx;
	ory = iradialy;
	orp = radialpoint;
}

VOID PointerAction(ix,iy,buttonaction,fx,fy)
WORD ix,iy,buttonaction;
STDFLT fx,fy;
{

	if (mouseaction==NULL) return;

	(*mouseaction)(ix,iy,buttonaction,fx,fy);

}

#ifdef OLDOSC
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

	SetRGB4(vp, ClippedColor, 0,0,cstep);
	if (selected_clip) SetRGB4(vp, SelectedColor, cstep,0,cstep);
	else	SetRGB4(vp, SelectedColor, cstep,0,0);

	nclock=rintvl;

	return;

}	
#endif	

turn_mouse_on()
{

	ModifyIDCMP(w,	MOUSEBUTTONS | MENUPICK |
			GADGETUP | RAWKEY | GADGETDOWN | REQCLEAR |
			ACTIVEWINDOW | INACTIVEWINDOW);

}

VOID ActGadget(dogad,doreq)
struct Gadget *dogad;
struct Requester *doreq;
{

	int i;

	for (i=0; i<4; i++) {
	   if (ActivateGadget(dogad,w,doreq)) {
			printf("Text requester activated\n");
			break;
		}
		Delay(2);
	}
}

VOID CloseRequest(req,win)
struct Requester *req;
struct Window *win;
{

	PurgeIDCMP(win->UserPort);
	EndRequest(req, win);
	ClearOutReq = TRUE;
}

sit_n_spin(inreq,atype,escape,affirm)	/* Wait until the last requester clears the screen */
struct Requester *inreq;
int atype;
struct Gadget *escape,*affirm;
{

   	ULONG class,signals;
   	USHORT code;
	struct Gadget *GetGadget,*RatMove=NULL;
	struct Gadget *SaveGadget=NULL,*LastGadget=NULL;

	PurgeIDCMP(w->UserPort);
	
	ClearPointer(w);
	PointerOn=FALSE;
	ClearOutReq=FALSE;
	
	turn_mouse_on();

	Timer_Bit = (1L << timerport->mp_SigBit);
	IDCMP_Bit = (1L << w->UserPort->mp_SigBit);


	FOREVER {
	
		if (ClearOutReq) {
			SaveGadget = NULL;
			RatMove = NULL;
			ClearOutReq=FALSE;
			return;
		}
	
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
						LastGadget = GetGadget;
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
					case 0x4d: /* Down Arrow */
					  if (LastGadget==NULL) 
					  	LastGadget = inreq->ReqGadget;
					  else FOREVER {
					    LastGadget=LastGadget->NextGadget;
					    if (LastGadget==NULL) break;
					    if ((LastGadget->GadgetType&0x0F)==
					    	STRGADGET) break;
					  }					
					  if (LastGadget!=NULL) {
					    if ((LastGadget->GadgetType&0x0F)==
					    	STRGADGET)
						   ActGadget(LastGadget,inreq);
					  }
					break;
					      
					case 0x4c: /* Up arrow */
					break;
					}
				}
			}
		}		
	}
}

#asm
* ----------------------------------------------------------------------
* PurgeIDCMP(iport)
*  register struct MsgPort *iport;
* Replies to all messages from a port, till no more messages pending
* ----------------------------------------------------------------------
MENUVERIFY	equ	$2000
MENUHOT		equ	$1
MENUCANCEL	equ	$2
im_Class	equ	$14
im_Code		equ	$18
	cseg
	public	_LVOGetMsg
	public	_LVOReplyMsg

	public	_PurgeIDCMP
_PurgeIDCMP:
	link	a5,#0
	move.l	a6,-(sp)

; while ( (imsg = (struct IntuiMessage *)GetMsg(iport)) != 0L )
	movea.l	4,a6
purg1:
	movea.l	8(a5),a0
	jsr	_LVOGetMsg(a6)
	tst.l	d0
	 beq.s	purgedone
	movea.l	d0,a1			;a1 = (struct IntuiMessage *)msg
	cmpi.l	#MENUVERIFY,im_Class(a1) ;Right Mouse button?
	 bne.s	1$			;No
	cmpi.w	#MENUHOT,im_Code(a1)	;Is our window active?
	 bne.s	1$			;No
	move.w	#MENUCANCEL,im_Code(a1)	;Otherwise cancel menu operations

1$
	jsr	_LVOReplyMsg(a6)	;ReplyMsg(imsg)
	bra.s	purg1

purgedone:
	move.l	(sp)+,a6
	unlk	a5
	rts
#endasm
