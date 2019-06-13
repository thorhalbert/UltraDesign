#include "ucad.h"
#include <exec/memory.h>
#include <ctype.h>

#ifdef F_FFP
STDFLT ldexpFFP();
#define LDEXP ldexpFFP
STDFLT frexpFFP();
#define FREXP frexpFFP
#else
#define FREXP frexp
#define LDEXP ldexp
#endif

FILE *intro;
BOOL ateof=FALSE;
LONG	recount;


STDFLT imult = 25.4;

STDFLT xmin,ymin,xmax,ymax;

#define INTRO_MAGIC 1234500

STDFLT FFPTieee(inv)
ULONG inv;
{
	ULONG mantissa,exp;
	STDFLT ans,mant;

	if (inv==0) return(0.0);	/* Clean zero */
			
	mantissa = (inv>>8) & 0x00FFFFFF;
	if (inv&0x80) mantissa = - mantissa;
	
	exp = (inv&0x7F) - 64;	/* Excess 64 notation */
	mant = mantissa;
	
	ans = LDEXP(mant, exp-24);
	
	return(ans);	
}

BOOL getpiece(len,buffer)
int len;
VOID *buffer;
{

	recount = fread(buffer, 1, len, intro);
	if (recount==0) {
		if (feof(intro)) {
			ateof = TRUE;
			return(TRUE);
		}
		if (ferror(intro)) return(FALSE);
		printf("Got a recount 0????\n");
		return(FALSE);
	}
	if (recount!=len) {
		printf("File corrupted\n");
		return(FALSE);
	}
	
	return(TRUE);
}

UBYTE transmap[16] = { 0,1,2,3,6,7,8,9,10,11,12,13,14,15,6,7 };

VOID setupcurrents(color,linetype,thick,layer)
{

	UBYTE	Penname[50];
	UBYTE	Layername[50];
	UBYTE	usecolor;
	UWORD	usepat;
	
	LAYER_A	*ucadlayer;
	PEN_A	*ucadpen;
			
	usecolor = transmap[color];
	
	sprintf(Penname,"C%x_L%x%s",color,linetype,
		(thick&0x1)?"_Thick":"_Normal");
		
	sprintf(Layername,"Layer_%x",layer);
	
	ucadlayer = Layer_Search(Layername);
	if (ucadlayer==NULL) {

		ucadlayer = (LAYER_A *) 
				GI_DrawAlloc(sizeof(LAYER_A));
	
		if (ucadlayer==NULL) {
			printf("No memory to save layer id %s",Layername);
			return;
		}
			
		ucadlayer->Layer_ID = 0;
		strcpy(ucadlayer->Layer_Name,Layername);
		ucadlayer->Color_ID = usecolor;
		ucadlayer->Layer_Flags = 0;
		ucadlayer->Use_Count = 0;
		ucadlayer->Comment = "Intro Layer";

		Link_Layer(ucadlayer);
		
	}
	
	Current_Layer = ucadlayer;

	ucadpen = Pen_Search(Penname);

	if (ucadpen==NULL) {

		ucadpen = (PEN_A *) 
				GI_DrawAlloc(sizeof(PEN_A));

		if (ucadpen==NULL) {
			printf("No memory to save pen ");
			return;
		}
	
		ucadpen->Pen_ID = 0;
		strcpy(ucadpen->Pen_Name,Penname);
		ucadpen->Pen_Flags = 0;
		usepat = 0xFFFF;
		switch (linetype) {
		case 0:	usepat = 0xFFFF;	break;
		case 1:	usepat = 0xCCCC;	break;
		case 2:	usepat = 0xFF00;	break;
		case 3: usepat = 0xFFF0;	break;
		case 4:	usepat = 0xFFCC;	break;
		case 5:	usepat = 0xCC3C;	break;
		case 6:	usepat = 0xFCCC;	break;
		}
		ucadpen->Line_Pattern = usepat;
		ucadpen->Hard_Color = 0;
		ucadpen->Use_Count = 0;
		ucadpen->pen_next = NULL;
		ucadpen->pen_prev = NULL;
		ucadpen->Comment = "Intro Linestyle";
		ucadpen->PenOrd = color;
		ucadpen->Carousel = 0;
		ucadpen->AutoNum = 0;
		ucadpen->Color_ID = usecolor;
	
		Link_Pen(ucadpen);

	}
	
	Current_Pen = ucadpen;
	
}
	
#define GET(x,y) if (!getpiece(x,y)) return(FALSE);
#define EOFNO if (ateof) { printf("Premature EOF"); return(TRUE); }
#define EOFOK if (ateof) return(TRUE);

getstring()
{

	UBYTE points,color,linetype,continued,layer,extra;
	LONG ffpx,ffpy,lp;
	STDFLT coordx,coordy,lx,ly;
	
	GET(1,&points)
	EOFOK
	
/*	printf("Points = %d\n",points);	*/
	
	GET(1,&color)
	EOFNO
	
/*	printf("Color = %d\n",color);	*/
	
	GET(1,&linetype)
	EOFNO
	
/*	printf("Linetype = %d\n",linetype);	*/

	GET(1,&continued)
	EOFNO
	
/*	printf("Continued = %d\n",continued);	*/
	
	GET(1,&extra)
	EOFNO
	
/*	printf("Extra = %d\n",extra);	*/
	
	GET(1,&layer)
	EOFNO
	
/*	printf("Layer = %d\n",layer);	*/
	
	setupcurrents(color,linetype,extra,layer);
	
	for (lp=0; lp<points; lp++) {
	
		GET(4,&ffpx)
		EOFNO
		
		GET(4,&ffpy)
		EOFNO
		
		coordx = 0;
		coordx = FFPTieee(ffpx) * imult;	
		
		coordy = 0;
		coordy = FFPTieee(ffpy) * imult;
		
/*		printf("%f,%f\n",coordx,coordy);	*/

		if (lp>0) CreateLine(lx,ly,coordx,coordy,0);

		lx = coordx;
		ly = coordy;
		
		if (lx<xmin) xmin = lx;		/* Found drawing bounds */
		if (lx>xmax) xmax = lx;
		if (ly<ymin) ymin = ly;
		if (ly>ymax) ymax = ly;
		
	}
	
	return(TRUE);		/* Tis peachy */
}		

BOOL procintro()
{

	LONG	recognizer;
	
	if (!getpiece(4,&recognizer)) {
		if (ateof) printf("Premature end of file");
		else printf("I/O Error reading file - file corrupted");
		return(FALSE);
	}
	
	if (recognizer!=INTRO_MAGIC) {
		printf("Intro File Magic number was %ld, not %ld!\n",
			recognizer,INTRO_MAGIC);
		return(FALSE);
	}
	
	printf("Code = %d\n",recognizer);
	
	FOREVER {
		if (feof(intro)) return(TRUE);
		if (ferror(intro)) return(FALSE);
		if (ateof) return(TRUE);
		
		getstring();		/* Get a string of lines */
		
	}	/* Must exit at top of loop */
	
}

ULONG introcolors[16] = {
	0xddd,0x0,0xdf,0x666,0xddd,0xc0,0xf00,0x90f,
	0xa30,0xff0,0xff,0xf7f,0xf,0xf00,0xfda,0xfff };
	
setupcolors()
{

	FILE *getcolors;
	int i;
	UBYTE usecolor;
	
	getcolors = fopen("cadverter.rgb","r");
	
	if (getcolors==NULL) getcolors = fopen("introcad.rgb","r");
	
	if (getcolors!=NULL) {
		for (i=0; i<16; i++) fscanf(getcolors,"%x",&introcolors[i]);
		fclose(getcolors);
	}

	for (i=0; i<14; i++) {	/* We can only use first 10 */
		usecolor = transmap[i];
		
		SetRGB4(vp,  i,
			(introcolors[usecolor]>>8)&0xF,
			(introcolors[usecolor]>>4)&0xF,
			(introcolors[usecolor])&0xF);
	}
	
}
	

BOOL dointro(readfile)
UBYTE *readfile;
{

	STDFLT wd;
	int loop;

	setupcolors();
	
	intro = fopen(readfile,"r");
	if (intro==NULL) {
		printf("Cannot find your intro file: %s\n",readfile);
		exit(1);
	}

	ClearView();
	
	xmin = ymin = MAXBIG;
	xmax = ymax = -MAXBIG;
			
	ateof = FALSE;
	if (!procintro()) return(FALSE);

	wd = xmax - xmin;

	xviewMIN = xmin - (wd*.05);	/* 5% overshoot */
	xviewMAX = xmax + (wd*.05);
	
	wd = ymax - ymin;
	
	yviewMIN = ymin - (wd*.05);
	yviewMAX = ymax + (wd*.05);
	
	lintype = LIN_EDI;
	Use_Bounds = TRUE;
	Snaps_On = TRUE;
	Grid_On = FALSE;
	
	xgrid = 1.0 * imult;
	ygrid = 1.0 * imult;
	xgriddiv = ygriddiv = 4;
	
	xsnap = ysnap = 0.1 * imult;
	
	Color_Source = 3;		/* Color by pen */

 	fclose(intro);

}
