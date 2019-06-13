/*--------------------------------------------------------------------------*/

#include "iff.h"
#include "gio.h"

/*-----------------------Common Elements-------------------------------*/

/*-----------------Rev 0 of IFF file - Final Production Version---------------*/

#define U2VI_0	MakeID('2','D','O','F')		/* View Form Name */

#define COPY_0	MakeID('(','c',')',' ')		/* Copyright Declaration */
#define LIMS_0	MakeID('L','I','M','S')		/* Limits Structure */
#define PALE_0	MakeID('P','A','L','E')		/* Pallete definition */
#define DIME_0	MakeID('D','I','M','E')		/* Define dimension constants */
#define BMEM_0	MakeID('B','M','E','M')		/* Boundary memory */

#define ATTR_0	MakeID('A','T','T','R')		/* Attributes FORM */

#define GRPI_0	MakeID('G','R','P','I')		/* Declare a Group */
#define LYER_0	MakeID('L','Y','E','R')		/* Declare a Layer */
#define PENS_0	MakeID('P','E','N','S')		/* Declare a Pen Style */
#define TXST_0	MakeID('T','X','S','T')		/* Text style structure */
#define PATT_0	MakeID('P','A','T','T')		/* Hatch pattern */

#define OBJE_0	MakeID('O','B','J','E')		/* Objects FORM */

#define VECT_0	MakeID('V','E','C','T')		/* Vector Structure */
#define CIRC_0	MakeID('C','I','R','C')		/* Ellipse Structure */
#define TEXT_0	MakeID('T','E','X','T')		/* Text Structure */
#define HATC_0	MakeID('H','A','T','C')		/* Hatching Structure */

#define LIMS0_REV 0

struct iff_lims_0 {			/* Limits structure */

/****/	UWORD		defrev;		/* Revision of structure */

/*00*/	UWORD		viewflags;	/* Flags and defaults */
#define		LIM0FLAG_CCEN 0x0001	/* Center on? */
#define		LIM0FLAG_HAND 0x0002	/* Handles on? */
#define		LIM0FLAG_COOR 0x0004	/* Coordinates on? */
#define		LIM0FLAG_SNAP 0x0008	/* Use local snaps? */
#define		LIM0FLAG_GRID 0x0010	/* Turn the grids on? */
#define		LIM0FLAG_BOUN 0x0020	/* Constrain to max/min? */
#define		LIM0FLAG_ORTH 0x0040	/* Ortho switch enabled */
#define		LIM0FLAG_QHAT 0X0080	/* Quick hatching enabled */
/*00*/	UBYTE		takecolors;	/* Where to take colors from */
#define		LIM0COLO_GRUP 1		/* Take colors from the group */
#define		LIM0COLO_LYER 2		/* Take colors from the layer */
#define		LIM0COLO_PEN  3		/* Take colors from the pen */

/*00*/	UWORD		current_group;	/* Current group pointer */
/*00*/	UWORD		current_layer;	/* Current layer */
/*00*/	UWORD		current_pen;	/* Current pen style */
/*00*/	UWORD		current_style;	/* Style form now in effect */
/*00*/	UWORD		template_pen;	/* Template pen name */
/*00*/	UWORD		template_style;	/* Template style */

/*00*/	UWORD		max_group;	/* Number of group blocks */
/*00*/	UWORD		max_layer;	/* Number of layer blocks */
/*00*/	UWORD		max_style;	/* Number of style blocks */
/*00*/	UWORD		max_pen;	/* Number of pen blocks */

/*00*/	PORTFLOAT	xsnap,ysnap;	/* Axis snap to figures */
/*00*/	PORTFLOAT	xoffs,yoffs;	/* Offset to even snap multiples */
/*00*/	PORTFLOAT	snaprot;	/* Snap grid plane rotation in rads */
/*00*/	PORTFLOAT	xgrid,ygrid;	/* Whole unit screen grid divisions */
/*00*/	PORTFLOAT	xgoff,ygoff;	/* Grid offsets */
/*00*/	WORD		xgdiv,ygdiv;	/* Grid subunit divisors */
/*00*/	PORTFLOAT	gridrot;	/* Rotation of grid plane */
/*00*/	PORTFLOAT	minx,miny;	/* Min X and Y view sizes */
/*00*/	PORTFLOAT	maxx,maxy;	/* Max X and Y sizes */
/*00*/	WORD		qtextlim;	/* Qtext limit in pixels */
/*00*/ 	PORTFLOAT	paperunit;	/* Size of paper unit */
/*00*/ 	PORTFLOAT	viewunit;	/* Size of paper unit */
/*00*/	PORTFLOAT	viewscale;	/* paperunit/viewunit scale fact */
/*00*/	PORTFLOAT	arrowangle;	/* Angle of arrowhead */
/*00*/	PORTFLOAT	arrowlen;	/* Length of arrowhead */
/*00*/	PORTFLOAT	orthoang;	/* Orthogonal Angle */
/*00*/	PORTFLOAT	filletrad;	/* Default fillet radius */
/*00*/	PORTFLOAT	lintolerance;	/* Linear resolution (mm) */
/*00*/	WORD		linpoints;	/* Linear decimal points */
/*00*/	WORD		lintype;	/* Linear units enumeration */
/*00*/	PORTFLOAT	angtolerance;	/* Angular tolerance */
/*00*/	WORD		angpoints;	/* Angular decimal points */
/*00*/	WORD		angtype;	/* Angular unit type */

};

#define MAXCOLOR 32			/* Total number of colors */
#define MAXPRESETS 20			/* Total number preset color types */
#define PALE0_REV 0

struct iff_pale_0 {			/* Pallete definitions */

/****/	UWORD		defrev;		/* Revision of structure */

/*00*/	UWORD		NumUcad;	/* Number of reserved colors neg cols */
/*00*/	UWORD		NumSpec;	/* Number of colors reserved for special */
/*00*/	UWORD		NumTot;		/* Grand total number of colors */
/*00*/	WORD		Presets[MAXPRESETS]; /* Preset colors, <0 not set */
#define 	PALE0_GRID 0		/* Grid Color */
#define		PALE0_MIST 1		/* Background pattern color */
#define		PALE0_BNDS 2		/* View boundary color */
#define		PALE0_GHST 3		/* Ghosted Object Color */
#define		PALE0_COOR 4		/* Coordinate display color */
#define		PALE0_DRAW 5		/* Drawing info display color */
#define		PALE0_ERAS 6		/* Screen background color (erase) */
/*00*/	struct _pale0_sets {
/*00*/		UWORD	RedMix;		/* 16 bits of red */
/*00*/		UWORD	BlueMix;	/* 16 bits of blue */
/*00*/		UWORD	GreenMix;	/* 16 bits of green */
/*00*/	} colors[MAXCOLOR];		/* Number of elements - trunc at will */

};

#define DIME0_REV 0

struct iff_dime_0 {			/* Dimensioning defaults */

/****/	UWORD		defrev;		/* Revision of structure */

/*00*/	ULONG		DimFlags;	/* Yes/No answers */
#define 	DIM0FLAG_FIXED 0x0001	/* Angles are fixed, otherwise free */

/*00*/	UWORD		WitLayer;	/* Witness layer */
/*00*/	UWORD		WitGroup;	/* Witness group */
/*00*/	UWORD		WitPen;		/* Witness pen style */

/*00*/	UWORD		DimLinLayer;	/* Dim Line Layer */
/*00*/	UWORD		DimLinGroup;	/* Dimension Line Group */
/*00*/	UWORD		DimLinPen;	/* Dim line pen style */

/*00*/	UWORD		DimBoxStyle;	/* Text style of dim box */
/*00*/	UWORD		DimBoxLayer;	/* Dim box layer */
/*00*/	UWORD		DimBoxGroup;	/* Dim box group */
/*00*/	UWORD		DimBoxPen;	/* Dim Box Pen */

/*00*/	PORTFLOAT	WitOffset;	/* Witness offset */
/*00*/	PORTFLOAT	WitOver;	/* Witness overshoot */

/*00*/	PORTFLOAT	DimLinOffset;	/* Dim line offset */
/*00*/	PORTFLOAT	DimBoxOffset;	/* Dim box offset */

/*00*/	PORTFLOAT	DimAngle;	/* Angle if fixed */
/*00*/	PORTFLOAT	FromFlip;	/* From autoflip */
/*00*/	PORTFLOAT	ToFlip;		/* To autoflip */
/*00*/	PORTFLOAT	FreeOffset;	/* Add to free angles */

};

#define BMEM0_REV 0

struct iff_bmem_0 {			/* Boundary Settings */

/****/	UWORD		defrev;		/* Revision of structure */

/*00*/	UWORD		memflags;	/* Boundary flags */
#define BMEM0_KEYPAD 0x0001		/* This is a keypad number */

/*00*/	UWORD		keyid;		/* Id of the key */

/*00*/	PORTFLOAT	xbeg,ybeg;	/* View Beginings */
/*00*/	PORTFLOAT	xsiz,ysiz;	/* Size of the view */
/*00*/	PORTFLOAT	viewrot;	/* Rotation of offset (rads) */
};

#define GRPI0_REV 0

struct iff_grpi_0 {			/* Group definition */

/****/	UWORD		defrev;		/* Revision of structure */

/*00*/	UWORD		GroupID;	/* Number of this group node */
/*00*/	UBYTE		GroupName[32];	/* Name of the group */

/*00*/	WORD		Color_ID;	/* Color register number */
/*00*/	UWORD		Group_Flags;	/* Group information flags */
#define		GRPI0FLG_LOCK 0x0001	/* Group is locked out */
#define		GRPI0FLG_GHOS 0x0002	/* Group is display ghosted */
#define		GRPI0FLG_DOFF 0x0004	/* Group display is off */
#define 	GRPI0FLG_POFF 0x0008	/* Group ploting is off */
/*00*/	UWORD		Libr_Flags;	/* Library mode flags */
#define		GRPI0LIB_ASSY 0x0001	/* This begins an assembly */
/*00*/	UWORD		Parent;		/* Number of parent (0 is root) */
/*00*/	UWORD		Next;		/* Number of next group on same level */
/*00*/	UWORD		Previous;	/* Backwards link to prev group */
/*00*/	UWORD		Descendants;	/* Pointer to root of lower level */

	UBYTE		Comment[257];	/* Data with trailing null */	

};

#define LYER0_REV 0

struct iff_lyer_0 {			/* Layer Definition */

/****/	UWORD		defrev;		/* Revision of structure */

/*00*/	UWORD		LayerID;	/* Number of this layer */
/*00*/	UBYTE		LayerName[32];	/* Name of the layer */

/*00*/	WORD		Color_ID;	/* Color id for the layer */
/*00*/	UWORD		Layer_Flags;	/* Layer information flags */
#define		LYER0FLG_LOCK 0x0001	/* This layer is locked */
#define		LYER0FLG_GHOS 0x0002	/* This layer is ghosted */
#define		LYER0FLG_DOFF 0x0004	/* This layer is not displayed */
#define		LYER0FLG_POFF 0x0008	/* This layer is not plotted */
#define		LYER0FLG_DEF  0x0010	/* This is the default layer */

/*00*/	UBYTE		Comment[257];	/* Data with trailing null */	

};

#define PENS0_REV0 0

struct iff_pens_0_0 {			/* Pen style definition */

/****/	UWORD		defrev;		/* Revision of structure */

/*00*/	UWORD		PenID;		/* Number of the Pen style */
/*00*/	UBYTE		PenName[32];	/* Name of the pen */

/*00*/	WORD		Color_ID;	/* Color reg for the pen */
/*00*/	UWORD		Pen_Flags;	/* Pen information for the flag */
#define		PENS0FLG_LOCK 0x0001	/* This pen style is locked */
#define		PENS0FLG_GHOS 0x0002	/* The pen is ghosted */
#define		PENS0FLG_DOFF 0x0004	/* The pen display is off */
#define		PENS0FLG_POFF 0x0008	/* The pen is not plotted */
#define		PENS0FLG_DEF  0x0010	/* This is the default pen */

/*00*/	UBYTE		Numbits;	/* Number of bits in pattern */
/*00*/	ULONG		Pattern1;	/* Pattern 1 */
/*00*/	ULONG		Pattern2;	/* Pattern 2 */

/*00*/	UBYTE		Comment[257];	/* Data with trailing null */	

};

#define PENS0_REV1 10

struct iff_pens_0_1 {			/* Pen style definition */

/****/	UWORD		defrev;		/* Revision of structure */

/*00*/	UWORD		PenID;		/* Number of the Pen style */
/*00*/	UBYTE		PenName[32];	/* Name of the pen */

/*00*/	WORD		Color_ID;	/* Color reg for the pen */
/*00*/	UWORD		Pen_Flags;	/* Pen information for the flag */
#define		PENS0FLG_LOCK 0x0001	/* This pen style is locked */
#define		PENS0FLG_GHOS 0x0002	/* The pen is ghosted */
#define		PENS0FLG_DOFF 0x0004	/* The pen display is off */
#define		PENS0FLG_POFF 0x0008	/* The pen is not plotted */
#define		PENS0FLG_DEF  0x0010	/* This is the default pen */

/*10*/	PORTFLOAT	PenWidth;	/* Width of logical pen */

/*00*/	UBYTE		Numbits;	/* Number of bits in pattern */
/*00*/	ULONG		Pattern1;	/* Pattern 1 */

/*00*/	ULONG		HardColor;	/* Color of pen for hardcopy */

/*10*/	PORTFLOAT	BitWidth;	/* Width of each bit */

/*10*/	UWORD		PenOrd;		/* Physical Pen Order */
/*10*/	UWORD		Carousel;	/* Carousel number */
/*10*/	UWORD		AutoNum;	/* Auto pen number */

/*00*/	UBYTE		Comment[257];	/* Data with trailing null */	

};

#define TXST0_REV 0

struct iff_txst_0 {			/* Lettering style definition */

/****/	UWORD		defrev;		/* Revision of structure */

/*00*/	UWORD		LetterID;	/* Lettering ID number */
/*00*/	UBYTE		StyleName[32];	/* Lettering style name */
/*00*/	UBYTE		FontName[32];	/* Stroke Font name */

/*00*/	PORTFLOAT	Paper_Height;	/* Height of stroke in paper units */
/*00*/	PORTFLOAT	Prime_Height;	/* Height of stroke font view units */
/*00*/	PORTFLOAT	Slant_Angle;	/* -45 to 45 degree slant */
/*00*/	PORTFLOAT	Aspect;		/* Height over width ratio */

/*00*/	UBYTE		TextFlags;	/* Standard flags */
#define		TXST0FLG_DEF 0x0001	/* This is the default style */

/*00*/	UBYTE		Comment[257];	/* Data with trailing null */	

};

#define PATT0_REV 0

struct iff_patt_0 {			/* Hatch Pattern definition */

/****/	UWORD		defrev;		/* Revision of structure */

/*00*/	UWORD		PatternID;	/* Lettering ID number */
/*00*/	UBYTE		File_Name[40];	/* The hatch file name */
/*00*/	UBYTE		Hatch_Name[40];	/* The pattern name */
/*00*/	UBYTE		Pattern_Type;	/* What kind of pattern is this */
#define 	PATT0FLG_SIMPAT	1	/* Simple pattern type */

/*00*/	UWORD		Patt_Flags;	/* Flags */
};

#define VECTORMAX 128			/* Maximum number of vector bundles */

#define VECT0_REV 0

struct iff_vect_0 {			/* Vector storage information */

/****/	UWORD		defrev;		/* Revision of structure */

/*00*/	UWORD		NumberVects;	/* Total number of vectors */

/*00*/	struct _v0bundle {
/*00*/		UBYTE 	VecFlags;	/* Flags for this vector pair */
#define		VECT0FLG_P1AR 0x0001	/* Arrowhead facing p1 point */
#define		VECT0FLG_P2AR 0x0002	/* Arrowhead facing p2 point */
#define		VECT0FLG_HATCH 0x0004	/* Part of a hatching pattern */
/*00*/		UWORD	Group_ID;	/* Group pair is in */
/*00*/		UWORD	Layer_ID;	/* Layer pair is in */
/*00*/		UWORD	Pen_ID;		/* Pen to draw vector with */
/*00*/		PORTFLOAT	p1x,p1y; /* From point */
/*00*/		PORTFLOAT	p2x,p2y; /* To point */
/*00*/	} vectbundles[VECTORMAX+4];	/* Total number of bundles allowed */

};

#define HATCH0_REV 0

struct iff_hatch_0 {

	UWORD		defrev;

	UBYTE		FillType;
#define FILL0_COLOR 1		/* Fill solid with current color */
#define FILL0_SPATT 2		/* Fill with simple pattern */
#define FILL0_CPATT 3		/* Fill with pattern from library */
	UWORD		FillFlags;

/*00*/	UWORD	Group_ID;	/* Group pair is in */
/*00*/	UWORD	Layer_ID;	/* Layer pair is in */
/*00*/	UWORD	Pen_ID;		/* Pen to draw vector with */

	UWORD	Hatch_ID;	/* Hatching pattern id */
	UWORD	Block_ID;	/* Block for complex pattern */

	UWORD	Number_Polys;	/* How many polygons we dealin' with here */	
	PORTFLOAT Hatch_Rotation; /* How far is pattern rotated */
	PORTFLOAT Hatch_Scale;	/* Scale of pattern (1.0) */
};	

#define VSTRMAX 512

#define VSTR0_REV 0

struct iff_vstr_0 {

	UWORD		defrev;		/* Version number */

	UWORD		NumberStrs;	/* Number of Vector string blocks */

	UBYTE		StrFlags;	/* String Flags */
#define VSTRM0FLAG_CONT 0x01		/* String is a continuation from prev */

	PORTFLOAT	sx,sy;		/* The starting pt of string */

	UWORD		Group_ID;	/* Group id of string (must be same) */
	UWORD		Layer_ID;	/* Start layer */
	UWORD		Pen_ID;		/* Start Pen */

	struct _s0bundle {
		UBYTE	StrFlags;
#define		VSTR0FLG_P1AR 0x01	/* Arrow facing prev pt from this pt */
#define		VSTR0FLG_P2AR 0x02	/* Opp arrow */
#define		VSTR0FLG_SPLN 0x04	/* This is a spline member */
#define		VSTR0FLG_LONG 0x80	/* This is a long packet */
		PORTFLOAT px,py;	/* The X and Y of this point */

		UBYTE	Blend;		/* Blending number for spline */
		UWORD	Layer_ID;	/* This layer id */
		UWORD	Pen_ID;		/* The pen id */
	} svecbunds[VSTRMAX+1];		/* The bundle */
};

#define CIRCLEMAX 64			/* Number allowed in a bundle */

#define CIRC0_REV 0

struct iff_circ_0 {			/* Circle/Ellipse storage */

/****/	UWORD		defrev;		/* Revision of structure */

/*00*/	UWORD		NumberCircs;	/* Total number of ellipses */

/*00*/	struct _c0bundle {
/*00*/		UBYTE	CircFlags;	/* Flags for circle status */
#define		CIRC0FLG_ELLIP 0x0001	/* Has two radii */
#define		CIRC0FLG_LIMTS 0x0002	/* Has beginning and ending points */
#define		CIRC0FLG_ROTA  0x0004	/* Rotated off x, y axis */
#define		CIRC0FLG_HATCH 0x0008	/* Part of hatching pattern */
/*00*/		UWORD	Group_ID;	/* Group data is in */
/*00*/		UWORD	Layer_ID;	/* Layer data is in */
/*00*/		UWORD	Pen_ID;		/* Pen to draw circle with */
/*00*/		PORTFLOAT centx,centy,
/*00*/			radiusx,radiusy,
/*00*/			starta,enda,
/*00*/			rotation;	/* Essential data */
/*00*/		} circbundles[CIRCLEMAX+4];

};

#define TEXT0_REV 0

struct iff_text_0 {			/* Text structure storage */

/****/	UWORD		defrev;		/* Revision of structure */

/*00*/	UBYTE		TextFlags;	/* Text flagging */
#define		TEXT0FLG_XFLP 0x0001	/* Flip about x axis */
#define		TEXT0FLG_YFLP 0x0002	/* Flip about y axis */
#define		TEXT0FLG_CFLP 0x0004	/* Flip chars individually */
/*00*/	UBYTE		Handle_Pos;	/* Define handle position */
#define		TEXT0HND_LEFT 1		/* Handle is left bottom */
#define		TEXT0HND_CENT 2		/* Handle is in center */
#define		TEXT0HND_RIGH 3		/* Handle right bottom corner */
/*00*/	PORTFLOAT	Xpos,Ypos;	/* Placement of handle */
/*00*/	PORTFLOAT	Rotation;	/* Rotation about handle point */
/*00*/	UWORD		Letter_ID;	/* What lettering style is this? */
/*00*/	UWORD		Group_ID;	/* Group data is in */
/*00*/	UWORD		Layer_ID;	/* Layer data is in */
/*00*/	UWORD		Pen_ID;		/* Pen to draw text with */

/*00*/	UWORD		textlen;		/* Length of text minus null */

/*00*/	UBYTE		TextData[257];	/* Data with trailing null */	
};
