/*----------------------------------------------------------------------*
 *	ucad.h - WURBsoft CAD system project				*
 *----------------------------------------------------------------------*/

/*---------------Various fun includes for the Amiga---------------------*/

#define VERDIS "UltraDesign Paste-Up V1.1(49)"

#include <exec/types.h>
#include <clib/macros.h>
#include <stdio.h>
/* #include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/exec.h> */
#undef max
#undef min
#undef abs
#include <math.h>
#include <intuition/intuition.h>

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

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
#define abs(a) ((a)<0?-(a):(a))

#define PI   3.14159265358979323846
#define PID2 1.57079632679489661923	/* PI divided by 2 */
#define PID4 0.78539816339744830962	/* PI divided by 4 */
#define I_PI 0.31830988618379067154	/* Inverse of PI */
#define I_PID2 0.63661977236758134308	/* Inverse of PID2 */

struct	SpriteImage {
	UWORD	posctl[2];
	UWORD	sprdata[2];
	UWORD	reserved[2];
};

#define STDFLT float		/* For now */
#define LOCAL static
struct	_portfloat {		/* Portable floating point unit */
	WORD	exponent;
	LONG	mantissa;
};

#define PORTFLOAT struct _portfloat

VOID setzero(/* PORTFLOAT * */);
STDFLT PORTtoF(/* PORTFLOAT * */);
VOID FtoPORT(/* STDFLT, PORTFLOAT * */);

#include "plotbase.h"
#include "fonts.h"
#include "drawcontext.h"
#include "attributes.h"
#include "mempools.h"

#define rdrawline(x1,y1,x2,y2)	cdrawline((x1),(y1),(x2),(y2),TRUE)

#define v_move(x,y) lastx = x, lasty = y
#define v_draw(x,y) rdrawline(lastx,lasty,x,y), lastx=x, lasty=y
#define v_drawn(x,y) \
	cdrawline(lastx,lasty,x,y,FALSE), lastx=x, lasty=y

#define TWOPI (2.0*PI)
#define normalize(ang) while(ang<0.0) ang+=TWOPI; while(ang>TWOPI) ang-=TWOPI

#define MM_IN 0.03937			/* Number of inches per mm */

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

extern struct Menu Titles[];

#define MASKMAX 40

#define DEFGLB(a,b) extern a b
#define DEFGLI(a,b,c) extern a b

#include "pglobals.h"

struct mesdef {
	UBYTE	mestype;
	UBYTE	*mesloc;
};

#define INF	0
#define ACT	1
#define WAR	2
#define ERR	3

#include "pcleartext.h"

#define MainMess(mess) SMainMess(&mess)
#define InfoMess(mode,mess) SInfoMess(mode,&mess)
#define EnterSub(mod,out,action) SEnterSub(mod,&out,action)
#define ExitSub(pos) SExitSub(&pos)

BOOL	SEnterSub();

#define UCADDEPTH 3

