/*----------------------------------------------------------------------*
 *	ucad.h - WURBsoft CAD system project				*
 *----------------------------------------------------------------------*/

/* #define DEMO */	/* Disable Saves When Set */
/* #define DOFFP */	/* Use fast ffp for certain key routines */

/*---------------Various fun includes for the Amiga---------------------*/

#include <exec/types.h>
#include <clib/macros.h>
#include <stdio.h>
/*#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/exec.h>*/
#include <graphics/gfxmacros.h>
#undef max
#undef min
#undef abs
#include <math.h>
#include <libraries/dos.h>

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
#define abs(a) ((a)<0?-(a):(a))

#define streq(a,b) (strcmp(a,b)==0)

#define printf Dprintf

#ifdef F_FFP
#define MAXBIG 1e18
#endif

#ifdef F_IEEE
#define MAXBIG 1e31
#endif

#ifdef F_881
#define MAXBIG 1e31
#endif

#define PI   3.14159265358979323846
#define PID2 1.57079632679489661923	/* PI divided by 2 */
#define PID4 0.78539816339744830962	/* PI divided by 4 */
#define I_PI 0.31830988618379067154	/* Inverse of PI */
#define I_PID2 0.63661977236758134308	/* Inverse of PID2 */

#include "versions.h"

#define FAST register
#define LOCAL static
#define STDFLT float		/* For now */

struct	_portfloat {		/* Portable floating point unit */
	WORD	exponent;
	LONG	mantissa;
};

#define PORTFLOAT struct _portfloat

VOID setzero(/* PORTFLOAT **/);
STDFLT PORTtoF(/*PORTFLOAT **/);
VOID FtoPORT(/*STDFLT, PORTFLOAT **/);
VOID *AllocRemember();

#include "amigahooks.h"
#include "attributes.h"
#include "mempools.h"

/*----------------Various project definitions---------------------------*/

#define TOPMARGIN 10		/* Area where screen starts after window bar */
#define HORIZBAR 7		/* Height of the horizontal proportional bar */
#define VERTBAR 15		/* Width of the vertical prop bar */
#define BOTTOMAREA 0		/* Size reserved for the gadget area */
#define LEFTAREA 38		/* Protocol bar moved to left side */

#define V_GADGET 21		/* Name of the vertical prop gadget */
#define H_GADGET 22		/* Name of the horizontal prop gadget */


/*-----------------Codes for resources we have opened-------------------*/

#define F_INTUITION 0x000001
#define F_GRAPHICS  0x000002
#define F_MATH      0x000004
#define F_MATHTRANS 0x000008
#define F_CONSOLE   0x000010
#define F_COLORTAB  0x000020
#define F_SETSTORE  0x000040
#define F_DISKFONT  0x000080

/*---------------------Mouse movement information--------------------------*/

#define MOVEMENT 1		/* User Moved the Mouse with button down */
#define LBUTTONDOWN 2		/* User depressed left button */
#define LBUTTONUP 3		/* User released left button */
#define UMOVEMENT 4		/* User moved mouse with button up */
#define RBUTTONDOWN 5		/* User right button down */
#define RBUTTONUP 6		/* User right button was released */

/*---------------------Virtual window clipper results----------------------*/

#define WIND_OFF 0	/* Line was not in window */
#define WIND_IN 1	/* Line was all in window */
#define WIND_CLIP 3	/* Line fit in window after being clipped */

/*---------------------Internal structures--------------------------*/

struct	SpriteImage {
	UWORD	posctl[2];
	UWORD	sprdata[2];
	UWORD	reserved[2];
};

/*----------------Define global areas----------------------------*/

extern struct drawing_node *Root_Node,*Last_Node;

extern WORD CurrentDrawMode;
extern VOID (*mouseaction)();
extern VOID (*actionsave)();
extern VOID (*subaction)();
extern VOID (*WrapupDrawMode)();
extern VOID (*CleanupShadows)();

#define	xviewend (xviewbegin+xviewsize)
#define	yviewend (yviewbegin+yviewsize)

extern BOOL rolldir;

extern struct Menu Titles[];

extern struct	SpriteImage	BlankPointer;

/*------------------Define global subroutines for posterity---------------*/

BOOL	ConditionMouse();		/* In pointers.c */
VOID	UpdateMousePosition();		/* In pointers.c */

VOID	PerformLine(/*WORD,WORD,WORD,STDFLT,STDFLT*/);
VOID	GoLineDrawMode();
BOOL	SelectLegal(/*struct drawing_node *,BOOL,BOOL,BOOL*/);

/*------------------File I/O variables----------------------------------*/

#define colormask(x) (x & ((1<<Sdepth)-1))
#define inversecolor(x) colormask(~(x))

#define rdrawline(x1,y1,x2,y2)	cdrawline((x1),(y1),(x2),(y2),TRUE)

#define v_move(x,y) (lastx = (x), lasty = (y))
#define v_draw(x,y) rdrawline(lastx,lasty,(x),(y)), (lastx=(x),lasty=(y))
#define v_drawn(x,y) \
	cdrawline(lastx,lasty,(x),(y),FALSE), (lastx=(x),lasty=(y))

#define TWOPI (2.0*PI)
#define normalize(ang) while(ang<0.0) ang+=TWOPI; while(ang>TWOPI) ang-=TWOPI

#define ANG_DEGD 	1
#define ANG_DEGM 	2
#define ANG_DEGMS 	3
#define ANG_rad		4
#define ANG_Grad	5

#define LIN_EDI		1
#define LIN_EFI		2
#define LIN_DF		3
#define LIN_DI		4
#define LIN_MM		5
#define LIN_CM		6
#define LIN_M		7
#define LIN_MILE	8
#define LIN_KM		9

#define MM_IN 0.03937			/* Number of inches per mm */

struct mesdef {
	UBYTE	mestype;
	char	*mesloc;
};

#define INF	0
#define ACT	1
#define WAR	2
#define ERR	3

#include "cleartext.h"

#define MainMess(mess) SMainMess(&mess)
#define InfoMess(mode,mess) SInfoMess(mode,&mess)
#define EnterSub(mod,out,action) SEnterSub(mod,&out,action)
#define ExitSub(pos) SExitSub(&pos)
#define EnterSide(mod,out,action) SEnterSide(mod,&out,action)
#define ExitSide(pos) SExitSide(&pos)

BOOL	SEnterSub();

STDFLT	ConvEllipNotation(/* STDFLT,STDFLT,STDFLT */);
STDFLT 	hypot(/* STDFLT,STDFLT */);

UBYTE	*DispLin(/* STDFLT */), *DispAng(/* STDFLT */);
STDFLT	ParAng(/* UBYTE *, BOOL * */);
STDFLT	ParLin(/* UBYTE *, BOOL * */);
STDFLT	caltsnap();
UBYTE	*StoreComment(/* UBYTE * */);

#define MASKMAX 40
#define PATHMAX 256

#define DEFGLB(a,b) extern a b
#define DEFGLI(a,b,c) extern a b

#define UCADDEPTH 4

#include "globals.h"

