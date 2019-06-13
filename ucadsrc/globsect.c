/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	globsects.c - define global sections
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
 *	1 	13-Sep-87 SH	Split out from everywhere
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

#undef DEFCLR
#undef DEFMOD

#define DEFCLR(mesnam,mestype,messtr) struct mesdef mesnam = {mestype,messtr}
#define DEFMOD(varnam,varstr) char *varnam = (UBYTE *) varstr

#include "cleartext.h"

#undef DEFGLB
#undef DEFGLI

#define DEFGLB(a,b) a b
#define DEFGLI(a,b,c) a b = c

#include "globals.h"

#ifdef EXECPHAND
matherr(in)
struct exception *in;
{

	printf("Received some math error %d %s(%e,%e)=%e\n",in->type,in->name,
		in->arg1,in->arg2,in->retval);

	return(0);
}

double except(type,name,arg1,arg2,retval)
int type;
char *name;
double arg1,arg2,retval;
{

	printf("Got error %d in %s(%e,%e)=%e\n",type,name,arg1,arg2,retval);
	return(retval);
}

#endif
