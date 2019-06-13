/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	attrib_basics.c - manipulate basic attrib structures
 *
 */

/*
 *	Copyright (C) 1986, 1987, 1988 by WURBsoft Inc., and Scott Halbert.
 *
 *	No portion of this product in any form (this includes but is not
 *	limited to code sources, binaries, documentation, data or other), 
 *	may be used, transferred, or kept by any parties other than the 
 *	owners (WURBsoft Inc. and Scott Halbert),  unless the proper 
 *	licenses have been executed between the user and WURBsoft Inc.
 *
 *	Any use not covered by a legal software license is considered 
 *	by us to be Software Piracy, and constitutes a violation of our 
 *	rights.  Don't get us mad.
 *
 */

/*
 *	Modification History
 *
 *	0	11-Jan-87 SH	Create new template
 *	1	 3-Nov-88 SH 	Creation
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

BOOL match();

int cmplyname(in1,in2)
LAYER_A *in1,*in2;
{

	return(asccmp(in1->Layer_Name,in2->Layer_Name));

}

VOID Link_Layer(instr)
LAYER_A *instr;
{

	LAYER_A *loop;

	instr->layer_next = NULL;
	instr->layer_prev = NULL;

	if (Root_Layer==NULL) {
		Root_Layer = Last_Layer = instr;
		return;
	}

	for (loop=Root_Layer; loop!=NULL; loop=loop->layer_next)
		if (cmplyname(instr,loop)<0) break;

	if (loop==NULL) {
		Last_Layer->layer_next = instr;
		instr->layer_prev = Last_Layer;
		Last_Layer = instr;
		return;
	}
	
	if (loop->layer_prev==NULL) {
		Root_Layer = instr;
		loop->layer_prev = instr;
		instr->layer_next = loop;
		return;
	}

	loop->layer_prev->layer_next = instr;
	instr->layer_next = loop;
	instr->layer_prev = loop->layer_prev;
	loop->layer_prev = instr;

	return;
}

int cmppnname(in1,in2)
PEN_A *in1,*in2;
{

	return(asccmp(in1->Pen_Name,in2->Pen_Name));

}

VOID Link_Pen(instr)
PEN_A *instr;
{

	PEN_A *loop;

	instr->pen_next = NULL;
	instr->pen_prev = NULL;

	if (Root_Pen==NULL) {
		Root_Pen = Last_Pen = instr;
		return;
	}

	for (loop=Root_Pen; loop!=NULL; loop=loop->pen_next)
		if (cmppnname(instr,loop)<0) break;

	if (loop==NULL) {
		Last_Pen->pen_next = instr;
		instr->pen_prev = Last_Pen;
		Last_Pen = instr;
		return;
	}
	
	if (loop->pen_prev==NULL) {
		Root_Pen = instr;
		loop->pen_prev = instr;
		instr->pen_next = loop;
		return;
	}

	loop->pen_prev->pen_next = instr;
	instr->pen_next = loop;
	instr->pen_prev = loop->pen_prev;
	loop->pen_prev = instr;

	return;
}

int cmptsname(in1,in2)
STYLE_A *in1,*in2;
{

	return(asccmp(in1->Letter_Style_ID,in2->Letter_Style_ID));

}

VOID Link_Style(instr)
STYLE_A *instr;
{

	STYLE_A *loop;

	instr->next_style = NULL;
	instr->prev_style = NULL;

	if (Root_Style==NULL) {
		Root_Style = Style_Last = instr;
		return;
	}

	for (loop=Root_Style; loop!=NULL; loop=loop->next_style)
		if (cmptsname(instr,loop)<0) break;

	if (loop==NULL) {
		Style_Last->next_style = instr;
		instr->prev_style = Style_Last;
		Style_Last = instr;
		return;
	}
	
	if (loop->prev_style==NULL) {
		Root_Style = instr;
		loop->prev_style = instr;
		instr->next_style = loop;
		return;
	}

	loop->prev_style->next_style = instr;
	instr->next_style = loop;
	instr->prev_style = loop->prev_style;
	loop->prev_style = instr;

	return;
}

GROUP_A *Group_Level(inlevel,insearch)
GROUP_A *inlevel;
UBYTE *insearch;
{

	GROUP_A *looper;

	for (looper=inlevel; looper!=NULL; looper=looper->next) 
		if (match(insearch,looper->Group_Name)) return(looper);

	return(NULL);

}

AttachLine(innode,x1,y1,x2,y2,infl)
STDFLT x1,y1,x2,y2;
int infl;
struct drawing_node *innode;
{

	struct line_mode createline;
	struct drawing_node *newpool;

	lastnode = NULL;

	createline.line_flags=infl;
	createline.x1=x1;
	createline.y1=y1;
	createline.x2=x2;
	createline.y2=y2;

	newpool=StoStrLine(&createline);

	UpdateTitle();
	if (CurrentRoller!=NULL) {
		CurrentRoller->node_flags &= (~NODE_SELECTED);
		v_reset();
		GenericRender(CurrentRoller);
	}

	CurrentRoller=newpool;
	CurrentRoller->node_flags |= NODE_SELECTED;
	v_reset();
	GenericRender(CurrentRoller);
	GenericRender(innode);
	lastnode = newpool;

	/* Delete node off end, and respice after innode */

	if (Last_Node->node_previous!=NULL) {
		Last_Node->node_previous->node_subsequent = NULL;
		Last_Node = Last_Node->node_previous;
	} else 	Last_Node = Root_Node = NULL;

	if (innode->node_subsequent==NULL) {	/* Put back on end */
		innode->node_subsequent = newpool;
		Last_Node = newpool;
	} else {				/* Link into list */
		newpool->node_subsequent = innode->node_subsequent;
		newpool->node_previous = innode;
		innode->node_subsequent->node_previous = newpool;
		innode->node_subsequent = newpool;
	}
	
}
	
CreateLine(x1,y1,x2,y2,infl)
STDFLT x1,y1,x2,y2;
int infl;
{

	struct line_mode createline;
	struct drawing_node *newpool;

	lastnode = NULL;

	createline.line_flags=infl;
	createline.x1=x1;
	createline.y1=y1;
	createline.x2=x2;
	createline.y2=y2;

	newpool=StoStrLine(&createline);

	if (newpool==NULL) {
		printf("No memory to create that line!\n");
		return;
	}

	if (!quietupdate) {
		UpdateTitle();
		if (CurrentRoller!=NULL) {
			CurrentRoller->node_flags &= (~NODE_SELECTED);
			v_reset();
			GenericRender(CurrentRoller);
		}
		CurrentRoller=newpool;
		CurrentRoller->node_flags |= NODE_SELECTED;
		v_reset();
		GenericRender(CurrentRoller);
	}

	lastnode = newpool;
}

CreateText(text,hx,hy,ang,handle,flipflags,instyle)
UBYTE *text,handle;
STDFLT hx,hy,ang;
UWORD flipflags;
STYLE_A *instyle;
{

	struct text_mode createtext;
	struct drawing_node *newpool;

	lastnode = NULL;

	createtext.letstyle = instyle;
	createtext.Handle_Flag = handle;
	createtext.x1 = hx;
	createtext.y1 = hy;
	createtext.ang = ang;
	createtext.Text_Bits = flipflags;
	createtext.text = GI_DrawAlloc(strlen(text)+2);

	if (createtext.text==NULL) {
		printf("No memory to store the text\n");
		return;
	}

	sprintf(createtext.text,"%s",text);

	newpool = StoStrText(&createtext);
	
	if (newpool==NULL) {
		printf("No memory to store the text\n");
		return;
	}

	if (!quietupdate) {
		UpdateTitle();
		if (CurrentRoller!=NULL) {
			CurrentRoller->node_flags &= (~NODE_SELECTED);
			GenericRender(CurrentRoller);
		}
		CurrentRoller=newpool;
		CurrentRoller->node_flags |= NODE_SELECTED;
		GenericRender(CurrentRoller);
	}

	lastnode = newpool;
}

CreateEllipse(cx,cy,rx,ry,rot,sa,ea)
STDFLT cx,cy,rx,ry,rot,sa,ea;
{

	struct ellip_mode createellipse;
	struct drawing_node *newpool;

	normalize(sa);
	normalize(ea);
	normalize(rot);

	lastnode=NULL;

	createellipse.ellip_flags=0;
	createellipse.x1=cx;
	createellipse.y1=cy;

	createellipse.xradius=rx;
	createellipse.yradius=ry;

	createellipse.rotation=rot;
	createellipse.fromang=sa;
	createellipse.toangle=ea;

	newpool=StoStrEllip(&createellipse);

	if (newpool==NULL) {
		printf("No memory to create that ellipse!\n");
		return;
	}

	if (!quietupdate) {
		UpdateTitle();
		if (CurrentRoller!=NULL) {
			CurrentRoller->node_flags &= (~NODE_SELECTED);
			GenericRender(CurrentRoller);
		}
		CurrentRoller=newpool;
		CurrentRoller->node_flags |= NODE_SELECTED;
		GenericRender(CurrentRoller);
	}

	lastnode = newpool;
}

CreateHatch(hatchdata,insertat)
O_HATCH *hatchdata;
D_NODE	*insertat;
{

	D_NODE *newpool;

	lastnode=NULL;

	newpool=StoStrHatch(hatchdata,insertat);

	if (newpool==NULL) {
		printf("No memory to create that ellipse!\n");
		return;
	}

	if (!quietupdate) {
		UpdateTitle();
		if (CurrentRoller!=NULL) {
			CurrentRoller->node_flags &= (~NODE_SELECTED);
			GenericRender(CurrentRoller);
		}
		CurrentRoller=newpool;
		CurrentRoller->node_flags |= NODE_SELECTED;
		GenericRender(CurrentRoller);
	}

	lastnode = newpool;
}

#define MAXNAMLEN 257

#ifdef OLDMATCH
/*
 * match:
 *  pattern matcher.  Takes a string and a pattern possibly containing
 *  the wildcard characters '*' and '?'.  Returns true if the pattern
 *  matches the string, false otherwise.
 *
 * Input: a string and a wildcard pattern.
 * Returns: 1 if match, 0 if no match.
 */
 
BOOL match(pattern,string) 
char *pattern,*string; 
{
    char *psave,*ssave;			/* back up pointers for failure */
    psave = ssave = NULL;

    FOREVER {
	for (; tolower(*pattern) == tolower(*string); pattern++,string++)  
					/* skip first */
	    if (*string == '\0') return(TRUE);	/* end of strings, succeed */
	if (*string != '\0' && *pattern == '?') {
	    pattern++;			/* '?', let it match */
	    string++;
	} else if (*pattern == '*') {	/* '*' ... */
	    psave = ++pattern;		/* remember where we saw it */
	    ssave = string;		/* let it match 0 chars */
	} else if (ssave != NULL && *ssave != '\0') {	/* if not at end  */
  					/* ...have seen a star */
	    string = ++ssave;		/* skip 1 char from string */
	    pattern = psave;		/* and back up pattern */
	} else return(FALSE);		/* otherwise just fail */
    }
}
#else
/*
 * match -- match wildcard pattern to string
 *    allows multiple '*'s and works without backtracking
 *    upper and lower case considered equivalent
 *    written by Jack Rouse
 *    working without backtracking is cute, but is this usually going
 *       to be the most efficient method?
 */
BOOL match(pattern, target)
register char *pattern, *target;
{
	int link[MAXNAMLEN];		/* list of matches to try in pattern */
	register int first, last;	/* first and last items in list */
	register int here, next;	/* current and next list items */
	char lowch;			/* current target character */

	/* start out trying to match at first position */
	first = last = 0;
	link[0] = -1;

	/* go through the target */
	for (; *target; ++target)
	{
		/* get lowercase target character */
		lowch = tolower(*target);

		/* go through all positions this round and build next round */
		last = -1;
		for (here = first; here >= 0; here = next)
		{
			next = link[here];
			switch (pattern[here])
			{
			case '*':
				/* try match at here+1 this round */
				/*!!!check needed only if "**" allowed? */
				if (next != here + 1)
				{
					link[here + 1] = next;
					next = here + 1;
				}
				/* retry match at here next round */
				break;
			default:
				if (tolower(pattern[here]) != lowch)
					continue;
				/* matched, fall through */
			case '?':
				/* try match at here+1 next round */
				++here;
				break;
			}
			/* try match at here value next round */
			if (last < 0)
				first = here;
			else
				link[last] = here;
			last = here;
		}
		/* if no positions left, match failed */
		if (last == -1) return(0);
		/* terminate list */
		link[last] = -1;
	}

	/* at end of target, skip empty matches */
	while (pattern[last] == '*')
		++last;

	return(pattern[last] == '\0');
}
#endif

int asccmp(in1,in2)
char *in1,*in2;
{

	FOREVER {
		if (*in1!='\0'&&*in2=='\0') return(1);
		if (*in2!='\0'&&*in1=='\0') return(-1);
		if (*in2=='\0'&&*in1=='\0') return(0);
		if (toupper(*in1)>toupper(*in2)) return(1);
		if (toupper(*in2)>toupper(*in1)) return(-1);
		in1++; in2++;
	}
}

VOID sethatchgroup(ingroup,setgrp)
D_NODE *ingroup,*setgrp;
{

	O_LINE *thisobj,*lineobj;
	D_NODE *wander,*hatchd;

	hatchd = ingroup;
	if (ingroup==NULL) return;
	ingroup = ingroup->node_subsequent;
	if (ingroup==NULL) return;

	if (ingroup->node_id==NODE_ELLIP) {
		ingroup->above_group = setgrp;
		return;
	}

	if (ingroup->node_id!=NODE_LINE) return;

	thisobj = GetLineData(ingroup);
	for (wander = ingroup; wander!=NULL; wander=wander->node_subsequent){
		if (wander->node_id!=NODE_LINE) break;
		lineobj = GetLineData(wander);
		if ((thisobj!=lineobj) &&
		    ((lineobj->x1!=thisobj->x2) ||
		     (lineobj->y1!=thisobj->y2))) break;
		wander->above_group = setgrp;
	}
}

BOOL ishatched(inhatch)
D_NODE *inhatch;
{

	O_LINE *thisobj,*lineobj;
	D_NODE *wander;
	
	/* Wander backwards to beginning of the list */
	
	if (inhatch->node_id==NODE_ELLIP) {
		wander = inhatch->node_previous;
		if (wander==NULL) return(FALSE);
		if (wander->node_id==NODE_HATCH) return(TRUE);
		return(FALSE);
	}
	
	if (inhatch->node_id!=NODE_LINE) return(FALSE);
	
	thisobj = GetLineData(inhatch);
	for (wander = inhatch->node_previous; wander!=NULL; 
	    wander=wander->node_previous) {
	    	if (wander->node_id==NODE_HATCH) return(TRUE);
		if (wander->node_id!=NODE_LINE) break;
		lineobj = GetLineData(wander);
		if ((lineobj->x2==thisobj->x1) &&
		    (lineobj->y2==thisobj->y1)) {
		    	inhatch = wander;	/* Walk back */
		    	thisobj = lineobj;
		} else return(FALSE);
	}
	
	return(FALSE);
}
	
D_NODE *gethatch(inhatch)
D_NODE *inhatch;
{

	O_LINE *thisobj,*lineobj;
	D_NODE *wander;
	
	/* Wander backwards to beginning of the list */
	
	if (inhatch->node_id==NODE_ELLIP) {
		wander = inhatch->node_previous;
		if (wander==NULL) return(FALSE);
		if (wander->node_id==NODE_HATCH) return(wander);
		return(FALSE);
	}
	
	if (inhatch->node_id!=NODE_LINE) return(NULL);
	
	thisobj = GetLineData(inhatch);
	for (wander = inhatch->node_previous; wander!=NULL; 
	    wander=wander->node_previous) {
	    	if (wander->node_id==NODE_HATCH) return(wander);
		if (wander->node_id!=NODE_LINE) break;
		lineobj = GetLineData(wander);
		if ((lineobj->x2==thisobj->x1) &&
		    (lineobj->y2==thisobj->y1)) {
		    	inhatch = wander;	/* Walk back */
		    	thisobj = lineobj;
		} else return(NULL);
	}
	
	return(NULL);
}

FILE *openprt(name)
UBYTE *name;
{

	UBYTE outname[100];
	FILE *newfile;
	
	sprintf(outname,"%s%s.rpt",ReportDev,name);
	
	newfile = fopen(outname,"w");

	if (newfile==NULL) {
		complain(NULL,"Recover",4,0,0,0,
			"Cannot open print device",
			outname,NULL,NULL);
		return(NULL);
	}
	
	return(newfile);
	
}

