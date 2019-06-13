/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	ifflib0 - get a type 0 library iff file
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
 *	5	15-Aug-87 SH	Add the library scroller code
 *	6	 4-Oct-87 SH	Begin rev 0 of library mode
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include "viewiff.h"
#include "libraries.h"
	
/*----------------------------------Macros-----------------------------------*/

#define CkErr(expression) { if (ifferror==IFF_OKAY) ifferror=(expression);}
#define div32(x) ((x)>>5)
#define rem32(x) ((x)&0x1F)
#define STACKBUFF 1024

#define rmalloc(x) AllocMem(x,0)
#define rmfree(x,s) FreeMem(x,s)

/*-----------------------------Forward References----------------------------*/

struct libdirect *Lib_Search();

/*-------------------------External Symbol Definitions-----------------------*/

struct Text_Fonts *Stroke_Search();
BOOL LoadStrokeFont();

extern BOOL CheckAbort();

/*-------------------------Global Variable Definitions-----------------------*/

extern	IFFP ifferror;

extern	BOOL	drawingdirty;

extern	IFFP iffp;
LOCAL	WORD 	group_m=0,
		pen_m=0,
		style_m=0,
		layer_m=0;

extern	GROUP_A **Group_Array;
extern	PEN_A **Pen_Array;
extern	LAYER_A **Layer_Array;
extern	STYLE_A **Style_Array;

LOCAL	struct libdirect **Lib_Array = NULL;

LOCAL	PEN_A **B_Pen_Array = NULL;
LOCAL	LAYER_A **B_Layer_Array = NULL;
LOCAL	STYLE_A **B_Style_Array = NULL;

extern	APTR	scrollanswer;

struct	libdirect	*Root_Library=NULL,
			*Current_Library=NULL,
			*Current_Item=NULL;

struct drawing_node 	*Item_Root=NULL,
			*Item_Last=NULL;

STDFLT	lib_rotate = 0.0;	/* Insert rotation amount */
STDFLT	lib_scale = 1.0;	/* Insert scaling */
BOOL	lib_yflip = FALSE;	/* Flip Y coords over */
BOOL	lib_xflip = FALSE;	/* Flip X coords over */
BOOL	lib_autoclip = TRUE;	/* Clip on insert */

extern struct loadchunk {
	UBYTE 	*getchunk;
	ULONG	sizechunk;
} insave;
struct loadchunk StuffChunk(/* GroupContext * */);

/*-------------------------------Code Section--------------------------------*/

IFFP IffErr();
VOID setzero();
float PORTtoF();
VOID FtoPORT();
float infloat();

/*-------------------------Version 0 support------------------------------*/

IFFP loadLibLIMS_0(groupin)
GroupContext *groupin;
{

	struct iff_lims_0 *limbuf;
	int t;

	insave = StuffChunk(groupin);
	limbuf = (struct iff_lims_0 *) insave.getchunk;

	group_m	= limbuf->max_group+1;
	layer_m = limbuf->max_layer+1;
	pen_m	= limbuf->max_pen+1;
	style_m	= limbuf->max_style+1;

	Lib_Array = (struct libdirect **) 
		rmalloc(sizeof(struct Lib_Array *) * group_m);

	if (Lib_Array==NULL) {
		printf("Couldn't allocate the Lib_Array %d\n",group_m);
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

	for (t=0; t<group_m; t++) Lib_Array[t]=NULL;
	for (t=0; t<style_m; t++) Style_Array[t]=NULL;
	for (t=0; t<layer_m; t++) Layer_Array[t]=NULL;
	for (t=0; t<pen_m; t++) Pen_Array[t]=NULL;

	FreeChunk(insave);

	return(IFF_OKAY);
}

IFFP ProcLibU2VI_0(groupin)
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
			iffp=loadLibLIMS_0(&thiscontext);
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

IFFP loadLibGRPI_0(incontext)
GroupContext *incontext;
{

	struct iff_grpi_0 *grpbuf;
	struct libdirect *newgroup;

	insave = StuffChunk(incontext);
	grpbuf = (struct iff_grpi_0 *) insave.getchunk;

	if (grpbuf==NULL) return(CLIENT_ERROR);	/* Out of memory */

	newgroup = (struct libdirect *) 
			GI_DrawAlloc(sizeof(struct libdirect));

	if (newgroup==NULL) {
		printf("No memory to save group id %s",grpbuf->GroupName);
		return(CLIENT_ERROR);
	}

	printf("Loaded group %s id %d\n",grpbuf->GroupName,grpbuf->GroupID);

	newgroup->Cur_ID = grpbuf->GroupID;
	sprintf(newgroup->Group_Name,"%s",grpbuf->GroupName);
	newgroup->Color_ID = grpbuf->Color_ID;
	newgroup->Group_Flags = grpbuf->Group_Flags;
	newgroup->Libr_Flags = grpbuf->Libr_Flags;
	newgroup->parent = (int) grpbuf->Parent;
	newgroup->next = (int) grpbuf->Next;
	newgroup->previous = (int) grpbuf->Previous;
	newgroup->descendants = (int) grpbuf->Descendants;
	newgroup->desc_last = NULL;
	newgroup->Comment = StoreComment(grpbuf->Comment);

	Lib_Array[newgroup->Cur_ID] = newgroup;

	FreeChunk(insave);

	return(IFF_OKAY);

}

IFFP ProcLibATTR_0(incontext)
GroupContext *incontext;
{
	GroupContext thiscontext;
	
	printf("ProcLibATTR_0\n");

	iffp=OpenRGroup(incontext, &thiscontext);
	CheckIFFP();

	do
		switch (iffp=GetF1ChunkHdr(&thiscontext)) {

		case GRPI_0:
			fprintf(stdout,"GRPI\n");
			iffp=loadLibGRPI_0(&thiscontext);
			CheckIFFP();
			break;

		case LYER_0:
			fprintf(stdout,"LYER\n");
			iffp=loadLYER_0(&thiscontext);
			CheckIFFP();
			break;

		case PENS_0:
			fprintf(stdout,"PENS\n");
			iffp=loadPENS_0(&thiscontext);
			CheckIFFP();
			break;

		case TXST_0:
			fprintf(stdout,"TXST\n");
			iffp=loadTXST_0(&thiscontext);
			CheckIFFP();
			break;

		} while (iffp >= IFF_OKAY);

	return(CloseRGroup(&thiscontext));	
	
}

IFFP ProcGetATTR_0(incontext)
GroupContext *incontext;
{
	GroupContext thiscontext;

	printf("ProcGetATTR_0\n");

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

		} while (iffp >= IFF_OKAY);

	return(CloseRGroup(&thiscontext));	
	
}

IFFP GetLibForms(incontext)
GroupContext *incontext;
{

	switch (incontext->subtype) {

	case U2VI_0:	
		printf("*** Type 0 Library Set ***\n");
		return(ProcLibU2VI_0(incontext));
	case ATTR_0:	return(ProcLibATTR_0(incontext));

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

int lastmax = 0;
SetLast(ingrp)
struct libdirect *ingrp;
{

	if (ingrp==NULL) {
		lastmax = 0;
		ingrp = Root_Library;
	}

	for (; ingrp!=NULL; ingrp = ingrp->next) {
		if (ingrp->Cur_ID>lastmax) lastmax = ingrp->Cur_ID;
		if (ingrp->descendants) SetLast(ingrp->descendants);
		ingrp->Last_ID = lastmax;

	}
}

VOID FixGroup()
{
	
	struct libdirect *grp;

	int a;

	for (a=0; a<group_m; a++) {
		grp = Lib_Array[a];
		if (grp==NULL) continue;

		if (grp->parent) grp->parent =
			Lib_Array[(int) grp->parent];
		else	Root_Library = grp;

		if (grp->next) 
			grp->next = Lib_Array[(int) grp->next];
		if (grp->previous) 
			grp->previous = Lib_Array[(int) grp->previous];
		if (grp->descendants) 
			grp->descendants = Lib_Array[(int) grp->descendants];

	}

	SetLast(NULL);

}

freelibs()
{
	struct drawing_node *prvnod;

	if (Lib_Array) 
		rmfree((UBYTE *) Lib_Array,
		sizeof(struct Lib_Array *) * group_m);

	if (B_Layer_Array)
		rmfree((UBYTE *) B_Layer_Array,
		sizeof(struct Layer_Array *) * layer_m);

	if (B_Style_Array)
		rmfree((UBYTE *) B_Style_Array,
		sizeof(STYLE_A *) *style_m);

	if (B_Pen_Array)
		rmfree((UBYTE *) B_Pen_Array,
		sizeof(PEN_A *) * pen_m);

	Lib_Array = NULL;
	B_Layer_Array = NULL;
	B_Style_Array = NULL;
	B_Pen_Array = NULL;

	while (Item_Last) {			/* Clear out old lib entry */
		prvnod = Item_Last->node_previous;
		FreeMem(Item_Last, Item_Last->node_length);
		Item_Last = prvnod;
	}

	Item_Root = NULL;
	Item_Last = NULL;

	Current_Library = NULL;
	Root_Library = NULL;
	Current_Item = NULL;

}

UBYTE	uselibrary[200];

IFFP getlib(infnm,afnm)
UBYTE *infnm,*afnm;
{

	BPTR	infile;
	IFFP	retiffp;
	long	errret;
	UBYTE ptitle[80];

	ClientFrame mainframe;
	
	struct	Group_Attrib	*curgrpsave;
	struct	Layer_Attrib	*curlaysave;
	struct	Pen_Attrib	*curpensave;

	curgrpsave = Current_Group;
	curlaysave = Current_Layer;
	curpensave = Current_Pen;
	
	grpoffs = 0;

	sprintf(ptitle,"Use library %.40s",afnm);
	DispMess(ptitle);

	infile=GOpen(infnm,MODE_OLDFILE);
	if (infile==0) {
		errret=IoErr();
		sprintf(ptitle,"Cannot get %s - error %ld",
			afnm,errret);
		DispMess(ptitle);
		return(NO_FILE);
	}

	freelibs();
	
	group_m=pen_m=style_m=layer_m=0;

	mainframe.getList=SkipGroup;
	mainframe.getProp=SkipGroup;

	mainframe.getForm = GetLibForms;
	mainframe.getCat=SkipGroup;

	quietupdate=TRUE;
	ifferror = IFF_OKAY;
	
	retiffp=ReadIFF(infile, &mainframe);
	
	quietupdate=FALSE;

	GClose(infile);

	FixGroup();
	
	B_Pen_Array = Pen_Array;
	B_Layer_Array = Layer_Array;
	B_Style_Array = Style_Array;

	Current_Group = curgrpsave;
	Current_Layer = curlaysave;
	Current_Pen = curpensave;

	setpath();	/* Just in case */
	
	UpdateTitle();	

	Current_Library = Root_Library;
	sprintf(uselibrary,"%s",infnm);
	printf("Using library %s, address %x\n",uselibrary,Current_Library);

	return(retiffp);

}

extern UBYTE LibrPath[],ArgText[];
BOOL Req_Use_Libr();

BOOL Use_Libr()
{

	UBYTE fname[255];
	int a;

	if (!Req_Use_Libr()) return(FALSE);

	if ((a=strlen(LibrPath))==0) sprintf(fname,"%s",ArgText);
	else if (a>0&&LibrPath[a-1]==':')sprintf(fname,"%s%s",LibrPath,ArgText);
	else sprintf(fname,"%s/%s",LibrPath,ArgText);

	getlib(fname,ArgText);

	return(FALSE);
}

/*----------------------------Select Item Code-----------------------------*/

UBYTE 	ItemPath[300];
	
struct libdirect *Group_Search();

VOID LibGet()
{

	int a;
	char *fp,*sp;
	UBYTE nbuf[40];
	struct libdirect *srch;

	Current_Library = Root_Library;
	
	sp = ItemPath;
	while (*sp==' '||*sp=='\t'||*sp=='/') sp++;

	a = 0;
	FOREVER {
		ItemPath[a++] = *sp;
		if (*sp=='\0') break;
		sp++;
	}

	fp = ItemPath;

	while (fp!=NULL) {
		sp = strchr(fp,'/');
		if (sp!=NULL) *sp='\0';
		
		sprintf(nbuf,"%s",fp);
		fp = sp;
		if (fp!=NULL) fp++;

		srch = Lib_Search(nbuf);	/* Try to find it */

		if (srch==NULL||(int)srch==2) {
			setlibpath();
			return;
		}

		Current_Library = srch;

	}

	setlibpath();
	return;
}

setlibpath()
{

	struct libdirect *ingp;
	UBYTE	slosh[257];

	ingp = Current_Library;
	ItemPath[0]='\0';

	for (; ingp!=NULL; ingp = ingp->parent) {
		if (ingp->parent==NULL) break;

		if (ItemPath[0]=='\0') sprintf(ItemPath,"%s",ingp->Group_Name);
		else {
			sprintf(slosh,"%s/%s",ingp->Group_Name,ItemPath);
			sprintf(ItemPath,"%s",slosh);
		}
	}

}

/*------------------------Set up scroller for styles------------------------*/

struct libdirect *Lib_Parent(current_in,inmask,descend)
struct libdirect *current_in;
UBYTE *inmask;
BOOL descend;
{

	if (Current_Library->parent==NULL) return(NULL);

	if (descend) {
		Current_Library = Current_Library->parent;
		setlibpath();
		return(NULL);
	}

	return((struct libdirect *) 1); /* Flag 1 will specify parent */

}

struct libdirect *Lib_Next(current_in,inmask)
struct libdirect *current_in;
UBYTE *inmask;
{

	if (current_in==NULL) return(NULL);

	FOREVER {
		current_in=current_in->next;
		if (current_in==NULL) return(NULL);

		if (match(inmask,current_in->Group_Name))
			return(current_in);

	}
}

struct libdirect *Lib_Prev(current_in,inmask)
struct libdirect *current_in;
UBYTE *inmask;
{

	FOREVER {
		current_in=current_in->previous;
		if (current_in==NULL) return(NULL);
	
		if (match(inmask,current_in->Group_Name))
			return(current_in);

	}
}

struct libdirect *Lib_First(current_in,inmask,descend)
struct libdirect *current_in;
UBYTE *inmask;
BOOL descend;
{

	if (descend) LibGet();

	current_in = Current_Library->descendants;

	if (current_in==NULL) return(NULL);

	if (match(inmask,current_in->Group_Name))
		return(current_in);

	FOREVER {
		current_in=current_in->next;
		if (current_in==NULL) return(NULL);
	
		if (match(inmask,current_in->Group_Name))
			return(current_in);
	}

}

struct libdirect *Lib_Search(insearch)
UBYTE *insearch;
{

	struct libdirect *looper;

	for (looper=Current_Library->descendants; looper!=NULL; 
				looper=looper->next) 
		if (match(insearch,looper->Group_Name)) return(looper);

	return(NULL);

}

struct libdirect *Lib_Descend(current_in,inmask,descend)
struct libdirect *current_in;
UBYTE *inmask;
BOOL descend;
{

	if ((int) current_in==1) return(NULL);	/* Here is the parent */

	if (current_in->Libr_Flags&LIBR_ASSY) return(NULL);

	if (descend) {
		Current_Library = current_in;
		setlibpath();
		return(NULL);
	}
			
	return(current_in);

}

VOID Lib_Form(current_in,outform)
struct libdirect *current_in;
UBYTE *outform;
{

	if (current_in==NULL) return;

	if (((int) current_in)==1) {
		sprintf(outform,"/ Parent");
		return;
	}

	sprintf(outform,"  %-32.32s",current_in->Group_Name);

}


VOID Lib_Name(current_in,outform)
struct libdirect *current_in;
UBYTE *outform;
{

	if (current_in==NULL) return;

	sprintf(outform,"%s",current_in->Group_Name);

}

BOOL Lib_Sc(title,load,header,Holf,exist)
UBYTE *title,*load,*header;
struct libdirect **Holf;
BOOL exist;
{

	BOOL ret;

	ret=Scroller(Lib_First,
		Lib_Next,
		Lib_Prev,
		Lib_Search,
		Lib_Parent,
		Lib_Descend,
		Lib_Form,
		Lib_Name,
		TRUE,
		TRUE,
		exist,
		*Holf,
		load,
		"  Cancel  ",
		title,
		header,
		Holf,
		ItemMask,
		ItemPath);

	return(ret);

}

LOCAL struct libdirect *curlib=NULL;
BOOL Lib_CurrentItem()
{

	struct 	libdirect *savecurrent;
	struct	drawing_node	*save_root,*save_last,*prvnod;
	int 	groupelements,i;

	BPTR	infile;
	IFFP	retiffp;
	long	errret;
	UBYTE ptitle[80];
	ClientFrame mainframe;
	
	struct	Group_Attrib	*curgrpsave;
	struct	Layer_Attrib	*curlaysave;
	struct	Pen_Attrib	*curpensave;

	curgrpsave = Current_Group;
	curlaysave = Current_Layer;
	curpensave = Current_Pen;
	
	savecurrent = Current_Library;

	if (Current_Library==NULL) {
		InfoMess(LIBRARY_MODE,LIBRARY_MESS_SELLIB);
		return(FALSE);
	}

	curlib = Current_Library;

	if (!Lib_Sc("Select Library Item","  Select  ","Library Item Name",
			&curlib,FALSE)) {
		Current_Library = savecurrent;
		setlibpath();

		return(FALSE);
	}

	setlibpath();

	Current_Item = scrollanswer;
	if (Current_Item==NULL) return(FALSE);

	while (Item_Last) {			/* Clear out old lib entry */
		prvnod = Item_Last->node_previous;
		FreeMem(Item_Last, Item_Last->node_length);
		Item_Last = prvnod;
	}
		
	groupelements = Current_Item->Last_ID - Current_Item->Cur_ID + 1;

	Group_Array = (GROUP_A **) 
		rmalloc(sizeof(struct Group_Array *) * groupelements);

	if (Group_Array==NULL) {
		DispMess("Couldn't allocate the Group_Array");
		return(FALSE);
	}

	Pen_Array = B_Pen_Array;		/* Set these up for iffget */
	Layer_Array = B_Layer_Array;
	Style_Array = B_Style_Array;

	grpoffs = Current_Item->Cur_ID;
	grpmax = Current_Item->Last_ID;

	printf("Select from %d to %d\n",grpoffs,grpmax);

	for (i=grpoffs; i<=grpmax; i++) Group_Array[i-grpoffs]=Lib_Array[i];

	infile=GOpen(uselibrary,MODE_OLDFILE);
	if (infile==0) {
		errret=IoErr();
		sprintf(ptitle,"Cannot get use file - error %ld",errret);
		DispMess(ptitle);
		return(FALSE);
	}

	Item_Root = NULL;
	Item_Last = NULL;

	save_root = Root_Node;
	save_last = Last_Node;

	Root_Node = NULL;
	Last_Node = NULL;

	mainframe.getList=SkipGroup;
	mainframe.getProp=SkipGroup;

	mainframe.getForm = GetGetForms;
	mainframe.getCat=SkipGroup;

	quietupdate=TRUE;
	ifferror = IFF_OKAY;
	retiffp=ReadIFF(infile, &mainframe);
	printf("Iff status %ld\n",retiffp);
	quietupdate=FALSE;

	printf("Select from %d to %d\n",grpoffs,grpmax);

	GClose(infile);
		
	Item_Root = Root_Node;
	Item_Last = Last_Node;

	Root_Node = save_root;
	Last_Node = save_last;
	
	i = 0;
	for (prvnod=Item_Root; prvnod!=NULL; prvnod=prvnod->node_subsequent) {
		i++;
	}

	printf("%d objects loaded in\n",i);

	rmfree(Group_Array,sizeof(struct Group_Array *) * groupelements);

	Current_Group = curgrpsave;
	Current_Layer = curlaysave;
	Current_Pen = curpensave;
	
	setpath();

	UpdateTitle();	

	return(TRUE);
}

VOID ItemPoint(sx,sy,action,xr,yr)
WORD sx,sy,action;
STDFLT xr,yr;
{

	if (action==RBUTTONDOWN) {
		ExitSub(LIBRARY_MESS_ABORT);
		return;
	}

	switch (action) {
	case LBUTTONDOWN:
		DropItem(xr,yr);
		SExitSub(NULL);
		return;

	}
}

BOOL Place_Item()
{

	if (Current_Item==NULL) {
		InfoMess(LIBRARY_MODE,LIBRARY_MESS_SELITEM);
		return(FALSE);
	}

	if (!EnterSub(LIBRARY_MODE,LIBRARY_MESS_EXIT,ItemPoint)) return(FALSE);
	MainMess(LIBRARY_MESS_PUT);

	return(FALSE);
}

DropItem(xc,yc)
STDFLT xc,yc;
{

	char	grp[40],ptitle[80];
	int 	findfile,inc;
	UBYTE	*tfrin,*tfrout;
	struct	Group_Attrib	new_grp,*nen;
	struct	libdirect	*nod;
	struct 	drawing_node 	*drawout,*drawin;
	STDFLT	x,y,tcos,tsin;
	struct	Group_Attrib	*curgrpsave;
	struct	Layer_Attrib	*curlaysave;
	struct	Pen_Attrib	*curpensave;
	struct	line_mode	*linemode;
	struct	ellip_mode	*ellipmode;
	struct	text_mode	*textmode;

	if (Current_Item==NULL) return;

/*	printf("Drop the item\n");	*/

	curgrpsave = Current_Group;
	curlaysave = Current_Layer;
	curpensave = Current_Pen;
	
	tcos = cos(lib_rotate);
	tsin = sin(lib_rotate);

	sprintf(grp,"%s",Current_Item->Group_Name);
	sprintf(ptitle,"%s",grp);

	nen = Group_Level(Current_Group->descendants,ptitle);
	
	if (nen!=NULL) {
		grp[26]='\0';
		if (grp[0]=='\0') sprintf(grp,"newgroup");
		for (findfile=1; findfile<99999; findfile++) {
			sprintf(ptitle,"%s[%d]",grp,findfile);
			nen = Group_Level(Current_Group->descendants,ptitle);
			if (nen==NULL) break;
		}
	}

	sprintf(new_grp.Group_Name,"%s",ptitle);
	new_grp.Use_Count = 0;
	new_grp.Color_ID = Current_Item->Color_ID;
	new_grp.Group_ID = 0;
	new_grp.Group_Flags = Current_Item->Group_Flags;
	new_grp.Libr_Flags = Current_Item->Libr_Flags;
	
	new_grp.next = NULL;
	new_grp.previous = NULL;
	new_grp.parent = Current_Group;
	new_grp.descendants = NULL;
	new_grp.desc_last = NULL;

	new_grp.cursel = Current_Group->cursel;

	nen = groupinsert(new_grp,Current_Group,TRUE);

	grpoffs = Current_Item->Cur_ID;
	grpmax = Current_Item->Last_ID;

	Lib_Array[grpoffs]->grp_alias = nen;

	for (inc=grpoffs+1; inc<=grpmax; inc++) {

		nod = Lib_Array[inc];
		if (nod==NULL) continue;

/*		printf("Created group %s\n",nod->Group_Name);	*/

		nen = (GROUP_A *) 
			rmalloc(sizeof(GROUP_A));

		if (nen==NULL) {
			printf("Cannot allocate new groups\n");
			return;
		}

		sprintf(nen->Group_Name,"%s",nod->Group_Name);
		nen->Use_Count = 0;
		nen->Color_ID = nod->Color_ID;
		nen->Group_ID = nod->Cur_ID;
		nen->Group_Flags = nod->Group_Flags;
		nen->Libr_Flags = nod->Libr_Flags;
	
		nen->parent = NULL;
		nen->next = NULL;
		nen->previous = NULL;
		nen->descendants = NULL;
		nen->desc_last = NULL;
	
		nen->Comment = nod->Comment;

		nod->grp_alias = nen;

	}

	for (inc=grpoffs+1; inc<=grpmax; inc++) {

		nod = Lib_Array[inc];
		if (nod==NULL) continue;

		if (nod->parent)
			nod->grp_alias->parent = nod->parent->grp_alias;

		if (nod->previous)
			nod->grp_alias->previous = nod->previous->grp_alias;

		if (nod->next)
			nod->grp_alias->next = nod->next->grp_alias;

		if (nod->descendants)
		  nod->grp_alias->descendants = nod->descendants->grp_alias;

		if (nod->grp_alias->next==NULL&&nod->grp_alias->parent!=NULL)
			nod->grp_alias->parent->desc_last = nod->grp_alias;

	}

	nod = Lib_Array[grpoffs];

	if (nod->descendants) {
		nod->grp_alias->descendants = nod->descendants->grp_alias;
		printf("dsc=%d ",nod->descendants->Cur_ID);
	}

/*	if (nod->grp_alias->next==NULL&&nod->grp_alias->parent!=NULL)
		nod->grp_alias->parent = nod;	*/

/*	printf("Inserting Object\n");	*/

	for (drawin=Item_Root; drawin!=NULL; drawin=drawin->node_subsequent) {

		drawout = (struct Drawing_Node *) 
			rmalloc(drawin->node_length);

		if (drawout==NULL) {
			printf("Cannot allocate library objects\n");
			return;
		}

		tfrin = (UBYTE *) drawin;
		tfrout = (UBYTE *) drawout;

		for (inc=0; inc<drawin->node_length; inc++) { 
			*tfrout = *tfrin;
			tfrout++;
			tfrin++;
		}

		Current_Group = ((struct libdirect *)
			drawout->above_group)->grp_alias;

		Current_Layer = drawout->node_layer;
		Current_Pen = drawout->pen_attach;

		LinkIntoGroup(drawout);

		if (lib_autoclip) drawout->node_flags |= NODE_INCLIP;		

		drawingdirty = TRUE;	/* Probably dirty now */

		switch(drawout->node_id) {
		case NODE_LINE:
			linemode = GetLineData(drawout);
			if (lib_yflip) {
				linemode->y1 = - linemode->y1;
				linemode->y2 = - linemode->y2;
			}
			if (lib_xflip) {
				linemode->x1 = - linemode->x1;
				linemode->x2 = - linemode->x2;
			}
			linemode->x1 *= lib_scale;
			linemode->y1 *= lib_scale;
			linemode->x2 *= lib_scale;
			linemode->y2 *= lib_scale;
			
			x = linemode->x1;
			y = linemode->y1;
			linemode->x1 = x*tcos-y*tsin;
			linemode->y1 = x*tsin+y*tcos;

			x = linemode->x2;
			y = linemode->y2;
			linemode->x2 = x*tcos-y*tsin;
			linemode->y2 = x*tsin+y*tcos;

			linemode->x1 += xc;
			linemode->y1 += yc;
			linemode->x2 += xc;
			linemode->y2 += yc;
			break;

		case NODE_ELLIP:
			ellipmode = GetEllipData(drawout);

			ellipmode->xradius *= lib_scale;
			ellipmode->yradius *= lib_scale;
			ellipmode->x1 *= lib_scale;
			ellipmode->y1 *= lib_scale;
			if (lib_xflip) {
				ellipmode->x1 = - ellipmode->x1;

				ellipmode->rotation = PI - ellipmode->rotation;
				x = ellipmode->fromang;
				y = ellipmode->toangle;
				ellipmode->fromang = PI - y;
				ellipmode->toangle = PI - x;
			}
			if (lib_yflip) {
				ellipmode->y1 = - ellipmode->y1;

				ellipmode->rotation = TWOPI - ellipmode->rotation;
				x = ellipmode->fromang;
				y = ellipmode->toangle;
				ellipmode->fromang = TWOPI - y;
				ellipmode->toangle = TWOPI - x;
			}

			x = ellipmode->x1;
			y = ellipmode->y1;
			ellipmode->x1 = x*tcos-y*tsin;
			ellipmode->y1 = x*tsin+y*tcos;

			ellipmode->rotation += lib_rotate;
			ellipmode->fromang += lib_rotate;
			ellipmode->toangle += lib_rotate;

			normalize(ellipmode->rotation);
			normalize(ellipmode->fromang);
			normalize(ellipmode->toangle);

			ellipmode->x1 += xc;
			ellipmode->y1 += yc;
			break;

		case NODE_TEXT:
			textmode = GetTextData(drawout);

			if (lib_yflip) {
				textmode->y1 = - textmode->y1;
				if (textmode->Text_Bits&TEXT_FLIPY)
					textmode->Text_Bits&=(~TEXT_FLIPY);
				else	textmode->Text_Bits|=TEXT_FLIPY;
			}
			if (lib_xflip) {
				textmode->x1 = - textmode->x1;
				if (textmode->Text_Bits&TEXT_FLIPX)
					textmode->Text_Bits&=(~TEXT_FLIPX);
				else	textmode->Text_Bits|=TEXT_FLIPX;
			}

			x = textmode->x1;
			y = textmode->y1;
			textmode->x1 = x*tcos-y*tsin;
			textmode->y1 = x*tsin+y*tcos;

			textmode->x1 += xc;
			textmode->y1 += yc;

			textmode->ang += lib_rotate / .01745329;
			while (textmode->ang>360.0) textmode->ang -= 360.0;
			while (textmode->ang<0.0)   textmode->ang += 360.0;
			break;

		}

		GenericRender(drawout);
	}

	Current_Group = curgrpsave;
	Current_Layer = curlaysave;
	Current_Pen = curpensave;

	printf("Try setpath... ");
	setpath();
	printf(" Done.\n");
	
}

LOCAL BOOL exflag=TRUE;
LOCAL int postaction;
LOCAL struct Requester LibrEditRequester;

extern UBYTE undobuf[];

VOID LRHandler(ingad)
struct Gadget *ingad;
{
	CloseRequest(&LibrEditRequester,w);

	switch (ingad->GadgetID) {
	case 20:
		exflag=FALSE;
		postaction = 0;
		return;
	case 21:
		exflag=FALSE;
		postaction = 3;
		return;
	}

	return;

}

LOCAL UBYTE LRData[2][32];
LOCAL struct StringInfo LRStringSpec[] = {
	{ &LRData[0][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &LRData[1][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL }};


VOID LRBeepRestore()
{

	int a;

	sprintf(&LRData[0][0],"%.2f",lib_rotate / .01745329);
	sprintf(&LRData[1][0],"%.4f",lib_scale);

	for (a=0; a<2; a++) StgSet(&LRStringSpec[a]);

	DisplayBeep(screen);

	RefreshGadgets(LibrEditRequester.ReqGadget,w,&LibrEditRequester);
	return;
}

VOID LRStringHandler(ingad)
struct Gadget *ingad;
{

	int button,a;
	STDFLT inquery;

	Forbid();
	ingad->Flags &= ~SELECTED;	/* Make sure it is unselected */
	Permit();

	button = ingad->GadgetID-30;

	if (sscanf(&LRData[button][0],"%f",&inquery)!=1) {
		LRBeepRestore();
		return;
	}

	switch (button) {
	case 0:
		inquery *= .01745329;		/* Cvt from degs to rads */
		while (inquery>TWOPI) 
			inquery -= TWOPI;
		while (inquery<0.0) 
			inquery += TWOPI;
		lib_rotate = inquery;

		break;
	case 1:	
		if (inquery<=0.0) {
			LRBeepRestore();
			return;
		}
		lib_scale = inquery;
		break;
	}

	sprintf(&LRData[0][0],"%.2f",lib_rotate / .01745329);
	sprintf(&LRData[1][0],"%.4f",lib_scale);

	for (a=0; a<2; a++) StgSet(&LRStringSpec[a]);

	RefreshGadgets(LibrEditRequester.ReqGadget,w,&LibrEditRequester);
		
	return;

}

LOCAL struct IntuiText LRListText[] = {
	{ 2,3,JAM2,0,0,NULL,"  Cancel  ",NULL },
	{ 2,3,JAM2,0,0,NULL,"    OK    ",NULL },
	{ 3,2,JAM2,0,0,NULL," X-Flip ",NULL },
	{ 3,2,JAM2,0,0,NULL," Y-Flip ",NULL },
	{ 3,2,JAM2,0,0,NULL,"  Clip  ",NULL }};

#define LRACTIVE GADGIMMEDIATE | RELVERIFY
#define LRTACTIVE GADGIMMEDIATE | RELVERIFY | TOGGLESELECT

LOCAL struct Gadget LRList[] = {
	{ &LRList[1], 146,75,80,8,GADGHCOMP,LRACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&LRListText[0],0,NULL,20,LRHandler},
	{ &LRList[2], 30,75,80,8,GADGHCOMP,LRACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&LRListText[1],0,NULL,21,LRHandler},
	{ &LRList[3], 5,50,64,8,GADGHCOMP,LRTACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&LRListText[2],0,NULL,40, NULL },
	{ &LRList[4], 85,50,64,8,GADGHCOMP,LRTACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&LRListText[3],0,NULL,40, NULL },
	{ &LRList[5], 165,50,64,8,GADGHCOMP,LRTACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&LRListText[4],0,NULL,40, NULL },
	{ &LRList[6], 125, 25, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &LRStringSpec[0], 30, LRStringHandler  },
	{ NULL, 125, 35, 80, 9, GADGHCOMP, 
	  RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, STRGADGET|REQGADGET, 
	  NULL, NULL, NULL, 0, &LRStringSpec[1], 31, LRStringHandler  }};

LOCAL struct IntuiText LRHeadings[] = {
	{ 3,1,JAM2,5,4,NULL,"Set Library Insert Info",&LRHeadings[1]},
	{ 2,1,JAM2,35,25,NULL,  "Rotation:",&LRHeadings[2]},
	{ 2,1,JAM2,35,35,NULL,"   Scale:",NULL}};

Library_Settings()
{

	STDFLT	slib_rotate;
	STDFLT	slib_scale;
	BOOL	slib_yflip;
	BOOL	slib_xflip;
	BOOL	slib_autoclip;

	int a;

	slib_rotate=lib_rotate;
	slib_scale=lib_scale;
	slib_yflip=lib_yflip;
	slib_xflip=lib_xflip;
	slib_autoclip=lib_autoclip;

	exflag = TRUE;

	while (exflag) {

		InitRequester(&LibrEditRequester);

		LibrEditRequester.Width=260;
		LibrEditRequester.Height=88;
		LibrEditRequester.LeftEdge=(XSsize-LibrEditRequester.Width)/2;
		LibrEditRequester.TopEdge=(YSsize-LibrEditRequester.Height)/2;

		LibrEditRequester.BackFill=1;

		LibrEditRequester.ReqText = &LRHeadings[0];
		LibrEditRequester.ReqGadget = &LRList[0];

		LRList[2].Flags &= (~SELECTED);
		LRList[3].Flags &= (~SELECTED);
		LRList[4].Flags &= (~SELECTED);

		if (lib_xflip)
			LRList[2].Flags |= SELECTED;
		if (lib_yflip)
			LRList[3].Flags |= SELECTED;
		if (lib_autoclip)
			LRList[4].Flags |= SELECTED;

		sprintf(&LRData[0][0],"%.2f",lib_rotate / .01745329);
		sprintf(&LRData[1][0],"%.4f",lib_scale);

		for (a=0; a<2; a++) StgSet(&LRStringSpec[a]);

#ifdef _TURNSY
		LibrEditRequester.Flags = NOISYREQ;
#endif

		SafeRequest(&LibrEditRequester, w);

		postaction = 0;

		DoBorders(&LibrEditRequester,15,0,0);
		
		sit_n_spin(&LibrEditRequester,0,&LRList[0],&LRList[1]);
				/* Wait for requesters to exit */

		switch (postaction) {
		case 0:
			lib_rotate=slib_rotate;
			lib_scale=slib_scale;
			lib_yflip=slib_yflip;
			lib_xflip=slib_xflip;
			lib_autoclip=slib_autoclip;
			break;
		case 3:
			lib_xflip = FALSE;
			lib_yflip = FALSE;
			lib_autoclip = FALSE;
			if (LRList[2].Flags & SELECTED) 
				lib_xflip = TRUE;
			if (LRList[3].Flags & SELECTED) 
				lib_yflip = TRUE;
			if (LRList[4].Flags & SELECTED) 
				lib_autoclip = TRUE;
			break;
		}
				
	}

}
