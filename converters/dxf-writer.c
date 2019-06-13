#include "ucad.h"
#include "ctype.h"

/* 16-May-90 SH  	We were not writing the flags record in layers when
			the flag was zero...  This freaked out the dxf
			reader in autocad		*/

VOID doATTRIB();
STDFLT ConvEllipNotation();

FILE *outdxf;

STDFLT cvtval=1.0;

UBYTE	fixbuf[100];

UBYTE *fixname(inname)
UBYTE *inname;
{

	int i;
	UBYTE c;
	
	for (i=0; i<strlen(inname); i++) {
		c = inname[i];
		if (!isalnum(c)) c = 'x';
		fixbuf[i] = c;
	}
	
	fixbuf[i] = '\0';

	return(fixbuf);
}
	
	
int	dxfcolor(x)
int x;
{
	x-=5;
	if (x<0) x = 0;
	if (x>15) x = 15;

	return(x);
}

VOID	writestring(x,text)
int x;
UBYTE *text;
{

	fprintf(outdxf,"%3d\n",x);
	fprintf(outdxf,"%s\n",text);
}

#define writename(x,text) writestring(x,fixname(text))

VOID 	writefloat(x,flt)
int x;
STDFLT flt;
{

	fprintf(outdxf,"%3d\n",x);
	fprintf(outdxf,"%.6g\n",flt);
	
}

STDFLT ToDeg(),ToRad();

VOID	writeangle(x,flt)
int x;
STDFLT flt;
{

	writefloat(x,ToDeg(flt));
	
}

VOID 	writelength(x,flt)
int x;
STDFLT flt;
{

	fprintf(outdxf,"%3d\n",x);
	fprintf(outdxf,"%.6g\n",flt*cvtval);
	
}

VOID	writecoord(base,x,y)
int base;
STDFLT x,y;
{

	writelength(base,x);
	writelength(base+10,y);
}

VOID	writeint(x,val)
int x;
LONG val;
{
	fprintf(outdxf,"%3d\n",x);
	fprintf(outdxf,"%6ld\n",val);
}

VOID doHEADER()
{

	int i;

	writestring(0,"SECTION");	
	writestring(2,"HEADER");
	
	i = 1;
	switch (angtype) {
	case ANG_DEGD:
		i = 1;	break;
	case ANG_DEGMS:
		i = 2;	break;
	case ANG_Grad:
		i = 3; 	break;
	case ANG_rad:
		i = 4;	break;
	}
	
	writestring(9,"$AUNITS");	
	writeint(70,i);

	writestring(9,"$AUPREC");
	writeint(70,angpoints);

	cvtval = 1.0/25.4;
	
	i = 1;
	switch (lintype) {
	case LIN_MM:
		cvtval = 1.0;
		i = 1;	break;
	case LIN_EFI:
		i = 4;	break;
	}
	
	writestring(9,"$LUNITS");
	writeint(70,i);

	writestring(9,"$LUPREC");
	writeint(70,linpoints);

	writestring(9,"$LIMCHECK");
	writeint(70,Use_Bounds?1:0);
	
	writestring(9,"$EXTMAX");
	writecoord(10,xviewMAX,yviewMAX);			

	writestring(9,"$EXTMIN");
	writecoord(10,xviewMIN,yviewMIN);

	writestring(9,"$SNAPBASE");
	writecoord(10,xoffset,yoffset);
	
	writestring(9,"$SNAPUNIT");
	writecoord(10,xsnap,ysnap);
	
	writestring(9,"$SNAPMODE");
	writeint(70,Snaps_On?1:0);

	writestring(9,"$GRIDMODE");
	writeint(70,Grid_On?1:0);

	writestring(9,"$GRIDUNIT");
	writecoord(10,xgrid,ygrid);
	
	writestring(9,"$CECOLOR");
	writeint(62,256);
	
	writestring(9,"$CELTYPE");
	writestring(6,"BYLAYER");

	writestring(0,"ENDSEC");

}

VOID doTABLE_LTYPE()
{

	PEN_A *looper,*loopup;
	int doem,highwater,i,j;
	BOOL bit;
	UBYTE pendesc[20];	
	STDFLT calc[20],sign;	
	BOOL bits[20];
	
	doem = 0;

	for (looper=Root_Pen; looper!=NULL; looper=looper->pen_next) {
		looper->Pen_ID = 0;
		if (looper->Line_Pattern==0xFFFF) continue;
		if (looper->Line_Pattern==0) continue;
		
		for (loopup=Root_Pen; loopup!=looper; loopup=loopup->pen_next) {
			if (loopup->Line_Pattern==looper->Line_Pattern)	{
				looper->Pen_ID = loopup->Pen_ID;
			}
		}
		
		if (looper->Pen_ID==0) {
			doem++;
			looper->Pen_ID = doem;
		}
	}
	
	writestring(0,"TABLE");
	writestring(2,"LTYPE");
	writeint(70,doem+1);
	
	writestring(0,"LTYPE");
	writestring(2,"CONTINUOUS");	/* Create solid line type */
	writeint(70,64);
	writestring(3,"Solid Line");
	writeint(72,65);
	writeint(73,0);
	writefloat(40,0.0);
				
	highwater = -1;
	for (looper=Root_Pen; looper!=NULL; looper=looper->pen_next) {
		if (looper->Pen_ID<highwater) continue;
		if (looper->Pen_ID==0) continue;
		
		highwater = looper->Pen_ID;
		
		writestring(0,"LTYPE");
		
		sprintf(pendesc,"PEN_%d",highwater);
		writename(2,pendesc);
	
		writeint(70,64);
				
		for (i=0; i<16; i++){
			pendesc[i] = ' ';
			bits[i] = FALSE;
		}
		
		pendesc[16] = '\0';
		
		for (i=0; i<16; i++) 
			if (looper->Line_Pattern&(1<<(15-i))) 
				pendesc[i] = '-';
		
		writestring(3,pendesc);

#define EQUIV(a,b)  ((a&&b)|((!a)&&(!b)))	/* Logical equivalence */

		bit = (looper->Line_Pattern&(1<<15))?1:0;
		j = 0;
		bits[j] = bit;
		for (i=0; i<16; i++) calc[i] = 0.0;
		for (i=0; i<16; i++) {
			if (!EQUIV(bit,(looper->Line_Pattern&(1<<(15-i))))) {
				bit = !bit;
				j++;
				bits[j] = bit;
			}
				
			calc[j] += 1.0 /*looper->BitWidth*/;
		}				

		writeint(72,65);
		j++;
		writeint(73,j);
		
		if (bits[0]) {
			for (i=0; i<j; i++) {
				sign = bits[i]?1.0:(-1.0);
				writelength(40,sign*calc[i]);
			}
		} else {
			for (i=1; i<j; i++) {
				sign = bits[i]?1.0:(-1.0);
				writelength(40,sign*calc[i]);
			}
			i = 0;
			sign = bits[i]?1.0:(-1.0);
			writelength(40,sign*calc[i]);
		}
	}
	
	writestring(0,"ENDTAB");
	
}

VOID doTABLE_LAYER()
{

	LAYER_A *looper;
	int doem,lflag;
	
	doem = 0;
	
	for (looper=Root_Layer; looper!=NULL; looper=looper->layer_next) 
		doem++;
		
	writestring(0,"TABLE");
	writestring(2,"LAYER");
	writeint(70,doem+1);

	for (looper=Root_Layer; looper!=NULL; looper=looper->layer_next) {

		writestring(0,"LAYER");
			
		writename(2,looper->Layer_Name);
		writeint(62,dxfcolor(looper->Color_ID));
		
		writestring(6,"CONTINUOUS");
		
		lflag = 64;
		if (looper->Layer_Flags&LAYER_DOFF) lflag |= 1;
		writeint(70,lflag);		/* Autocad problem */

	}
	
	writestring(0,"ENDTAB");
	
}

VOID doTABLE_STYLE()
{

	STYLE_A *looper;
	int doem;
	UBYTE *styname;
	
	doem = 0;
	
	for (looper=Root_Style; looper!=NULL; looper=looper->next_style) 
		doem++;
		
	writestring(0,"TABLE");
	writestring(2,"STYLE");
	writeint(70,doem+1);

	for (looper=Root_Style; looper!=NULL; looper=looper->next_style) {
	
		writestring(0,"STYLE");
	
		writename(2,looper->Letter_Style_ID);
		writeint(70,64);
		writelength(40,looper->Prime_Height);
		writelength(41,looper->Prime_Height*.2);
		writelength(42,looper->Prime_Height);
		writefloat(50,looper->Slant);
		writeint(71,0);
		
		styname = "STANDARD";
		if (casecmp(looper->draw_font->Font_Name_ID,"simplex1"))
			styname = "TXT";
		if (casecmp(looper->draw_font->Font_Name_ID,"simplex2"))
			styname = "SIMPLEX";
		if (casecmp(looper->draw_font->Font_Name_ID,"duplex1"))
			styname = "COMPLEX";
		if (casecmp(looper->draw_font->Font_Name_ID,"duplex_italic"))
			styname = "ITALIC";
		
		writename(3,styname);
		/* writestring(4,""); */

	}
	
	writestring(0,"ENDTAB");
	
}

VOID doTABLES()
{

	writestring(0,"SECTION");
	writestring(2,"TABLES");

	doTABLE_LTYPE();
	doTABLE_LAYER();
	doTABLE_STYLE();
	
	writestring(0,"ENDSEC");
	
}

#define NOTROUND 	0x01
#define ISANARC  	0x02
#define ISROTATED 	0x04

LONG	cir = 0;

int 	ellipname = 0;

#define NOISE .001

BOOL iswierd(ellipse)
O_ELLIP *ellipse;
{

	cir = 0;
	
	if (ellipse->xradius!=ellipse->yradius) cir |= NOTROUND;
	if (abs(ellipse->rotation)>NOISE) cir |= ISROTATED;
	if (abs(ellipse->fromang)>NOISE) cir |= ISANARC;
	if (abs(TWOPI-ellipse->toangle)>NOISE) cir |= ISANARC;
	
	if ((!(cir&NOTROUND))&&(!(cir&ISANARC))) cir &= (~ISROTATED);
	
	return(cir);
}
	
VOID doBLOCK1(node)
D_NODE *node;
{

	O_ELLIP *ellipse;
	UBYTE ename[20];
	STDFLT s,e;
	
	ellipse = GetEllipData(node);
	
	if (!iswierd(ellipse)) return;
	
	if (cir&(NOTROUND|ISROTATED)==0) return;
	
	sprintf(ename,"E%d",ellipname++);
	
	writestring(0,"BLOCK");
	
	writename(2,ename);
	writecoord(10,0.0,0.0);
	writeint(70,0);
	
	if (cir&ISANARC) {
	
		s = ellipse->fromang;
		e = ellipse->toangle;

		s -= ellipse->rotation;
		e -= ellipse->rotation;
		while (s<0.0) s+=TWOPI;
		while (e<0.0) e+=TWOPI;
		
		if (cir&NOTROUND) {

			s = ConvEllipNotation(s,ellipse->xradius,
				ellipse->yradius);			
			e = ConvEllipNotation(e,ellipse->xradius,
				ellipse->yradius);				
			
		}
		
		writestring(0,"ARC");
		doATTRIB(node);
		writecoord(10,0.0,0.0);
		writefloat(40,1.0);		/* Don't convert twice */
		writeangle(50,s);
		writeangle(51,e);
	}
	
	else {
	
		writestring(0,"CIRCLE");
		doATTRIB(node);
		writecoord(10,0.0,0.0);
		writefloat(40,1.0);		/* Don't convert twice */
		
	}
	
	writestring(0,"ENDBLK");

}

VOID doBLOCK()
{

	D_NODE *lnklst;

	ellipname = 1;

	writestring(0,"SECTION");
	writestring(2,"BLOCKS");
			
	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
	
		switch (lnklst->node_id) {

		case NODE_ELLIP:
			doBLOCK1(lnklst);
			break;
		}
	}
	
	writestring(0,"ENDSEC");	
}

VOID doATTRIB(node)
D_NODE *node;
{

	UBYTE penn[40];
	int color;

	writename(8,node->node_layer->Layer_Name);
	if (node->pen_attach->Pen_ID>0) {
		sprintf(penn,"PEN_%d",node->pen_attach->Pen_ID);
		writename(6,penn);
	}

	color = -1;	
	switch (Color_Source) {
	case 1:
		color = node->above_group->Color_ID;
		break;
	case 2:
		color = node->node_layer->Color_ID;
		break;
	case 3:
		color = node->pen_attach->Color_ID;
		break;
	}
	
	if (color>=0) writeint(62,dxfcolor(color));

}

VOID doLINE(node)
D_NODE *node;
{
	O_LINE *line;
	
	writestring(0,"LINE");
	
	doATTRIB(node);
	
	line = GetLineData(node);

	writecoord(10,line->x1,line->y1);
	writecoord(11,line->x2,line->y2);
	
}

VOID doELLIP(node)
D_NODE *node;
{

	O_ELLIP *ellipse;
	UBYTE ename[20];
	STDFLT s,e;
	UBYTE penn[40];
	int color;

	
	ellipse = GetEllipData(node);
	
	if (!iswierd(ellipse)) {
		writestring(0,"CIRCLE");
		doATTRIB(node);
		writecoord(10,ellipse->x1,ellipse->y1);
		writelength(40,ellipse->xradius);
		return;
	}
	
	if (cir&(NOTROUND|ISROTATED)==0) {
		writestring(0,"ARC");
		doATTRIB(node);
		writecoord(10,ellipse->x1,ellipse->y1);
		writelength(40,ellipse->xradius);
		writeangle(50,ellipse->fromang);
		writeangle(51,ellipse->toangle);
		return;
	}
	
	sprintf(ename,"E%d",ellipname++);
	
	writestring(0,"INSERT");
	
	writename(8,node->node_layer->Layer_Name);
	if (node->pen_attach->Pen_ID>0) {
		sprintf(penn,"PEN_%d",node->pen_attach->Pen_ID);
		writename(6,penn);
	}
	
	writename(2,ename);
	writecoord(10,ellipse->x1,ellipse->y1);
	
	writelength(41,ellipse->xradius);
	writelength(42,ellipse->yradius);
	
	if (cir&ISROTATED) writeangle(50,ellipse->rotation);
	
}

VOID doTEXT(node)
D_NODE *node;
{

	O_TEXT *text;
	int handle,flags;

	text = GetTextData(node);

	writestring(0,"TEXT");
	doATTRIB(node);
	writecoord(10,text->x1,text->y1);
	writelength(40,text->letstyle->Prime_Height);
	if (text->ang!=0.0) writefloat(50,text->ang);
	writestring(1,text->text);
	writename(7,text->letstyle->Letter_Style_ID);
	
	flags = 0;
	
	if (text->Text_Bits&TEXT_FLIPX)	flags |= 2;
	if (text->Text_Bits&TEXT_FLIPY) flags |= 4;
	
	if (flags!=0) writeint(71,flags);

	handle = 0;
	if (text->Handle_Flag==1) handle = 0;
	if (text->Handle_Flag==2) handle = 1;
	if (text->Handle_Flag==3) handle = 2;

	if (handle!=0) writeint(72,handle);
	
}

VOID doENTITIES()
{

	D_NODE *lnklst;
	
	ellipname = 1;

	writestring(0,"SECTION");
	writestring(2,"ENTITIES");
	
	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
	
		switch (lnklst->node_id) {

		case NODE_LINE: 
			doLINE(lnklst);
			break;
		case NODE_ELLIP:
			doELLIP(lnklst);
			break;
		case NODE_TEXT:
			doTEXT(lnklst);
			break;
		case NODE_HATCH:
			break;
		}
	
	}
	
	writestring(0,"ENDSEC");
}

VOID doDXF(name)
UBYTE *name;
{

	printf("Write file %s\n",name);
	
	outdxf = fopen(name,"w");
	if (outdxf==NULL) return;

	doHEADER();
	doTABLES();
	doBLOCK();
	doENTITIES();
	
	writestring(0,"EOF");
	
	fclose(outdxf);
	
}
