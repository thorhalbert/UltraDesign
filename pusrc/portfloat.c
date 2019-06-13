/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	portfloat.c - Floating point access/setup
 *
 */

/*
 *	Copyright (C) 1987, 1988 by WURBsoft Inc., and Scott Halbert.
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
 *	1	23-Jan-88 SH	Get routines out of iffput0
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "plotsetup.h"

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

/*--------------------------Support Subroutines--------------------------*/

#ifdef F_FFP
STDFLT ldexpFFP();
#define LDEXP ldexpFFP
STDFLT frexpFFP();
#define FREXP frexpFFP
#else
#define FREXP frexp
#define LDEXP ldexp
#endif

VOID setzero(inport)
PORTFLOAT *inport;
{

	inport->mantissa=0;
	inport->exponent=0;


	return;

}
STDFLT PORTtoF(inport)
PORTFLOAT *inport;
{

	STDFLT out,loader;

	loader=inport->mantissa;

	out=LDEXP(loader,inport->exponent-31);	

	return((STDFLT) out);
}

VOID FtoPORT(infloat, outport)
STDFLT infloat;
PORTFLOAT *outport;
{

	int exp;
	STDFLT frac,in;

	in=infloat;
	frac=FREXP(in,&exp);

	outport->exponent=exp;

	in=LDEXP(frac,31);	

	outport->mantissa=in;

	return;
}

#ifdef F_FFP
STDFLT ldexpFFP(infrac, newexp)
STDFLT infrac;
int newexp;
{

	int oldfrac;

	union {
		STDFLT	inflt;
		UBYTE	fltmn[4];
	} manip;

	if (infrac==0.0) return(0.0);		/* Can't improve it */

	manip.inflt = infrac;
	oldfrac = (manip.fltmn[3]&127) - 64;	/* Get the exponent */

	newexp += oldfrac;
	newexp += 64;
	if (newexp<0) newexp = 0;
	if (newexp>127) newexp = 127;

	manip.fltmn[3] &= (~127);
	manip.fltmn[3] |= newexp;		/* Put the new exponent in */

/*	printf("frac=%g exp=%d out=%g\n",infrac,newexp-64,manip.inflt); */

	return(manip.inflt);		/* Return the modified part */

}

STDFLT frexpFFP(infltp,outexp)
STDFLT infltp;
int *outexp;
{
	union {
		STDFLT	inflt;
		UBYTE	fltmn[4];
	} manip;

	manip.inflt = infltp;
	*outexp = (manip.fltmn[3]&127) - 64;	/* Get the exponent */

	manip.fltmn[3] &= (~127);
	manip.fltmn[3] |= 64;		/* Put in zero exponent */

/*	printf("frexp(%g) = %g exp %d\n",infltp,manip.inflt,*outexp); */

	return(manip.inflt);
}

/* double floor(x)
double x;
{
	LONG y;

	y = x;

	return((double) y);
} */

#define HALFPI (PI/2.0)

double dsign(x,y)
double x;
double y;
{
    if (x > 0.0) {
	if (y > 0.0) {
	    return (x);
	} else {
	    return (-x);
	}
    } else {
	if (y < 0.0) {
	    return (x);
	} else {
	    return (-x);
	}
    }
}

double atan2(y,x)
double x;
double y;
{
    double dsign(), atan();

    if (x == 0.0) {
	return (dsign(HALFPI,y));
    } else if (x > 0.0) {
	return (atan(y/x));
    } else {
	return (atan(y/x) + dsign(PI,y));
    }
}

#endif
