/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	iffget0 - get a type 0 iff file
 *
 */

/*
 *	Copyright (C) 1986, 1987 by WURBsoft Inc., and Scott Halbert.
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
 *	1	11-Jan-87 SH	Beginning of History
 *	2	 1-Jun-87 SH	Start on rev 2 of development iff format
 *	3	22-Jul-87 SH	Remove some stack variables
 *	4	 5-Aug-87 SH	Divide out of 2dviewiff
 *	5	 6-Aug-87 SH	Make more bulletproof
 *	6	10-Aug-87 SH	Add library load
 *	7	 2-Oct-87 SH	Create production version 1.0 iff format
 *	8	13-Nov-87 SH	Port over for loader
 *	9	19-Apr-90 SH	Double click on load file crashes
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "plotsetup.h"
#include "viewiff.h"
	
/*----------------------------------Macros-----------------------------------*/

#define CkErr(expression) { if (ifferror==IFF_OKAY) ifferror=(expression);}
#define div32(x) ((x)>>5)
#define rem32(x) ((x)&0x1F)
#define STACKBUFF 1024

UBYTE *DrawMem();
#define rmalloc(x) AllocMem(x,0)
#define rmfree(x,s) FreeMem(x,s)

#define BombLoad(a) printf("\007Error *** %s ***\n",a)

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

struct Text_Fonts *Stroke_Search();
BOOL LoadStrokeFont();

/*-------------------------Global Variable Definitions-----------------------*/

IFFP ifferror;
IFFP iffp;

LOCAL WORD group_m,pen_m,style_m,layer_m;

LOCAL int maxcols;

/*-------------------------------Code Section--------------------------------*/

IFFP IffErr();
float infloat();

/*-------------------------Version 0 support------------------------------*/

LOCAL WORD group_c,pen_c,style_c,layer_c,pen_t,style_t;

struct loadchunk {
	UBYTE 	*getchunk;
	ULONG	sizechunk;
} insave;

UBYTE *StoreComment(instr)
UBYTE *instr;
{

	UBYTE 	*ost;
	int	onum;

	if (instr==NULL) onum = 1;
	else	onum = strlen(instr)+1;

	ost = DrawMem(onum);
	if (ost==NULL) return(NULL);

	if (instr==NULL) ost[0]='\0';
	else	sprintf(ost,"%s",instr);

	return(ost);
}

VOID FreeChunk(inload)
struct loadchunk inload;
{

	rmfree(inload.getchunk,inload.sizechunk);

}

struct loadchunk StuffChunk(inchunk)
GroupContext *inchunk;
{

	struct loadchunk answer;

	answer.sizechunk = ChunkMoreBytes(inchunk);

	answer.getchunk = rmalloc(answer.sizechunk);
	if (answer.getchunk==NULL) {
		BombLoad("Insufficient memory to load file");
		return(answer);
	}

	iffp=IFFReadBytes(inchunk,answer.getchunk,answer.sizechunk);
	if (iffp!=IFF_OKAY) {
		BombLoad("IFF read failed");
		FreeChunk(answer);
		answer.getchunk = NULL;
		return(answer);
	}		

	return(answer);
}
	
IFFP loadLIMS_0(groupin)
GroupContext *groupin;
{

	struct iff_lims_0 *limbuf;
	int t;

	insave = StuffChunk(groupin);
	limbuf = (struct iff_lims_0 *) insave.getchunk;

	if (limbuf==NULL) return(CLIENT_ERROR);		/* Out of memory */

/*	printf("LIMS0\n");	*/

	switch (limbuf->defrev) {
	case LIMS0_REV:

		if (insave.sizechunk<sizeof(struct iff_lims_0)) {
			FreeChunk(insave);
			BombLoad("Limit record illegal size");
			return(CLIENT_ERROR);
		}
			
	/*	xsnap = PORTtoF(&limbuf->xsnap);
		ysnap = PORTtoF(&limbuf->ysnap);
		xoffset = PORTtoF(&limbuf->xoffs);
		yoffset = PORTtoF(&limbuf->yoffs);
		snaprot = PORTtoF(&limbuf->snaprot);
		
		xgrid = PORTtoF(&limbuf->xgrid);
		ygrid = PORTtoF(&limbuf->ygrid);
		xgridoff = PORTtoF(&limbuf->xgoff);
		ygridoff = PORTtoF(&limbuf->ygoff);
		xgriddiv = limbuf->xgdiv;
		ygriddiv = limbuf->ygdiv;
		gridrot = PORTtoF(&limbuf->gridrot);	*/
	
		Current_View->XViewMIN = PORTtoF(&limbuf->minx);
		Current_View->YViewMIN = PORTtoF(&limbuf->miny);
		Current_View->XViewMAX = PORTtoF(&limbuf->maxx);
		Current_View->YViewMAX = PORTtoF(&limbuf->maxy);
	
	/*	qtextlim = limbuf->qtextlim;
	
		paperunit = PORTtoF(&limbuf->paperunit);
		viewunit = PORTtoF(&limbuf->viewunit);		*/

		Current_View->ViewScale = PORTtoF(&limbuf->viewscale);	
	
		Current_View->ArrowAngle = PORTtoF(&limbuf->arrowangle);
		Current_View->Arrowlen = PORTtoF(&limbuf->arrowlen);
	
	/*	orthoangle = PORTtoF(&limbuf->orthoang);
		filletrad = PORTtoF(&limbuf->filletrad);
	
		lintolerance = PORTtoF(&limbuf->lintolerance);
		angtolerance = PORTtoF(&limbuf->angtolerance);
		linpoints = limbuf->linpoints;
		angpoints = limbuf->angpoints;
		lintype = limbuf->lintype;
		angtype = limbuf->angtype;
	
		skew=1.0;

		Center_On=Handles_On=Coord_On=
			Snaps_On=Grid_On=Use_Bounds=FALSE;
		Ortho_Switch=FALSE;

		if (limbuf->viewflags & LIM0FLAG_CCEN) Center_On=TRUE;
		if (limbuf->viewflags & LIM0FLAG_HAND) Handles_On=TRUE;
		if (limbuf->viewflags & LIM0FLAG_COOR) Coord_On=TRUE;
		if (limbuf->viewflags & LIM0FLAG_SNAP) Snaps_On=TRUE;
		if (limbuf->viewflags & LIM0FLAG_GRID) Grid_On=TRUE;
		if (limbuf->viewflags & LIM0FLAG_BOUN) Use_Bounds=TRUE;
		if (limbuf->viewflags & LIM0FLAG_ORTH) Ortho_Switch=TRUE;
	
		Color_Source = limbuf->takecolors;	*/

		group_m=pen_m=style_m=layer_m=0;
		group_c=pen_c=style_c=layer_c=pen_t=style_t=0;
	
		group_c = limbuf->current_group;
		pen_c	= limbuf->current_pen;
		style_c	= limbuf->current_style;
		layer_c	= limbuf->current_layer;
		pen_t	= limbuf->template_pen;
		style_t	= limbuf->template_style;
	
		group_m	= limbuf->max_group+1;
		layer_m = limbuf->max_layer+1;
		pen_m	= limbuf->max_pen+1;
		style_m	= limbuf->max_style+1;
		FreeChunk(insave);

		break;

	default:	FreeChunk(insave);
			BombLoad("Unrecognized limit record");	
			return(CLIENT_ERROR);	/* Unrecognized iff */
	}
	
	Current_View->Group_Array = (struct Group_Attrib **) 
		DrawMem(sizeof(struct Group_Attrib *) * group_m);
	Current_View->num_groups = group_m;

	if (Current_View->Group_Array==NULL) {
		printf("Couldn't allocate the Group_Array %d\n",group_m);
		return(CLIENT_ERROR);
	}

	Current_View->Layer_Array = (struct Layer_Attrib **)
		DrawMem(sizeof(struct Layer_Attrib *) * layer_m);
	Current_View->num_layers = layer_m;

	if (Current_View->Layer_Array==NULL) {
		printf("Couldn't allocate the Layer_Array %d\n",layer_m);
		return(CLIENT_ERROR);
	}

	Current_View->Style_Array = (struct Lettering_Style **)
		DrawMem(sizeof(struct Lettering_Styles *) * style_m);
	Current_View->num_styles = style_m;

	if (Current_View->Style_Array==NULL) {
		printf("Couldn't allocate the Style_Array %d\n",style_m);
		return(CLIENT_ERROR);
	}

	Current_View->Pen_Array = (struct Pen_Attrib **)
		DrawMem(sizeof(struct Pen_Attrib *) * pen_m);
	Current_View->num_pens = pen_m;

	if (Current_View->Pen_Array==NULL) {
		printf("Couldn't allocate the Pen_Array %d\n",pen_m);
		return(CLIENT_ERROR);
	}

	for (t=0; t<group_m; t++) Current_View->Group_Array[t]=NULL;
	for (t=0; t<style_m; t++) Current_View->Style_Array[t]=NULL;
	for (t=0; t<layer_m; t++) Current_View->Layer_Array[t]=NULL;
	for (t=0; t<pen_m; t++) Current_View->Pen_Array[t]=NULL;

	return(IFF_OKAY);
}

IFFP loadBMEM_0(groupin)
GroupContext *groupin;
{

	struct iff_bmem_0 *bmembuf;

/*	printf("BMEM\n");	*/

	insave = StuffChunk(groupin);
	bmembuf = (struct iff_bmem_0 *) insave.getchunk;

	if (bmembuf==NULL) return(CLIENT_ERROR);	/* Out of memory */

	FreeChunk(insave);

	return(IFF_OKAY);
}

IFFP loadDIMS_0(groupin)
GroupContext *groupin;
{

	struct iff_dims_0 *dimsbuf;

/*	printf("DIMS\n");	*/

	insave = StuffChunk(groupin);
	dimsbuf = (struct iff_dims_0 *) insave.getchunk;

	if (dimsbuf==NULL) return(CLIENT_ERROR);	/* Out of memory */

	FreeChunk(insave);

	return(IFF_OKAY);
}

IFFP loadPALE_0(groupin)
GroupContext *groupin;
{

	struct iff_pale_0 *palebuf;

/*	printf("PALE\n");	*/

	insave = StuffChunk(groupin);
	palebuf = (struct iff_pale_0 *) insave.getchunk;

	if (palebuf==NULL) return(CLIENT_ERROR);	/* Out of memory */

	FreeChunk(insave);

	return(IFF_OKAY);
}

IFFP ProcU2VI_0(groupin)
GroupContext *groupin;
{
	GroupContext thiscontext;

	iffp=OpenRGroup(groupin, &thiscontext);
	CheckIFFP();

	do
		switch (iffp=GetF1ChunkHdr(&thiscontext)) {

		case COPY_0:
			break;

		case LIMS_0:
			iffp=loadLIMS_0(&thiscontext);
			CheckIFFP();
			break;

		} while (iffp >= IFF_OKAY);

	return(CloseRGroup(&thiscontext));	

}

IFFP ProcGetU2VI_0(groupin)
GroupContext *groupin;
{
	GroupContext thiscontext;

	iffp=OpenRGroup(groupin, &thiscontext);
	CheckIFFP();

	do
		switch (iffp=GetF1ChunkHdr(&thiscontext)) {

		case COPY_0:
			break;

		case LIMS_0:
			break;

		} while (iffp >= IFF_OKAY);

	return(CloseRGroup(&thiscontext));	

}

IFFP loadGRPI_0(groupin)
GroupContext *groupin;
{

	struct iff_grpi_0 *grpbuf;
	struct Group_Attrib *newgroup;
	int t;

	insave = StuffChunk(groupin);
	grpbuf = (struct iff_grpi_0 *) insave.getchunk;

	if (grpbuf==NULL) return(CLIENT_ERROR);	/* Out of memory */

/*	printf("GRPI0\n");	*/

	switch (grpbuf->defrev) {
	case GRPI0_REV:

		t = sizeof(struct iff_grpi_0) - 255;
		t += strlen(grpbuf->Comment);

		if (insave.sizechunk<t) {
			FreeChunk(insave);
			BombLoad("Group record illegal size");
			return(CLIENT_ERROR);
		}

		newgroup = (struct Group_Attrib *) 
				DrawMem(sizeof(struct Group_Attrib));

		if (newgroup==NULL) {
			printf("No memory to save group id %s",
				grpbuf->GroupName);
			return(CLIENT_ERROR);
		}

		newgroup->Group_ID = grpbuf->GroupID;
		sprintf(newgroup->Group_Name,"%s",grpbuf->GroupName);
		newgroup->Color_ID = grpbuf->Color_ID;
		newgroup->Group_Flags = grpbuf->Group_Flags;
		newgroup->Libr_Flags = grpbuf->Libr_Flags;
		newgroup->Use_Count = 0;
		newgroup->parent = (int) grpbuf->Parent;
		newgroup->next = (int) grpbuf->Next;
		newgroup->previous = (int) grpbuf->Previous;
		newgroup->descendants = (int) grpbuf->Descendants;
		newgroup->desc_last = NULL;
		newgroup->Comment = StoreComment(grpbuf->Comment);
	
		t = newgroup->Group_ID;
		if (t<0&&t>group_m) {
			printf("Illegal group code %d on group %s\n",
				t,newgroup->Group_Name);
			t = 1;
		}

		Current_View->Group_Array[t] = newgroup;

		FreeChunk(insave);
		break;

	default:	FreeChunk(insave);
			BombLoad("Unrecognized group record");	
			return(CLIENT_ERROR);	/* Unrecognized iff */
	}

	return(IFF_OKAY);

}

IFFP loadLYER_0(groupin)
GroupContext *groupin;
{

	struct iff_lyer_0 *layerbuf;
	struct Layer_Attrib *newlayer;
	int t;

	insave = StuffChunk(groupin);
	layerbuf = (struct iff_lyer_0 *) insave.getchunk;

	if (layerbuf==NULL) return(CLIENT_ERROR);	/* Out of memory */

/*	printf("LYER0\n");	*/

	switch (layerbuf->defrev) {
	case LYER0_REV:

		t = sizeof(struct iff_lyer_0) - 255;
		t += strlen(layerbuf->Comment);

		if (insave.sizechunk<t) {
			FreeChunk(insave);
			BombLoad("Layer record illegal size");
			return(CLIENT_ERROR);
		}

		newlayer = (struct Layer_Attrib *) 
				DrawMem(sizeof(struct Layer_Attrib));
	
		if (newlayer==NULL) {
			printf("No memory to save layer id %s",layerbuf->LayerName);
			return(CLIENT_ERROR);
		}
			
		newlayer->Layer_ID = layerbuf->LayerID;
		sprintf(newlayer->Layer_Name,"%s",layerbuf->LayerName);
		newlayer->Color_ID = layerbuf->Color_ID;
		newlayer->Layer_Flags = layerbuf->Layer_Flags;
		newlayer->Use_Count = 0;
		newlayer->layer_next = NULL;
		newlayer->Comment = StoreComment(layerbuf->Comment);

		Current_View->Layer_Array[newlayer->Layer_ID] = newlayer;

		FreeChunk(insave);
		break;

	default:	FreeChunk(insave);
			BombLoad("Unrecognized layer record");	
			return(CLIENT_ERROR);	/* Unrecognized iff */
	}

	return(IFF_OKAY);
}

fix_logical(inpen)
PEN_A *inpen;
{

	ULONG makecolor;
	struct logical_pen	*lpen;
	
	lpen = &(inpen->pass_pen);
	makecolor = inpen->Hard_Color;
	
	lpen->Pen_Blue = (makecolor & 0xFF) << 8;
	makecolor >>= 8;
	lpen->Pen_Green = (makecolor & 0xFF) << 8;
	makecolor >>= 8;
	lpen->Pen_Red = (makecolor & 0xFF) << 8;
	
	lpen->Order = inpen->PenOrd;
	lpen->Carousel = inpen->Carousel;
	lpen->AutoNum = inpen->AutoNum;
	
	lpen->Line_Width = inpen->PenWidth;
	lpen->Line_Pattern = inpen->Line_Pattern;
	lpen->Bit_Width = inpen->BitWidth;
	
	lpen->Identity = 0;
	lpen->IdentPtr = NULL;

}


IFFP loadPENS_0(groupin)
GroupContext *groupin;
{

	struct iff_pens_0_0 *penbuf0;
	struct iff_pens_0_1 *penbuf1;
	PEN_A *newpen;
	int t;

	insave = StuffChunk(groupin);
	penbuf0 = (struct iff_pens_0_0 *) insave.getchunk;

	if (penbuf0==NULL) return(CLIENT_ERROR);	/* Out of memory */

/*	printf("PENS0\n");	*/

	switch (penbuf0->defrev) {
	case PENS0_REV0:

		t = sizeof(struct iff_pens_0_0) - 255;
		t += strlen(penbuf0->Comment);

		if (insave.sizechunk<t) {
			FreeChunk(insave);
			BombLoad("Pen record illegal size");
			return(CLIENT_ERROR);
		}

		newpen = (PEN_A *) 
				DrawMem(sizeof(PEN_A));

		if (newpen==NULL) {
			printf("No memory to save pen id %s",penbuf0->PenName);
			return(CLIENT_ERROR);
		}
	
		newpen->Pen_ID = penbuf0->PenID;
		sprintf(newpen->Pen_Name,"%s",penbuf0->PenName);
		newpen->Color_ID = penbuf0->Color_ID;
		newpen->Pen_Flags = penbuf0->Pen_Flags;
		newpen->Line_Pattern = penbuf0->Pattern1;
		newpen->Use_Count = 0;
		newpen->pen_next = NULL;
		newpen->pen_prev = NULL;
		newpen->Comment = StoreComment(penbuf0->Comment);
		newpen->PenOrd = 0;
		newpen->Carousel = 0;
		newpen->AutoNum = 0;
		newpen->BitWidth = 1.0;
		newpen->PenWidth = 0.5;
		newpen->Hard_Color = 0;		/* Black */
		
		fix_logical(newpen);
	
		Current_View->Pen_Array[newpen->Pen_ID] = newpen;

		FreeChunk(insave);
		break;
	case PENS0_REV1:

		penbuf1 = (struct iff_pens_0_1 *) insave.getchunk;
		t = sizeof(struct iff_pens_0_1) - 255;
		t += strlen(penbuf1->Comment);

		if (insave.sizechunk<t) {
			FreeChunk(insave);
			BombLoad("Pen record illegal size");
			return(CLIENT_ERROR);
		}

		newpen = (PEN_A *) 
				DrawMem(sizeof(PEN_A));

		if (newpen==NULL) {
			printf("No memory to save pen id %s",penbuf1->PenName);
			return(CLIENT_ERROR);
		}
	
		newpen->Pen_ID = penbuf1->PenID;
		sprintf(newpen->Pen_Name,"%s",penbuf1->PenName);
		newpen->Color_ID = penbuf1->Color_ID;
		newpen->Pen_Flags = penbuf1->Pen_Flags;
		newpen->Line_Pattern = penbuf1->Pattern1;
		newpen->Hard_Color = penbuf1->HardColor;
		newpen->Use_Count = 0;
		newpen->pen_next = NULL;
		newpen->pen_prev = NULL;
		newpen->Comment = StoreComment(penbuf1->Comment);
		newpen->PenWidth = PORTtoF(&(penbuf1->PenWidth));
		newpen->BitWidth = PORTtoF(&(penbuf1->BitWidth));
		newpen->PenOrd = penbuf1->PenOrd;
		newpen->Carousel = penbuf1->Carousel;
		newpen->AutoNum = penbuf1->AutoNum;
		
		fix_logical(newpen);
	
		Current_View->Pen_Array[newpen->Pen_ID] = newpen;
		FreeChunk(insave);
		break;

	default:	FreeChunk(insave);
			BombLoad("Unrecognized penstyle record");	
			return(CLIENT_ERROR);	/* Unrecognized iff */
	}

	return(IFF_OKAY);

}

IFFP loadTXST_0(groupin)
GroupContext *groupin;
{

	struct iff_txst_0 *txtbuf;
	struct Lettering_Styles *newtext;
	int t;

	insave = StuffChunk(groupin);
	txtbuf = (struct iff_txst_0 *) insave.getchunk;

	if (txtbuf==NULL) return(CLIENT_ERROR);	/* Out of memory */

/*	printf("TXST0\n");	*/
	
	switch (txtbuf->defrev) {
	case TXST0_REV:

		t = sizeof(struct iff_txst_0) - 255;
		t += strlen(txtbuf->Comment);

		if (insave.sizechunk<t) {
			FreeChunk(insave);
			BombLoad("Pen record illegal size");
			return(CLIENT_ERROR);
		}

		newtext = (struct Lettering_Styles *) 
				DrawMem(sizeof(struct Lettering_Styles));

		if (newtext==NULL) {
			printf("No memory to save lettering style %s",txtbuf->StyleName);
			return(CLIENT_ERROR);
		}

		newtext->Letter_ID = txtbuf->LetterID;
		sprintf(newtext->Letter_Style_ID,"%s",txtbuf->StyleName);
		newtext->Letter_Flags = txtbuf->TextFlags;
		newtext->Prime_Height=PORTtoF(&(txtbuf->Prime_Height));
		newtext->Slant=PORTtoF(&(txtbuf->Slant_Angle));
		newtext->Aspect=PORTtoF(&(txtbuf->Aspect));
		newtext->Use_Count = 0;
		newtext->next_style = NULL;
		newtext->Comment = StoreComment(txtbuf->Comment);

		Current_View->Style_Array[newtext->Letter_ID] = newtext;
		newtext->draw_font = Stroke_Search(txtbuf->FontName);
		if (newtext->draw_font==NULL) {
			if (!LoadStrokeFont(txtbuf->FontName)) 
				newtext->draw_font = Root_Font;
			else {
				newtext->draw_font = 
					Stroke_Search(txtbuf->FontName);
				if (newtext->draw_font==NULL) 
					newtext->draw_font = Root_Font;
			}
		}

		FreeChunk(insave);
		break;

	default:	FreeChunk(insave);
			BombLoad("Unrecognized textstyle record");	
			return(CLIENT_ERROR);	/* Unrecognized iff */
	}
	return(IFF_OKAY);
}

IFFP loadPATT_0(groupin)
GroupContext *groupin;
{

	struct iff_patt_0 *pattbuf;
	UBYTE nameagg[100];
	T_HATCH *gethat;
	int t;

	/* DCK("PATT0") */
	
	insave = StuffChunk(groupin);
	pattbuf = (struct iff_patt_0 *) insave.getchunk;

	if (pattbuf==NULL) return(CLIENT_ERROR);	/* Out of memory */

	switch (pattbuf->defrev) {
	case PATT0_REV:

		t = sizeof(struct iff_patt_0);

		if (insave.sizechunk<t) {
			FreeChunk(insave);
			BombLoad("Hatch record illegal size");
			return(CLIENT_ERROR);
		}

		gethat = Hatch_Search_File(pattbuf->File_Name);
		
		if (gethat==NULL)
			(VOID) LoadHatch(pattbuf->File_Name);
		
		sprintf(nameagg,"%s&%s",pattbuf->File_Name,
					pattbuf->Hatch_Name);
		
		gethat = Hatch_Search(nameagg);
	
		if (gethat==NULL)
			complain(NULL,"Recover",4,3,0,0,
				"Cannot find hatch pattern - why?",
				pattbuf->File_Name,
				pattbuf->Hatch_Name,NULL);
		else gethat->Hatch_ID = pattbuf->PatternID;
		
		FreeChunk(insave);
		break;

	default:	FreeChunk(insave);
			BombLoad("Unrecognized pattern record");	
			return(CLIENT_ERROR);	/* Unrecognized iff */
	}

	return(IFF_OKAY);
}

IFFP ProcATTR_0(incontext)
GroupContext *incontext;
{
	GroupContext thiscontext;

	iffp=OpenRGroup(incontext, &thiscontext);
	CheckIFFP();

	do
		switch (iffp=GetF1ChunkHdr(&thiscontext)) {

		case GRPI_0:
			iffp=loadGRPI_0(&thiscontext);
			CheckIFFP();
			break;

		case LYER_0:
			iffp=loadLYER_0(&thiscontext);
			CheckIFFP();
			break;

		case PENS_0:
			iffp=loadPENS_0(&thiscontext);
			CheckIFFP();
			break;

		case TXST_0:
			iffp=loadTXST_0(&thiscontext);
			CheckIFFP();
			break;
			
		case PATT_0:
			iffp=loadPATT_0(&thiscontext);
			CheckIFFP();
			break;

		} while (iffp >= IFF_OKAY);

	return(CloseRGroup(&thiscontext));	
	
}

IFFP ProcGetATTR_0(incontext)
GroupContext *incontext;
{
	GroupContext thiscontext;

	iffp=OpenRGroup(incontext, &thiscontext);
	CheckIFFP();

	do
		switch (iffp=GetF1ChunkHdr(&thiscontext)) {

		case GRPI_0:
			break;

		case LYER_0:
			break;

		case PENS_0:
			break;

		case TXST_0:
			break;
			
		case PATT_0:
			break;

		} while (iffp >= IFF_OKAY);

	return(CloseRGroup(&thiscontext));	
	
}

IFFP GetForms(incontext)
GroupContext *incontext;
{

	switch (incontext->subtype) {

	case U2VI_0:	
/*		printf("*** Type 0 File Set ***\n");	*/
		return(ProcU2VI_0(incontext));
	case ATTR_0:
/*		printf("Attributes seen\n");	*/
		return(ProcATTR_0(incontext));

	default:	return(IFF_OKAY);
	}

	return(IFF_OKAY);

}

IFFP GetGetForms(incontext)
GroupContext *incontext;
{

	switch (incontext->subtype) {

	case U2VI_0:	return(ProcGetU2VI_0(incontext));

	case ATTR_0:	return(ProcGetATTR_0(incontext));

	case OBJE_0:	return(ProcOBJE_0(incontext));

	default:	return(IFF_OKAY);
	}

	return(IFF_OKAY);

}

IFFP loadVECT_0(groupin)
GroupContext *groupin;
{

	struct iff_vect_0 *vectbuf;
	int  looper,gx;
	int vld=0,fg;
	STDFLT p1x,p1y,p2x,p2y;
	int t;

	insave = StuffChunk(groupin);
	vectbuf = (struct iff_vect_0 *) insave.getchunk;

	if (vectbuf==NULL) return(CLIENT_ERROR);	/* Out of memory */

/*	printf("VECT0\n");	*/
	
	switch (vectbuf->defrev) {
	case VECT0_REV:

		t=sizeof(UWORD)*2;
		t+=(sizeof(struct _v0bundle)) * vectbuf->NumberVects;

		if (insave.sizechunk<t) {
			FreeChunk(insave);
			BombLoad("Vector record illegal size");
			return(CLIENT_ERROR);
		}

		for (looper=0; looper<vectbuf->NumberVects; looper++) {
		
			gx = vectbuf->vectbundles[looper].Group_ID;

			fg = vectbuf->vectbundles[looper].VecFlags;
			p1x=PORTtoF(&(vectbuf->vectbundles[looper].p1x));
			p1y=PORTtoF(&(vectbuf->vectbundles[looper].p1y));
			p2x=PORTtoF(&(vectbuf->vectbundles[looper].p2x));
			p2y=PORTtoF(&(vectbuf->vectbundles[looper].p2y));

			Current_Group = Current_View->Group_Array[gx];

			Current_Layer = 
				Current_View->Layer_Array[vectbuf->vectbundles[looper].Layer_ID];
			Current_Pen = 
				Current_View->Pen_Array[vectbuf->vectbundles[looper].Pen_ID];

			CreateLine(p1x,p1y,p2x,p2y,fg);
			vld++;
		}		

		FreeChunk(insave);
		break;

	default:	FreeChunk(insave);
			BombLoad("Unrecognized vector record");	
			return(CLIENT_ERROR);	/* Unrecognized iff */
	}

	return(IFF_OKAY);
}

IFFP loadCIRC_0(groupin)
GroupContext *groupin;
{

	struct iff_circ_0 *circbuf;
	int  looper,gx;
	STDFLT cenx,ceny,radx,rady,rot,angs,ange;
	int t;

	insave = StuffChunk(groupin);
	circbuf = (struct iff_circ_0 *) insave.getchunk;

	if (circbuf==NULL) return(CLIENT_ERROR);	/* Out of memory */

/*	printf("CIRC0\n");	*/

	switch (circbuf->defrev) {
	case CIRC0_REV:

		t=sizeof(UWORD)*2;
		t+=(sizeof(struct _c0bundle)) * circbuf->NumberCircs;

		if (insave.sizechunk<t) {
			FreeChunk(insave);
			BombLoad("Ellipse record illegal size");
			return(CLIENT_ERROR);
		}

		for (looper=0; looper<circbuf->NumberCircs; looper++) {
		
			gx = circbuf->circbundles[looper].Group_ID;

			cenx=PORTtoF(&(circbuf->circbundles[looper].centx));
			ceny=PORTtoF(&(circbuf->circbundles[looper].centy));
			radx=PORTtoF(&(circbuf->circbundles[looper].radiusx));
			rady=PORTtoF(&(circbuf->circbundles[looper].radiusy));
			rot=PORTtoF(&(circbuf->circbundles[looper].rotation));
			angs=PORTtoF(&(circbuf->circbundles[looper].starta));
			ange=PORTtoF(&(circbuf->circbundles[looper].enda));

			Current_Group = Current_View->Group_Array[gx];
			Current_Layer = 
				Current_View->Layer_Array[circbuf->circbundles[looper].Layer_ID];
			Current_Pen = 
				Current_View->Pen_Array[circbuf->circbundles[looper].Pen_ID];

			CreateEllipse(cenx,ceny,radx,rady,rot,angs,ange);
		}		

		FreeChunk(insave);
		break;

	default:	FreeChunk(insave);
			BombLoad("Unrecognized ellipse record");	
			return(CLIENT_ERROR);	/* Unrecognized iff */
	}

	return(IFF_OKAY);
}

IFFP loadTEXT_0(groupin)
GroupContext *groupin;
{

	struct iff_text_0 *textbuf;
	STDFLT hx,hy,ang;
	UBYTE *text,handle;
	UWORD flipflags,gx;
	int t;

	insave = StuffChunk(groupin);
	textbuf = (struct iff_text_0 *) insave.getchunk;

	if (textbuf==NULL) return(CLIENT_ERROR);	/* Out of memory */

/*	printf("TEXT0\n");	*/

	switch (textbuf->defrev) {
	case TEXT0_REV:

		t = sizeof (struct iff_text_0) - 256;
		t += textbuf->textlen;

		if (insave.sizechunk<t) {
			FreeChunk(insave);
			BombLoad("Ellipse record illegal size");
			return(CLIENT_ERROR);
		}

		gx = textbuf->Group_ID;

		hx=PORTtoF(&(textbuf->Xpos));
		hy=PORTtoF(&(textbuf->Ypos));
		ang=PORTtoF(&(textbuf->Rotation));

		flipflags = textbuf->TextFlags;
		handle = textbuf->Handle_Pos;
		text = textbuf->TextData;

		Current_Group = Current_View->Group_Array[gx];
		Current_Style = Current_View->Style_Array[textbuf->Letter_ID];
		Current_Layer = Current_View->Layer_Array[textbuf->Layer_ID];
		Current_Pen = Current_View->Pen_Array[textbuf->Pen_ID];

		CreateText(text,hx,hy,ang,handle,flipflags,Current_Style);

		FreeChunk(insave);
		break;

	default:	FreeChunk(insave);
			BombLoad("Unrecognized text record");	
			return(CLIENT_ERROR);	/* Unrecognized iff */
	}

	return(IFF_OKAY);
}

IFFP loadHATC_0(groupin)
GroupContext *groupin;
{

	struct iff_hatch_0 *hatchbuf;
	O_HATCH newhatch;
	int t;
	int gx;
	T_HATCH *hatchloop;

	/* DCK("HATC0") */

	insave = StuffChunk(groupin);
	hatchbuf = (struct iff_hatch_0 *) insave.getchunk;

	if (hatchbuf==NULL) return(CLIENT_ERROR);	/* Out of memory */

	switch (hatchbuf->defrev) {
	case HATCH0_REV:

		t = sizeof (struct iff_hatch_0);

		/* printf("Size=%d hatch=\"%s\"\n",t,hatchbuf->HatchData); */

		if (insave.sizechunk<t) {
			FreeChunk(insave);
			BombLoad("Hatch record illegal size");
			return(CLIENT_ERROR);
		}

		gx = hatchbuf->Group_ID;

		newhatch.hatch_type = hatchbuf->FillType;
		newhatch.hatch_flags = hatchbuf->FillFlags;
		newhatch.number_polys = hatchbuf->Number_Polys;

		newhatch.pattern_rot = PORTtoF(&(hatchbuf->Hatch_Rotation));
		newhatch.pattern_scale = PORTtoF(&(hatchbuf->Hatch_Scale));

		Current_Group = Current_View->Group_Array[gx];
		Current_Layer = Current_View->Layer_Array[hatchbuf->Layer_ID];
		Current_Pen = Current_View->Pen_Array[hatchbuf->Pen_ID];

		if (Current_Pen==NULL) {
			printf("Ahh!!!  Null Pen!!!!\n");
			FreeChunk(insave);
			BombLoad("Null pen pointer!!!");
			return(CLIENT_ERROR);
		}
		
		if (newhatch.hatch_type==FILL0_SPATT) {
		
			newhatch.hatch_pattern = NULL;
			if (hatchbuf->Hatch_ID!=0) {
				for (hatchloop=Root_Hatch; hatchloop!=NULL;
						hatchloop=hatchloop->next) {
					if (hatchloop->Hatch_ID==hatchbuf->Hatch_ID) 
						newhatch.hatch_pattern = hatchloop;
				}
			}
			
		} else {
		
			newhatch.hatch_type = HATCH_SOLID;
			newhatch.hatch_pattern = NULL;
		}
			
		CreateHatch(&newhatch,NULL);	/* Put us at top of heap */

		FreeChunk(insave);
		break;

	default:	FreeChunk(insave);
			BombLoad("Unrecognized hatch record");	
			return(CLIENT_ERROR);	/* Unrecognized iff */
	}

	return(IFF_OKAY);
}

IFFP ProcOBJE_0(groupin)
GroupContext *groupin;
{
	GroupContext thiscontext;

	iffp=OpenRGroup(groupin, &thiscontext);
	CheckIFFP();

	do
		switch (iffp=GetF1ChunkHdr(&thiscontext)) {

		case VECT_0:
			iffp=loadVECT_0(&thiscontext);
			CheckIFFP();
			break;

		case CIRC_0:
			iffp=loadCIRC_0(&thiscontext);
			CheckIFFP();
			break;

		case TEXT_0:
			iffp=loadTEXT_0(&thiscontext);
			CheckIFFP();
			break;
			
		case HATC_0:
			iffp=loadHATC_0(&thiscontext);
			CheckIFFP();
			break;

		} while (iffp >= IFF_OKAY);

	return(CloseRGroup(&thiscontext));	
	
}

/*--------------------------Main iff read code-----------------------------*/

IFFP getview(file,infnm)
UBYTE *infnm,*file;
{

	BPTR	infile;
	IFFP	retiffp;
	long	errret;

	ClientFrame mainframe;

	infile=GOpen(infnm,MODE_OLDFILE);
	if (infile==0) {
		errret=IoErr();
		complain(NULL,"Recover",5,0,0,0,
			"Cannot find view file:",
			infnm,NULL,NULL,NULL);
		return(NO_FILE);
	}

	Current_View = DrawMem(sizeof(struct ViewContext));
	if (Current_View==NULL) {
		printf("Not enough memory to proceed!\n");
		return;
	}

	sprintf(Current_View->FullPath,"%s",infnm);
	sprintf(Current_View->ViewName,"%s",file);

	Current_View->ViewFlags = VIEW_VIEWLOCK;

	Current_View->XViewMIN = 0.0;
	Current_View->XViewMAX = 0.0;
	Current_View->YViewMIN = 0.0;
	Current_View->YViewMAX = 0.0;

	Current_View->ViewRot = 0.0;
	Current_View->XNewOrigin = SheetMinIX;
	Current_View->YNewOrigin = SheetMinIY;
	Current_View->ArrowAngle = PI/6.0;
	Current_View->Arrowlen = 12.0;
	Current_View->ViewScale = 1.0;

	Current_View->Group_Array = NULL;
	Current_View->Pen_Array = NULL;
	Current_View->Layer_Array = NULL;
	Current_View->Style_Array = NULL;

	Current_View->next = NULL;
	Current_View->last = NULL;

	mainframe.getList=SkipGroup;
	mainframe.getProp=SkipGroup;

	mainframe.getForm = GetForms;
	mainframe.getCat=SkipGroup;

	retiffp=ReadIFF(infile, &mainframe);

	GClose(infile);

	if (retiffp==IFF_OKAY)	{
		/*	InfoMess(LOAD_MODE,LOAD_MESS_GOTIT); */
	
		if (Last_View==NULL) Root_View = Last_View = Current_View;
		else {
			Last_View->next = Current_View;
			Current_View->last = Last_View;
			Last_View = Current_View;
		}
		disp_driv();

	}/*	else	InfoMess(LOAD_MODE,LOAD_MESS_ERROR); */

	return(retiffp);

}


extern UBYTE ViewPath[],ArgText[];
BOOL Req_Load_View();

BOOL inload=FALSE;		/* Interlock */

BOOL Load_View()
{

	UBYTE fname[255];
	int a;

	if (inload) return(FALSE);	/* Don't double call - edit 9 */
	inload = TRUE;

	if (DriverSegHook==NULL) {
		inload = FALSE;
		return(WarnLoad());
	}

	if (!Req_Load_View()) {
		inload = FALSE;
		return(FALSE);
	}

	if ((a=strlen(ViewPath))==0) sprintf(fname,"%s",ArgText);
	else if (a>0&&ViewPath[a-1]==':')sprintf(fname,"%s%s",ViewPath,ArgText);
	else sprintf(fname,"%s/%s",ViewPath,ArgText);

	getview(ArgText,fname);

	inload=FALSE;

	return(FALSE);
}

IFFP loadview()
{

	BPTR	infile;
	IFFP	retiffp;
	long	errret;

	ClientFrame mainframe;

/*	printf("Load in %s\n",Current_View->FullPath);	*/

	infile=GOpen(Current_View->FullPath,MODE_OLDFILE);
	if (infile==0) {
		errret=IoErr();
		complain(NULL,"Recover",5,0,5,0,
			"Cannot find view file:",
			Current_View->FullPath,
			"What Happened?  It was here a minute ago!",NULL);
		return(NO_FILE);
	}

	mainframe.getList=SkipGroup;
	mainframe.getProp=SkipGroup;

	mainframe.getForm = GetGetForms;
	mainframe.getCat=SkipGroup;

	retiffp=ReadIFF(infile, &mainframe);

	closehatch();

	GClose(infile);
	return(retiffp);

}
