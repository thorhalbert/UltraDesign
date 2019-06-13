/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	Units.c - Angular and Linear Units
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
 *	1	14-Jun-87 SH	Begin Coding 
 *	2	 7-Jul-88 SH	Try to fix architectural units
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

BOOL Ang_DegD() {	angtype = ANG_DEGD;	return(TRUE); }
BOOL Ang_DegM() {	angtype = ANG_DEGM;	return(TRUE); }
BOOL Ang_DegMS() {	angtype = ANG_DEGMS;	return(TRUE); }
BOOL Ang_Rad() {	angtype = ANG_rad;	return(TRUE); }
BOOL Ang_Grad() {	angtype = ANG_Grad;	return(TRUE); }

BOOL Lin_EDI() {	lintype = LIN_EDI;	return(TRUE); }
BOOL Lin_EFI() {	lintype = LIN_EFI;	return(TRUE); }
BOOL Lin_DF() {		lintype = LIN_DF;	return(TRUE); }
BOOL Lin_DI() {		lintype = LIN_DI;	return(TRUE); }
BOOL Lin_MM() {		lintype = LIN_MM;	return(TRUE); }
BOOL Lin_CM() {		lintype = LIN_CM;	return(TRUE); }
BOOL Lin_M() {		lintype = LIN_M;	return(TRUE); }
BOOL Lin_MILE() {	lintype = LIN_MILE;	return(TRUE); }
BOOL Lin_KM() {		lintype = LIN_KM;	return(TRUE); }

#define MAXB 15
LOCAL int nextfree=0;
LOCAL UBYTE DispBuff[MAXB+1][30];

UBYTE *OutFrac(in,pts)		/* In always in mm */
STDFLT in;
int pts;
{

	STDFLT a,b,c;
	int i,j;
	UBYTE ofmt[20],*out;
	
	if (pts<=0) return("");

	out = &DispBuff[nextfree++][0];
	if (nextfree>=MAXB) nextfree=0;

	a = floor(in);
	b = in-a;

	for (a=0,c=1.0; a<pts; a++, c*=10.0);

	b *= c;
	b += .5;	/* Round */
	j = b;
	
	if (b>c) j = 0;

	sprintf(ofmt,".%%0%dd",pts);
	sprintf(out,ofmt,j);

	return(out);
}

UBYTE *OutInt(in,pts)
STDFLT in;
int pts;
{

	STDFLT a,b,c;
	int j,ja;
	UBYTE *out;

	out = &DispBuff[nextfree++][0];
	if (nextfree>=MAXB) nextfree=0;

	a = floor(in);
	b = in-a;

	for (a=0,c=1.0; a<pts; a++, c*=10.0);

	b *= c;
	b += .5;	/* Round */
	j = b;
	
	ja = 0;
	if (b>c) ja = 1;

	j = floor(in);
	j += ja;

	sprintf(out,"%d",j);
	return(out);
}

STDFLT tollst[10] = { 1e0, 1e-1, 1e-2, 1e-3, 1e-4, 1e-5, 1e-6, 1e-7,
		     1e-8, 1e-9 };

UBYTE *DoDecimal(sgn,a,out,pts)
UBYTE *sgn,*out;
STDFLT a;
int pts;
{
	UBYTE test[40];
	STDFLT tst;
	
	if (pts<0) pts = 0;
	if (pts>9) pts = 9;
	
	if (abs(a)<(tollst[pts]/2.0)) a = 0;	/* Too small */

	sprintf(test,"%s%s%s",sgn,OutInt(a,pts),OutFrac(a,pts));
	if (strlen(test)<14) {
		strcpy(out,test);
		return(out);
	}

	sprintf(test,"%s%s",sgn,OutInt(a,pts));
	if (strlen(test)<14) {
		strcpy(out,test);
		return(out);
	}

	sprintf(out,"%s%.5e",sgn,a);
	return(out);
}

UBYTE *DispLin(x)
STDFLT x;
{

	STDFLT a,b,c,d,e,f;
	int j,k,l;
	UBYTE *out,test[50],frc[20],*sgn;
	BOOL neg;

	out = &DispBuff[nextfree++][0];
	if (nextfree>=MAXB) nextfree=0;

	neg=FALSE;
	if (x<0) {
		neg=TRUE;
		x = -x;
	}

	sgn = neg?"-":"";
	switch (lintype) {
	case LIN_EDI:
		a = x * MM_IN;	/* Cvt from mm to inches */
		b = floor(a / 12.0);	/* Number of feet */

		c = a - (b*12.0);	/* Remaining inches */

		sprintf(test,"%s%s'%s%s\"",
			sgn,OutInt(b,linpoints),
				OutInt(c,linpoints),
				OutFrac(c,linpoints));
		if (strlen(test)<14) {
			strcpy(out,test);
			return(out);
		}

		sprintf(test,"%s%s'%s\"",sgn,OutInt(b,linpoints),
				OutInt(c,linpoints));
		if (strlen(test)<14) {
			strcpy(out,test);
			return(out);
		}

		sprintf(test,"%s%s\'",sgn,OutInt(b,linpoints));
		if (strlen(test)<14) {
			strcpy(out,test);
			return(out);
		}

		sprintf(out,"%s%.5e\'",sgn,a/12.0);
		return(out);
	case LIN_EFI:
		a = x * MM_IN;	/* Cvt from mm to inches */
		b = floor(a / 12.0);	/* Number of feet */

		c = a - (b*12.0); /* Remaining inches */

		d = c - floor(c);	/* Get the fraction of inches */
		c = floor(c);

		e = lintolerance * MM_IN;
		if (e<(1.0/64.0)) e = (1.0/64.0);
		
		f = 1.0/e;		/* Denominator */
		j = f;
		k = 1;
		
		while (j>1&&k<64) {	/* Insure the frac was power of 2 */
			k <<= 1;
			j >>= 1;
		}
		
		if (k>64) k = 64;
		if (k<1) k = 1;
		e = 1.0 / ((STDFLT) k);

		f = d / e;	/* Try to find the numerator */
		j = f;			/* Round */

		while (((j&1)==0)&&(k>1)) {
			j >>= 1;
			k >>= 1;
		}
						
		if (k<=1) frc[0]='\0';
		else sprintf(frc," %d/%d",j,k);

		sprintf(test,"%s%s'%s%s\"",sgn,OutInt(b,0),
			OutInt(c,0),frc);
		if (strlen(test)<14) {
			strcpy(out,test);
			return(out);
		}

		sprintf(test,"%s%s'%s\"",sgn,OutInt(b,0),OutInt(c,0));
		if (strlen(test)<14) {
			strcpy(out,test);
			return(out);
		}

		sprintf(test,"%s%s'",sgn,OutInt(b,0));
		if (strlen(test)<14) {
			strcpy(out,test);
			return(out);
		}

		sprintf(out,"%s%.5e'",sgn,a/12.0);
		return(out);
	case LIN_DF:
		a = x * MM_IN;	/* Cvt from mm to inches */
		a /= 12.0;
		DoDecimal(sgn,a,out,linpoints);
		break;
	case LIN_MILE:
		a = x * MM_IN;	/* Cvt from mm to inches */
		a /= 12.0;
		a /= 5280.0;
		DoDecimal(sgn,a,out,linpoints);
		break;
	case LIN_DI:
		a = x * MM_IN;	/* Cvt from mm to inches */
		DoDecimal(sgn,a,out,linpoints);
		break;
	case LIN_M:
		a = x / 1000.0;
		DoDecimal(sgn,a,out,linpoints);
		break;
	case LIN_KM:
		a = x / (1000.0*1000.0);
		DoDecimal(sgn,a,out,linpoints);
		break;
	case LIN_CM:
		a = x / 10.0;
		DoDecimal(sgn,a,out,linpoints);
		break;
	default:
		a = x;
		DoDecimal(sgn,a,out,linpoints);
		break;
	}
}

UBYTE *DispAng(in)		/* In always in rads */
STDFLT in;
{

	STDFLT a,b,c,d;
	UBYTE *out,test[50];

	out = &DispBuff[nextfree++][0];
	if (nextfree>=MAXB) nextfree=0;

	normalize(in);		/* Get straight */

	switch (angtype) {

	case ANG_DEGD:
		a = (in/TWOPI) * 360.0;
		DoDecimal(FALSE,a,test,angpoints);
		sprintf(out,"%s°",test);
		return(out);
	case ANG_DEGM:
		a = (in/TWOPI) * 360.0;
		b = floor(a);		/* Total Degrees */

		c = ((a-b)*60.0);		/* Remaining minutes */

		sprintf(test,"%s°%s%s'",
			OutInt(b,angpoints),
			OutInt(c,angpoints),
			OutFrac(c,angpoints));
		if (strlen(test)<14) {
			strcpy(out,test);
			return(out);
		}

		sprintf(test,"%s°%s'",OutInt(b,angpoints),
			OutInt(c,angpoints));
		return(out);
	case ANG_DEGMS:
		a = (in/TWOPI) * 360.0;
		b = floor(a);		/* Total Degrees */

		c = ((a-b)*60.0);		/* Remaining minutes */
		a = floor(c);		/* Total minutes */

		d = ((c-a)*60.0);	/* Total seconds */
		sprintf(test,"%s°%s'%s%s\"",
			OutInt(b,angpoints),
			OutInt(c,angpoints),
			OutInt(d,angpoints),
			OutFrac(d,angpoints));
		if (strlen(test)<14) {
			strcpy(out,test);
			return(out);
		}

		sprintf(test,"%s°%s'%s\"",OutInt(b,angpoints),
			OutInt(c,angpoints),
			OutInt(d,angpoints));
		if (strlen(test)<14) {
			strcpy(out,test);
			return(out);
		}
		sprintf(out,"%s°%s'",OutInt(b,angpoints),
			OutInt(c,angpoints));
		return(out);
	case ANG_rad:
		DoDecimal(FALSE,in,out,angpoints);
		return(out);
	case ANG_Grad:
		a = (in/TWOPI) * 400.0;
		DoDecimal(FALSE,a,out,angpoints);
		return(out);

	}
}	

/* Input algorithm SA

	Strip all ° ' or " from text, make spaces

	any string of  XXX XX/XX	interpreted as 1 fraction

	feet form
		YYY		Feet
		YYY YYY		Feet Inches

	Degrees
		YYY		Degrees
		YYY YYY		Degrees Minutes
		YYY YYY YYY	Degrees Minutes Seconds

	All other forms accept only one number
*/

LOCAL BOOL	AnsNeg=FALSE;
LOCAL STDFLT	inparts[3];
LOCAL int	totpar=0;

BOOL	ParseIn(instr)
UBYTE *instr;
{

	UBYTE	cutbuf[80];
	UBYTE	par1[30],*in,*out,bf;
	BOOL	insp=TRUE;
	STDFLT	a,b,c;
	int	t;

	/* printf("/%s/ org - ",instr); */

	AnsNeg = FALSE;
	totpar = 0;

	out = cutbuf;
	for (in=instr; *in!=NULL; in++) {
		bf = *in;
		if (bf=='°') bf = ' ';
		if (bf=='\'') bf = ' ';
		if (bf=='"') bf = ' ';
		if (bf==' '&&insp) continue;
		if (bf==' ') insp=TRUE;
		else	insp=FALSE;

		*out++ = bf;
	}
	*out++ = '\0';

	in = cutbuf;

	FOREVER {
		if (*in==' ') in++;
		if (*in=='\0') return(TRUE);

		out = par1;

		for (; *in!=' '&&*in!='\0'; in++) *out++ = *in;
		*out++ = '\0';

		t=sscanf(par1,"%f/%f",&a,&b);
		if (t==2) {
			c = a/b;
			if (c<0.0) {
				c = -c;
				AnsNeg = TRUE;
			}

			/* printf("Fraction %f/%f=%f seen\n",a,b,c); */
			
			if (totpar==0) inparts[totpar++] = c;
			else	inparts[totpar-1] += c;
			continue;
		}

		t=sscanf(par1,"%f",&c);
		if (t==1) {
			if (c<0.0) {
				c = -c;
				AnsNeg = TRUE;
			}

			if (totpar>=3) return(FALSE); /* What is this junk? */
			inparts[totpar++] = c;

			continue;
		}
		return(FALSE);
	}
}

STDFLT ParAng(instr,error)
UBYTE *instr;
BOOL *error;
{
	STDFLT dgs,rad;
	int i;

	*error = FALSE;
	if (!ParseIn(instr)||totpar==0) {
		*error = TRUE;
		return(0.0);
	}
	
	/* for (i=0; i<totpar; i++) {
		printf("Ans %d: %f\n",i,inparts[i]);
	} */

	switch (angtype) {

	case ANG_DEGD:
	case ANG_DEGM:
	case ANG_DEGMS:
		switch(totpar) {
		case 1:
			dgs = inparts[0];
			break;
		case 2:
			dgs = inparts[0] +
				((1.0/60.0)*inparts[1]);
			break;
		case 3:
			dgs = inparts[0] +
				((1.0/60.0)*inparts[1])+
				((1.0/3600.0)*inparts[2]);
			break;
		}

		rad=(dgs/360.0)*TWOPI;
		break;
	case ANG_rad:
		if (totpar!=1) {
			*error = TRUE;
			return(0.0);
		}
		rad = inparts[0];
		break;
	case ANG_Grad:
		if (totpar!=1) {
			*error = TRUE;
			return(0.0);
		}
		rad = (inparts[0]/400.0)*TWOPI;
		break;
	}

	if (AnsNeg) rad = -rad;
	normalize(rad);
	return(rad);
}

STDFLT ParLin(instr,error)
UBYTE *instr;
BOOL *error;
{
	STDFLT fts,mm;
	int i;

	*error = FALSE;
	if (!ParseIn(instr)||totpar==0) {
		*error = TRUE;
		return(0.0);
	}

	/* for (i=0; i<totpar; i++) {
		printf("Ans %d: %f\n",i,inparts[i]);
	} */

	switch (lintype) {

	case LIN_EDI:
	case LIN_EFI:
	case LIN_DF:
		switch(totpar) {
		case 1:
			fts = inparts[0];
			break;
		case 2:
			fts = inparts[0] +
				((1.0/12.0)*inparts[1]);
			break;
		case 3:
			*error = TRUE;
			return(0.0);
		}
		fts *= 12;		/* Cvt to inches */
		mm = fts / MM_IN;	/* Cvt to mm */
		break;
	case LIN_DI:	
		if (totpar!=1) {
			*error = TRUE;
			return(0.0);
		}
		mm = inparts[0] / MM_IN;	/* Convert */
		break;
	case LIN_MILE:	
		if (totpar!=1) {
			*error = TRUE;
			return(0.0);
		}
		mm = (inparts[0] * 12.0 * 5280.0) / MM_IN;	/* Convert */
		break;
	case LIN_MM:
		if (totpar!=1) {
			*error = TRUE;
			return(0.0);
		}
		mm = inparts[0];
		break;
	case LIN_CM:
		if (totpar!=1) {
			*error = TRUE;
			return(0.0);
		}
		mm = inparts[0] * 10.0;
		break;
	case LIN_M:
		if (totpar!=1) {
			*error = TRUE;
			return(0.0);
		}
		mm = inparts[0] * 1000.0;
		break;
	case LIN_KM:
		if (totpar!=1) {
			*error = TRUE;
			return(0.0);
		}
		mm = inparts[0] * 1000.0 * 1000.0;
		break;
	}

	if (AnsNeg) mm = -mm;

	return(mm);
}
