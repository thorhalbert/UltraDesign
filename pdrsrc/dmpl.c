#include "driversetup.h"

struct	plotter_base	*PB;

#define DMPLCAP 	PLFLAG_VECTOR | PLFLAG_CIRCARC | \
		PLFLAG_PENSRT | PLFLAG_PHYPEN | PLFLAG_AUTOPN	

#define IOS .06350

struct	preset_data {
	ULONG	setflags;
	UBYTE	*plotname;
	UBYTE	*comment;
	LONG	xmin,xmax,ymin,ymax;
	STDFLT	xres,yres;
	UWORD	autopens;
#define TOTALPRESETS 7
} dmpl_presets[] = {
	{ DMPLCAP, "DMP-40","Houston DMP-40",	0,3700,0,2600,	0.1,0.1, 20},
	{ DMPLCAP, "DMP-41","Houston DMP-41",	0,3500,0,2600,	0.1,0.1, 20},
	{ DMPLCAP, "DMP-42","Houston DMP-42",	0,3500,0,2600,	0.1,0.1, 20},
	{ DMPLCAP, "LP4000-A","LP4000-A",	0,4000,0,3200,	IOS,IOS, 20},
	{ DMPLCAP, "LP4000-B","LP4000-B",	0,6600,0,4400,	IOS,IOS, 20},
	{ DMPLCAP, "LP4000-C","LP4000-C",	0,8800,0,6800,  IOS,IOS, 20},
	{ DMPLCAP, "LP4000-D","LP4000-D",	0,14400,0,9200,  IOS,IOS, 20},
} ;

BOOL dmpl_initdriver(inbase)
struct plotter_base *inbase;
{

	PB = inbase;			/* Set up our context */
	PB->MPI->NumPresets = TOTALPRESETS;	/* Tell em how many */

	return(TRUE);
}

BOOL dmpl_dopreset(presnum)
int presnum;
{

	if (presnum<0) return(FALSE);
	if (presnum>=TOTALPRESETS) return(FALSE);	/* Out of range */

	sprintf(PB->MPI->PlotName,"%s",dmpl_presets[presnum].plotname);
	sprintf(PB->MPI->Comment,"%s",dmpl_presets[presnum].comment);

	PB->MPI->MinCX = dmpl_presets[presnum].xmin;
	PB->MPI->MaxCX = dmpl_presets[presnum].xmax;
	PB->MPI->MinCY = dmpl_presets[presnum].ymin;
	PB->MPI->MaxCY = dmpl_presets[presnum].ymax;

	PB->MPI->StepX = dmpl_presets[presnum].xres;
	PB->MPI->StepY = dmpl_presets[presnum].yres;

	PB->MPI->PlotType = 1;		/* Pen plotter */

	PB->MPI->PlotFlags = dmpl_presets[presnum].setflags;

	PB->MPI->AutoPens = dmpl_presets[presnum].autopens;

	return(TRUE);
}

VOID dmpl_stepaction() { return; }

BOOL dmpl_initplot()
{
	PB->MPI->PassPerPage = 1;	/* Just need one per pen per page */

	plotf("\n;:\n");		/* Set to plot mode (ESC0) */
	plotf("H\n");			/* Send Device Home */
	plotf("L0\n\n");		/* Draw solid lines */
	return(TRUE);

}

#define MAXPRES 20
LONG	xpres[MAXPRES+2],
	ypres[MAXPRES+2];
int	pres=0;
LONG	lasx,lasy;

VOID _clearpres()
{

	int i;
	if (pres<=0) return;

	plotf("RD");

	for (i=0; i<pres; i++) {
		if (i==0) plotf("%d,%d",xpres[i],ypres[i]);
		else plotf(",%d,%d",xpres[i],ypres[i]);
	}
	plotf("\n\n");
	pres = 0;
}

VOID dmpl_movetopoint(x,y)
LONG	x,y;
{

	_clearpres();	
	plotf("AU %d,%d\n\n",x,y);	/* Move pen up to x,y */
	lasx = x;
	lasy = y;
	return;
}

VOID dmpl_drawaline(x,y)
LONG	x,y;
{

	LONG tx,ty;

	if (pres>MAXPRES) _clearpres();

	tx = x - lasx;
	ty = y - lasy;
	if (tx==0&&ty==0) return;
	lasx = x;
	lasy = y;
	xpres[pres] = tx;
	ypres[pres] = ty;
	pres++;

	return;

}

VOID dmpl_drawellipse(x,y,rx,ry,ba,ea,er)	/* Circle-Arc Only */
LONG x,y,rx,ry;
STDFLT ba,ea,er;
{

	UWORD	bad,ead;

	normalize(ba);				/* Fix into normal range */
	normalize(ea);

	bad = (ba/TWOPI) * 360.0;		/* Convert to degrees */
	ead = (ea/TWOPI) * 360.0;

	if (bad==ead) {
		bad = 0;
		ead = 360;
	}

/*	plotf("C%d,%d,%d,%d,%d\n\n",x,y,rx,bad,ead);	*//* Draw circle */

	return;		
}

VOID dmpl_endplot()
{

	_clearpres();

	plotf("H\n");		/* Home the plotter */

	return;

}

VOID dmpl_setuppen(inlogical,inphysical,mountpen,setup)
struct physical_pen *inphysical;
struct logical_pen *inlogical;
int mountpen;
BOOL setup;				/* First Initialization Pass? */
{

	_clearpres();

	if (setup) return;		/* Dmpland's don't need this */

	if (mountpen>=1) {			/* Cannot unload the pen */
		plotf("P %d\n\n",mountpen);	/* Pick up given pen */
		return;
	}

	return;

}

VOID dmpl_setpenvelocity() { return; }
VOID dmpl_setpenaccel() { return; }

VOID dmpl_expungedriver() { return; }

VOID dmpl_beginpass() 		/* Only a single pass for a plotter */
{

	PB->MPI->MinPX = PB->MPI->MinCX;
	PB->MPI->MaxPX = PB->MPI->MaxCX;
	PB->MPI->MinPY = PB->MPI->MinCY;
	PB->MPI->MaxPY = PB->MPI->MaxCY;

	return; 
}
