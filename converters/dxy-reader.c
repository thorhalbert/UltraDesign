#include "defn.h"

dxycom(op)
char    op;
{
    int intval;

    switch (op)
    {
    case 'H':			/* HOME */
    case 'h':
	break;

    case 'D':			/* DRAW */
    case 'd':
	plotps(DRAW);
	break;

    case 'M':			/* MOVE */
    case 'm':
	plotps(MOVE);
	break;

    case 'I':			/* RELATIVE DRAW */
    case 'i':
	plotps(RDRAW);
	break;

    case 'R':			/* RELATIVE MOVE */
    case 'r':
	plotps(RMOVE);
	break;

    case 'L':			/* LINE TYPE */
    case 'l':
	line_type(LINETYPE);
	break;

    case 'B':			/* LINE SCALE */
    case 'b':
	line_type(LINESCALE);
	break;

    case 'X':			/* AXIS */
    case 'x':
	{
	    int     p, q, r;

	    p = rint(getval());
	    q = rint(getval());
	    r = rint(getval());
	    unimplemented("Axis");
	}
	break;

    case 'P':			/* PRINT */
    case 'p':
	textps(TEXT);
	break;

    case 'S':			/* ALPHA SCALE */
    case 's':
	{
	    int n;

	    if (SIGNED_NUMERIC)
	        n = rint(getval());
	    else
		n = 3;
	    char_height = (n + 1) * 0.8 * SCALE;
	    char_width = (n + 1) * 0.4 * SCALE;
	    char_space = (n + 1) * 0.2 * SCALE;
	}
	setpstyle();
	break;

    case 'Q':			/* ALPHA ROTATE */
    case 'q':
	intval = rint(getval());
	switch (intval)
	{
	case 0:
	    char_angle = 0.0;
	    break;

	case 1:
	    char_angle = 90.0;
	    break;

	case 2:
	    char_angle = 180.0;
	    break;

	case 3:
	    char_angle = 270.0;
	    break;
	}
	break;

    case 'N':			/* MARK */
    case 'n':
	textps(MARK);
	break;

    case 'J':			/* PEN CHANGE */
    case 'j':
	linesize();
	break;

    case 'C':			/* CIRCLE */
    case 'c':
	circle(CIRCLE);
	break;

    case 'E':			/* RELATIVE CIRCLE */
    case 'e':
	circle(RCIRCLE);
	break;

    case 'A':			/* CIRCLE CENTER */
    case 'a':
	circle(CCIRCLE);
	break;

    case 'G':			/* ARC + CIRCLE */
    case 'g':
	circle(ACIRCLE);
	break;

    case 'K':			/* SEGMENT AND INDICATION LINES FOR CIRCLES */
    case 'k':
	circle(SCIRCLE);
	break;

    case 'T':			/* HATCHING */
    case 't':
	rectangle();
	break;

    case '^':			/* CALL HP-GL / RD-GL COMMANDS */
	end_draw();
	if ((ch = getc(stream)) != EOF)
	    hpglcom(ch);
	break;

    default:
    	unknowndxy(op);
	break;
    }
}
