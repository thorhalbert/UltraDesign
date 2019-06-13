/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	stylehandle - basic structure maint/functions
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
 *
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

STYLE_A *Style_Parent(current_in,inmask,descend)
STYLE_A *current_in;
UBYTE *inmask;
BOOL descend;
{

	return(NULL);

}

STYLE_A *Style_Next(current_in,inmask)
STYLE_A *current_in;
UBYTE *inmask;
{

	FOREVER {
		current_in=current_in->next_style;
		if (current_in==NULL) return(NULL);
	
		if (match(inmask,current_in->Letter_Style_ID))
			return(current_in);

	}
}

STYLE_A *Style_Prev(current_in,inmask)
STYLE_A *current_in;
UBYTE *inmask;
{

	FOREVER {
		current_in=current_in->prev_style;
		if (current_in==NULL) return(NULL);
	
		if (match(inmask,current_in->Letter_Style_ID))
			return(current_in);

	}
}

STYLE_A *Style_First(current_in,inmask,descend)
STYLE_A *current_in;
UBYTE *inmask;
BOOL descend;
{
	current_in = Root_Style;

	if (match(inmask,current_in->Letter_Style_ID))
		return(current_in);

	return(Style_Next(Root_Style,inmask));

}

STYLE_A *Style_Search(insearch)
UBYTE *insearch;
{

	STYLE_A *looper;

	for (looper=Root_Style; looper!=NULL; looper=looper->next_style) 
		if (match(insearch,looper->Letter_Style_ID)) return(looper);

	return(NULL);

}

STYLE_A *Style_Descend(current_in,inmask,descend)
STYLE_A *current_in;
UBYTE *inmask;
BOOL descend;
{

	return(NULL);

}

VOID Style_Form(current_in,outform)
STYLE_A *current_in;
UBYTE *outform;
{

	UBYTE odis[140];
	sprintf(odis,"%-32.32s %4d %s",
		current_in->Letter_Style_ID,current_in->Use_Count,
		current_in->draw_font->Font_Name_ID);
	odis[59]='\0';
	sprintf(outform,"%s",odis);
}


VOID Style_Name(current_in,outform)
STYLE_A *current_in;
UBYTE *outform;
{

	sprintf(outform,"%s",current_in->Letter_Style_ID);

}


Unlink_Style(instyle)
STYLE_A *instyle;
{

	if (instyle->next_style!=NULL) {
		if (instyle->prev_style!=NULL) {
			instyle->prev_style->next_style=
				instyle->next_style;
			instyle->next_style->prev_style=
				instyle->prev_style;
		} else {
			Root_Style=instyle->next_style;
			instyle->next_style->prev_style=NULL;
		}
	} else {
		if (instyle->prev_style!=NULL) {
			instyle->prev_style->next_style=NULL;
			Style_Last=instyle->prev_style;
		} else {
			Root_Style = Style_Last = NULL;
		}
	}
}

/*------------------------Set up scroller for fonts------------------------*/

struct Text_Fonts *Stroke_Parent(current_in,inmask,descend)
struct Text_Fonts *current_in;
UBYTE *inmask;
BOOL descend;
{

	return(NULL);

}

struct Text_Fonts *Stroke_Next(current_in,inmask)
struct Text_Fonts *current_in;
UBYTE *inmask;
{

	FOREVER {
		current_in=current_in->next_font;
		if (current_in==NULL) return(NULL);
	
		if (match(inmask,current_in->Font_Name_ID))
			return(current_in);

	}
}

struct Text_Fonts *Stroke_Prev(current_in,inmask)
struct Text_Fonts *current_in;
UBYTE *inmask;
{

	FOREVER {
		current_in=current_in->prev_font;
		if (current_in==NULL) return(NULL);
	
		if (match(inmask,current_in->Font_Name_ID))
			return(current_in);

	}
}

struct Text_Fonts *Stroke_First(current_in,inmask,descend)
struct Text_Fonts *current_in;
UBYTE *inmask;
BOOL descend;
{
	current_in = Root_Font;

	if (match(inmask,current_in->Font_Name_ID))
		return(current_in);

	return(Stroke_Next(current_in,inmask));

}

struct Text_Fonts *Stroke_Search(insearch)
UBYTE *insearch;
{

	struct Text_Fonts *looper;

	for (looper=Root_Font; looper!=NULL; looper=looper->next_font) 
		if (match(insearch,looper->Font_Name_ID)) return(looper);

	return(NULL);

}

struct Text_Fonts *Stroke_Descend(current_in,inmask,descend)
struct Text_Fonts *current_in;
UBYTE *inmask;
BOOL descend;
{

	return(NULL);

}

VOID Stroke_Form(current_in,outform)
struct Text_Fonts *current_in;
UBYTE *outform;
{

	sprintf(outform,"%-32.32s %4d %5d",
		current_in->Font_Name_ID,current_in->Use_Count,
			current_in->Font_Size);

}


VOID Stroke_Name(current_in,outform)
struct Text_Fonts *current_in;
UBYTE *outform;
{

	sprintf(outform,"%s",current_in->Font_Name_ID);

}

