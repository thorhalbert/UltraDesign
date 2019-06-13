%{

#include "ucad.h"
#include "dialog.h"

/*
 *     +++
 *     +++
 *  +++++++++	The variable type binding
 *  +++++++++
 *     +++
 *     +++
 */

#define YYSTYPE union __yystype

YYSTYPE {
	void	*voidt;
	char	*ctype;
	STDFLT	ftype;
	LONG	itype;
	struct	coordset coord;
};

BOOL	parerr=FALSE;

STDFLT ParseLinear();
STDFLT ParseAngular();
#define dolin(a,b) ParseLinear(a)
#define doang(a,b) ParseAngular(a)

%}

/*
 *     +++
 *     +++
 *  +++++++++	Token definitions and their types
 *  +++++++++
 *     +++
 *     +++
 */

%token	<voidt>	SLASH_LOG
%token	<voidt>	SLASH_NOLOG
%token	<voidt>	SLASH_QUIET
%token	<voidt>	SLASH_NOQUIET
%token	<voidt>	SLASH_CONFIRM
%token	<voidt>	SLASH_NOCONFIRM
%token	<voidt>	SLASH_POPOUT
%token	<voidt>	SLASH_NOPOPOUT
%token	<voidt>	SLASH_VIEW
%token	<voidt>	SLASH_LIBRARY
%token	<voidt>	SLASH_FONT
%token	<voidt>	SLASH_HATCH
%token	<voidt>	SLASH_GROUP
%token	<voidt>	SLASH_LAYER
%token	<voidt>	SLASH_PEN
%token	<voidt>	SLASH_STYLE
%token	<voidt>	SLASH_ROOT
%token	<voidt>	SLASH_CURRENT
%token	<voidt>	SLASH_ASSEMBLY
%token	<voidt>	SLASH_NOASSEMBLY
%token	<voidt>	SLASH_LOCK
%token	<voidt>	SLASH_NOLOCK
%token	<voidt>	SLASH_GHOST
%token	<voidt>	SLASH_NOGHOST
%token	<voidt>	SLASH_PLOT
%token	<voidt>	SLASH_NOPLOT
%token	<voidt>	SLASH_DISPLAY
%token	<voidt>	SLASH_NODISPLAY
%token	<voidt>	SLASH_COMMENT
%token	<voidt>	SLASH_COLOR
%token	<voidt>	SLASH_HEIGHT
%token	<voidt>	SLASH_SHEET
%token	<voidt>	SLASH_SLANT
%token	<voidt>	SLASH_ASPECT
%token	<voidt>	SLASH_TEMPLATE
%token	<voidt>	SLASH_HARDCOLOR
%token	<voidt> SLASH_QUIT
%token	<voidt>	SLASH_ROTATE
%token 	<voidt> SLASH_START
%token	<voidt> SLASH_END
%token	<voidt>	SLASH_CONT
%token	<voidt>	SLASH_FLIP
%token	<voidt>	SLASH_HANDLE
%token	<voidt>	SLASH_FILL
%token	<voidt>	SLASH_PATTERN
%token	<voidt>	SLASH_FILE
%token	<voidt>	HATCH	
%token	<voidt>	TEXTC
%token	<voidt>	EQUALS
%token	<voidt>	LOAD
%token	<voidt>	SAVE
%token	<voidt>	MERGE
%token	<voidt>	USE
%token	<voidt>	NEW
%token	<voidt>	SET
%token	<voidt>	GROUP
%token	<voidt>	LAYER
%token	<voidt>	LINE
%token	<voidt>	ELLIPSE
%token	<voidt>	PEN
%token	<voidt>	STYLE
%token	<voidt>	RENAME
%token	<voidt>	EXPUNGE
%token	<voidt>	DELETE
%token	<voidt>	COLOR
%token	<voidt>	CREATE
%token	<voidt>	CURRENT
%token	<voidt>	TAKE
%token	<voidt>	EXIT
%token	<voidt> COMMA
%token	<voidt> SLASH_ABS
%token	<voidt> SLASH_REL
%token	<voidt> SLASH_ANGLE
%token	<voidt> SLASH_TURTLE
%token	<voidt> SLASH_TOARROW
%token	<voidt> SLASH_FROMARROW
%token	<ctype>	STRING
%token	<ctype>	SIMPLE_NUMBER
%token	<ctype>	FLOAT_NUMBER
%token	<ctype>	HEX_NUMBER
%token	<ctype>	TEXT_STRING
%token	<voidt>	ERROR_TEXT
%token  <voidt> AT_SIGN
%token	<ctype> VARIABLE

%token 	<voidt>	CLIP	
%token 	<voidt>	SLASH_DRAG	
%token 	<voidt>	SLASH_OFFSET	
%token 	<voidt>	SLASH_CLONE	
%token 	<voidt>	SLASH_FROM	
%token 	<voidt>	SLASH_TO	
%token 	<voidt>	SLASH_STEADY	
%token 	<voidt>	SLASH_SCALE	
%token 	<voidt>	SLASH_FACTOR	
%token 	<voidt>	SLASH_X
%token 	<voidt>	SLASH_Y
%token 	<voidt>	SLASH_CENTER	

/*
 *     +++
 *     +++
 *  +++++++++	Internal return types
 *  +++++++++
 *     +++
 *     +++
 */

%type 	<ctype> file_name attrib_name string attribtype undif_unit
%type	<ctype> var_value
%type	<itype> color_number hexnumber decnumber lineflags
%type	<ftype> angular_unit linear_unit float_val
%type	<coord> abs_coord rel_coord abs_angle rel_angle pair lin_pair at_fun

%start	mainloop

%%

/*
 *     +++
 *     +++
 *  +++++++++	Main parse chain
 *  +++++++++
 *     +++
 *     +++
 */

mainloop :	changes
	|	setcolor
	|	createattrib
	|	setattrib
	|	usefile
	|	new
	|	setcurrent
	|	loadfile
	|	savefile
	|	mergefile
	|	line
	|	ellipse
	|	hatch
	|	text
	| 	take
	|	exitout
	|	variable
	|	clip
	;


/*
 *     +++
 *     +++
 *  +++++++++	Primative tokens
 *  +++++++++
 *     +++
 *     +++
 */

file_name : 	TEXT_STRING
			{ $$ = Stostring1($1); }
	|	STRING
			{ $$ = Stostring2($1); }
	|	SIMPLE_NUMBER
			{ $$ = Stostring1($1); }
	|	HEX_NUMBER
			{ $$ = Stostring1($1); }
	;

attrib_name : 	TEXT_STRING
			{ $$ = Stostring1($1); }
	|	STRING
			{ $$ = Stostring2($1); }
	|	SIMPLE_NUMBER
			{ $$ = Stostring1($1); }
	|	HEX_NUMBER
			{ $$ = Stostring1($1); }
	;

hexnumber : 	SIMPLE_NUMBER
			{ $$ = Stostring1($1); }
	|	HEX_NUMBER
			{ $$ = Stostring1($1); }
	;

decnumber : 	SIMPLE_NUMBER
			{ sscanf($1,"%d",&$$); }
	;

string : 	STRING
			{ $$ = Stostring2($1); }
	;

color_number : 	SIMPLE_NUMBER
			{ $$ = atoi($1); }
	;

undif_unit : 	STRING
			{ $$ = Stostring2($1); }
	|	SIMPLE_NUMBER
			{ $$ = Stostring1($1); }
	|	TEXT_STRING
			{ $$ = Stostring1($1); }
	|	HEX_NUMBER
			{ $$ = Stostring1($1); }
	|	FLOAT_NUMBER
			{ $$ = Stostring1($1); }
	;

linear_unit : 	STRING
			{ $$ = dolin(Stostring2($1), &parerr); }
	|	TEXT_STRING
			{ $$ = dolin($1, &parerr); }
	|	SIMPLE_NUMBER
			{ $$ = dolin($1, &parerr); }
	|	HEX_NUMBER
			{ $$ = dolin($1, &parerr); }
	|	FLOAT_NUMBER
			{ $$ = dolin($1, &parerr); }
	;

angular_unit :	STRING
			{ $$ = doang(Stostring2($1), &parerr); }
	|	TEXT_STRING
			{ $$ = doang($1, &parerr); }
	|	SIMPLE_NUMBER
			{ $$ = doang($1, &parerr); }
	|	HEX_NUMBER
			{ $$ = doang($1, &parerr); }
	|	FLOAT_NUMBER
			{ $$ = doang($1, &parerr); }
	;

float_val : 	SIMPLE_NUMBER
			{ sscanf($1, "%f", &$$); }
	|	HEX_NUMBER
			{ sscanf($1, "%f", &$$); }
	|	FLOAT_NUMBER
			{ sscanf($1, "%f", &$$); }
	;


lineflags :	lineflags lineflags
			{ $$ = $1 | $2 }
	|	SLASH_FROMARROW
			{ $$ = COORD_FROM; }
	|	SLASH_TOARROW
			{ $$ = COORD_TO; }
	;

pair : 		undif_unit COMMA undif_unit
			{ $$.a = $1; 
			  $$.b = $3;
			  $$.flags = 0;
			  $$.ftype = 0; }

/* Don't forget that the corrds from getmouse must fill x,y and a,b */
						
at_fun :	AT_SIGN
			{ 
			  DM_GetMouse(&$$); 
			  $$.flags = 0;
			  $$.ftype = COORD_ABS; }
		
						
lin_pair : 	linear_unit COMMA linear_unit
			{ $$.x = $1; 
			  $$.y = $3;
			  $$.flags = 0;
			  $$.ftype = COORD_ABS; }
	|	at_fun
			{
			  $$ = $1; }
		
		
abs_coord :	pair
			{ $$ = $1;
			  $$.ftype = COORD_ABS; }
	|	pair lineflags SLASH_ABS lineflags
			{ $$ = $1;
			  $$.flags = $2 | $4;
			  $$.ftype = COORD_ABS; }
	|	pair lineflags SLASH_ABS
			{ $$ = $1;
			  $$.flags = $2;
			  $$.ftype = COORD_ABS; }
	|	pair SLASH_ABS lineflags
			{ $$ = $1;
			  $$.flags = $3;
			  $$.ftype = COORD_ABS; }
	|	pair SLASH_ABS
			{ $$ = $1;
			  $$.ftype = COORD_ABS; }
	|	pair lineflags 
			{ $$ = $1;
			  $$.flags = $2;
			  $$.ftype = COORD_ABS; }
	|	at_fun
			{ $$ = $1; }
	;
		
rel_coord :	pair lineflags SLASH_REL lineflags
			{ $$ = $1;
			  $$.flags = $2 | $4;
			  $$.ftype = COORD_REL; }
	|	pair lineflags SLASH_REL
			{ $$ = $1;
			  $$.flags = $2;
			  $$.ftype = COORD_REL; }
	|	pair SLASH_REL lineflags
			{ $$ = $1;
			  $$.flags = $3;
			  $$.ftype = COORD_REL; }
	|	pair SLASH_REL
			{ $$ = $1;
			  $$.ftype = COORD_REL; }
	;
	
abs_angle :	pair lineflags SLASH_ANGLE lineflags
			{ $$ = $1;
			  $$.flags = $2 | $4;
			  $$.ftype = COORD_AANG; }
	|	pair lineflags SLASH_ANGLE
			{ $$ = $1;
			  $$.flags = $2;
			  $$.ftype = COORD_AANG; }
	|	pair SLASH_ANGLE lineflags
			{ $$ = $1;
			  $$.flags = $3;
			  $$.ftype = COORD_AANG; }
	|	pair SLASH_ANGLE
			{ $$ = $1;
			  $$.ftype = COORD_AANG; }
	;
		
rel_angle :	pair lineflags SLASH_TURTLE lineflags
			{ $$ = $1;
			  $$.flags = $2 | $4;
			  $$.ftype = COORD_RANG; }
	|	pair lineflags SLASH_TURTLE
			{ $$ = $1;
			  $$.flags = $2;
			  $$.ftype = COORD_RANG; }
	|	pair SLASH_TURTLE lineflags
			{ $$ = $1;
			  $$.flags = $3;
			  $$.ftype = COORD_RANG; }
	|	pair SLASH_TURTLE
			{ $$ = $1;
			  $$.ftype = COORD_RANG; }
	;
		

/*
 *     +++
 *     +++
 *  +++++++++	Basic information flags parsing
 *  +++++++++
 *     +++
 *     +++
 */


infoflags :	infoflags infoflags
	|	SLASH_LOG
			{ __info.log = SLASH_LOG_TYPE; }
	|	SLASH_NOLOG
			{ __info.log = SLASH_NOLOG_TYPE; }
	|	SLASH_NOQUIET
			{ __info.log = SLASH_LOG_TYPE; }
	|	SLASH_QUIET
			{ __info.log = SLASH_NOLOG_TYPE; }
	|	SLASH_CONFIRM
			{ __info.conf = SLASH_CONFIRM_TYPE; }
	|	SLASH_NOCONFIRM
			{ __info.conf = SLASH_NOCONFIRM_TYPE; }
	|	SLASH_POPOUT
			{ __info.conf = SLASH_POPOUT_TYPE; }
	|	SLASH_NOPOPOUT
			{ __info.conf = SLASH_NOCONFIRM_TYPE; }
	;
	
/*
 *     +++
 *     +++
 *  +++++++++	file type switches for load and save
 *  +++++++++
 *     +++
 *     +++
 */

fileswitch :	infoflags
	|	fileswitch fileswitch
	|	SLASH_VIEW
			{ __info.type = SLASH_VIEW_TYPE; }
	|	SLASH_LIBRARY
			{ __info.type = SLASH_LIBRARY_TYPE; }
	|	SLASH_FONT
			{ __info.type = SLASH_FONT_TYPE; }
	|	SLASH_HATCH
			{ __info.type = SLASH_HATCH_TYPE; }
	;

/*
 *     +++
 *     +++
 *  +++++++++	attribute switches
 *  +++++++++
 *     +++
 *     +++
 */

attribswitch :	infoflags
	|	attribswitch attribswitch
	|	SLASH_GROUP
			{ __info.attrib = SLASH_GROUP_TYPE; }
	|	SLASH_LAYER
			{ __info.attrib = SLASH_LAYER_TYPE; }
	|	SLASH_PEN
			{ __info.attrib = SLASH_PEN_TYPE; }
	|	SLASH_STYLE
			{ __info.attrib = SLASH_STYLE_TYPE; }
	;

/*
 *     +++
 *     +++
 *  +++++++++	Load file
 *  +++++++++
 *     +++
 *     +++
 */

loadfile :	LOAD fileswitch file_name fileswitch 
			{ DM_LoadFile($3, __info); }
	|	LOAD file_name fileswitch 
			{ DM_LoadFile($2, __info); }
	|	LOAD fileswitch file_name
			{ DM_LoadFile($3, __info); }
	|	LOAD file_name
			{ DM_LoadFile($2, __info); }
	;
	
/*
 *     +++
 *     +++
 *  +++++++++	Save File
 *  +++++++++
 *     +++
 *     +++
 */

savefile :	SAVE fileswitch file_name fileswitch
			{ DM_SaveFile($3, __info); }
	|	SAVE file_name fileswitch
			{ DM_SaveFile($2, __info); }
	|	SAVE fileswitch file_name
			{ DM_SaveFile($3, __info); }
	|	SAVE file_name
			{ DM_SaveFile($2, __info); }
	;

	
/*
 *     +++
 *     +++
 *  +++++++++	Merge files
 *  +++++++++
 *     +++
 *     +++
 */

	
mergefile :	MERGE mergeswitch file_name mergeswitch
			{ DM_MergeFile($3, __info); }
	|	MERGE file_name mergeswitch
			{ DM_MergeFile($2, __info); }
	|	MERGE mergeswitch file_name
			{ DM_MergeFile($3, __info); }
	|	MERGE file_name
			{ DM_MergeFile($2, __info); }
	;

mergeswitch :	fileswitch
	|	mergeswitch mergeswitch
	|	SLASH_ROOT
			{ __info.where = SLASH_ROOT_TYPE; }
	|	SLASH_CURRENT
			{ __info.where = SLASH_CURRENT_TYPE; }
	;

/*
 *     +++
 *     +++
 *  +++++++++	Use Files
 *  +++++++++
 *     +++
 *     +++
 */

usefile :	USE fileswitch file_name fileswitch
			{ DM_UseFile($3, __info); }
	|	USE file_name fileswitch
			{ DM_UseFile($2, __info); }
	|	USE fileswitch file_name
			{ DM_UseFile($3, __info); }
	|	USE file_name
			{ DM_UseFile($2, __info); }
	;


/*
 *     +++
 *     +++
 *  +++++++++	new file
 *  +++++++++
 *     +++
 *     +++
 */


new :		NEW fileswitch
			{ DM_NewBuffer(__info); }
	|	NEW 
			{ DM_NewBuffer(__info); }
	;

/*
 *     +++
 *     +++
 *  +++++++++	Set current group,layer,pen,style
 *  +++++++++
 *     +++
 *     +++
 */


setcurrent :	SET CURRENT attribswitch attrib_name attribswitch
			{ DM_SetCurrent($4, __info); }
	|	SET CURRENT attrib_name attribswitch
			{ DM_SetCurrent($3, __info); }
	|	SET CURRENT attribswitch attrib_name
			{ DM_SetCurrent($4, __info); }
	;

/*
 *     +++
 *     +++
 *  +++++++++	Set attributes
 *  +++++++++
 *     +++
 *     +++
 */


template :	SLASH_TEMPLATE EQUALS attrib_name
			{ __info.template = $3; }
	;

pattribs :	cattribs
	|	pattribs pattribs
	|	hardcolor
	;

hardcolor :	SLASH_HARDCOLOR EQUALS hexnumber
			{ __info.atflag.hardcolor = $3; }
	;

gattribs :	cattribs
	|	gattribs gattribs
	|	SLASH_ASSEMBLY
			{ __info.atflag.assembly = 2; }
	|	SLASH_NOASSEMBLY
			{ __info.atflag.assembly = 1; }
	;

cattribs :	infoflags
	|	cattribs cattribs
	|	template
	|	commentswitch
	|	SLASH_LOCK
			{ __info.atflag.lock = 2; }
	|	SLASH_NOLOCK
			{ __info.atflag.lock = 1; }
	|	SLASH_GHOST
			{ __info.atflag.ghost = 2; }
	|	SLASH_NOGHOST
			{ __info.atflag.ghost = 1; }
	|	SLASH_PLOT
			{ __info.atflag.plot = 2; }
	|	SLASH_NOPLOT
			{ __info.atflag.plot = 1; }
	|	SLASH_DISPLAY
			{ __info.atflag.display = 2; }
	|	SLASH_NODISPLAY
			{ __info.atflag.display = 1; }
	|	colorswitch
	;
	
commentswitch :	SLASH_COMMENT EQUALS string
			{ __info.atflag.comment = $3; }
	;
	
colorswitch :	SLASH_COLOR EQUALS color_number
			{ __info.atflag.color = $3; }
	;
	
sattribs :	infoflags
	|	sattribs sattribs
	|	commentswitch
	|	template
	|	fontswitch
	|	heightswitch
	|	sheetswitch
	|	slantswitch
	|	aspectswitch
	;
	
fontswitch :	SLASH_FONT EQUALS attrib_name
			{ __info.atflag.font = $3; }
	;
	
heightswitch :	SLASH_HEIGHT EQUALS linear_unit
			{ __info.atflag.height = $3; }
	;
	
sheetswitch :	SLASH_SHEET EQUALS linear_unit
			{ __info.atflag.sheet = $3; }
	;
	
slantswitch :	SLASH_SLANT EQUALS angular_unit
			{ __info.atflag.slant = $3; }
	;
	
aspectswitch:	SLASH_ASPECT EQUALS float_val
			{ __info.atflag.aspect = $3; }
	;

setattrib :	SET attribtype
			{ DM_SetAttrib($2, __info); }
	;
	
attribtype:	GROUP gattribs attrib_name gattribs
			{ $$ = $3;
			  __info.maintype = GROUP_TYPE; }
	|	GROUP gattribs attrib_name
			{ $$ = $3;
			  __info.maintype = GROUP_TYPE; }
	|	GROUP attrib_name gattribs
			{ $$ = $2;
			  __info.maintype = GROUP_TYPE; }
	|	GROUP attrib_name
			{ $$ = $2;
			  __info.maintype = GROUP_TYPE; }
	|	LAYER cattribs attrib_name cattribs
			{ $$ = $3;
			  __info.maintype = LAYER_TYPE; }
	|	LAYER cattribs attrib_name 
			{ $$ = $3;
			  __info.maintype = LAYER_TYPE; }
	|	LAYER attrib_name cattribs
			{ $$ = $2;
			  __info.maintype = LAYER_TYPE; }
	|	LAYER attrib_name
			{ $$ = $2;
			  __info.maintype = LAYER_TYPE; }
	|	PEN pattribs attrib_name pattribs
			{ $$ = $3;
			  __info.maintype = PEN_TYPE; }
	|	PEN pattribs attrib_name 
			{ $$ = $3;
			  __info.maintype = PEN_TYPE; }
	|	PEN attrib_name pattribs
			{ $$ = $2;
			  __info.maintype = PEN_TYPE; }
	|	PEN attrib_name
			{ $$ = $2;
			  __info.maintype = PEN_TYPE; }
	|	STYLE sattribs attrib_name sattribs
			{ $$ = $3;
			  __info.maintype = STYLE_TYPE; }
	|	STYLE sattribs attrib_name
			{ $$ = $3;
			  __info.maintype = STYLE_TYPE; }
	|	STYLE attrib_name sattribs
			{ $$ = $2;
			  __info.maintype = STYLE_TYPE; }
	|	STYLE attrib_name
			{ $$ = $2;
			  __info.maintype = STYLE_TYPE; }
	;

/*
 *     +++
 *     +++
 *  +++++++++	Create any given attribute
 *  +++++++++
 *     +++
 *     +++
 */

createattrib :	CREATE attribtype
			{ DM_CreateAttrib($2, __info); }
	;

/*
 *     +++
 *     +++
 *  +++++++++	Attribute bookkeeping
 *  +++++++++
 *     +++
 *     +++
 */
			
changes : 	RENAME  attribswitch attrib_name attribswitch 
				attrib_name attribswitch
			{ DM_RenameAttrib($3, $5, __info); }
	|	RENAME  attrib_name attribswitch 
				attrib_name attribswitch
			{ DM_RenameAttrib($2, $4, __info); }
	|	RENAME  attribswitch attrib_name  
				attrib_name attribswitch
			{ DM_RenameAttrib($3, $4, __info); }
	|	RENAME  attribswitch attrib_name attribswitch 
				attrib_name 
			{ DM_RenameAttrib($3, $5, __info); }
	|	RENAME  attrib_name
				attrib_name attribswitch
			{ DM_RenameAttrib($2, $3, __info); }
	|	RENAME  attribswitch attrib_name 
				attrib_name
			{ DM_RenameAttrib($3, $4, __info); }
	|	RENAME  attrib_name attrib_name
			{ DM_RenameAttrib($2, $3, __info); }
	|	RENAME  attrib_name attribswitch 
				attrib_name 
			{ DM_RenameAttrib($2, $4, __info); }
	| 	MERGE  attribswitch attrib_name attribswitch 
				attrib_name attribswitch
			{ DM_MergeAttrib($3, $5, __info); }
	|	MERGE  attrib_name attribswitch 
				attrib_name attribswitch
			{ DM_MergeAttrib($2, $4, __info); }
	|	MERGE  attribswitch attrib_name  
				attrib_name attribswitch
			{ DM_MergeAttrib($3, $4, __info); }
	|	MERGE  attribswitch attrib_name attribswitch 
				attrib_name 
			{ DM_MergeAttrib($3, $5, __info); }
	|	MERGE  attrib_name
				attrib_name attribswitch
			{ DM_MergeAttrib($2, $3, __info); }
	|	MERGE  attribswitch attrib_name 
				attrib_name
			{ DM_MergeAttrib($3, $4, __info); }
	|	MERGE  attrib_name attrib_name
			{ DM_MergeAttrib($2, $3, __info); }
	|	MERGE  attrib_name attribswitch 
				attrib_name 
			{ DM_MergeAttrib($2, $4, __info); }
	|	EXPUNGE attribswitch 
			{ DM_ExpungeAttrib(__info); }
	|	EXPUNGE
			{ DM_ExpungeAttrib(__info); }
	|	DELETE  attribswitch attrib_name attribswitch
			{ DM_DeleteAttrib($3, __info); }
	|	DELETE  attrib_name attribswitch
			{ DM_DeleteAttrib($2, __info); }
	|	DELETE  attribswitch attrib_name
			{ DM_DeleteAttrib($3, __info); }
	|	DELETE  attrib_name
			{ DM_DeleteAttrib($2, __info); }
	;

/*
 *     +++
 *     +++
 *  +++++++++	Set color register
 *  +++++++++
 *     +++
 *     +++
 */

setcolor :	SET COLOR infoflags color_number infoflags hexnumber infoflags
			{ DM_SetColor($4,$6); }
	|	SET COLOR infoflags color_number infoflags hexnumber 
			{ DM_SetColor($4,$6); }
	|	SET COLOR infoflags color_number infoflags hexnumber infoflags
			{ DM_SetColor($4,$6); }
	|	SET COLOR infoflags color_number hexnumber
			{ DM_SetColor($4,$5); }
	|	SET COLOR infoflags color_number infoflags hexnumber infoflags
			{ DM_SetColor($4,$6); }
	|	SET COLOR infoflags color_number infoflags hexnumber
			{ DM_SetColor($4,$6); }
	|	SET COLOR infoflags color_number infoflags hexnumber infoflags
			{ DM_SetColor($4,$6); }
	|	SET COLOR color_number hexnumber
			{ DM_SetColor($3,$4); }
	;

/*
 *     +++
 *     +++
 *  +++++++++	The Line Command
 *  +++++++++
 *     +++
 *     +++
 */

first_piece :	abs_coord
		{ DM_StartLine($1); }

line_piece :	line_piece line_piece
	|	rel_coord
		{ DM_MoreLine($1); }
	|	abs_angle
		{ DM_MoreLine($1); }
	|	rel_angle
		{ DM_MoreLine($1); }
	|	abs_coord
		{ DM_MoreLine($1); }
	;

line :		LINE first_piece line_piece
			{ }
	|	LINE SLASH_CONT line_piece
			{ }
	;

/*
 *     +++
 *     +++
 *  +++++++++	The Ellipse command
 *  +++++++++
 *     +++
 *     +++
 */

ellipsw :	ellipsw ellipsw
	|	SLASH_START EQUALS angular_unit
		{ __obj.sa = $3; }
	|	SLASH_END EQUALS angular_unit
		{ __obj.ea = $3; }
	|	SLASH_ROTATE EQUALS angular_unit
		{ __obj.rot = $3; }
	;
	
ellipse :	ELLIPSE lin_pair COMMA linear_unit
		{ __obj.x = $2.x;
		  __obj.y = $2.y;
		  __obj.xr = $4;
		  __obj.yr = $4;
		  DM_Ellipse(__obj); }
	|	ELLIPSE lin_pair COMMA linear_unit ellipsw
		{ __obj.x = $2.x;
		  __obj.y = $2.y;
		  __obj.xr = $4;
		  __obj.yr = $4;
		  DM_Ellipse(__obj); }
	|	ELLIPSE lin_pair COMMA linear_unit COMMA linear_unit
		{ __obj.x = $2.x;
		  __obj.y = $2.y;
		  __obj.xr = $4;
		  __obj.yr = $6;
		  DM_Ellipse(__obj); }
	|	ELLIPSE lin_pair COMMA linear_unit COMMA linear_unit ellipsw
		{ __obj.x = $2.x;
		  __obj.y = $2.y;
		  __obj.xr = $4;
		  __obj.yr = $6;
		  DM_Ellipse(__obj); }
	;
	

/*
 *     +++
 *     +++
 *  +++++++++	The Hatch Command
 *  +++++++++
 *     +++
 *     +++
 */

hatchsw :	hatchsw hatchsw
	|	SLASH_FILE EQUALS attrib_name
		{ __obj.file = $3; }
	|	SLASH_PATTERN EQUALS attrib_name	
		{ __obj.pattern = $3; }
	|	SLASH_ROTATE EQUALS angular_unit
		{ __obj.rot = $3; }
	|	SLASH_SCALE EQUALS float_val
		{ __obj.scale = $3; }
	;
	
hatch :		HATCH SLASH_FILL
		{ DM_HatchFill(); }
	|	HATCH hatchsw
		{ DM_HatchPattern(__obj); }
	;

/*
 *     +++
 *     +++
 *  +++++++++	The text command
 *  +++++++++
 *     +++
 *     +++
 */

textsw :	textsw textsw
	|	SLASH_FLIP EQUALS decnumber
		{ __obj.flags |= ($3 & 0x7); }
	| 	SLASH_STYLE EQUALS attrib_name
		{ __obj.stylename = $3; }
	|	SLASH_ROTATE EQUALS angular_unit
		{ __obj.rot = $3; }
	|	SLASH_HANDLE EQUALS decnumber
		{ __obj.flags &= ~(3<<8);	/* Mask off */
		  __obj.flags |= ($3 & 0x3) <<8; }
	;
	
text :		TEXTC lin_pair string
		{ __obj.x = $2.x;
		  __obj.y = $2.y;
		  __obj.tstring = $3;
		  DM_Text(__obj); }
	|	TEXTC lin_pair string textsw
		{ __obj.x = $2.x;
		  __obj.y = $2.y;
		  __obj.tstring = $3;
		  DM_Text(__obj); }

/*
 *     +++
 *     +++
 *  +++++++++	The take command
 *  +++++++++
 *     +++
 *     +++
 */

take :		TAKE infoflags file_name infoflags
			{ DM_TakeFile(__info, $3); }
	|	TAKE infoflags file_name
			{ DM_TakeFile(__info, $3); }
	|	TAKE file_name infoflags
			{ DM_TakeFile(__info, $2); }
	|	TAKE file_name
			{ DM_TakeFile(__info, $2); }
	;

/*
 *     +++
 *     +++
 *  +++++++++	The exit command
 *  +++++++++
 *     +++
 *     +++
 */

exitout : 	EXIT SLASH_QUIT decnumber SLASH_QUIT
			{ DM_Exit(1,$3); }
	|	EXIT SLASH_QUIT decnumber
			{ DM_Exit(1,$3); }
	|	EXIT decnumber SLASH_QUIT
			{ DM_Exit(1,$2); }
	|	EXIT decnumber
			{ DM_Exit(0,$2); }
	|	EXIT SLASH_QUIT
			{ DM_Exit(1,0); }
	|	EXIT
			{ DM_Exit(0,0); }
	;
			
/*
 *     +++
 *     +++
 *  +++++++++	Set variable value command
 *  +++++++++
 *     +++
 *     +++
 */

var_value :	VARIABLE
			{ $$ = Stostring1($1); }
	;

variable :	var_value EQUALS undif_unit
			{ DM_SetVariable($1, $3); }

/*
 *     +++
 *     +++
 *  +++++++++	Basic Clip commands
 *  +++++++++
 *     +++
 *     +++
 */

clip :
		CLIP SLASH_DRAG SLASH_OFFSET EQUALS lin_pair
			{ DM_ClipDrag($5.x,$5.y); }
	|	CLIP SLASH_CLONE SLASH_FROM EQUALS lin_pair 
			SLASH_TO EQUALS lin_pair
			{ DM_ClipCloneFrom($5.x,$5.y);
			  DM_ClipCloneTo($8.x,$8.y); }
	|	CLIP SLASH_CLONE SLASH_TO EQUALS lin_pair 
			SLASH_FROM EQUALS lin_pair
			{ DM_ClipCloneFrom($8.x,$8.y);
			  DM_ClipCloneTo($5.x,$5.y); }
	|	CLIP SLASH_CLONE SLASH_FROM EQUALS lin_pair 
			{ DM_ClipCloneFrom($5.x,$5.y); }
	|	CLIP SLASH_CLONE SLASH_TO EQUALS lin_pair 
			{ DM_ClipCloneTo($5.x,$5.y); }
	|	CLIP SLASH_SCALE SLASH_STEADY EQUALS lin_pair
			SLASH_FACTOR EQUALS linear_unit
			{ DM_ClipScale($5.x,$5.y,$8); }
	|	CLIP SLASH_SCALE SLASH_FACTOR EQUALS linear_unit
			SLASH_SCALE EQUALS lin_pair
			{ DM_ClipScale($8.x,$8.y,$5); }
	|	CLIP SLASH_ROTATE SLASH_STEADY EQUALS lin_pair
			SLASH_ANGLE EQUALS angular_unit
			{ DM_ClipRotate($5.x,$5.y,$8); }	
	|	CLIP SLASH_ROTATE SLASH_ANGLE EQUALS angular_unit
			SLASH_STEADY EQUALS lin_pair
			{ DM_ClipRotate($8.x,$8.y,$5); }	
	|	CLIP SLASH_FLIP SLASH_X EQUALS linear_unit
			{ DM_ClipFlipX($5); }
	|	CLIP SLASH_FLIP SLASH_X SLASH_CENTER EQUALS lin_pair
			{ DM_ClipFlipX($6.x); }
	|	CLIP SLASH_FLIP SLASH_Y EQUALS linear_unit
			{ DM_ClipFlipY($5); }
	|	CLIP SLASH_FLIP SLASH_Y SLASH_CENTER EQUALS lin_pair
			{ DM_ClipFlipY($6.y); }
	;
				  			  
/*
echo :		ECHO SLASH_NOTERM string
			{ DM_Echo(FALSE,$3); }
	|	ECHO string
			{ DM_Echo{TRUE,$2); }
	|	ECHO SLASH_SCREEN string
			{ DM_EchoScreen($3); }
	;

hatch :		HATCH SLASH_FILL fill_flags
	|	HATCH pattern_flags
	;
	
HATCH	
	/FILL
	/SOLID
	/FILE=patternfile
	/PATTERN=pattername
	/ROTATE=x
	/SCALE=x
	
	/PREVIOUS
	/SELECT

ECHO	"bla"
	/NOTERM
	/SCREEN

BIND
	/DIALOG=filename
	/AREXX=filename
	/INTERNAL=XXX
		/RAWKEY=x
		/KEY=x
		/ALT
		/CONTROL
		/LEFT
		/RIGHT


----------------------------------String Substitution Commands

$VARIABLE

	$$BOUND_X_MIN		<FLOAT-LIN>
	$$BOUND_Y_MIN		<FLOAT-LIN>
	$$BOUND_X_MAX		<FLOAT-LIN>
	$$BOUND_Y_MAX		<FLOAT-LIN>
	
	$$SNAP_X		<FLOAT-LIN>
	$$SNAP_Y		<FLOAT-LIN>
	$$OFFSET_X		<FLOAT-LIN>
	$$OFFSET_Y		<FLOAT-LIN>
	
	$$GRID_X		<FLOAT-LIN>
	$$GRID_Y		<FLOAT-LIN>
	$$GRID_OFFSET_X		<FLOAT-LIN>
	$$GRID_OFFSET_Y		<FLOAT-LIN>
	$$GRID_X_DIV		<INT>
	$$GRID_Y_DIV		<INT>
	
	$$Q_TEXT		<INT>
	
	$$PAPER_UNIT		<FLOAT>
	$$VIEW_UNIT		<FLOAT>
	
	$$ARROW_ANGLE		<FLOAT-ANG>
	$$ARROW_LENGTH		<FLOAT-ANG>
	
	$$LINEAR_TOL		<FLOAT-LIN>
	$$LINEAR_POINTS		<INT>
	$$ANGULAR_TOL		<FLOAT-ANG>
	$$ANGULAR_POINTS	<INT>
	
	$$CENTER_ON		<BOOL>
	$$COORDS_ON		<BOOL>
	$$DRAW_INFO		<BOOL>
	$$GRID_ON		<BOOL>
	$$BOUNDS_ON		<BOOL>
	$$ORTHO_SWITCH		<BOOL>
	$$Q_HATCH		<BOOL>
	
	$$LINEAR_UNITS		<INT>
	$$ANGULAR_UNITS		<INT>
	
	$$TEMP_Y_ORIGIN		<FLOAT-LIN>
	$$TEMP_X_ORIGIN		<FLOAT-LIN>
	
	$$DIM_WIT_PICK		<FLOAT-LIN>
	$$DIM_WIT_OVER		<FLOAT-LIN>
	$$DIM_BOX_OFFSET	<FLOAT-LIN>
	$$DIM_USE_FIXED		<BOOL>
	$$DIM_FIXED_ANGLE	<FLOAT-LIN>
	$$DIM_FLIP_FROM		<FLOAT-LIN>
	$$DIM_FLIP_TO		<FLOAT-LIN>
	$$DIM_FREE_OFFSET	<FLOAT-LIN>	

$$INTERNAL

F$FUNCTION

	F$FIND_ENDPOINT		<COORD>
	F$FIND_MIDPOINT		<COORD>
	F$FIND_CENTER		<COORD>
	F$FIND_INTER		<COORD>
	F$FIND_2INTER		<COORD>
	F$MOUSE			<COORD>
	

G$FUNCTION(arg1,arg)

	G$COORD_TO_X(arg)	<FLOAT> = <COORD>
	G$COORD_TO_Y(arg)	<FLOAT> = <COORD>
	G$FLT_TO_COORD(argx,argy) <COORD> = <FLOAT>,<FLOAT>

	



	
<MOUSE_COORDS(ident,query)>	returns "x,y" in current units
	
<MOUSE_COORDS_INT(ident,query)>	returns "x,y" in internal unit (mm's)

<GET_VARIABLE(ident)>		returns "x" in current units (if has units)

<GET_VARIABLE_INT(ident)>	returns "x" in internal units

<GET_FLAG(ident)>		returns "0 or 1"

<LINEAR_UNIT(value)>		convert internal unit (mm) to current unit

<ANGULAR_UNIT(value)>		convert internal angle (rad) to current angle

@		gets mouse input, prompts with current field name

VARIABLE = VALUE
FLAG = ON/OFF

---------------------------------------------------------------------------

SET COLOR
	/STYLE=RGB
	/STYLE=HLS
	/STYLE=GREY

SET/ORIGIN x,y
	/RELATIVE
	/TEMPORARY
	/PERMANENT

[UN]CLIP/SELECTED

[UN]CLIP/BOX
	/RECTANGLE
		/INSIDE
		/ALL
		/OUTSIDE
	/GROUP=wildname
	/LAYER=
	/PEN=
	/STYLE=
	/TEXT
	/LINES
	/ELLIPSES
	/HATCHING
	/ONSCREEN
	/OFFSCREEN
	

ZOOM
	/FULL
	/OUT=x
	/IN=x
	
	/WIDTH=x
	/HEIGHT=x
	/CENTER=x,y
	/LLCORNER=x,y
	/RLCORNER=x,y
	/LUCORNER=x,y
	/RUCORNER=x,y

SHOW
DIR
	/GROUP wildname
	/STYLE 
	/PEN 
	/LAYER 
	/SELECTED
	/CLIP
	/VARIABLE wildname
	/FLAG wildname
	/BINDINGS
	/[NO]PRINT
	/OUTPUT=file_name
	/APPEND=file_name


*/

%%

#include "lex.yy.c"

