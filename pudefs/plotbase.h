/* Main plotter interface - UltraCAD */

struct plot_info {
	UBYTE	PlotName[17];			/* Name of plotter */
	UBYTE	Comment[31];			/* Description of plotter */
	LONG	MinCX,MaxCX,MinCY,MaxCY;	/* Size of bed in counts */
	STDFLT	StepX,StepY;			/* Size of step in mm */
	UWORD	PhysPens;			/* Number physical pens */
	WORD	AutoPens;			/* Number of autopens */
	UWORD	PlotType;			/* What kind of plotter? */
#define  PLOT_PENPLOT 	1	/* Physical Pen/Ink Plotter */
#define  PLOT_DOTMAT	2	/* Dot matrix type device */
#define	 PLOT_DESCRIP	3	/* Laser Printer with description language */
	ULONG	PlotFlags;			/* Flags for plotter */
#define  PLFLAG_VECTOR	0x00000002	/* Plotter can plot vectors */
#define	 PLFLAG_CIRCLE	0x00000004	/* Plotter can do full circles */
#define	 PLFLAG_CIRCARC	0x00000008	/* Can do arcs of circles */
#define  PLFLAG_ELLIP	0x00000010	/* Can do full ellipses */
#define  PLFLAG_ELLARC	0x00000020	/* Can do ellipses with arcs */
#define	 PLFLAG_ROTELP	0x00000040	/* Rotated full ellipses possible */
#define	 PLFLAG_ROTARC	0x00000080	/* Rotated ellipses with arcs */
#define	 PLFLAG_PENVEL	0x00000100	/* Pen velocity is settable */
#define	 PLFLAG_PENACC	0x00000200	/* Pen acceleration is settable */
#define	 PLFLAG_LINSTY	0x00000400	/* Plotter can do its own line styles */
#define	 PLFLAG_PENSRT	0x00000800	/* Sort in physical pen order */
#define  PLFLAG_PHYPEN	0x00001000	/* Plotter needs physical pens */
#define  PLFLAG_AUTOPN	0x00002000	/* Pen has autopen capability */
#define  PLFLAG_PGFEED	0x00004000	/* Plotter can page by itself 
					   endplot does and end page */
#define  PLFLAG_PRINTER 0x00008000	/* Must use printer device */
#define  PLFLAG_TOPDOWN	0x00010000	/* Segments from top down */
	UWORD	NumPresets;		/* Total number of presets */
	WORD	PassPerPage;		/* Number of passes needed per page 
					   set by initplot */
	LONG	MinPX,MinPY,MaxPX,MaxPY; /* Max and min corrds for pass */
};

struct main_interface {
	UWORD	subver;			/* Number of subroutines supported */
	VOID	(*dataout)(/* buffer,size */);	
	VOID	(*filereq)(/* path,mask,flags,retval */);
	VOID	(*reporterr)(/* errortext */);
	VOID	(*getfreemem)(/* retchip,retfast */);
	
};

struct plot_interface {
	UWORD	numbsubs;		/* Number of plot subs available */
	BOOL	(*initdriver)(/* MainBase */);
	BOOL	(*dopreset)();
	BOOL	(*initplot)();
	VOID	(*stepaction)(/* StepDirection */);
	VOID	(*movetopoint)(/* x,y */);
	VOID	(*drawaline)(/* x,y */);
	VOID	(*setpenvelocity)(/* mm/sec */);
	VOID	(*setpenaccel)(/* mm/sec**2 */);
	VOID	(*drawellipse)(/* x,y,xr,yr,sa,ea,rot */);
	VOID	(*setuppen)(/* logicalpen,physicalpen,setup */);
	VOID	(*endplot)();
	VOID	(*expungedriver)();
	VOID	(*beginpass)();
};

struct	plotter_base {
	struct	plot_interface	*PLI;
	struct	main_interface	*MNI;
	struct	plot_info	*MPI;
	ULONG	MinFree;
	ULONG	BufMax;
	ULONG	BufMin;
	APTR	customsetup;
	UWORD	customsize;
};

struct PlotterSegment {
	ULONG	NextSegment;		/* BPTR to next segment */
	ULONG	runAlert;		/* MOVEQ #0, D0 : RTS */
	UWORD	Version;
	UWORD	Revision;
	APTR	customsetup;		/* A custom definition area */
	UWORD	customsize;		/* Size of the custom area */
	struct	plot_interface	PLI;	/* Driver passes us this */

};

struct logical_pen {
	ULONG	Pen_Red;
	ULONG	Pen_Green;
	ULONG	Pen_Blue;
	STDFLT	Line_Width;
	UWORD	Line_Pattern;
	STDFLT	Bit_Width;
	UWORD	Order;
	UWORD	Carousel;
	UWORD	AutoNum;
	UWORD	Identity;		/* Driver can set this */
	VOID	*IdentPtr;		/* Driver can set things here */
};
