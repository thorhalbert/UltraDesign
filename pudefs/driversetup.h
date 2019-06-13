/*----------------------------------------------------------------------*
 *	ucad.h - WURBsoft CAD system project				*
 *----------------------------------------------------------------------*/

/*---------------Various fun includes for the Amiga---------------------*/

#include <exec/types.h>
#include <clib/macros.h>
#include <stdio.h>
#undef max
#undef min
#undef abs
#include <math.h>
#include <intuition/intuition.h>

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
#define abs(a) ((a)<0?-(a):(a))

#define PI   3.14159265358979323846
#define PID2 1.57079632679489661923	/* PI divided by 2 */
#define PID4 0.78539816339744830962	/* PI divided by 4 */
#define I_PI 0.31830988618379067154	/* Inverse of PI */
#define I_PID2 0.63661977236758134308	/* Inverse of PID2 */

#define STDFLT float		/* For now */
#define LOCAL static

#include "plotbase.h"

#define TWOPI (2.0*PI)
#define normalize(ang) while(ang<0.0) ang+=TWOPI; while(ang>TWOPI) ang-=TWOPI

#define MM_IN 0.03937			/* Number of inches per mm */
#define IN_MM 25.4			/* Number mm per inch */
