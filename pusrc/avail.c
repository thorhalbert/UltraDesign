/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	avail.c - Determine available free memory
 *
 */

/*
 *	Copyright (C) 1986, 1987 by WURBsoft Inc., and Scott Halbert.
 *
 *	No portion of this product in any form (this includes but is not
 *	limited to code sources, binaries, documentation, data or other), 
 *	may be used, transferred, or kept by any parties other than the 
 *	owners (WURBsoft Inc. and Scott Halbert),  unless the proper 
 *	licenses have been executed between the user and WURBsoft Inc.
 *
 *	Any use not covered by a legal software license is considered 
 *	by us to be Software Piracy, and constitutes a violation of our 
 *	rights.  Don't get us mad.
 *
 */

/*
 *	Modification History
 *
 *	0	11-Jan-87 SH	Create new template
 *	1	11-Jan-87 SH	Beginning of mod history
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "plotsetup.h"
#include <exec/types.h>
#include <exec/exec.h> 
#include <exec/execbase.h> 
#include <graphics/gfxmacros.h>

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

extern struct ExecBase *SysBase;

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

ULONG minmem = 50000;

ULONG AvailMem ();

/* maxsize -- determine the total maximum size for all regions
 *	of the given type.  This code must be executed while
 *	FORBIDDEN (it accesses shared system structures).
 */
ULONG
maxsize (t)
unsigned long t;
{
    /* THIS CODE MUST ALWAYS BE CALLED WHILE FORBIDDEN */
    ULONG size = 0;
    struct MemHeader *mem;
    struct ExecBase *eb = SysBase;

    for (mem = (struct MemHeader *)eb->MemList.lh_Head;
	    mem->mh_Node.ln_Succ;
	    mem = (struct MemHeader *)mem->mh_Node.ln_Succ) {
	if (mem -> mh_Attributes & t) {
	    size += ((ULONG) mem -> mh_Upper - (ULONG) mem -> mh_Lower);
	}
    }

    return size;
}

ULONG TotAvail()
{
    ULONG   avail1,
            avail2;

    Forbid ();
    avail1 = AvailMem (MEMF_CHIP);
    avail2 = AvailMem (MEMF_FAST);
    Permit ();

	return(avail1+avail2);

}

ULONG CHIPAvail()
{
    ULONG   avail1;

    Forbid ();
    avail1 = AvailMem (MEMF_CHIP);
    Permit ();

	return(avail1);

}

ULONG FASTAvail()
{
    ULONG   avail1;

    Forbid ();
    avail1 = AvailMem (MEMF_FAST);
    Permit ();

	return(avail1);

}

BOOL NoughFree()
{

	return(CHIPAvail()>minmem);
	
}

struct Screen *SafeScreen(innew)
struct NewScreen *innew;
{

	ULONG bigin;
    	ULONG   avail1;

	bigin = RASSIZE(innew->Width, innew->Height) * UCADDEPTH;

    	Forbid();
    	avail1 = AvailMem (MEMF_CHIP);
    	Permit();

	printf("%d <? %d %d\n",avail1,minmem,bigin);

	if (avail1<(minmem+bigin)) return(NULL);

	return(OpenScreen(innew));

}

struct Window *SafeWindow(innew)
struct NewWindow *innew;
{

	ULONG bigin;
    	ULONG   avail1;

	bigin = 30000;/*RASSIZE(innew->Width, innew->Height) * UCADDEPTH;*/

    	Forbid();
    	avail1 = AvailMem (MEMF_CHIP);
    	Permit();

	printf("%d <? %d %d\n",avail1,minmem,bigin);

	if (avail1<(minmem+bigin)) return(NULL);

	return(OpenWindow(innew));

}

int SafeRequest(inreq, inwin)
struct Requester *inreq;
struct Window *inwin;
{

	ULONG bigin;
    	ULONG   avail1;

	bigin = RASSIZE(inreq->Width, inreq->Height) * UCADDEPTH;

    	Forbid();
    	avail1 = AvailMem (MEMF_CHIP);
    	Permit();

	printf("%d <? %d %d\n",avail1,minmem,bigin);

	if (avail1<(minmem+bigin)) return(0);
	
	return(Request(inreq, inwin));

}

VOID MemFree(chipfree,fastfree)
ULONG *chipfree,*fastfree;
{
    ULONG   avail1,
            avail2;

    Forbid ();
    avail1 = AvailMem (MEMF_CHIP);
    avail2 = AvailMem (MEMF_FAST);
    Permit ();

	*chipfree = avail1;
	*fastfree = avail2;

	return;

}
