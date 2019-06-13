/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	pools.c - free and used memory pool manager
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
 *	2	16-May-87 SH	Lobotomize and start over
 *	3	31-May-87 SH	More rewriting
 *	4	 1-Aug-87 SH	Update dirty flag
 *	5	16-Aug-87 SH	Do our own frees now, don't use remalloc
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include <exec/memory.h>

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

 
/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

VOID FreeOnlyDraw()
{
	D_NODE *prvnod;

	while (Last_Node) {
		prvnod = Last_Node->node_previous;
		FreeMem(Last_Node, Last_Node->node_length);
		Last_Node = prvnod;
	}
		
	Root_Node = NULL;
	Last_Node = NULL;

	PoolsSome=FALSE;
	return;
}

VOID FreeAllPools()
{
	D_NODE *prvnod;

	if (!PoolsSome) return;

	while (Last_Node) {
		prvnod = Last_Node->node_previous;
		FreeMem(Last_Node, Last_Node->node_length);
		Last_Node = prvnod;
	}
		
	Root_Node = NULL;
	Last_Node = NULL;

	GI_FreeDraw();			/* Other things are stored this way */
	PoolsSome=FALSE;
	return;
}

D_NODE *CreateObjNode(size,createstyle)
UWORD size;
UWORD createstyle;
{

	UWORD fullsize;
	D_NODE *tempnode;

	fullsize=(sizeof (D_NODE)) + size;

	tempnode=(D_NODE *) AllocMem(fullsize,0);
	PoolsSome=TRUE;

	if (tempnode==NULL) return(NULL);

	tempnode->node_id=createstyle; 
	tempnode->node_flags = 0;
	tempnode->node_length=fullsize;

	drawingdirty = TRUE;		/* Drawing object added */

	return(tempnode);		
}

VOID FixCurrents()
{

	if (Current_Group==NULL) {
		printf("Group was found to be null\n");
		Current_Group = Root_Group;
	}
	
	if (Current_Layer==NULL) {
		printf("Layer was found to be null\n");
		Current_Layer = Root_Layer;
	}
	
	if (Current_Pen==NULL) {
		printf("Pen was found to be null\n");
		Current_Pen = Root_Pen;
	}
}

/*
 *	void LinkIntoGroup(D_NODE *)
 *
 *	Link this new item into the current group
 *
 */

VOID LinkIntoGroup(newstr)
D_NODE *newstr;
{

	FixCurrents();

	newstr->above_group = Current_Group;
	newstr->node_layer = Current_Layer;
	newstr->pen_attach = Current_Pen;

	newstr->above_group->Use_Count++;
	newstr->node_layer->Use_Count++;
	newstr->pen_attach->Use_Count++;

	newstr->node_subsequent = NULL;
	newstr->node_previous = NULL;
	
	if (Last_Node==NULL)	Root_Node = Last_Node = newstr;
	else {
		Last_Node->node_subsequent = newstr;
		newstr->node_previous = Last_Node;
		Last_Node = newstr;
	}
	
	return;
}
	
/*
 *	void SpliceIntoGroup(D_NODE *,D_NODE *)
 *
 *	Splice this new item into the current group
 *
 */

VOID SpliceIntoGroup(newstr,inpoint)
D_NODE *newstr;
D_NODE *inpoint;
{

	D_NODE *left,*middle,*right;

	if (inpoint == NULL) {
		LinkIntoGroup(newstr);
		return;
	}

	FixCurrents();

	newstr->above_group = Current_Group;
	newstr->node_layer = Current_Layer;
	newstr->pen_attach = Current_Pen;

	newstr->above_group->Use_Count++;
	newstr->node_layer->Use_Count++;
	newstr->pen_attach->Use_Count++;

	newstr->node_subsequent = NULL;
	newstr->node_previous = NULL;
	
	/* I need to do this to make it clear */
	
	left = inpoint->node_previous;
	middle = newstr;
	right = inpoint;
	
	if (left==NULL) {	/* New Root (no left node) */
		Root_Node = middle;
		middle->node_previous = NULL;
		middle->node_subsequent = right;
		right->node_previous = middle;
	} else {				/* Splice in */
		left->node_subsequent = middle;
		middle->node_previous = left;
		middle->node_subsequent = right;
		right->node_previous = middle;
	}
	
	return;
}

VOID SnipObject(object,root,tail,account)
D_NODE *object,**root,**tail;
BOOL account;
{

	if (object->node_subsequent!=NULL) {
		if (object->node_previous!=NULL) {
			object->node_previous->node_subsequent=
				object->node_subsequent;
			object->node_subsequent->node_previous=
				object->node_previous;
		} else {
			*root=object->node_subsequent;
			object->node_subsequent->node_previous=NULL;
		}
	} else {
		if (object->node_previous!=NULL) {
			object->node_previous->node_subsequent=NULL;
			*tail=object->node_previous;
		} else {
			*root = *tail = NULL;
		}
	}

	if (account) {

		object->above_group->Use_Count--;
		object->node_layer->Use_Count--;
		object->pen_attach->Use_Count--;

		if (object->node_id==NODE_TEXT)
			GetTextData(object)->letstyle->Use_Count--;
	}

}

VOID SnipObjectMain(obj,acc)
D_NODE *obj;
BOOL acc;
{
	SnipObject(obj,&Root_Node,&Last_Node,acc);
}

VOID AddToRoot(object,root,tail,account)
D_NODE *object,**root,**tail;
BOOL account;
{

	if (account) {

		object->above_group->Use_Count++;
		object->node_layer->Use_Count++;
		object->pen_attach->Use_Count++;
		
	}

	object->node_subsequent = NULL;
	object->node_previous = NULL;
	
	/* I need to do this to make it clear */
	
	if (*root==NULL) *root = *tail = object;
	else {
		object->node_subsequent = *root;
		(*root)->node_previous = object;
		*root = object;
	}
}	

VOID AddToEnd(object,root,tail,account)
D_NODE *object,**root,**tail;
BOOL account;
{

	if (account) {

		object->above_group->Use_Count++;
		object->node_layer->Use_Count++;
		object->pen_attach->Use_Count++;
		
	}

	object->node_subsequent = NULL;
	object->node_previous = NULL;
	
	/* I need to do this to make it clear */
	
	if (*root==NULL) *root = *tail = object;
	else {
		(*tail)->node_subsequent = object;
		object->node_previous = *tail;
		*tail = object;
	}
}	

D_NODE *CopyNode(innode)
D_NODE *innode;
{

	D_NODE *tempnode;

	tempnode=(D_NODE *) AllocMem(innode->node_length,0);
	if (tempnode==NULL) return(NULL);

	movmem((VOID *) innode, (VOID *) tempnode, innode->node_length);
	
	tempnode->node_subsequent = NULL;
	tempnode->node_previous = NULL;
	
	return(tempnode);
}

/*
 *	D_NODE *StoStrLine(O_LINE *)
 *
 *	Take the data proffered by the calling subroutine and place
 *	it along with a nodeptr structure into the next available
 *	pool storage space
 *
 */

D_NODE *StoStrLine(inline)
O_LINE *inline;
{

	D_NODE *newstr;

	newstr=CreateObjNode(sizeof(O_LINE),NODE_LINE);

	*(GetLineData(newstr)) = *inline;	

	LinkIntoGroup(newstr);

	return(newstr);

}

/*
 *	D_NODE *StoStrEllip(O_ELLIP *)
 *
 *	Take the data proffered by the calling subroutine and place
 *	it along with a nodeptr structure into the next available
 *	pool storage space
 *
 */

D_NODE *StoStrEllip(inline)
O_ELLIP *inline;
{

	D_NODE *newstr;

	newstr=CreateObjNode(sizeof(O_ELLIP),NODE_ELLIP);

	*(GetEllipData(newstr)) = *inline;	

	LinkIntoGroup(newstr);

	return(newstr);

}


/*
 *	D_NODE *StoStrText(O_TEXT *)
 *
 *	Take the data proffered by the calling subroutine and place
 *	it along with a nodeptr structure into the next available
 *	pool storage space
 *
 */

D_NODE *StoStrText(inline)
O_TEXT *inline;
{

	D_NODE *newstr;

	newstr=CreateObjNode(sizeof(O_TEXT),NODE_TEXT);

	*(GetTextData(newstr)) = *inline;	

	LinkIntoGroup(newstr);

	inline->letstyle->Use_Count++;

	return(newstr);

}

/*
 *	D_NODE *StoStrHatch(O_HATCH *)
 *
 *	Take the data proffered by the calling subroutine and place
 *	it along with a nodeptr structure into the next available
 *	pool storage space
 *
 *	Gets spliced into the memory list before point inspoint
 *
 */

D_NODE *StoStrHatch(inline,inspoint)
O_HATCH *inline;
D_NODE *inspoint;
{

	D_NODE *newstr;
	O_HATCH *newhatch;

	newstr=CreateObjNode(sizeof(O_HATCH),NODE_HATCH);

	newhatch = GetHatchData(newstr);
	
	*newhatch = *inline;
		
	SpliceIntoGroup(newstr,inspoint);

	return(newstr);

}
