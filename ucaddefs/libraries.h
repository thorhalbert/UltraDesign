/* Libraries definition */

#include "attributes.h"

struct 	libdirect {
	UBYTE	Group_Name[MAXGROUPNAME+1];	/* Name of group */
	LONG	Use_Count;			/* Number of objects */
	UWORD	Color_ID;			/* Group mode coloring */
	WORD	Cur_ID;				/* Grouping pointer for IFF */
	UWORD	Group_Flags;			/* Group flags as above */
	UWORD	Libr_Flags;			/* Library flags if applic */
	UBYTE	Org_Cnt;			/* Count of origin and 
						   handle points if any */
	STDFLT	*Org_Points;			/* Orgcnt*2 points X,Y pairs */

	struct 	libdirect *parent;		/* Parent node of this */
	struct 	libdirect *next;		/* Next sibling on this level */
	struct 	libdirect *previous;		/* Prev sibling */
	struct 	libdirect *descendants;	/* Link to first lower level */
	struct 	libdirect *desc_last;	/* Last descendant */

	BOOL	cursel;				/* Group/desc are selected */

	UWORD	Last_ID;			/* Last number of last desc */

	UBYTE	*Comment;			/* Possible Comment */

	struct 	Group_Attrib	*grp_alias;	/* Pointer to real McCoy */
};

extern	struct	libdirect	*Root_Library,*Current_Library;
