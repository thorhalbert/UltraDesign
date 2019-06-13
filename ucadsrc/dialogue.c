/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	dialogue.c - The macro input system
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
 *	1	 9-Oct-88 SH	Initial dialogue input system
 *	2	10-Mar-89 SH	Add the parser
 *	3	26-Jul-89 SH	Add the hooks for console devices including
 *				 conman hook.  These are so arexx can have
 *				 standard in/out
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include "iff.h"
#include <ctype.h>
#include <devices/console.h>

#define MESSAGETABLE
#include "dialog.h"

static int parerr;
int deblen,debinc;
UBYTE *linin;

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

/*
 *     +++
 *     +++
 *  +++++++++	Variable table
 *  +++++++++
 *     +++
 *     +++
 */

#define V_LINEAR 	1
#define V_ANGULAR	2
#define V_COORD		3
#define V_BOOL		4
#define V_LONG		5
#define V_ULONG		6
#define V_FLOAT		7
#define V_PFLOAT	8
#define V_WORD		9

#define V_ELEMENT	10
#define V_LINE		11
#define V_CIRCLE	12
#define V_TEXT		13
#define V_HATCH		14

#define V_LAYER		20
#define V_PEN		21
#define V_STYLE		22
#define V_GROUP		23

#define V_FONT		30
#define V_PATTERN	31

#define V_LINTYPE	50
#define V_ANGTYPE	51

BOOL 	d_debug = FALSE;
BOOL 	d_internal = FALSE;
STDFLT 	d_linear = 0.0;
STDFLT 	d_angular = 0.0;

struct	_itable {
	char	*vname;
	UBYTE	type;
	UBYTE	flags;
	void	*variable;
} internal_v[] = {
  { "$$BOUND_X_MIN",	V_LINEAR,	0,	&xviewMIN	},	
  { "$$BOUND_Y_MIN",	V_LINEAR,	0,	&yviewMIN	},	
  { "$$BOUND_X_MAX",	V_LINEAR,	0,	&xviewMAX	},	
  { "$$BOUND_Y_MAX",	V_LINEAR,	0,	&yviewMAX	},	
  { "$$SNAP_X",		V_LINEAR,	0,	&xsnap		},	
  { "$$SNAP_Y",		V_LINEAR,	0,	&ysnap		},	
  { "$$OFFSET_X",	V_LINEAR,	0,	&xoffset	},	
  { "$$OFFSET_Y",	V_LINEAR,	0,	&yoffset	},	
  { "$$GRID_X",		V_LINEAR,	0,	&xgrid		},	
  { "$$GRID_Y",		V_LINEAR,	0,	&ygrid		},	
  { "$$GRID_OFFSET_X",	V_LINEAR,	0,	&xgridoff	},	
  { "$$GRID_OFFSET_Y",	V_LINEAR,	0,	&ygridoff	},	
  { "$$GRID_X_DIV",	V_WORD,		0,	&xgriddiv	},	
  { "$$GRID_Y_DIV",	V_WORD,		0,	&ygriddiv	},	
  { "$$Q_TEXT",		V_WORD,		0,	&qtextlim	},	
  { "$$PAPER_UNIT",	V_LINEAR,	0,	&paperunit	},	
  { "$$VIEW_UNIT",	V_LINEAR,	0,	&viewunit	},	
  { "$$ARROW_ANGLE",	V_ANGULAR,	0,	&Arrow_Angle	},	
  { "$$ARROW_LENGTH",	V_ANGULAR,	0,	&Arrow_Len	},	
  { "$$LINEAR_TOL",	V_LINEAR,	0,	&lintolerance	},	
  { "$$LINEAR_POINTS",	V_WORD,		0,	&linpoints	},	
  { "$$ANGULAR_TOL",	V_ANGULAR,	0,	&angtolerance	},	
  { "$$ANGULAR_POINTS",	V_WORD, 	0,	&angpoints	},	
  { "$$CENTER_ON",	V_BOOL,		0,	&Center_On	},	
  { "$$COORDS_ON",	V_BOOL,		0,	&Coord_On	},	
  { "$$DRAW_INFO",	V_BOOL,		0,	&Draw_Info	},	
  { "$$GRID_ON",	V_BOOL,		0,	&Grid_On	},	
  { "$$BOUNDS_ON",	V_BOOL,		0,	&Use_Bounds	},	
  { "$$ORTHO_SWITCH",	V_BOOL,		0,	&Ortho_Switch	},	
  { "$$Q_HATCH",	V_BOOL,		0,	&Q_Hatch	},	
  { "$$LINEAR_UNITS",	V_LINTYPE,	0,	&angtype	},	
  { "$$ANGULAR_UNITS",	V_ANGTYPE,	0,	&lintype	},	
  { "$$TEMP_Y_ORIGIN",	V_LINEAR,	0,	&x_origin	},	
  { "$$TEMP_X_ORIGIN",	V_LINEAR,	0,	&y_origin	},	
  { "$$DIM_WIT_PICK",	V_LINEAR,	0,	&Wit_Pick	},	
  { "$$DIM_WIT_OVER",	V_LINEAR,	0,	&Wit_Over	},	
  { "$$DIM_BOX_OFFSET",	V_LINEAR,	0,	&Dim_Line_Offset },	
  { "$$DIM_USE_FIXED",	V_BOOL,		0,	&Use_Fixed_Angle },	
  { "$$DIM_FIXED_ANGLE",V_LINEAR,	0,	&Fixed_Angle	},	
  { "$$DIM_FLIP_FROM",	V_LINEAR,	0,	&Free_Flip_From	},	
  { "$$DIM_FLIP_TO",	V_LINEAR,	0,	&Free_Flip_To	},	
  { "$$DIM_FREE_OFFSET",V_LINEAR,	0,	&Free_Offset	},
  { "$$INTERN_UNIT",	V_BOOL,		0,	&d_internal 	}, 
  { "$$DEBUG",		V_BOOL,		0,	&d_debug 	}, 
  { "$$TEST_LINEAR",	V_LINEAR,	0,	&d_linear	},
  { "$$TEST_ANGULAR",	V_ANGULAR,	0,	&d_angular	},
  { NULL, 		0,		0,	NULL }};

LONG	d_return = 0;

BOOL 	ucadscreen;

VOID 	writeout();
VOID 	idledummy();			/* Don't do anything in check */
VOID 	popdefault();			/* On output, pop out default 
					   console */
					   
#define perm(x) StoreComment(x)
#define DIALOCRLF DIALO("\n\r");
#define DIALO(bla) if (dialogout!=NULL) (*dialogout)(bla)

#define DIALOG_HOOK 1
#define AREXX_HOOK 2
#define MENU_HOOK 3
#define TAKE_HOOK 4

VOID (*dialogout)() = popdefault;	/* Routine to output a string */
VOID (*dialogcheck)() = idledummy;	/* Check for any valid input */

prompt()
{
	if (dialogout==popdefault) return;	/* Don't bother */

	DIALO("\r>>> ");
}

VOID writecheck();
VOID idledummy()
{}	/* Don't wait */

BOOL dialopen=FALSE;

STDFLT ParseActual(instr)
UBYTE *instr;
{

	STDFLT n;
	int i;
			
	i = sscanf(instr,"%f",&n);
	
	if (i==1) return(n);
	
	parerr = TRUE;
	
	return(0.0);
}

STDFLT ParseLinear(instr)
UBYTE *instr;
{

	STDFLT n;

	if (d_internal) return(ParseActual(instr));
			
	n = ParLin(instr, &parerr);

	return(n);
}

STDFLT ParseAngular(instr)
UBYTE *instr;
{

	STDFLT n;
	
	if (d_internal) return(ParseActual(instr));
			
	n = ParAng(instr, &parerr);

	return(n);
}

VOID CloseDialog()
{

	if (!dialopen) return;		/* It's not open */

	Close(dialog_in);

	dialog_in = Input();
	dialog_out = Output();

	dialopen = FALSE;
		
	dialogout = idledummy;
	dialogcheck = idledummy;
	
}

BOOL OpenDialogWB()
{

	UBYTE tstr[256];
	BPTR addrs;

	if (dialopen) CloseDialog();

	sprintf(tstr,dialog_wb1,screen);

	addrs = safe_read(tstr);
	
	if (addrs==0) {

		sprintf(tstr,dialog_wb2,screen);

		addrs = safe_read(tstr);
	
	}

	if (addrs==0) return(FALSE);
	
	dialopen = TRUE;
	
	dialog_in = addrs;
	dialog_out = addrs;
	
	dialogout = writeout;
	dialogcheck = writecheck;

	prompt();
			
	return(TRUE);

}

BOOL OpenDialogUS()
{

	UBYTE tstr[256];
	BPTR addrs;

	if (dialopen) CloseDialog();

	sprintf(tstr,dialog_mn1,screen);

	addrs = safe_read(tstr);
	
	if (addrs==0) {

		sprintf(tstr,dialog_mn2,screen);

		addrs = safe_read(tstr);
	
	}

	if (addrs==0) return(FALSE);
	
	dialopen = TRUE;
	
	dialog_in = addrs;
	dialog_out = addrs;
	
	dialogout = writeout;
	dialogcheck = writecheck;

	prompt();
			
	return(TRUE);

}

VOID writeout(data)
UBYTE *data;
{

	if (!dialopen) popdefault();
	if (!dialopen) return;
	
	Write(dialog_out, data, strlen(data));
	
}

VOID writecheck()
{

	UBYTE tstr[257];
	int rlen;

	if (!dialopen) popdefault();
	if (!dialopen) return;

	if (WaitForChar(dialog_in, 0)) {
		rlen = Read(dialog_in, tstr, 256);
		if (rlen<=0) {
			fprintf(stdout,"Read error %d\n",rlen);
			return;
		}
		tstr[rlen] = '\0';
		if (tstr[rlen-1]=='\n') tstr[rlen-1] = '\0';
		if (tstr[rlen-1]=='\r') tstr[rlen-1] = '\0';
		ProcessLine(tstr,DIALOG_HOOK);
	}
	
}


VOID popdefault()
{

	if (dialopen) return;

	switch (*dialog_def) {
	case 'M':
	case 'm':

		if (!OpenDialogWB())
			if (!OpenDialogUS()) {
				oneliner("Cannot open dialog screen");
				return;
			}
		break;
	default:
		if (!OpenDialogUS())
			if (!OpenDialogWB()) {
				oneliner("Cannot open dialog screen");
				return;
			}
		break;
	}	
	
}

char *rdit;
BOOL hiteof=FALSE;

int getybuf(buf,max_size)
char *buf;
int max_size;
{

	if (hiteof) return(0);

	buf[0] = *rdit++;
	
	if (buf[0]=='\n'||buf[0]=='\0') {
		hiteof = TRUE;
		buf[0] = '\0';
		return(0);
	}

	return(1);
}

extern INFO __info;

VOID inforeset()
{

	__info.log = 0;
	__info.conf = 0;
	__info.type = 0;
	__info.attrib = 0;
	__info.where = 0;
	__info.atflag.assembly = 0;
	__info.atflag.lock = 0;
	__info.atflag.plot = 0;
	__info.atflag.display = 0;
	__info.atflag.color = 100;
	__info.atflag.comment = NULL;
	__info.atflag.font = NULL;
	__info.atflag.height = 0.0;
	__info.atflag.sheet = 0.0;
	__info.atflag.slant = -100.0;
	__info.atflag.aspect = 0.0;
	__info.atflag.hardcolor = NULL;
	__info.maintype = 0;
	__info.template = NULL;
	
	__obj.x = 0.0;
	__obj.y = 0.0;
	__obj.xr = 0.0;
	__obj.yr = 0.0;
	__obj.sa = 0.0;
	__obj.ea = TWOPI;
	__obj.rot = 0.0;
	__obj.scale = 1.0;
	__obj.tstring = NULL;
	__obj.flags = 0;
	__obj.stylename = NULL;
	__obj.file = NULL;
	__obj.pattern = NULL;
	
	
}

BOOL retcolor(incol,red,green,blue)
UBYTE *incol;
UWORD *red,*green,*blue;
{

	int i,j,k,l,sf;
	UBYTE tbuf[20];
	int tans;
	
	i = strlen(incol);
	
	switch (i) {
	case 3:
		k = 1;
		sf = 12;
		break;
	case 6:
		k = 2;
		sf = 8;
		break;
	case 9:
		k = 3;
		sf = 4;
		break;
	case 12:
		k = 4;
		sf = 0;
		break;
	default:
		return(FALSE);
	}
	
	for (l=0,i=k*0; i<(k*0)+k; l++,i++) tbuf[l]=incol[i];
	tbuf[l] = '\0';
	if (sscanf(tbuf,"%x",&tans)!=1) return(FALSE);
	*red = tans << sf;
	
	for (l=0,i=k*1; i<(k*1)+k; l++,i++) tbuf[l]=incol[i];
	tbuf[l] = '\0';
	if (sscanf(tbuf,"%x",&tans)!=1) return(FALSE);
	*green = tans << sf;
	
	for (l=0,i=k*2; i<(k*2)+k; l++,i++) tbuf[l]=incol[i];
	tbuf[l] = '\0';
	if (sscanf(tbuf,"%x",&tans)!=1) return(FALSE);
	*blue = tans << sf;

	return(TRUE);
}

int col24(incol)
UBYTE *incol;
{

	UWORD red,green,blue;
	ULONG retc;
	
	if (!retcolor(incol,&red,&green,&blue)) return(-1);
	
	retc = ((red >> 8) << 16) |
	       ((green >> 8) << 8) | 
	       (blue >> 8);
	       
	return(retc);

}

int reclevel=0;

struct Remember *DialogStore=NULL;

char *dstore(in)
char *in;
{

	char *newb;
	
	newb = AllocRemember(&DialogStore, strlen(in)+2, 0);
	
	fprintf(stdout,"Alloc level %d /%s/ %x\n",reclevel,in,newb);
	if (newb==NULL) return(NULL);
	
	strcpy(newb,in);
	
	return(newb);
}

VOID dfree()
{

	if (DialogStore!=NULL)
		FreeRemember(&DialogStore, TRUE);
	DialogStore=NULL;
}

char *Stostring1(in)
char *in;
{

	char tbuf[200];
	int i;
	
	for (i=0; *in!='\0'; in++) {
		if (*in=='\0'||*in=='\t') continue;
		if (i<=190) tbuf[i++] = *in;
	}
	
	tbuf[i++]= '\0';
	
	return(dstore(tbuf));
		
}

char *Stostring2(in)
char *in;
{

	char tbuf[200];
	int i;
	
	for (i=0; *in!='\0'; in++) {
		if (*in=='\0'||*in=='\t') continue;
		if (*in=='"') continue;
		if (i<=190) tbuf[i++] = *in;
	}
	
	tbuf[i++]= '\0';
	
	return(dstore(tbuf));
		
}

int hooktype;

VOID MenuLine(inbuf)
UBYTE *inbuf;
{


	ProcessLine(inbuf,MENU_HOOK);
}

VOID RexxLine(inbuf)
UBYTE *inbuf;
{

	ProcessLine(inbuf,AREXX_HOOK);
}

ProcessLine(inbuf,hook) 
UBYTE *inbuf;
int hook;
{

	UBYTE dchar,message[256];
	
	deblen = 0;
	debinc = 0;
	hooktype = hook;	
	linin=inbuf;
	
	dchar = '-';
	switch (hook) {
	case DIALOG_HOOK:	dchar = 'D';	break;
	case AREXX_HOOK:	dchar = 'R';	break;
	case MENU_HOOK:		dchar = 'M';	break;
	case TAKE_HOOK:		dchar = 'T';	break;
	}
	
	if (!d_debug) dchar = '*';
	
	if (dchar!='*') {
		sprintf(message,"\r%c-> %s",dchar,inbuf);
		DIALO(message);
		DIALOCRLF;
	}
	
	while (linin==' ') linin++;

	if (*linin=='\0'||
	    *linin=='!') {
	    
	    if (hook==DIALOG_HOOK) prompt();
	    return;
	}

	reclevel++;
		
	hiteof = FALSE;
	rdit = linin;
	
	indialog = TRUE;

#ifdef USE_AREXX
	if (RexxParse(linin)) {
		if (hook==DIALOG_HOOK) prompt();
		return;	/* It was a rexx command */
	}
#endif

	inforeset();
	resetparse(); 
	yyparse(); 

	reclevel--;
	if (reclevel<=0) dfree();
	
	indialog = FALSE;

    	if (hook==DIALOG_HOOK) prompt();
	
}

VOID yyerror(a)
char *a;
{

	int i;

	if (hooktype==DIALOG_HOOK||d_debug) {
		DIALO("?Error: ");
		DIALO(a);
		DIALOCRLF;
		DIALO(linin);
		DIALOCRLF;
		for (i=0; i<deblen; i++) DIALO(" ");
		DIALO("^");
		DIALOCRLF;
	}
	
	d_return = DIALOG_PARSER;

}

/*--------------------------Parser Support Code-------------------------*/

VOID setiffreturn(ret)
IFFP ret;
{

	switch (ret) {
	case END_MARK:
	case IFF_OKAY:
	case IFF_DONE:	d_return = DIALOG_OK;		break;
	case DOS_ERROR:	d_return = DIALOG_DOSERROR;	break;
	case NOT_IFF:	d_return = DIALOG_NOTIFF;	break;
	case NO_FILE:	d_return = DIALOG_NOFILE;	break;
	case CLIENT_ERROR: d_return = DIALOG_CLIENT;	break;
	case BAD_FORM:	d_return = DIALOG_BADFORM;	break;
	case SHORT_CHUNK: d_return = DIALOG_SHORTCHUNK;	break;
	case BAD_IFF:	d_return = DIALOG_BADIFF;	break;
	default:	d_return = DIALOG_UNKNOWNIFF;
	}
}

char *dialogmessage(inmess)
long inmess;
{

	int i;
	
	for (i=0; messagetable[i].messnum>=0; i++) {
		if (messagetable[i].messnum == inmess) 
			return(messagetable[i].error);
	}
	
	return("Unknown error???");
}

VOID logentry(info,message)
char *message;
INFO info;
{

	switch (info.log) {
	case 0:				/* Default */
	case SLASH_LOG_TYPE:
		if (hooktype==DIALOG_HOOK||d_debug) {
			DIALO(message);
			DIALOCRLF;
		}
		break;
	default:
		break;
	}
}		

#define DEDBUG(a) /* printf("Deb: %s\n",a); */

VOID DM_TakeFile(info, file)
UBYTE *file;
INFO info;
{

	UBYTE *rdbuf=NULL;
	FILE *inf=NULL;
	int i;
	
	rdbuf = AllocMem(514,0);

	FOREVER {
	
		inf = fopen(file,"r");
		if (inf==NULL) {
			/* printf("Could not open file %s\n",file);*/
			break;
		}
	
		FOREVER {
				
			if (fgets(rdbuf,512,inf)==NULL) break;
			i = strlen(rdbuf);
			if (rdbuf[i-1]=='\n') rdbuf[i-1] = '\0';
			
			ProcessLine(rdbuf,TAKE_HOOK); 
		}
		
		break;
	
	}
	
	if (inf!=NULL) fclose(inf);
	if (rdbuf!=NULL) FreeMem(rdbuf, 514);

}

VOID DM_Dofile(file)
UBYTE *file;
{

	INFO dummy;
	
	DM_TakeFile(dummy, file);
	
}
	 

VOID DM_LoadFile(file,info)
UBYTE *file;
INFO info;
{

	int i;
	BOOL libmode;
	UBYTE	fullfile[256],message[400];
	IFFP ret;
	
	DEDBUG("DM_LoadFile")

	switch (info.type) {
	case SLASH_VIEW_TYPE:
		libmode = FALSE;
		break;
	case SLASH_LIBRARY_TYPE:
		libmode = TRUE;
		break;
	default:
		yyerror("Cannot load that type of file");
		d_return = DIALOG_BADFILETYPE;
		return;
	}

	strcpy(fullfile,file);
	if (strrchr(fullfile,'.')==NULL) {
	
		while (fullfile[i=(strlen(fullfile)-1)]==' ') 
			fullfile[i] = '\0';
			
		strcpy(&fullfile[strlen(fullfile)],libmode?".ulb":".uvw");
		printf("Load file %s\n",fullfile);
	}

	ret = getview(" "," ",fullfile,libmode);
	Savnam = FALSE;
	
	setiffreturn(ret);
	
	if (d_return==DIALOG_OK) 
		sprintf(message,"[%s file %s loaded sucessfully]",
			libmode?"Library":"View",fullfile);
	else	sprintf(message,"?Error loading %s file %s - %s",
			libmode?"Library":"View",fullfile,
			dialogmessage(d_return));
	logentry(info,message);
	
}

VOID DM_SaveFile(file,info) 
UBYTE *file;
INFO info;
{

	int i;
	BOOL libmode;
	UBYTE	fullfile[256],message[400];
	IFFP ret;

	DEDBUG("DM_SaveFile")
	
	switch (info.type) {
	case SLASH_VIEW_TYPE:
		libmode = FALSE;
		break;
	case SLASH_LIBRARY_TYPE:
		libmode = TRUE;
		break;
	default:
		yyerror("Cannot save that type of file");
		d_return = DIALOG_BADFILETYPE;
		return;
	}

	strcpy(fullfile,file);
	if (strrchr(fullfile,'.')==NULL) {
		while (fullfile[i=(strlen(fullfile)-1)]==' ') 
			fullfile[i] = '\0';
			
		strcpy(&fullfile[strlen(fullfile)],libmode?".ulb":".uvw");
		printf("Load file %s\n",fullfile);
	}

	ret = putview("","","",fullfile,FALSE,FALSE,libmode);
	Savnam = FALSE;
	
	setiffreturn(ret);
	
	if (d_return==DIALOG_OK) 
		sprintf(message,"[%s file %s save sucessfully]",
			libmode?"Library":"View",fullfile);
	else	sprintf(message,"?Error saving %s file %s - %s",
			libmode?"Library":"View",fullfile,
			dialogmessage(d_return));
	logentry(info,message);
	
}

VOID DM_MergeFile(file, info) 
UBYTE *file;
INFO info;
{

	printf("DM_MergeFile %s\n",file);

}

VOID DM_UseFile(file,info) 
UBYTE *file;
INFO info;
{

	printf("DM_UseFile %s\n",file);

}

VOID DM_NewBuffer(info) 
INFO info;
{

	UBYTE message[255];

	BOOL libmode;

	DEDBUG("DM_NewBuffer")

	switch (info.type) {
	case SLASH_VIEW_TYPE:
		libmode = FALSE;
		break;
	case SLASH_LIBRARY_TYPE:
		libmode = TRUE;
		break;
	default:
		yyerror("Cannot edit that type of file");
		d_return = DIALOG_BADFILETYPE;
		return;
	}
	
	Clear_Work(libmode);
	
	sprintf(message,"[All drawing elements cleared (%s mode)]",
		libmode?"Library":"View");
		
	logentry(info,message);

}

extern UBYTE GroupPath[];
BOOL SearchGroupPath();

VOID DM_Current_Group(attrib,info)
UBYTE *attrib;
INFO info;
{

	UBYTE message[255];
	
	if (!SearchGroupPath(attrib)) {
		sprintf(message,"?Cannot find group %s",attrib);
		yyerror(message);
		d_return = DIALOG_NOGROUP;
		return;
	}	

	sprintf("[Current Group set to %s]",GroupPath);
	d_return = DIALOG_OK;
	logentry(info,message);

}

VOID DM_Current_Layer(attrib,info)
UBYTE *attrib;
INFO info;
{

	LAYER_A *findlayer;
	UBYTE message[255];
	
	findlayer = Layer_First(NULL,attrib,FALSE);
	if (findlayer==NULL) {
		sprintf(message,"?Could not find layer %s",attrib);
		yyerror(message);
		d_return = DIALOG_NOLAYER;
		return;
	}	
	
	sprintf(message,"[Current Layer set to %s]",findlayer->Layer_Name);
	Current_Layer = findlayer;
	d_return = DIALOG_OK;
	logentry(info,message);
	
}

VOID DM_Current_Pen(attrib,info)
UBYTE *attrib;
INFO info;
{

	PEN_A *findPen;
	UBYTE message[255];
	
	findPen = Pen_First(NULL,attrib,FALSE);
	if (findPen==NULL) {
		sprintf(message,"?Could not find Pen %s",attrib);
		yyerror(message);
		d_return = DIALOG_NOPEN;
		return;
	}	
	
	sprintf(message,"[Current Pen set to %s]",findPen->Pen_Name);
	Current_Pen = findPen;
	d_return = DIALOG_OK;
	logentry(info,message);
	
}

VOID DM_Current_Style(attrib,info)
UBYTE *attrib;
INFO info;
{

	STYLE_A *findstyle;
	UBYTE message[255];
	
	findstyle = Style_First(NULL,attrib,FALSE);
	if (findstyle==NULL) {
		sprintf(message,"?Could not find style %s",attrib);
		yyerror(message);
		d_return = DIALOG_NOSTYLE;
		return;
	}	
	
	sprintf(message,"[Current Text Style set to %s]",
		findstyle->Letter_Style_ID);
	Current_Style = findstyle;
	d_return = DIALOG_OK;
	logentry(info,message);
	
}

VOID DM_SetCurrent(attrib,info) 
UBYTE *attrib;
INFO info;
{

	DEDBUG("DM_SetCurrent")

	switch (info.attrib) {
	case SLASH_GROUP_TYPE:
		DM_Current_Group(attrib,info);
		break;
	case SLASH_LAYER_TYPE:
		DM_Current_Layer(attrib,info);
		break;
	case SLASH_PEN_TYPE:
		DM_Current_Pen(attrib,info);
		break;
	case SLASH_STYLE_TYPE:
		DM_Current_Style(attrib,info);
		break;
	}

}

VOID DM_Set_Group(attrib,info)
UBYTE *attrib;
INFO info;
{
	GROUP_A *savegroup;
	UBYTE message[255];
	
	savegroup = Current_Group;
	
	if (!SearchGroupPath(attrib)) {
		sprintf(message,"?Could not find group %s",attrib);
		yyerror(message);
		d_return = DIALOG_NOGROUP;
		return;
	}	

	switch (info.atflag.lock) {
	case 1:	Current_Group->Libr_Flags &= (~LIBR_ASSY); break;
	case 2:	Current_Group->Libr_Flags |= (LIBR_ASSY); break;
	}
	switch (info.atflag.lock) {
	case 1:	Current_Group->Group_Flags &= (~GROUP_LOCKED); break;
	case 2:	Current_Group->Group_Flags |= (GROUP_LOCKED); break;
	}
	switch (info.atflag.display) {
	case 1:	Current_Group->Group_Flags |= (GROUP_DOFF); break;
	case 2:	Current_Group->Group_Flags &= (~GROUP_DOFF); break;
	}
	switch (info.atflag.plot) {
	case 1:	Current_Group->Group_Flags |= (GROUP_POFF); break;
	case 2:	Current_Group->Group_Flags &= (~GROUP_POFF); break;
	}
	switch (info.atflag.ghost) {
	case 1:	Current_Group->Group_Flags &= (~GROUP_GHOSTED); break;
	case 2:	Current_Group->Group_Flags |= (GROUP_GHOSTED); break;
	}
		
	if (info.atflag.color<16) 
		Current_Group->Color_ID = info.atflag.color;
	
	if (info.atflag.comment!=NULL) 
		Current_Group->Comment = perm(info.atflag.comment);
			
	sprintf(message,"[Group %s  %s%s%s%s%s]",GroupPath,
		Current_Group->Group_Flags&GROUP_LOCKED?"/LOCK":"/NOLOCK",
		Current_Group->Group_Flags&GROUP_DOFF?"/NODISPLAY":"/DISPLAY",
		Current_Group->Group_Flags&GROUP_GHOSTED?"/GHOST":"/NOGHOST",
		Current_Group->Group_Flags&GROUP_POFF?"/NOPLOT":"/PLOT",
		Current_Group->Libr_Flags&LIBR_ASSY?"/NOASSY":"/ASSY");
	logentry(info,message);
	
	d_return = DIALOG_OK;

	Current_Group = savegroup;
	
	setpath();
	
}

VOID DM_Set_Layer(attrib,info)
UBYTE *attrib;
INFO info;
{

	LAYER_A *findlayer;
	UBYTE message[255];
	
	findlayer = Layer_First(NULL,attrib,FALSE);
	if (findlayer==NULL) {
		sprintf(message,"?Could not find layer %s",attrib);
		yyerror(message);
		d_return = DIALOG_NOLAYER;
		return;
	}	

	for (findlayer=Layer_First(NULL,attrib,FALSE);
		findlayer!=NULL; findlayer=Layer_Next(findlayer,attrib)) {
		
		switch (info.atflag.lock) {
		case 1:	findlayer->Layer_Flags &= (~LAYER_LOCKED); break;
		case 2:	findlayer->Layer_Flags |= (LAYER_LOCKED); break;
		}
		switch (info.atflag.display) {
		case 1:	findlayer->Layer_Flags |= (LAYER_DOFF); break;
		case 2:	findlayer->Layer_Flags &= (~LAYER_DOFF); break;
		}
		switch (info.atflag.plot) {
		case 1:	findlayer->Layer_Flags |= (LAYER_POFF); break;
		case 2:	findlayer->Layer_Flags &= (~LAYER_POFF); break;
		}
		switch (info.atflag.ghost) {
		case 1:	findlayer->Layer_Flags &= (~LAYER_GHOSTED); break;
		case 2:	findlayer->Layer_Flags |= (LAYER_GHOSTED); break;
		}
		
		if (info.atflag.color<16) 
			findlayer->Color_ID = info.atflag.color;
		
		if (info.atflag.comment!=NULL) 
			findlayer->Comment = perm(info.atflag.comment);
			
		sprintf(message,"[Layer %s%s%s%s%s]",findlayer->Layer_Name,
			findlayer->Layer_Flags&LAYER_LOCKED?"/LOCK":"/NOLOCK",
			findlayer->Layer_Flags&LAYER_DOFF?"/NODISPLAY":"/DISPLAY",
			findlayer->Layer_Flags&LAYER_GHOSTED?"/GHOST":"/NOGHOST",
			findlayer->Layer_Flags&LAYER_POFF?"/NOPLOT":"/PLOT");
		logentry(info,message);
	}
	
	d_return = DIALOG_OK;
	
}

VOID DM_Set_Pen(attrib,info)
UBYTE *attrib;
INFO info;
{

	PEN_A *findpen;
	UBYTE message[255];
	int setc;
	
	findpen = Pen_First(NULL,attrib,FALSE);
	if (findpen==NULL) {
		sprintf(message,"?Could not find pen %s",attrib);
		yyerror(message);
		d_return = DIALOG_NOPEN;
		return;
	}	

	for (findpen=Pen_First(NULL,attrib,FALSE);
		findpen!=NULL; findpen=Pen_Next(findpen,attrib)) {
		
		switch (info.atflag.lock) {
		case 1:	findpen->Pen_Flags &= (~PEN_LOCKED); break;
		case 2:	findpen->Pen_Flags |= (PEN_LOCKED); break;
		}
		switch (info.atflag.display) {
		case 1:	findpen->Pen_Flags |= (PEN_DOFF); break;
		case 2:	findpen->Pen_Flags &= (~PEN_DOFF); break;
		}
		switch (info.atflag.plot) {
		case 1:	findpen->Pen_Flags |= (PEN_POFF); break;
		case 2:	findpen->Pen_Flags &= (~PEN_POFF); break;
		}
		switch (info.atflag.ghost) {
		case 1:	findpen->Pen_Flags &= (~PEN_GHOSTED); break;
		case 2:	findpen->Pen_Flags |= (PEN_GHOSTED); break;
		}
		
		if (info.atflag.hardcolor!=NULL) {
			setc = col24(info.atflag.hardcolor);
			if (setc<0) return;
			findpen->Hard_Color = setc;
		}
			
		if (info.atflag.color<16) 
			findpen->Color_ID = info.atflag.color;
			
		if (info.atflag.comment!=NULL) 
			findpen->Comment = perm(info.atflag.comment);
		
		sprintf(message,"[Pen %s%s%s%s%s/HARDCOLOR=%06x]",
			findpen->Pen_Name,
			findpen->Pen_Flags&PEN_LOCKED?"/LOCK":"/NOLOCK",
			findpen->Pen_Flags&PEN_DOFF?"/NODISPLAY":"/DISPLAY",
			findpen->Pen_Flags&PEN_GHOSTED?"/GHOST":"/NOGHOST",
			findpen->Pen_Flags&PEN_POFF?"/NOPLOT":"/PLOT",
			findpen->Hard_Color);
		logentry(info,message);
	}
	
	d_return = DIALOG_OK;
	
}

VOID DM_Set_Style(attrib,info)
UBYTE *attrib;
INFO info;
{

	STYLE_A *findstyle;
	struct Text_Fonts *tfont;
	UBYTE message[255];
	
	findstyle = Style_First(NULL,attrib,FALSE);
	if (findstyle==NULL) {
		sprintf(message,"?Could not find style %s",attrib);
		yyerror(message);
		d_return = DIALOG_NOSTYLE;
		return;
	}	

	for (findstyle=Style_First(NULL,attrib,FALSE);
		findstyle!=NULL; findstyle=Style_Next(findstyle,attrib)) {
		
		if (info.atflag.font!=NULL) {
			tfont = Stroke_First(NULL,info.atflag.font,FALSE);
			if (tfont==NULL) {
				sprintf(message,"?Could not find font %s",
					info.atflag.font);
				yyerror(message);
				d_return = DIALOG_NOFONT;
				return;
			}
			findstyle->draw_font = tfont;
		}
		
		if (info.atflag.height>0.0) 
			findstyle->Prime_Height = info.atflag.height;
		
		if (info.atflag.sheet>0.0) 
			findstyle->Paper_Height = info.atflag.sheet;
		
		if (info.atflag.slant>=-45.0&&info.atflag.slant<=45.0) 
			findstyle->Slant = info.atflag.slant;
		
		if (info.atflag.aspect>0.0) 
			findstyle->Aspect = info.atflag.aspect;
		
		if (info.atflag.comment!=NULL) 
			findstyle->Comment = perm(info.atflag.comment);
		
		sprintf(message,
		    "[Style %s/FONT=%s/HEIGHT=%s/SLANT=%.1f/ASPECT=%.2f]",
			findstyle->Letter_Style_ID,
			findstyle->draw_font->Font_Name_ID,
			DispLin(findstyle->Prime_Height),
			findstyle->Slant,
			findstyle->Aspect);
		
		logentry(info,message);
	}
	
	d_return = DIALOG_OK;
	
}

VOID DM_SetAttrib(attrib,info) 
UBYTE *attrib;
INFO info;
{

	printf("DM_SetAttrib %s\n",attrib);
	
	switch (info.maintype) {
	case GROUP_TYPE:
		DM_Set_Group(attrib,info);
		break;
	case LAYER_TYPE:
		DM_Set_Layer(attrib,info);
		break;
	case PEN_TYPE:
		DM_Set_Pen(attrib,info);
		break;
	case STYLE_TYPE:
		DM_Set_Style(attrib,info);
		break;
	}

}

VOID DM_Create_Group(attrib,info)
UBYTE *attrib;
INFO info;
{

	UBYTE message[255];
	GROUP_A *template,*newgroup;
	
	attrib = CleanAttribs(attrib);
	if (*attrib=='\0') return;

	if (SearchGroupPath(attrib)) {
		sprintf(message,"?Group already on file %s",attrib);
		yyerror(message);
		d_return = DIALOG_NOGROUP;
		return;
	}	

	/* if (info.template==NULL) Not yet implemented */
	template = Current_Group;
	/* else {
		template = Layer_First(NULL,info.template,FALSE);
		if (template==NULL) {
			sprintf(message,"?Template layer %s not found",
				info.template);
			yyerror(message);
			d_return = DIALOG_NOGROUP;
			return;
		}
	} */
	
	newgroup = (GROUP_A *) 
		GI_RemAlloc(sizeof (GROUP_A));
	if (newgroup==NULL) {
		DispMess("Insufficient memory to create new group - sorry");
		return;
	}

	*newgroup = *template;

	strcpy(ArgText,attrib);
	ArgText[30] = '\0';

	sprintf(newgroup->Group_Name,"%s",ArgText);
	newgroup->Use_Count = 0;

	Link_Group(newgroup,Current_Group);

	switch (info.atflag.lock) {
	case 1:	newgroup->Libr_Flags &= (~LIBR_ASSY); break;
	case 2:	newgroup->Libr_Flags |= (LIBR_ASSY); break;
	}
	switch (info.atflag.lock) {
	case 1:	newgroup->Group_Flags &= (~GROUP_LOCKED); break;
	case 2:	newgroup->Group_Flags |= (GROUP_LOCKED); break;
	}
	switch (info.atflag.display) {
	case 1:	newgroup->Group_Flags |= (GROUP_DOFF); break;
	case 2:	newgroup->Group_Flags &= (~GROUP_DOFF); break;
	}
	switch (info.atflag.plot) {
	case 1:	newgroup->Group_Flags |= (GROUP_POFF); break;
	case 2:	newgroup->Group_Flags &= (~GROUP_POFF); break;
	}
	switch (info.atflag.ghost) {
	case 1:	newgroup->Group_Flags &= (~GROUP_GHOSTED); break;
	case 2:	newgroup->Group_Flags |= (GROUP_GHOSTED); break;
	}
		
	if (info.atflag.color<16) 
		newgroup->Color_ID = info.atflag.color;
	
	if (info.atflag.comment!=NULL) 
		newgroup->Comment = perm(info.atflag.comment);
			
	sprintf(message,"[Create group %s  %s%s%s%s%s]",GroupPath,
		newgroup->Group_Flags&GROUP_LOCKED?"/LOCK":"/NOLOCK",
		newgroup->Group_Flags&GROUP_DOFF?"/NODISPLAY":"/DISPLAY",
		newgroup->Group_Flags&GROUP_GHOSTED?"/GHOST":"/NOGHOST",
		newgroup->Group_Flags&GROUP_POFF?"/NOPLOT":"/PLOT",
		newgroup->Libr_Flags&LIBR_ASSY?"/NOASSY":"/ASSY");
	logentry(info,message);
	
	d_return = DIALOG_OK;

}

VOID DM_Create_Layer(attrib,info)
UBYTE *attrib;
INFO info;
{

	LAYER_A *newlayer,*template;
	UBYTE message[255];

	attrib = CleanAttribs(attrib);
	if (*attrib=='\0') return;

	newlayer = Layer_First(NULL,attrib,FALSE);
	if (newlayer!=NULL) {
		sprintf(message,"?Layer %s already on file",attrib);
		yyerror(message);
		d_return = DIALOG_GOTLAYER;
		return;
	}	
	
	if (info.template==NULL) template = Current_Layer;
	else {
		template = Layer_First(NULL,info.template,FALSE);
		if (template==NULL) {
			sprintf(message,"?Template layer %s not found",
				info.template);
			yyerror(message);
			d_return = DIALOG_NOLAYER;
			return;
		}
	}
	
	newlayer = (LAYER_A *) 
		GI_RemAlloc(sizeof (LAYER_A));
	if (newlayer==NULL) {
		DispMess("Insufficient memory to create new layer - sorry");
		return;
	}

	*newlayer = *template;

	strcpy(ArgText,attrib);
	ArgText[30] = '\0';

	strcpy(newlayer->Layer_Name,ArgText);
	newlayer->Use_Count = 0;

	Link_Layer(newlayer);
	
	switch (info.atflag.lock) {
	case 1:	newlayer->Layer_Flags &= (~LAYER_LOCKED); break;
	case 2:	newlayer->Layer_Flags |= (LAYER_LOCKED); break;
	}
	
	switch (info.atflag.display) {
	case 1:	newlayer->Layer_Flags |= (LAYER_DOFF); break;
	case 2:	newlayer->Layer_Flags &= (~LAYER_DOFF); break;
	}
	
	switch (info.atflag.plot) {
	case 1:	newlayer->Layer_Flags |= (LAYER_POFF); break;
	case 2:	newlayer->Layer_Flags &= (~LAYER_POFF); break;
	}

	switch (info.atflag.ghost) {
	case 1:	newlayer->Layer_Flags &= (~LAYER_GHOSTED); break;
	case 2:	newlayer->Layer_Flags |= (LAYER_GHOSTED); break;
	}
		
	if (info.atflag.color<16) 
		newlayer->Color_ID = info.atflag.color;
		
	if (info.atflag.comment!=NULL) 
		newlayer->Comment = perm(info.atflag.comment);
			
	sprintf(message,"[Created layer %s%s%s%s%s]",newlayer->Layer_Name,
		newlayer->Layer_Flags&LAYER_LOCKED?"/LOCK":"/NOLOCK",
		newlayer->Layer_Flags&LAYER_DOFF?"/NODISPLAY":"/DISPLAY",
		newlayer->Layer_Flags&LAYER_GHOSTED?"/GHOST":"/NOGHOST",
		newlayer->Layer_Flags&LAYER_POFF?"/NOPLOT":"/PLOT");
	logentry(info,message);
	
	d_return = DIALOG_OK;
	
}

VOID DM_Create_Pen(attrib,info)
UBYTE *attrib;
INFO info;
{

	PEN_A *newpen,*template;
	UBYTE message[255];
	int setc;
	
	attrib = CleanAttribs(attrib);
	if (*attrib=='\0') return;

	newpen = Pen_First(NULL,attrib,FALSE);
	if (newpen!=NULL) {
		sprintf(message,"?Pen %s already on file",attrib);
		yyerror(message);
		d_return = DIALOG_GOTPEN;
		return;
	}	
	
	if (info.template==NULL) template = Current_Pen;
	else {
		template = Pen_First(NULL,info.template,FALSE);
		if (template==NULL) {
			sprintf(message,"?Template pen %s not found",
				info.template);
			yyerror(message);
			d_return = DIALOG_NOPEN;
			return;
		}
	}
	
	newpen = (PEN_A *) 
		GI_RemAlloc(sizeof (PEN_A));
	if (newpen==NULL) {
		DispMess("Insufficient memory to create new pen - sorry");
		return;
	}

	*newpen = *template;

	strcpy(ArgText,attrib);
	ArgText[30] = '\0';

	sprintf(newpen->Pen_Name,"%s",ArgText);
	newpen->Use_Count = 0;

	Link_Pen(newpen);
	
	switch (info.atflag.lock) {
	case 1:	newpen->Pen_Flags &= (~PEN_LOCKED); break;
	case 2:	newpen->Pen_Flags |= (PEN_LOCKED); break;
	}
	switch (info.atflag.display) {
	case 1:	newpen->Pen_Flags |= (PEN_DOFF); break;
	case 2:	newpen->Pen_Flags &= (~PEN_DOFF); break;
	}
	switch (info.atflag.plot) {
	case 1:	newpen->Pen_Flags |= (PEN_POFF); break;
	case 2:	newpen->Pen_Flags &= (~PEN_POFF); break;
	}
	switch (info.atflag.ghost) {
	case 1:	newpen->Pen_Flags &= (~PEN_GHOSTED); break;
	case 2:	newpen->Pen_Flags |= (PEN_GHOSTED); break;
	}
		
	if (info.atflag.hardcolor!=NULL) {
		setc = col24(info.atflag.hardcolor);
		if (setc<0) return;
		newpen->Hard_Color = setc;
	}
			
	if (info.atflag.color<16) 
		newpen->Color_ID = info.atflag.color;
			
	if (info.atflag.comment!=NULL) 
		newpen->Comment = perm(info.atflag.comment);
		
	sprintf(message,"[Create pen %s%s%s%s%s/HARDCOLOR=%06x]",
		newpen->Pen_Name,
		newpen->Pen_Flags&PEN_LOCKED?"/LOCK":"/NOLOCK",
		newpen->Pen_Flags&PEN_DOFF?"/NODISPLAY":"/DISPLAY",
		newpen->Pen_Flags&PEN_GHOSTED?"/GHOST":"/NOGHOST",
		newpen->Pen_Flags&PEN_POFF?"/NOPLOT":"/PLOT",
		newpen->Hard_Color);
	logentry(info,message);
	
	d_return = DIALOG_OK;
	
}

VOID DM_Create_Style(attrib,info)
UBYTE *attrib;
INFO info;
{

	STYLE_A *newstyle,*template;
	struct Text_Fonts *tfont;
	UBYTE message[255];
	
	attrib = CleanAttribs(attrib);
	if (*attrib=='\0') return;

	newstyle = Style_First(NULL,attrib,FALSE);
	if (newstyle!=NULL) {
		sprintf(message,"?Style %s already on file",attrib);
		yyerror(message);
		d_return = DIALOG_GOTSTYLE;
		return;
	}	
	
	if (info.template==NULL) template = Current_Style;
	else {
		template = Style_First(NULL,info.template,FALSE);
		if (template==NULL) {
			sprintf(message,"?Template style %s not found",
				info.template);
			yyerror(message);
			d_return = DIALOG_NOSTYLE;
			return;
		}
	}
	
	newstyle = (STYLE_A *) 
		GI_RemAlloc(sizeof (STYLE_A));
	if (newstyle==NULL) {
		DispMess("Insufficient memory to create new style - sorry");
		return;
	}

	*newstyle = *template;

	strcpy(ArgText,attrib);
	ArgText[30] = '\0';

	sprintf(newstyle->Letter_Style_ID,"%s",ArgText);
	newstyle->Use_Count = 0;

	Link_Style(newstyle);
	
	if (info.atflag.font!=NULL) {
		tfont = Stroke_First(NULL,info.atflag.font,FALSE);
		if (tfont==NULL) {
			sprintf(message,"?Could not find font %s",
				info.atflag.font);
			yyerror(message);
			d_return = DIALOG_NOFONT;
			return;
		}
		newstyle->draw_font = tfont;
	}
		
	if (info.atflag.height>0.0) 
		newstyle->Prime_Height = info.atflag.height;
		
	if (info.atflag.sheet>0.0) 
		newstyle->Paper_Height = info.atflag.sheet;
		
	if (info.atflag.slant>=-45.0&&info.atflag.slant<=45.0) 
		newstyle->Slant = info.atflag.slant;
		
	if (info.atflag.aspect>0.0) 
		newstyle->Aspect = info.atflag.aspect;
		
	if (info.atflag.comment!=NULL) 
		newstyle->Comment = perm(info.atflag.comment);
		
	sprintf(message,
	    "[Style %s/FONT=%s/HEIGHT=%s/SLANT=%.1f/ASPECT=%.2f]",
		newstyle->Letter_Style_ID,
		newstyle->draw_font->Font_Name_ID,
		DispLin(newstyle->Prime_Height),
		newstyle->Slant,
		newstyle->Aspect);
	
	logentry(info,message);

	d_return = DIALOG_OK;
	
}

VOID DM_CreateAttrib(attrib,info) 
UBYTE *attrib;
INFO info;
{

	printf("DM_CreateAttrib %s\n",attrib);
	
	switch (info.maintype) {
	case GROUP_TYPE:
		DM_Create_Group(attrib,info);
		break;
	case LAYER_TYPE:
		DM_Create_Layer(attrib,info);
		break;
	case PEN_TYPE:
		DM_Create_Pen(attrib,info);
		break;
	case STYLE_TYPE:
		DM_Create_Style(attrib,info);
		break;
	}

}

VOID DM_Rename_Group(from,to,info)
UBYTE *from,*to;
INFO info;
{
	GROUP_A *find,*save,*mother,*tfind;

	save = Current_Group;

	from = CleanAttribs(from);
	if (*from=='\0') return;
	to = CleanAttribs(to);
	if (*to=='\0') return;

	find = Group_Next(Group_First(NULL,from),from);
	if (find==NULL) {
		sprintf(message,"?Could not find group %s",from);
		yyerror(message);
		d_return = DIALOG_NOGROUP;
		Current_Group = save;
		setpath();
		return;
	}	
	
	tfind = Group_Next(Group_First(NULL,to),to);
	if (tfind!=NULL) {
		sprintf(message,"?Already a group named %s",to);
		yyerror(message);
		d_return = DIALOG_NOGROUP;
		Current_Group = save;
		setpath();
		return;
	}	

	mother = find->parent;
	Unlink_Group(find);
	strcpy(find->Group_Name,to);
	Link_Group(find,mother);
	
	Current_Group = save;
	setpath();
	
}

VOID DM_Rename_Layer(from,to,info)
UBYTE *from,*to;
INFO info;
{
	LAYER_A *find,*nfind;

	from = CleanAttribs(from);
	if (*from=='\0') return;
	to = CleanAttribs(to);
	if (*to=='\0') return;

	find = Layer_First(NULL,from,FALSE);
	if (find==NULL) {
		sprintf(message,"?Could not find layer %s",from);
		yyerror(message);
		d_return = DIALOG_NOLAYER;
		return;
	}	

	nfind = Layer_First(NULL,to,FALSE);
	if (nfind!=NULL) {
		sprintf(message,"?There is already a layer %s",to);
		yyerror(message);
		d_return = DIALOG_NOLAYER;
		return;
	}	

	Unlink_Layer(find);
	sprintf(find->Layer_Name,"%s",to);
	find->layer_next = find->layer_prev = NULL;
	Link_Layer(find);

	return;

}

VOID DM_Rename_Pen(from,to,info)
UBYTE *from,*to;
INFO info;
{
	PEN_A *find,*nfind;

	from = CleanAttribs(from);
	if (*from=='\0') return;
	to = CleanAttribs(to);
	if (*to=='\0') return;

	find = Pen_First(NULL,from,FALSE);
	if (find==NULL) {
		sprintf(message,"?Could not find pen %s",from);
		yyerror(message);
		d_return = DIALOG_NOPEN;
		return;
	}	

	nfind = Pen_First(NULL,to,FALSE);
	if (nfind!=NULL) {
		sprintf(message,"?There is already a pen %s",to);
		yyerror(message);
		d_return = DIALOG_NOPEN;
		return;
	}	

	Unlink_Pen(find);
	sprintf(find->Pen_Name,"%s",to);
	find->pen_next = find->pen_prev = NULL;
	Link_Pen(find);

	return;

}

VOID DM_Rename_Style(from,to,info)
UBYTE *from,*to;
INFO info;
{
	STYLE_A *find,*nfind;

	from = CleanAttribs(from);
	if (*from=='\0') return;
	to = CleanAttribs(to);
	if (*to=='\0') return;

	find = Style_First(NULL,from,FALSE);
	if (find==NULL) {
		sprintf(message,"?Could not find style %s",from);
		yyerror(message);
		d_return = DIALOG_NOSTYLE;
		return;
	}	

	nfind = Style_First(NULL,to,FALSE);
	if (nfind!=NULL) {
		sprintf(message,"?There is already a style %s",to);
		yyerror(message);
		d_return = DIALOG_NOSTYLE;
		return;
	}	

	Unlink_Style(find);
	sprintf(find->Letter_Style_ID,"%s",to);
	find->next_style = find->prev_style = NULL;
	Link_Style(find);

	return;

}

VOID DM_RenameAttrib(from,to,info) 
UBYTE *from,*to;
INFO info;
{

	printf("DM_RenameAttrib %s %s\n",from,to);

	switch (info.maintype) {
	case GROUP_TYPE:
		DM_Rename_Group(from,to,info);
		break;
	case LAYER_TYPE:
		DM_Rename_Layer(from,to,info);
		break;
	case PEN_TYPE:
		DM_Rename_Pen(from,to,info);
		break;
	case STYLE_TYPE:
		DM_Rename_Style(from,to,info);
		break;
	}

}

VOID DM_MergeAttrib(from,to,info) 
UBYTE *from,*to;
INFO info;
{

	printf("DM_MergeAttrib %s %s\n",from,to);

}

VOID DM_ExpungeAttrib(info) 
INFO info;
{

	printf("DM_ExpungeAttrib\n");

}

VOID DM_Delete_Layer(attrib,info)
UBYTE *attrib;
INFO info;
{

	LAYER_A *findlayer;
	D_NODE *lnklst;
	UBYTE message[255],*mes;
	int items;
	
	findlayer = Layer_First(NULL,attrib,FALSE);
	if (findlayer==NULL) {
		sprintf(message,"?Could not find layer %s",attrib);
		yyerror(message);
		d_return = DIALOG_NOLAYER;
		return;
	}	

	for (findlayer=Layer_First(NULL,attrib,FALSE);
		findlayer!=NULL; findlayer=Layer_Next(findlayer,attrib)) {
		
		items = 0;
		
		for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
			if (lnklst->node_layer!=findlayer) continue;
			if (lnklst->node_flags&NODE_ONSCREEN) {
				lnklst->node_flags &= (~NODE_SELECTED);
				lnklst->node_flags |= NODE_DELETED;
				GenericRender(lnklst);	/* Erase from screen */
			}
			items++;
			DeleteNode(lnklst);
		}

		mes = NULL;
		if (Current_Layer==findlayer)
			mes = "Cannot delete the current layer";
		if (Dim_Box_Layer==findlayer) 
			mes = "Cannot delete the dimension box layer";
		if (Wit_Layer==findlayer) 
			mes = "Cannot delete the dimension witness layer";
		if (Dim_Line_Layer==findlayer) 
			mes = "Cannot delete the dimension line layer";

		if (mes==NULL) { 
			sprintf(message,
			   "[Layer %s -- deleted, %d drawing items deleted]",
				findlayer->Layer_Name,items);
			Unlink_Layer(findlayer);  /* Get rid of old layer */
		} else	sprintf(message,
			   "%%Layer %s -- %s",findlayer->Layer_Name,mes);

		logentry(info,message);

	}
	
	d_return = DIALOG_OK;


	InventoryAll(FALSE);	/* Get a new inventory count */
}

VOID DM_Delete_Style(attrib,info)
UBYTE *attrib;
INFO info;
{

	STYLE_A *findstyle;
	D_NODE *lnklst;
	UBYTE message[255],*mes;
	int items;
	
	findstyle = Style_First(NULL,attrib,FALSE);
	if (findstyle==NULL) {
		sprintf(message,"?Could not find style %s",attrib);
		yyerror(message);
		d_return = DIALOG_NOSTYLE;
		return;
	}	

	for (findstyle=Style_First(NULL,attrib,FALSE);
		findstyle!=NULL; findstyle=Style_Next(findstyle,attrib)) {
		
		items = 0;
		
		for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
			if (lnklst->node_id!=NODE_TEXT) continue;
			if (GetTextData(lnklst)->letstyle!=findstyle) continue;
			if (lnklst->node_flags&NODE_ONSCREEN) {
				lnklst->node_flags &= (~NODE_SELECTED);
				lnklst->node_flags |= NODE_DELETED;
				GenericRender(lnklst);	/* Erase from screen */
			}
			items++;
			DeleteNode(lnklst);
		}

		mes = NULL;
		if (Current_Style==findstyle)
			mes = "Cannot delete the current style";
		if (Dim_Box_Style==findstyle) 
			mes = "Cannot delete the dimension box style";
		if (Default_Style==findstyle) 
			mes = "Cannot delete the default style";

		if (mes==NULL) { 
			sprintf(message,
			   "[Style %s -- deleted, %d drawing items deleted]",
				findstyle->Letter_Style_ID,items);
			Unlink_Style(findstyle);  /* Get rid of old style */
		} else	sprintf(message,
			   "%%Style %s -- %s",findstyle->Letter_Style_ID,mes);

		logentry(info,message);

	}
	
	d_return = DIALOG_OK;


	InventoryAll(FALSE);	/* Get a new inventory count */
}

VOID DM_Delete_Pen(attrib,info)
UBYTE *attrib;
INFO info;
{

	PEN_A *findpen;
	D_NODE *lnklst;
	UBYTE message[255],*mes;
	int items;
	
	findpen = Pen_First(NULL,attrib,FALSE);
	if (findpen==NULL) {
		sprintf(message,"?Could not find pen %s",attrib);
		yyerror(message);
		d_return = DIALOG_NOPEN;
		return;
	}	

	for (findpen=Pen_First(NULL,attrib,FALSE);
		findpen!=NULL; findpen=Pen_Next(findpen,attrib)) {
		
		items = 0;
		
		for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
			if (lnklst->pen_attach!=findpen) continue;
			if (lnklst->node_flags&NODE_ONSCREEN) {
				lnklst->node_flags &= (~NODE_SELECTED);
				lnklst->node_flags |= NODE_DELETED;
				GenericRender(lnklst);	/* Erase from screen */
			}
			items++;
			DeleteNode(lnklst);
		}

		mes = NULL;
		if (Current_Pen==findpen)
			mes = "Cannot delete the current pen";
		if (Default_Pen==findpen)
			mes = "Cannot delete the default pen";
		if (Dim_Box_Pen==findpen) 
			mes = "Cannot delete the dimension box pen";
		if (Wit_Pen==findpen) 
			mes = "Cannot delete the dimension witness pen";
		if (Dim_Line_Pen==findpen) 
			mes = "Cannot delete the dimension line pen";

		if (mes==NULL) { 
			sprintf(message,
			   "[Pen %s -- deleted, %d drawing items deleted]",
				findpen->Pen_Name,items);
			Unlink_Pen(findpen);  /* Get rid of old pen */
		} else	sprintf(message,
			   "%%Pen %s -- %s",findpen->Pen_Name,mes);

		logentry(info,message);

	}
	
	d_return = DIALOG_OK;


	InventoryAll(FALSE);	/* Get a new inventory count */
}

DM_KillBranch(ingroup,info)
INFO info;
GROUP_A *ingroup;
{

	GROUP_A *looper;
	struct drawing_node *lnklst;
	UBYTE message[255];
	int items;

	if (ingroup==NULL) return;

	items = 0;
	
	for (lnklst=Root_Node; lnklst!=NULL; lnklst=lnklst->node_subsequent) {
		if (lnklst->above_group!=ingroup) continue;
		if (lnklst->node_flags&NODE_ONSCREEN) {
			lnklst->node_flags &= (~NODE_SELECTED);
			lnklst->node_flags |= NODE_DELETED;
			GenericRender(lnklst);		/* Erase from screen */
			if (CurrentRoller==lnklst) 
				CurrentRoller=lnklst->node_subsequent;
		}
		DeleteNode(lnklst);
		items++;
	}

	for (looper=ingroup->descendants; looper!=NULL; looper=looper->next)
		DM_KillBranch(looper,info);
		
	if (ingroup!=Root_Group) {
		sprintf(message,"[Group %s -- deleted, %d drawing items deleted]",
				ingroup->Group_Name,items);
		logentry(info,message);
		
		Unlink_Group(ingroup);	/* Cannot kill root */
	}	else {
		sprintf(message,"[Root not deleted, but %d drawing items deleted]",
				items);
		logentry(info,message);
	}
	
}
	

VOID DM_Delete_Group(attrib,info)
UBYTE *attrib;
INFO info;
{ 

	GROUP_A *savegroup,*findgroup;
	D_NODE *lnklst;
	UBYTE message[255],*mes;
	int items;
	
	savegroup = Current_Group;
	
	findgroup = Group_First(NULL,attrib,FALSE);
	if (findgroup==NULL) {
		sprintf(message,"?Could not find group %s",attrib);
		yyerror(message);
		d_return = DIALOG_NOGROUP;
		Current_Group = savegroup;
		setpath();
		return;
	}	

	for (findgroup=Group_Next(Group_First(NULL,attrib),attrib);
		findgroup!=NULL; findgroup=Group_Next(findgroup,attrib)) {
		
		items = 0;
		

		if (savegroup==findgroup) savegroup = Root_Group;
		if (Dim_Box_Group==findgroup) Dim_Box_Group = savegroup;
		if (Wit_Group==findgroup) Wit_Group = savegroup;
		if (Dim_Line_Group==findgroup) Dim_Line_Group = savegroup; 
		
		DM_KillBranch(findgroup);

		Current_Group = savegroup;
		setpath();
	}
	
}

VOID DM_DeleteAttrib(attrib,info) 
UBYTE *attrib;
INFO info;
{

	printf("DM_DeleteAttrib %s\n",attrib);
	
	switch (info.attrib) {
	case SLASH_GROUP_TYPE:
		DM_Delete_Group(attrib,info);
		break;
	case SLASH_LAYER_TYPE:
		DM_Delete_Layer(attrib,info);
		break;
	case SLASH_PEN_TYPE:
		DM_Delete_Pen(attrib,info);
		break;
	case SLASH_STYLE_TYPE:
		DM_Delete_Style(attrib,info);
		break;
	}

}

VOID DM_SetColor(what,to) 
int what;
UBYTE *to;
{

	UWORD	red,green,blue;
	
	if (!retcolor(to,&red,&green,&blue)) {
		d_return = DIALOG_BADCOLOR;
		return;
	}

	GI_SetColor(what, red,green,blue);

}

VOID DM_Exit(quit,code)
BOOL quit;
int code;
{
	if (quit) abort(code);

}	

VOID DM_Ellipse(inobj)
OBJ inobj;
{

	CreateEllipse(inobj.x+x_origin,inobj.y+y_origin,inobj.xr,inobj.yr,
		      inobj.rot,inobj.sa,inobj.ea);

}

VOID DM_Text(inobj)
OBJ inobj;
{

	STYLE_A *findstyle;
	UBYTE	flipflags;
	UBYTE	handle;
	
	handle = (inobj.flags >> 8) &0x3;
	if (handle<1) handle = 1;
	if (handle>3) handle = 1;
	
	flipflags = inobj.flags &0x7;
	
	findstyle = Current_Style;
	
	if (inobj.stylename != NULL) {
		findstyle = Style_First(NULL,inobj.stylename,FALSE);
		if (findstyle==NULL) {
			sprintf(message,"?Could not find style %s",inobj.stylename);
			yyerror(message);
			d_return = DIALOG_NOSTYLE;
			return;
		}	
	}

	CreateText(inobj.tstring,inobj.x+x_origin,inobj.y+y_origin,
			inobj.rot*(180.0/PI),
			handle,flipflags,findstyle);
			
}

LOCAL STDFLT bxp,byp,brot;

VOID DM_StartLine(invert)
struct coordset invert;
{

	brot = 0.0;
	bxp = ParseLinear(invert.a)+x_origin;
	byp = ParseLinear(invert.b)+y_origin;
	
}

VOID DM_MoreLine(invert)
struct coordset invert;
{

	STDFLT nx,ny,rt;
	
	switch (invert.ftype) {
	case COORD_ABS:
		nx = ParseLinear(invert.a)+x_origin;
		ny = ParseLinear(invert.b)+y_origin;	
		break;
	case COORD_REL:
		nx = bxp + ParseLinear(invert.a);
		ny = byp + ParseLinear(invert.b);
		break;
	case COORD_RANG:
		rt = brot + ParseAngular(invert.a);
		brot = rt;
		nx = bxp + (cos(rt)*ParseLinear(invert.b));
		ny = byp + (sin(rt)*ParseLinear(invert.b));
		break;
	case COORD_AANG:
		rt = ParseAngular(invert.a);
		brot = rt;
		nx = bxp + (cos(rt)*ParseLinear(invert.b));
		ny = byp + (sin(rt)*ParseLinear(invert.b));
		break;
	}

	normalize(brot);
	
	CreateLine(bxp,byp,nx,ny,0);
	bxp = nx;
	byp = ny;

}

/*
 *     +++
 *     +++
 *  +++++++++	Return the mouse click string
 *  +++++++++
 *     +++
 *     +++
 */

VOID returnclick(ostr)
UBYTE *ostr;
{
	UBYTE a[40];
	STDFLT xr,yr;

	ActivateWindow(w);	
			
	while (!Catch_MouseClick(&xr,&yr));

	if (d_internal)		sprintf(ostr,"%d %g,%g",V_COORD,xr,yr);
	else			sprintf(ostr,"%d %s,%s",V_COORD,
					DispLin(xr),
					DispLin(yr));	
			
	return;	

}

/*
 *     +++
 *     +++
 *  +++++++++	The get mouse coordinate function '@'
 *  +++++++++
 *     +++
 *     +++
 */

VOID DM_GetMouse(incoord)
struct coordset *incoord;
{
	UBYTE a[40];
	STDFLT xr,yr;
	
	ActivateWindow(w);	
			
	while (!Catch_MouseClick(&xr,&yr));
	
	incoord->x = xr;
	incoord->y = yr;
	
	if (d_internal)		sprintf(a,"%g",xr);
	else			strcpy(a,DispLin(xr));
	
	incoord->a = Stostring1(a);
	
	if (d_internal)		sprintf(a,"%g",yr);
	else			strcpy(a,DispLin(yr));
	
	incoord->b = Stostring1(a);

	return;	

}

/*
 *     +++
 *     +++
 *  +++++++++	Set internal variable function
 *  +++++++++
 *     +++
 *     +++
 */

VOID setvar(invar,setto)
struct _itable *invar;
UBYTE *setto;
{

	UBYTE message[255];
	int t,i;
	STDFLT	fans;
	ULONG	lans;
	BOOL	bans;
	WORD	wans;
	UBYTE	ucvar[200],*a,*b,c;
	STDFLT	*fptr;
	ULONG	*lptr;
	WORD	*wptr;
	BOOL	*boolptr;
	
	boolptr = (BOOL *) invar->variable;
	fptr = (STDFLT *) invar->variable;
	lptr = (ULONG *) invar->variable;
	wptr = (WORD *) invar->variable;

	parerr=FALSE;

	while (*setto==' ') setto++;

	for (b=ucvar,a=setto; *a!='\0'; b++,a++) {
		c = *a;
		if (islower(c)) c = toupper(c);
		*b = c;
	}
	
	while (ucvar[strlen(ucvar)-1]==' ') ucvar[strlen(ucvar)-1] = '\0';
	
	*b = '\0';
	
	switch (invar->type) {
	case V_LINEAR:
		fans = ParseLinear(ucvar, &parerr);
		break;
	case V_ANGULAR:
		fans = ParseAngular(ucvar, &parerr);
		break;
	case V_ULONG:
		i = sscanf(ucvar,"%l",&lans);
		if (i!=1) parerr=TRUE;
		break;	
	case V_WORD:
		i = sscanf(ucvar,"%d",&t);
		wans = t;
		if (i!=1) parerr=TRUE;
		break;	
	case V_PFLOAT:
		i = sscanf(ucvar,"%f",&fans);
		if (i!=1) parerr=TRUE;
		break;	
	case V_BOOL:
		bans = 99;
		if (streq(ucvar,"ON"))	bans = TRUE;
		if (streq(ucvar,"T"))	bans = TRUE;
		if (streq(ucvar,"YES"))	bans = TRUE;
		if (streq(ucvar,"TRUE")) bans = TRUE;
		if (streq(ucvar,"OK"))	bans = TRUE;
		if (streq(ucvar,"1"))	bans = TRUE;
		
		if (streq(ucvar,"OFF"))	bans = FALSE;
		if (streq(ucvar,"NIL"))	bans = FALSE;
		if (streq(ucvar,"NILL")) bans = FALSE;
		if (streq(ucvar,"NO"))	bans = FALSE;
		if (streq(ucvar,"FALSE")) bans = FALSE;
		if (streq(ucvar,"0"))	bans = FALSE;
	
		if (bans==99) parerr=TRUE;
		break;
	case V_LINTYPE:
		i = sscanf(ucvar,"%d",&wans);
		if (i!=1) wans = 99;
		if (streq(ucvar,"EDI"))	wans = LIN_EDI;
		if (streq(ucvar,"EFI"))	wans = LIN_EFI;
		if (streq(ucvar,"DF"))	wans = LIN_DF;
		if (streq(ucvar,"DI"))	wans = LIN_DI;
		if (streq(ucvar,"MM"))	wans = LIN_MM;
		if (streq(ucvar,"CM"))	wans = LIN_CM;
		if (streq(ucvar,"M"))	wans = LIN_M;
		if (streq(ucvar,"MILE")) wans = LIN_MILE;
		if (streq(ucvar,"KM"))	wans = LIN_KM;
		if (wans==99) parerr=TRUE;
		t = 2;
		break;
	case V_ANGTYPE:
		i = sscanf(ucvar,"%d",&wans);
		if (i!=1) wans = 99;
		if (streq(ucvar,"DEGD")) wans = ANG_DEGD;
		if (streq(ucvar,"DEGM")) wans = ANG_DEGM;
		if (streq(ucvar,"DEGMS")) wans = ANG_DEGMS;
		if (streq(ucvar,"RAD")) wans = ANG_rad;
		if (streq(ucvar,"GRAD")) wans = ANG_Grad;
		if (wans==99) parerr=TRUE;
		t = 2;		
		break;
	}

	if (parerr) {
		sprintf(message,"?Not a good value %s",ucvar);
		yyerror(message);
	
		d_return = DIALOG_BADVALUE;
		return;
	}

	switch (invar->type) {
	case V_LINEAR:
	case V_ANGULAR:
	case V_PFLOAT:
		*fptr = fans;
		printf("The fans was %g\n",fans);
		break;
	case V_ULONG:
		*lptr = lans;
		printf("The lans was %d\n",lans);
		break;
	case V_BOOL:
		*boolptr = bans;
		printf("The bans was %d\n",bans);
		break;
	case V_LINTYPE:
	case V_ANGTYPE:
	case V_WORD:
		*wptr = wans;
		printf("The wans was %d\n",(int) wans);
		break;
	}
			
	printf("We found variable %s set to /%s/\n",invar->vname,ucvar);
}

/*
 *     +++
 *     +++
 *  +++++++++	Show internal variable
 *  +++++++++
 *     +++
 *     +++
 */

VOID showvar(invar,setto)
struct _itable *invar;
UBYTE *setto;
{

	UBYTE message[255];
	int t,i;
	UBYTE	*a;
	STDFLT	*fptr;
	ULONG	*lptr;
	WORD	*wptr;
	BOOL	*boolptr;
	
	boolptr = (BOOL *) invar->variable;
	fptr = (STDFLT *) invar->variable;
	lptr = (ULONG *) invar->variable;
	wptr = (WORD *) invar->variable;

	parerr=FALSE;

	switch (invar->type) {
	case V_LINEAR:
		if (d_internal)	sprintf(message,"%d %g",V_LINEAR,*fptr);
		else		sprintf(message,"%d %s",V_LINEAR,DispLin(*fptr));
		break;
	case V_ANGULAR:
		if (d_internal)	sprintf(message,"%d %g",V_LINEAR,*fptr);
		else		sprintf(message,"%d %s",V_LINEAR,DispAng(*fptr));
		break;
	case V_ULONG:
		sprintf(message,"%d %d",V_ULONG,*lptr);
		break;	
	case V_WORD:
		sprintf(message,"%d %d",V_WORD,*lptr);
		break;	
	case V_PFLOAT:
		sprintf(message,"%d %g",V_PFLOAT,*lptr);
		break;	
	case V_BOOL:
		sprintf(message,"%d %d",V_BOOL,*boolptr);
		break;
	case V_LINTYPE:
		if (d_internal)	sprintf(message,"%d %d",V_LINTYPE,*wptr);
		else {
			a = "Illegal";
			switch (*wptr) {
			case LIN_EDI:	a = "EDI";
			case LIN_EFI:	a = "EFI";
			case LIN_DF:	a = "DF";
			case LIN_DI:	a = "DI";
			case LIN_MM:	a = "MM";
			case LIN_CM:	a = "CM";
			case LIN_M:	a = "M";
			case LIN_MILE:	a = "MILE";
			case LIN_KM:	a = "KM";
			}
			
			sprintf(message,"%d %s",V_LINTYPE,a);
		}
		break;
	case V_ANGTYPE:
		if (d_internal)	sprintf(message,"%d %d",V_ANGTYPE,*wptr);
		else {
			a = "Illegal";
			switch (*wptr) {
			case ANG_DEGD:	a = "DEGD";
			case ANG_DEGM:	a = "DEGM";
			case ANG_DEGMS:	a = "DEGMS";
			case ANG_rad:	a = "RAD";
			case ANG_Grad:	a = "GRAD";
			}
			
			sprintf(message,"%d %s",V_ANGTYPE,a);
	
		}
		break;
	}

	strcpy(setto,message);

}

VOID DM_SetVariable(varname,setto)
UBYTE *varname,*setto;
{

	UBYTE message[255];

	UBYTE	fvar[200],*a,*b,c;
	int	i;
	
	printf("Set variable %s to %s\n",varname,setto);
	
	for (b=fvar,a=varname; *a!='\0'; b++,a++) {
		c = *a;
		if (c==' ') break;
		if (islower(c)) c = toupper(c);
		*b = c;
	}
	
	*b = '\0';
	if (fvar[0]=='\0') return;

	for (i=0; internal_v[i].vname!=NULL; i++) {
		if (strcmp(internal_v[i].vname,fvar)==0) {
			setvar(&internal_v[i],setto);
			return;
		}
	}

	sprintf(message,"?No such variable %s",fvar);
	yyerror(message); 
	
	d_return = DIALOG_NOVARIABLE;
	return;

}

VOID CheckResult(varname,setto)
UBYTE *varname,*setto;
{

	UBYTE message[255];

	UBYTE	fvar[200],*a,*b,c;
	int	i;
	
	printf("Return variable %s to %s\n",varname,setto);
	
	for (b=fvar,a=varname; *a!='\0'; b++,a++) {
		c = *a;
		if (c==' ') break;
		if (islower(c)) c = toupper(c);
		*b = c;
	}
	
	*b = '\0';
	if (fvar[0]=='\0') return;

	for (i=0; internal_v[i].vname!=NULL; i++) {
		if (strcmp(internal_v[i].vname,fvar)==0) {
			showvar(&internal_v[i],setto);
			return;
		}
	}

	if (streq(fvar,"@")) {
		returnclick(setto);
		return;
	}

	sprintf(message,"?No such variable %s",fvar);
	yyerror(message); 
	
	d_return = DIALOG_NOVARIABLE;
	return;

}

/*
 *     +++
 *     +++
 *  +++++++++	Clip/Clone functions
 *  +++++++++
 *     +++
 *     +++
 */

VOID DM_ClipDrag(x,y)
STDFLT x,y;
{
	FlipAbout(0.0,0.0,x+x_origin,y+y_origin,0.0,1.0,FALSE,FALSE,NULL);
}

extern STDFLT OrgX,ToX,OrgY,ToY;

VOID DM_ClipCloneFrom(x,y)
STDFLT x,y;
{

	OrgX = x+x_origin;
	OrgY = y+y_origin;
	
}

VOID DM_ClipCloneTo(x,y)
STDFLT x,y;
{

	ToX = x+x_origin;
	ToY = y+y_origin;
	
	ClonePieces();

}

VOID DM_ClipScale(x,y,scale)
STDFLT x,y,scale;
{
	FlipAbout(x+x_origin,y+y_origin,x+x_origin,y+y_origin,
		0.0,scale,FALSE,FALSE,NULL);
}

VOID DM_ClipRotate(x,y,ang)
STDFLT x,y,ang;
{
	FlipAbout(x+x_origin,y+y_origin,x+x_origin,y+y_origin,
		ang,1.0,FALSE,FALSE,NULL);
}

VOID DM_ClipFlipX(x)
STDFLT x;
{
	FlipAbout(x+x_origin,0.0,x+x_origin,0.0,0.0,1.0,TRUE,FALSE,NULL);
}

VOID DM_ClipFlipY(y)
STDFLT y;
{
	FlipAbout(0.0,y+y_origin,0.0,y+y_origin,0.0,1.0,FALSE,TRUE,NULL);
}

VOID DM_Echo(ret,string)
BOOL ret;
UBYTE *string;
{}

VOID DM_EchoScreen(string)
UBYTE *string;
{
	TitleMessage(string);
}

O_HATCH newdhatch;


VOID DM_FinishHatching()
{

	D_NODE	*find_node,*wander;
	O_LINE	*lineobj,*thisobj;
	WORD	sx,sy;
	D_NODE *donode;

	newdhatch.hatch_flags = 0;
	newdhatch.number_polys = 1;

	if (Last_Node==NULL) {
		yyerror("No objects in the file to hatch???");
		d_return = DIALOG_NOOBJECTS;
		return;
	}
	
	if (Last_Node->node_id==NODE_ELLIP) {
		CreateHatch(&newdhatch, Last_Node);
		return;
	}
	
	if (Last_Node->node_id!=NODE_LINE) {
		yyerror("No lines or ellipses are last!!");
		d_return = DIALOG_NOOBJECTS;
		return;
	}
	
	find_node = Last_Node;
	
	thisobj = GetLineData(find_node);
	for (wander = find_node->node_previous; wander!=NULL; 
	    wander=wander->node_previous) {
		if (wander->node_id!=NODE_LINE) break;
		lineobj = GetLineData(wander);
		if ((lineobj->x2==thisobj->x1) &&
		    (lineobj->y2==thisobj->y1)) {
		    	find_node = wander;	/* Walk back */
		    	thisobj = lineobj;
		} else break;
	}
	
	wander = find_node->node_subsequent;
	if (wander!=NULL) {
		if (wander->node_id!=NODE_LINE) {
			yyerror("One line does not a polygon make");
			d_return = DIALOG_NOOBJECTS;
			return;
		}
		lineobj = GetLineData(wander);
		if ((lineobj->x1!=thisobj->x2) ||
		    (lineobj->y1!=thisobj->y2)) {
			yyerror("One line does not a polygon make");
			d_return = DIALOG_NOOBJECTS;
			return;
		}
	} else {
		yyerror("One line does not a polygon make");
		d_return = DIALOG_NOOBJECTS;
		return;
	}
			
	CreateHatch(&newdhatch, find_node);

}
			

VOID DM_HatchFill()
{

	newdhatch.hatch_type = HATCH_SOLID;
	newdhatch.hatch_pattern = NULL;
	newdhatch.pattern_rot = 0.0;
	newdhatch.pattern_scale = 1.0;

	DM_FinishHatching();
	
}

VOID DM_HatchPattern(obj)
OBJ obj;
{

	UBYTE nameagg[100];
	T_HATCH *gethat;

	if (obj.file==NULL) {
		if (Current_Hatch==NULL) {
			yyerror("No hatch file switch specified");
			d_return = DIALOG_MISSING;
			return;
		}
		obj.file = Current_Hatch->File_Name;
		if (obj.file==NULL) {
			yyerror("No hatch file switch specified");
			d_return = DIALOG_MISSING;
			return;
		}
	}
			

	if (obj.pattern==NULL) {
		if (Current_Hatch==NULL) {
			yyerror("No hatch pattern switch specified");
			d_return = DIALOG_MISSING;
			return;
		}
		obj.pattern = Current_Hatch->Hatch_Name;
		if (obj.pattern==NULL) {
			yyerror("No hatch pattern switch specified");
			d_return = DIALOG_MISSING;
			return;
		}
	}

	gethat = Hatch_Search_File(obj.file);
	
	if (gethat==NULL)
		(VOID) LoadHatch(obj.file);
		
	sprintf(nameagg,"%s&%s",obj.file,
				obj.pattern);
		
	gethat = Hatch_Search(nameagg);
	
	if (gethat==NULL) {
		yyerror("Your hatch file or pattern was not found");
		d_return = DIALOG_NOHATCH;
		return;
	}

	newdhatch.hatch_type = HATCH_PATTERN;
	newdhatch.hatch_pattern = gethat;
	newdhatch.pattern_rot = obj.rot;
	newdhatch.pattern_scale = obj.scale;
	
	DM_FinishHatching();
	
}
