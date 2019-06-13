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
 *	8	30-Jan-88 SH	Do the dimensioning defaults correctly
 *	9	 4-Feb-88 SH	Add view memory and dimensioning defaults
 *	10	14-Feb-88 SH	Don't load palletes in merge mode 
 *	11	19-Apr-88 SH	Merge is not linking in the groups properly
 *	12	 9-Jan-89 SH	Add Hatch Load
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include "viewiff.h"
	
/*----------------------------------Macros-----------------------------------*/

#define CkErr(expression) { if (ifferror==IFF_OKAY) ifferror=(expression);}
#define div32(x) ((x)>>5)
#define rem32(x) ((x)&0x1F)
#define STACKBUFF 1024

#define rmalloc(x) AllocMem(x,0)
#define rmfree(x,s) FreeMem(x,s)

#define DCK(a) /*fprintf(stdout,"Read "a"\n");*/

#define BombLoad(a) printf("\007Error *** %s ***\n",a)

/*-----------------------------Forward References----------------------------*/

VOID CheckClip();

/*-------------------------External Symbol Definitions-----------------------*/

T_FONT *Stroke_Search();
BOOL LoadStrokeFont();

extern BOOL CheckAbort();

/*-------------------------Global Variable Definitions-----------------------*/

extern	IFFP ifferror;
extern	IFFP iffp;

LOCAL WORD group_m,pen_m,style_m,layer_m;

LOCAL GROUP_A *temp_root;
LOCAL BOOL setclip;

LOCAL int maxcols;

/*-------------------------------Code Section--------------------------------*/

IFFP IffErr();
VOID setzero();
float PORTtoF();
VOID FtoPORT();
float infloat();

/*-------------------------Version 0 support------------------------------*/

LOCAL WORD group_c,pen_c,style_c,layer_c,pen_t,style_t;
LOCAL WORD witlayer_c,witgroup_c,witpen_c,dimlinlayer_c,dimlingroup_c,
	dimlinpen_c,dimboxlayer_c,dimboxgroup_c,dimboxpen_c,dimboxstyle_c;

struct loadchunk {
	UBYTE 	*getchunk;
	ULONG	sizechunk;
} insave;

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

	regroup=TRUE;

	insave = StuffChunk(groupin);
	limbuf = (struct iff_lims_0 *) insave.getchunk;

	if (limbuf==NULL) return(CLIENT_ERROR);		/* Out of memory */

	DCK("LIMS0")

	switch (limbuf->defrev) {
	case LIMS0_REV:

		if (insave.sizechunk<sizeof(struct iff_lims_0)) {
			FreeChunk(insave);
			BombLoad("Limit record illegal size");
			return(CLIENT_ERROR);
		}
			
		if (!setclip) {
			xsnap = PORTtoF(&limbuf->xsnap);
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
			gridrot = PORTtoF(&limbuf->gridrot);
	
			xviewMIN = PORTtoF(&limbuf->minx);
			yviewMIN = PORTtoF(&limbuf->miny);
			xviewMAX = PORTtoF(&limbuf->maxx);
			yviewMAX = PORTtoF(&limbuf->maxy);
	
			qtextlim = limbuf->qtextlim;
	
			paperunit = PORTtoF(&limbuf->paperunit);
			viewunit = PORTtoF(&limbuf->viewunit);
			viewscale = PORTtoF(&limbuf->viewscale);
	
			Arrow_Angle = PORTtoF(&limbuf->arrowangle);
			Arrow_Len = PORTtoF(&limbuf->arrowlen);
	
			orthoangle = PORTtoF(&limbuf->orthoang);
			filletrad = PORTtoF(&limbuf->filletrad);
	
			lintolerance = PORTtoF(&limbuf->lintolerance);
			angtolerance = PORTtoF(&limbuf->angtolerance);
			linpoints = limbuf->linpoints;
			angpoints = limbuf->angpoints;
			lintype = limbuf->lintype;
			angtype = limbuf->angtype;
	
			skew=1.0;

			Center_On=Handles_On=Coord_On=
				Snaps_On=Grid_On=Use_Bounds=Q_Hatch=FALSE;
			Ortho_Switch=FALSE;

			if (limbuf->viewflags & LIM0FLAG_CCEN) Center_On=TRUE;
			if (limbuf->viewflags & LIM0FLAG_HAND) Handles_On=TRUE;
			if (limbuf->viewflags & LIM0FLAG_COOR) Coord_On=TRUE;
			if (limbuf->viewflags & LIM0FLAG_SNAP) Snaps_On=TRUE;
			if (limbuf->viewflags & LIM0FLAG_GRID) Grid_On=TRUE;
			if (limbuf->viewflags & LIM0FLAG_BOUN) Use_Bounds=TRUE;
			if (limbuf->viewflags & LIM0FLAG_ORTH) Ortho_Switch=TRUE;
			if (limbuf->viewflags & LIM0FLAG_QHAT) Q_Hatch = TRUE;
	
			Color_Source = limbuf->takecolors;
		}

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
	
	grpoffs = 0;
	grpmax = group_m;

	Group_Array = (GROUP_A **) 
		rmalloc(sizeof(struct Group_Array *) * group_m);

	if (Group_Array==NULL) {
		printf("Couldn't allocate the Group_Array %d\n",group_m);
		return(CLIENT_ERROR);
	}

	Layer_Array = (LAYER_A **)
		rmalloc(sizeof(struct Layer_Array *) * layer_m);

	if (Layer_Array==NULL) {
		printf("Couldn't allocate the Layer_Array %d\n",layer_m);
		return(CLIENT_ERROR);
	}

	Style_Array = (STYLE_A **)
		rmalloc(sizeof(STYLE_A *) * style_m);

	if (Style_Array==NULL) {
		printf("Couldn't allocate the Style_Array %d\n",style_m);
		return(CLIENT_ERROR);
	}

	Pen_Array = (PEN_A **)
		rmalloc(sizeof(PEN_A *) * pen_m);

	if (Pen_Array==NULL) {
		printf("Couldn't allocate the Pen_Array %d\n",pen_m);
		return(CLIENT_ERROR);
	}

	for (t=0; t<group_m; t++) Group_Array[t]=NULL;
	for (t=0; t<style_m; t++) Style_Array[t]=NULL;
	for (t=0; t<layer_m; t++) Layer_Array[t]=NULL;
	for (t=0; t<pen_m; t++) Pen_Array[t]=NULL;

	return(IFF_OKAY);
}

IFFP loadPALE_0(groupin)
GroupContext *groupin;
{

	struct iff_pale_0 *palebuf;
	int a;
	ULONG red,green,blue;
	
	DCK("PALE0")

	insave = StuffChunk(groupin);
	if (setclip) {
		FreeChunk(insave);
		return(IFF_OKAY);
	}

	maxcols = 1 << Sdepth;	

	palebuf = (struct iff_pale_0 *) insave.getchunk;

	if (palebuf==NULL) return(CLIENT_ERROR);	/* Out of memory */

	switch (palebuf->defrev) {
	case PALE0_REV:

		a = sizeof(struct iff_pale_0);
		a -= sizeof(struct _pale0_sets) * (MAXCOLOR-palebuf->NumTot);

		if (insave.sizechunk<a) {
			FreeChunk(insave);
			BombLoad("Pallette record illegal size");
			return(CLIENT_ERROR);
		}


		for (a=0; a<palebuf->NumTot; a++) {
			if (a>=maxcols) continue;
			red = palebuf->colors[a].RedMix;
			green = palebuf->colors[a].GreenMix;
			blue = palebuf->colors[a].BlueMix;
			GI_SetColor(a,red,green,blue);
		}

		for (a=0; a<MAXPRESETS; a++) {
			if (palebuf->Presets[a]<0) continue;	/* Not set */
			/* break; */
			switch(a) {
	
			/* Here are the presets we know about */
	
			case PALE0_GRID: 
				Grid_Color = palebuf->Presets[a]; 
				break;
			case PALE0_MIST: 
				Mist_Color = palebuf->Presets[a]; 
				break;
			case PALE0_BNDS: 
				Bounds_Color = palebuf->Presets[a]; 
				break;
			case PALE0_GHST: 
				Ghost_Color = palebuf->Presets[a]; 
				break;
			case PALE0_COOR: 
				Coord_Color = palebuf->Presets[a]; 
				break;
			case PALE0_DRAW: 
				Draw_Color = palebuf->Presets[a]; 
				break;
			case PALE0_ERAS: 
				Erased_Color = palebuf->Presets[a]; 
				break;

			}
		}
		FreeChunk(insave);
		break;

	default:	FreeChunk(insave);
			BombLoad("Unrecognized pallette record");	
			return(CLIENT_ERROR);	/* Unrecognized iff */
	}

	return(IFF_OKAY);
}

IFFP loadDIME_0(groupin)
GroupContext *groupin;
{

	struct iff_dime_0 *dimebuf;
	int a;

	DCK("DIME0")

	insave = StuffChunk(groupin);
	if (setclip) {
		FreeChunk(insave);
		return(IFF_OKAY);
	}

	dimebuf = (struct iff_dime_0 *) insave.getchunk;

	if (dimebuf==NULL) return(CLIENT_ERROR);	/* Out of memory */

	switch (dimebuf->defrev) {
	case DIME0_REV:

		a = sizeof(struct iff_dime_0);

		if (insave.sizechunk<a) {
			FreeChunk(insave);
			BombLoad("Dimenstion defaults record illegal size");
			return(CLIENT_ERROR);
		}

		witlayer_c = dimebuf->WitLayer;
		witgroup_c = dimebuf->WitGroup;
		witpen_c = dimebuf->WitPen;
		dimlinlayer_c = dimebuf->DimLinLayer;
		dimlingroup_c = dimebuf->DimLinGroup;
		dimlinpen_c = dimebuf->DimLinPen;
		dimboxlayer_c = dimebuf->DimBoxLayer;
		dimboxgroup_c = dimebuf->DimBoxGroup;
		dimboxpen_c = dimebuf->DimBoxPen;
		dimboxstyle_c = dimebuf->DimBoxStyle;

		Wit_Pick = PORTtoF(&dimebuf->WitOffset);
		Wit_Over = PORTtoF(&dimebuf->WitOver);
		Dim_Line_Offset = PORTtoF(&dimebuf->DimLinOffset);
		Dim_Box_Offset = PORTtoF(&dimebuf->DimBoxOffset);
		Fixed_Angle = PORTtoF(&dimebuf->DimAngle);
		Free_Flip_From = PORTtoF(&dimebuf->FromFlip);
		Free_Flip_To = PORTtoF(&dimebuf->ToFlip);
		Free_Offset = PORTtoF(&dimebuf->FreeOffset);

		Use_Fixed_Angle = FALSE;
		if (dimebuf->DimFlags&DIM0FLAG_FIXED) Use_Fixed_Angle = TRUE;

		FreeChunk(insave);
		break;

	default:	FreeChunk(insave);
			BombLoad("Unrecognized dimension default record");	
			return(CLIENT_ERROR);	/* Unrecognized iff */
	}

	return(IFF_OKAY);
}

IFFP loadBMEM_0(groupin)
GroupContext *groupin;
{

	struct iff_bmem_0 *bmembuf;
	int a,ct;

	DCK("BMEM0")

	insave = StuffChunk(groupin);
	if (setclip) {
		FreeChunk(insave);
		return(IFF_OKAY);
	}

	bmembuf = (struct iff_bmem_0 *) insave.getchunk;

	if (bmembuf==NULL) return(CLIENT_ERROR);	/* Out of memory */

	switch (bmembuf->defrev) {
	case BMEM0_REV:

		a = sizeof(struct iff_bmem_0);

		if (insave.sizechunk<a) {
			FreeChunk(insave);
			BombLoad("View memory record illegal size");
			return(CLIENT_ERROR);
		}

		if (!(bmembuf->memflags&BMEM0_KEYPAD)) {
			FreeChunk(insave);
			break;
		}

		ct = bmembuf->keyid;
		if (ct<0||ct>9) {
			FreeChunk(insave);
			break;
		}

		vset[ct] = TRUE;
		xvbeg[ct] = PORTtoF(&bmembuf->xbeg);
		yvbeg[ct] = PORTtoF(&bmembuf->ybeg);
		xvsiz[ct] = PORTtoF(&bmembuf->xsiz);
		yvsiz[ct] = PORTtoF(&bmembuf->xsiz);

		FreeChunk(insave);
		break;

	default:	FreeChunk(insave);
			BombLoad("Unknown View memory record");	
			return(CLIENT_ERROR);	/* Unrecognized iff */
	}

	return(IFF_OKAY);
}


IFFP ProcU2VI_0(groupin)
GroupContext *groupin;
{
	GroupContext thiscontext;

	DCK("U2VI0")

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

		case PALE_0:
			iffp=loadPALE_0(&thiscontext);
			CheckIFFP();
			break;

		case DIME_0:
			iffp=loadDIME_0(&thiscontext);
			CheckIFFP();
			break;

		case BMEM_0:
			iffp=loadBMEM_0(&thiscontext);
			CheckIFFP();
			break;

		} while (iffp >= IFF_OKAY);

	return(CloseRGroup(&thiscontext));	

}

IFFP loadGRPI_0(groupin)
GroupContext *groupin;
{

	struct iff_grpi_0 *grpbuf;
	GROUP_A *newgroup;
	int t;

	DCK("GRPI0")

	insave = StuffChunk(groupin);
	grpbuf = (struct iff_grpi_0 *) insave.getchunk;

	if (grpbuf==NULL) return(CLIENT_ERROR);	/* Out of memory */

	switch (grpbuf->defrev) {
	case GRPI0_REV:

		t = sizeof(struct iff_grpi_0) - 255;
		t += strlen(grpbuf->Comment);

		if (insave.sizechunk<t) {
			FreeChunk(insave);
			BombLoad("Group record illegal size");
			return(CLIENT_ERROR);
		}

		newgroup = (GROUP_A *) 
				GI_DrawAlloc(sizeof(GROUP_A));

		if (newgroup==NULL) {
			printf("No memory to save group id %s",grpbuf->GroupName);
			return(CLIENT_ERROR);
		}

		newgroup->Group_ID = grpbuf->GroupID;
		sprintf(newgroup->Group_Name,"%s",grpbuf->GroupName);
		if (maxcols<1) maxcols = 1 << Sdepth;	
		newgroup->Color_ID = grpbuf->Color_ID%maxcols;
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

		Group_Array[t] = newgroup;

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
	LAYER_A *newlayer;
	int t;

	DCK("LYER0")

	insave = StuffChunk(groupin);
	layerbuf = (struct iff_lyer_0 *) insave.getchunk;

	if (layerbuf==NULL) return(CLIENT_ERROR);	/* Out of memory */

	switch (layerbuf->defrev) {
	case LYER0_REV:

		t = sizeof(struct iff_lyer_0) - 255;
		t += strlen(layerbuf->Comment);

		if (insave.sizechunk<t) {
			FreeChunk(insave);
			BombLoad("Layer record illegal size");
			return(CLIENT_ERROR);
		}

		newlayer = Layer_Search(layerbuf->LayerName);
		if (newlayer!=NULL) {			/* Already here */
			Layer_Array[layerbuf->LayerID] = newlayer;
			FreeChunk(insave);
			return(IFF_OKAY);
		}
		
		newlayer = (LAYER_A *) 
				GI_DrawAlloc(sizeof(LAYER_A));

		if (newlayer==NULL) {
			printf("No memory to save layer id %s",layerbuf->LayerName);
			return(CLIENT_ERROR);
		}
			
		newlayer->Layer_ID = layerbuf->LayerID;
		strcpy(newlayer->Layer_Name,layerbuf->LayerName);
		if (maxcols<1) maxcols = 1 << Sdepth;	
		newlayer->Color_ID = layerbuf->Color_ID%maxcols;
		newlayer->Layer_Flags = layerbuf->Layer_Flags;
		newlayer->Use_Count = 0;
		newlayer->Comment = StoreComment(layerbuf->Comment);
			
		Layer_Array[newlayer->Layer_ID] = newlayer;

		Link_Layer(newlayer);
		
		FreeChunk(insave);
		break;

	default:	FreeChunk(insave);
			BombLoad("Unrecognized layer record");	
			return(CLIENT_ERROR);	/* Unrecognized iff */
	}

	return(IFF_OKAY);
}

IFFP loadPENS_0(groupin)
GroupContext *groupin;
{

	struct iff_pens_0_0 *penbuf0;
	struct iff_pens_0_1 *penbuf1;
	PEN_A *newpen;
	int t;

	DCK("PENS0")

	insave = StuffChunk(groupin);
	penbuf0 = (struct iff_pens_0_0 *) insave.getchunk;

	if (penbuf0==NULL) return(CLIENT_ERROR);	/* Out of memory */

	switch (penbuf0->defrev) {
	case PENS0_REV0:

		t = sizeof(struct iff_pens_0_0) - 255;
		t += strlen(penbuf0->Comment);

		if (insave.sizechunk<t) {
			FreeChunk(insave);
			BombLoad("Pen record illegal size");
			return(CLIENT_ERROR);
		}
	
		newpen = Pen_Search(penbuf0->PenName);
		if (newpen!=NULL) {
			Pen_Array[penbuf0->PenID] = newpen;
			FreeChunk(insave);
			return(IFF_OKAY);
		}

		newpen = (PEN_A *) 
				GI_DrawAlloc(sizeof(PEN_A));

		if (newpen==NULL) {
			printf("No memory to save pen id %s",penbuf0->PenName);
			return(CLIENT_ERROR);
		}
	
		newpen->Pen_ID = penbuf0->PenID;
		sprintf(newpen->Pen_Name,"%s",penbuf0->PenName);
		if (maxcols<1) maxcols = 1 << Sdepth;	
		newpen->Color_ID = penbuf0->Color_ID%maxcols;
		newpen->Pen_Flags = penbuf0->Pen_Flags;
		newpen->Line_Pattern = penbuf0->Pattern1;
		newpen->Use_Count = 0;
		newpen->pen_next = NULL;
		newpen->pen_prev = NULL;
		newpen->Comment = StoreComment(penbuf0->Comment);
		newpen->PenOrd = 0;
		newpen->Carousel = 0;
		newpen->AutoNum = 0;
		newpen->BitWidth = 0.5;
		newpen->PenWidth = 0.5;
	
		Pen_Array[newpen->Pen_ID] = newpen;

		Link_Pen(newpen);

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

		/* printf("Pen array = %x\n",Pen_Array);
		printf("New Style Pen - name %s id=%d\n",
			penbuf1->PenName,penbuf1->PenID); */
		newpen = Pen_Search(penbuf1->PenName);
		if (newpen!=NULL) {
			/* printf("Found pen was already there! - was %s\n",
				newpen->Pen_Name); */
			Pen_Array[penbuf1->PenID] = newpen;
			FreeChunk(insave);
			return(IFF_OKAY);
		}

		newpen = (PEN_A *) 
				GI_DrawAlloc(sizeof(PEN_A));

		if (newpen==NULL) {
			printf("No memory to save pen id %s",penbuf1->PenName);
			return(CLIENT_ERROR);
		}
	
		newpen->Pen_ID = penbuf1->PenID;
		sprintf(newpen->Pen_Name,"%s",penbuf1->PenName);
		if (maxcols<1) maxcols = 1 << Sdepth;	
		newpen->Color_ID = penbuf1->Color_ID%maxcols;
		newpen->Pen_Flags = penbuf1->Pen_Flags;
		newpen->Line_Pattern = penbuf1->Pattern1;
		newpen->Hard_Color = penbuf1->HardColor;
		newpen->Use_Count = 0;
		newpen->pen_next = NULL;
		newpen->pen_prev = NULL;
		newpen->Comment = StoreComment(penbuf1->Comment);
		newpen->PenOrd = penbuf1->PenOrd;
		newpen->Carousel = penbuf1->Carousel;
		newpen->AutoNum = penbuf1->AutoNum;
	
		Pen_Array[newpen->Pen_ID] = newpen;

		Link_Pen(newpen);

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
	STYLE_A *newtext;
	int t;

	DCK("TXST0")

	insave = StuffChunk(groupin);
	txtbuf = (struct iff_txst_0 *) insave.getchunk;

	if (txtbuf==NULL) return(CLIENT_ERROR);	/* Out of memory */

	switch (txtbuf->defrev) {
	case TXST0_REV:

		t = sizeof(struct iff_txst_0) - 255;
		t += strlen(txtbuf->Comment);

		if (insave.sizechunk<t) {
			FreeChunk(insave);
			BombLoad("Pen record illegal size");
			return(CLIENT_ERROR);
		}

		newtext = Style_Search(txtbuf->StyleName);

		if (newtext!=NULL) {
			Style_Array[txtbuf->LetterID] = newtext;
			FreeChunk(insave);
			return(IFF_OKAY);
		}

		newtext = (STYLE_A *) 
				GI_DrawAlloc(sizeof(STYLE_A));

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

		Style_Array[newtext->Letter_ID] = newtext;

		Link_Style(newtext);

		newtext->draw_font = Stroke_Search(txtbuf->FontName);
		if (newtext->draw_font==NULL) {
			if (!LoadStrokeFont(txtbuf->FontName)) {
				printf("Couldn't find font text font %s\n",
					txtbuf->FontName);
				newtext->draw_font = Root_Font;
				return(IFF_OKAY);
			}
			newtext->draw_font = Stroke_Search(txtbuf->FontName);
			if (newtext->draw_font==NULL) 
				newtext->draw_font = Root_Font;
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

	DCK("PATT0")

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

IFFP ProcATTR_0(groupin)
GroupContext *groupin;
{
	GroupContext thiscontext;

	DCK("ATTR0")

	iffp=OpenRGroup(groupin, &thiscontext);
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

IFFP loadVECT_0(groupin)
GroupContext *groupin;
{

	struct iff_vect_0 *vectbuf;
	int  looper,gx;
	int vld=0,fg;
	STDFLT p1x,p1y,p2x,p2y;
	int t;

	DCK("VECT0")

	insave = StuffChunk(groupin);
	vectbuf = (struct iff_vect_0 *) insave.getchunk;

	if (vectbuf==NULL) return(CLIENT_ERROR);	/* Out of memory */

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
			if (gx<grpoffs||gx>grpmax) continue;

			fg = vectbuf->vectbundles[looper].VecFlags;
			p1x=PORTtoF(&(vectbuf->vectbundles[looper].p1x));
			p1y=PORTtoF(&(vectbuf->vectbundles[looper].p1y));
			p2x=PORTtoF(&(vectbuf->vectbundles[looper].p2x));
			p2y=PORTtoF(&(vectbuf->vectbundles[looper].p2y));

			Current_Group = Group_Array[gx-grpoffs];

			Current_Layer = 
			   Layer_Array[vectbuf->vectbundles[looper].Layer_ID];
			Current_Pen = 
			   Pen_Array[vectbuf->vectbundles[looper].Pen_ID];

			CreateLine(p1x,p1y,p2x,p2y,fg);
			CheckClip();
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

	DCK("CIRC0")

	insave = StuffChunk(groupin);
	circbuf = (struct iff_circ_0 *) insave.getchunk;

	if (circbuf==NULL) return(CLIENT_ERROR);	/* Out of memory */

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
			if (gx<grpoffs||gx>grpmax) continue;

			cenx=PORTtoF(&(circbuf->circbundles[looper].centx));
			ceny=PORTtoF(&(circbuf->circbundles[looper].centy));
			radx=PORTtoF(&(circbuf->circbundles[looper].radiusx));
			rady=PORTtoF(&(circbuf->circbundles[looper].radiusy));
			rot=PORTtoF(&(circbuf->circbundles[looper].rotation));
			angs=PORTtoF(&(circbuf->circbundles[looper].starta));
			ange=PORTtoF(&(circbuf->circbundles[looper].enda));

			Current_Group = Group_Array[gx-grpoffs];
			Current_Layer = 
			 Layer_Array[circbuf->circbundles[looper].Layer_ID];
			Current_Pen = 
			 Pen_Array[circbuf->circbundles[looper].Pen_ID];

			CreateEllipse(cenx,ceny,radx,rady,rot,angs,ange);
			CheckClip();
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

	DCK("TEXT0")

	insave = StuffChunk(groupin);
	textbuf = (struct iff_text_0 *) insave.getchunk;

	if (textbuf==NULL) return(CLIENT_ERROR);	/* Out of memory */

	switch (textbuf->defrev) {
	case TEXT0_REV:

		t = sizeof (struct iff_text_0) - 256;
		t += textbuf->textlen;

		/* printf("Size=%d text=\"%s\"\n",t,textbuf->TextData); */

		if (insave.sizechunk<t) {
			FreeChunk(insave);
			BombLoad("Ellipse record illegal size");
			return(CLIENT_ERROR);
		}

		gx = textbuf->Group_ID;
		if (gx<grpoffs||gx>grpmax) return(IFF_OKAY);	/* Throw out */

		hx=PORTtoF(&(textbuf->Xpos));
		hy=PORTtoF(&(textbuf->Ypos));
		ang=PORTtoF(&(textbuf->Rotation));

		flipflags = textbuf->TextFlags;
		handle = textbuf->Handle_Pos;
		text = textbuf->TextData;
		
		/* printf("G-%d(%d) S-%d(%d) L-%d(%d) P-%d(%d)\n",
			gx-grpoffs,group_m,
			textbuf->Letter_ID,style_m,
			textbuf->Layer_ID,layer_m,
			textbuf->Pen_ID,pen_m);	*/
			
		Current_Group = Group_Array[gx-grpoffs];
		Current_Style = Style_Array[textbuf->Letter_ID];
		Current_Layer = Layer_Array[textbuf->Layer_ID];
		Current_Pen = Pen_Array[textbuf->Pen_ID];

		/* printf("Gp=%x",Current_Group);
		printf(" %s\n",Current_Group->Group_Name);
		printf("St=%x",Current_Style);
		printf(" %s\n",Current_Style->Letter_Style_ID);
		printf("Ly=%x",Current_Layer);
		printf(" %s\n",Current_Layer->Layer_Name);
		printf("Pn=%x",Current_Pen);	*/
		if (Current_Pen==NULL) {
			printf("Ahh!!!  Null Pen!!!!\n");
			FreeChunk(insave);
			BombLoad("Null pen pointer!!!");
			return(CLIENT_ERROR);
		}

		CreateText(text,hx,hy,ang,handle,flipflags,Current_Style);
		CheckClip();

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

	DCK("HATC0")

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
		if (gx<grpoffs||gx>grpmax) return(IFF_OKAY);	/* Throw out */

		newhatch.hatch_type = hatchbuf->FillType;
		newhatch.hatch_flags = hatchbuf->FillFlags;
		newhatch.number_polys = hatchbuf->Number_Polys;

		newhatch.pattern_rot = PORTtoF(&(hatchbuf->Hatch_Rotation));
		newhatch.pattern_scale = PORTtoF(&(hatchbuf->Hatch_Scale));

		Current_Group = Group_Array[gx-grpoffs];
		Current_Layer = Layer_Array[hatchbuf->Layer_ID];
		Current_Pen = Pen_Array[hatchbuf->Pen_ID];

		if (Current_Pen==NULL) {
			printf("Ahh!!!  Null Pen!!!!\n");
			FreeChunk(insave);
			BombLoad("Null pen pointer!!!");
			return(CLIENT_ERROR);
		}
		
		newhatch.hatch_pattern = NULL;
		if (hatchbuf->Hatch_ID!=0) {
			for (hatchloop=Root_Hatch; hatchloop!=NULL;
					hatchloop=hatchloop->next) {
				if (hatchloop->Hatch_ID==hatchbuf->Hatch_ID) 
					newhatch.hatch_pattern = hatchloop;
			}
		}

		if (hatchbuf->Hatch_ID!=0 && newhatch.hatch_pattern==NULL)
			newhatch.hatch_type = HATCH_SOLID;
			
		CreateHatch(&newhatch,NULL);	/* Put us at top of heap */
		CheckClip();

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

	DCK("OBJE0")

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

IFFP GetForms(groupin)
GroupContext *groupin;
{

	switch (groupin->subtype) {

/*	case U2VI_8:	printf("*** Prototype-8 view loaded ***\n");
			return(ProcU2VI_8(groupin));
	case ATTR_8:	return(ProcATTR_8(groupin));
	case OBJE_8:	return(ProcOBJE_8(groupin));	*/

	case U2VI_0:	printf("*** View loaded ***\n");
			return(ProcU2VI_0(groupin));
	case ATTR_0:	return(ProcATTR_0(groupin));
	case OBJE_0:	return(ProcOBJE_0(groupin));

	default:	return(BAD_IFF);
	}

	return(IFF_OKAY);

}

VOID FixAttribs(setcur)
BOOL setcur;
{
	
	GROUP_A *grp;

	int a;


	for (a=1; a<group_m; a++) {

		grp = Group_Array[a];
		if (grp==NULL) continue;

		if (grp->parent) grp->parent =
			Group_Array[(int) grp->parent];
		else {
			if (a!=1) 
			   printf("Got a group root which is not %d\n",a);
			if (setcur) Root_Group = grp;
			else	temp_root = grp;
		}

		grp->desc_last = NULL;
		if (grp->next) grp->next = Group_Array[(int) grp->next];
		if (grp->next==NULL&&grp->parent!=NULL) 
			grp->parent->desc_last = grp;
		if (grp->previous) grp->previous =
			Group_Array[(int) grp->previous];
		if (grp->descendants) grp->descendants = 
			Group_Array[(int) grp->descendants];

	}

	if (setcur) {
		Current_Group = Root_Group;
		Current_Layer = Root_Layer;
		Current_Pen = Root_Pen;
		Current_Style = Root_Style;
		Default_Pen = Current_Pen;
		Default_Style = Current_Style;

		if (group_c>0) Current_Group = Group_Array[group_c];
		if (layer_c>0) Current_Layer = Layer_Array[layer_c];
		if (pen_c>0)   Current_Pen = 	Pen_Array[pen_c];
		if (style_c>0) Current_Style = Style_Array[style_c];
		if (pen_t>0)   Default_Pen = 	Pen_Array[pen_t];
		if (style_t>0) Default_Style = Style_Array[style_t];

		B_Wit_Layer = TRUE;
		B_Wit_Group = TRUE;
		B_Wit_Pen = TRUE;
		
		B_Dim_Line_Layer = TRUE;
		B_Dim_Line_Group = TRUE;
		B_Dim_Line_Pen = TRUE;
		
		B_Dim_Box_Layer = TRUE;
		B_Dim_Box_Group = TRUE;
		B_Dim_Box_Pen = TRUE;
		B_Dim_Box_Style = TRUE;


		if (witlayer_c==0) witlayer_c = -1;
		if (witgroup_c==0) witgroup_c = -1;
		if (witpen_c==0) witpen_c = -1;

		if (dimlinlayer_c==0) dimlinlayer_c = -1;
		if (dimlingroup_c==0) dimlingroup_c = -1;
		if (dimlinpen_c==0) dimlinpen_c = -1;

		if (dimboxlayer_c==0) dimboxlayer_c = -1;
		if (dimboxgroup_c==0) dimboxgroup_c = -1;
		if (dimboxpen_c==0) dimboxpen_c = -1;
		if (dimboxstyle_c==0) dimboxstyle_c = -1;

		Wit_Layer = Layer_Array[abs(witlayer_c)];
		if (witlayer_c<0) B_Wit_Layer = FALSE;
		Wit_Group = Group_Array[abs(witgroup_c)];
		if (witgroup_c<0) B_Wit_Group = FALSE;
		Wit_Pen = Pen_Array[abs(witpen_c)];
		if (witpen_c<0) B_Wit_Pen = FALSE;

		Dim_Line_Layer = Layer_Array[abs(dimlinlayer_c)];
		if (dimlinlayer_c<0) B_Dim_Line_Layer = FALSE;
		Dim_Line_Group = Group_Array[abs(dimlingroup_c)];
		if (dimlingroup_c<0) B_Dim_Line_Group = FALSE;
		Dim_Line_Pen = Pen_Array[abs(dimlinpen_c)];
		if (dimlinpen_c<0) B_Dim_Line_Pen = FALSE;

		Dim_Box_Layer = Layer_Array[abs(dimboxlayer_c)];
		if (dimboxlayer_c<0) B_Dim_Box_Layer = FALSE;
		Dim_Box_Group = Group_Array[abs(dimboxgroup_c)];
		if (dimboxgroup_c<0) B_Dim_Box_Group = FALSE;
		Dim_Box_Pen = Pen_Array[abs(dimboxpen_c)];
		if (dimboxpen_c<0) B_Dim_Box_Pen = FALSE;
		Dim_Box_Style = Style_Array[abs(dimboxstyle_c)];	
		if (dimboxstyle_c<0) B_Dim_Box_Style = FALSE;

	}	

	rmfree((UBYTE *) Group_Array,sizeof(struct Group_Array *) * group_m);

	rmfree((UBYTE *) Layer_Array,sizeof(struct Layer_Array *) * layer_m);

	rmfree((UBYTE *) Style_Array,sizeof(STYLE_A *) *style_m);

	rmfree((UBYTE *) Pen_Array,sizeof(PEN_A *) * pen_m);


}

VOID CheckClip()
{

	if (!setclip) return;

	if (lastnode==NULL) return;

	lastnode->node_flags |= NODE_INCLIP;

}

VOID getprep()
{
	T_HATCH *hatchloop;

	for (hatchloop=Root_Hatch; hatchloop!=NULL;
			hatchloop=hatchloop->next) {
		hatchloop->Use_Count = 0;
		hatchloop->Hatch_ID = 0;
	}
}

IFFP getview(path,file,infnm,libmode)
UBYTE *path;
UBYTE *file;
UBYTE *infnm;
BOOL libmode;
{

	BPTR	infile;
	IFFP	retiffp;
	long	errret,loop;

	ClientFrame mainframe;

	grpoffs = 0;

	if (Win_Setup) {
		SExitSub(NULL);		/* Clear any secondary modes */

		InfoMess(LOAD_MODE,LOAD_MESS_LOAD);
	}

	infile=GOpen(infnm,MODE_OLDFILE);
	if (infile==0) {
		errret=IoErr();

		printf("Cannot get %s - error %ld\n",infnm,errret);
		InfoMess(LOAD_MODE,LOAD_MESS_ERROR);

		return(NO_FILE);
	}

	freelibs();		/* If loaded */
	FreeAllPools();		/* Free now that we're serious */
	BaseVirSetup();

	setclip = FALSE;	/* Don't place all in clipboard */

	Root_Style=Style_Last=NULL;
	Root_Layer=Last_Layer=NULL;
	Root_Pen=Last_Pen=NULL;

	CurrentRoller = NULL;		/* Clear the roller */

	witlayer_c = 0;
	witgroup_c = 0;
	witpen_c = 0;
	dimlinlayer_c = 0;
	dimlingroup_c = 0;
	dimlinpen_c = 0;
	dimboxlayer_c = 0;
	dimboxgroup_c = 0;
	dimboxpen_c = 0;
	dimboxstyle_c = 0;

	Wit_Layer = Dim_Line_Layer = Dim_Box_Layer = NULL;
	Wit_Group = Dim_Line_Group = Dim_Box_Group = NULL;
	Wit_Pen = Dim_Line_Pen = Dim_Box_Pen = NULL;
	Dim_Box_Style = NULL;

	for (loop=0; loop<10; loop++) vset[loop] = FALSE;
	
	mainframe.getList=SkipGroup;
	mainframe.getProp=SkipGroup;

	mainframe.getForm = GetForms;
	mainframe.getCat=SkipGroup;

	getprep();

	quietupdate=TRUE;
	ifferror = IFF_OKAY;
	retiffp=ReadIFF(infile, &mainframe);
	quietupdate=FALSE;

	GClose(infile);

	FixAttribs(TRUE);

	mensetup(0x0F+Color_Source);
	mensetup(0x110+lintype);
	mensetup(0x100+angtype);
	fullmenu(0);

	drawingdirty=FALSE;

	Library_Mode = libmode;

	setpath();		/* Fix up the current group information */

	fullzoomout();
	RepaintWindow();

	Savnam = TRUE;
	Savlib = libmode;
	sprintf(Savefile,"%s",file);
	sprintf(Savepath,"%s",path);

	if (retiffp==IFF_OKAY)	InfoMess(LOAD_MODE,LOAD_MESS_GOTIT);
	else			InfoMess(LOAD_MODE,LOAD_MESS_ERROR);

	return(retiffp);

}

IFFP mrgview(infnm,afnm,ingrp)
UBYTE *infnm;
UBYTE *afnm;
GROUP_A *ingrp;
{

	BPTR	infile;
	IFFP	retiffp;
	long	errret,findfile;
	UBYTE 	ptitle[132],grp[40],*fs;
	STYLE_A	*save_style;
	GROUP_A	*save_group,new_grp,*new_group;
	LAYER_A	*save_layer;
	PEN_A	*save_pen;

	ClientFrame mainframe;

	SExitSub(NULL);		/* Clear any secondary modes */

	InfoMess(MERGE_MODE,MERGE_MESS_MERGE);

	save_style = Current_Style;
	save_group = Current_Group;
	save_layer = Current_Layer;
	save_pen = Current_Pen;

	grpoffs = 0;

	infile=GOpen(infnm,MODE_OLDFILE);
	if (infile==0) {
		errret=IoErr();
		printf("Cannot get %s - error %ld",afnm,errret);
		InfoMess(MERGE_MODE,MERGE_MESS_ERROR);

		return(NO_FILE);
	}

	sprintf(grp,"%s",afnm);
	if ((fs=strrchr(grp,'.'))!=NULL) *fs='\0';
	grp[26]='\0';
	if (grp[0]=='\0') sprintf(grp,"newgroup");
	sprintf(ptitle,"%s",grp);

	new_group = Group_Level(ingrp->descendants,ptitle);
	
	if (new_group!=NULL) {
		for (findfile=1; findfile<99999; findfile++) {
			sprintf(ptitle,"%s_%d",grp,findfile);
			new_group = Group_Level(ingrp->descendants,ptitle);
			if (new_group==NULL) break;
		}
	}

	maxcols = 1 << Sdepth;	

	setclip = TRUE;		/* Place all in clipboard */

	mainframe.getList=SkipGroup;
	mainframe.getProp=SkipGroup;

	mainframe.getForm = GetForms;
	mainframe.getCat=SkipGroup;

	temp_root = NULL;

	getprep();

	quietupdate=TRUE;
	ifferror = IFF_OKAY;
	retiffp=ReadIFF(infile, &mainframe);
	quietupdate=FALSE;

	GClose(infile);

	FixAttribs(FALSE);

	if (temp_root==NULL) {
		printf("Root came back null!\n");
		return(CLIENT_ERROR);
	}

	new_grp = *temp_root;

	sprintf(temp_root->Group_Name,"%s",ptitle);
	sprintf(ptitle,"Merged %s",infnm);
	temp_root->Comment = StoreComment(ptitle);
	
	(VOID) Link_Group(temp_root, ingrp);
	temp_root->descendants = new_grp.descendants;
	temp_root->desc_last = new_grp.desc_last;

	Current_Style = save_style;
	Current_Group = save_group;
	Current_Layer = save_layer;
	Current_Pen = save_pen;

	setpath();

	RepaintWindow();

	drawingdirty=TRUE;

	if (retiffp==IFF_OKAY)	InfoMess(MERGE_MODE,MERGE_MESS_GOTIT);
	else			InfoMess(MERGE_MODE,MERGE_MESS_ERROR);

	return(retiffp);

}

