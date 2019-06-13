#include "driversetup.h"

struct	plotter_base	*PB;

#define MAXPATH 500	/* Maximum vectors in path list */

#define PSCAP 	PLFLAG_VECTOR | \
		PLFLAG_CIRCARC
		
#define PSS 	(25.4/720.0)		/* mm/step */

struct	preset_data {
	ULONG	setflags;
	UBYTE	*plotname;
	UBYTE	*comment;
	LONG	xmin,xmax,ymin,ymax;
	STDFLT	xres,yres;
	BOOL	flip;
#define TOTALPRESETS 8
} ps_presets[] = {
	{ PSCAP,"PS_A_P",  "PS US/A P",   0,  6120, 0,  7920, PSS, PSS, FALSE },
	{ PSCAP,"PS_B_P",  "PS US/B P",   0, 12240, 0, 15840, PSS, PSS, FALSE },
	{ PSCAP,"PS_A4_P", "PS MET/A4 P", 0,  5953, 0,  8419, PSS, PSS, FALSE },
	{ PSCAP,"PS_A3_P", "PS MET/A3 P", 0,  8419, 0, 11906, PSS, PSS, FALSE },
	{ PSCAP,"PS_A_L",  "PS US/A L",   0,  7920, 0,  6120, PSS, PSS, FALSE },
	{ PSCAP,"PS_B_L",  "PS US/B L",   0, 15840, 0, 12240, PSS, PSS, FALSE },
	{ PSCAP,"PS_A4_L", "PS MET/A4 L", 0,  8419, 0,  5953, PSS, PSS, FALSE },
	{ PSCAP,"PS_A3_L", "PS MET/A3 L", 0, 11906, 0,  8419, PSS, PSS, FALSE },
};

BOOL ps_initdriver(inbase)
struct plotter_base *inbase;
{

	PB = inbase;			/* Set up our context */
	PB->MPI->NumPresets = TOTALPRESETS;	/* Tell em how many */

	return(TRUE);
}

BOOL ps_dopreset(presnum)
int presnum;
{

	if (presnum<0) return(FALSE);
	if (presnum>=TOTALPRESETS) return(FALSE);	/* Out of range */

	sprintf(PB->MPI->PlotName,"%s",ps_presets[presnum].plotname);
	sprintf(PB->MPI->Comment,"%s",ps_presets[presnum].comment);

	PB->MPI->MinCX = ps_presets[presnum].xmin;
	PB->MPI->MaxCX = ps_presets[presnum].xmax;
	PB->MPI->MinCY = ps_presets[presnum].ymin;
	PB->MPI->MaxCY = ps_presets[presnum].ymax;

	PB->MPI->StepX = ps_presets[presnum].xres;
	PB->MPI->StepY = ps_presets[presnum].yres;

	PB->MPI->PlotType = PLOT_DESCRIP;		/* Laser plotter */

	PB->MPI->PlotFlags = ps_presets[presnum].setflags;

	return(TRUE);
}

VOID ps_stepaction() { return; }

ULONG ofx=9999,ofy=9999;
ULONG veccnt=0;

BOOL ps_initplot()
{

	PB->MPI->PassPerPage = 1;	/* Total needed */

#ifdef PPS
	plotf("\n\n\n\004\004\n\n\n");	/* Clear and EOF */
#endif

#ifdef EPS
	plotf("%%!PS-Adobe-2.0 EPSF-2.0\n");
#endif
#ifdef PPS
	plotf("%%!PS-Adobe-2.0\n");
#endif
	plotf("%%%%BoundingBox: %d %d %d %d\n",
		PB->MPI->MinCX/10,
		PB->MPI->MinCY/10,
		PB->MPI->MaxCX/10,
		PB->MPI->MaxCY/10);
#ifdef EPS
	plotf("%% EPS written by UltraDesign-PasteUp (c) WURBsoft Inc.\n");
#endif
#ifdef PPS
	plotf("%% PS written by UltraDesign-PasteUp (c) WURBsoft Inc.\n");
#endif
	plotf("%%%%EndComments\n");
	plotf("gsave\n");
	plotf("/Courier findfont 10 scalefont setfont\n");
	plotf(".25 setlinewidth 0 setlinecap\n");
	plotf("1 setlinejoin 0 setgray\n");
	plotf("/rl {10 div exch 10 div exch rlineto} bind def\n");
	plotf("/ml {10 div exch 10 div exch moveto} bind def\n");
	plotf("%%%%EndProlog\n");
	  

	ofx=9999; ofy=9999;
	veccnt = 0;

	return(TRUE);

}

VOID psvecclr()
{

	if (veccnt>0) 
		plotf("stroke\n");

	veccnt = 0;
	
}

VOID ps_movetopoint(x,y)
ULONG	x,y;
{

	if (ofx==x&&ofy==y) return;
	ofx = x;
	ofy = y;

	psvecclr();
	
	plotf("newpath %ld %ld ml\n",x,y);	/* Move pen up to x,y */
	return;
}

VOID ps_drawaline(x,y)
ULONG	x,y;
{

	if (ofx==x&&ofy==y) return;
	
	plotf("%ld %ld rl\n",x-ofx,y-ofy);
	veccnt++;
	
	if (veccnt>MAXPATH) {
		psvecclr();
		plotf("newpath %ld %ld ml\n",x,y);
	}

	ofx = x;
	ofy = y;
	
	return;

}

VOID ps_drawellipse(x,y,rx,ry,ba,ea,er)	/* Circle-Arc Only */
ULONG x,y,rx,ry;
STDFLT ba,ea,er;
{


	return;		
}

VOID ps_endplot()
{

	psvecclr();

	plotf("%%%%Trailer\n");
	plotf("grestore\n");
	plotf("%%!End of File\n");

#ifdef PPS
	plotf("showpage\n");		/* Print out page */
	plotf("\n\n\n\004\004\n\n\n");	/* Clear and EOF */
#endif

	return;
}

VOID ps_setuppen(inphysical,inlogical,mountpen,setup)
struct physical_pen *inphysical;
struct logical_pen *inlogical;
int mountpen;
BOOL setup;				/* First Initialization Pass? */
{

	return;
}

VOID ps_setpenvelocity(mmsec)
STDFLT mmsec;
{ 

	return;

}

VOID ps_setpenaccel() { return; }

VOID ps_expungedriver() { return; }

VOID ps_beginpass() 		/* Only a single pass for a plotter */
{

	PB->MPI->MinPX = PB->MPI->MinCX;
	PB->MPI->MaxPX = PB->MPI->MaxCX;
	PB->MPI->MinPY = PB->MPI->MinCY;
	PB->MPI->MaxPY = PB->MPI->MaxCY;

	return; 
}
