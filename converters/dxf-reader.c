#include "ucad.h"
#include <exec/memory.h>
#include <ctype.h>

VOID procentity();

UBYTE	StrFields[10][257];
STDFLT	FltFields[50];
LONG	IntFields[20];

#define FLT(x)	(FltFields[x-10])
#define	INT(x)	(IntFields[x-60])
#define	STR(x)	(&StrFields[x][0])

UBYTE	Readbuf[257];

LONG	FType;
UBYTE	InStr[257];
STDFLT	InFlt;
LONG	InInt;

UBYTE	FieldSet[80];

FILE 	*dxf;

struct	_dxf_style {
	struct	_dxf_style	*next;
	UBYTE	*stylename;
	STDFLT	textheight;			/* 0.0 = no fixed height */
	STDFLT	slant;
	STDFLT	lastheight;
	UWORD	genflags;
	UBYTE	*fontname;
	UBYTE	*bigfont;
} *dxf_style_root=NULL,*dxf_style_tail=NULL;

struct	_dxf_layer {
	struct	_dxf_layer	*next;
	UBYTE	*linetype;			/* Name of default ltype */
	UWORD	color;				/* What color is this */
	BOOL	layeroff;			/* Is layer on or off */
	BOOL	layerfroze;			/* Is the layer frozen */
	UBYTE	*layername;			/* Name given to layer */
	LAYER_A	*ucadlayer;			/* Pointer to new ucad layer */
} *dxf_layer_root=NULL,*dxf_layer_tail=NULL;

#define DXF_LINE 1
struct	_dxf_line {
	STDFLT	x[2],y[2];
};

#define DXF_POINT 2
struct	_dxf_point {
	STDFLT	x,y;
};

#define DXF_CIRCLE 3
struct	_dxf_circle {
	STDFLT	x,y;
	STDFLT	rad;
};

#define DXF_ARC 4
struct	_dxf_arc {
	STDFLT	x,y;
	STDFLT	rad;
	STDFLT	s,e;
};

#define DXF_TRACE 5
struct 	_dxf_trace {
	STDFLT	x[4],y[4];
};

#define DXF_SOLID 6
struct	_dxf_solid {
	UBYTE	n;
	STDFLT	x[4],y[4];
};

#define DXF_TEXT 7
struct	_dxf_text {
	STDFLT	x,y;
	STDFLT	ang;
	STDFLT	ax,ay;
	STYLE_A	*style;
	struct	_dxf_style	*dxfstyle;
	UBYTE	*text;
	LONG	genflag;
	LONG	just;
};

#define DXF_SHAPE 8
struct 	_dxf_shape {
	UBYTE 	*shape;
	STDFLT	x,y;
	STDFLT	size;
	STDFLT	rotation;
	STDFLT	xscale;
	STDFLT	oblique;
};

#define DXF_INSERT 9
struct	_dxf_insert {
	STDFLT	x,y;
	STDFLT	sx,sy;
	STDFLT	rot;
	LONG	column,row;
	STDFLT	scol,srow;
	UBYTE	*block;
};

#define	DXF_POLY 10
struct	_dxf_poly {
	STDFLT	swid,ewid;
	LONG	flags;
};

#define DXF_VERTEX 11
struct	_dxf_vertex {
	STDFLT	x,y;
	STDFLT	swid,ewid;
	STDFLT	bulge,tangent;
	LONG	flags;
};

struct	_dxf_mem {
	struct	_dxf_mem	*next;	
	LAYER_A	*layer;	
	PEN_A	*pen;
	UWORD	itemcolor;
	UBYTE	type;
	union	_memtype {
		struct 	_dxf_line	line;
		struct	_dxf_point	point;
		struct	_dxf_circle	circle;
		struct	_dxf_arc	arc;
		struct	_dxf_trace	trace;
		struct	_dxf_solid	solid;
		struct	_dxf_text	text;
		struct	_dxf_shape	shape;
		struct	_dxf_insert	insert;
		struct	_dxf_poly	poly;
		struct	_dxf_vertex	vertex;
	} mem;
} *entity_root=NULL,*entity_tail=NULL;

struct	_block {
	struct	_block	*next;
	struct	_dxf_mem	*dxf_root,*dxf_tail;
	UBYTE	*BlockName;
	STDFLT	basex,basey;
	LONG	blockflags;
} *block_root=NULL,*block_tail=NULL,*current_block=NULL;

struct	Remember	*dxf_list=NULL;

int casecmp(s1,s2)
UBYTE *s1,*s2;
{

	char ts1,ts2;
	
	FOREVER {
		ts1 = *s1++;
		ts2 = *s2++;
		
		if (ts1=='\0'&&ts2=='\0') return(0);
		
		ts1 = toupper(ts1);
		ts2 = toupper(ts2);
		
		if (ts1>ts2) return(1);
		if (ts1<ts2) return(-1);
		
		continue;
	}
}

VOID *Dxf_Alloc(size)
int size;
{

	UBYTE *outloc;

	outloc = AllocRemember(&dxf_list, size, MEMF_PUBLIC|MEMF_CLEAR);

	if (outloc!=NULL) return(outloc);	/* We got fast memory */

	printf("Unable to allocate %d mainline bytes\n",size);

	return(NULL);

}

VOID *dxf_str(ins)
UBYTE *ins;
{

	UBYTE *news;
	
	news = Dxf_Alloc(strlen(ins)+1);
	if (news==NULL) return;
	
	strcpy(news,ins);
	
	return(news);
}

VOID Dxf_Free()
{

	if (dxf_list==NULL) return;		/* No Action */
	FreeRemember(&dxf_list, TRUE);
	dxf_list = NULL;

}

struct _dxf_mem *create_mem()
{

	struct	_dxf_mem *new;
	
	new = Dxf_Alloc(sizeof(struct _dxf_mem));
	if (new==NULL) {
		oneliner("No memory in dxf file reader");
		return(NULL);
	}
	
	new->layer=NULL;	
	new->next=NULL;
	
	return(new);
}

struct	_block	*create_block()
{

	struct	_block	*new;
	
	new = Dxf_Alloc(sizeof(struct _block));
	if (new==NULL) {
		oneliner("No memory in dxf file reader");
		return(NULL);
	}
	
	new->next = NULL;
	new->dxf_root = NULL;
	new->dxf_tail = NULL;
	
	return(new);
}

struct	_dxf_style	*create_style()
{

	struct _dxf_style *new;
	
	new = Dxf_Alloc(sizeof(struct _dxf_style));
	if (new==NULL) {
		oneliner("No memory in dxf file reader");
		return(NULL);
	}
	
	new->next = NULL;
	
	return(new);
}

struct	_dxf_layer	*create_layer()
{

	struct _dxf_layer *new;
	
	new = Dxf_Alloc(sizeof(struct _dxf_layer));
	if (new==NULL) {
		oneliner("No memory in dxf file reader");
		return(NULL);
	}
	
	new->next = NULL;
	
	return(new);
}

VOID addblock(inblock)
struct	_block *inblock;
{

	inblock->next=NULL;

	if (block_tail==NULL) block_root = inblock;
	else	block_tail->next = inblock;
	
	block_tail = inblock;
	
}

VOID addentity(inent)
struct	_dxf_mem *inent;
{

	inent->next = NULL;
	
	if (entity_tail==NULL) entity_root = inent;
	else	entity_tail->next = inent;
	
	entity_tail = inent;
	
}

VOID addstyle(inent)
struct	_dxf_style *inent;
{

	inent->next = NULL;
	
	if (dxf_style_tail==NULL) dxf_style_root = inent;
	else	dxf_style_tail->next = inent;
	
	dxf_style_tail = inent;
	
}

VOID addlayer(inent)
struct	_dxf_layer *inent;
{

	inent->next = NULL;
	
	if (dxf_layer_tail==NULL) dxf_layer_root = inent;
	else	dxf_layer_tail->next = inent;
	
	dxf_layer_tail = inent;
	
}

VOID addtoblock(inent)
struct	_dxf_mem *inent;
{

	inent->next = NULL;
	
	if (current_block->dxf_tail==NULL) current_block->dxf_root = inent;
	else	current_block->dxf_tail->next = inent;
	
	current_block->dxf_tail = inent;
}

resfields()
{

	int i;
	for (i=0; i<80; i++) FieldSet[i] = 0;
}

BOOL grabpair()
{

	int gettype,a;

	if (fgets(Readbuf, 256, dxf)==NULL) return(FALSE);
	for (a=strlen(Readbuf)-1; a>=0; a--) {
		if (Readbuf[a]=='\r'||
		    Readbuf[a]=='\n'||
		    Readbuf[a]==' ') Readbuf[a] = '\0';
		else break;
	}
	
	sscanf(Readbuf,"%d",&FType);

	if (fgets(Readbuf, 256, dxf)==NULL) return(FALSE);
	for (a=strlen(Readbuf)-1; a>=0; a--) {
		if (Readbuf[a]=='\r'||
		    Readbuf[a]=='\n'||
		    Readbuf[a]==' ') Readbuf[a] = '\0';
		else break;
	}

	if (FType<10) {
		strcpy(InStr,Readbuf);
		return(TRUE);
	}
	if (FType<60) {
		sscanf(Readbuf,"%f",&InFlt);
		return(TRUE);
	}
	if (FType<80) {
		sscanf(Readbuf,"%d",&InInt);
		return(TRUE);
	}
	
	printf("What was code %d\n",FType);
	return(FALSE);
}

BOOL hitret=FALSE;

VOID ret()
{
/*	if (hitret) printf("\n");
	hitret=FALSE;	*/
}

int section=0;
VOID (*whendone)() = NULL;

UBYTE varname[50];

STDFLT 	deftextsize=0.0;
UBYTE 	deftextfont[128];
UWORD	cecolor=256;		/* 0=By block, 256=by layer */
UBYTE	deflinestyle[128];

STDFLT mult=1.0;

VOID checkvar()
{

	if (casecmp(varname,"$AUNITS")==0) {	/* Angle type */
		switch (INT(70)) {
		case 1:		/* Decimal degrees */
			angtype = ANG_DEGD;	break;
		case 2:		/* DMS */
			angtype = ANG_DEGMS;	break;
		case 3:		/* Grads */
			angtype = ANG_Grad;	break;
		case 4:		/* Radians */
			angtype = ANG_rad;	break;
		case 5:		/* Surveyors units */
			angtype = ANG_DEGMS;	/* We don't have these */
		}
	}
	if (casecmp(varname,"$AUPREC")==0) {	/* Angle precision */
		angpoints = INT(70);
		if (angpoints<0) angpoints = 0;
	}
	if (casecmp(varname,"$LUNITS")==0) {	/* Linear units */
		switch (INT(70)) {
		case 1:		/* Scientific */
			lintype = LIN_MM;
			mult = 1.0;
		case 2:		/* Decimal */
			lintype = LIN_MM;
			mult = 1.0;
		case 3:		/* Engineering */
			lintype = LIN_MM;
			mult = 1.0;
		case 4: 	/* Architectural */
			lintype = LIN_EFI;
			mult = 25.4;
		}
	}
	if (casecmp(varname,"$LUPREC")==0) {	/* Linear precision */
		linpoints = INT(70);
		if (linpoints<0) linpoints = 0;
		if (lintype==LIN_EFI) lintolerance = 1.0/linpoints;
	}
	if (casecmp(varname,"$CECOLOR")==0) {	/* Default item coloring */
		cecolor = INT(62);
	}
	if (casecmp(varname,"$CELTYPE")==0) {	/* Default line type */
		strcpy(deflinestyle,STR(6));
	}
	if (casecmp(varname,"$COORDS")==0) {	/* Coord display */
	}
	if (casecmp(varname,"$LIMCHECK")==0) {	/* Bounds lock */
		if (INT(70)) Use_Bounds = TRUE;
		else Use_Bounds = FALSE;
	}
	if (casecmp(varname,"$EXTMAX")==0) {	/* Max view limits */
		xviewMAX = FLT(10);
		yviewMAX = FLT(20);
	}
	if (casecmp(varname,"$EXTMIN")==0) {	/* Min view limits */
		xviewMIN = FLT(10);
		yviewMIN = FLT(20);
	}
	if (casecmp(varname,"$SNAPBASE")==0) {	/* Snap offset */
		xoffset = FLT(10);
		yoffset = FLT(20);
	}
	if (casecmp(varname,"$SNAPUNIT")==0) {	/* Snap Unit */
		xsnap = FLT(10);
		ysnap = FLT(20);
	}
	if (casecmp(varname,"$SNAPMODE")==0) {	/* Snaps on/off */
		if (INT(70)) Snaps_On = TRUE;
		else Snaps_On = FALSE;
	}
	if (casecmp(varname,"$GRIDMODE")==0) {	/* Grids on/off */
		if (INT(70)) Grid_On = TRUE;
		else Grid_On = FALSE;
	}
	if (casecmp(varname,"$GRIDUNIT")==0) {	/* Grids sizes */
		xgridoff = ygridoff = 1.0;
		xgriddiv = ygriddiv = 1;		
		xgrid = FLT(10);
		ygrid = FLT(20);
	}
	if (casecmp(varname,"$TEXTSIZE")==0) {	/* Default text height */
		deftextsize = FLT(40);
	}
	if (casecmp(varname,"$TEXTSTYLE")==0) {	/* Default text style name */
		strcpy(deftextfont,STR(7));
	}
}

VOID headvar() 
{
	
	strcpy(varname,InStr);
	whendone = checkvar;
	return;

}

VOID starttable()
{

/*	printf("Begin Table ");
	if (FieldSet[2]) printf("%s ",STR(2));
	if (FieldSet[70]) printf("Max %d",INT(70));
	printf("\n");	*/
}

VOID linetype()
{

/*	printf("Line Style ");
	if (FieldSet[2]) printf("%s ",STR(2));

	printf("\n");	*/
}

VOID layertype()
{

	struct _dxf_layer *newlayer;
	LAYER_A *ucadlayer;
	int color;
	
	newlayer = create_layer();
	if (newlayer==NULL) return;

/*	printf("Layer ");
	if (FieldSet[2]) printf("%s ",STR(2));
	printf("Color: %d ",INT(62));
	if (FieldSet[6]) printf("Linetype: %s ",STR(6));	*/

	newlayer->linetype = NULL;
	if (FieldSet[6]) newlayer->linetype = dxf_str(STR(6));
	newlayer->layeroff = FALSE;
	color = INT(62);
	if (color<0) {
		newlayer->layeroff = TRUE;
		color = -color;
	}
	newlayer->color = color;
	color += 5;
	if (color>15) {
		if (color&1) color = 14;
		else color = 15;
	}
	
	newlayer->layername = dxf_str(STR(2));
	newlayer->layerfroze = FALSE;
	if (INT(70)&0x01) newlayer->layerfroze = TRUE;
	
	addlayer(newlayer);		

	ucadlayer = Layer_Search(newlayer->layername);
	if (ucadlayer==NULL) {
	
		ucadlayer = (LAYER_A *) 
				GI_DrawAlloc(sizeof(LAYER_A));
	
		if (ucadlayer==NULL) {
			printf("No memory to save layer id %s",newlayer->layername);
			return;
		}
			
		ucadlayer->Layer_ID = 0;
		strcpy(ucadlayer->Layer_Name,newlayer->layername);
		ucadlayer->Color_ID = color;
		ucadlayer->Layer_Flags = 0;
		if (newlayer->layeroff) 
			ucadlayer->Layer_Flags |= LAYER_DOFF | LAYER_POFF;
		if (newlayer->layerfroze) 
			ucadlayer->Layer_Flags |= LAYER_LOCKED;
		ucadlayer->Use_Count = 0;
		ucadlayer->Comment = "Dxf Layer";

		Link_Layer(ucadlayer);
		
		newlayer->ucadlayer = ucadlayer;

	}
/*	printf("\n");	*/
}

VOID styletype()
{
	struct _dxf_style *newstyle;

	newstyle = create_style();
	if (newstyle==NULL) return;

/*	printf("Text Style ");
	if (FieldSet[2]) printf("%s ",STR(2));
	if (FieldSet[3]) printf("PrimFont: %s ",STR(3));
	if (FieldSet[4]) printf("BigFont: %s ",STR(4));
	if (FieldSet[40]) printf("Height: %.3f ",FLT(40));	*/
	
	if (!FieldSet[40]) FLT(40) = deftextsize;
	if (FLT(40)<=0.0) FLT(40) = 1.0;
	
	newstyle->textheight = FLT(40);
	if (FieldSet[50]) newstyle->slant = FLT(40);
	else newstyle->slant = 0.0;
	if (FieldSet[71]) newstyle->genflags = INT(71);
	else newstyle->genflags = 0;
	if (FieldSet[3]) newstyle->fontname = dxf_str(STR(3));
	else newstyle->fontname = deftextfont;
	if (newstyle->fontname==NULL||*newstyle->fontname=='\0') 
		newstyle->fontname = "TXT";
	newstyle->stylename = dxf_str(STR(2));

	addstyle(newstyle);
		
/*	printf("\n");	*/
}

VOID gettable() 
{
	if (casecmp(InStr,"TABLE")==0)	whendone = starttable;
	if (casecmp(InStr,"LTYPE")==0)	whendone = linetype;
	if (casecmp(InStr,"LAYER")==0)	whendone = layertype;
	if (casecmp(InStr,"STYLE")==0)	whendone = styletype;
}

BOOL	entitymode=FALSE;

checklayer() 
{

	struct _dxf_layer *looper,*floop;
	PEN_A *newpen;
	UBYTE *penname,comm[120];
	int color;

	if (!FieldSet[8]) return;
	
	floop = NULL;
	for (looper=dxf_layer_root; looper!=NULL; looper=looper->next) 
		if (casecmp(looper->layername,STR(8))==0) floop=looper;
		
	if (floop==NULL) return;
	
	Current_Layer = floop->ucadlayer;
	
	color = floop->color;
	penname = floop->linetype;
	if (FieldSet[62]) color = INT(62);
	if (FieldSet[6]) penname = STR(6);
	
	sprintf(comm,"%s_%d",penname,color);
	
	newpen = Pen_Search(comm);

	if (newpen==NULL) {

		newpen = (PEN_A *) 
				GI_DrawAlloc(sizeof(PEN_A));

		if (newpen==NULL) {
			printf("No memory to save pen ");
			return;
		}
	
		newpen->Pen_ID = 0;
		strcpy(newpen->Pen_Name,comm);
		newpen->Pen_Flags = 0;
		newpen->Line_Pattern = 0xFFFF;
		newpen->Hard_Color = 0;
		newpen->Use_Count = 0;
		newpen->pen_next = NULL;
		newpen->pen_prev = NULL;
		newpen->Comment = "DXF Linestyle";
		newpen->PenOrd = color;
		newpen->Carousel = 0;
		newpen->AutoNum = 0;
		color += 5;
		if (color>15) {
			if (color&1) color = 14;
			else color = 15;
		}
		newpen->Color_ID = color;
	
		Link_Pen(newpen);

	}
	
	Current_Pen = newpen;
	return;


}

STDFLT ToRad(inf)
STDFLT inf;
{
	STDFLT res;
	res = (inf/180.0)* PI;
	return(res);
}
STDFLT ToDeg(inf)
STDFLT	inf;
{
	STDFLT res;
	res = (inf/PI) * 180.0;
	return(res);
}

STDFLT flabs(inf)
STDFLT inf;
{
	if (inf<0.0) return(-inf);
	return(inf);
}

/*	Create DXF memory tables 	*/

struct	_dxf_mem	*newent;

#define	ENT(x)	\
	newent = create_mem(); if (newent==NULL) return;	\
	newent->type = x;
	
#define NENT(x) newent->mem.x

#define FIN	\
	newent->layer = Current_Layer; \
	newent->pen = Current_Pen; \
	if (entitymode) addentity(newent);	\
	else	addtoblock(newent)

struct _block *kepit=NULL;

VOID dumpit()
{
	return;
}

VOID line_ent()
{

	ENT(DXF_LINE);
	
/*	printf("LINE ");	*/
	checklayer();
	
	NENT(line).x[0] = FLT(10);
	NENT(line).y[0] = FLT(20);
	NENT(line).x[1] = FLT(11);
	NENT(line).y[1] = FLT(21);

	hitret=TRUE;
	
	FIN;
	
	dumpit();
}

VOID point_ent()
{
	ENT(DXF_POINT);
	
/*	printf("POINT ");	*/
	checklayer();
	
	NENT(point).x = FLT(10);
	NENT(point).y = FLT(20);

	hitret=TRUE;
	
	dumpit();
}

VOID circle_ent()
{
	ENT(DXF_CIRCLE);
	
/*	printf("CIRCLE ");	*/
	checklayer();

	NENT(circle).x = FLT(10);
	NENT(circle).y = FLT(20);
	NENT(circle).rad = FLT(40);
	
	hitret=TRUE;
	
	FIN;
	
	dumpit();
}

VOID arc_ent()
{
	ENT(DXF_ARC);
	
/*	printf("ARC ");	*/
	checklayer();
	
	NENT(arc).x = FLT(10);
	NENT(arc).y = FLT(20);
	NENT(arc).rad = FLT(40);
	NENT(arc).s = ToRad(FLT(50));
	NENT(arc).e = ToRad(FLT(51));

	hitret=TRUE;
	
	FIN;
	
	dumpit();
}

int inventfont=1;

VOID text_ent()
{
	STYLE_A *looper,*gotone,*newtext;
	struct _dxf_style *stloop,*floop;
	UBYTE *getstyle,*getfont,comm[128];
	
	ENT(DXF_TEXT);
	
/*	printf("TEXT(%s) ",STR(1));	*/
	checklayer();
	
	getstyle = "STANDARD";
	if (FieldSet[7]) getstyle = STR(7);
	else 	if (deftextfont[0]!='\0') getstyle = deftextfont;
	
	floop = NULL;
	for (stloop=dxf_style_root; stloop!=NULL; stloop=stloop->next) 
		if (casecmp(stloop->stylename,getstyle)==0) floop=stloop;
		
	if (floop==NULL) {

		if (!FieldSet[40]) FLT(40) = 0.0;
		if (!FieldSet[41]) FLT(41) = 1.0;
		if (!FieldSet[50]) FLT(50) = 0.0;
		if (!FieldSet[51]) FLT(51) = 0.0;
		if (!FieldSet[71]) INT(71) = 0;
		if (!FieldSet[72]) INT(72) = 0;
		
		getfont = "simplex2";
		
		printf("Cannot find style %s\n",getstyle);
		
		strcpy(comm,"DXF Default (no style seen)");
		
	} else {
	
		if (!FieldSet[40]) FLT(40) = floop->textheight;
		if (!FieldSet[41]) FLT(41) = 1.0;
		if (!FieldSet[50]) FLT(50) = 0.0;
		if (!FieldSet[51]) FLT(51) = floop->slant;
		if (!FieldSet[71]) INT(71) = floop->genflags;
		if (!FieldSet[72]) INT(72) = 0;
		
		getfont = NULL;
		if (casecmp(floop->fontname,"TXT")==0)     getfont = "simplex1";
		if (casecmp(floop->fontname,"SIMPLEX")==0) getfont = "simplex2";
		if (casecmp(floop->fontname,"COMPLEX")==0) getfont = "duplex1";
		if (casecmp(floop->fontname,"ITALIC")==0)  getfont = "duplex1_italic";
		if (getfont==NULL) {
			getfont = "simplex2";
			sprintf(comm,"DXF Unknown Font %s",floop->fontname);
		} else	sprintf(comm,"DXF Font %s",floop->fontname);
		
	}
	
	if (FLT(40)<=0.0) FLT(40) = deftextsize;
	if (FLT(40)<=0.0) FLT(40) = 1.0;	/* What can you do? */

	gotone = NULL;
	for (looper=Root_Style; looper!=NULL; looper=looper->next_style) {
		if (looper->Prime_Height!=FLT(40)*mult) continue;
		if (looper->Aspect!=FLT(41)) continue;
		if (looper->Slant!=FLT(51)) continue;
		if (casecmp(looper->Comment,comm)!=0) continue;
		gotone = looper;
		break;
	}
	
	if (gotone==NULL) {
		
		newtext = (STYLE_A *) 
				GI_DrawAlloc(sizeof(STYLE_A));

		if (newtext==NULL) {
			printf("No memory to save lettering style\n");
			abort("No Memory");
		}

		newtext->Letter_ID = 0;
		sprintf(newtext->Letter_Style_ID,"%s_%d",getstyle,inventfont++);
		newtext->Letter_Flags = 0;
		newtext->Prime_Height=FLT(40)*mult;
		newtext->Slant=FLT(51);
		newtext->Aspect=FLT(41);	/* Typical difference? */
		newtext->Use_Count = 0;
		newtext->Comment = dxf_str(comm);

		newtext->next_style=NULL;
		newtext->prev_style=NULL;

		Link_Style(newtext);

		newtext->draw_font = Stroke_Search(getfont);
		if (newtext->draw_font==NULL) {
			if (!LoadStrokeFont(getfont)) {
				printf("Couldn't find font text font %s\n",
					getfont);
				newtext->draw_font = Root_Font;
			} else {
				newtext->draw_font = Stroke_Search(getfont);
				if (newtext->draw_font==NULL) 
					newtext->draw_font = Root_Font;
			}
		}
		gotone = newtext;
	}

	NENT(text).x = FLT(10);
	NENT(text).y = FLT(20);
	NENT(text).ang = FLT(50);		/* This is degrees???? */
	NENT(text).style = gotone;
	NENT(text).text = dxf_str(STR(1));
	NENT(text).genflag = INT(71);
	NENT(text).just = INT(72);
	if (INT(72)!=0) {
		NENT(text).ax = FLT(11);
		NENT(text).ay = FLT(21);
	}

	hitret=TRUE;

	FIN;
	
	dumpit();
	
}

VOID insert_ent()
{
	ENT(DXF_INSERT);
	checklayer();
	
/*	printf("INSERT(%s) ",STR(2));	*/
	
	if (!FieldSet[41]) FLT(41) = 1.0;
	if (!FieldSet[42]) FLT(42) = 1.0;
	if (!FieldSet[50]) FLT(50) = 0.0;
	if (!FieldSet[44]) FLT(44) = 0.0;
	if (!FieldSet[45]) FLT(45) = 0.0;
	if (!FieldSet[70]) INT(70) = 1;
	if (!FieldSet[71]) INT(71) = 1;

	if (!FieldSet[10]) FLT(10) = 0.0;
	if (!FieldSet[20]) FLT(20) = 0.0;
	
	NENT(insert).x = FLT(10);
	NENT(insert).y = FLT(20);
	NENT(insert).sx = FLT(41);
	NENT(insert).sy = FLT(42);
	NENT(insert).rot = ToRad(FLT(50));
	NENT(insert).column = INT(70);
	NENT(insert).row = INT(71);
	NENT(insert).scol = FLT(44);
	NENT(insert).srow = FLT(45);
	NENT(insert).block = dxf_str(STR(2));
	
	hitret=TRUE;
	
	FIN;
	
	dumpit();
}

struct _dxf_mem *lastpoly;

VOID poly_ent()
{

	ENT(DXF_POLY);
	
	lastpoly = newent;
	
	checklayer();
/*	printf("POLY ");	*/
	
	NENT(poly).swid = FLT(40);
	NENT(poly).ewid = FLT(41);
	NENT(poly).flags = INT(70);
	
	hitret = TRUE;
	
	FIN;	
	
	dumpit();
	
}

VOID vertex_ent()
{

	ENT(DXF_VERTEX);

	checklayer();
	
/*	printf("VERTEX ");	*/
			
	if (!FieldSet[40]) FLT(40) = lastpoly->mem.poly.swid;
	if (!FieldSet[41]) FLT(41) = lastpoly->mem.poly.ewid;
	if (!FieldSet[50]) FLT(50) = 0.0;
	
	NENT(vertex).x = FLT(10);
	NENT(vertex).y = FLT(20);
	NENT(vertex).swid = FLT(40);
	NENT(vertex).ewid = FLT(41);
	NENT(vertex).bulge = FLT(42);
	NENT(vertex).tangent = FLT(50);
	NENT(vertex).flags = INT(70);
	
	hitret = TRUE;
	
	FIN;
	
	dumpit();
}

VOID trace_ent()
{

	ENT(DXF_TRACE);
	
	checklayer();
	
/*	printf("TRACE ");	*/
	
	NENT(trace).x[0] = FLT(10);
	NENT(trace).x[1] = FLT(11);
	NENT(trace).x[2] = FLT(12);
	NENT(trace).x[3] = FLT(13);
	NENT(trace).y[0] = FLT(20);
	NENT(trace).y[1] = FLT(21);
	NENT(trace).y[2] = FLT(22);
	NENT(trace).y[3] = FLT(23);
	
	hitret = TRUE;
	
	FIN;
	
	dumpit();
}
	
VOID solid_ent()
{

	ENT(DXF_SOLID);
	
	checklayer();
	
/*	printf("SOLID ");	*/
	
	NENT(solid).x[0] = FLT(10);
	NENT(solid).x[1] = FLT(11);
	NENT(solid).x[2] = FLT(12);
	NENT(solid).x[3] = FLT(13);
	NENT(solid).y[0] = FLT(20);
	NENT(solid).y[1] = FLT(21);
	NENT(solid).y[2] = FLT(22);
	NENT(solid).y[3] = FLT(23);
	
	if (FLT(12)==FLT(13)&&
	    FLT(22)==FLT(23)) NENT(solid).n = 3;
	else	NENT(solid).n = 4;
	
	hitret = TRUE;
	
	FIN;
	
	dumpit();
}
	
VOID getentity()
{

	whendone = NULL;
	if (casecmp(InStr,"ENDBLK")==0)	return;
	if (casecmp(InStr,"ENDSEC")==0)	return;
	if (casecmp(InStr,"SEQEND")==0)	return;
	if (casecmp(InStr,"EOF")==0)	return;
	if (casecmp(InStr,"LINE")==0)	whendone = line_ent;
	if (casecmp(InStr,"POINT")==0)	whendone = point_ent;
	if (casecmp(InStr,"CIRCLE")==0)	whendone = circle_ent;
	if (casecmp(InStr,"ARC")==0)	whendone = arc_ent;
	if (casecmp(InStr,"TEXT")==0)	whendone = text_ent;
	if (casecmp(InStr,"INSERT")==0)	whendone = insert_ent;
	if (casecmp(InStr,"POLYLINE")==0)whendone = poly_ent;
	if (casecmp(InStr,"VERTEX")==0)	whendone = vertex_ent;
	if (casecmp(InStr,"TRACE")==0)	whendone = trace_ent;
	if (casecmp(InStr,"SOLID")==0)	whendone = solid_ent;

	if (whendone==NULL) {
		ret();
		printf("Entity not supported: %s\n",InStr);
	}
		
}

VOID startblock()
{

	ret();
/*	printf("Begin block ");
	if (FieldSet[2]) printf("%s ",STR(2));
	printf("\n");	*/
	
	current_block = create_block();
	if (current_block==NULL) return;
	
	if (!FieldSet[10]) FLT(10) = 0.0;
	if (!FieldSet[20]) FLT(20) = 0.0;
	current_block->BlockName = dxf_str(STR(2));
	current_block->basex = FLT(10);
	current_block->basey = FLT(20);
	current_block->blockflags = INT(70);
	
	if (casecmp(STR(2),"VOL")==0) kepit = current_block;
	dumpit();
	
	addblock(current_block);
	dumpit();
}

VOID getblock() 
{

	if (casecmp(InStr,"BLOCK")==0) {
		whendone = startblock;
		return;
	}
	getentity();	

}

VOID procpair()
{
	if (FType==0||FType==9) {
		if (whendone!=NULL) (*whendone)();	/* Clean up business */
		whendone=NULL;
		resfields();
		
		if (FType==0&&casecmp(InStr,"SECTION")==0) {
			ret();
			if (!grabpair()) return;
			if (FType!=2) {
				printf("Not a section name?? %d %s\n",
					FType,InStr);
				return;
			}
			printf("Section %s\n",InStr);
			
			if (casecmp(InStr,"HEADER")==0) section=1;
			else if (casecmp(InStr,"TABLES")==0) section=2;
			else if (casecmp(InStr,"BLOCKS")==0) section=3;
			else if (casecmp(InStr,"ENTITIES")==0) section=4;
			else {
				printf("Unknown section - skipping\n");
				section = 100;
			}
			return;	/* Go get another set */
		}
		switch(section) {
		case 1:
			if (FType==9) headvar();
			break;
		case 2:
			gettable();
			break;
		case 3:
			entitymode = FALSE;
			getblock();
			break;
		case 4:
			entitymode = TRUE;
			getentity();
			break;
		case 100:
			break;
		}
	}
	
	if (FType<10) 		strcpy(&StrFields[FType][0],Readbuf);
	else	if (FType<60) 	sscanf(Readbuf,"%f",&FltFields[FType-10]);
	else	if (FType<80)	sscanf(Readbuf,"%d",&IntFields[FType-60]);
	
	FieldSet[FType] = 1;
	
}	

VOID process()
{
	
	while (grabpair()) procpair();
}

int curtrans=0;

struct {
	STDFLT	xo,yo,ang;
	STDFLT	xs,ys;
	STDFLT	sina,cosa;
} transforms[100];

STDFLT xslate(ix,iy)
STDFLT ix,iy;
{

	STDFLT tx,ty;
	int i;
	
	for (i=curtrans-1; i>=0; i--) {
		ix *= transforms[i].xs;
		iy *= transforms[i].ys;
		
		tx = ix*transforms[i].cosa - iy*transforms[i].sina;
		ty = ix*transforms[i].sina + iy*transforms[i].cosa;
		
		ix = tx;
		iy = ty;
		
		ix += transforms[i].xo;
		iy += transforms[i].yo;
		
	}
		
	return(ix);
}

STDFLT yslate(ix,iy)
STDFLT ix,iy;
{

	STDFLT tx,ty;
	int i;
	
	for (i=curtrans-1; i>=0; i--) {
		ix *= transforms[i].xs;
		iy *= transforms[i].ys;
		
		tx = ix*transforms[i].cosa - iy*transforms[i].sina;
		ty = ix*transforms[i].sina + iy*transforms[i].cosa;
		
		ix = tx;
		iy = ty;

		ix += transforms[i].xo;
		iy += transforms[i].yo;
		
	}
	
	return(iy);
}

STDFLT rotang(iang)
STDFLT iang;
{

	int i;
	
	for (i=curtrans-1; i>=0; i--) {
		iang += transforms[i].ang;
	}
	
	return(iang);
}

/*	Create UCAD structures */

#define LYER if (newent->layer!=NULL) Current_Layer = newent->layer; \
	Current_Pen = newent->pen

VOID do_line()
{
	STDFLT x[2],y[2];
	
/*	printf("LINE ");	*/
	
	x[0] = xslate(NENT(line).x[0],NENT(line).y[0]);
	y[0] = yslate(NENT(line).x[0],NENT(line).y[0]);
	
	x[1] = xslate(NENT(line).x[1],NENT(line).y[1]);
	y[1] = yslate(NENT(line).x[1],NENT(line).y[1]);
	
	LYER;
	
	CreateLine(x[0]*mult,y[0]*mult,x[1]*mult,y[1]*mult,0);

}

VOID do_point()
{
	STDFLT x,y;
	
/*	printf("POINT ");	*/

	x = xslate(NENT(point).x,NENT(point).y);
	y = yslate(NENT(point).x,NENT(point).y);
	
	LYER;
	
	CreateLine(x*mult,y*mult,x*mult,(y+.0001)*mult,0);

}

VOID do_circle()
{

	STDFLT x,y,xrad,yrad,rot,tx,ty;
	int i;
	
/*	printf("CIRCLE ");	*/
	
	x = NENT(circle).x;
	y = NENT(circle).y;
	xrad = NENT(circle).rad;
	yrad = NENT(circle).rad;
	rot = 0.0;
 
	for (i=curtrans-1; i>=0; i--) {			/* Translation */
		xrad *= flabs(transforms[i].xs);
		yrad *= flabs(transforms[i].ys);
		
		if (transforms[i].xs<0.0) {		/* X flip */
			x = -x;
			
			rot = PI - rot;
		}
		if (transforms[i].ys<0.0) {		/* Y flip */
			y = -y;
			rot = TWOPI - rot;
		}
		
		x *= flabs(transforms[i].xs);
		y *= flabs(transforms[i].ys);
		
		tx = x*transforms[i].cosa - y*transforms[i].sina;
		ty = x*transforms[i].sina + y*transforms[i].cosa;
		
		x = tx;
		y = ty;
		
		rot += transforms[i].ang;

		normalize(rot);

		x += transforms[i].xo;
		y += transforms[i].yo;
		
	}
	
	LYER;
 
	CreateEllipse(x*mult,y*mult,xrad*mult,yrad*mult,rot,0.0,TWOPI);

}

VOID do_arc()
{

	STDFLT x,y,xrad,yrad,rot,sang,eang,t1,t2,tx,ty;
	int i;
	
/*	printf("ARC ");	*/
	
	x = NENT(arc).x;
	y = NENT(arc).y;
	xrad = NENT(arc).rad;
	yrad = NENT(arc).rad;
	rot = 0.0;
	sang = NENT(arc).s;
	eang = NENT(arc).e;
 
	for (i=curtrans-1; i>=0; i--) {			/* Translation */
		xrad *= flabs(transforms[i].xs);
		yrad *= flabs(transforms[i].ys);
		
		if (transforms[i].xs<0.0) {		/* X flip */
			x = -x;
			
			rot = PI - rot;
			t1 = sang;
			t2 = eang;
			sang = PI - t2;
			eang = PI - t1;
		}
		if (transforms[i].ys<0.0) {		/* Y flip */
			y = -y;
			rot = TWOPI - rot;
			t1 = sang;
			t2 = eang;
			sang = TWOPI - t2;
			eang = TWOPI - t1;
		}
		
		x *= flabs(transforms[i].xs);
		y *= flabs(transforms[i].ys);
		
		tx = x*transforms[i].cosa - y*transforms[i].sina;
		ty = x*transforms[i].sina + y*transforms[i].cosa;
		
		x = tx;
		y = ty;
		
		rot += transforms[i].ang;
		sang += transforms[i].ang;
		eang += transforms[i].ang;

		normalize(rot);
		normalize(sang);
		normalize(eang);

		x += transforms[i].xo;
		y += transforms[i].yo;
		
	}
	
	LYER;
 
	CreateEllipse(x*mult,y*mult,xrad*mult,yrad*mult,rot,sang,eang);

}

VOID do_text()
{

	int handle,flipflags;
	STDFLT x,y,ang;
	
/*	printf("TEXT ");	*/
	
	x = xslate(NENT(text).x,NENT(text).y);
	y = yslate(NENT(text).x,NENT(text).y);
	ang = ToDeg(rotang(ToRad(NENT(text).ang)));

	flipflags = 0;
	if (NENT(text).genflag&2) flipflags |= TEXT_FLIPX;
	if (NENT(text).genflag&4) flipflags |= TEXT_FLIPY;

	switch (NENT(text).just) {
	case 0:
		handle = 1; break;
	case 1:
		handle = 2; break;
	case 2:
		handle = 3; break;
	default:
		/* unstext_complain(); */
		handle = 1; break;
	}
	
/*	printf("\"%s\"",NENT(text).text);	*/

	LYER;

	CreateText(NENT(text).text,x*mult,y*mult,ang,
		handle,flipflags,NENT(text).style);

}

VOID do_trace()
{

	STDFLT	x[4],y[4];
	
/*	printf("TRACE ");	*/
	
	x[0] = xslate(NENT(trace).x[0],NENT(trace).y[0]);
	y[0] = yslate(NENT(trace).x[0],NENT(trace).y[0]);

	x[1] = xslate(NENT(trace).x[1],NENT(trace).y[1]);
	y[1] = yslate(NENT(trace).x[1],NENT(trace).y[1]);

	x[2] = xslate(NENT(trace).x[2],NENT(trace).y[2]);
	y[2] = yslate(NENT(trace).x[2],NENT(trace).y[2]);
	
	x[3] = xslate(NENT(trace).x[3],NENT(trace).y[3]);
	y[3] = yslate(NENT(trace).x[3],NENT(trace).y[3]);
	
	LYER;
	
	CreateLine(x[0]*mult,y[0]*mult,x[1]*mult,y[1]*mult,0);
	CreateLine(x[1]*mult,y[1]*mult,x[2]*mult,y[2]*mult,0);
	CreateLine(x[2]*mult,y[2]*mult,x[3]*mult,y[3]*mult,0);
	CreateLine(x[3]*mult,y[3]*mult,x[0]*mult,y[0]*mult,0);

}

VOID do_solid()
{

	STDFLT	x[4],y[4];
	
/*	printf("SOLID ");	*/
	
	x[0] = xslate(NENT(solid).x[0],NENT(solid).y[0]);
	y[0] = yslate(NENT(solid).x[0],NENT(solid).y[0]);

	x[1] = xslate(NENT(solid).x[1],NENT(solid).y[1]);
	y[1] = yslate(NENT(solid).x[1],NENT(solid).y[1]);

	x[2] = xslate(NENT(solid).x[2],NENT(solid).y[2]);
	y[2] = yslate(NENT(solid).x[2],NENT(solid).y[2]);
	
	x[3] = xslate(NENT(solid).x[3],NENT(solid).y[3]);
	y[3] = yslate(NENT(solid).x[3],NENT(solid).y[3]);
	
	LYER;
				
	CreateLine(x[0]*mult,y[0]*mult,x[1]*mult,y[1]*mult,0);
	CreateLine(x[1]*mult,y[1]*mult,x[2]*mult,y[2]*mult,0);
	if (NENT(solid).n==4) {
		CreateLine(x[2]*mult,y[2]*mult,x[3]*mult,y[3]*mult,0);
		CreateLine(x[3]*mult,y[3]*mult,x[0]*mult,y[0]*mult,0);
	} else 	CreateLine(x[2]*mult,y[2]*mult,x[0]*mult,y[0]*mult,0);
	
}

BOOL pol=TRUE,clos=TRUE,inpoly=FALSE;
STDFLT	polx,poly,spolx,spoly;

VOID do_poly()
{

/*	printf("POLY ");	*/
	
	pol = TRUE;
	if (NENT(poly).flags&0x1) clos=TRUE;
	
}

VOID do_vertex()
{

	STDFLT x,y;

/*	printf("VERTEX ");	*/
	x = xslate(NENT(vertex).x,NENT(vertex).y);
	y = yslate(NENT(vertex).x,NENT(vertex).y);
	
	if (pol) {
		pol = FALSE;
		spolx = polx = x;
		spoly = poly = y;
		return;
	}
	
	inpoly = TRUE;
	
	LYER;
	
	CreateLine(polx*mult,poly*mult,x*mult,y*mult,0);
	
	polx = x;
	poly = y;

}

VOID do_seqend()
{

	if (!clos) return;

	LYER;
	
	CreateLine(polx*mult,poly*mult,spolx*mult,spoly*mult,0);
	
}

VOID doblock(inblock)
struct _block *inblock;
{

	struct	_dxf_mem	*entity;
	
	for (entity=inblock->dxf_root; entity!=NULL; entity=entity->next) {
		if (entity==entity->next) 
			printf("*******Block Entity Looping*******\n");
		procentity(entity);
	}
	
}		

VOID do_insert()
{
	struct	_block	*looper,*gotit;
	int	row,col,savtrans,numl,rowm,colm;
	STDFLT	x,y;
	struct	_dxf_mem *savent;

/*	printf("INSERT(%s)",NENT(insert).block);	*/
	
	gotit = NULL;
	for (looper=block_root; looper!=NULL; looper=looper->next) 
		if (casecmp(NENT(insert).block,looper->BlockName)==0) gotit=looper;
		
	if (gotit==NULL) {
		printf("\n could not find block %s\n",NENT(insert).block);
		return;
	}
	
	if (curtrans>90) {
		printf("Too many embedded blocks\n");
		return;
	}
	
	savent = newent;	/* Recursion eats up newent */
#define SENT savent->mem.insert
	
	numl = 0;
	
	colm = SENT.column;
	rowm = SENT.row;
	
	x = SENT.x;
	for (col = 0; col<colm; col++) {
	    y = SENT.y;
	    for (row = 0; row<rowm; row++) {
	    	
		savtrans = curtrans;
		
		transforms[curtrans].xo = x;
		transforms[curtrans].yo = y;
		transforms[curtrans].ang = SENT.rot;
		transforms[curtrans].xs = SENT.sx;
		transforms[curtrans].ys = SENT.sy;
		transforms[curtrans].sina = sin(SENT.rot);
		transforms[curtrans].cosa = cos(SENT.rot);
		curtrans++;
		
		transforms[curtrans].xo = - gotit->basex;
		transforms[curtrans].yo = - gotit->basey;
		transforms[curtrans].ang = 0.0;
		transforms[curtrans].xs = 1.0;
		transforms[curtrans].ys = 1.0;
		transforms[curtrans].sina = sin(0.0);
		transforms[curtrans].cosa = cos(0.0);
		curtrans++;
		
		doblock(gotit);
		
		newent = savent;
		curtrans = savtrans;
		
		y += transforms[curtrans].sina*SENT.srow;
	    }
	    
	    x += transforms[curtrans].cosa*SENT.scol;
	}
	
		
}

VOID procentity(entity)
struct _dxf_mem *entity;
{

	int i;
	
	newent = entity;

	switch (entity->type) { 
	case DXF_POLY:		do_poly(entity);	return;
	case DXF_VERTEX:	do_vertex(entity);	return;
	}
	if (inpoly) do_seqend();
	
	switch (entity->type) { 
	case DXF_LINE:		do_line(entity);	break;
	case DXF_POINT:		do_point(entity);	break;
	case DXF_CIRCLE:	do_circle(entity);	break;
	case DXF_ARC:		do_arc(entity);		break;
	case DXF_TRACE:		do_trace(entity);	break;
	case DXF_SOLID:		do_solid(entity);	break;
	case DXF_TEXT:		do_text(entity);	break;
	case DXF_INSERT:	do_insert(entity);	break;
	}

	
}

dxftoucad()
{

	struct	_dxf_mem	*looper;
	
	curtrans = 0;
/*	printf("\nStart ucad conversion\n");	*/
	
	for (looper=entity_root; looper!=NULL; looper=looper->next) {
		if (looper->next==looper) printf("******Main Entity Loop*******\n");
		procentity(looper);
		
	}
}

dodxf(readfile)
UBYTE *readfile;
{

	int loop;

	deftextsize=0.0;
 	deftextfont[0] = '\0';
	cecolor=256;		/* 0=By block, 256=by layer */
	deflinestyle[0] = '\0';

	dxf = fopen(readfile,"r");
	if (dxf==NULL) {
		printf("Cannot find your dxf file: %s\n",readfile);
		exit(1);
	}

	Dxf_Free();		/* We don't need anymore */

	ClearView();

	entity_root = entity_tail = NULL;
	block_root = block_tail = current_block = NULL;
	dxf_style_root = dxf_style_tail = NULL;
	dxf_layer_root = dxf_layer_tail = NULL;
	dxf_list = NULL;
	
	resfields();	
	process();
	dxftoucad();
	
	xviewMAX *= mult;
	yviewMAX *= mult;
	xviewMIN *= mult;
	yviewMIN *= mult;
	xoffset *= mult;
	yoffset *= mult;
	xsnap *= mult;
	ysnap *= mult;
	xgrid *= mult;
	ygrid *= mult;
	
	Color_Source = 3;	/* Color by pen */
	
					/* Standard AutoCAD colors */
	SetRGB4(vp,  6,12, 0, 0);		/* 1=Red */
	SetRGB4(vp,  7,12,12, 0);		/* 2=Yellow */
	SetRGB4(vp,  8, 0,12, 0);		/* 3=Green */
	SetRGB4(vp,  9, 0,12,12);		/* 4=Cyan */
	SetRGB4(vp, 10, 0, 0,12);		/* 5=Blue */
	SetRGB4(vp, 11,12, 0,12);		/* 6=Magenta */
	SetRGB4(vp, 12,12,12,12);		/* 7=White */
	SetRGB4(vp, 13, 6, 6, 6);		/* 8=Grey */
	SetRGB4(vp, 14, 6, 0, 0);		/* Even - dk red */
	SetRGB4(vp, 15, 0, 0, 6);		/* Odd - dk blue */

	fclose(dxf);

}
