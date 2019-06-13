/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	scroller.c - Main scrolling requester
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
 *	1	11-May-87 SH	Generate new program from scratch
 *	2	13-Jun-87 SH	Add layer requesters
 *	3	28-Jun-87 SH	Layout tree requester code
 *	4	12-Sep-87 SH	Add Instant Response to prop slider
 *	5 	13-Sep-87 SH	Break attributes.c into little pieces
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "plotsetup.h"
#include <ctype.h>

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

BOOL match();
VOID PBoxInput(),MBoxInput(),TBoxInput(),SelHandle(),OKHandler(),SlideHandle();
VOID Link_Style();
VOID Link_Layer();

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

#define SCR_PARENT 0x0001	/* Entry accesses parent */
#define SCR_SUBDIR 0x0002	/* Subdirectory name block */
#define SCR_HEADER 0x0004	/* Header entry - not selectable */
#define SCR_SELENT 0x0010	/* Selectable entry */

#define GBASE 42
#define NUMFLDS 8
#define BOTM (GBASE+(NUMFLDS*10))
LOCAL short Bord1[] = { 0,0, 500,0, 500,2, 0,2, 
	0,0, 500-1,0, 500-1,BOTM+37, 0,BOTM+37, 0,0 };
LOCAL short Bord2[] = 
	{ 16,40, 16,28, 497,28, 497,40, 16,40, 16,BOTM, 497,BOTM, 497,40 };
LOCAL struct Border BorderArray[] = {
 	{ 0,14, 0,0,JAM1, 9, &Bord1[0], &BorderArray[1] },
	{ 0,0,  0,0,JAM1, 8, &Bord2[0], NULL }};

LOCAL struct StringInfo PathStringSpec = 
	{ NULL, undobuf, 1, 100,0,0,0,0,0,0,NULL,0,NULL };
LOCAL struct Gadget PathBoxGadget = { NULL, 55, 18, 340, 9, GADGHCOMP, 
	RELVERIFY|GADGIMMEDIATE, STRGADGET|REQGADGET, NULL, NULL,
	NULL, 0, &PathStringSpec, 0, PBoxInput };

LOCAL struct StringInfo MaskStringSpec = 
	{ NULL, undobuf, 1, MASKMAX,0,0,0,0,0,0,NULL,0,NULL };
LOCAL struct Gadget MaskBoxGadget = { &PathBoxGadget, 300, 4, 180, 9, 
	GADGHCOMP, RELVERIFY|GADGIMMEDIATE, STRGADGET|REQGADGET, NULL, NULL,
	NULL, 0, &MaskStringSpec, 0, MBoxInput };

LOCAL struct StringInfo BoxStringSpec = 
	{ ArgText, undobuf, 1, 100,0,0,0,0,0,0,NULL,0,NULL };
LOCAL struct Gadget TextBoxGadget = { &MaskBoxGadget, 55, BOTM+8, 340, 9, 
	GADGHCOMP, RELVERIFY|GADGIMMEDIATE, STRGADGET|REQGADGET, NULL, NULL,
	NULL, 0, &BoxStringSpec, 0, TBoxInput };

LOCAL struct Requester ScrollerRequester;
LOCAL struct IntuiText TitleTexts[] = {
	{ 3,1,JAM2,5,4,NULL,NULL,	&TitleTexts[1] },
	{ 2,1,JAM2,18,30,NULL,NULL,	&TitleTexts[2] },
	{ 2,1,JAM2,5,18,NULL," Path:",	&TitleTexts[3] },
	{ 2,1,JAM2,5,170,NULL,"Entry:",	&TitleTexts[4] },
	{ 2,1,JAM2,255,4,NULL,"Mask:",	NULL },
	{ 2,1,JAM2,55,18,NULL, NULL, NULL }};		/* Path display */

LOCAL APTR accessdata[13];
LOCAL WORD accessflags[13];
LOCAL struct IntuiText OKListText[2] = {
	{ 2,3,JAM2,0,0,NULL,NULL,NULL },
	{ 2,3,JAM2,0,0,NULL,NULL,NULL }};
#define OKACTIVE GADGIMMEDIATE | RELVERIFY
LOCAL struct Gadget OKList[] = {
	{ &OKList[1], 320,BOTM+23,80,8,GADGHCOMP,OKACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&OKListText[0],0,NULL,20,OKHandler},
	{ &TextBoxGadget, 100,BOTM+23,80,8,GADGHCOMP,OKACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&OKListText[1],0,NULL,21,OKHandler}};

LOCAL struct Image SlideImage;

LOCAL struct PropInfo	Slider =
{ AUTOKNOB | FREEVERT,0,0,0xFFFF, 0xFFFF,0,0,0,0,0,0};

LOCAL struct Gadget	SlideProp =
{ &OKList[0],0,40,16,NUMFLDS*10,GADGIMAGE,RELVERIFY|GADGIMMEDIATE,PROPGADGET,
	(APTR) &SlideImage,NULL,NULL,0,(APTR) &Slider,0,SlideHandle }; 

#define DFL 58
LOCAL UBYTE DataFields[NUMFLDS][DFL+1];
LOCAL struct IntuiText ScrollList[] = {
	{ 0,1,JAM2,0,0,NULL,&DataFields[0][0],NULL },
	{ 0,1,JAM2,0,0,NULL,&DataFields[1][0],NULL },
	{ 0,1,JAM2,0,0,NULL,&DataFields[2][0],NULL },
	{ 0,1,JAM2,0,0,NULL,&DataFields[3][0],NULL },
	{ 0,1,JAM2,0,0,NULL,&DataFields[4][0],NULL },
	{ 0,1,JAM2,0,0,NULL,&DataFields[5][0],NULL },
	{ 0,1,JAM2,0,0,NULL,&DataFields[6][0],NULL },
	{ 0,1,JAM2,0,0,NULL,&DataFields[7][0],NULL }};
/*	{ 0,1,JAM2,0,0,NULL,&DataFields[8][0],NULL },
	{ 0,1,JAM2,0,0,NULL,&DataFields[9][0],NULL },
	{ 0,1,JAM2,0,0,NULL,&DataFields[10][0],NULL },
	{ 0,1,JAM2,0,0,NULL,&DataFields[11][0],NULL }}; */
#define GADACTIVE GADGIMMEDIATE | RELVERIFY
#define GWID 464
#define GLF 18
LOCAL struct Gadget GadList[] = {
	{ &GadList[1],GLF,GBASE,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[0],0,NULL,20,SelHandle},
	{ &GadList[2],GLF,GBASE+10,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[1],0,NULL,21,SelHandle},
	{ &GadList[3],GLF,GBASE+20,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[2],0,NULL,22,SelHandle},
	{ &GadList[4],GLF,GBASE+30,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[3],0,NULL,23,SelHandle},
	{ &GadList[5],GLF,GBASE+40,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[4],0,NULL,24,SelHandle},
	{ &GadList[6],GLF,GBASE+50,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[5],0,NULL,25,SelHandle},
	{ &GadList[7],GLF,GBASE+60,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[6],0,NULL,26,SelHandle},
	{ &SlideProp,GLF,GBASE+70,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[7],0,NULL,27,SelHandle}};
/*	{ &GadList[9],GLF,GBASE+80,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[8],0,NULL,28,SelHandle},
	{ &GadList[10],GLF,GBASE+90,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[9],0,NULL,29,SelHandle},
	{ &GadList[11],GLF,GBASE+100,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[10],0,NULL,30,SelHandle},
	{ &SlideProp,GLF,GBASE+110,GWID,8,GADGHCOMP,GADACTIVE,BOOLGADGET|REQGADGET,
		NULL,NULL,&ScrollList[11],0,NULL,31,SelHandle}};	*/

/*-------------------------------Code Section--------------------------------*/

LOCAL VOID (*get_formname)(),(*get_formdata)();
LOCAL APTR 	(*get_retfirst)(),(*get_retnext)(),(*get_retprev)(),
		(*get_srchrec)(),(*get_getparent)(),(*get_godescend)();
APTR scrollanswer=NULL,cur_entry;
LOCAL ULONG KepSecs,KepMics;
LOCAL int KepButton;
LOCAL BOOL scrollokcancel;

VOID StgSet(instr)
struct StringInfo *instr;
{

	Forbid();
	instr->DispPos = 0;
	instr->NumChars = strlen(instr->Buffer);

	Permit();
}

advstr()
{
	int a,b;

	for(a=0; a<NUMFLDS-1; a++) {
		accessdata[a]=accessdata[a+1];
		accessflags[a]=accessflags[a+1];
		ScrollList[a].FrontPen = ScrollList[a+1].FrontPen;
		ScrollList[a].BackPen = ScrollList[a+1].BackPen;
		sprintf(&DataFields[a][0],"%s",&DataFields[a+1][0]);
	}
	GadList[NUMFLDS-1].UserData=SelHandle;
	GadList[NUMFLDS-1].GadgetID=20+NUMFLDS-1;
	accessdata[NUMFLDS-1]=NULL;
	accessflags[NUMFLDS-1]=NULL;
	for (b=0; b<DFL; b++) DataFields[NUMFLDS-1][b]=' ';
	DataFields[NUMFLDS-1][DFL]='\0';
	ScrollList[NUMFLDS-1].FrontPen=0;
	ScrollList[NUMFLDS-1].BackPen=1;

}

int osp=-1;
	
UpdatePaint(sp)
int sp;
{

	APTR begptr,othptr,tptr;
	int a,b,dlin,plin,tbody,in;
	STDFLT r;

	in = sp;
	if (in<0) osp= -1;
	for (a=0; a<NUMFLDS; a++) {
		GadList[a].UserData=SelHandle;
		GadList[a].GadgetID=20+a;
		accessdata[a]=NULL;
		accessflags[a]=NULL;
		for (b=0; b<DFL; b++) DataFields[a][b]=' ';
		DataFields[a][DFL]='\0';
		ScrollList[a].FrontPen=0;
		ScrollList[a].BackPen=1;
	}

	if (cur_entry==NULL) 
		cur_entry=(*get_retfirst)(NULL,MaskStringSpec.Buffer,FALSE);	

	tbody = 0;
	begptr = (*get_retfirst)(NULL,MaskStringSpec.Buffer,FALSE);	
	for (othptr=begptr; othptr!=NULL; 
		   othptr=(*get_retnext)(othptr,MaskStringSpec.Buffer)) tbody++;

	tptr=(*get_getparent)(begptr,MaskStringSpec.Buffer,FALSE);
	if (tptr!=NULL) tbody ++;

	if (sp>=0) {
		a = tbody - NUMFLDS;
		if (a<1) a=1;
		sp = (((Slider.VertPot+1) * a) >> 16) + NUMFLDS;
		if (sp==tbody-1) sp=tbody;
		if (sp>tbody) sp = tbody;
		if (sp<NUMFLDS) sp=NUMFLDS;
		/* if (osp==sp) return; */
		osp = sp;
	}

	plin = 9999;
	dlin = 0;

	a = 0;
	tptr=(*get_getparent)(begptr,MaskStringSpec.Buffer,FALSE);
	if (tptr!=NULL) {
		accessdata[a]=tptr;
		accessflags[a]|=SCR_PARENT;
		ScrollList[a].FrontPen=3;
		(*get_formdata)(tptr,&DataFields[a][0]);
		for (b=strlen(&DataFields[a][0]); b<DFL; b++) 
				DataFields[a][b]=' ';
		DataFields[a][DFL]='\0';
		dlin++;
		plin--;
		a++;
	}
	for (othptr=begptr; othptr!=NULL; 
		   othptr=(*get_retnext)(othptr,MaskStringSpec.Buffer)) {

		if (a>=NUMFLDS) {
			advstr();
			a = NUMFLDS-1;
		}

		if (sp<0&&othptr==cur_entry) plin = NUMFLDS/2;

		accessdata[a]=othptr;
		(*get_formdata)(othptr,&DataFields[a][0]);
		for (b=strlen(&DataFields[a][0]); b<DFL; b++) 
				DataFields[a][b]=' ';
		DataFields[a][DFL]='\0';
		if ((*get_godescend)
			(othptr,MaskStringSpec.Buffer,FALSE)!=NULL) {
			ScrollList[a].FrontPen=3;
			accessflags[a]|=SCR_SUBDIR;
		} else 	accessflags[a]|=SCR_SELENT;
		dlin++;
		plin--;
		a++;
		if (sp>=0&&dlin>=sp) break;
		if (plin<0&&dlin>=NUMFLDS) break;
	}

	a = dlin - NUMFLDS;
	if (a<0) a=0;
	b = tbody - NUMFLDS;
	if (b<1) b=1;

	r = ((STDFLT) a) / ((STDFLT) b);
	r *= ((STDFLT) MAXPOT);

	a = r;
	if (dlin>=tbody) a = MAXPOT;
	if (a>MAXPOT) a=MAXPOT;
	if (a<0) a=0;

	if (dlin>NUMFLDS) dlin=NUMFLDS;
	if (dlin>tbody) dlin=tbody;
	if (tbody<NUMFLDS) tbody=dlin = 1;

	r = ((STDFLT) dlin) / ((STDFLT) tbody);
	r *= ((STDFLT) MAXBODY);

	b = r;
	if (b>MAXBODY) b = MAXBODY;
	if (b<0) b=0;

	if (in<0) 
		ModifyProp(&SlideProp,w,&ScrollerRequester,
			AUTOKNOB | FREEVERT, 0, a, 0, b);

	RefreshGadgets(&GadList[0],w,&ScrollerRequester);
}

VOID SelHandle(ingad)
struct Gadget *ingad;
{

	APTR ptrnam;
	int gadnum;

	if (ingad==NULL) return;

	gadnum = ingad->GadgetID-20;

	if (gadnum<0||gadnum>11) return;
	ptrnam = accessdata[gadnum];
	if (ptrnam==NULL) return;
	scrollanswer = ptrnam;
	if (accessflags[gadnum]&SCR_PARENT) {
		(*get_getparent)(ptrnam,MaskStringSpec.Buffer,TRUE);
		cur_entry = NULL;
		ArgText[0]='\0';			/* Blank out */
		StgSet(&BoxStringSpec);
		UpdatePaint(-1);
		return;
	}
	if (accessflags[gadnum]&SCR_SUBDIR) {
		(*get_godescend)(ptrnam,MaskStringSpec.Buffer,TRUE);
		cur_entry = NULL;
		ArgText[0]='\0';			/* Blank out */
		StgSet(&BoxStringSpec);
		UpdatePaint(-1);
		return;
	}
	(*get_formname)(ptrnam,ArgText);
	StgSet(&BoxStringSpec);
	RefreshGadgets(ingad,w,&ScrollerRequester);
	if (KepButton==gadnum) {
		if (DoubleClick(KepSecs,KepMics,StoSecs,StoMics)) {
			EndRequest(&ScrollerRequester,w);
			scrollokcancel = TRUE;
		}
	} 
	KepButton = gadnum;
	KepSecs = StoSecs;
	KepMics = StoMics;

}
	
VOID TBoxInput(ingad)
struct Gadget *ingad;
{

	return;

}

VOID SlideHandle(ingad)
struct Gadget *ingad;
{
	UpdatePaint(1);
	return;

}

VOID MBoxInput(ingad)
struct Gadget *ingad;
{

	if (ingad==NULL) return;

	cur_entry = NULL;
	ArgText[0]='\0';			/* Blank out */
	StgSet(&BoxStringSpec);
	UpdatePaint(-1);
	return;

}

VOID PBoxInput(ingad)
struct Gadget *ingad;
{

	if (ingad==NULL) return;

	(*get_retfirst)(NULL,MaskStringSpec.Buffer,TRUE);
	cur_entry = NULL;
	ArgText[0]='\0';			/* Blank out */
	StgSet(&BoxStringSpec);
	UpdatePaint(-1);
	return;

}

	
VOID OKHandler(ingad)
struct Gadget *ingad;
{

	if (ingad==NULL) return;

	EndRequest(&ScrollerRequester,w);	/* Do manually */
	
	switch (ingad->GadgetID) {
	case 20:
		scrollokcancel = FALSE;
		break;
	case 21:
		scrollokcancel = TRUE;
		break;
	}

	return;

}

VOID CleanOutArgText()		/* Need to mod for extended ascii */
{

	UBYTE *inpass,*outpass;

	inpass = ArgText;
	outpass = ArgText;

	for (; *inpass!='\0'; inpass++) {
		if (iscntrl(*inpass)||isspace(*inpass)) continue;
		if (*inpass=='/'||*inpass=='\\'||
		    *inpass=='*'||*inpass=='?'||*inpass==':') continue;
		*outpass++=*inpass;
	}

	*outpass='\0';

	StgSet(&BoxStringSpec);
}

BOOL Scroller(retfirst,retnext,retprev,srchrec,getparent,godescend,formdata,
	formname,pathflag,maskflag,mustbenew,defaultitem,affirm,cancel,
	title,header,startp,inmask,inpath)
APTR (*retfirst)();	/* Routine to get root of current list */
APTR (*retnext)();	/* Return successor item */
APTR (*retprev)();	/* Return predecessor item */
APTR (*srchrec)();	/* Search table for given key */
APTR (*getparent)();	/* Get parent node for any level */
APTR (*godescend)();	/* If a branch, go to top of descendant node */
VOID (*formdata)();	/* Convert a given pointer to display format */
VOID (*formname)();	/* Get the name part only */
BOOL pathflag;		/* Can the path be changed */
BOOL maskflag;		/* Is a mask allowed? */
BOOL mustbenew;		/* Old items cannot be accessed */
APTR defaultitem;	/* Begin item */
UBYTE *affirm;		/* Affirmation icon string */
UBYTE *cancel;		/* Negative icon string */
UBYTE *title;		/* Title bar information */
UBYTE *header;		/* Header bar information */
APTR *startp;		/* Context location */
UBYTE *inmask;		/* Incoming mask */
UBYTE *inpath;		/* Incoming path spec */
{

	int a,b;

	printf("Requester Entered\n");

	get_retfirst = retfirst;
	get_retnext = retnext;
	get_retprev = retprev;
	get_srchrec = srchrec;
	get_getparent = getparent;
	get_godescend = godescend;
	get_formdata = formdata;
	get_formname = formname;

	InitRequester(&ScrollerRequester);

	printf("Requester inited\n");

	for (a=0; a<NUMFLDS; a++) {
		GadList[a].UserData=SelHandle;
		GadList[a].GadgetID=20+a;
		accessdata[a]=NULL;
		accessflags[a]=NULL;
		for (b=0; b<DFL; b++) DataFields[a][b]=' ';
		DataFields[a][DFL]='\0';
		ScrollList[a].FrontPen=0;
		ScrollList[a].BackPen=1;
	}

	ScrollerRequester.Width=500;
	ScrollerRequester.Height=BOTM+38;
	ScrollerRequester.LeftEdge=(XSsize-ScrollerRequester.Width)/2;
	ScrollerRequester.TopEdge=(YSsize-ScrollerRequester.Height)/2;

	ScrollerRequester.BackFill=1;

	ScrollerRequester.Flags = NOISYREQ;

	ScrollerRequester.ReqGadget = &GadList[0];
	ScrollerRequester.ReqBorder = &BorderArray[0];
	TitleTexts[0].IText = title;
	TitleTexts[1].IText = header;
	ScrollerRequester.ReqText = &TitleTexts[0];

	MaskStringSpec.Buffer = inmask;
	StgSet(&MaskStringSpec);
	PathStringSpec.Buffer = inpath;
	StgSet(&PathStringSpec);

	MaskBoxGadget.NextGadget = &PathBoxGadget;
	TitleTexts[4].NextText = NULL;

	if (!pathflag) {
		MaskBoxGadget.NextGadget = NULL;
		TitleTexts[4].NextText = &TitleTexts[5];
		TitleTexts[5].IText = inpath;
	}

	OKListText[0].IText = cancel;
	OKListText[1].IText = affirm;

	cur_entry = *startp;
	if (cur_entry==NULL) cur_entry=(*get_retfirst)(NULL,inmask,FALSE);	

	scrollanswer = NULL;

	ArgText[0]='\0';			/* Blank out */
	if (defaultitem!=NULL) {		/* Set to default, if any */
		(*get_formname)(defaultitem,ArgText);
		scrollanswer = defaultitem;
	}

	StgSet(&BoxStringSpec);

	printf("Requester Set up\n");

	SafeRequest(&ScrollerRequester, w);

	printf("Requested\n");

	UpdatePaint(-1);

	sit_n_spin(&ScrollerRequester,1,&OKList[0],&OKList[1]);		/* Wait for requesters to exit */

	if (cur_entry!=NULL) *startp = cur_entry;

	CleanOutArgText();
	scrollanswer = (*get_srchrec)(ArgText);
	if (scrollanswer!=NULL) *startp = scrollanswer;

	if (ArgText[0]=='\0') return(FALSE);

	if (mustbenew&&scrollanswer!=NULL) {
		/* DispMess("Entry cannot exist already!"); */
		return(FALSE);
	}

	return(scrollokcancel);

}

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
