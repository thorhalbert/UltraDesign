/*
 *
 *	The following definations allow for the efficient 
 *	translation of DXY and RD-GL codes to PostScript code
 *
 */

#include <stdio.h>
#include <math.h>
#include <ctype.h>

#define CR	'\015'
#define LF	'\012'
#define SPACE	'\040'

#define rint(a) ((int) (a))

#define SIGNED_NUMERIC  (ungetc(( ch = getc(stream)),stream) != EOF ) &&\
		(((ch>='0') && (ch<='9')) || (ch=='-') || (ch=='+')\
		|| (ch==' ') || (ch==',') || (ch == '.'))

#define CIRCLE	21		/* DXY Circle */
#define RCIRCLE 22		/* DXY Relative Circle */
#define CCIRCLE 23		/* DXY Centered Circle */
#define ACIRCLE 24		/* DXY Arc plus Circle */
#define SCIRCLE 25		/* DXY Segment Circle */
#define RDGLCIRCLE 26		/* RD-GL Circle */

#define TEXT	31
#define MARK	32

#define LINETYPE	41
#define LINESCALE	42
#define LINE_TYPE_SCALE	43
/*
 *	Files to open if any
 */
FILE	*stream;
FILE	*fopen();
/*
 *	Plotting Parameters that will contain the necessary PostScript
 *	commands to plot (see dxy2ps.c for the initialisation) and
 *	ps_macros.c for the plotting macros).
 */
#define MOVE 1
#define RMOVE 2
#define DRAW 3
#define RDRAW 4
/*
 *	Definition of "ch" used in SIGNED_NUMERIC
 */
char	ch;
/*
 *	Define the function getval() which returns a real number.
 */
float	getval();
/*
 *	Scaling parameters used for translation from DXY and RD-GL
 *	coordinate sytem to the PostScript coordinate system which
 *	has been defined in millimeters. (See above)
 */
float	SCALE;
float	XSCALE;
float	YSCALE;
float	xmax, xmin;
float	ymax, ymin;
/*
 *	End of line terminator (RD-GL / HP-GL)
 */
char	EOL;
/*
 *	PostScript Coordinate parameters
 */
float	lastXmove;
float	lastYmove;
float	absX;
float	absY;
float	xval;
float	yval;
float	xoffset, yoffset;
/*
 *	Extra parameters
 */
float	char_angle;
float	char_height;
float	char_width;
float	char_space;
float	char_slant;

char	*font;

char	symbol;

int	dcount;
/*
 *	Degree radian conversion parameter ie: deg_rad = asin(1) / 90.0;
 *	( Defined in dxy2ps.c or rdgl2ps.c )
 */
float	deg_rad;
/*
 *	Line / pen size parameter (max 9 sizes)
 */
float	pen_size[9];
/*
 *	Paper size (ie A3 or A4) and Mode (HPGL or DXY)
 */
char	*PaperSize;
char	*Mode;
/*
 *	Flags
 */
int	LANDSCAPE;
int	DRAW_FLAG;
int	PLOTABS;
int	PENDOWN;
int	SETDOT; 	/* HP-GL commands only */
int	SYMBOL;		/* HP-GL commands only */
