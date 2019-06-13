#define COORD_ABS 	0x01
#define COORD_REL 	0x02
#define COORD_AANG 	0x03
#define COORD_RANG	0x04

#define COORD_FROM	0x01
#define COORD_TO	0x02

struct coordset {
	char 	*a,*b;
	UBYTE	ftype;
	UBYTE	flags;
	STDFLT	x,y;
};

#define OBJ struct _objdata

OBJ {
	STDFLT	x,y;
	STDFLT	xr,yr;
	STDFLT	sa,ea;
	STDFLT	rot;
	STDFLT	scale;
	UBYTE	*tstring;
	ULONG	flags;
	UBYTE	*stylename;
	UBYTE	*file;
	UBYTE	*pattern;
} __obj;

#define INFO struct _infodata

INFO  {
	UBYTE	log;
	UBYTE	conf;
	UBYTE	type;
	UBYTE	attrib;
	UBYTE	where;
	struct _attype {
		UBYTE 	assembly;
		UBYTE	lock;
		UBYTE	ghost;
		UBYTE	plot;
		UBYTE	display;
		UWORD	color;
		UBYTE	*comment;
		UBYTE	*font;
		STDFLT	height;
		STDFLT	sheet;
		STDFLT	slant;
		STDFLT	aspect;
		UBYTE	*hardcolor;
	} atflag;
	UBYTE	maintype;
	UBYTE	*template;
} __info;

#define SLASH_LOG_TYPE		1
#define SLASH_NOLOG_TYPE	2
#define SLASH_CONFIRM_TYPE	3
#define SLASH_NOCONFIRM_TYPE	4
#define SLASH_POPOUT_TYPE	5
#define SLASH_VIEW_TYPE		6
#define SLASH_LIBRARY_TYPE	7
#define SLASH_HATCH_TYPE	8
#define SLASH_FONT_TYPE		9
#define SLASH_GROUP_TYPE	10
#define SLASH_LAYER_TYPE	11
#define SLASH_PEN_TYPE		12
#define SLASH_STYLE_TYPE	13
#define SLASH_ROOT_TYPE		14
#define SLASH_CURRENT_TYPE	15
#define GROUP_TYPE		16
#define PEN_TYPE		17
#define LAYER_TYPE		18
#define STYLE_TYPE		19

#include "errors.h"

#ifdef MESSAGETABLE
struct _messtable {
	LONG	messnum;
	char	*error;
} messagetable[] = {

 { DIALOG_OK,		"All OK" },
 { DIALOG_REXX,		"All OK - Response message sent" },
 { DIALOG_PARSER,	"Error parsing input line" },
 { DIALOG_BADFILETYPE,	"File type non-sequiteur" },
 { DIALOG_BADCOLOR,	"Hex color string bad" },
 { DIALOG_NOWILDS,	"No Wildcards allowed here" },
 { DIALOG_NOVARIABLE,	"No variable of that name exists" },
 { DIALOG_BADVALUE,	"Value has bad format" },
 { DIALOG_DOSERROR,	"Got a dos error - bad read, etc" },
 { DIALOG_NOFILE,	"Could not find requested file" },
 { DIALOG_CLIENT,	"Some programming error" },
 { DIALOG_BADFORM,	"Bad IFF form error" },
 { DIALOG_SHORTCHUNK,	"Bad IFF chunk error" },
 { DIALOG_BADIFF,	"Mal-formed IFF file" },
 { DIALOG_NOTIFF,	"This is not an IFF file" },
 { DIALOG_UNKNOWNIFF,	"Unknown iff error" },
 { DIALOG_NOLAYER,	"Layer not found" },
 { DIALOG_NOPEN,	"Pen not found" },
 { DIALOG_NOGROUP,	"Group not found" },
 { DIALOG_NOSTYLE,	"Style not found" },
 { DIALOG_NOHATCH,	"Hatch pattern not found" },
 { DIALOG_NOFONT,	"Font not found" },
 { DIALOG_GOTLAYER,	"Layer already here" },
 { DIALOG_GOTPEN,	"Pen already here" },
 { DIALOG_GOTGROUP,	"Group already here" },
 { DIALOG_GOTSTYLE,	"Style already here" },
 { DIALOG_GOTHATCH,	"Hatch already here" },
 { DIALOG_GOTFONT,	"Font already here" },
 { DIALOG_NOREXSYS,	"No rexxsyslib.library found" },
 { DIALOG_NOREXPORT,	"No rexx message port found" },
 { -1, NULL } };
 
#endif
