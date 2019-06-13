#include "ucad.h"
#include <exec/memory.h>
#include <ctype.h>

#define DEBTEX

#define normalizei(ang) if(ang<0.0) ang+=TWOPI; if(ang>TWOPI) ang-=TWOPI

struct _part {

	UBYTE	name[50];
	D_NODE	*list;
	struct _part *next;

} *root_parts=NULL,*tail_parts=NULL;

FILE *aegis;
LOCAL BOOL ateof=FALSE,filled;
LONG	recount;

double atof();
long atol();
BOOL procaegis();
VOID FreeAegis();		

UBYTE AegisFont[40];

STDFLT AegisAspect = 1.3;		/* Aspect fudge */

LOCAL STDFLT imult = 25.4;		/* Scale multipler */

#define UNIT(x) ((x)*imult)

STDFLT xmin,ymin,xmax,ymax;

#define AEGIS_DRAW1 81086	/* Most have this */
#define AEGIS_PART2 81186	/* Parts supposed to have this */
#define AEGIS_DRAW2 32086	/* 2000 supposed to have this */
#define AEGIS_PART1 40186	/* Parts supposed to have this */

STDFLT qsin(a)
STDFLT a;
{
	return(sin(a));
}

STDFLT qcos(a)
STDFLT a;
{
	return(cos(a));
}

/*
 *     +++
 *     +++
 *  +++++++++	Aegis Color Map translator
 *  +++++++++
 *     +++
 *     +++
 */

extern UBYTE transmap[16];

ULONG aegiscolors[16] = {
	0xddd,0x0,0xdf,0x666,0xddd,0xc0,0xf00,0x90f,
	0xa30,0xff0,0xff,0xf7f,0xf,0xf00,0xfda,0xfff };
	
setupacol()
{

	FILE *getcolors;
	int i;
	
	getcolors = fopen("cadverter.aegis","r");
	
	if (getcolors==NULL) getcolors = fopen("aegisdraw.rgb","r");
	
	if (getcolors!=NULL) {
		for (i=0; i<16; i++) fscanf(getcolors,"%x",&aegiscolors[i]);
		fclose(getcolors);
	}

	for (i=0; i<10; i++) {	/* We can only use first 10 */
		
		SetRGB4(vp,  6+i,
			(aegiscolors[i+4]>>8)&0xF,
			(aegiscolors[i+4]>>4)&0xF,
			(aegiscolors[i+4])&0xF);
	}
	
}

/*
 *     +++
 *     +++
 *  +++++++++	Attribute maintenance
 *  +++++++++
 *     +++
 *     +++
 */

VOID setupattribs(color,layer,pattern,weight,filled,smooth,freehand)
LONG color,layer,pattern,weight;
BOOL filled,smooth,freehand;
{

	UBYTE	Penname[50];
	UBYTE	Layername[50],ws[10],ps[10],fl[10],*o;
	UBYTE	usecolor;
	UWORD	usepat;
	
	LAYER_A	*ucadlayer;
	PEN_A	*ucadpen;
			
	usecolor = transmap[color];
	
	ws[0] = '\0';
	if (weight>0) sprintf(ws,"_W%x",weight);
	
	ps[0] = '\0';
	if (pattern>0) sprintf(ps,"_P%x",pattern);
	
	o = fl;
	*o++ = '_';
	if (filled) *o++ = 'F';
	if (smooth) *o++ = 'S';
	if (freehand) *o++ = 'H';
	
	if (fl[1]=='\0') fl[0] = '\0';
	
	*o++ = '\0';
	
	sprintf(Penname,"C%x%s%s%s",color,ws,ps,fl);
		
	sprintf(Layername,"L%x_C%x",layer,color);
	
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
		ucadlayer->Comment = "Aegis Layer";

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
		if (!filled&pattern>0) {
			switch (pattern) {
			case 1:	usepat = 0xFFFF;	break;
			case 2:	usepat = 0xCCCC;	break;
			case 3:	usepat = 0xFF00;	break;
			case 4: usepat = 0xFFF0;	break;
			case 5:	usepat = 0xFFCC;	break;
			case 6:	usepat = 0xCC3C;	break;
			case 7:	usepat = 0xFCCC;	break;
			}
		}
		ucadpen->Line_Pattern = usepat;
		ucadpen->Hard_Color = 0;
		ucadpen->Use_Count = 0;
		ucadpen->pen_next = NULL;
		ucadpen->pen_prev = NULL;
		ucadpen->Comment = "Aegis Linestyle";
		ucadpen->PenOrd = color;
		ucadpen->Carousel = 0;
		ucadpen->AutoNum = 0;
		ucadpen->Color_ID = usecolor;
	
		Link_Pen(ucadpen);

	}
	
	Current_Pen = ucadpen;
	
}
	
LONG styleid = 0;
T_FONT *gofont = NULL;
	
VOID atextattrs(height,caspect)
STDFLT height,caspect;
{
	STYLE_A *find;
			
	for (find = Root_Style; find!=NULL; find=find->next_style) {
	
		if (find->Prime_Height==height &&
		    find->Aspect==caspect) {
		    	Current_Style = find;
			return;
		}
	}
	
	find = (LAYER_A *) GI_DrawAlloc(sizeof(STYLE_A));
	
	if (find==NULL) {
		printf("No memory to save style\n");
		return;
	}
	
	sprintf(find->Letter_Style_ID,"Style_%d",styleid++);

	if (gofont==NULL) {
	
		gofont = Stroke_Search(AegisFont);
		if (gofont==NULL) 
			if (LoadStrokeFont(AegisFont)) 
				gofont = Stroke_Search(AegisFont);
		gofont = Stroke_Search("simplex2");
		if (gofont==NULL) 
			if (LoadStrokeFont("simplex2")) 
				gofont = Stroke_Search("simplex2");
		
	}
	
	if (gofont==NULL) {
		printf("No fonts found!\n");
		exit(1);
	}		
	
	find->draw_font = gofont;			
	find->Use_Count = 0;
	find->Paper_Height = height;
	find->Prime_Height = height;
	find->Aspect = caspect;
	find->Slant = 0.0;
	find->prev_style = NULL;
	find->next_style = NULL;
	find->Letter_Flags = 0;
	find->Comment = "Text Style";
	
	Link_Style(find);
	
	Current_Style = find;
	
}

VOID textattrs(width,height)
STDFLT width,height;
{

	STDFLT caspect;
	
	caspect = width/height * AegisAspect;
	
	atextattrs(height,caspect);
}

/*
 *     +++
 *     +++
 *  +++++++++	Suck in aegis data tokens
 *  +++++++++
 *     +++
 *     +++
 */

#define DOCRLF

STDFLT expectfloat()
{

	UBYTE stbuf[100],c,*o;
	BOOL fc;
	STDFLT answer;
	
	fc = FALSE;		/* We haven't seen data yet */
	
	o = stbuf;
	
	FOREVER {
	
		c = agetc(aegis);
		/* putchar(c); */
		if (c==EOF) {
			ateof = TRUE;
			return(0);
		}
		
		if ((!fc)&&isspace(c)) continue;	/* Nothing yet */
		
		if (isspace(c)&&fc) break;		/* End of data */
		
		*o++ = c;
		
		fc = TRUE;
	}
	
	*o++ = '\0';
	
	answer = atof(stbuf);
	
	return(answer);

}

LONG expectint()
{

	UBYTE stbuf[100],c,*o;
	BOOL fc;
	LONG answer;
	
	fc = FALSE;		/* We haven't seen data yet */
	
	o = stbuf;
	
	FOREVER {
	
		c = agetc(aegis);
		/* putchar(c); */
		if (c==EOF) {
			ateof = TRUE;
			return(0);
		}
		
		if ((!fc)&&isspace(c)) continue;	/* Nothing yet */
		
		if (isspace(c)&&fc) break;		/* End of data */
		
		*o++ = c;
		
		fc = TRUE;
	}
	
	*o++ = '\0';
	
	answer = atol(stbuf);
	
	return(answer);

}

UBYTE strbuf[300];

UBYTE *expectstring()
{

	UBYTE c,*o;
	BOOL fc;
	
	fc = FALSE;
	
	o = strbuf;
	
	/* printf("S"); */
	
	FOREVER {
	
		c = agetc(aegis);
		/* putchar(c); */
		if (c==EOF) {
			ateof = TRUE;
			return(NULL);
		}
		
		if (!fc) {
		
			if (isspace(c)) continue;
		
			if (c!='"') return(NULL);	/* Bad string */
		
			fc = TRUE;
			
			continue;
			
		}
		
		if (c=='"') break;	/* End of string */
		
		if (c=='\\') {
			c = agetc(aegis);	/* Literal subst */
			/*putchar(c);*/
		}
		
		*o++ = c;
	}
	
	*o++ = '\0';
	
	return(strbuf);

}

/*
 *     +++
 *     +++
 *  +++++++++	Handle the normal drawing elements
 *  +++++++++
 *     +++
 *     +++
 */

VOID AD_Polygon()
{

	LONG i,j;
	STDFLT x,y,lx,ly;

	printf("Poly ");

	j = 0;
	FOREVER {
	
		i = expectint();	/* Continuation field */
		
		if (i==0) return;
		
		x = UNIT(expectfloat());
		y = UNIT(expectfloat());
		
		if (j>0) CreateLine(lx,ly,x,y,0);
		
		j++;
		lx = x;
		ly = y;
	}
		
}

VOID AD_Circle()
{
	STDFLT x,y,r;
	
	printf("Circle ");

	x = UNIT(expectfloat());
	y = UNIT(expectfloat());
	
	r = UNIT(expectfloat());
	
	CreateEllipse(x,y,r,r,0.0,0.0,TWOPI);
}

VOID AD_Text()
{
	STDFLT x,y,w,h,a,x1,y1,xt,yt;
	UBYTE *str;
	STDFLT pcos,psin;
	
	printf("Text ");
	
	x = UNIT(expectfloat());
	y = UNIT(expectfloat());
	
	w = UNIT(expectfloat());
	h = UNIT(expectfloat());
	
	a = expectfloat() * (180.0/PI);

	pcos = qcos(a * .01745329);
	psin = qsin(a * .01745329);
			
	y1 = h * .25;
	x1 = 0.0;
				
	xt = x1*pcos-y1*psin+x;
	yt = x1*psin+y1*pcos+y;

	str = expectstring();

	textattrs(w,h*.70);
	
	CreateText(str,xt,yt,a,LEFT_HANDLE,0,Current_Style);
}

VOID AD_Dimension()
{

	printf("Dimension ");

	(VOID) expectfloat();
	(VOID) expectfloat();
	(VOID) expectfloat();
	(VOID) expectfloat();
	(VOID) expectfloat();
	(VOID) expectfloat();
	
	return;

}

VOID droppart(part,xc,yc,xs,ys,ang)
struct _part *part;
STDFLT xc,yc,xs,ys,ang;
{

	D_NODE *lnklst,*newlnk;
	STDFLT	xt,yt,tsin,tcos;
	struct	line_mode	*linemode;
	struct	ellip_mode	*ellipmode;
	struct	text_mode	*textmode;
	
	STDFLT thx,thy,height,*sizar,width,slant;
	STDFLT ll,lr,pcos,psin;
	STYLE_A *instyle;
	UBYTE *inc;
	
	tsin = qsin(ang);
	tcos = qcos(ang);

	for (lnklst=part->list; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
	
		switch(lnklst->node_id) {
		case NODE_LINE:
			newlnk = StoStrLine(GetLineData(lnklst));
			linemode = GetLineData(newlnk);

			xt = linemode->x1;
			yt = linemode->y1;
			linemode->x1 = xt*tcos-yt*tsin;
			linemode->y1 = xt*tsin+yt*tcos;

			xt = linemode->x2;
			yt = linemode->y2;
			linemode->x2 = xt*tcos-yt*tsin;
			linemode->y2 = xt*tsin+yt*tcos;

			linemode->x1 *= xs;
			linemode->y1 *= ys;
			linemode->x2 *= xs;
			linemode->y2 *= ys;
			
			linemode->x1 += xc;
			linemode->y1 += yc;
			linemode->x2 += xc;
			linemode->y2 += yc;
			
			break;

		case NODE_ELLIP:
			newlnk = StoStrEllip(GetEllipData(lnklst));
			ellipmode = GetEllipData(newlnk);

			xt = ellipmode->x1;
			yt = ellipmode->y1;
			ellipmode->x1 = xt*tcos-yt*tsin;
			ellipmode->y1 = xt*tsin+yt*tcos;

			ellipmode->rotation += ang;
			ellipmode->fromang += ang;
			ellipmode->toangle += ang;
			
			normalize(ellipmode->rotation);
			normalize(ellipmode->fromang);
			normalize(ellipmode->toangle);
			
			ellipmode->x1 *= xs;
			ellipmode->y1 *= ys;
			ellipmode->x1 += xc;
			ellipmode->y1 += yc;
			ellipmode->xradius *= xs;
			ellipmode->yradius *= ys;
			
			break;

		case NODE_TEXT:
			newlnk = StoStrText(GetTextData(lnklst));
			textmode = GetTextData(newlnk);
			
			atextattrs(textmode->letstyle->Prime_Height,
				  textmode->letstyle->Aspect);

			textmode->letstyle = Current_Style;
			
			pcos = qcos(textmode->ang * .01745329);
			psin = qsin(textmode->ang * .01745329);
				
			/* Recalculate handle for center of box */
			
			instyle = textmode->letstyle;
			height = instyle->Prime_Height;
			slant = instyle->Slant;
				
			inc = textmode->text;
			
			width = 0.0;

			sizar = instyle->draw_font->font_link->Char_Widths;

			while (*inc) {
				width += sizar[(int) *inc];
				inc++;
			}

			width *= height * instyle->Aspect;

			switch(textmode->Handle_Flag) {

			case LEFT_HANDLE:
				ll = 0.0;
				lr = width;
				break;
			case CENTER_HANDLE:
				ll = - width/2.0;
				lr = - ll;
				break;
			case RIGHT_HANDLE:
				ll = - width;
				lr = 0.0;
				break;
			}
				
			thx = textmode->x1;
			thy = textmode->y1;
			
			yt = height/2.0;
			xt = ll + (slant*yt) + width/2.0;
				
			textmode->x1 = xt*pcos-yt*psin+thx;
			textmode->y1 = xt*psin+yt*pcos+thy;

			textmode->x1 *= xs;
			textmode->y1 *= ys;

			xt = textmode->x1;
			yt = textmode->y1;
				
			textmode->x1 = xt*tcos-yt*tsin;
			textmode->y1 = xt*tsin+yt*tcos;

			textmode->x1 += xc;
			textmode->y1 += yc;

			thx = textmode->x1;
			thy = textmode->y1;
			
			textmode->ang += ang / .01745329;
			while (textmode->ang>360.0) textmode->ang -= 360.0;
			while (textmode->ang<0.0)   textmode->ang += 360.0;
				
			yt = - (height/2.0);
			xt = -(ll + (slant*yt) + width/2.0);
				
			/* Angle changed, so recalc */
			pcos = qcos(textmode->ang * .01745329);
			psin = qsin(textmode->ang * .01745329);
				
			textmode->x1 = xt*pcos-yt*psin+thx;
			textmode->y1 = xt*psin+yt*pcos+thy;

			break;
		case NODE_HATCH:
			newlnk = StoStrHatch(GetHatchData(lnklst),NULL);
			break;
		}
	
	}

}

VOID AD_Part()
{

	UBYTE *name;
	STDFLT x,y,xs,ys,ang;
	struct _part *part;
	
	name = expectstring();
	
	printf("Part %s ",name);
	
	x = UNIT(expectfloat());
	y = UNIT(expectfloat());
	xs = expectfloat();
	ys = expectfloat();
	
	ang = expectfloat();

	(VOID) expectint();
	
	for (part=root_parts; part!=NULL; part=part->next) {
		if (strcmp(part->name,name)==0) {
			printf("Found Part\n");
			droppart(part,x,y,xs,ys,ang);
		}
	}

}

VOID AD_Arc()
{
	STDFLT x,y,r,s,e;

	printf("Arc ");

	x = UNIT(expectfloat());
	y = UNIT(expectfloat());
	r = UNIT(expectfloat());
	
	s = expectfloat();
	e = expectfloat();
	
	CreateEllipse(x,y,r,r,0.0,s,e);
}

VOID AD_Ellipse()
{
	STDFLT x,y,rx,ry,q;

	printf("Ellipse ");

	x = UNIT(expectfloat());
	y = UNIT(expectfloat());
	
	rx = UNIT(expectfloat());
	ry = UNIT(expectfloat());
	
	q = expectfloat();		/* What the hell is this field */
	
	CreateEllipse(x,y,rx,ry,0.0,0.0,TWOPI);
}

VOID AD_StorePart()
{

	UBYTE *name;
	struct _part *newpart;

	(VOID) expectint();		/* Eat count */
	
	name = expectstring();
	
	printf("Store Part %s\n",name);

	(VOID) expectfloat();
	(VOID) expectfloat();
	(VOID) expectfloat();
	(VOID) expectfloat();

	newpart = (struct _part *) GI_DrawAlloc(sizeof(struct _part));
	
	if (newpart==NULL) {
		printf("No memory to save part %s\n",name);
		return;
	}
	
	strcpy(newpart->name,name);
	newpart->list = NULL;
	newpart->next = NULL;
	
	if (tail_parts==NULL) root_parts = newpart;
	else tail_parts->next = newpart;
	
	tail_parts = newpart;
	
	if (!procaegis()) {
		printf("Wierd exit from procaegis()\n");
		return;
	}
	
	printf("Store OK\n");
	
	newpart->list = Root_Node;		/* Here is the list */
	
	Root_Node = Last_Node = NULL;
	
	return;	

}

/*
 *     +++
 *     +++
 *  +++++++++	Read and validate aegis file header
 *  +++++++++
 *     +++
 *     +++
 */

BOOL readheader()
{

	LONG magic,flags;

	BOOL ok;
			
	magic = expectint();			/* Get and check magic num */
	
	ok = FALSE;
	
	if (magic!=AEGIS_DRAW1) ok = TRUE;
	if (magic!=AEGIS_DRAW2) ok = TRUE;
	if (magic!=AEGIS_PART1) ok = TRUE;
	if (magic!=AEGIS_PART2) ok = TRUE;
	
	if (!ok) return(FALSE);

	printf("Magic OK\n");

	xviewMIN = UNIT(expectfloat());		/* Get drawing boundaries */
	yviewMIN = UNIT(expectfloat());
	
	xviewMAX = xviewMIN + UNIT(expectfloat());
	yviewMAX = yviewMIN + UNIT(expectfloat());
	
	(VOID) expectint();			/* Eat and ignore flags */
	
	paperunit = imult;			/* Set plotter scale */
	viewunit = UNIT(expectfloat());

	(VOID) expectstring();			/* Eat and ignore name */	

	return(TRUE);				/* Beauty header */

}

/*
 *     +++
 *     +++
 *  +++++++++	Top level parser
 *  +++++++++
 *     +++
 *     +++
 */

BOOL procaegis()
{

	LONG grouptype,color,layer,pattern,weight,flags;
	BOOL smooth,freehand;

	printf("Aegis Entry\n");

	FOREVER {
			
		grouptype = expectint();	/* Get the group header */
		
		if (ateof) {
			printf("EOF Found!!\n");
			return(FALSE);	/* No mas data */
		}
	
		if (grouptype==(-1)) return(TRUE);	/* End of section */
		
		if (grouptype==0) {
			AD_StorePart();
			continue;
		}
	
		(VOID) expectint();		/* Eat/Discard size */
	
		(VOID) expectfloat();		/* Eat/Discard xmin */
		(VOID) expectfloat();		/* Eat/Discard ymin */
		(VOID) expectfloat();		/* Eat/Discard xmax */
		(VOID) expectfloat();		/* Eat/Discard ymax */

		color = expectint();		/* Get color */
		layer = expectint();		/* Get layer */
		pattern = expectint();		/* Get fill/line pattern */
		weight = expectint();		/* Get the line weight */
		flags = expectint();		/* Get the flags */
	
		filled = smooth = freehand = FALSE;
	
		if (flags&0x01) filled = TRUE;
		if (flags&0x02) smooth = TRUE;
		if (flags&0x08) freehand = TRUE;
	
		setupattribs(color,layer,pattern,weight,filled,smooth,freehand);

		switch (grouptype) {
		case 1:	AD_Polygon();	break;
		case 2:	AD_Circle();	break;
		case 3:	AD_Text();	break;
		case 4:	AD_Dimension();	break;
		case 5:	AD_Part();	break;
		case 6:	AD_Arc();	break;
		case 7:	AD_Ellipse();	break;
		default:
			printf("Found main level type %d\n",grouptype);
			break;
		}
	
	}
}

BOOL doaegis(readfile)
UBYTE *readfile;
{

	STDFLT wd;
	int loop;

	setupacol();
	styleid = 0;
	
#ifdef DEBTEX
	printf("PreOpen\n");
#endif
			
	aegis = fopen(readfile,"r");
	if (aegis==NULL) {
		printf("Cannot find your aegis file: %s\n",readfile);
		exit(1);
	}

	FreeAegis();		
	
	ClearView();
	
	ateof = FALSE;
	
#ifdef DEBTEX
	printf("Start header\n");
#endif			
	
	if (!readheader()) {
		fclose(aegis);
		return(FALSE);
	}
	
#ifdef DEBTEX
	printf("Start definitions\n");
#endif
	
	if (!procaegis()) {
		fclose(aegis);
		return(FALSE);
	}

#ifdef DEBTEX
	printf("Start drawings\n");
#endif
	
	if (!procaegis()) {
		fclose(aegis);
		return(FALSE);
	}

	lintype = LIN_EDI;
	Use_Bounds = TRUE;
	Snaps_On = TRUE;
	Grid_On = FALSE;
	
	xgrid = 1.0 * imult;
	ygrid = 1.0 * imult;
	xgriddiv = ygriddiv = 4;
	
	xsnap = ysnap = 0.1 * imult;
	
	Color_Source = 3;		/* Color by pen */

 	fclose(aegis);

	return(TRUE);
}

VOID FreeAegis()
{

	struct _part *looper,*next;
	D_NODE *node;

	for (looper=root_parts; looper!=NULL; looper=looper->next) {
		for (node=looper->list; node!=NULL; node=next) {
			next = node->node_subsequent;
			FreeMem(node, node->node_length);
		}
	}
	
	root_parts = NULL;
	tail_parts = NULL;
}
