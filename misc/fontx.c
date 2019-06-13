/*----------------------------------------------------------------------*
 *	ucad.h - WURBsoft CAD system project				*
 *----------------------------------------------------------------------*/

/*---------------Various fun includes for the Amiga---------------------*/

#include <exec/types.h>
#include <clib/macros.h>
#include <stdio.h>
#include <math.h>

#define STDFLT float		/* For now */
typedef	struct	_portfloat {		/* Portable floating point unit */
	WORD	exponent;
	LONG	mantissa;
} PORTFLOAT;

#include "fonts.h"

struct Stroke_Font Font_Ctl;
struct Load_Font_20 Load_Font;
WORD Font_Table[7000];

FILE *ifile,*ofile;

STDFLT calwid(chr,font)
UBYTE chr;
struct Stroke_Font *font;
{

	int ina,b,o,pen,x,y,c;
	int cr,l,xc,yc;
	UBYTE j;
	STDFLT rx,ry,rx0;

	rx0 = 0.0;

	j = chr;

	l=font->Char_Xtable[j]-1;

	if (l<0) return(0.0);		/* No Char There */
	
	o=font->Word_Table[l];

/*	printf("Index %d, Access %d ",l,o);	*/

	ina= -o;

	for (;;ina=font->Word_Table[++l]) {

		if (ina<=0) break;

		y=ina&0x7F;
		ina>>=7;

		x=ina&0x7F;
		ina>>=7;

		pen=ina;

		rx = font->Font_Horiz * ((STDFLT) (x-64));

		rx0 += rx;

	}
	
	rx0 += font->Font_Width;
	return(rx0);

}

VOID FtoPORT(STDFLT, PORTFLOAT *);

write_port()
{
	int i;

	Load_Font.Font_Magic = Font_Ctl.Font_Magic;
	Load_Font.Font_Version = Font_Ctl.Font_Version;
	FtoPORT(Font_Ctl.Font_Width,&Load_Font.Font_Width);
	FtoPORT(Font_Ctl.Font_Vertical,&Load_Font.Font_Vertical);
	FtoPORT(Font_Ctl.Font_Horiz,&Load_Font.Font_Horiz);

	for (i=0; i<256; i++) {
		Load_Font.Char_Xtable[i] = Font_Ctl.Char_Xtable[i];
		FtoPORT(Font_Ctl.Char_Widths[i],&Load_Font.Char_Widths[i]);
	}

	Load_Font.Num_Chars = Font_Ctl.Num_Chars;
	Load_Font.Word_Table = Font_Ctl.Word_Table;
}

main(argc,argv)
int argc;
char *argv[];
{

	int a,b,c,swp;

	if (argc!=4) {
		printf("Usage %s infont.dat outfont.ufnt swap\n",argv[0]);
		exit(1);
	}

	sscanf(argv[3],"%d",&swp);

	printf("Stroke font translator %s->%s\n",argv[1],argv[2]);
	for (a=0; a<7000; a++) Font_Table[a]=0;
	for (a=0; a<256; a++) Font_Ctl.Char_Xtable[a]=0;

	Font_Ctl.Font_Magic = CUR_MAGIC_NUM;
	Font_Ctl.Font_Version = CUR_STROKE_VER;

	Font_Ctl.Word_Table = NULL;

	ifile = fopen(argv[1],"r");
	if (ifile==NULL) {
		printf("Cannot open input file %s\n",argv[1]);
		exit(1);
	}

	ofile = fopen(argv[2],"w");
	if (ofile==NULL) {
		printf("Cannot open output file %s\n",argv[2]);
		exit(1);
	}

	fscanf(ifile,"%f %f %f",
		&Font_Ctl.Font_Width,
		&Font_Ctl.Font_Vertical,
		&Font_Ctl.Font_Horiz);

	fscanf(ifile,"%d",&a);

	if (a>256) {
		printf("Table size is wrong %d>256\n",a);
		exit(1);
	}

	for (b=0; b<a; b++) {
		fscanf(ifile,"%d",&c);
		Font_Ctl.Char_Xtable[b] = c;
	}

	/* Swap the slashed oh with the unslashed zero to make ok */
	if (swp) {
		printf("Swap the zero with the oh\n");
		b = Font_Ctl.Char_Xtable[(int) '0'];
		Font_Ctl.Char_Xtable[(int) '0'] = 
				Font_Ctl.Char_Xtable[(int) 'O'];
		Font_Ctl.Char_Xtable[(int) 'O'] = b; 
	}

	fscanf(ifile,"%d",&a);

	if (a>7000) {
		printf("Stroke table too big %d>7000\n",a);
		exit(1);
	}

	printf("Stroke table size %d\n",a);

	for (b=0; b<a; b++) {
		fscanf(ifile,"%d",&c);
		Font_Table[b] = c;
	}
	
	Font_Ctl.Num_Chars = a;
	Font_Ctl.Word_Table = Font_Table;

	for (c=0; c<256; c++) {
		Font_Ctl.Char_Widths[c] = calwid(c, &Font_Ctl);
/*		printf("Char %d, width %.3f\n",c,Font_Ctl.Char_Widths[c]); */
	}

	write_port();

	b=fwrite(&Load_Font,sizeof (struct Load_Font_20), 1, ofile);
	c=fwrite((char *) Font_Table, 2, a, ofile);

	printf("Wrote %d and %d\n",b,c);

	fclose(ofile);
	fclose(ifile);

}

VOID FtoPORT(infloat, outport)
STDFLT infloat;
PORTFLOAT *outport;
{

	int exp;
	double frac,in;

	in=infloat;
	frac=frexp(in,&exp);

	outport->exponent=exp;

	in=ldexp(frac,31);	

	outport->mantissa=in;

	return;
}
