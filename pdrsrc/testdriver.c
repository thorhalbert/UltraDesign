#include "plotsetup.h"

struct PlotterSegment *plothook;
APTR	inhook;

struct main_interface	maininter;
struct plotter_base	mainbase;
struct plot_info	maininfo;

UBYTE *BPTR_TO_CPTR(inptr)
APTR	inptr;
{

	ULONG	med;

	med = (ULONG) inptr;
	med <<= 2;
	inptr = (APTR) med;

	return((UBYTE *) med);
}

VOID writeout(buf,len)
UBYTE *buf;
int len;
{
	fwrite(buf,len,1,stdout);
}

main()
{

	inhook = LoadSeg("roland.pdr");
	if (inhook==0) {
		printf("Loadseg failed\n");
		return;
	}

	plothook = (struct PlotterSegment *) BPTR_TO_CPTR(inhook);

	printf("Loaded roland.pdr----interface.numbsumbs=%d\n",
		plothook->PLI.numbsubs);

	mainbase.PLI = &plothook->PLI;	
	mainbase.MNI = &maininter;
	mainbase.MPI = &maininfo;

	maininter.dataout = writeout;		/* Data output routine */

	(*mainbase.PLI->initdriver)(&mainbase);

	(*mainbase.PLI->dopreset)(0);
	(*mainbase.PLI->initplot)();
	
	(*mainbase.PLI->movetopoint)(10,10);
	(*mainbase.PLI->drawaline)(10,100);
	(*mainbase.PLI->drawaline)(100,100);
	(*mainbase.PLI->drawaline)(100,10);
	(*mainbase.PLI->drawaline)(10,10);

	(*mainbase.PLI->endplot)();
	(*mainbase.PLI->expungedriver)();

	UnLoadSeg(inhook);		/* Unload code */
}
