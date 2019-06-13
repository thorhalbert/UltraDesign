/*--------------------------------------------------------------------------*/

#include "iff.h"
#include "gio.h"

/*-----------------------Common Elements-------------------------------*/

#ifdef OLDVER8
/*-----------------Rev 8 of IFF file - pre-production version--------------*/

#define U2VI_8	MakeID('U','2','V','8')		/* View Form Name */
#define U2LB_8	MakeID('U','2','L','8')		/* View Form Name */

#define COPY_8	MakeID('(','c',')',' ')		/* Copyright Declaration */
#define LIMS_8	MakeID('L','I','M','8')		/* Limits Structure */
#define PALE_8	MakeID('P','A','L','8')		/* Pallete definition */

#define ATTR_8	MakeID('A','T','T','8')		/* Attributes FORM */

#define DIME_8	MakeID('D','I','M','8')		/* Define dimensional units */
#define GRPI_8	MakeID('G','R','P','8')		/* Declare a Group */
#define LYER_8	MakeID('L','Y','E','8')		/* Declare a Layer */
#define PENS_8	MakeID('P','E','N','8')		/* Declare a Pen Style */
#define TXST_8	MakeID('T','X','S','8')		/* Text style structure */

#define OBJE_8	MakeID('O','B','J','8')		/* Objects FORM */

#define VECT_8	MakeID('V','E','C','8')		/* Vector Structure */
#define CIRC_8	MakeID('C','I','R','8')		/* Ellipse Structure */
#define TEXT_8	MakeID('T','E','X','8')		/* Text Structure */

struct iff_lims_8 {			/* Limits structure */

	PORTFLOAT	minx,miny;	/* Min X and Y view sizes */
	PORTFLOAT	maxx,maxy;	/* Max X and Y sizes */
	PORTFLOAT	xsnap,ysnap;	/* Axis snap to figures */
	PORTFLOAT	xoffs,yoffs;	/* Offset to even snap multiples */
	PORTFLOAT	xgrid,ygrid;	/* Whole unit screen grid divisions */
	PORTFLOAT	xgoff,ygoff;	/* Grid offsets */
	WORD		xgdiv,ygdiv;	/* Grid subunit divisors */
	PORTFLOAT	scalefactor;	/* view * scalef = paper scale */
	WORD		qtextlim;	/* Qtext limit in pixels */
	PORTFLOAT	paperx,papery;	/* view origin on paper origin */
	PORTFLOAT	paperrotation;	/* rotation of view origin on paper */
	UBYTE		paperflips;	/* flip flags on paper */
#define		LIM8FLIP_X 0x0001	/* Mirror flip X axis */
#define		LIM8FLIP_Y 0x0002	/* Mirror flip Y axis */
	UWORD		viewflags;	/* Flags and defaults */
#define		LIM8FLAG_CCEN 0x0001	/* Center on? */
#define		LIM8FLAG_HAND 0x0002	/* Handles on? */
#define		LIM8FLAG_COOR 0x0004	/* Coordinates on? */
#define		LIM8FLAG_SNAP 0x0008	/* Use local snaps? */
#define		LIM8FLAG_GRID 0x0010	/* Turn the grids on? */
#define		LIM8FLAG_BOUN 0x0020	/* Constrain to max/min? */
#define		LIM8FLAG_ORTH 0x0040	/* Ortho switch enabled */
#define		LIM8FLAG_UNSE 0x8000	/* Bits were unset for some reason */
#define		LIM8DEFAULTS LIM8FLAG_CCEN|LIM8FLAG_COOR|LIM8FLAG_SNAP| \
			LIM8FLAG_BOUN
	UBYTE		takecolors;	/* Where to take colors from */
#define		LIM8COLO_GRUP 1		/* Take colors from the group */
#define		LIM8COLO_LYER 2		/* Take colors from the layer */
#define		LIM8COLO_PEN  3		/* Take colors from the pen */
	UWORD		current_group;	/* Current group pointer */
	UWORD		current_layer;	/* Current layer */
	UWORD		current_pen;	/* Current pen style */
	UWORD		current_style;	/* Style form now in effect */
	UWORD		template_pen;	/* Template pen name */
	UWORD		template_style;	/* Template style */

	UWORD		max_group;	/* Number of group blocks */
	UWORD		max_layer;	/* Number of layer blocks */
	UWORD		max_style;	/* Number of style blocks */
	UWORD		max_pen;	/* Number of pen blocks */
	UWORD		max_dime;	/* Number of dimension blocks */

};

#define MAXCOLOR 16			/* Total number of colors */

struct iff_pale_8 {			/* Pallete definitions */

	UWORD		NumUcad;	/* Number of reserved colors neg cols */
	UWORD		NumUser;	/* Number of normal user colors */
	struct _pale8_sets {
		UWORD	RedMix;		/* 16 bits of red */
		UWORD	BlueMix;	/* 16 bits of blue */
		UWORD	GreenMix;	/* 16 bits of green */
	} colors[MAXCOLOR];		/* Number of elements - trunc at will */
};

struct iff_grpi_8 {			/* Group definition */

	UWORD		GroupID;	/* Number of this group node */
	UBYTE		GroupName[32];	/* Name of the group */

	WORD		Color_ID;	/* Color register number */
	UWORD		Group_Flags;	/* Group information flags */
#define		GRPI8FLG_LOCK 0x0001	/* Group is locked out */
#define		GRPI8FLG_GHOS 0x0002	/* Group is display ghosted */
#define		GRPI8FLG_DOFF 0x0004	/* Group display is off */
#define 	GRPI8FLG_POFF 0x0008	/* Group ploting is off */
	UWORD		Libr_Flags;	/* Library mode flags */
#define		GRPI8LIB_ASSY 0x0001	/* This begins an assembly */
	UWORD		Parent;		/* Number of parent (0 is root) */
	UWORD		Next;		/* Number of next group on same level */
	UWORD		Previous;	/* Backwards link to prev group */
	UWORD		Descendants;	/* Pointer to root of lower level */
	UWORD		Dimensioning;	/* Pointer to dimensioning, 0 use def */

	UWORD		NumOrgs;	/* Number of handles or org points */
	PORTFLOAT	OrgPoints[8];	/* Pairs of handle points */

};

struct iff_lyer_8 {			/* Layer Definition */

	UWORD		LayerID;	/* Number of this layer */
	UBYTE		LayerName[32];	/* Name of the layer */

	WORD		Color_ID;	/* Color id for the layer */
	UWORD		Layer_Flags;	/* Layer information flags */
#define		LYER8FLG_LOCK 0x0001	/* This layer is locked */
#define		LYER8FLG_GHOS 0x0002	/* This layer is ghosted */
#define		LYER8FLG_DOFF 0x0004	/* This layer is not displayed */
#define		LYER8FLG_POFF 0x0008	/* This layer is not plotted */
#define		LYER8FLG_DEF  0x0010	/* This is the default layer */

};

struct iff_pens_8 {			/* Pen style definition */

	UWORD		PenID;		/* Number of the Pen style */
	UBYTE		PenName[32];	/* Name of the pen */

	WORD		Color_ID;	/* Color reg for the pen */
	UWORD		Pen_Flags;	/* Pen information for the flag */
#define		PENS8FLG_LOCK 0x0001	/* This pen style is locked */
#define		PENS8FLG_GHOS 0x0002	/* The pen is ghosted */
#define		PENS8FLG_DOFF 0x0004	/* The pen display is off */
#define		PENS8FLG_POFF 0x0008	/* The pen is not plotted */
#define		PENS8FLG_DEF  0x0010	/* This is the default pen */
	PORTFLOAT	PenWidth;	/* Width of the pen in mms */
	UWORD		OverStroke;	/* Weight of pen */

	UBYTE		Numbits;	/* Number of bits in pattern */
	ULONG		Pattern1;	/* Pattern 1 */
	ULONG		Pattern2;	/* Pattern 2 */

	PORTFLOAT	Bit_Size;	/* Width of each bit in mms */

};

struct iff_txst_8 {			/* Lettering style definition */

	UWORD		LetterID;	/* Lettering ID number */
	UBYTE		StyleName[32];	/* Lettering style name */
	UBYTE		FontName[32];	/* Stroke Font name */

	PORTFLOAT	Prime_Height;	/* Height of stroke font paper units */
	PORTFLOAT	Slant_Angle;	/* -45 to 45 degree slant */
	PORTFLOAT	Aspect;		/* Height over width ratio */

	UBYTE		TextFlags;	/* Standard flags */
#define		TXST8FLG_DEF 0x0001	/* This is the default style */
};

#define VECTORMAX 128			/* Maximum number of vector bundles */

struct iff_vect_8 {			/* Vector storage information */

	UWORD		NumberVects;	/* Total number of vectors */

	struct _v8bundle {
		UBYTE 	VecFlags;	/* Flags for this vector pair */
#define		VECT8FLG_P1AR 0x0001	/* Arrowhead facing p1 point */
#define		VECT8FLG_P2AR 0x0002	/* Arrowhead facing p2 point */
		UWORD	Group_ID;	/* Group pair is in */
		UWORD	Layer_ID;	/* Layer pair is in */
		UWORD	Pen_ID;		/* Pen to draw vector with */
		PORTFLOAT	p1x,p1y; /* From point */
		PORTFLOAT	p2x,p2y; /* To point */
	} vectbundles[VECTORMAX+4];	/* Total number of bundles allowed */

};

#define CIRCLEMAX 64			/* Number allowed in a bundle */

struct iff_circ_8 {			/* Circle/Ellipse storage */

	UWORD		NumberCircs;	/* Total number of ellipses */

	struct _c8bundle {
		UBYTE	CircFlags;	/* Flags for circle status */
#define		CIRC8FLG_ELLIP 0x0001	/* Has two radii */
#define		CIRC8FLG_LIMTS 0x0002	/* Has beginning and ending points */
#define		CIRC8FLG_ROTA  0x0004	/* Rotated off x, y axis */
		UWORD	Group_ID;	/* Group data is in */
		UWORD	Layer_ID;	/* Layer data is in */
		UWORD	Pen_ID;		/* Pen to draw circle with */
		PORTFLOAT centx,centy,
			radiusx,radiusy,
			starta,enda,
			rotation;	/* Essential data */
		} circbundles[CIRCLEMAX+4];

};

struct iff_text_8 {			/* Text structure storage */

	UBYTE		TextFlags;	/* Text flagging */
#define		TEXT8FLG_XFLP 0x0001	/* Flip about x axis */
#define		TEXT8FLG_YFLP 0x0002	/* Flip about y axis */
#define		TEXT8FLG_CFLP 0x0004	/* Flip chars individually */
	UBYTE		Handle_Pos;	/* Define handle position */
#define		TEXT8HND_LEFT 1		/* Handle is left bottom */
#define		TEXT8HND_CENT 2		/* Handle is in center */
#define		TEXT8HND_RIGH 3		/* Handle right bottom corner */
	PORTFLOAT	Xpos,Ypos;	/* Placement of handle */
	PORTFLOAT	Rotation;	/* Rotation about handle point */
	UWORD		Letter_ID;	/* What lettering style is this? */
	UWORD		Group_ID;	/* Group data is in */
	UWORD		Layer_ID;	/* Layer data is in */
	UWORD		Pen_ID;		/* Pen to draw text with */

	UWORD		textlen;		/* Length of text minus null */

	UBYTE		TextData[257];	/* Data with trailing null */	
};
#endif
