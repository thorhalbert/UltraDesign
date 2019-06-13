/*
 *	RDGL/HPGL Reader
 */

#include "defn.h"

/* The folowing defaults should be 0.5% and 1.0% for the respective  character
 * width and height, however this is too small when scaled to Postcript
 * charcter sizes.
 */
float DEFWIDTH = 0.0075;	/* 0.75 % of P2x - P1x for default char width */
float DEFHEIGHT = 0.015;	/* 1.5 % of P2y - P1y for default char height */

hpglcom(op1)
char    op1;
{
    char    op2;
    int     no_op;		/* No operation value */

    switch (op1)
    {
    case 'A':
    case 'a':
	op2 = getc(stream);
	switch (op2)
	{
	case 'A':		/* Arc Absolute (not HP-GL) */
	case 'a':
	    break;

	case 'P':		/* Automatic Pen Lift (HP-GL only) */
	case 'p':
	    while (((ch = getc(stream)) != EOF) && ch != ';');
	    break;

	case 'R':		/* Arc Relative (not HP-GL) */
	case 'r':
	    break;

	default:
		unknown(op1,op2);
	}
	break;

    case 'C':
    case 'c':
	op2 = getc(stream);
	switch (op2)
	{
	case 'A':		/* Alternate Character Set (Not Used) */
	case 'a':
	    while (((ch = getc(stream)) != EOF) && ch != ';');
	    unimplemented("Alternate Char Set");
	    break;

	case 'I':		/* Circle */
	case 'i':
	    circle(RDGLCIRCLE);
	    break;

	case 'P':		/* Character Plot */
	case 'p':
	    {
		float   xspace, yspace;

		xspace = getval() * XSCALE * SCALE * (char_width + char_space);
		yspace = getval() * YSCALE * SCALE * (char_width + char_space);
		end_draw();
		/*printf("    %g mm %g mm %s\n", xspace, yspace, RMOVE);*/
	    }
	    break;

	case 'S':		/* Standard Character Set */
	case 's':
	    while (((ch = getc(stream)) != EOF) && ch != ';');
	    break;

	default:
		unknown(op1,op2);
	}
	break;

    case 'D':
    case 'd':
	op2 = getc(stream);
	switch (op2)
	{
	case 'C':		/* Digitize Clear (Not Used) */
	case 'c':
	    break;

	case 'F':		/* Default */
	case 'f':
	    SETDOT = 0;
	    SYMBOL = 0;
	    PENDOWN = 0;
	    EOL = '\003';
	    char_angle = 0;
	    char_slant = 0;
	    char_width = DEFWIDTH * (xmax - xmin) * XSCALE * SCALE;
	    char_height = DEFHEIGHT * (ymax - ymin) * YSCALE * SCALE;
	    char_space = char_width * (1/.67 - 1);
	    setpstyle();
	    break;

	case 'I':		/* Absolute Direction */
	case 'i':
	    {
		float   run, rise;

		if (SIGNED_NUMERIC)
		{
		    run = getval() * XSCALE;
		    rise = getval() * YSCALE;
		    char_angle = atan2(rise, run) / deg_rad;
		}
		else
		    char_angle = 0;
	    }
	    break;

	case 'P':		/* Digitize Point (Not Used) */
	case 'p':
	    break;

	case 'R':		/* Relative Direction */
	case 'r':
	    {
		float   run, rise;

		if (SIGNED_NUMERIC)
		{
		    run = getval() * XSCALE;
		    rise = getval() * YSCALE;
		    char_angle += atan2(rise, run) / deg_rad;
		}
		else
		    char_angle = 0;
	    }
	    break;

	case 'T':		/* Define Terminator */
	case 't':
	    if ((ch = getc(stream)) != EOF)
		EOL = ch;	/* End of label terminator */
	    break;

	default:
		unknown(op1,op2);
	}
	break;

    case 'E':			/* NOT HP-GL */
    case 'e':
	op2 = getc(stream);
	switch (op2)
	{
	case 'A':		/* Edge Rectangle Absolute */
	case 'a':
	    break;

	case 'R':		/* Edge Rectangle Relative */
	case 'r':
	    break;

	case 'W':		/* Edge Wedge */
	case 'w':
	    break;

	default:
		unknown(op1,op2);
	}
	break;

    case 'F':			/* NOT HP-GL */
    case 'f':
	op2 = getc(stream);
	switch (op2)
	{
	case 'T':		/* Fill Type */
	case 't':
	    break;

	default:
		unknown(op1,op2);
	}
	break;

    case 'I':
    case 'i':
	op2 = getc(stream);
	switch (op2)
	{
	case 'M':		/* Input Mask (Not Used) */
	case 'm':
	    break;

	case 'N':		/* Initialize */
	case 'n':
	    SETDOT = 0;
	    SYMBOL = 0;
	    PENDOWN = 0;
	    EOL = '\003';
	    char_angle = 0;
	    char_slant = 0;
	    char_width = DEFWIDTH * (xmax - xmin) * XSCALE * SCALE;
	    char_height = DEFHEIGHT * (ymax - ymin) * YSCALE * SCALE;
	    char_space = char_width * (1/0.67 - 1);
	    setpstyle();
	    break;

	case 'P':		/* Input P1 and P2 (Not Used) */
	case 'p':
	    while (((ch = getc(stream)) != EOF) && ch != ';');
	    unimplemented("IP command");
	    break;

	case 'W':		/* Input Window */
	case 'w':
	    while (((ch = getc(stream)) != EOF) && ch != ';');
	    unimplemented("IW Command");
	    break;

	default:
		unknown(op1,op2);
	}
	break;

    case 'L':
    case 'l':
	op2 = getc(stream);
	switch (op2)
	{
	case 'B':		/* Label */
	case 'b':
	    textps(TEXT);
	    break;

	case 'T':		/* Line Type */
	case 't':
	    line_type(LINE_TYPE_SCALE);
	    break;

	default:
		unknown(op1,op2);
	}
	break;

    case 'O':			/* NOT USED */
    case 'o':
	op2 = getc(stream);
	switch (op2)
	{
	case 'A':		/* Output Actual Position (Not HP-GL) */
	case 'a':
	    break;

	case 'C':		/* Output Commanded Position */
	case 'c':
	    break;

	case 'D':		/* Output Digitise */
	case 'd':
	    break;

	case 'E':		/* Output Error */
	case 'e':
	    break;

	case 'P':		/* Output P1 and P2 */
	case 'p':
	    break;

	case 'S':		/* Output Status */
	case 's':
	    break;

	case 'W':		/* Output Window (Not HP-GL) */
	case 'w':
	    break;

	default:
		unknown(op1,op2);
	}
	break;

    case 'P':
    case 'p':
	op2 = getc(stream);
	switch (op2)
	{
	case 'A':		/* Plot Absolute */
	case 'a':
	    PLOTABS = 1;
	    if (SIGNED_NUMERIC)
		if (SETDOT || SYMBOL)
		    plotdot(MOVE);
		else if (PENDOWN)
		    plotps(DRAW);
		else
		    plotps(MOVE);
	    break;

	case 'D':		/* Pen Down */
	case 'd':
	    PENDOWN = 1;
	    if (SIGNED_NUMERIC)
		if (SETDOT)
		    plotdot(MOVE);
		else if (PLOTABS)
		    plotps(DRAW);
		else 
		    plotps(RDRAW);
	    break;

	case 'R':		/* Plot Relative */
	case 'r':
	    PLOTABS = 0;
	    if (SIGNED_NUMERIC)
		if (SETDOT || SYMBOL)
		    plotdot(RMOVE);
		else if (PENDOWN)
		    plotps(RDRAW);
		else
		    plotps(RMOVE);
	    break;

	case 'T':		/* Pen Thickness (Not HP-GL) */
	case 't':
	    {
		float   linewidth;

		linewidth = getval() * SCALE;		/* In mm */
	    }
	    break;

	case 'U':		/* Pen Up */
	case 'u':
	    PENDOWN = 0;
	    if (SIGNED_NUMERIC)
		if (SETDOT)
		    plotdot(MOVE);
		else if (PLOTABS)
		    plotps(MOVE);
		else
		    plotps(RMOVE);
	    break;

	default:
		unknown(op1,op2);
	}
	break;

    case 'R':			/* Not HP-GL */
    case 'r':
	op2 = getc(stream);
	switch (op2)
	{
	case 'A':		/* Shade Rectange Absolute */
	case 'a':
	    break;

	case 'R':		/* Shade Rectangle Relative */
	case 'r':
	    break;

	default:
		unknown(op1,op2);
	}
	break;

    case 'S':
    case 's':
	op2 = getc(stream);
	switch (op2)
	{
	case 'A':		/* Select Alternate Set (Not Used) */
	case 'a':
	    break;

	case 'C':		/* Scale */
	case 'c':
	    if (SIGNED_NUMERIC)
		xmin = getval();
	    else
		break;

	    if (SIGNED_NUMERIC)
		xmax = getval();
	    if (SIGNED_NUMERIC)
		ymin = getval();
	    if (SIGNED_NUMERIC)
		ymax = getval();
	    unimplemented("Scaling");
	    break;

	case 'I':		/* Absolute Character Size */
	case 'i':
	    if (SIGNED_NUMERIC)
	    {
		char_width = getval() * 10 * SCALE;	/* In mm */
		char_height = getval() * 10 * SCALE;	/* In mm */
	    } else
	    {
		if ((ch = getc(stream)) == ';')
		{
		    char_width = 2.7 * SCALE;
		    char_height = 1.9 * SCALE;
		}
	    }
	    char_space = char_width * (1/.67 - 1);
	    setpstyle();
	    break;

	case 'L':		/* Character Slant */
	case 'l':
	    if (SIGNED_NUMERIC)
		char_slant = char_height * getval();
	    else
		char_slant = 0;

	    char_space = char_width * (1/.67 - 1);
	    setpstyle();
	    break;

	case 'M':		/* Symbol Mode */
	case 'm':
	    if ((ch = getc(stream)) != EOF && ch != ';' && isgraph(ch) > 0)
	    {
		 symbol = ch;
		 SYMBOL = 1;
	    }
	    else
		SYMBOL = 0;
	    break;

	case 'P':		/* Pen Select */
	case 'p':
	    linesize();
	    break;

	case 'R':		/* Relative Character Size */
	case 'r':
	    {
		float   pwidth, pheight;

		if (SIGNED_NUMERIC)
		{
		    pwidth = getval() * SCALE;		/* Percent */
		    pheight = getval() * SCALE;		/* Percent */
		} else
		{
		    pwidth = DEFWIDTH * 100 * SCALE;
		    pheight = DEFHEIGHT * 100 * SCALE;
		}
		char_width = (xmax - xmin) * XSCALE * pwidth / 100.0;
		char_height = (ymax - ymin) * YSCALE * pheight / 100.0;
	        char_space = char_width * (1/.67 - 1);
	    }
	    setpstyle();
	    break;

	default:
		unknown(op1,op2);
	}
	break;

    case 'T':
    case 't':
	op2 = getc(stream);
	switch (op2)
	{
	case 'L':		/* Tick Length */
	case 'l':
	    while (((ch = getc(stream)) != EOF) && ch != ';');
	    unimplemented("Tick Length");
	    break;

	default:
		unknown(op1,op2);
	}
	break;

    case 'U':
    case 'u':
	op2 = getc(stream);
	switch (op2)
	{
	case 'C':		/* User Defined Character */
	case 'c':
	    while (((ch = getc(stream)) != EOF) && ch != ';');
	    unimplemented("User Character Set");
	    break;

	default:
		unknown(op1,op2);
	}
	break;

    case 'V':
    case 'v':
	op2 = getc(stream);
	switch (op2)
	{
	case 'S':		/* Velocity Select */
	case 's':
	    while (((ch = getc(stream)) != EOF) && ch != ';');
	    break;

	default:
		unknown(op1,op2);
	}
	break;

    case 'W':			/* Not HP-GL */
    case 'w':
	op2 = getc(stream);
	switch (op2)
	{
	case 'G':		/* Shade Wedge */
	case 'g':
	    break;

	default:
		unknown(op1,op2);
	}
	break;

    case 'X':
    case 'x':
	op2 = getc(stream);
	switch (op2)
	{
	case 'T':		/* X Tick */
	case 't':
	    while (((ch = getc(stream)) != EOF) && ch != ';');
	    unimplemented("X tick");
	    break;

	default:
		unknown(op1,op2);
	}
	break;

    case 'Y':
    case 'y':
	op2 = getc(stream);
	switch (op2)
	{
	case 'T':		/* Y Tick */
	case 't':
	    while (((ch = getc(stream)) != EOF) && ch != ';');
	    unimplemented("Y tick");
	    break;

	default:
		unknown(op1,op2);
	}
	break;

    default:
    	unknown(op1,0);
	break;
    }
}
