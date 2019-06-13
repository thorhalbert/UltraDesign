/*
 * Returns a real number 
 */

#include "ucad.h"
#include <ctype.h>

#include "defn.h"

VOID setppen();
extern STDFLT xmin,xmax,ymin,ymax;

#define ERROR "Two or more decimal places in a number"

VOID Bsetup(scale)
STDFLT scale;
{

    	LANDSCAPE = 1;			/* Display plot in Landscape mode */

    	SCALE = 1;			/* Default Scale */

    	PLOTABS = 1;			/* Absolute plot coordinates */

    	PENDOWN = 0;			/* Penup */

    	SETDOT = 0;			/* HP-GL only for linetype = 0 */

	SYMBOL = 0;			/* HP-GL only */

    	EOL = '\003';			/* End of line terminator default */

    	char_angle = 0;			/* Degrees */
    	char_slant = 0;			/* tan(angle) */
    	char_height = 2.7;		/* mm */
    	char_space = 0.8;		/* mm */
    	char_width = 1.9;		/* mm */

    	xoffset = yoffset = 0;

    	deg_rad = asin(1.0) / 90.0;

	XSCALE = scale;
	YSCALE = scale;
	
	xmax = -MAXBIG;
	ymax = -MAXBIG;
	xmin = MAXBIG;
	ymin = MAXBIG;
	
	setppen();
	
}

VOID CheckMinMax(xval,yval)
STDFLT xval,yval;
{
	if (xval<xmin) xmin = xval;
	if (xval>xmax) xmax = xval;
	if (yval<ymin) ymin = yval;
	if (yval>ymax) ymax = yval;
}

VOID FileMinMax()
{

	xviewMAX = xmax;
	xviewMIN = xmin;
	yviewMAX = ymax;
	yviewMIN = ymin;

}

void unknown(a,b)
char a,b;
{
	if (b!=0)
		printf("Unknown command %c%c\n",a,b);
	else	printf("Unknown command %c\n",a);
}

void unknowndxy(a)
char a;
{
	unknown(a,0);
}

void unimplemented(a)
char *a;
{

	printf("Unimplemented: %s\n",a);
}

double atof();

STDFLT
getval()
{
    char    valbuf[20];
    STDFLT   value;
    int     DECIMAL = 0;
    int     i;

    /* Null the value buffer "valbuf" */
    for (i = 0; i < 20; i++)
	valbuf[i] = '\0';

    i = 0;

    ch = getc(stream);

    while ((ch == ' ') || (ch == ','))
	ch = getc(stream);

    while ((ch >= '0' && ch <= '9') || ch == '.' || ch == '-' || ch == '+')
    {
	if (ch == '.')
	{
	    if (DECIMAL)
	    {
		fprintf(stderr,"Error: %s\n", ERROR);
		return(0.0);
	    }
	    DECIMAL = 1;
	}
	valbuf[i++] = ch;
	ch = getc(stream);
    }
    ungetc(ch, stream);		/* Put non numeric char back */
    value = atof(valbuf);
 /*   printf("%s %g\n",valbuf,value); */
    return (value);
}

end_draw()
{
    if (DRAW_FLAG)
	DRAW_FLAG = 0;
    
    dcount = 0;
}

plotps(type)
int type;
{
    if (type == MOVE || type == RMOVE)
    {
	while (SIGNED_NUMERIC)
	{
	    if (type == MOVE)
	    {
		end_draw();
		absX = lastXmove = getval() * XSCALE;
		absY = lastYmove = getval() * YSCALE;
	    } else
	    if (type == RMOVE)
	    {
		end_draw();
		lastXmove = absX += getval() * XSCALE;
		lastYmove = absY += getval() * YSCALE;
	    }
	}
    }
    else			/* Must be a DRAW or RDRAW */
    {
	while (SIGNED_NUMERIC)
	{
	    xval = getval() * XSCALE;
	    yval = getval() * YSCALE;
	    if (!DRAW_FLAG) DRAW_FLAG = 1;

	    if (type == RDRAW)
	    {
	    	CheckMinMax(absX,absY);
	    	CreateLine(absX,absY,absX+xval,absY+yval,0);
		absX += xval;
		absY += yval;
	    	CheckMinMax(absX,absY);
	    } else
	    if (type == DRAW)
	    {
	    	CheckMinMax(absX,absY);
	    	CreateLine(absX,absY,xval,yval,0);
		absX = xval;
		absY = yval;
	    	CheckMinMax(absX,absY);
	    } else
	    {
		fprintf(stderr, "Error: expecting draw command not %d\n", type);
		return;
	    }
	}
    }
}

#define MAXBUFSIZE 100

textps(type)
int     type;
{
    char    string[4];
    char    buffer[MAXBUFSIZE];
    STDFLT   Xch, Ych;
    int	    intval;
    int     i;

    end_draw();

    if (type == TEXT)
    {
	for (i = 0 ; i < MAXBUFSIZE; i++)	/* Clear buffer */
	    buffer[i] = NULL;

	i = 0;					/* Reset buffer counter */

	while (((ch = getc(stream)) != EOF)
			&& ch != CR && ch != LF && ch != EOL)
	{
	    buffer[i++] = ch;
/*	    printf("%g %g 1 %g (%c) Text\n", absX, absY, char_angle, ch);
 *	    absX += (char_width + char_space) * cos(char_angle * deg_rad);
 *	    absY += (char_width + char_space) * sin(char_angle * deg_rad);
 */
	}
    	CheckMinMax(absX,absY);
	CreateText(buffer,absX,absY,char_angle,LEFT_HANDLE,0,Current_Style);

    } else			/* Must be a MARK */
    {
	int     symb_num;

	if (SIGNED_NUMERIC)
	    symb_num = getval();
	else
	{
	    fprintf(stderr,
		    "Error: expecting a symbol number not %c (%d)",
		    symb_num, symb_num);
	    return;
	}
	intval = (int)(getval() + 0.5);
	switch (intval)
	{
	case 0:
	    strcpy(string, "*");
	    break;

	case 1:
	    strcpy(string, "+");
	    break;

	case 2:
	    strcpy(string, "#");
	    break;

	case 3:
	    strcpy(string, "@");
	    break;

	case 4:
	    strcpy(string, "%");
	    break;

	case 5:
	    strcpy(string, "|");
	    break;

	case 6:
	    strcpy(string, "=");
	    break;

	case 7:
	    strcpy(string, "&");
	    break;

	case 9:
	    strcpy(string, "O");
	    break;

	case 10:
	    strcpy(string, "0");
	    break;

	case 11:
	    strcpy(string, "Y");
	    break;

	case 12:
	    strcpy(string, "X");
	    break;

	case 13:
	    strcpy(string, "Z");
	    break;

	case 14:
	    strcpy(string, "S");
	    break;

	case 15:
	    strcpy(string, "Q");
	    break;

	default:
	    fprintf(stderr, "Warning symbol number is %d\n", symb_num);
	    strcpy(string, "*");
	    break;
	}
    	CheckMinMax(absX,absY);
	CreateText(buffer,absX,absY,char_angle,LEFT_HANDLE,0,Current_Style);
	/*printf("%g %g 5 %g (%s) Text\n", absX, absY, char_angle, string);*/
    }
}

STDFLT   linescale = 0.0;
int     linestyle = 0;
int	ipen = 0;

VOID setppen()
{

	UBYTE Penname[40];
	PEN_A *ucadpen;
	
	sprintf(Penname,"PEN%d_TYPE%d",ipen,linestyle);
	
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
		ucadpen->Line_Pattern = 0xFFFF;
		ucadpen->Hard_Color = 0;
		ucadpen->Use_Count = 0;
		ucadpen->pen_next = NULL;
		ucadpen->pen_prev = NULL;
		ucadpen->Comment = "Aegis Linestyle";
		ucadpen->PenOrd = 0;
		ucadpen->Carousel = 0;
		ucadpen->AutoNum = ipen;
		ucadpen->Color_ID = 6+ipen;
		if (ucadpen->Color_ID>15) ucadpen->Color_ID = 4;
	
		Link_Pen(ucadpen);

	}
	
	Current_Pen = ucadpen;
}

line_type(line)
int     line;
{
    STDFLT   down1, down2;
    STDFLT   up1, up2;
    STDFLT   diagonal;

    end_draw();

    if (line == LINE_TYPE_SCALE)			/* HP-GL only */
    {
	if (SIGNED_NUMERIC)
	{
	    linestyle = rint(getval());
	}
	else
	{
	    linestyle = 0;
	    setppen();
	    return;
	}

	diagonal = pow((xmax - xmin),2.0) + pow((ymax - ymin),2.0);
	diagonal = pow(diagonal,0.5);
	if (SIGNED_NUMERIC)
	    linescale = getval() * 0.01 * diagonal * XSCALE * SCALE * 0.2;
	else
	    linescale = 0.015 * diagonal * XSCALE * SCALE * 0.2;
    } else
    if (line == LINESCALE)	/* DXY commands only */
    {
	if (SIGNED_NUMERIC)
	    linescale = getval() / 80 * SCALE;
	else {
	    setppen();
	    return;
	}
    } else
    if (line == LINETYPE)	/* DXY commands only */
    {
	if (SIGNED_NUMERIC)
	{
	    linestyle = rint(getval());

	}
	else {
	    setppen();
	    return;
	}
    } else
    {
	fprintf(stderr, "Error: Unknown line flag in linetype.c\n");
	setppen();
	return;
    }
    /*
     * select a line style/type and scale/pitch 
     */
    switch (linestyle)
    {
    case 6:
	down1 = 2.0;
	up1 = up2 = 1.5;
	down2 = 1.5;
	break;

    case 5:
	down1 = 3.0;
	up1 = up2 = 1.0;
	down2 = 1.5;
	break;

    case 4:
	down1 = 4.0;
	up1 = up2 = 0.75;
	down2 = 1.0;
	break;

    case 3:
	down1 = 5.0;
	up1 = up2 = 1.0;
	down2 = 0.5;
	break;

    case 2:
	down1 = 5.5;
	up1 = 1.0;
	up2 = down2 = 0.0;
	break;

    case 1:
	down1 = 5.0;
	up1 = 1.5;
	up2 = down2 = 0.0;
	break;

    case -1:
	down1 = 1.5;
	up1 = 3.5;
	down2 = 1.5;
	up2 = 0.0;
	break;

    case -2:
	down1 = 2.0;
	up1 = 2.5;
	down2 = 2.0;
	up2 = 0.0;
	break;

    case -3:
	down1 = 2.5;
	up1 = up2 = 1.75;
	down2 = 0.5;
	break;

    case -4:
	down1 = 2.5;
	up1 = up2 = 1.5;
	down2 = 1.5;
	break;

    case -5:
	down1 = down2 = 2.0;
	up1 = up2 = 1.25;
	break;

    case -6:
	down1 = down2 = 1.75;
	up1 = up2 = 1.5;
	break;
    }
    if (linestyle == 0)
    {
	SETDOT = 1;		/* For HP-GL only */

    } else
    {
	SETDOT = 0;		/* For HP-GL only */
	if (linescale <= 0) linescale = SCALE;
	down1 *= linescale;
	up2 *= linescale;
	up1 *= linescale;
	down2 *= linescale;
    }
    setppen();
    return;
    
}


linesize()
{

    if (SIGNED_NUMERIC)
	ipen = getval() - 1;

    setppen();
}

circle(type)
int	type;
{
    STDFLT	radius;
    STDFLT	start_angle = 0;
    STDFLT	end_angle = 360;
    STDFLT	chord_angle = 0;
    STDFLT	Xc, Yc;
    STDFLT	percent;
    STDFLT	distance;
    STDFLT	length;
    STDFLT	angle;

    end_draw();

    switch (type)
    {
    case RDGLCIRCLE:
	if (SIGNED_NUMERIC)
	    radius = getval() * XSCALE;
	if (SIGNED_NUMERIC)
	    chord_angle = getval();
	Xc = absX;
	Yc = absY;
	break;

    case CIRCLE:
	if (SIGNED_NUMERIC)
	    absX = Xc = getval() * XSCALE;
	if (SIGNED_NUMERIC)
	    absY = Yc = getval() * YSCALE;
	if (SIGNED_NUMERIC)
	    radius = getval() * XSCALE;
	if (SIGNED_NUMERIC)
	    start_angle = getval();
	if (SIGNED_NUMERIC)
	    end_angle = getval();
	if (SIGNED_NUMERIC)
	    chord_angle = getval();
	break;

    case RCIRCLE:
	if (SIGNED_NUMERIC)
	    radius = getval() * XSCALE;
	if (SIGNED_NUMERIC)
	    start_angle = getval();
	if (SIGNED_NUMERIC)
	    end_angle = getval();
	if (SIGNED_NUMERIC)
	    chord_angle = getval();
	angle = deg_rad * (90.0 - start_angle);
	Xc = absX - radius * cos(angle);
	angle = deg_rad * (90.0 - start_angle);
	Yc = absY + radius * sin(angle);
	break;

    case CCIRCLE:
	if (SIGNED_NUMERIC)
	   absX = getval() * XSCALE;
	if (SIGNED_NUMERIC)
	   absY = getval() * YSCALE;
	break;

    case ACIRCLE:
	Xc = absX;
	Yc = absY;
	if (SIGNED_NUMERIC)
	    radius = getval() * XSCALE;
	if (SIGNED_NUMERIC)
	    start_angle = getval();
	if (SIGNED_NUMERIC)
	    end_angle = getval();
	if (SIGNED_NUMERIC)
	    chord_angle = getval();
	break;

    case SCIRCLE:
	if (SIGNED_NUMERIC)
	    percent = getval();
	if (SIGNED_NUMERIC)
	    distance = getval();
	if (SIGNED_NUMERIC)
	    length = getval();
	fprintf("Warning: segment and indication lines not available yet\n");
	break;
    }

    CheckMinMax(absX-radius,absY+radius);
    CheckMinMax(absX+radius,absY+radius);
    CheckMinMax(absX-radius,absY-radius);
    CheckMinMax(absX+radius,absY-radius);
    
    CreateEllipse(Xc,Yc,radius,radius,start_angle,end_angle);

}

rectangle()
{
    int		hatch;
    STDFLT	width, height;
    STDFLT	hatch_spacing;
    STDFLT	hatch_angle;
    
    if (SIGNED_NUMERIC)
	hatch = getval();
    if (SIGNED_NUMERIC)
	width = getval() * XSCALE;
    if (SIGNED_NUMERIC)
	height = getval() * YSCALE;
    if (SIGNED_NUMERIC)
	hatch_spacing = getval() * XSCALE;
    if (SIGNED_NUMERIC)
	hatch_angle = getval();

    end_draw();
    
    CheckMinMax(xval,yval);
    CheckMinMax(xval+width,yval+height);
    
    CreateLine(xval      ,yval       ,xval+width,yval       ,0);
    CreateLine(xval+width,yval       ,xval+width,yval+height,0);
    CreateLine(xval+width,yval+height,xval      ,yval+height,0);
    CreateLine(xval      ,yval+height,xval      ,yval       ,0);
    
    /* printf("%g %g M\n", xval, yval);
    printf("%g 0 I\n", width);
    printf("0 %g I\n", height);
    printf("-%g 0 I\n", width);
    printf("closepath stroke\n"); */
    
    if (hatch != 2)
        fprintf(stderr, "Warning: Cross hatching not implemented\n");
}

/*
 * Plot a dot or symbol on the paper 
 */

plotdot(type)
char   *type;
{
    end_draw();

    while (SIGNED_NUMERIC)
    {
	xval = getval() * XSCALE;
	yval = getval() * YSCALE;
	if (type == RMOVE)
	{
	    absX += xval;
	    absY += yval;
	} else
	if (type == MOVE)
	{
	    absX = xval;
	    absY = yval;
	} else
	{
	    fprintf(stderr, "Error: expecting move command not %s\n", type);
	    return;
	}
/*
	if (SYMBOL)
	    printf("%g %g 5 %g (%c) Text\n", absX, absY, char_angle, symbol);
    	else if (SETDOT)
	{
    	    printf("newpath\n");
    	    printf("  %g %g %s %g %g %s\n", absX, absY, MOVE, absX, absY, DRAW);
	    printf("stroke\n");
	} */
    }
}

int Pstyleid=1;

VOID setpstyle()
{
	STYLE_A *find;
	T_FONT *gofont;
			
	for (find = Root_Style; find!=NULL; find=find->next_style) {
	
		if (find->Prime_Height==char_height &&
		    find->Slant==char_slant) {
		    	Current_Style = find;
			return;
		}
	}
	
	find = (LAYER_A *) GI_DrawAlloc(sizeof(STYLE_A));
	
	if (find==NULL) {
		printf("No memory to save style\n");
		return;
	}
	
	sprintf(find->Letter_Style_ID,"Style_%d",Pstyleid++);

	if (gofont==NULL) {
	
		gofont = Stroke_Search("simplex2");
		if (gofont==NULL) 
			if (LoadStrokeFont("simplex2")) 
				gofont = Stroke_Search("simplex2");
		gofont = Stroke_Search("duplex1");
		if (gofont==NULL) 
			if (LoadStrokeFont("duplex1")) 
				gofont = Stroke_Search("duplex1");
	}
	
	if (gofont==NULL) {
		printf("No fonts found!\n");
		return;
	}		
	
	find->draw_font = gofont;			
	find->Use_Count = 0;
	find->Paper_Height = char_height;
	find->Prime_Height = char_height;
	find->Aspect = 1.0;
	find->Slant = char_slant;
	find->prev_style = NULL;
	find->next_style = NULL;
	find->Letter_Flags = 0;
	find->Comment = "Text Style";
	
	Link_Style(find);
	
	Current_Style = find;
	
}

VOID dohpgl(file)
UBYTE *file;
{
	FILE *hpgl;
	UBYTE op;
	
	hpgl = fopen(file,"r");
	if (hpgl==NULL) return;
	
	stream = hpgl;
	
	ClearView();

	Bsetup(0.02487);
	
    	while ((op = getc(stream)) != EOF) {
		if (feof(stream)) break;
		if (isalpha(op) > 0)
	    		hpglcom(op);
	}

	FileMinMax();
	
	fclose(hpgl);
	
}

VOID dodxy(file)
UBYTE *file;
{
	FILE *dxy;
	UBYTE op;
	
	dxy = fopen(file,"r");
	if (dxy==NULL) return;
	
	stream = dxy;

	ClearView();

	Bsetup(0.1);
	
    	while ((op = getc(stream)) != EOF) {
		if (feof(stream)) break;
		if (isalpha(op) > 0)
	    		dxycom(op);
	}

	FileMinMax();
	
	fclose(dxy);
	
}
