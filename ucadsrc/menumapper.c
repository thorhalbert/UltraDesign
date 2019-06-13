/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	menumapper.c - mouse/menu item dispatcher table
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
 *	8	24-Sep-88 SH	Split apart into dispatchtab
 *	9	25-Mar-89 SH	Rewrite, split tables off to .h's
 *				 build menu from file and create
 *				 binding tables
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include <ctype.h>

/*-------------------definitions for menus-menus.h--------------------------*/

/*---Amiga rawkey to reality keymap---*/

#define KDF(ky,ac,mp) { ky, ac, mp },		/* Quick macro for tables */

struct _maps {
	char	*mc,*ac;
	UBYTE	keyno;
};

/*---Internal function access table---*/

struct internal_tab {
	char	*funkey;
	BOOL	*access;		/* Access to bool toggle */
	BOOL	(*Implement)();		/* Execute function */
	BOOL	activate;		/* Do It? */
	UWORD	excludec;		/* Special exclude code */
	struct	menu_items  *menptr;	/* Pointer to menu if any */
};

/*---Amiga Pull down menu tables---*/

struct Menu *Menu_Root=NULL,*Menu_Tail=NULL;

int menstart = 0;	/* Coordinate for starting */
int levelstart = 0;	/* Start line of menu items */
int sublevel = 0;	/* Level of line in submenu */

struct menu_items {

	struct	MenuItem	menunode;	/* The menu item */
	struct	IntuiText	textnode;	/* The text for the menu */
	UBYTE	orglength;			/* Original text size */
	UBYTE	curlen;				/* Alloc'd length now */
	struct	key_bindings	*thisbind;	/* Is this bound? */
	struct	internal_tab	*intcmd;	/* Pointer to internal tab */
	UBYTE	*dialogcommand;			/* Dialog mode */
	struct	menu_items	*next;		/* Next subitem link */
		
} *MenuItem_Root=NULL,*MenuItem_Tail=NULL;

struct MenuItem *SubItem_Root=NULL,*SubItem_Tail=NULL,
		*Cur_Root=NULL,*Cur_Tail=NULL;

/*---Ucad Key bindings table---*/
struct key_bindings {

	struct 	key_bindings	*next;		/* Next item in list */
	struct	internal_tab	*intcmd;	/* Internal function ptr */
	UBYTE	*dialogcommand;			/* A dialog command */
	UBYTE	keytype;			/* Key Type Flags */
	struct	_maps		*whichkey;	/* Which key */
	int	argument;			/* Optional Argument */

} *Bind_Root=NULL,*Bind_Tail=NULL;

/*---Include the tables---*/

#include "keymapping.h"		/* Here is the keymap file */
#include "internalfun.h"	/* Internal function table */

/*---Meta key bits---*/

#define ALT_KEYS 	(IEQUALIFIER_LALT | IEQUALIFIER_RALT)
#define SHIFT_KEYS 	(IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT) 
#define SHIFT_ONLY 	(IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT) 
#define CTRL_KEYS 	(IEQUALIFIER_CONTROL) 
#define LF_AMIGA 	(IEQUALIFIER_LCOMMAND)
#define RT_AMIGA 	(IEQUALIFIER_RCOMMAND)

#define ANY_ALT 	(ALT_KEYS|SHIFT_KEYS|CTRL_KEYS|LF_AMIGA|RT_AMIGA)

/*---Meta-Chord flags---*/

#define ALT_FLAG	0x01	/* Alt key */
#define CTR_FLAG	0x02	/* Control key */
#define LFA_FLAG	0x04	/* Left-Amiga key */
#define RTA_FLAG	0x08	/* Right-Amiga key */
#define SHF_FLAG	0x10	/* Must be shifted (only for shift range keys
				   like the numbers, not for caps) */

/*-------------------------------Code Section--------------------------------*/

/*---Copy from start to end to out (don't copy end)---*/

char *strend(ins)
char *ins;
{
	FOREVER {
		if (*ins=='\0') return(ins);
		ins++;
	}
}

VOID gettween(out,start,end,mc)
UBYTE *out,*start,*end;
int mc;
{
	
	while (start<end&&mc>0) {
		*out = *start++;
		if (*out=='\t') *out = ' ';
		out++;
		mc--;
	}
	
	*out++ = '\0';
}

UBYTE spac[40];
UBYTE *spacelist(nspac)
int nspac;
{

	UBYTE *a;
	int i;
	
	a = spac;
	
	for (i = 0; i<nspac; i++) 
		*a++ = ' ';
	
	*a = '\0';
	
	return(spac);
}

VOID killtrail(chk)
char *chk;
{
	char *fnd;
	
	fnd = strend(chk);
	
	fnd--;
	
	while (fnd>=chk) {
		if (*fnd=='\n' ||
		    *fnd==' ' ||
		    *fnd=='\t') *fnd-- = '\0';
		else break;
	}
}

extern struct TextAttr UcadFont;

struct IntuiText tempintui = 
 { 3,2, JAM2, 0,0, &UcadFont, NULL, NULL };

/*---Add binding to binding list---*/

BOOL BindKey(keytext,intfun,dialog)
UBYTE *keytext;
struct internal_tab *intfun;
UBYTE *dialog;
{
	
	struct key_bindings *newbinding;
	UBYTE *a,b;
	int i;

	if (dialog==NULL&&intfun==NULL) return(FALSE);	/* Nothing to bind to */
	
	if (dialog!=NULL&&intfun!=NULL) return(FALSE);	/* Can't bind to both! */	

	newbinding = GI_RemAlloc(sizeof (struct key_bindings));
	if (newbinding==NULL) return(FALSE);
	
	newbinding->next = NULL;
	
	newbinding->intcmd = intfun;
	newbinding->dialogcommand = dialog;
	
	newbinding->keytype = 0;
	newbinding->whichkey = NULL;
	newbinding->argument = 0;

	for (a = keytext; *a!='-'; a++) {
	
		if (*a==' '||
		    *a=='\t') continue;
		
		if (*a=='\0') return(FALSE);	/* Bad format */
		
		b = *a;
		
		if (islower(b)) b = toupper(b);
		
		switch (b) {
		case 'C':
			newbinding->keytype |= CTR_FLAG;
			break;
		case 'A':
			newbinding->keytype |= ALT_FLAG;
			break;
		case 'L':
			newbinding->keytype |= LFA_FLAG;
			break;
		case 'R':
			newbinding->keytype |= RTA_FLAG;
			break;
		default:
			printf("Bogus meta type %c????\n",b);
			return(FALSE);
		}
	}
	
	a++;	/* Step over the delimiter (-) */

	for (; *a!='\0'; a++) {
	
		if (*a==' '||
		    *a=='\t') continue;
		
		for (i=0; key_mapping[i].mc!=NULL; i++) {
			if (strcmp(key_mapping[i].mc,a)==0) {
				newbinding->whichkey = &key_mapping[i];
				break;
			}
			if (key_mapping[i].ac!=NULL)
			 if (strcmp(key_mapping[i].ac,a)==0) {
			 	newbinding->whichkey = &key_mapping[i];
				newbinding->keytype |= SHF_FLAG;
				break;
			 }
		}
		
		break;
		
	}
	
	if (key_mapping[i].mc==NULL) return(FALSE);
	
	
	if (Bind_Tail==NULL) Bind_Root = newbinding;
	else	Bind_Tail->next = newbinding;
	
	Bind_Tail = newbinding;
	
	return(TRUE);
	
}				

UBYTE bindval[20];
UBYTE *BindText(inbind)
struct key_bindings *inbind;
{

	BOOL anync=FALSE;
	UBYTE *a;

	bindval[0] = '\0';
			
	if (inbind->whichkey==NULL) return(bindval);;
	
	a = bindval;
	
	if (inbind->keytype & CTR_FLAG) 
		*a++ = '^';
		
	if (inbind->keytype & ALT_FLAG) {
		*a++ = 'A';
		anync = TRUE;
	}
	
	if (inbind->keytype & LFA_FLAG) {
		*a++ = 'L';
		anync = TRUE;
	}
	
	if (inbind->keytype & RTA_FLAG) {
		*a++ = 'R';
		anync = TRUE;
	}
	
	if (anync) *a++ = '-';
	
	if (inbind->keytype & SHF_FLAG) 
		strcpy(a,inbind->whichkey->ac);
	else	strcpy(a,inbind->whichkey->mc);
	
	return(bindval);
}

VOID ParseMenu(menlev,mentex,funtex,com)
int menlev;
UBYTE *mentex,*funtex,*com;
{

	UBYTE *temptext;
	struct Menu *newmenu;
	struct menu_items *newitem;
	int i;
	
	/* printf("Lev=%d men=/%s/ fun=/%s/ com=/%s/\n",
		menlev,mentex,funtex,com); */
	
	if (menlev<1 || menlev>3) return;	/* Bad level - ignore */

	if (menlev==1) {
		/* Create a main level menu entry */
		
		temptext = GI_RemAlloc(strlen(mentex)+1);
		if (temptext==NULL) return;
		strcpy(temptext,mentex);
		
		newmenu = GI_RemAlloc(sizeof(struct Menu));
		if (newmenu==NULL) return;
		
		newmenu->NextMenu = NULL;
		newmenu->LeftEdge = 0;
		newmenu->TopEdge = 0;
		newmenu->Width = 0;
		newmenu->Height = 0;
		newmenu->Flags = MENUENABLED;
		newmenu->MenuName = temptext;
		newmenu->FirstItem = NULL;
		
		tempintui.IText = temptext;
		
		newmenu->Width = atoi(com);
		newmenu->LeftEdge = atoi(funtex);
				
		if (newmenu->Width<=0) 
			newmenu->Width = IntuiTextLength(&tempintui)+5;
		if (newmenu->LeftEdge<=0) 
			newmenu->LeftEdge = menstart;
		
		menstart = newmenu->LeftEdge + newmenu->Width + 15;
		
		if (Menu_Tail==NULL) Menu_Root = newmenu;
		else	Menu_Tail->NextMenu = newmenu;
		
		Menu_Tail = newmenu;
		
		levelstart = 0;
		
		Cur_Root = Cur_Tail = NULL;
		
		return;
	}
		
	newitem = GI_RemAlloc(sizeof(struct menu_items));

	newitem->menunode.NextItem = NULL;
	newitem->menunode.LeftEdge = 0;
	switch (menlev) {
	case 2:
		newitem->menunode.TopEdge = levelstart;
		levelstart += 10;		/* 10 for topaz type */
		break;
	case 3:
		newitem->menunode.TopEdge = sublevel;
		newitem->menunode.LeftEdge = 40;
		sublevel += 10;
		break;
	}

	newitem->curlen = strlen(mentex)+1;
	temptext = AllocMem(newitem->curlen,0);
	if (temptext==NULL) return;
	strcpy(temptext,mentex);
		
	newitem->next = NULL;
	newitem->menunode.Height = 10;
	newitem->menunode.Flags = ITEMENABLED | ITEMTEXT | HIGHCOMP;
	newitem->menunode.MutualExclude = 0;
	newitem->menunode.ItemFill = NULL;
	newitem->menunode.SelectFill = 0;
	newitem->menunode.Command = 0;
	newitem->menunode.SubItem = NULL;
	newitem->menunode.NextSelect = 0;

	newitem->textnode = tempintui;		/* Initialize */
	newitem->textnode.IText = temptext;

	newitem->menunode.ItemFill = (void *) 
		&newitem->textnode;
		
	newitem->menunode.Width = IntuiTextLength(&newitem->textnode);

	/* Link into item list */
			
	if (MenuItem_Tail==NULL)	MenuItem_Root = newitem;
	else			MenuItem_Tail->next = newitem;
	
	MenuItem_Tail = newitem;
	
	switch (menlev) {
	case 2:
		if (Menu_Tail==NULL) return;	/* No main */
	
		SubItem_Root = SubItem_Tail = NULL;
		sublevel = 0;
		
		if (Cur_Tail==NULL)	Cur_Root = &newitem->menunode;
		else		Cur_Tail->NextItem = &newitem->menunode;
		
		Cur_Tail = &newitem->menunode;
		
		if (Menu_Tail->FirstItem==NULL) 
			Menu_Tail->FirstItem = Cur_Root;
			
		break;
	case 3:
		if (Cur_Root==NULL) return;	/* No sub */
		
		if (SubItem_Tail==NULL) SubItem_Root = &newitem->menunode;
		else		SubItem_Tail->NextItem = &newitem->menunode;
		
		SubItem_Tail = &newitem->menunode;
		
		if (Cur_Tail->SubItem==NULL)
			Cur_Tail->SubItem = SubItem_Root;
			
		break;
		
	}
	
	newitem->orglength = strlen(mentex);
	newitem->intcmd = NULL;
	newitem->dialogcommand = NULL;
	newitem->thisbind = NULL;

/*---Bind the command to the menu---*/
			
	temptext = NULL;
	
	if (com[0]=='\0') return;	/* No binding requested */
	if (com[0]=='$') {
		newitem->dialogcommand = GI_RemAlloc(strlen(com)+1);
		if (newitem->dialogcommand==NULL) return;
		strcpy(newitem->dialogcommand,&com[1]);	/* Skip over $ */
		
	} else {
		for (i=0; access_tables[i].funkey!=NULL; i++) 
		
			if (strcmp(access_tables[i].funkey,com)==0) 
				newitem->intcmd = &access_tables[i];

		if (newitem->intcmd==NULL) {
			printf("No internal function named %s - no bind\n",com);
			return;
		}
		
		newitem->intcmd->menptr = newitem;	/* Backbind to internal table */
	
	}
		
	if (funtex[0]!='\0') 
		if (BindKey(funtex,newitem->intcmd,newitem->dialogcommand))
			newitem->thisbind = Bind_Tail;
	
}

/*---Free menu text at ucad exit time---*/

VOID FreeMenus()
{

	struct menu_items *looper;
	
	for (looper=MenuItem_Root; looper!=NULL; looper=looper->next) 
		FreeMem(looper->textnode.IText,looper->curlen);

}	

/*---Load in the menu database---*/

BOOL menushere=TRUE;

VOID LoadMenuFile()
{

	FILE *infile;
	UBYTE parbod[256],parsl[1025],temp[40],mentex[100],funtex[20],com[257];
	UBYTE *a,*b,*c;
	int i;

	strcpy(parbod,MakeFname(HomePath,"UltraDesign",".menus"));
	infile = fopen(parbod,"r");
	if (infile==NULL) {
		dspalert("UltraDesign -- Vital File Missing",
			"Ultradesign.menus not present!!");
		ParseMenu(1,"File","","");
		ParseMenu(2,"Load","L-L","LVW");
		ParseMenu(2,"Quit","-Q","QUT");
		ParseMenu(1,"Display","","");
		ParseMenu(2,"Repaint","-R","REP");
		ParseMenu(2,"Zoom in","-Z","ZIN");
		menushere=FALSE;
		return;
	}

	FOREVER {
		if (fgets(parsl,1024,infile)==NULL) break;
		
		i=strlen(parsl);
		if (parsl[i-1]=='\n') parsl[i-1]='\0';
		
		if (parsl[0]=='#') continue;		/* Comment */
		
		a = &parsl[0];
		while(*a==' '||*a=='\t') a++;
		
		if (*a=='\0') continue;
		
		mentex[0] = '\0';
		temp[0] = '\0';
		funtex[0] = '\0';
		com[0] = '\0';
		
		/* Search for the menu level */
		
		b = strchr(a,'|');	/* Find first delim */

		if (b==NULL) continue;	/* What???? - skip line */
						
		gettween(temp,a,b,10);
		a = b+1;		/* After delim */
		
		/* Now look for the label */
		
		b = strchr(a,'|');
		
		if (b==NULL) b = strend(a);	/* Find null */
		
		gettween(mentex,a,b,90);
		killtrail(mentex);
		a = b+1;
		
		/* If there is one, get the key binding */
		
		if (*b!='\0') {
		   
		    b = strchr(a,'|');
		   
		    if (b==NULL) b = strend(a);
		
		    gettween(funtex,a,b,18);
		    killtrail(funtex);
		    a = b+1;
		    
		    /* Get the command line */
		    
		    if (*b!='\0') {
		    
		    	b = strchr(a,'|');
			
			if (b==NULL) b = strend(a);
			
			gettween(com,a,b,255);
			killtrail(com);
		 	a = b+1;
		    }
		}
		
		ParseMenu(atoi(temp),mentex,funtex,com);
	}	

	fclose(infile);
		 
}

/*---Add the key bindings to the menu and clean up---*/

VOID PolishItem(inchain)
struct MenuItem *inchain;
{

	struct MenuItem *looper;
	struct menu_items *item;
	int mxln,mxbx,tlen;

	UBYTE	ttex[100];
	UBYTE	otex[100];
	
	if (inchain==NULL) return;
	
	/* Go through and find the max length */
	
	mxln = 5;
	
	for (looper=inchain; looper!=NULL; looper=looper->NextItem) {
	
		item = (struct menu_items *) looper;	/* Cheat */		

		strcpy(otex,item->textnode.IText);
		otex[item->orglength] = '\0';	/* Cut off prev bindings */
		
		if (item->thisbind==NULL) strcpy(ttex,otex);
		else	sprintf(ttex,"%s  %s",otex,BindText(item->thisbind));
		
		if (strlen(ttex)>mxln) mxln = strlen(ttex);
	}
	
	mxbx = 0;	/* Now calc max box width in pixels */
	
	for (looper=inchain; looper!=NULL; looper=looper->NextItem) {
	
		item = (struct menu_items *) looper;	/* Cheat */		

		strcpy(otex,item->textnode.IText);
		otex[item->orglength] = '\0';	/* Cut off prev bindings */

		if (item->thisbind==NULL) strcpy(ttex,otex);
		else {
		
			sprintf(ttex,"%s  %s",otex,BindText(item->thisbind));
			sprintf(ttex,"%s  %s%s",otex,
				spacelist(mxln-strlen(ttex)),
				BindText(item->thisbind));
				
		}
		
		FreeMem(item->textnode.IText,item->curlen);
		
		item->curlen = strlen(ttex)+1;
		item->textnode.IText = AllocMem(item->curlen,0);
		if (item->textnode.IText==NULL) return;
		strcpy(item->textnode.IText,ttex);
		
		tlen = IntuiTextLength(&item->textnode)+2;
		
		if (tlen>mxbx) mxbx = tlen;
		
	}

	/* Fix the box width */	
					
	for (looper=inchain; looper!=NULL; looper=looper->NextItem) 
		looper->Width = mxbx;
}

VOID PolishMenu()
{

	struct Menu *lop1;
	struct MenuItem *lop2;
		
	for (lop1= Menu_Root; lop1!=NULL; lop1=lop1->NextMenu) {
	  PolishItem(lop1->FirstItem);
	  for (lop2=lop1->FirstItem; lop2!=NULL; lop2=lop2->NextItem) 
	    PolishItem(lop2->SubItem);
	  
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
	struct menu_items *looper;
	
	for (looper=MenuItem_Root; looper!=NULL; looper=looper->next) 
	   if (looper==((struct menu_items *) inmen)) {

	   	if (looper->intcmd!=NULL) fac = looper->intcmd->funkey;
	   }

	it = inmen->ItemFill;
	fprintf(file,"%s %s %s\n\r",fac,prm,it->IText);
}
	

BOOL Report_Menus()
{

	int a;
	struct Menu *lop1;
	struct MenuItem *lop2,*lop3;
	int menu,item,sub;
	FILE *ofil;
	
	ofil = openprt("MenuReport");
	if (ofil==NULL) return(FALSE);
	
	fprintf(ofil,"%s Pull-Down Menus and Key Assignments\n\n\r",
		ucadname);
	
	for (lop1= Menu_Root,menu=0; lop1!=NULL; lop1=lop1->NextMenu,menu++) {
	  fprintf(ofil,"    --- %s\n\r",lop1->MenuName);
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
	struct internal_tab *menidx;
	struct Menu *lop1;
	struct MenuItem *lop2,*lop3;
	int menu,item,sub;
	int left1;
	
	LoadMenuFile();
	PolishMenu();
	
	SetMenuStrip(w,Menu_Root);

}


VOID	mensetup(setcode)
int setcode;
{

	int a,setc;
	struct internal_tab *menidx;

	if (setcode==0) setcode = 0xFFFF;	/* Extra special case */
	setc = setcode>>4;

	for (a=0; access_tables[a].funkey!=NULL; a++) {
		menidx = &access_tables[a];
					
		if (menidx->excludec>0) menidx->menptr->menunode.Flags |= CHECKIT;

		if (menidx->excludec==setcode) {
			menidx->menptr->menunode.Flags |= CHECKED;
			continue;
		} else 	if ((menidx->excludec>>4)==setc) {
			menidx->menptr->menunode.Flags &= ~CHECKED;
			continue;
		}

		if (menidx->access!=NULL&&menidx->menptr!=NULL) {
			menidx->menptr->menunode.Flags |= CHECKIT;
			menidx->menptr->menunode.Flags &= ~CHECKED;
			if (*(menidx->access)) 
				menidx->menptr->menunode.Flags |= CHECKED;
		}

	}
}

fullmenu(incode)
int incode;
{	

	ClearMenuStrip(w);

	mensetup(incode);

	SetMenuStrip(w,Menu_Root);
}

VOID Perform_Menu(menidx)
struct internal_tab *menidx;
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

		menidx->menptr->menunode.Flags &= ~CHECKED;
		if (*(menidx->access)) 
			menidx->menptr->menunode.Flags |= CHECKED;
	}

	if (menidx->excludec!=0) mensetup(menidx->excludec);

	if (menidx->Implement!=NULL) {
		result=(*menidx->Implement)();
	}

	SetMenuStrip(w,Menu_Root);

}

VOID	MenuActivated(incode)
int incode;
{


	int menu,item,sub;
	int a;
	struct internal_tab *menidx;
	struct MenuItem *seladdr;
	struct menu_items *looper;
	
	if (incode==MENUNULL) return;		/* No action */

	menu = MENUNUM(incode);
	item = ITEMNUM(incode);
	sub = SUBNUM(incode);

	seladdr = ItemAddress(Menu_Root,incode);
	if (seladdr==NULL) return;		/* What?? */
	
	for (looper=MenuItem_Root; looper!=NULL; looper=looper->next) {
		if (seladdr == (&looper->menunode)) {
			if (looper->intcmd!=NULL) Perform_Menu(looper->intcmd);
			else if (looper->dialogcommand!=NULL)
				MenuLine(looper->dialogcommand);
			break;
		}
	}
}	

VOID ExCommand(bind)
struct key_bindings *bind;
{

	if (bind->intcmd!=NULL) Perform_Menu(bind->intcmd);
	else if (bind->dialogcommand!=NULL) MenuLine(bind->dialogcommand);	
}

int dotpoint;


VOID GetPointAct(sx,sy,action,xr,yr)
WORD sx,sy,action;
STDFLT xr,yr;
{

	if (action==RBUTTONDOWN) {
		ExitSub(GETPOINT_MESS_ABORT);
		return;
	}

	switch (action) {
	case LBUTTONDOWN:
		keyx[dotpoint]=dotx;
		keyy[dotpoint]=doty;
		keyset[dotpoint]=TRUE;
		SExitSub(NULL);
		return;

	}
}

GetPoint()
{

	if (!EnterSub(GETPOINT_MODE,GETPOINT_MESS_EXIT,GetPointAct)) return(FALSE);
	MainMess(GETPOINT_MESS_FIND);

	return(FALSE);
}

VOID	KeyStroke(code,qual)
int code,qual;
{

	int a,b,p= -2;
	BOOL go;
	struct key_bindings *looper;
	int qtype;

	if (code>=0x60) return;

	RidSpurious();
	
	qual &= (SHIFT_KEYS|ALT_KEYS|LF_AMIGA|RT_AMIGA|CTRL_KEYS);
	
	switch(code){
	case 0x4F:	if (qual==0) {
				Roll_Left();
				return;
			}
			break;
	case 0x4E:	if (qual==0) {
				Roll_Right();
				return;	
			}
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
			dotpoint = p;
			GetPoint();
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

	qtype = 0;
	
	if (qual&SHIFT_KEYS) qtype = SHF_FLAG;
	if (qual&ALT_KEYS) qtype = ALT_FLAG;
	if (qual&LF_AMIGA) qtype = LFA_FLAG;
	if (qual&RT_AMIGA) qtype = RTA_FLAG;
	if (qual&CTRL_KEYS) qtype = CTR_FLAG;

	for (b=0; key_mapping[b].mc!=NULL; b++)
		if (key_mapping[b].keyno==code) break;

	if (key_mapping[b].mc==NULL) {
		InfoMess(DISPATCH_MODE,DISPATCH_ILLEGAL);
		printf("Unknown key %x qual %x\n",code,qual);
		return;
	}

	for (looper=Bind_Root; looper!=NULL; looper=looper->next) {
	
		if (looper->whichkey!=(&key_mapping[b])) continue;
		
		if ((looper->keytype&SHF_FLAG)&&
		    (qtype&SHF_FLAG)) {
		    	ExCommand(looper);
			return;
		}
		
		qtype &= (~SHF_FLAG);
		
		if (looper->keytype==qtype) {
		    	ExCommand(looper);
			return;
		}
	}
	
	InfoMess(DISPATCH_MODE,DISPATCH_ILLEGAL);
	printf("Unknown key %x qual %x\n",code,qual);
	return;
}
