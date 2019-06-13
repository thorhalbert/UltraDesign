/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	grouphandle.c - group structure mechanics
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
 *	1	10-Nov-88 SH	Pull in stuff from groupreq
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

GROUP_A *Group_Search();

GROUP_A *savelast,*lastassy;
UBYTE GroupPath[300];

/*
 *     +++
 *     +++
 *  +++++++++	Parse down through GroupPath and find the group pointer
 *  +++++++++	Current_Group will have new path
 *     +++
 *     +++
 */


BOOL GroupGet(complain)
BOOL complain;
{

	int a;
	char *fp,*sp;
	UBYTE nbuf[40];
	GROUP_A *srch,*savgrp;

	savgrp = Current_Group;
	Current_Group = Root_Group;
	
	sp = GroupPath;
	while (*sp==' '||*sp=='\t'||*sp=='/') sp++;

	a = 0;
	FOREVER {
		GroupPath[a++] = *sp;
		if (*sp=='\0') break;
		sp++;
	}

	fp = GroupPath;

	while (fp!=NULL) {
		sp = strchr(fp,'/');
		if (sp!=NULL) *sp='\0';
		
		sprintf(nbuf,"%s",fp);
		fp = sp;
		if (fp!=NULL) fp++;

		srch = Group_Search(nbuf);	/* Try to find it */

		if (srch==NULL||(int)srch==2) {
			if (complain)
				oneliner("Illegal Path specified!");
			Current_Group = savgrp;
			setpath();
			return(FALSE);
		}

		Current_Group = srch;

	}

	setpath();
	return(TRUE);		/* Found it */
}

/*
 *     +++
 *     +++
 *  +++++++++	Set the current group to the string
 *  +++++++++
 *     +++
 *     +++
 */

BOOL SearchGroupPath(inpath)
UBYTE *inpath;
{

	strcpy(GroupPath, inpath);
	
	return(GroupGet(FALSE));
}

/*
 *     +++
 *     +++
 *  +++++++++	Given Current_Group, fill in GroupPath
 *  +++++++++
 *     +++
 *     +++
 */

setpath()
{

	GROUP_A *ingp;
	UBYTE	slosh[257];

	GroupPath[0]='\0';

	for (ingp = Current_Group; ingp!=NULL; ingp = ingp->parent) {
		if (ingp->parent==NULL) break;

		if (GroupPath[0]=='\0') 
			sprintf(GroupPath,"%s",ingp->Group_Name);
		else {
			sprintf(slosh,"%s/%s",ingp->Group_Name,GroupPath);
			sprintf(GroupPath,"%s",slosh);
		}
	}

	setassystuff();

}

/*
 *     +++
 *     +++
 *  +++++++++	Scroller Code for Groups (Heirarchical)
 *  +++++++++
 *     +++
 *     +++
 */


GROUP_A *Group_Parent(current_in,inmask,descend)
GROUP_A *current_in;
UBYTE *inmask;
BOOL descend;
{

	if (Current_Group->parent==NULL) return(NULL);

	if (descend) {
		Current_Group = Current_Group->parent;
		setpath();
		return(NULL);
	}

	return((GROUP_A *) 1); /* Flag 1 will specify parent */

}

GROUP_A *Group_Next(current_in,inmask)
GROUP_A *current_in;
UBYTE *inmask;
{

	if ((int) current_in==2) {
		current_in = Current_Group->descendants;
		if (current_in==NULL) return(NULL);
		if (match(inmask,current_in->Group_Name))
			return(current_in);
	}

	if (current_in==NULL) return(NULL);

	FOREVER {
		current_in=current_in->next;
		if (current_in==NULL) return(NULL);

		if (match(inmask,current_in->Group_Name))
			return(current_in);

	}
}

GROUP_A *Group_Prev(current_in,inmask)
GROUP_A *current_in;
UBYTE *inmask;
{

	if ((int) current_in==2) return(NULL);

	FOREVER {
		current_in=current_in->previous;
		if (current_in==NULL) return((GROUP_A *) 2);
	
		if (match(inmask,current_in->Group_Name))
			return(current_in);

	}
}

GROUP_A *Group_First(current_in,inmask,descend)
GROUP_A *current_in;
UBYTE *inmask;
BOOL descend;
{

	if (descend) 
		(VOID) GroupGet();

	current_in = (GROUP_A *) 2;	/* Special flag for top */

	return(current_in);
}

GROUP_A *Group_Search(insearch)
UBYTE *insearch;
{

	GROUP_A *looper;
	char *umat;

	umat = insearch;
	while (*umat==' ') umat++;
	if (umat=='\0') return((GROUP_A *) 2);	/* Default */
	
	if (match(insearch,"Drawing_Objects")) 
		return((GROUP_A *) 2);

	for (looper=Current_Group->descendants; looper!=NULL; 
				looper=looper->next) 
		if (match(insearch,looper->Group_Name)) return(looper);

	return(NULL);

}

GROUP_A *Group_Descend(current_in,inmask,descend)
GROUP_A *current_in;
UBYTE *inmask;
BOOL descend;
{

	if ((int) current_in==2) return(NULL);	/* Here are the objects */

	if (descend) {
		Current_Group = current_in;
		setpath();
		return(NULL);
	}
			
	return((GROUP_A *)(- ((int) current_in)));

}

VOID Group_Form(current_in,outform)
GROUP_A *current_in;
UBYTE *outform;
{

	char odis[10];
	sprintf(odis,"       ");
	if (current_in==NULL) return;

	if (((int) current_in)==1) {
		sprintf(outform,"/ Parent");
		return;
	}

	if (((int) current_in)==2) {
		if (Current_Group->Group_Flags&GROUP_GHOSTED) odis[0]='G';
		if (Current_Group->Group_Flags&GROUP_DOFF) odis[1]='D';
		if (Current_Group->Group_Flags&GROUP_LOCKED) odis[4]='L';
		if (Current_Group->Group_Flags&GROUP_POFF) odis[2]='P';
		if (Current_Group->Libr_Flags&LIBR_ASSY) odis[3]='A';

		sprintf(outform,"  %-32.32s %6d %s",
			"Drawing_Objects",
			Current_Group->Use_Count,odis);	/* Count of objects */
		Scr_Color = Current_Group->Color_ID;
		return;
	}

	if (current_in->Group_Flags&GROUP_GHOSTED) odis[0]='G';
	if (current_in->Group_Flags&GROUP_DOFF) odis[1]='D';
	if (current_in->Group_Flags&GROUP_LOCKED) odis[2]='L';
	if (current_in->Group_Flags&GROUP_POFF) odis[3]='P';
	if (current_in->Libr_Flags&LIBR_ASSY) odis[4]='A';

	sprintf(outform,"  %-32.32s %6d %s",
		current_in->Group_Name,
		current_in->Use_Count,
		odis);

		Scr_Color = current_in->Color_ID;
}

VOID Group_Name(current_in,outform)
GROUP_A *current_in;
UBYTE *outform;
{

	if (current_in==NULL) return;

	if ((int) current_in==2) sprintf(outform,"%s","Drawing_Objects");
	else sprintf(outform,"%s",current_in->Group_Name);

}

/*
 *     +++
 *     +++
 *  +++++++++	Group linked-tree maintenance routines
 *  +++++++++	
 *     +++
 *     +++
 */

int cmpgrnm(in1,in2)
GROUP_A *in1,*in2;
{

	return(asccmp(in1->Group_Name,in2->Group_Name));
}

GROUP_A *Link_Group(inent,inmother)
GROUP_A *inent,*inmother;
{

	GROUP_A *loop;

	inent->parent = inmother;
	inent->next = NULL;
	inent->previous = NULL;
	inent->descendants = NULL;
	inent->desc_last = NULL;

	if (inmother->descendants==NULL) {
		inmother->descendants = inmother->desc_last = inent;
		return(inent);
	}

	for (loop = inmother->descendants; loop!=NULL; loop=loop->next)
		if (cmpgrnm(inent,loop)<0) break;

	if (loop==NULL) {
		inmother->desc_last->next = inent;
		inent->previous = inmother->desc_last;
		inmother->desc_last = inent;
		return(inent);
	}
	
	if (loop->previous==NULL) {
		inmother->descendants = inent;
		loop->previous = inent;
		inent->next = loop;
		return(inent);
	}

	loop->previous->next = inent;
	inent->next = loop;
	inent->previous = loop->previous;
	loop->previous = inent;

	return(inent);
}

GROUP_A *groupinsert(instr,inmother)
GROUP_A instr,*inmother;
{

	GROUP_A *inent;

	inent = (GROUP_A *) 
		GI_DrawAlloc(sizeof(GROUP_A));
	if (inent==NULL) return(NULL);

	*inent = instr;

	Link_Group(inent,inmother);

	return(inent);
}

Unlink_Group(ingroup)
GROUP_A *ingroup;
{
	GROUP_A *inmother;

	inmother = ingroup->parent;

	if (ingroup->next!=NULL) {
		if (ingroup->previous!=NULL) {
			ingroup->previous->next=ingroup->next;
			ingroup->next->previous=ingroup->previous;
		} else {
			inmother->descendants=ingroup->next;
			ingroup->next->previous=NULL;
		}
	} else {
		if (ingroup->previous!=NULL) {
			ingroup->previous->next=NULL;
			inmother->desc_last=ingroup->previous;
		} else {
			inmother->descendants = inmother->desc_last = NULL;
		}
	}
}

UBYTE *StoreComment(instr)
UBYTE *instr;
{

	UBYTE 	*ost;
	int	onum;

	if (instr==NULL) onum = 1;
	else	onum = strlen(instr)+1;

	ost = GI_DrawAlloc(onum);
	if (ost==NULL) return(NULL);

	if (instr==NULL) ost[0]='\0';
	else	sprintf(ost,"%s",instr);

	return(ost);
}


/*
 *     +++
 *     +++
 *  +++++++++	Set flags to determine which parts of the group tree
 *  +++++++++	are in the current assembly (used in libraries)
 *     +++
 *     +++
 */

grpclear(ingrp,set)
GROUP_A *ingrp;
BOOL set;
{

	for (; ingrp!=NULL; ingrp = ingrp->next) {
		ingrp->cursel = set;
		if (ingrp->descendants!=NULL) grpclear(ingrp->descendants,set);
	}
}

setassystuff()
{

	GROUP_A *bckdsc;

	if (Root_Group->Libr_Flags & LIBR_ASSY) 
		Root_Group->Libr_Flags &= (~LIBR_ASSY);	/* Can't be */

	grpclear(Root_Group,FALSE);

	lastassy = NULL;

	for (bckdsc = Current_Group; bckdsc!=NULL; bckdsc = bckdsc->parent) 
		if (bckdsc->Libr_Flags & LIBR_ASSY) lastassy = bckdsc;

	if (lastassy) {
		lastassy->cursel = TRUE;
		grpclear(lastassy->descendants,TRUE);
	}
			
}

VOID CountBranch(ingroup,grpcount,objcount)
GROUP_A *ingroup;
int *grpcount,*objcount;
{

	GROUP_A *looper;

	if (ingroup==NULL) return;

	*grpcount += 1;
	*objcount += ingroup->Use_Count;

	for (looper=ingroup->descendants; looper!=NULL; looper=looper->next)
		CountBranch(looper,grpcount,objcount);

	return;
}

