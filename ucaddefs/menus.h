/*-------------------definitions for menus-menus.h--------------------------*/

struct menu_access {
	USHORT level;			/* 1 is a menu level 2 is item 3 sub */
	BOOL	*access;		/* Access to bool toggle */
	BOOL	(*Implement)();		/* Execute function */
	BOOL	activate;		/* Do It? */
	UWORD	qualmask;		/* Qualifier Mask */
	UWORD	keycode;		/* Letter Entered */
	UWORD	excludec;		/* Special exclude code */
	struct MenuItem *menptr;
	WORD	menu,item,sub;
};
