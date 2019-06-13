/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	iffput0.c - Put an iff version 0 file out
 *
 */

/*
 *	Copyright (C) 1906, 1907 by WURBsoft Inc., and Scott Halbert.
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
 *	4	 5-Aug-87 SH	Break apart from 2dviewiff.c to iffput0.c
 *	5	11-Aug-87 SH	Add library mode save
 *	6	22-Aug-87 SH	Small bug kept dead wood eliminator from
 *				 cleaning up groups
 *	7	27-Sep-87 SH	Convert from version 8 to 0 (final rev 1.0)
 *	8	 2-Jan-88 SH	Put in new pen structure for writes
 *	9	 4-Feb-88 SH	Add dimensioning data type and view memory
 *	10	 8-Jan-89 SH	Add hatch save
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include "viewiff.h"
#include <workbench/workbench.h>
/* #include <proto/icon.h> */
	
/*----------------------------------Macros-----------------------------------*/

#define CkErr(expression) { if (ifferror==IFF_OKAY) ifferror=(expression);}
#define div32(x) ((x)>>5)
#define rem32(x) ((x)&0x1F)
#define STACKBUFF 1024

#define rmalloc(x) AllocMem(x,0)
#define rmfree(x,s) FreeMem(x,s)

/*-----------------------------Forward References----------------------------*/

VOID	setzero(),
	dumptables(),
	dumpheaders(),
	putDIME(),
	putBMEM(),
	putLIMS(),
	putPALE(),
	FtoPORT(),
	dumpallgroups(),
	initvect(),
	placevect(),
	flushvect(),
	initcirc(),
	placecirc(),
	flushcirc(),
	placetext(),
	placehatch(),
	TraverseSetup(),
	InventoryAll(),
	CheckClip();


/*-------------------------External Symbol Definitions-----------------------*/

struct Text_Fonts *Stroke_Search();
BOOL LoadStrokeFont();

extern struct ViewPort *vp;

extern BOOL CheckAbort();

/*-------------------------Global Variable Definitions-----------------------*/

IFFP ifferror=IFF_OKAY;

IFFP iffp;
LOCAL WORD group_m,pen_m,style_m,dime_m,layer_m,hatch_m;

LOCAL BOOL setclip;

/*-------------------------------Code Section--------------------------------*/

IFFP IffErr()
{
	IFFP i;
	i=ifferror;
	ifferror=0;
	return(i);
}

/*------------------------Write IFF view file----------------------------*/

IFFP putview(path,file,iflnm,afln,clip,optim,libr)
UBYTE *iflnm,*afln,*path,*file;
BOOL clip,optim,libr;
{

	BPTR	outfile;
	long	errret;
	GroupContext fileContext, formContext;
	UBYTE 	*copyright = "Copyright 1987 - WURBsoft Inc.";
	UBYTE 	ptitle[80],icnam[200];
	UBYTE	*savvec;
	UBYTE	ftmp[120],ftmp2[120];
	struct	DiskObject *newicon;
#if GIO_ACTIVE
	UBYTE wbuff[STACKBUFF];
#endif
	setclip = clip;
	ifferror = IFF_OKAY;

	InfoMess(SAVE_MODE,SAVE_MESS_SAVE);

	sprintf(ftmp,"%s(ucad-temporary).uvw",iflnm);
	sprintf(ftmp2,"%s%s",iflnm,afln);
	outfile=GOpen(ftmp,MODE_NEWFILE);
	if (outfile==0) {
		errret=IoErr();

		printf("Cannot save %s - error %ld\n",afln,errret);
		InfoMess(SAVE_MODE,SAVE_MESS_ERROR);

		return(NO_FILE);
	}

	GWriteDeclare(outfile,&wbuff,STACKBUFF);

	CkErr(OpenWIFF(outfile, &fileContext, szNotYetKnown));

	CkErr(StartWGroup(&fileContext,FORM,szNotYetKnown,U2VI_0,&formContext));

/*	CkErr(PutCk(&formContext, COPY_0, strlen(copyright), 
		(BYTE *) copyright)); */

	InventoryAll(FALSE);
	TraverseSetup();
	dumpheaders(&formContext);
	dumptables(&formContext);
	dumpallgroups(&formContext,libr);
	InventoryAll(FALSE);

	if (ifferror!=IFF_OKAY) {
		printf(ptitle,"Error while saving %s!\n",afln);
		drawingdirty = TRUE;	/* Don't let them escape */
		GClose(outfile);
		DeleteFile(ftmp);	/* Delete the temporary */
		printf("Got an ifferror=%d\n",ifferror);
		perror("iffput0");
		/* poserr("iffput0"); */
		InfoMess(SAVE_MODE,SAVE_MESS_ERROR);
		return(IFF_OKAY);
	}	
	
	CkErr(EndWGroup(&formContext));
	CkErr(CloseWGroup(&fileContext));

	GWriteUndeclare(outfile);

	GClose(outfile);

	DeleteFile(ftmp2);	/* Delete if exists or not */
	if (!Rename(ftmp,ftmp2)) {
		printf("Couldn't rename temporary to your output name!\n");
		return(IFF_OKAY);
	}

	newicon = GetDiskObject(ftmp2);		/* See if already an icon */
	if (newicon!=NULL) {
		FreeDiskObject(newicon);	/* Toss this */
	} else {

		strcpy(icnam,MakeFname(TemplatePath,libr?"library":"view",NULL));

		newicon = GetDiskObject(icnam);		/* Get our template */
		if (newicon==NULL&&libr==FALSE) {
			strcpy(icnam,MakeFname(TemplatePath,"drawing",NULL));
			newicon = GetDiskObject(icnam);	/* Get our template */
		}
		if (newicon!=NULL) {
			strcpy(ftmp,MakeFname(HomePath,VERNAME,NULL));
			savvec = newicon->do_DefaultTool;
			newicon->do_DefaultTool = ftmp;
			newicon->do_CurrentX = NO_ICON_POSITION;
			newicon->do_CurrentY = NO_ICON_POSITION;
			PutDiskObject(ftmp2,newicon);	/* Make icon */
			newicon->do_DefaultTool = savvec; /* Put back */
			FreeDiskObject(newicon);	/* Free old stuff */
		}
	}

	InfoMess(SAVE_MODE,SAVE_MESS_GOTIT);

	if (!clip) {
		Savnam = TRUE;
		Savlib = libr;
		strcpy(Savefile,file);
		strcpy(Savepath,path);
	}

	drawingdirty=FALSE;

	return(IFF_OKAY);
}

VOID dumpallgroups(overcontext,libmod)
GroupContext *overcontext;
BOOL libmod;
{
	GroupContext ThisGroup;
	struct iff_vect_0 *mainvectortable;
	struct iff_circ_0 *maincircletable;
	WORD groupwalker,disgroup,type;	
	int totalwrote=0;

	D_NODE *lnklst;

	CkErr(StartWGroup(overcontext,FORM,szNotYetKnown,OBJE_0,&ThisGroup));

	mainvectortable=rmalloc(sizeof(struct iff_vect_0));

	if (mainvectortable==NULL) {
		printf("Cannot malloc enough memory for iff_vect_0\n");
		return;
	}

	maincircletable=rmalloc(sizeof(struct iff_circ_0));

	if (maincircletable==NULL) {
		printf("Cannot malloc enough memory for iff_circ_0\n");
		return;
	}

	initvect(&ThisGroup,mainvectortable);
	initcirc(&ThisGroup,maincircletable);

	/* Write data out in the group heirarchy order */
	for (groupwalker=0; groupwalker<=group_m; groupwalker++) {

		for (lnklst=Root_Node; lnklst!=NULL; 
			lnklst=lnklst->node_subsequent) {

			if (setclip&&
				!(lnklst->node_flags&NODE_INCLIP)) continue;

			disgroup = lnklst->above_group->Group_ID;
			if (libmod&&disgroup!=groupwalker) continue;

			totalwrote++;

			switch(lnklst->node_id) {
			case NODE_LINE:
				flushcirc(&ThisGroup,maincircletable);
				placevect(&ThisGroup,mainvectortable,lnklst);
				break;
			case NODE_ELLIP:
				flushvect(&ThisGroup,mainvectortable);
				placecirc(&ThisGroup,maincircletable,lnklst);
				break;
			case NODE_TEXT:
				flushvect(&ThisGroup,mainvectortable);
				flushcirc(&ThisGroup,maincircletable);
				placetext(&ThisGroup,lnklst);
				break;
			case NODE_HATCH:
				type = 0;
				if (lnklst->node_subsequent!=NULL) 
					type = lnklst->node_subsequent->node_id;
				switch (type) {
				case NODE_LINE:
				case NODE_ELLIP:
					flushvect(&ThisGroup,mainvectortable);
					flushcirc(&ThisGroup,maincircletable);
					placehatch(&ThisGroup,lnklst);
					break;
				default:
					printf("Bogus hatch purged!\n");
				}
				break;
			}
		}
		if (!libmod) break;
	}

	flushvect(&ThisGroup,mainvectortable);
	flushcirc(&ThisGroup,maincircletable);

	rmfree(mainvectortable,sizeof(struct iff_vect_0));
	rmfree(maincircletable,sizeof(struct iff_circ_0));
	
	CkErr(EndWGroup(&ThisGroup));
}

/*----------------Line Mode Handling-----------------------------------*/

VOID initvect(overcontext,vectin)
GroupContext *overcontext;
struct iff_vect_0 *vectin;
{

	vectin->NumberVects=0;

	return;
}

VOID placevect(overcontext,vectin,vectline)
GroupContext *overcontext;
struct iff_vect_0 *vectin;
D_NODE *vectline;
{

	int curidx;
	O_LINE *inline;

	inline = GetLineData(vectline);		/* Get line pointer */
	
	if (vectin->NumberVects>=VECTORMAX)
		flushvect(overcontext,vectin);

	curidx = vectin->NumberVects;

	vectin->vectbundles[curidx].Group_ID=vectline->above_group->Group_ID;
	vectin->vectbundles[curidx].Layer_ID=vectline->node_layer->Layer_ID;
	vectin->vectbundles[curidx].Pen_ID=vectline->pen_attach->Pen_ID;

	vectin->vectbundles[curidx].VecFlags=inline->line_flags;

	FtoPORT(inline->x1,&(vectin->vectbundles[curidx].p1x));
	FtoPORT(inline->y1,&(vectin->vectbundles[curidx].p1y));
	FtoPORT(inline->x2,&(vectin->vectbundles[curidx].p2x));
	FtoPORT(inline->y2,&(vectin->vectbundles[curidx].p2y));
	
	vectin->NumberVects++;

	return;

}

VOID flushvect(overcontext,vectin)
GroupContext *overcontext;
struct iff_vect_0 *vectin;
{

	unsigned olen;

	if (vectin->NumberVects==0) return;

	vectin->defrev = VECT0_REV;

	olen=sizeof(UWORD)*2;
	olen+=(sizeof(struct _v0bundle)) * vectin->NumberVects;

	CkErr(PutCk(overcontext, VECT_0, olen, (BYTE *) vectin));

	initvect(overcontext,vectin);

	return;

}

/*-----------------Circle/Ellipse Handling---------------------------*/

VOID initcirc(overcontext,circin)
GroupContext *overcontext;
struct iff_circ_0 *circin;
{

	circin->NumberCircs=0;

	return;
}

VOID placecirc(overcontext,circin,circline)
GroupContext *overcontext;
struct iff_circ_0 *circin;
D_NODE *circline;
{

	int bundle;
	O_ELLIP *inellip;

	if (circin->NumberCircs>=CIRCLEMAX)
		flushcirc(overcontext,circin);

	inellip = GetEllipData(circline);

	bundle=circin->NumberCircs++;

	circin->circbundles[bundle].CircFlags=
		CIRC0FLG_ELLIP | CIRC0FLG_LIMTS | CIRC0FLG_ROTA;

	circin->circbundles[bundle].Group_ID=circline->above_group->Group_ID;
	circin->circbundles[bundle].Layer_ID=circline->node_layer->Layer_ID;
	circin->circbundles[bundle].Pen_ID=circline->pen_attach->Pen_ID;

	FtoPORT(inellip->x1,&(circin->circbundles[bundle].centx));
	FtoPORT(inellip->y1,&(circin->circbundles[bundle].centy));
	FtoPORT(inellip->xradius,&(circin->circbundles[bundle].radiusx));
	FtoPORT(inellip->yradius,&(circin->circbundles[bundle].radiusy));
	FtoPORT(inellip->rotation,&(circin->circbundles[bundle].rotation));
	FtoPORT(inellip->fromang,&(circin->circbundles[bundle].starta));
	FtoPORT(inellip->toangle,&(circin->circbundles[bundle].enda));

	return;

}

VOID flushcirc(overcontext,circin)
GroupContext *overcontext;
struct iff_circ_0 *circin;
{

	unsigned olen;

	if (circin->NumberCircs==0) return;

	circin->defrev = CIRC0_REV;

	olen=sizeof(UWORD)*2;
	olen+=(sizeof(struct _c0bundle)) * circin->NumberCircs;

	CkErr(PutCk(overcontext, CIRC_0, olen, (BYTE *) circin));

	initcirc(overcontext,circin);

	return;

}

/*--------------------------Text Handling--------------------------------*/

VOID placetext(overcontext,innode)
GroupContext *overcontext;
D_NODE *innode;
{

	O_TEXT *intext;
	struct iff_text_0 textbuf;
	int olen;

	intext = GetTextData(innode);

	textbuf.defrev = TEXT0_REV;

	FtoPORT(intext->x1,&(textbuf.Xpos));
	FtoPORT(intext->y1,&(textbuf.Ypos));
	FtoPORT(intext->ang,&(textbuf.Rotation));

	textbuf.TextFlags = intext->Text_Bits;
	textbuf.Handle_Pos = intext->Handle_Flag;
	
	textbuf.Group_ID=innode->above_group->Group_ID;
	textbuf.Layer_ID=innode->node_layer->Layer_ID;
	textbuf.Pen_ID=innode->pen_attach->Pen_ID;

	textbuf.Letter_ID = intext->letstyle->Letter_ID;

	textbuf.textlen = strlen(intext->text);
	strcpy(textbuf.TextData,intext->text);

	olen = sizeof (struct iff_text_0) - 256;
	olen += textbuf.textlen;

	CkErr(PutCk(overcontext, TEXT_0, olen, (BYTE *) &textbuf));

}

/*--------------------------Hatch Handling--------------------------------*/

VOID placehatch(overcontext,innode)
GroupContext *overcontext;
D_NODE *innode;
{

	struct hatch_mode *inhatch;
	struct iff_hatch_0 hatchbuf;
	int olen;

	inhatch = GetHatchData(innode);

	hatchbuf.defrev = HATCH0_REV;

	hatchbuf.FillType = inhatch->hatch_type;
	hatchbuf.FillFlags = inhatch->hatch_flags;
	hatchbuf.Number_Polys = inhatch->number_polys;
	
	FtoPORT(inhatch->pattern_rot,&(hatchbuf.Hatch_Rotation));
	FtoPORT(inhatch->pattern_scale,&(hatchbuf.Hatch_Scale));
	
	hatchbuf.Group_ID=innode->above_group->Group_ID;
	hatchbuf.Layer_ID=innode->node_layer->Layer_ID;
	hatchbuf.Pen_ID=innode->pen_attach->Pen_ID;
	
	hatchbuf.Hatch_ID = 0;
	hatchbuf.Block_ID = 0;
	
	if (inhatch->hatch_type==HATCH_PATTERN) 
		hatchbuf.Hatch_ID = inhatch->hatch_pattern->Hatch_ID;

	CkErr(PutCk(overcontext, HATC_0, sizeof(struct iff_hatch_0), (BYTE *) &hatchbuf));

}

/*--------------------------Write out header information--------------------*/

VOID dumpheaders(overcontext)
GroupContext *overcontext;
{

	putLIMS(overcontext);
	putPALE(overcontext);
	putDIME(overcontext);
	putBMEM(overcontext);
}

VOID putLIMS(outcontext)
GroupContext *outcontext;
{

	struct iff_lims_0 outlims;

	outlims.defrev = LIMS0_REV;

	outlims.viewflags = 0;

	if (Center_On)	outlims.viewflags |= LIM0FLAG_CCEN;
	if (Handles_On)	outlims.viewflags |= LIM0FLAG_HAND;
	if (Coord_On)	outlims.viewflags |= LIM0FLAG_COOR;
	if (Snaps_On)	outlims.viewflags |= LIM0FLAG_SNAP;
	if (Grid_On)	outlims.viewflags |= LIM0FLAG_GRID;
	if (Use_Bounds)	outlims.viewflags |= LIM0FLAG_BOUN;
	if (Ortho_Switch) outlims.viewflags |= LIM0FLAG_ORTH;
	if (Q_Hatch)	outlims.viewflags |= LIM0FLAG_QHAT;

	outlims.takecolors = Color_Source;

	outlims.current_group = Current_Group->Group_ID;
	outlims.current_layer = Current_Layer->Layer_ID;
	outlims.current_pen = Current_Pen->Pen_ID;
	outlims.current_style = Current_Style->Letter_ID;
	outlims.template_pen = Default_Pen->Pen_ID;
	outlims.template_style = Default_Style->Letter_ID;

	outlims.max_group = group_m;
	outlims.max_layer = layer_m;
	outlims.max_pen   = pen_m;
	outlims.max_style = style_m;

	FtoPORT(xsnap,&outlims.xsnap);
	FtoPORT(ysnap,&outlims.ysnap);
	FtoPORT(xoffset,&outlims.xoffs);
	FtoPORT(yoffset,&outlims.yoffs);
	FtoPORT(snaprot,&outlims.snaprot);

	FtoPORT(xgrid,&outlims.xgrid);
	FtoPORT(ygrid,&outlims.ygrid);
	FtoPORT(xgridoff,&outlims.xgoff);
	FtoPORT(ygridoff,&outlims.ygoff);
	outlims.xgdiv = xgriddiv;
	outlims.ygdiv = ygriddiv;
	FtoPORT(gridrot,&outlims.gridrot);

	FtoPORT(xviewMIN,&outlims.minx);
	FtoPORT(yviewMIN,&outlims.miny);
	FtoPORT(xviewMAX,&outlims.maxx);
	FtoPORT(yviewMAX,&outlims.maxy);

	outlims.qtextlim = qtextlim;

	FtoPORT(paperunit,&outlims.paperunit);
	FtoPORT(viewunit,&outlims.viewunit);
	FtoPORT(viewscale,&outlims.viewscale);

	FtoPORT(Arrow_Angle,&outlims.arrowangle);
	FtoPORT(Arrow_Len,&outlims.arrowlen);

	FtoPORT(orthoangle,&outlims.orthoang);
	FtoPORT(filletrad,&outlims.filletrad);

	FtoPORT(lintolerance,&outlims.lintolerance);
	FtoPORT(angtolerance,&outlims.angtolerance);
	outlims.linpoints = linpoints;
	outlims.angpoints = angpoints;
	outlims.lintype = lintype;
	outlims.angtype = angtype;

	CkErr(PutCk(outcontext, LIMS_0, sizeof(struct iff_lims_0),(BYTE *) &outlims));

	return;
}

VOID putDIME(outcontext)
GroupContext *outcontext;
{

	struct iff_dime_0 outdime;

	outdime.defrev = DIME0_REV;

	outdime.DimFlags = 0;

	if (Use_Fixed_Angle)	outdime.DimFlags |= DIM0FLAG_FIXED;

#define setdim(a,b,c,d) outdime.a = b->c; if (!d) outdime.a = - (b->c)

	setdim(WitLayer,Wit_Layer,Layer_ID,B_Wit_Layer);
	setdim(WitGroup,Wit_Group,Group_ID,B_Wit_Group);
	setdim(WitPen,Wit_Pen,Pen_ID,B_Wit_Pen);
	
	setdim(DimLinLayer,Dim_Line_Layer,Layer_ID,B_Dim_Line_Layer);
	setdim(DimLinGroup,Dim_Line_Group,Group_ID,B_Dim_Line_Group);
	setdim(DimLinPen,Dim_Line_Pen,Pen_ID,B_Dim_Line_Pen);
	
	setdim(DimBoxLayer,Dim_Box_Layer,Layer_ID,B_Dim_Box_Layer);
	setdim(DimBoxGroup,Dim_Box_Group,Group_ID,B_Dim_Box_Group);
	setdim(DimBoxPen,Dim_Box_Pen,Pen_ID,B_Dim_Box_Pen);
	setdim(DimBoxStyle,Dim_Box_Style,Letter_ID,B_Dim_Box_Style);

	FtoPORT(Wit_Pick,&outdime.WitOffset);
	FtoPORT(Wit_Over,&outdime.WitOver);
	FtoPORT(Dim_Line_Offset,&outdime.DimLinOffset);
	FtoPORT(Dim_Box_Offset,&outdime.DimBoxOffset);
	FtoPORT(Fixed_Angle,&outdime.DimAngle);
	FtoPORT(Free_Flip_From,&outdime.FromFlip);
	FtoPORT(Free_Flip_To,&outdime.ToFlip);
	FtoPORT(Free_Offset,&outdime.FreeOffset);

	CkErr(PutCk(outcontext, DIME_0, sizeof(struct iff_dime_0),
		(BYTE *) &outdime));

	return;
}

VOID putBMEM(outcontext)
GroupContext *outcontext;
{

	struct iff_bmem_0 outbmem;
	int ct;

	for (ct=0; ct<10; ct++) {
		if (!vset[ct]) continue;
		outbmem.defrev = BMEM0_REV;
		outbmem.memflags = BMEM0_KEYPAD;
		outbmem.keyid = ct;
		FtoPORT(xvbeg[ct],&outbmem.xbeg);
		FtoPORT(yvbeg[ct],&outbmem.ybeg);
		FtoPORT(xvsiz[ct],&outbmem.xsiz);
		FtoPORT(yvsiz[ct],&outbmem.ysiz);
		FtoPORT(0.0,&outbmem.viewrot);		/* Not rotated */

		CkErr(PutCk(outcontext, BMEM_0, sizeof(struct iff_bmem_0),
			(BYTE *) &outbmem));
	}

	return;
}

VOID putPALE(outcontext)
GroupContext *outcontext;
{

	struct iff_pale_0 outpal;
	int a,outs,numcols;
	ULONG red,blue,green;

	if (vp->ColorMap==NULL) return;		/* There is no colormap? */

	outpal.defrev = PALE0_REV;

	for (a=0; a<MAXPRESETS; a++) outpal.Presets[a] = -1;	/* Nobody */

	outpal.Presets[PALE0_GRID] = Grid_Color;
	outpal.Presets[PALE0_MIST] = Mist_Color;
	outpal.Presets[PALE0_BNDS] = Bounds_Color;
	outpal.Presets[PALE0_GHST] = Ghost_Color;
	outpal.Presets[PALE0_COOR] = Coord_Color;
	outpal.Presets[PALE0_DRAW] = Draw_Color;
	outpal.Presets[PALE0_ERAS] = Erased_Color;

	numcols = 1 << Sdepth;	

	outpal.NumUcad = 4;		/* Four normal ucad dedicated */
	outpal.NumSpec = 2;		/* Two special purpose */
	outpal.NumTot = numcols;

	for (a=0; a<numcols; a++) {
		GI_GetColor(a, &red,&green,&blue);

		outpal.colors[a].RedMix = red;
		outpal.colors[a].GreenMix = green;
		outpal.colors[a].BlueMix = blue;
	}

	outs = sizeof(struct iff_pale_0);
	outs -= sizeof(struct _pale0_sets) * (MAXCOLOR-numcols);

	CkErr(PutCk(outcontext,PALE_0,outs,(BYTE *) &outpal));

}

/*---------------------------Write out tables----------------------------*/

VOID SetGroup(ingrp,setvar)
GROUP_A *ingrp;
int setvar;
{

	for (; ingrp!=NULL; ingrp=ingrp->next) {
		ingrp->Use_Count = setvar;

		if (ingrp->descendants!=NULL) 
			SetGroup(ingrp->descendants,setvar);

	}
}

VOID CountLayer(layerloop,set)
LAYER_A *layerloop;
int set;
{
	if (layerloop==NULL) return;
	if (layerloop->Use_Count<0) layerloop->Use_Count = set;
	else if (set==1) layerloop->Use_Count++;
}

VOID CountPen(penloop,set)
PEN_A *penloop;
int set;
{
	if (penloop==NULL) return;
	if (penloop->Use_Count<0) penloop->Use_Count = set;
	else if (set==1) penloop->Use_Count++;
}
	
VOID CountGroup(grouploop,set)
GROUP_A *grouploop;
int set;
{
	GROUP_A *dgroup;

	if (grouploop==NULL) return;
	if (grouploop->Use_Count<0) grouploop->Use_Count = set;
	else if (set==1) grouploop->Use_Count++;

	for (dgroup = grouploop->parent; dgroup!=NULL; 
			dgroup = dgroup->parent) {
		if (dgroup->Use_Count<0) dgroup->Use_Count = 0;
	}			/* Get all ancestors */
}

VOID CountStyle(styleloop,set)
STYLE_A *styleloop;
int set;
{
	if (styleloop==NULL) return;
	if (styleloop->Use_Count<0) styleloop->Use_Count = set;
	else if (set==1) styleloop->Use_Count++;
}		

VOID CountHatch(hatchloop,set)
T_HATCH *hatchloop;
int set;
{
	if (hatchloop==NULL) return;
	hatchloop->Use_Count++;
}		

VOID InventoryAll(setneg)
BOOL setneg;
{

	D_NODE *lnklst;

	int itemcount=0,setvar;
	STYLE_A *styleloop;
	LAYER_A *layerloop;
	PEN_A *penloop;
	GROUP_A *grouploop,*dgroup;
	T_HATCH *hatchloop;

	setvar = 0;
	if (setneg) setvar = -1;
	
	for (styleloop=Root_Style; styleloop!=NULL; 
			styleloop=styleloop->next_style) {
 		styleloop->Use_Count = setvar;
	}

	for (layerloop=Root_Layer; layerloop!=NULL; 
			layerloop=layerloop->layer_next) {
		layerloop->Use_Count = setvar;
	}

	for (penloop=Root_Pen; penloop!=NULL; penloop=penloop->pen_next) {
		penloop->Use_Count = setvar;
	}
	
	for (hatchloop=Root_Hatch; hatchloop!=NULL;
			hatchloop=hatchloop->next) {
		hatchloop->Use_Count = 0;
	}

	SetGroup(Root_Group,setvar);

	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {

		if (setclip&&!(lnklst->node_flags&NODE_INCLIP)) continue;
		itemcount++;

		grouploop = lnklst->above_group;
		layerloop = lnklst->node_layer;
		penloop = lnklst->pen_attach;
		styleloop = NULL;
		if (lnklst->node_id==NODE_TEXT) 
			styleloop = GetTextData(lnklst)->letstyle;
		hatchloop = NULL;
		if (lnklst->node_id==NODE_HATCH&&
		     (GetHatchData(lnklst)->hatch_type==HATCH_PATTERN)) 
		      	hatchloop = GetHatchData(lnklst)->hatch_pattern;

		CountLayer(layerloop,1);		
		CountPen(penloop,1);
		CountGroup(grouploop,1);	
		CountStyle(styleloop,1);
		CountHatch(hatchloop,1);
		
	}

	CountGroup(Root_Group,0);
	CountLayer(Current_Layer,0);
	CountPen(Current_Pen,0);
	CountStyle(Current_Style,0);
	CountGroup(Current_Group,0);
	CountPen(Default_Pen,0);
	CountStyle(Default_Style,0);
	CountLayer(Wit_Layer,0);
	CountGroup(Wit_Group,0);
	CountPen(Wit_Pen,0);
	CountLayer(Dim_Line_Layer,0);
	CountGroup(Dim_Line_Group,0);
	CountPen(Dim_Line_Pen,0);
	CountLayer(Dim_Box_Layer,0);
	CountGroup(Dim_Box_Group,0);
	CountPen(Dim_Box_Pen,0);
	CountStyle(Dim_Box_Style,0);	

}

VOID RecurLoop(ingrp)
GROUP_A *ingrp;
{

	for (; ingrp!=NULL; ingrp=ingrp->next) {

		ingrp->Group_ID = -1;
		if (ingrp->Use_Count>=0) ingrp->Group_ID = group_m++;

		if (ingrp->descendants!=NULL) RecurLoop(ingrp->descendants);

	}
}

VOID TraverseSetup()
{

	STYLE_A *styleloop;
	LAYER_A *layerloop;
	PEN_A *penloop;
	T_HATCH *hatchloop;

	group_m = pen_m = style_m = dime_m = layer_m = hatch_m = 1;

	for (styleloop=Root_Style; styleloop!=NULL; 
		styleloop=styleloop->next_style) {

		styleloop->Letter_ID = -1;
		if (styleloop->Use_Count<0) continue;

		styleloop->Letter_ID = style_m++;
	}

	for (layerloop=Root_Layer; layerloop!=NULL; 
		layerloop=layerloop->layer_next) {

		layerloop->Layer_ID = -1;
		if (layerloop->Use_Count<0) continue;

		layerloop->Layer_ID = layer_m++;
	}

	for (penloop=Root_Pen; penloop!=NULL; penloop=penloop->pen_next) {
	
		penloop->Pen_ID = -1;
		if (penloop->Use_Count<0) continue;

		penloop->Pen_ID = pen_m++;
	}
	
	for (hatchloop=Root_Hatch; hatchloop!=NULL;
			hatchloop=hatchloop->next) {
		hatchloop->Hatch_ID = 0;
		if (hatchloop->Use_Count<=0) continue;
		
		hatchloop->Hatch_ID = hatch_m++;
	}
		
	RecurLoop(Root_Group);

}

LOCAL struct iff_grpi_0 hgbuf;

VOID putGRPI(inptr,outcontext)
GROUP_A *inptr;
GroupContext *outcontext;
{

	int i,osiz;
	GROUP_A *repair;

	for (; inptr!=NULL; inptr=inptr->next) {

		if (inptr->Group_ID<0) {
			continue;
		}
		
		hgbuf.defrev = GRPI0_REV;
		hgbuf.GroupID=inptr->Group_ID;
		strcpy(hgbuf.GroupName,inptr->Group_Name);
		for (i=strlen(hgbuf.GroupName); i<32; i++)
			hgbuf.GroupName[i]='\0';
		hgbuf.Color_ID=inptr->Color_ID;
		hgbuf.Group_Flags=inptr->Group_Flags;
		hgbuf.Libr_Flags=inptr->Libr_Flags;

		strcpy(hgbuf.Comment,inptr->Comment);

		hgbuf.Parent=0;
		hgbuf.Next=0;
		hgbuf.Previous=0;
		hgbuf.Descendants=0;

		if (inptr->parent!=NULL) hgbuf.Parent=inptr->parent->Group_ID;

		repair = inptr->next;
		while(repair!=NULL) {
			if (repair->Group_ID<0) {
				repair = repair->next;
				continue;
			}

			hgbuf.Next=repair->Group_ID;
			break;
		}

		repair = inptr->previous;
		while (repair!=NULL) {
			if (repair->Group_ID<0) {
				repair = repair->previous;
				continue;
			}

			hgbuf.Previous=	repair->Group_ID;
			break;
		}

		repair = inptr->descendants;
		while (repair!=NULL) {
			if (repair->Group_ID<0) {
				repair = repair->next;
				continue;
			}

			hgbuf.Descendants=repair->Group_ID;
			break;
		}

		osiz = sizeof(struct iff_grpi_0) - 255;
		osiz += strlen(hgbuf.Comment);

		CkErr(PutCk(outcontext,GRPI_0,osiz,(BYTE *) &hgbuf));

		if (repair!=NULL) 
			putGRPI(repair,outcontext);

	}
}

VOID putLYER(outcontext)
GroupContext *outcontext;
{

	int osiz;
	struct iff_lyer_0 lyerbuf;
	LAYER_A *lyer;

	for (lyer=Root_Layer; lyer!=NULL; lyer=lyer->layer_next) {
		if (lyer->Layer_ID<0) continue;
		lyerbuf.defrev = LYER0_REV;
		lyerbuf.LayerID=lyer->Layer_ID;
		strcpy(lyerbuf.LayerName,lyer->Layer_Name);
		lyerbuf.Color_ID = lyer->Color_ID;
		lyerbuf.Layer_Flags = lyer->Layer_Flags;
		strcpy(lyerbuf.Comment,lyer->Comment);
			
		osiz = sizeof(struct iff_lyer_0) - 255;
		osiz += strlen(lyerbuf.Comment);

		CkErr(PutCk(outcontext,LYER_0,osiz,(BYTE *) &lyerbuf));
	}
}

VOID putPENS(outcontext)
GroupContext *outcontext;
{

	int osiz;
	struct iff_pens_0_1 penbuf;		/* New pen record type */
	PEN_A *pen;

	for (pen=Root_Pen; pen!=NULL; pen=pen->pen_next) {
		if (pen->Pen_ID<0) continue;
		penbuf.defrev = PENS0_REV1;
		penbuf.PenID=pen->Pen_ID;
		strcpy(penbuf.PenName,pen->Pen_Name);
		penbuf.Color_ID=pen->Color_ID;
		penbuf.Pen_Flags=pen->Pen_Flags;
		penbuf.Numbits=16;
		penbuf.Pattern1=pen->Line_Pattern;
		penbuf.HardColor=pen->Hard_Color;
		penbuf.PenOrd = pen->PenOrd;
		penbuf.Carousel = pen->Carousel;
		penbuf.AutoNum = pen->AutoNum;

		strcpy(penbuf.Comment,pen->Comment);
		
		osiz = sizeof(struct iff_pens_0_1) - 255;
		osiz += strlen(penbuf.Comment);

		CkErr(PutCk(outcontext,PENS_0,osiz,(BYTE *) &penbuf));
	}
}

VOID putTXST(outcontext)
GroupContext *outcontext;
{

	int osiz;
	struct iff_txst_0 textbuf;
	STYLE_A *text;

	for (text=Root_Style;text!=NULL; text=text->next_style) {
		if (text->Letter_ID<0) continue;
		textbuf.defrev = TXST0_REV;
		textbuf.LetterID=text->Letter_ID;
		strcpy(textbuf.StyleName,text->Letter_Style_ID);
		strcpy(textbuf.FontName,text->draw_font->Font_Name_ID);
		FtoPORT(text->Paper_Height,&textbuf.Paper_Height);
		FtoPORT(text->Prime_Height,&textbuf.Prime_Height);
		FtoPORT(text->Aspect,&textbuf.Aspect);
		FtoPORT(text->Slant,&textbuf.Slant_Angle);

		strcpy(textbuf.Comment,text->Comment);
		
		osiz = sizeof(struct iff_txst_0) - 255;
		osiz += strlen(textbuf.Comment);

		CkErr(PutCk(outcontext,TXST_0,osiz,(BYTE *) &textbuf));

	}
}

VOID putPATT(outcontext)
GroupContext *outcontext;
{

	int osiz;
	struct iff_patt_0 pattbuf;
	T_HATCH *patt;

	for (patt=Root_Hatch;patt!=NULL; patt=patt->next) {
		if (patt->Hatch_ID<=0) continue;
		
		pattbuf.defrev = PATT0_REV;
		pattbuf.PatternID=patt->Hatch_ID;
		
		strcpy(pattbuf.File_Name,patt->File_Name);
		strcpy(pattbuf.Hatch_Name,patt->Hatch_Name);
		
		pattbuf.Pattern_Type = PATT0FLG_SIMPAT;
		
		osiz = sizeof(struct iff_patt_0);

		CkErr(PutCk(outcontext,PATT_0,osiz,(BYTE *) &pattbuf));

	}
}

VOID dumptables(overcontext)
GroupContext *overcontext;
{

	GroupContext ThisGroup;

	CkErr(StartWGroup(overcontext,FORM,szNotYetKnown,ATTR_0,&ThisGroup));

	putGRPI(Root_Group,&ThisGroup);
	putLYER(&ThisGroup);
	putPENS(&ThisGroup);
	putTXST(&ThisGroup);
	putPATT(&ThisGroup);

	CkErr(EndWGroup(&ThisGroup));
}
