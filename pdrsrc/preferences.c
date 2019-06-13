#include "driversetup.h"

struct	plotter_base	*PB;

#include <exec/memory.h>
#include <graphics/gfxmacros.h>
#include <devices/printer.h>
#include <devices/prtbase.h>

struct IODRPReq PReq;
struct PrinterData *PD;
struct PrinterExtendedData *PED;

#define COLMAX 256
#define COLEXP 8
BOOL ColorMode = FALSE;
int colnum=0,curcolor=0;
ULONG amigcolor[3][COLMAX];
ULONG powtab[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096 };

#define DEBUG	(*PB->MNI->reporterr)
#define OURVECTORS

BOOL pref_dopreset(presnum)
int presnum;
{

	STDFLT LinHgt,tf;
	LONG	tl;

	sprintf(PB->MPI->PlotName,"%s","preferences");
	sprintf(PB->MPI->Comment,"%s %u(%u)",
            PED->ped_PrinterName, PD->pd_SegmentData->ps_Version,
            PD->pd_SegmentData->ps_Revision);

	PB->MPI->StepX = IN_MM/((STDFLT) PED->ped_XDotsInch);
	PB->MPI->StepY = IN_MM/((STDFLT) PED->ped_YDotsInch);

	PB->MPI->MinCX = 0;
	PB->MPI->MaxCX = PED->ped_MaxXDots;
	PB->MPI->MinCY = 0; 

	if (PED->ped_MaxYDots<=0) {
		LinHgt = 1.0/6.0;
		if (PD->pd_Preferences.PrintSpacing==EIGHT_LPI) 
			LinHgt = 1.0/8.0;

		PB->MPI->MaxCY = ((STDFLT) PD->pd_Preferences.PaperLength) *
			LinHgt * ((STDFLT) PED->ped_YDotsInch);

	} else	PB->MPI->MaxCY = PED->ped_MaxYDots;

	PB->MPI->PlotType = 1;		/* Pen plotter */

	PB->MPI->PlotFlags = PLFLAG_PRINTER | PLFLAG_TOPDOWN;

	PB->MPI->AutoPens = 0;

	return(TRUE);
}

BOOL pref_initdriver(inbase)
struct plotter_base *inbase;
{

	int i;

	if (!InitBases()) return(FALSE);

	PB = inbase;			/* Set up our context */
	PB->MPI->NumPresets = 1;	/* One and only one */

	if (OpenDevice("printer.device", 0, &PReq, 0) != NULL) {
		return(FALSE);		/* It didn't open */
	}

        /* get pointer to printer data */
        PD = (struct PrinterData *)PReq.io_Device;
        /* get pointer to printer extended data */
        PED = &PD->pd_SegmentData->ps_PED;
        /* let's see what's there */

	(VOID) pref_dopreset(1);	/* Get info for printer */

	for (i=0; i<COLMAX; i++) 
		amigcolor[0][i]=~0; amigcolor[1][i]=~0; amigcolor[2][i]=~0; 
	
	colnum = 2;
	amigcolor[0][0]= ~0; amigcolor[1][0]= ~0; amigcolor[2][0]= ~0; 
	amigcolor[0][1]=  0; amigcolor[1][1]=  0; amigcolor[2][1]=  0; 

	if (PD->pd_Preferences.PrintShade == SHADE_BW) ColorMode = FALSE;
	else ColorMode = TRUE;

	return(TRUE);
}

VOID pref_stepaction() { return; }

struct BitMap BM;
struct RastPort BR;
struct ViewPort BV;
struct Layer LY;

UBYTE	*dispbuffer[COLEXP]=NULL;
UWORD	*bitmaps[COLEXP];
LONG	buffsize;
UWORD	bufplanes=0;
LONG 	xsize= -1, ysize= -1;
LONG	abufx,wordcol,abufy,starty,begy,framex,framey;
WORD	pass= -2;

BOOL trybuf(x,y)
{

	LONG xi,yi,t1,t2,cfree,i;
	wordcol = (x+15)>>4;

	buffsize = RASSIZE(x,y);	/* Assure longword boundary */

	(*PB->MNI->getfreemem)(&t1,&t2);
	cfree = t1+t2;		/* Total free memory */

	if (buffsize*bufplanes>(cfree-PB->MinFree)) return(FALSE); 
					/* Don't try it, not enough free */
					
	for (i=0; i<COLEXP; i++) dispbuffer[i] = NULL;

	for (i=0; i<bufplanes; i++) {
	
		dispbuffer[i] = AllocMem(buffsize, MEMF_PUBLIC);	/* Try to get
							memory from anywhere */
		
		if (dispbuffer[i]==NULL) {				
			for (i=0; i<COLEXP; i++) 
				if (dispbuffer[i]) 
					FreeMem(dispbuffer[i], buffsize);
			for (i=0; i<COLEXP; i++) dispbuffer[i] = NULL;
			return(FALSE);
		}
	}

	for (i=0; i<COLEXP; i++) bitmaps[i] = (UWORD *) dispbuffer[i];

	InitBitMap(&BM, bufplanes, x, y);
	for (i=0; i<bufplanes; i++) 
		BM.Planes[i] = dispbuffer[i];

	abufx = x;
	abufy = y;

	InitRastPort(&BR);
	BR.BitMap = &BM;

	InitVPort(&BV);
	BV.ColorMap = GetColorMap(1<<bufplanes);

	for (i=0; i<(1<<bufplanes); i++) 
		SetRGB4(&BV,i,
			amigcolor[0][i]>>12,
			amigcolor[1][i]>>12,
			amigcolor[2][i]>>12);

	return(TRUE);	/* Got it */

}

BOOL pref_initplot()
{

	int i;
	LONG	yi,dots;
	
	bufplanes = 0;	
	for (i=1; i<COLEXP; i++) 
		if (colnum>powtab[i-1]) bufplanes=i;
		
	DEBUG("Bufplanes = %d, colnum=%d\n",bufplanes,colnum);
		
	dots = PED->ped_NumRows;

	if (xsize != PB->MPI->MaxCX &&
	    ysize != PB->MPI->MaxCY) {

		for (i=0; i<COLEXP; i++) 
			if (dispbuffer[i]) FreeMem(dispbuffer[i], buffsize);
		for (i=0; i<COLEXP; i++) dispbuffer[i] = NULL;

		/* Try for maximum buffer, then ratchet down */
		if (!trybuf(PB->MPI->MaxCX,PB->MPI->MaxCY)) {
			for (yi = (PB->MPI->MaxCY/dots)*dots;
				yi>0; yi-=dots)
				if (trybuf(PB->MPI->MaxCX,yi)) break;
		}
			
		if (dispbuffer[0]==NULL) return(FALSE);	/* Not enough mem */

		xsize = PB->MPI->MaxCX;
		ysize = PB->MPI->MaxCY;

	}

	pass = -1;
	starty = 0;

	PB->MPI->PassPerPage = PB->MPI->MaxCY/abufy;
	if (PB->MPI->MaxCY%abufy>0) PB->MPI->PassPerPage++;

	return(TRUE);

}

#ifdef OURVECTORS

int curx,cury;

#define PX 0x01
#define MX 0x02
#define PY 0x04
#define MY 0x08

VOID setbit(x,y)
ULONG x,y;
{

	register int i;
	register ULONG 	ad,bit;
	register UWORD	outword;;

	if (x<=0) return;
	if (y<=0) return;
	if (x>=abufx) return;
	if (y>=abufy) return;

	ad = (wordcol * y) + (x>>4);
	bit = x&0xF;			/* What bit is it */

	outword = 1<<(15-bit);

	for (i=0; i<bufplanes; i++)
		if (curcolor&powtab[i]) bitmaps[i][ad] |= outword;
		else			bitmaps[i][ad] &= ~outword;

}

VOID calplot(run,rise,runstep,risestep)
int run,rise,runstep,risestep;
{

#define MAXDEP 15
	register int t2,t3;

	int t1,t4,limit,use,d1,d2,d3,op;
	int darr[MAXDEP+1],dlef[MAXDEP+1];

	limit=1;
	t2=0;
	d2=rise;
	d1=run;

	if (rise!=0) {
		for (t3=0; t3<MAXDEP; t3++) {
			if (d2==0) break;	/* How does this happen? */
			t4=d1/d2;
			if (t4<=0) break;
			if ((limit*=t4)>run) break;
			d2=d3=d1%d2;
			darr[t2]=t4;
			dlef[t2++]=0;
		}
	}

	darr[t2]=0;
	dlef[t2]=0;

	for (t1=0; t1<run; t1++) {
		use = 0;
		for (t3=0; t3<t2; t3++) {
			if (++dlef[t3]<darr[t3]) break;
			dlef[t3]=0;
			use=(use)?0:1;
		}
		switch(runstep) {
			case PX: curx++;	break;
			case MX: curx--;	break;
			case PY: cury++;	break;
			case MY: cury--;	break;
		}
		if (use) {
			switch(risestep) {
				case PX: curx++;	break;
				case MX: curx--;	break;
				case PY: cury++;	break;
				case MY: cury--;	break;
			}
		}
			
		setbit(curx,cury);

	}

	return;
}

VOID pref_movetopoint(x,y)
ULONG	x,y;
{

	/* y -= begy; */
	y = framey - y;

	curx=x;
	cury=y;

}


int run[] =  {PX,PY,MX,PY,PX,MY,MX,MY};
int rise[] = {PY,PX,PY,MX,MY,PX,MY,MX};

VOID pref_drawaline(x,y)
ULONG	x,y;
{

	int dx,dy,quad,t1/*,runstep,risestep*/;

	/* y -= begy; */
	y = framey - y;

	dx = x-curx;
	dy = y-cury;

	if (dx==0&&dy==0) return;

	quad=0;

	if (dy<0) {
		quad |= 4;
		dy = -dy;
	}
	if (dx<0) {
		quad |= 2;
		dx = -dx;
	}
	if (abs(dy)>abs(dx)) {
		quad |= 1;
		t1=dx;
		dx=dy;
		dy=t1;
	}
/*
	switch (quad) {
		case 0: case 4:	runstep=PX; break;
		case 1: case 3:	runstep=PY; break;
		case 2: case 6:	runstep=MX; break;
		case 5: case 7:	runstep=MY; break;
	}
	switch (quad) {
		case 1: case 5:	risestep=PX; break;
		case 3: case 7:	risestep=MX; break;
		case 0: case 2:	risestep=PY; break;
		case 4: case 6:	risestep=MY; break;
	} */
	
	setbit(curx,cury);
	calplot(dx,dy,run[quad],rise[quad]);

}

#else

VOID pref_movetopoint(x,y)
ULONG	x,y;
{

	/* y -= begy; */
	y = framey - y;
	Move(&BR, x,y);

}

VOID pref_drawaline(x,y)
ULONG	x,y;
{
	/* y -= begy; */
	y = framey - y;
	Draw(&BR, x,y);
}

#endif

VOID dumppass() 
{

	struct MsgPort *port,*msg;
	UBYTE signal;

	PD->pd_Preferences.PrintFlags &= ~DIMENSIONS_MASK;
	PD->pd_Preferences.PrintFlags |= PIXEL_DIMENSIONS | INTEGER_SCALING;
	PD->pd_Preferences.PrintImage = IMAGE_POSITIVE;
	PD->pd_Preferences.PrintAspect = ASPECT_HORIZ;	

	PD->pd_Preferences.PrintMaxWidth = framex;
	PD->pd_Preferences.PrintMaxHeight = framey;

	PReq.io_Command = PRD_DUMPRPORT;
	PReq.io_RastPort = &BR;
	PReq.io_ColorMap = BV.ColorMap;
	PReq.io_Modes = HIRES | LACE;
	PReq.io_SrcX = 0;
	PReq.io_SrcY = 0;
	PReq.io_SrcWidth = framex;
	PReq.io_SrcHeight = framey;
	PReq.io_DestCols = framex;
	PReq.io_DestRows = framey;
	PReq.io_Special = SPECIAL_NOFORMFEED;
	if (pass+1>=PB->MPI->PassPerPage) PReq.io_Special = 0;
	
	DEBUG("Dump Frame=%d\n",framey);

	port = CreatePort(NULL, 0L);
	if (port==NULL) return;	/* Couldn't create a port */
	PReq.io_Message.mn_ReplyPort = port;

	signal = port->mp_SigBit;

	DoIO(&PReq);
#ifdef ASYNC
	SendIO(&PReq);
	Wait(signal);
	while ((msg = GetMsg(port)) != NULL) {
		/* Got an error? */
		ReplyMsg(msg);
	}
#endif
	DeletePort(port);

	return;
}

VOID pref_endplot()
{

	dumppass();

	return;

}

VOID pref_setuppen(inphysical,inlogical,mountpen,setup)
struct physical_pen *inphysical;
struct logical_pen *inlogical;
int mountpen;
BOOL setup;				/* First Initialization Pass? */
{

	int i;
	
	curcolor = 0;
	 
	if (setup==TRUE) {
	
		DEBUG("Pen %d %d %d ",
			inlogical->Pen_Red>>12,
			inlogical->Pen_Green>>12,
			inlogical->Pen_Blue>>12);
			

		if (!ColorMode) return;

#define TB(x) (x&0xF000)		
		for (i=0; i<colnum; i++) {
			if (TB(inlogical->Pen_Red)!=TB(amigcolor[0][i])) continue;
			if (TB(inlogical->Pen_Green)!=TB(amigcolor[1][i])) continue;
			if (TB(inlogical->Pen_Blue)!=TB(amigcolor[2][i])) continue;
			DEBUG("Ident is %d (found)\n",i);
			inlogical->Identity = i;	/* That's same */
			return;
		}
		if (colnum>=COLMAX) {
			DEBUG("Ident is 1 (maxed)\n");
			inlogical->Identity = 1;	/* It is black */
			return;
		}
		
		amigcolor[0][colnum] = inlogical->Pen_Red;
		amigcolor[1][colnum] = inlogical->Pen_Green;
		amigcolor[2][colnum] = inlogical->Pen_Blue;
		
		DEBUG("Ident is %d (add)\n",colnum);
		
		inlogical->Identity = colnum++;

		return;
	}
	
	if (!ColorMode) curcolor=1;
	else curcolor = inlogical->Identity;
	
	return; 
}

VOID pref_setpenvelocity(mmsec)
STDFLT mmsec;
{ return; }

VOID pref_setpenaccel() { return; }

VOID pref_expungedriver() 
{ 
	int i;

        CloseDevice(&PReq); 	/* close the printer device */

	for (i=0; i<COLEXP; i++) 
		if (dispbuffer[i]) FreeMem(dispbuffer[i], buffsize);
	for (i=0; i<COLEXP; i++) dispbuffer[i] = NULL;

	ExpungeBases();	
	return; 
}

VOID clearrp()
{

	LONG i,j;

	for (j=0; j<bufplanes; j++) 
		for (i=0; i<buffsize; i++) dispbuffer[j][i] = 0;
}

VOID pref_beginpass() 		/* Only a single pass for a plotter */
{

	if (pass<0) 	{
		pass = 0;
		starty = PB->MPI->MaxCY;
	} else	{
		pass++;
		dumppass();
	}

	PB->MPI->MinPX = 0;
	PB->MPI->MaxPX = PB->MPI->MaxCX;

	framex = PB->MPI->MaxPX - PB->MPI->MinPX;

	if (starty-abufy<(LONG)PB->MPI->MinCY) PB->MPI->MinPY = PB->MPI->MinCY;
	else	PB->MPI->MinPY = starty - abufy;

	PB->MPI->MaxPY = starty;

	framey = PB->MPI->MaxPY - PB->MPI->MinPY;

	starty = begy = PB->MPI->MinPY;
	
	DEBUG("Pass %d to %d (%d)\n",PB->MPI->MinPY,PB->MPI->MaxPY,framey);

	clearrp();
	SetDrMd(&BR, JAM1);
	SetAPen(&BR, 1);
	SetDrPt(&BR, 0xFFFF);
		
	return;
}

VOID pref_drawellipse(x,y,rx,ry,ba,ea,er) 
ULONG x,y,rx,ry;
STDFLT ba,ea,er;
{ }
