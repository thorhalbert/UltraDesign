#include "driversetup.h"

struct	plotter_base	*PB;

#define HPCAP 	PLFLAG_VECTOR | PLFLAG_CIRCARC | \
		PLFLAG_PENSRT | PLFLAG_PHYPEN | PLFLAG_AUTOPN	
#define HPS 0.02487	/* mm/step */
#define HPCH	2	/* Chord setting for arcs */

#define IOS 0.06275

struct	preset_data {
	ULONG	setflags;
	UBYTE	*plotname;
	UBYTE	*comment;
	LONG	xmin,xmax,ymin,ymax;
	STDFLT	xres,yres;
	UWORD	autopens;
#define TOTALPRESETS 5
} hp_presets[] = {
	{ HPCAP,"HP_7475A_B", "HP-7475A US/B", 0,16640,0,10365,HPS,HPS, 6},
	{ HPCAP,"HP_7475A_A", "HP-7475A US/A", 0,10365,0, 7962,HPS,HPS, 6},
	{ HPCAP,"HP_7475A_A3", "HP-7475A MET/A3", 0,16158,0,11040,HPS,HPS, 6},
	{ HPCAP,"HP_7475A_A4", "HP-7475A MET/A4", 0,11040,0, 7721,HPS,HPS, 6},
	{ HPCAP,"IOLINE 4000", "IO4000 US/B", 0, 6400, 0, 4400, IOS, IOS,20 }};


BOOL hp_initdriver(inbase)
struct plotter_base *inbase;
{

	PB = inbase;			/* Set up our context */
	PB->MPI->NumPresets = TOTALPRESETS;	/* Tell em how many */

	return(TRUE);
}

BOOL hp_dopreset(presnum)
int presnum;
{

	if (presnum<0) return(FALSE);
	if (presnum>=TOTALPRESETS) return(FALSE);	/* Out of range */

	sprintf(PB->MPI->PlotName,"%s",hp_presets[presnum].plotname);
	sprintf(PB->MPI->Comment,"%s",hp_presets[presnum].comment);

	PB->MPI->MinCX = hp_presets[presnum].xmin;
	PB->MPI->MaxCX = hp_presets[presnum].xmax;
	PB->MPI->MinCY = hp_presets[presnum].ymin;
	PB->MPI->MaxCY = hp_presets[presnum].ymax;

	PB->MPI->StepX = hp_presets[presnum].xres;
	PB->MPI->StepY = hp_presets[presnum].yres;

	PB->MPI->PlotType = 1;		/* Pen plotter */

	PB->MPI->PlotFlags = hp_presets[presnum].setflags;

	PB->MPI->AutoPens = hp_presets[presnum].autopens;

	return(TRUE);
}

VOID hp_stepaction() { return; }

BOOL hp_initplot()
{

	PB->MPI->PassPerPage = 1;	/* Total needed */

	plotf("\r\r");			/* Clear input buffer */
	plotf("\033.I81;;17:\r");	/* Set xoff to dc1 tresh to 81 chrs */
	plotf("\033.N;19:\r");		/* Set xon char to dc3 */
	plotf("DF;\r");			/* Reset defaults */
	plotf("PU;\r");			/* Make sure the pen is up */
	return(TRUE);

}

VOID hp_movetopoint(x,y)
ULONG	x,y;
{

	plotf("PU%ld,%ld;\r",x,y);	/* Move pen up to x,y */
	return;
}

VOID hp_drawaline(x,y)
ULONG	x,y;
{

	plotf("PD%ld,%ld;\r",x,y);	/* Draw with pen down to x,y */
	return;

}

VOID hp_drawellipse(x,y,rx,ry,ba,ea,er)	/* Circle-Arc Only */
ULONG x,y,rx,ry;
STDFLT ba,ea,er;
{

	UWORD	bad,ead;
	ULONG	startx,starty;

	normalize(ba);				/* Fix into normal range */
	normalize(ea);

	bad = (ba/TWOPI) * 360.0;		/* Convert to degrees */
	ead = (ea/TWOPI) * 360.0;

	if (bad==ead) {
		bad = 0;
		ead = 360;
	}

/*	startx = sin(bad)*rx;
	starty = cos(bad)*rx; */

	plotf("PU%ld,%ld;\r",startx,starty);		/* Start of arc */
	plotf("AA%ld,%ld,%d,%d;\r",x,y,ead-bad,HPCH);	/* Draw arc */

	return;		
}

VOID hp_endplot()
{

	plotf("PU0,0;\r");		/* Home the plotter */

	return;

}

VOID hp_setuppen(inphysical,inlogical,mountpen,setup)
struct physical_pen *inphysical;
struct logical_pen *inlogical;
int mountpen;
BOOL setup;				/* First Initialization Pass? */
{

	if (setup) return;		/* HPGL's can't do this */

	if (mountpen>=0) {
		/* If mountpen==0 then put the old pen back */
		plotf("SP%d;\r",mountpen);	/* Pick up given pen */
		return;
	}

	return;
}

VOID hp_setpenvelocity(mmsec)
STDFLT mmsec;
{ 

	mmsec /= 10.0;			/* Convert to cm/sec */
	if (mmsec<0.0) mmsec = 0.0;
	if (mmsec>127.99) mmsec = 127.99;

	plotf("VS%.2f;\r",mmsec);
	return;

}

VOID hp_setpenaccel() { return; }

VOID hp_expungedriver() { return; }

VOID hp_beginpass() 		/* Only a single pass for a plotter */
{

	PB->MPI->MinPX = PB->MPI->MinCX;
	PB->MPI->MaxPX = PB->MPI->MaxCX;
	PB->MPI->MinPY = PB->MPI->MinCY;
	PB->MPI->MaxPY = PB->MPI->MaxCY;

	return; 
}

