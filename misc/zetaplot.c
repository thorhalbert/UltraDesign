#include <stdio.h>
#include <math.h>
#include <viewiff.h>
#define STDFLT float

#define TWOPI 6.2831853
extern int Enable_Abort;

#define PLT_PX 0x01
#define PLT_MX 0x02
#define PLT_PY 0x04
#define PLT_MY 0x08
#define PLT_PU 0x10
#define PLT_PD 0x20

#define PENPAUSE	for(i=0; i<30; i++) putc(0, PlotterDevice);

int curx,cury;
FILE *PlotterDevice;
int plotpen,maxx,marx;

PlotSetup()
{

	int i;

	PlotterDevice=fopen("par:","w");
	if (PlotterDevice==NULL) {
		printf("Couldn't open par:\n");
		exit(2);
	}

	putc(0,PlotterDevice);
	putc(PLT_PD,PlotterDevice);
	PENPAUSE;
	putc(PLT_PU,PlotterDevice);
	PENPAUSE;

	for (i=0; i<2200; i++) 
		putc(PLT_MY, PlotterDevice);

	putc(0,PlotterDevice);		/* Device Reset byte */

	curx=0;
	cury=0;
	plotpen=PLT_PU;

}
void calplot(run,rise,runstep,risestep)
int run,rise,runstep,risestep;
{

#define MAXDEP 10

	int t1,t2,t3,t4,limit,use,d1,d2,d3,op;
	int darr[MAXDEP+1],dlef[MAXDEP+1];

	limit=1;
	t2=0;
	d2=rise;
	d1=run;

	if (rise!=0) {
		for (t3=0; t3<MAXDEP; t3++) {
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
		use=0;
		for (t3=0; t3<t2; t3++) {
			if (++dlef[t3]<darr[t3]) break;
			dlef[t3]=0;
			use=(use)?0:1;
		}
		op=runstep;
		switch(runstep) {
			case PLT_PX: curx++;	break;
			case PLT_MX: curx--;	break;
			case PLT_PY: cury++;	break;
			case PLT_MY: cury--;	break;
		}
		if (use) {
			op|=risestep;
			switch(risestep) {
				case PLT_PX: curx++;	break;
				case PLT_MX: curx--;	break;
				case PLT_PY: cury++;	break;
				case PLT_MY: cury--;	break;
			}
		}
		putc(op, PlotterDevice);

	}
	return;
}

void PlotGoto(x,y)
int x,y;
{

	int dx,dy,quad,t1,runstep,risestep;
	
	dx=x-curx;
	dy=y-cury;

	quad=0;
	if (dy<0) {
		quad|=4;
		dy=-dy;
	}
	if (dx<0) {
		quad|=2;
		dx=-dx;
	}
	if (abs(dy)>abs(dx)) {
		quad|=1;
		t1=dx;
		dx=dy;
		dy=t1;
	}

	switch (quad) {
		case 0: case 4:	runstep=PLT_PX;	break;
		case 1: case 3:	runstep=PLT_PY; break;
		case 2: case 6:	runstep=PLT_MX;	break;
		case 5: case 7:	runstep=PLT_MY; break;
	}
	switch (quad) {
		case 1: case 5:	risestep=PLT_PX; break;
		case 3: case 7:	risestep=PLT_MX; break;
		case 0: case 2:	risestep=PLT_PY; break;
		case 4: case 6:	risestep=PLT_MY; break;
	}

	calplot(dx,dy,runstep,risestep);
}


void PlotPen(pen)
int pen;
{

	int i;

	if (pen==plotpen) return;
	pen&=0x30;

	putc(pen,PlotterDevice);
	PENPAUSE;

	plotpen=pen;

}
void PlotClose()
{

	if (PlotterDevice==NULL) return;
	PlotPen(PLT_PU);
	putc(0,PlotterDevice);			/* Let it idle */
	fclose(PlotterDevice);
}

STDFLT multipl;
void main()
{

	maxx=0; 
	marx=100;

	Enable_Abort=1;

	PlotSetup();

	getview();

	PlotClose();

}
	
STATIC IFFP ifferror=0;

#define CkErr(expression) { if (ifferror==IFF_OKAY) ifferror=(expression);}
#define div32(x) ((x)>>5)
#define rem32(x) ((x)&0x1F)

void	FtoPORT();

IFFP IffErr()
{
	IFFP i;
	i=ifferror;
	ifferror=0;
	return(i);
}

IFFP iffp;

void setzero(inport)
PORTFLOAT *inport;
{

	inport->mantissa=0;
	inport->exponent=0;

	return;

}
STDFLT PORTtoF(inport)
PORTFLOAT *inport;
{

	double out,loader,ldexp();

	loader=inport->mantissa;
	out=ldexp(loader,inport->exponent-31);

	return((STDFLT) out);
}

void FtoPORT(inSTDFLT, outport)
STDFLT inSTDFLT;
PORTFLOAT *outport;
{

	int exp;
	double frac,in,frexp();

	in=inSTDFLT;
	frac=frexp(in,&exp);

	outport->exponent=exp;
	
	in=ldexp(frac,31);

	outport->mantissa=in;

	return;
}

/*---------------Read iff view file-----------------------------------------*/

STDFLT xviewMIN,yviewMIN,xviewMAX,yviewMAX,snap,offset,skew;
STDFLT xviewbegin,yviewbegin,xviewsize,yviewsize;

IFFP loadLIMS(groupin)
GroupContext *groupin;
{

	struct _st_lims o_lims;

	iffp=IFFReadBytes(groupin,(BYTE *) &o_lims, sizeof(struct _st_lims));
	CheckIFFP();

	xviewMIN=PORTtoF(&o_lims.minx);
	yviewMIN=PORTtoF(&o_lims.miny);
	xviewMAX=PORTtoF(&o_lims.maxx);
	yviewMAX=PORTtoF(&o_lims.maxy);

	printf("	x min %f max %f - y min %f max %f\n",
		xviewMIN,xviewMAX,yviewMIN,yviewMAX);

	snap=PORTtoF(&o_lims.snap);
	offset=PORTtoF(&o_lims.offset);
	skew=PORTtoF(&o_lims.skew);

	printf("	snap %f offset %f skew %f\n",snap,offset,skew);

	fullzoomout();

	return(IFF_OKAY);
}

IFFP ProcU2VI(groupin)
GroupContext *groupin;
{
	GroupContext thiscontext;

	printf("Form U2VI seen\n");

	iffp=OpenRGroup(groupin, &thiscontext);
	CheckIFFP();

	do
		switch (iffp=GetF1ChunkHdr(&thiscontext)) {

		case COPY:
			printf("	COPY chunk seen\n");
			break;

		case LIMS:
			printf("	LIMS chunk seen\n");
			iffp=loadLIMS(&thiscontext);
			CheckIFFP();
			break;

		case LYER:
			printf("	LYER chunk seen\n");
			break;

		case LBRY:
			printf("	LBRY chunk seen\n");
			break;

		} while (iffp >= IFF_OKAY);
	
}

struct {
	BOOL plotted;
	STDFLT x1,y1,x2,y2;
} Arranger[2048];
int nexSTDFLT=0;

/*---------------Read iff view file-----------------------------------------*/

IFFP loadVECT(groupin)
GroupContext *groupin;
{

	struct _st_vect o_vect;
	int  looper;
	STDFLT x1,y1,x2,y2;

	nexSTDFLT=0;

	printf("	%d bytes read in",ChunkMoreBytes(groupin));
	iffp=IFFReadBytes(groupin,(BYTE *) &o_vect, ChunkMoreBytes(groupin));
	CheckIFFP();

	printf("	quan is %d\n",o_vect.vectorquan);

	for (looper=0; looper<o_vect.vectorquan; looper+=2) {
		x1=PORTtoF(&(o_vect.vectbundles[div32(looper)].
			pairsets[rem32(looper)].linepairx));	
		y1=PORTtoF(&(o_vect.vectbundles[div32(looper)].
			pairsets[rem32(looper)].linepairy));	
		x2=PORTtoF(&(o_vect.vectbundles[div32(looper)].
			pairsets[rem32(looper+1)].linepairx));	
		y2=PORTtoF(&(o_vect.vectbundles[div32(looper)].
			pairsets[rem32(looper+1)].linepairy));	
				
		/* printf("x1=%f y1=%f x2=%f y2=%f\n",x1,y1,x2,y2); */

		CreateLine(x1,y1,x2,y2);
	}		

	PlotEmAll();
	return(IFF_OKAY);
}


IFFP loadCIRC(groupin)
GroupContext *groupin;
{

	struct _st_circ o_circ;
	int  looper;
	STDFLT cenx,ceny,radx,rady,rot,angs,ange;

	printf("	%d bytes read in",ChunkMoreBytes(groupin));
	iffp=IFFReadBytes(groupin,(BYTE *) &o_circ, ChunkMoreBytes(groupin));
	CheckIFFP();

	printf("	quan is %d\n",o_circ.circlequan);

	for (looper=0; looper<o_circ.circlequan; looper++) {
		
		cenx=PORTtoF(&(o_circ.circbundles[looper].centx));
		ceny=PORTtoF(&(o_circ.circbundles[looper].centy));
		radx=PORTtoF(&(o_circ.circbundles[looper].radiusx));
		rady=PORTtoF(&(o_circ.circbundles[looper].radiusy));
		rot=PORTtoF(&(o_circ.circbundles[looper].rotation));
		angs=PORTtoF(&(o_circ.circbundles[looper].starta));
		ange=PORTtoF(&(o_circ.circbundles[looper].enda));

		CreateEllipse(cenx,ceny,radx,rady,rot,angs,ange);

	}		

	return(IFF_OKAY);
}

IFFP ProcGRUP(groupin)
GroupContext *groupin;
{
	GroupContext thiscontext;

	printf("Form GRUP seen\n");

	iffp=OpenRGroup(groupin, &thiscontext);
	CheckIFFP();

	do
		switch (iffp=GetF1ChunkHdr(&thiscontext)) {

		case GPID:
			printf("	GPID chunk seen\n");
			break;

		case CLAY:
			printf("	CLAY chunk seen\n");
			break;

		case CLIB:
			printf("	CLIB chunk seen\n");
			break;

		case VECT:
			printf("	VECT chunk seen\n");
			iffp=loadVECT(&thiscontext);
			CheckIFFP();
			break;

		case CIRC:
			printf("	CIRC chunk seen\n");
			iffp=loadCIRC(&thiscontext);
			CheckIFFP();
			break;

		case TEXT:
			printf("	TEXT chunk seen\n");
			break;

		case ELIB:
			printf("	ELIB chunk seen\n");
			break;

		} while (iffp >= IFF_OKAY);
	
}

IFFP GetForms(groupin)
GroupContext *groupin;
{

	switch (groupin->subtype) {

	case U2VI:	return(ProcU2VI(groupin));
	case GRUP:	return(ProcGRUP(groupin));
	default:	return(IFF_OKAY);
	}

}

IFFP getview()
{

	BPTR	infile;
	IFFP	retiffp;
	long	errret;

	ClientFrame mainframe;

	infile=GOpen("tempiff.view",MODE_OLDFILE);
	if (infile==0) {
		errret=IoErr();
		printf("Cannot open output file - dos error %ld\n",errret);
		return(NO_FILE);
	}

	mainframe.getList=SkipGroup;
	mainframe.getProp=SkipGroup;

	mainframe.getForm = GetForms;
	mainframe.getCat=SkipGroup;

	retiffp=ReadIFF(infile, &mainframe);
	printf("Iff status %ld\n",retiffp);

	GClose(infile);

	return(retiffp);

}

STDFLT lastfx=0.0,lastfy=0.0;

PlotEmAll()
{

	STDFLT tdist,dist1,tc1;
	BOOL flipped;
	int object,looper;

	while (TRUE) {

		object=-1;
		dist1=1e30;
		flipped=FALSE;

		for (looper=0; looper<nexSTDFLT; looper++) {

			if (Arranger[looper].plotted) continue;
			tc1=lastfx-Arranger[looper].x1;
			tc1*=tc1;
			tdist=tc1;
			tc1=lastfy-Arranger[looper].y1;
			tc1*=tc1;
			tdist+=tc1;
			if (tdist<dist1) {
				flipped=FALSE;
				object=looper;
				dist1=tdist;
			}
			tc1=lastfx-Arranger[looper].x2;
			tc1*=tc1;
			tdist=tc1;
			tc1=lastfy-Arranger[looper].y2;
			tc1*=tc1;
			tdist+=tc1;
			if (tdist<dist1) {
				flipped=TRUE;
				object=looper;
				dist1=tdist;
			}
		}

		if (object==-1) break;	/* Nothing left */

		if (flipped) {
			PlotLine(Arranger[object].x2,Arranger[object].y2,
				 Arranger[object].x1,Arranger[object].y1);
			lastfx=Arranger[object].x1;
			lastfy=Arranger[object].y1;
		} else {
			PlotLine(Arranger[object].x1,Arranger[object].y1,
				 Arranger[object].x2,Arranger[object].y2);
			lastfx=Arranger[object].x2;
			lastfy=Arranger[object].y2;
		}

		Arranger[object].plotted=TRUE;

	}

	return;
}

CreateLine(x1,y1,x2,y2)
STDFLT x1,y1,x2,y2;
{

	Arranger[nexSTDFLT].plotted=FALSE;

	Arranger[nexSTDFLT].x1=x1;
	Arranger[nexSTDFLT].x2=x2;
	Arranger[nexSTDFLT].y1=y1;
	Arranger[nexSTDFLT].y2=y2;
	nexSTDFLT++;
}

int lastx,lasty;

PlotLine(x1,y1,x2,y2)
STDFLT x1,y1,x2,y2;
{

	STDFLT t1,t;
	int ix,iy;

	t1=x1-xviewbegin;
	t=t1/xviewsize;
	t*=(17*200);
	ix=t;

	t1=y1-yviewbegin;
	t=t1/yviewsize;
	t*=(11*200);
	iy=t;
	
/*	printf("%d,%d to ",ix,iy); */

	if (ix!=lastx||iy!=lasty) {
		PlotPen(PLT_PU);
		PlotGoto(ix,iy);
	}

/*	printf("%d,%d\n",ix,iy); */

	t1=x2-xviewbegin;
	t=t1/xviewsize;
	t*=(17*200);
	ix=t;

	t1=y2-yviewbegin;
	t=t1/yviewsize;
	t*=(11*200);
	iy=t;
	
	PlotPen(PLT_PD);
	PlotGoto(ix,iy);

	lastx=ix;
	lasty=iy;

}

fullzoomout()
{

	STDFLT	borderx,centerx,bordery,centery,scaler;
	STDFLT	screenx,screeny;

	borderx=xviewMAX-xviewMIN;
	centerx=xviewMIN+(borderx/2.0);

	bordery=yviewMAX-yviewMIN;
	centery=yviewMIN+(bordery/2.0);

	screenx=200*17;			/* Scale up to 17*11 inch paper */
	screeny=200*11;

	scaler=screenx/screeny;

	if ((borderx/bordery)<scaler) {		/* Narrower than screen */
		borderx=bordery*scaler;
	} else {
		bordery=borderx/scaler;
	}

/*	borderx*=1.1;
	bordery*=1.1; */		/* Scale down by factor */

	xviewbegin=centerx-(borderx/2.0);
	yviewbegin=centery-(bordery/2.0);

	xviewsize=borderx;
	yviewsize=bordery;

	printf("       X Beg %f Size %f, Y Beg %f Size %f\n",
		xviewbegin,xviewsize,yviewbegin,yviewsize);

}
STDFLT ConvEllipNotation(inang,radiusx,radiusy)
STDFLT inang,radiusx,radiusy;
{

	STDFLT inslope,PID34,inter;

	PID34=PI;
	PID34+=PID2;

	if (inang==PID2) return(PID2);
	if (inang==PID34) return(PID34);

	inslope=tan(inang);

	inter=sqrt((radiusx*radiusx*radiusy*radiusy)/
		((radiusy*radiusy)+(radiusx*radiusx*inslope*inslope)))
			/radiusx;

	if (inang<=PID2) return(acos(inter));
	if (inang<=PI) return(acos(-inter));
	if (inang<=PID34) return(TWOPI-acos(-inter));
	return(TWOPI-acos(inter));
		
}

CreateEllipse(cenx,ceny,radx,rady,rot,angs,ange)
STDFLT cenx,ceny,radx,rady,rot,angs,ange;
{

	STDFLT xn,yn,xn1,yn1,sinth,costh,theta,difang,tx1,ty1,tx2,ty2;
	WORD ctr,qp;

	angs=ConvEllipNotation(angs,radx,rady);
	ange=ConvEllipNotation(ange,radx,rady);

	difang=ange-angs;
	if (difang<=0) difang+=TWOPI;

	qp=600;
	xn=qp;
	
	theta=difang/xn;

	sinth=sin(theta);
	costh=cos(theta);

	xn=cos(angs);
	yn=sin(angs);

	for (ctr=0; ctr<qp; ctr++) {

		xn1=(xn*costh)-(yn*sinth);
		yn1=(yn*costh)+(xn*sinth);

		tx1=cenx+(xn*radx);
		ty1=ceny+(yn*rady);
		tx2=cenx+(xn1*radx);
		ty2=ceny+(yn1*rady);

		PlotLine(tx1,ty1,tx2,ty2);

		xn=xn1;
		yn=yn1;

	}

}
