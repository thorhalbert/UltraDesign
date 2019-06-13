#include <intuition/intuition.h>

/* amigahooks.h */

struct GI_sgadget {
	SHORT	BorderArray[10];	
	WORD	GadgetClass;
	WORD	GadgetID;
	struct GI_sgadget *nextsgadget;
	UBYTE	*UndoBuffer;
	UBYTE	*StorageBuffer;
	UBYTE	*prefixtext;
	WORD	maxsize;
	struct IntuiText preftextintui;
	struct Border dispbox;
	struct Gadget amigagadget;
	struct StringInfo amigastrings;
	BOOL	(*servicefun)();
	BOOL	activated;
	BOOL	enabled;
	struct Window *thiswindow;
};
struct GI_bgadget {
	SHORT	BorderArray[10];	
	WORD	GadgetClass;
	WORD	GadgetID;
	struct GI_bgadget *nextbgadget;
	UBYTE	*bodyfixtext;
	WORD	maxsize;
	struct IntuiText bodytextintui;
	struct Border dispbox;
	struct Gadget amigagadget;
	BOOL	(*servicefun)();
	BOOL	activated;
	BOOL	enabled;
	struct Window *thiswindow;
};

#define GI_GADGETCENTER 1
#define GI_GADGETLEFT 2
#define GI_GADGETRIGHT 3

#define GI_STRINGGADGET 1
#define GI_BOOLSGADGET	2
#define GI_BUTTONGADGET	3

extern UBYTE *GI_RemAlloc();
