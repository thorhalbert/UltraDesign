#include "driversetup.h"

struct	plotter_base	*PB;

#define ROLCAP 	PLFLAG_VECTOR | PLFLAG_CIRCARC | \
		PLFLAG_PENSRT | PLFLAG_PHYPEN | PLFLAG_AUTOPN	

struct	preset_data {
	ULONG	setflags;
	UBYTE	*plotname;
	UBYTE	*comment;
	LONG	xmin,xmax,ymin,ymax;
	STDFLT	xres,yres;
	UWORD	autopens;
#define TOTALPRESETS 2
} rol_presets[] = {
	{ ROLCAP, "DXY-101","Roland DXY-101",	0,3700,0,2600,	0.1,0.1, 0},
	{ ROLCAP, "DXY-800","Roland DXY-800",	0,3500,0,2600,	0.1,0.1, 8}} ;

BOOL rol_initdriver(inbase)
struct plotter_base *inbase;
{

	PB = inbase;			/* Set up our context */
	PB->MPI->NumPresets = TOTALPRESETS;	/* Tell em how many */

	return(TRUE);
}

BOOL rol_dopreset(presnum)
int presnum;
{

	if (presnum<0) return(FALSE);
	if (presnum>=TOTALPRESETS) return(FALSE);	/* Out of range */

	sprintf(PB->MPI->PlotName,"%s",rol_presets[presnum].plotname);
	sprintf(PB->MPI->Comment,"%s",rol_presets[presnum].comment);

	PB->MPI->MinCX = rol_presets[presnum].xmin;
	PB->MPI->MaxCX = rol_presets[presnum].xmax;
	PB->MPI->MinCY = rol_presets[presnum].ymin;
	PB->MPI->MaxCY = rol_presets[presnum].ymax;

	PB->MPI->StepX = rol_presets[presnum].xres;
	PB->MPI->StepY = rol_presets[presnum].yres;

	PB->MPI->PlotType = 1;		/* Pen plotter */

	PB->MPI->PlotFlags = rol_presets[presnum].setflags;

	PB->MPI->AutoPens = rol_presets[presnum].autopens;

	return(TRUE);
}

VOID rol_stepaction() { return; }

BOOL rol_initplot()
{
	PB->MPI->PassPerPage = 1;	/* Just need one per pen per page */

	plotf("\n\n\n\n\0330\n\n");		/* Set to plot mode (ESC0) */
	plotf("H\n\n");			/* Send Device Home */
	plotf("L0\n\n");			/* Draw solid lines */
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

	plotf("I");

	for (i=0; i<pres; i++) {
		if (i==0) plotf("%d,%d",xpres[i],ypres[i]);
		else plotf(",%d,%d",xpres[i],ypres[i]);
	}
	plotf("\n\n");
	pres = 0;
}

VOID rol_movetopoint(x,y)
LONG	x,y;
{

	_clearpres();	
	plotf("M%d,%d\n\n",x,y);	/* Move pen up to x,y */
	lasx = x;
	lasy = y;
	return;
}

VOID rol_drawaline(x,y)
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

VOID rol_drawellipse(x,y,rx,ry,ba,ea,er)	/* Circle-Arc Only */
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

	plotf("C%d,%d,%d,%d,%d\n\n",x,y,rx,bad,ead);	/* Draw circle */

	return;		
}

VOID rol_endplot()
{

	_clearpres();

	plotf("H\n\n");		/* Home the plotter */

	return;

}

VOID rol_setuppen(inlogical,inphysical,mountpen,setup)
struct physical_pen *inphysical;
struct logical_pen *inlogical;
int mountpen;
BOOL setup;				/* First Initialization Pass? */
{

	_clearpres();

	if (setup) return;		/* Roland's don't need this */

	if (mountpen>=1) {			/* Cannot unload the pen */
		plotf("J%d\n\n",mountpen);	/* Pick up given pen */
		return;
	}

	return;

}

VOID rol_setpenvelocity() { return; }
VOID rol_setpenaccel() { return; }

VOID rol_expungedriver() { return; }

VOID rol_beginpass() 		/* Only a single pass for a plotter */
{

	PB->MPI->MinPX = PB->MPI->MinCX;
	PB->MPI->MaxPX = PB->MPI->MaxCX;
	PB->MPI->MinPY = PB->MPI->MinCY;
	PB->MPI->MaxPY = PB->MPI->MaxCY;

	return; 
}
