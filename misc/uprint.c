#include <math.h>
#include <exec/types.h>
#include <exec/exec.h>
#include <libraries/dos.h>
#include <libraries/dosextens.h>
#include <stdio.h>

/*  DETAB - program to remove tabs (intervals of 8) and substitute spaces */

#define MAXLIN 200

int tabs[MAXLIN];

void settab()
{

   int i;

   for (i=1; i<=MAXLIN; i++) {
      if (i%8==1) tabs[i]=TRUE;
      else  tabs[i]=FALSE;
   }

   return;

}

int tabpos(col)
{
   if (col>MAXLIN) return(TRUE);
   return(tabs[col]);
}

int pgncnt=0,lincnt=99;
char *ifn,*ofn;
FILE *ifile,*ofile;
char fcopy[80];

void main(argc,argv)
int argc;
char *argv[];
{

   int col,a;
   char c;

	ofn="prt:";

	switch(argc) {
	case 2:
		ifn=argv[1];
		break;
/*	case 3:
		ifn=argv[1];
		ofn=argv[2];
		break;	*/
	default:
		printf("Usage: uprint file {printer}\n");
		exit(1);
	}

	printf("uprint printing file %s to %s\n",ifn,ofn);

	sprintf(fcopy,"%s",ifn);
	ifn=fcopy;

	ifile=fopen(ifn,"r");
	if (ifile==NULL) {
		printf("Cannot open input file %s\n",ifn);
		exit(1);
	}

	ofile=fopen(ofn,"w");
	if (ofile==NULL) {
		printf("Cannot open output file %s\n",ofn);
		exit(1);
	}

	fprintf(ofile,"\033[2w\033[0z\033[88t\r");
		/* Set elite - 8LPI - 88 lines per page */

   settab();
   col=1;

	for (a=0; a<12; a++) putout(' ');
   while ((c=getc(ifile))!=EOF) {

	while (TRUE) {

		if (c=='\t') break;
		if (c=='\n') break;

		if (c=='\f') {
			c='\n';
			break;
		}

		if (c>=' ') break;
		c=0;

		break;
	}
      switch(c) {
         case '\t':
            do {
               putout(' ');
               col++;
            } while (tabpos(col)==FALSE);
            break;
         case '\n':
            putout(c);
	    lincnt++;
		for (a=0; a<12; a++) putout(' ');
            col=1;
            break;
	 case '\0':
		break;
         default:
            putout(c);
            col++;
		if (col>88) {
	            putout(c);
		    lincnt++;
			for (a=0; a<20; a++) putout(' ');
	            col=8;
	            break;
		}
      }

   }

	fputs("\r\f\r\r\r\r\r\r",ofile);
	fclose(ofile);
	fclose(ifile);

}

putout(ochr)
char ochr;
{

	if (lincnt>76) {
		if (pgncnt>0) fputs("\n\f",ofile);
		pgncnt++;

		fprintf(ofile,"\033[1m\r");
		fprintf(ofile," *** %60.60s  ",fcopy);
		prt();
		fprintf(ofile,"  Page%4d\n\n\n\n\n\n\n",pgncnt);
		fprintf(ofile,"\033[0m\r");

		lincnt=8;
	}

	fputc(ochr,ofile);

}


int day_month[]= {                       /* Needed for dotexttime()      */
   0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

int Cyear,Cmonth,Cday,Cdpm,Chours,Cminutes,Cseconds;

void GetCurTime()
{

	struct DateStamp dss;

	DateStamp(&dss);

	Cyear=1978;		/* AMIGA calendar epoch */

	Cday=dss.ds_Days;
	while (Cday>=366) {
		if (((Cyear-1976)%4)==0) {
			Cday-=366;
			Cyear++;
		} else if ((Cyear-1976)%4 != 0 && Cday >=365) {
			Cday -=365;
			Cyear++;
		}
	}
	Cdpm=365;
	if ((Cyear-1976)%4==0) {
		day_month[2]=29;
		Cdpm=366;
	}
	Cday=Cday%Cdpm;
	for (Cmonth=1;Cday>day_month[Cmonth]; Cmonth++)
		Cday -= day_month[Cmonth];

	Cday++;

	Cseconds=dss.ds_Tick / TICKS_PER_SECOND;
	Cseconds %= 60;
	Cminutes = dss.ds_Minute;
	Chours = Cminutes/60;
	Cminutes %= 60;

	day_month[2]=28;

	return;
}

#define CAL_CONS 1720982L
#define DAY_CONS 1L

long julday(year,mon,day)
int year,mon,day;
{

	long julian,yearprime,monprime,dayprime;

	yearprime=year;
	monprime=mon+1;
	dayprime=day;

	if (mon==1 || mon==2) {
		yearprime=year-1;
		monprime=mon+13;
	}

	julian=dayprime+CAL_CONS;
	julian+=(36525L*yearprime)/100L;
	julian+=(306001L*monprime)/10000L;

	return(julian);
}

int dow(inj)
long inj;
{
	inj+=DAY_CONS;
	return(inj%7);
}

char	*months[]=
{ "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" };
char 	*days[]=
{ "Mon","Tue","Wed","Thu","Fri","Sat","Sun" };

prt()
{

	long julres,today,bigpart,littlepart;
	int epoch,myear,mday,mhour,mminute,msecond;
	double fulljul;

	GetCurTime();
	julres=julday(Cyear,Cmonth,Cday);

	fprintf(ofile,"%s %d-%s-%04d",days[julres%7],Cday,months[Cmonth-1],Cyear);
		
}


