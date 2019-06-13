#include "driversetup.h"

extern	struct	plotter_base	*PB;

VOID plotf(pmask,d1,d2,d3,d4,d5,d6)
{

	UBYTE	obuf[255];

	sprintf(obuf,pmask,d1,d2,d3,d4,d5,d6);

	(*PB->MNI->dataout)(obuf,strlen(obuf));	/* Do data out */

}
