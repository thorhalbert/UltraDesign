/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	dispatchtab.c - mouse/menu item dispatcher table
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
 *	1	11-Jan-87 SH	Begin History
 *	2	16-May-87 SH	Add new attribute menu items
 *	3	13-Jun-87 SH	Add scale and pallette
 *	4	22-Jun-87 SH	Rewrite to be table driven 
 *	5	 1-Aug-87 SH	Use new menu items
 *	6	 3-Sep-87 SH	A new set of menus again
 *	7	 5-Aug-88 SH	New mapping, new key map table
 *	7	24-Sep-88 SH	Split apart into dispatchtab
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*-------------------definitions for menus-menus.h--------------------------*/

struct menu_access {
	char	*funkey;
	BOOL	*access;		/* Access to bool toggle */
	BOOL	(*Implement)();		/* Execute function */
	BOOL	activate;		/* Do It? */
	UWORD	excludec;		/* Special exclude code */
	int	keytype;		/* Qualifier Mask */
	char	*keykey;		/* Letter Entered */
	struct MenuItem *menptr;
	WORD	menu,item,sub;
};


/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

BOOL Scan_True = TRUE;
BOOL FileRequest();
extern struct Screen *screen;
extern struct Window *w;

BOOL	Load_View();
BOOL	N_Save_View();
BOOL	Load_Libr();
BOOL	N_Save_Libr();
BOOL	Clip_View();
BOOL	Use_Libr();
BOOL	Root_Merge();
BOOL	Curr_Merge();
BOOL	Clear_View();
BOOL	Clear_Libr();
BOOL	N_Update();
BOOL	Quit_Out();
BOOL	Style_Current();
BOOL	Style_Add();
BOOL	Style_Rename();
BOOL	Style_Merge();
BOOL	Style_Kill();
BOOL	Style_Expunge();
BOOL	Layer_Current();
BOOL	Layer_Add();
BOOL	Layer_Rename();
BOOL	Layer_Merge();
BOOL	Layer_Kill();
BOOL	Layer_Expunge();
BOOL	Pen_Current();
BOOL	Pen_Add();
BOOL	Pen_Rename();
BOOL	Pen_Merge();
BOOL	Pen_Kill();
BOOL	Pen_Expunge();
BOOL	Group_Current();
BOOL	Group_Add();
BOOL	Group_Rename();
BOOL	Group_Merge();
BOOL	Group_Prune();
BOOL	Group_Graft();
BOOL	Group_Expunge();
BOOL	Color_Layer();
BOOL	Color_Group();
BOOL	Color_Pen();
BOOL	RepaintWindow();
BOOL	Zoom_In();
BOOL	Zoom_All();
BOOL	Zoom_Full();
BOOL	Zoom_O50();
BOOL	Zoom_O100();
BOOL	Zoom_O200();
BOOL	Margin_Current();
BOOL	PalDo();
BOOL	RestorePalette();
BOOL	TempOrigin();
BOOL	ResetOrigin();
BOOL	PermOrigin();
BOOL	Draw_Line();
BOOL	Draw_Ellipse();
BOOL	Draw_Text();
BOOL	Draw_Dims();
BOOL	Draw_Hatch();
BOOL	SeekMid();
BOOL	SeekEnd();
BOOL	SeekPerp();
BOOL	SeekCent();
BOOL	SeekTang();
BOOL	SeekText();
BOOL	DelCurrent();
BOOL	ClipCurrent();
BOOL	UnClipCurrent();
BOOL	BoxIn();
BOOL	BoxAll();
BOOL 	Box_Out();
BOOL	group_group();
BOOL	group_layer();
BOOL	group_pen();
BOOL	group_lettering();
BOOL	Lib_CurrentItem();
BOOL	Library_Settings();
BOOL	Place_Item();
BOOL	Clip_Clear();
BOOL	Clip_Kill();
BOOL	Clip_Drag();
BOOL	Clip_Clone();
BOOL	Clip_Rotate();
BOOL	Razor_Line();
BOOL	EditItem();
BOOL	Scale_Even();
BOOL	x_flip();
BOOL	y_flip();
BOOL	LineHook();
BOOL	LineUnhook();
BOOL	Load_Font();
BOOL	Ang_DegD();
BOOL	Ang_DegM();
BOOL	Ang_DegMS();
BOOL	Ang_Rad();
BOOL	Ang_Grad();
BOOL	Lin_EDI();
BOOL	Lin_EFI();
BOOL	Lin_DF();
BOOL	Lin_DI();
BOOL	Lin_MM();
BOOL	Lin_CM();
BOOL	Lin_M();
BOOL	Lin_MILE();
BOOL	Lin_KM();
BOOL	Searcher();
BOOL	Report_Menus();
BOOL	ReportStyle();
BOOL 	OpenDialogWB();
BOOL	Roll_Left();
BOOL	Roll_Right();
BOOL 	Clip_String_Open();
BOOL	Clip_String_Closed();
BOOL	Sort_Clip();
BOOL	Copy_Top();
BOOL	Copy_Bottom();

/*-------------------------Global Variable Definitions-----------------------*/

#define ALT_KEYS 	(IEQUALIFIER_LALT | IEQUALIFIER_RALT)
#define SHIFT_KEYS \
	(IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT) 
#define SHIFT_ONLY 	(IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT) 
#define CTRL_KEYS 	(IEQUALIFIER_CONTROL) 
#define ANY_ALT (ALT_KEYS | SHIFT_KEYS | CTRL_KEYS)
#define LF_AMIGA IEQUALIFIER_LCOMMAND
#define RT_AMIGA IEQUALIFIER_RCOMMAND

#define ALT_FLAG 0x01
#define UCC_FLAG 0x02
#define LCC_FLAG 0x04
#define LET_FLAG (UCC_FLAG|LCC_FLAG)
#define CTRL_FLAG 0x08
#define LF_FLAG 0x10
#define RT_FLAG 0x20

#define _ALT ALT_FLAG
#define _UC UCC_FLAG
#define _LC LCC_FLAG
#define _CTR CTRL_FLAG
#define _LFA LF_FLAG
#define _RTA RT_FLAG
#define _LET 0

struct menu_access access_tables[] = {
	{ "LVW", NULL, Load_View, 	TRUE },
	{ "SVW", NULL, N_Save_View,	TRUE },
	{ "LLB", NULL, Load_Libr,	TRUE },
	{ "SLB", NULL, N_Save_Libr,	TRUE },
	{ "UVW", NULL, N_Update,	TRUE },
	{ "ULB", NULL, N_Update,	TRUE },
	{ "MVR", NULL, Root_Merge,	TRUE },
	{ "MVC", NULL, Curr_Merge,	TRUE },
	{ "SVC", NULL, Clip_View,	TRUE },
	{ "DLB", NULL, Use_Libr,	TRUE },
	{ "CVW", NULL, Clear_View,	TRUE },
	{ "CLB", NULL, Clear_Libr,	TRUE },
	{ "QUT", NULL, Quit_Out,	TRUE },
	{ "ALS", NULL, Style_Current, 	TRUE },
	{ "ALY", NULL, Layer_Current, 	TRUE },
	{ "AGR", NULL, Group_Current, 	TRUE },
	{ "APS", NULL, Pen_Current,	TRUE },
	{ "CBL", NULL, Color_Layer,	TRUE,	0x11 },
	{ "CBG", NULL, Color_Group, 	TRUE,	0x10 },
	{ "CBP", NULL, Color_Pen,	TRUE,	0x12 },
	{ "REP", NULL, RepaintWindow,	TRUE },
	{ "ZIN", NULL, Zoom_In,		TRUE },
	{ "ZFU", NULL, Zoom_Full, 	TRUE },
	{ "Z50", NULL, Zoom_O50,	TRUE },
	{ "Z10", NULL, Zoom_O100,	TRUE },
	{ "Z20", NULL, Zoom_O200,	TRUE },
	{ "MAR", NULL, Margin_Current,	TRUE },
	{ "EPL", NULL, PalDo,		TRUE },
	{ "RPA", NULL, RestorePalette,	TRUE },
	{ "STO", NULL, TempOrigin,	TRUE },
	{ "RTO", NULL, ResetOrigin,	TRUE },
	{ "SPO", NULL, PermOrigin,	TRUE },
	{ "CLN", NULL, Draw_Line,	TRUE,	0x50 },
	{ "CEL", NULL, Draw_Ellipse,	TRUE,	0x51 },
	{ "CTX", NULL, Draw_Text, 	TRUE,	0x52 },
	{ "CDM", NULL, Draw_Dims, 	TRUE,	0x53 },
	{ "CHF", NULL, Draw_Hatch,	TRUE,	0x54 },
	{ "SKM", NULL, SeekMid,		TRUE },
	{ "SKE", NULL, SeekEnd,		TRUE },
	{ "SKP", NULL, SeekPerp,	TRUE },
	{ "SKC", NULL, SeekCent,	TRUE },
	{ "DIA", NULL, OpenDialogWB,	TRUE },
	{ "DLI", NULL, DelCurrent,	TRUE },
	{ "ATC", NULL, ClipCurrent,	TRUE },
	{ "RFC", NULL, UnClipCurrent, 	TRUE },
	{ "SLI", NULL, Searcher,	TRUE },
	{ "BXI", NULL, BoxIn,		TRUE },
	{ "BXA", NULL, BoxAll, 		TRUE },
	{ "BXO", NULL, Box_Out,		TRUE },
	{ "CCG", NULL, group_group,	TRUE },
	{ "CCL", NULL, group_layer, 	TRUE },
	{ "CCP", NULL, group_pen,	TRUE },
	{ "CCS", NULL, group_lettering, TRUE },
	{ "RAZ", NULL, Razor_Line, 	TRUE },
	{ "SOS", &Scan_True, NULL,	FALSE },
	{ "SIC", &Scan_Clip, NULL,	TRUE },
	{ "SSG", &Scan_Group, NULL,	TRUE },
	{ "SSL", &Scan_Layer, NULL,	TRUE },
	{ "SSP", &Scan_Pen, NULL, 	TRUE },
	{ "SSS", &Scan_Text, NULL, 	TRUE },
	{ "CES", NULL, Clip_String_Open,TRUE },
	{ "CEO", NULL, Clip_String_Closed,TRUE },
	{ "CSR", NULL, Sort_Clip,	TRUE },
	{ "EDT", NULL, EditItem,	TRUE },
	{ "LNH", NULL, LineHook,	TRUE },
	{ "LNU", NULL, LineUnhook,	TRUE },
	{ "SEL", NULL, Lib_CurrentItem, TRUE },
	{ "ISL", NULL, Library_Settings, TRUE },
	{ "CTF", NULL, Copy_Top,	TRUE },
	{ "CTL", NULL, Copy_Bottom,	TRUE },
	{ "PLI", NULL, Place_Item,	TRUE },
	{ "CLR", NULL, Clip_Clear,	TRUE },
	{ "KLC", NULL, Clip_Kill,	TRUE },
	{ "DRC", NULL, Clip_Drag, 	TRUE },
	{ "CLC", NULL, Clip_Clone,	TRUE },
	{ "ROC", NULL, Clip_Rotate, 	TRUE },
	{ "SCC", NULL, Scale_Even,	TRUE },
	{ "XFL", NULL, x_flip,		TRUE },
	{ "YFL", NULL, y_flip,		TRUE },
	{ "SHC", &Center_On, NULL,	TRUE },
	{ "ATS", &Handles_On, NULL,	TRUE },
	{ "COD", &Coord_On,	NULL,	TRUE },
	{ "DRI", &Draw_Info, NULL,	TRUE },
	{ "CUS", &Snaps_On,	NULL,	TRUE },
	{ "GRI", &Grid_On, NULL,	TRUE },
	{ "BNL", &Use_Bounds, NULL,	TRUE },
	{ "OTS", &Ortho_Switch, NULL, 	TRUE },
	{ "CUR", &Cursors_On, NULL,	TRUE },
	{ "QHA", &Q_Hatch, NULL,	TRUE },
	{ "DDC", NULL, Ang_DegD,	TRUE,	0x101 },
	{ "DHM", NULL, Ang_DegM,	TRUE,	0x102 },
	{ "DMS", NULL, Ang_DegMS,	TRUE,	0x103 },
	{ "RAD", NULL, Ang_Rad,		TRUE,	0x104 },
	{ "GRD", NULL, Ang_Grad,	TRUE,	0x105 },
	{ "EDC", NULL, Lin_EDI,		TRUE,	0x111 },
	{ "ENA", NULL, Lin_EFI,		TRUE,	0x112 },
	{ "ENF", NULL, Lin_DF,		TRUE,	0x113 },
	{ "ENI", NULL, Lin_DI,		TRUE,	0x114 },
	{ "MMM", NULL, Lin_MM,		TRUE,	0x115 },
	{ "MCM", NULL, Lin_CM,		TRUE,	0x116 },
	{ "MTR", NULL, Lin_M,		TRUE,	0x117 },
	{ "ENM", NULL, Lin_MILE,	TRUE,	0x118 },
	{ "MKM", NULL, Lin_KM,		TRUE,	0x119 },
	{ "RPM", NULL, Report_Menus,	TRUE },
	{ "RPS", NULL, ReportStyle,	TRUE },
	{ NULL, NULL, NULL, FALSE },
};

#define KDF(ky,mp) { ky, mp },
static struct _maps {
	char	*mc;
	UBYTE	keyno;
} key_mapping[] = {
	KDF("K0",0x0f)
	KDF("K1",0x1d)
	KDF("K2",0x1e)
	KDF("K3",0x1f)
	KDF("K4",0x2d)
	KDF("K5",0x2e)
	KDF("K6",0x2f)
	KDF("K7",0x3d)
	KDF("K8",0x3e)
	KDF("K9",0x3f)
	KDF("K.",0x3c)
	KDF("KE",0x0d)
	KDF("K-",0x4a)
	KDF("AL",0x4f)
	KDF("AR",0x4e)
	KDF("AU",0x4c)
	KDF("AD",0x4d)
	KDF("1",0x01)
	KDF("2",0x02)
	KDF("3",0x03)
	KDF("4",0x04)
	KDF("5",0x05)
	KDF("6",0x06)
	KDF("7",0x07)
	KDF("8",0x08)
	KDF("9",0x09)
	KDF("0",0x0A)
	KDF("Q",0x10)
	KDF("W",0x11)
	KDF("E",0x12)
	KDF("R",0x13)
	KDF("T",0x14)
	KDF("Y",0x15)
	KDF("U",0x16)
	KDF("I",0x17)
	KDF("O",0x18)
	KDF("P",0x19)
	KDF("A",0x20)
	KDF("S",0x21)
	KDF("D",0x22)
	KDF("F",0x23)
	KDF("G",0x24)
	KDF("H",0x25)
	KDF("J",0x26)
	KDF("K",0x27)
	KDF("L",0x28)
	KDF("Z",0x31)
	KDF("X",0x32)
	KDF("C",0x33)
	KDF("V",0x34)
	KDF("B",0x35)
	KDF("N",0x36)
	KDF("M",0x37)
	KDF("ESC",0x45)
	KDF("TAB",0x42)
	KDF("BS",0x41)
	KDF("CR",0x44)
	KDF("DEL",0x46)
	KDF("F1",0x50)
	KDF("F2",0x51)
	KDF("F3",0x52)
	KDF("F4",0x53)
	KDF("F5",0x54)
	KDF("F6",0x55)
	KDF("F7",0x56)
	KDF("F8",0x57)
	KDF("F9",0x58)
	KDF("F10",0x59)
	KDF("HELP",0x5F)
	KDF(0,0)
};

/*-------------------------------Code Section--------------------------------*/

static UBYTE rval[20];
UBYTE *RetKeyVal(inkey)
UBYTE *inkey;
{
	
	int i;
	struct menu_access *menidx;
	UBYTE *a,*b;

	rval[0] = '\0';
	
	menidx=NULL;
	for (i=0; access_tables[i].funkey!=NULL; i++) 
		if (strcmp(inkey,access_tables[i].funkey)==0)
			menidx = &access_tables[i];
			
	if (menidx==NULL) return(rval);		/* Null String */
	if (menidx->keykey==NULL) return(rval);
				
	a = rval;
	if (menidx->keytype==_CTR) {
		strcpy(a,"C");
		a = &rval[strlen(rval)];
	}
	if (menidx->keytype==_ALT) {
		strcpy(a,"A");
		a = &rval[strlen(rval)];
	}
	if (menidx->keytype==_LFA) {
		strcpy(a,"L");
		a = &rval[strlen(rval)];
	}
	if (menidx->keytype==_RTA) {
		strcpy(a,"R");
		a = &rval[strlen(rval)];
	}
	*a++ = '-';

	strcpy(a,menidx->keykey);

	return(rval);
	
}

VOID StoKeyVal(inkey,inval)
UBYTE *inkey,*inval;
{

	int i;
	struct menu_access *menidx;
	UBYTE *a,*b,ts[5],kd[20];

	menidx=NULL;
	for (i=0; access_tables[i].funkey!=NULL; i++) 
		if (strcmp(inkey,access_tables[i].funkey)==0)
			menidx = &access_tables[i];
			
	if (menidx==NULL) return;

	menidx->keytype = 0;
	menidx->keykey = 0;
	
	if (inval[0]=='\0') return;	/* No Action */

	for (a=inval,b=ts; *a!='-'; a++,b++) *b = *a;
	*b++ = '\0';
	
	a++;
	for (b=kd; *a!='\0'; a++,b++) *b = *a;
	*b++ = '\0';
	
	if (kd[0]=='\0') return;
	
	if (ts[0]=='\0') menidx->keytype = _LET;
	else for (a=ts; *a!='\0'; a++) 
		switch (*a) {
		case 'C': menidx->keytype = _CTR; break;
		case 'A': menidx->keytype = _ALT; break;
		case 'L': menidx->keytype = _LFA; break;
		case 'R': menidx->keytype = _RTA; break;
		}
		
	menidx->keykey = GI_RemAlloc(strlen(kd)+1);
	if (menidx->keykey==NULL) return;
	strcpy(menidx->keykey,kd);
			
}				

VOID 	proclop(imenu,intex,menu,item,sub)
struct MenuItem *imenu;
struct IntuiText *intex;
int menu,item,sub;
{

	int i,j;
	UBYTE *a,*b,*c,fnam[4],ftxt[20];
	struct menu_access *menidx;

	a = intex->IText;
	b = rindex(a, '$');	/* Look for tag */
	if (b==NULL) return;	/* This is not functional */

	for (c=b-3,i=0; c<b; c++,i++) fnam[i] = *c;
	fnam[3]= '\0';
	
	for (c=b-4; c<=b; c++) *c = ' ';	/* Blank out key */
	
	menidx=NULL;
	for (i=0; access_tables[i].funkey!=NULL; i++) 
		if (strcmp(fnam,access_tables[i].funkey)==0)
			menidx = &access_tables[i];
		
	imenu->Flags &= ~(ITEMENABLED | CHECKED);
	if (menidx==NULL) {
		printf("No dispatch for %s\n",fnam);
		return;	/* No matching function */
	}
	
	if (menidx->menptr) printf("There was already a dispatch for %s\n",fnam);
	menidx->menptr = imenu;		/* Back Pointer */
	if (menidx->activate) imenu->Flags |= ITEMENABLED;
	
	menidx->menu = menu;
	menidx->item = item;
	menidx->sub = sub;
	
	if (menidx->keykey!=NULL) {
		a = ftxt;
		if (menidx->keytype==CTRL_FLAG) {
			strcpy(a,"^");
			a = &ftxt[strlen(ftxt)];
		}
		if (menidx->keytype==ALT_FLAG) {
			strcpy(a,"A-");
			a = &ftxt[strlen(ftxt)];
		}
		if (menidx->keytype==_LFA) {
			strcpy(a,"L-");
			a = &rval[strlen(rval)];
		}
		if (menidx->keytype==_RTA) {
			strcpy(a,"R-");
			a = &rval[strlen(rval)];
		}
		strcpy(a,menidx->keykey);
		for (a=b,i=strlen(ftxt)-1; i>=0; i--,a--)
			*a = ftxt[i];
	}
	
	if (menidx->access!=NULL&&menidx->menptr!=NULL) {
		menidx->menptr->MutualExclude = 0;
		menidx->menptr->Flags |= CHECKIT;
		if (*(menidx->access)) 
			menidx->menptr->Flags |= CHECKED;
	}
}

prtitem(inmen,file,prm)
UBYTE *prm;
FILE *file;
struct MenuItem *inmen;
{

	UBYTE *fac = "   ";
	struct IntuiText *it;
	int i;
		
	for (i=0; access_tables[i].funkey!=NULL; i++) 
		if (access_tables[i].menptr==inmen) fac = 
			access_tables[i].funkey; 

	it = inmen->ItemFill;
	fprintf(file,"%s %s %s\n",fac,prm,it->IText);
}
	

BOOL Report_Menus()
{

	int a;
	struct menu_access *menidx;
	struct Menu *lop1;
	struct MenuItem *lop2,*lop3;
	int menu,item,sub;
	FILE *ofil;
	
	ofil = fopen("prt:","w");	
	if (ofil==NULL) {
		complain(NULL,"Recover",4,0,0,0,
			"Cannot open print device (prt:)",
			NULL,NULL,NULL);
		return(FALSE);
	}
	
	fprintf(ofil,"%s Pull-Down Menu Abbreviation Assignments\n\n",
		ucadname);
	
	for (lop1= &Titles[0],menu=0; lop1!=NULL; lop1=lop1->NextMenu,menu++) {
	  fprintf(ofil,"    --- %s\n",lop1->MenuName);
	  for (lop2=lop1->FirstItem,item=0; lop2!=NULL; lop2=lop2->NextItem,item++) {
	    prtitem(lop2,ofil,"------");
	    for (lop3=lop2->SubItem,sub=0; lop3!=NULL; lop3=lop3->NextItem,sub++) {
	       prtitem(lop3,ofil,"---------");
	    }
	  }
	}
	
	fprintf(ofil,"\f");
	fclose(ofil);
	return(TRUE);
}	

VOID	Init_Tables()
{

	int a;
	struct menu_access *menidx;
	struct Menu *lop1;
	struct MenuItem *lop2,*lop3;
	int menu,item,sub;
	int left1;
	
	for (lop1= &Titles[0],menu=0; lop1!=NULL; lop1=lop1->NextMenu,menu++) {
	  if (lop1->LeftEdge<3) lop1->LeftEdge = 3;
	  left1 = lop1->LeftEdge;
	  for (lop2=lop1->FirstItem,item=0; lop2!=NULL; lop2=lop2->NextItem,item++) {
	    lop2->LeftEdge = 0;
	    proclop(lop2,lop2->ItemFill,menu,item,NOSUB);
	    for (lop3=lop2->SubItem,sub=0; lop3!=NULL; lop3=lop3->NextItem,sub++) {
	       lop3->LeftEdge = 50;
	       proclop(lop3,lop3->ItemFill,menu,item,sub);
	    }
	  }
	}
	
	for (a=0; access_tables[a].funkey!=NULL; a++)
		if (access_tables[a].menptr==NULL) 
			printf("No menu entry for dispatch %s\n",
				access_tables[a].funkey);
	
	SetMenuStrip(w,&Titles[0]);

}


VOID	mensetup(setcode)
int setcode;
{

	int a,setc;
	struct menu_access *menidx;

	if (setcode==0) setcode = 0xFFFF;	/* Extra special case */
	setc = setcode>>4;

	for (a=0; access_tables[a].funkey!=NULL; a++) {
		menidx = &access_tables[a];
					
		if (menidx->excludec>0) menidx->menptr->Flags |= CHECKIT;

		if (menidx->excludec==setcode) {
			menidx->menptr->Flags |= CHECKED;
			continue;
		} else 	if ((menidx->excludec>>4)==setc) {
			menidx->menptr->Flags &= ~CHECKED;
			continue;
		}

		if (menidx->access!=NULL&&menidx->menptr!=NULL) {
			menidx->menptr->Flags |= CHECKIT;
			menidx->menptr->Flags &= ~CHECKED;
			if (*(menidx->access)) 
				menidx->menptr->Flags |= CHECKED;
		}

	}

}

fullmenu(incode)
int incode;
{	

	ClearMenuStrip(w);

	mensetup(incode);

	SetMenuStrip(w,&Titles[0]);
}

VOID Perform_Menu(menidx)
struct menu_access *menidx;
{	

	BOOL result;

	RidSpurious();

	ClearMenuStrip(w);

	if (!menidx->activate) {

		DisplayBeep(screen);
		InfoMess(DISPATCH_MODE,DISPATCH_UNDERCONST);
			
		return;
	}

	if (menidx->access!=NULL&&menidx->menptr!=NULL) {
		*(menidx->access) = 
			(*menidx->access) ? FALSE:TRUE;

		menidx->menptr->Flags &= ~CHECKED;
		if (*(menidx->access)) 
			menidx->menptr->Flags |= CHECKED;
	}

	if (menidx->excludec!=0) mensetup(menidx->excludec);

	if (menidx->Implement!=NULL) {
		result=(*menidx->Implement)();
	}

	UpdateTitle();
	SetMenuStrip(w,&Titles[0]);
}

VOID	MenuActivated(incode)
int incode;
{

	int menu,item,sub;
	int a;
	struct menu_access *menidx;

	if (incode==MENUNULL) return;

	menu = MENUNUM(incode);
	item = ITEMNUM(incode);
	sub = SUBNUM(incode);

/*	printf("menu=%d, item=%d, sub=%d\n",menu,item,sub); */

	for (a=0; access_tables[a].funkey!=NULL; a++) {
		menidx = &access_tables[a];
					
		if (menu!=menidx->menu) continue;
		if (item!=menidx->item) continue;
		if (sub!=menidx->sub) continue;

		Perform_Menu(menidx);
	}
}

VOID	KeyStroke(code,qual)
int code,qual;
{

	int a,b,p= -2;
	BOOL go;
	struct menu_access *menidx;
	int qtype;

	if (code>=0x60) return;

	RidSpurious();

	switch(code){
	case 0x4F:	Roll_Left();
			return;
	case 0x4E:	Roll_Right();
			return;
	case 0x3C:	p = -1;	break;
	case 0x0F:	p = 0;	break;
	case 0x1D:	p = 1;	break;
	case 0x1E:	p = 2;	break;
	case 0x1F:	p = 3;	break;
	case 0x2D:	p = 4;	break;
	case 0x2E:	p = 5;	break;
	case 0x2F:	p = 6;	break;
	case 0x3D:	p = 7;	break;
	case 0x3E:	p = 8;	break;
	case 0x3F:	p = 9;	break;

	}
	if (!(qual&(LF_AMIGA|RT_AMIGA))) p= -2;

	if (p> -2) {
		if ((qual&CTRL_KEYS&&qual&LF_AMIGA)&&p>=0) {
			xvbeg[p] = xviewbegin;
			yvbeg[p] = yviewbegin;
			xvsiz[p] = xviewsize;
			yvsiz[p] = yviewsize;
			vset[p] = TRUE;
			return;
		}

		if ((qual&LF_AMIGA)&&p>=0) {
			if (!vset[p]) {
				InfoMess(DISPATCH_MODE,DISPATCH_NOVIEW);
				return;
			}
			xviewbegin = xvbeg[p];
			yviewbegin = yvbeg[p];
			xviewsize = xvsiz[p];
			yviewsize = yvsiz[p];
			RepaintWindow();
			return;
		}

		if ((qual&CTRL_KEYS&&qual&RT_AMIGA)&&(p!=-1)) {
			keyx[p]=dotx;
			keyy[p]=doty;
			keyset[p]=TRUE;
			return;
		}

		if (qual&RT_AMIGA) {
			if (p== -1) {
				if (!dotset) return;
				PointerAction(0,0,LBUTTONDOWN,dotx,doty);
				return;
			} else {
				if (!keyset[p]) return;
				PointerAction(0,0,LBUTTONDOWN,keyx[p],keyy[p]);
				return;
			}
		}
		return;
	}

	qual &= ANY_ALT;

	qtype = LCC_FLAG;
	if (qual&SHIFT_KEYS) qtype = UCC_FLAG;
	if (qual&ALT_KEYS) qtype = ALT_FLAG;
	if (qual&LF_AMIGA) qtype = LF_FLAG;
	if (qual&RT_AMIGA) qtype = RT_FLAG;
	if (qual&CTRL_KEYS) qtype = CTRL_FLAG;

	for (b=0; key_mapping[b].mc!=NULL; b++)
		if (key_mapping[b].keyno==code) break;

	if (key_mapping[b].mc!=NULL) {

		for (a=0; access_tables[a].funkey!=NULL; a++) {
			menidx = &access_tables[a];

			if (menidx->keykey==NULL) continue;
			if (strcmp(key_mapping[b].mc,menidx->keykey)!=0) continue;
			/* printf("Matched a %s (%d)\n",
				key_mapping[b].mc,qtype); */

			go = FALSE;
			
			if ((menidx->keytype==0)&&(qtype&LET_FLAG)) go=TRUE;
			if (menidx->keytype==qtype) go=TRUE;

			if (go) {
				/* printf("Matched\n"); */
				Perform_Menu(menidx);
				return;
			}
		}
	}

	InfoMess(DISPATCH_MODE,DISPATCH_ILLEGAL);
	printf("What was key %x qual %x\n",code,qual);

}
