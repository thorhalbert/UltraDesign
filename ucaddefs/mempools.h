/* mempools.h - memory pool and item allocation and definition */

/*------------------Drawing_node node id types------------------------------*/

#define NODE_LINE 10			/* Node is a line segment */
#define NODE_ELLIP 20			/* Node is an ellipse/circle */
#define NODE_TEXT 30			/* Node is a text segment */
#define NODE_HATCH 40			/* Node is a hatching pattern */

/*------------------Drawing_node node flags----------------------------------*/

#define NODE_ONSCREEN 0x0001		/* The node appears all or partially
					   on the current window */
#define NODE_INCLIP   0x0002		/* The node is also in the clipboard */
#define NODE_FULLWINDOW 0x0004		/* Segment is all in current window */
#define NODE_SELECTED 0x0008		/* Segment is currently selected */
#define NODE_DELETED  0x0010		/* Seg deleted - used to erase screen */
#define NODE_HATCHED  0x0020		/* Node is part of a hatch outline */

/*-----------------Drawing node structure-----------------------------------*/

#define D_NODE struct drawing_node
D_NODE {

	UBYTE		node_id;	/* Id type of this node */	
	UWORD		node_length;	/* Length of whole node */
	UBYTE		node_flags;	/* Flags for node */

	GROUP_A		*above_group;	/* Group Attachment */
	LAYER_A		*node_layer;	/* Layer Attachment */
	PEN_A		*pen_attach;	/* Pen Attachment */

	D_NODE 	*node_previous;		/* Previous list entity */
	D_NODE 	*node_subsequent;	/* Next list entry */

};

#define LINE_P1ARROW 0x01	/* Point 1 has an arrowhead on it */
#define LINE_P2ARROW 0x02	/* Point 2 has an arrowhead on it */

#define LINE_SPLINE  0x04	/* This line is a spline */

#define O_LINE struct line_mode
O_LINE {

	UBYTE	line_flags;	/* Flags for this line */

	STDFLT	x1,y1,x2,y2;	/* End points for line */
};

#define O_ELLIP struct ellip_mode
O_ELLIP {
	
	UBYTE	ellip_flags;

	STDFLT	x1,y1;			/* Center of ellipse */
	STDFLT	xradius,yradius;	/* Major and minor axes */	
	STDFLT	rotation;		/* Axis rotation */
	STDFLT	fromang,toangle;	/* Begin and end angle */
};

#define TEXT_FLIPX 0x0001	/* Flip over X axis */
#define TEXT_FLIPY 0x0002	/* Flip over Y axis */
#define TEXT_CHRFL 0x0004	/* Flip over each letter horizonally */

#define LEFT_HANDLE	1
#define CENTER_HANDLE	2
#define RIGHT_HANDLE	3

#define O_TEXT struct text_mode
O_TEXT {

	STYLE_A	*letstyle; /* pointer to entity stroke font header */
	UBYTE	Handle_Flag;		/* Where is handle attached */
	UBYTE	Text_Bits;		/* Flag bits */
	STDFLT	x1,y1;			/* Location of handle */
	STDFLT	ang;			/* Angle of text */
	UBYTE 	*text;			/* Pointer to the text */

};

#define HATCH_SOLID 1		/* Solid fill hatching */
#define HATCH_PATTERN 2		/* Simple pattern */
#define HATCH_COMPLEX 3		/* Pattern is in a block (not imp) */

#define HATCHF_WINDING 0x01	/* Use winding instead of odd-even clip */

#define O_HATCH struct hatch_mode
O_HATCH {

	UBYTE	hatch_type;		/* What kind of hatching is this */
	UBYTE	hatch_flags;		/* Flags for hatching */
	UWORD	number_polys;		/* Number of polygons which follow */
	STDFLT	pattern_rot;		/* Rotation of pattern in rads */
	STDFLT	pattern_scale;		/* Scale of pattern */
	T_HATCH	*hatch_pattern;		/* Hatching pattern table entry */
#ifdef NEWSTUFF
	BLOCK_A	*hatck_block;		/* Block for complex hatch type */
#endif	
	
};

/*-------------Define as much as possible as macros------------------------*/

#define lxtndr(n) (((UBYTE *) n) + (sizeof (D_NODE)))
#define GetLineData(node) ((O_LINE *) lxtndr(node))
#define GetEllipData(node) ((O_ELLIP *) lxtndr(node))
#define GetTextData(node) ((O_TEXT *) lxtndr(node))
#define GetHatchData(node) ((O_HATCH *) lxtndr(node))

extern VOID FreeAllPools();
extern D_NODE *CreateObjNode(/* UWORD,UWORD */);
extern D_NODE *StoStrLine(/* O_LINE * */);
extern D_NODE *StoStrEllip(/* O_ELLIP * */);
extern D_NODE *StoStrText(/* O_TEXT * */);
extern D_NODE *StoStrHatch(/* O_HATCH * */);
extern VOID LinkIntoGroup(/* D_NODE * */);
extern VOID SpliceIntoGroup(/* D_NODE *, DNODE_* */);

extern D_NODE *lastnode;
