struct	ViewContext {

	UBYTE	FullPath[256];
	UBYTE	ViewName[256];

	UWORD	ViewFlags;
#define VIEW_VIEWLOCK 	0x0001		/* Use view dimensions */
#define VIEW_SHEETLOCK	0x0002		/* Use sheet dimensions */

	STDFLT	XViewMIN,XViewMAX;
	STDFLT	YViewMIN,YViewMAX;

	STDFLT	ViewRot;
	STDFLT	RotSin,RotCos;

	STDFLT	XNewOrigin,YNewOrigin;

	STDFLT	ArrowAngle;
	STDFLT	Arrowlen;

	STDFLT	ViewScale;

	struct Group_Attrib 	**Group_Array;
	struct Pen_Attrib 	**Pen_Array;
	struct Layer_Attrib	**Layer_Array;
	struct Lettering_Styles **Style_Array;

	struct ViewContext 	*next,*last;

	STDFLT	XtrapMAX,XtrapMIN,YtrapMAX,YtrapMIN;	/* Trapezoid edges */
	
	ULONG	num_groups;
	ULONG	num_pens;
	ULONG	num_layers;
	ULONG	num_styles;
};
