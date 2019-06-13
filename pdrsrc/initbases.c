#include <driversetup.h>

VOID 	*DosBase = NULL,
	*GfxBase = NULL,
	*IntuitionBase = NULL,
	*SysBase = NULL;

VOID ExpungeBases()
{

	if (DosBase!=NULL) 	_CloseLibrary(DosBase);
	if (GfxBase!=NULL)	_CloseLibrary(GfxBase);
	if (IntuitionBase!=NULL) _CloseLibrary(IntuitionBase);

	return;
}

BOOL InitBases()
{
	int a;

#asm
	move.l	(sp)+,a6
	move.l	4,a6
	move.l	a6,_SysBase
	move.l	a6,-(sp)
#endasm

	a = 0;

	DosBase = _OpenLibrary("dos.library",0);
	if (DosBase==NULL) a = 1;

	GfxBase = _OpenLibrary("graphics.library",0);
	if (GfxBase == NULL) a = 1;

	IntuitionBase = _OpenLibrary("intuition.library",0);
	if (IntuitionBase==NULL) a = 1;

	if (a==1) ExpungeBases();

	return(!a);
}
