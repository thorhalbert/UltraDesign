#include <exec/types.h>
#include <exec/interrupts.h>
#include <graphics/view.h>

#define NO  FALSE
#define YES TRUE
#define LOCAL static

struct ViewPort *vp;

extern UWORD Color_Buff[16][3][2];		/* [idx][color][buf] */
#ifdef SCREEN_SAVER
extern LONG Screen_Counter;
#endif
LOCAL UWORD stcount=0,stloop=0;

#define NMINC 3

VBServ()  
{

#asm
      	movem.l  a2-a7/d2-d7,-(sp)
#endasm

#ifdef SCREEN_SAVER
	Screen_Counter--;	/* Decrement screen counter */
	if (Screen_Counter<0) Screen_Counter = -1;
#endif

	stcount++;

	if (stcount>NMINC) {
		stloop++;
		if (stloop>15) stloop=0;
		stcount = 0;

		SetRGB4(vp, 4, Color_Buff[stloop][0][0],
			       Color_Buff[stloop][1][0],
			       Color_Buff[stloop][2][0]);
		SetRGB4(vp, 5, Color_Buff[stloop][0][1],
			       Color_Buff[stloop][1][1],
			       Color_Buff[stloop][2][1]);
	}


#asm
      	movem.l  (sp)+,a2-a7/d2-d7
#endasm

   	return(0);  /* interrupt routines have to do this */
}


/*
 *  Code to install/remove cycling interrupt handler
 */

LOCAL char srvname[] = "UCVB";  /* Name of interrupt handler */
LOCAL struct Interrupt intServ;

typedef void (*VoidFunc)();

StartInts()
{

   intServ.is_Data = NULL;	/* Large code module uses absolute addresses */

   intServ.is_Code = (VoidFunc)&VBServ;
   intServ.is_Node.ln_Succ = NULL;
   intServ.is_Node.ln_Pred = NULL;
   intServ.is_Node.ln_Type = NT_INTERRUPT;
   intServ.is_Node.ln_Pri  = 0;
   intServ.is_Node.ln_Name = srvname;
   AddIntServer(5,&intServ);
}

StopInts() 
{ 
	RemIntServer(5,&intServ); 
}
