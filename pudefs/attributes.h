#ifndef ATTRIBUTE_INFO
#define ATTRIBUTE_INFO
#include "fonts.h"
#include "hatch.h"

/*---Structures for the storage of text fonts and lettering style blocks-----*/

#define STROKE_EXT ".Ufnt"		/* Standard font extension */
#define MAXFONTNAME 30			/* Max stroke file name */
#define MAXSTYLEID  30			/* Maximum lettering style id */

/*----------------Linked list of loaded fonts names and info-----------------*/

#define T_FONT struct Text_Fonts

T_FONT {

	UBYTE	Font_Name_ID[MAXFONTNAME+1];	/* Font file name */
	struct	Stroke_Font *font_link;		/* Location of font */
	LONG	Use_Count;			/* Number of uses */
	T_FONT 	*next_font;			/* Doubly linked list */
	T_FONT 	*prev_font;		
	ULONG	Font_Size;			/* How big is font? */

};

extern 	T_FONT 	*Root_Font,
		*Font_Last;

/*---------------Linked list for hatching patterns---------------------------*/

#define T_HATCH struct Hatch_Entry

extern	T_HATCH *Root_Hatch,
		*Hatch_Last;

/*-------------Doubly linked list of lettering style blocks------------------*/

#define STYLE_DEFAULT 0x0001

#define STYLE_A struct Lettering_Styles

STYLE_A {

	UBYTE	Letter_Style_ID[MAXSTYLEID+1];	/* Style id */
	T_FONT *draw_font;			/* Stroke font to use */
	LONG	Use_Count;			/* Number of uses */
	STDFLT	Paper_Height;			/* Height in paper scale */
	STDFLT	Prime_Height;			/* Initial height of text */
	STDFLT	Aspect;				/* Height over Width ratio */
	STDFLT	Slant;				/* Slant factor */
	STYLE_A *prev_style;			/* Backward Link */
	STYLE_A *next_style;			/* Forward Link */
	WORD	Letter_ID;			/* IFF link id */
	UBYTE	Letter_Flags;			/* Flags */
	UBYTE	*Comment;			/* Ptr to comment string */
};

extern 	STYLE_A *Root_Style,
		*Style_Last,
		*Current_Style,
		*Default_Style;

/*------------------------Information for Layer attrib---------------------*/

#define MAXLAYERNAME 30
#define LAYER_LOCKED	0x0001		/* Layer is locked - no changes */
#define LAYER_GHOSTED	0x0002		/* Layer displayed as ghost only */
#define LAYER_DOFF	0x0004		/* Layer not displayed at all */
#define LAYER_POFF	0x0008		/* Plotting off */

#define LAYER_A struct Layer_Attrib 

LAYER_A {

	UBYTE	Layer_Name[MAXLAYERNAME+1];	/* Name of layer */
	LONG	Use_Count;			/* Number of objects present */
	UWORD	Color_ID;			/* Layer Color Register # */
	UWORD	Layer_Flags;			/* Different flags of above */
	WORD	Layer_ID;			/* Layer Number Assigned */

	LAYER_A *layer_next;
	LAYER_A *layer_prev;

	UBYTE	*Comment;

};

extern 	LAYER_A *Root_Layer,
		*Last_Layer,
		*Current_Layer;
			
/*---------------------Information for Group attrib------------------------*/

#define MAXGROUPNAME 30
#define GROUP_LOCKED	0x0001		/* Group is locked - no changes */
#define GROUP_GHOSTED 	0x0002		/* Group is ghosted */
#define GROUP_DOFF	0x0004		/* Group display turned off */
#define GROUP_POFF	0x0008		/* Group is not plotted */
#define GROUP_ALTORG	0x0010		/* Use alternate group origin */
#define GROUP_NOBRANCH	0x0020		/* Only leaves allowed at this node */

#define LIBR_ASSY	0x0001		/* Here is an assembly */

#define GROUP_A struct Group_Attrib

GROUP_A {

	UBYTE	Group_Name[MAXGROUPNAME+1];	/* Name of group */
	LONG	Use_Count;			/* Number of objects */
	UWORD	Color_ID;			/* Group mode coloring */
	WORD	Group_ID;			/* Grouping pointer for IFF */
	UWORD	Group_Flags;			/* Group flags as above */
	UWORD	Libr_Flags;			/* Library flags if applic */

	GROUP_A *parent;			/* Parent node of this */
	GROUP_A *next;				/* Next sibling on this level */
	GROUP_A *previous;			/* Prev sibling */
	GROUP_A *descendants;			/* Link to first lower level */
	GROUP_A *desc_last;			/* Last descendant */

	BOOL	cursel;				/* Group/desc are selected */

	UBYTE	*Comment;
};

extern 	GROUP_A *Root_Group,		/* Prime group */
		*Current_Group;		/* Current group */

/*-------------------------Pen Attributes---------------------------------*/

#define MAXPENID    30			/* Maximum letters in pen id */

#define PEN_GHOSTED	0x0001		/* Write as a ghost */
#define PEN_LOCKED	0x0002		/* Lock pen activity */
#define PEN_DOFF	0x0004		/* Do not display pens */
#define PEN_POFF	0x0008		/* Pen does not plot */

#define PEN_A struct Pen_Attrib

PEN_A {

	UBYTE	Pen_Name[MAXPENID+1];	/* Pen Name */
	LONG	Use_Count;		/* Count of objects using pen */
	UWORD	Color_ID;		/* Pen Mode Coloring */
	
	UWORD	Line_Pattern;		/* Pattern sequence */
	STDFLT	BitWidth;		/* Width of each bit */

	WORD	Pen_ID;			/* IFF pen id */
	UBYTE	Pen_Flags;		/* Flag information */
	
	ULONG	Hard_Color;		/* Hardcopy color U8-R8-G8-B9 */

	PEN_A 	*pen_next;
	PEN_A 	*pen_prev;

	UBYTE	*Comment;

	STDFLT	PenWidth;

	UWORD	PenOrd;			/* Pen order */
	UWORD	Carousel;		/* Number of Carousel */
	UWORD	AutoNum;		/* Number of the pen */
	
	struct	logical_pen	pass_pen;

};

extern 	PEN_A 	*Root_Pen,		/* Top of pen structure */
		*Last_Pen,		/* Last pen in list */
		*Current_Pen,		/* Pen we are drawing with */
		*Default_Pen;		/* Template pen */

/*-------------------------------------------------------------------------*/
#endif /* ATTRIBUTE_INFO */
