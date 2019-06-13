#include "ucad.h"
#include <exec/memory.h>
#include <ctype.h>

#define rmalloc(s) AllocMem(s,0)
#define rmfree(x,s) FreeMem(x,s)

#define CK(x) if (!x) return(FALSE)

STDFLT lastxelement,lastyelement;

#define numvert 1024*10
#define bufsiz (numvert*sizeof(LONG))

BOOL anythinghappen;

UBYTE *hatchfile;
LONG population;
ULONG *bigtab;	/* Big table for numbers */

FILE *hatfile;

struct Hatch_Header obuf1;
struct Hatch_Entry obuf2;

/*---------------Search for the drawing element boundary box--------------*/

BOOL bboxany;
STDFLT bscale;
STDFLT bboxmaxx,bboxminx,bboxmaxy,bboxminy;

VOID TryBBoxPoint(inx,iny)
STDFLT inx,iny;
{

	bboxany = TRUE;				/* Got action */
	anythinghappen = TRUE;
	
	bboxmaxx = max(bboxmaxx, inx);		/* Find max/min */
	bboxmaxy = max(bboxmaxy, iny);
	
	bboxminx = min(bboxminx, inx);
	bboxminy = min(bboxminy, iny);

}

/* Search through drawing elements for target pieces */

VOID BBoxBounds(ingroup,inlayer)
GROUP_A *ingroup;
LAYER_A *inlayer;
{

	O_LINE *doline;
	
	D_NODE *looper;
	
	/* Eliminate all but proper pieces */
	for (looper=Root_Node; looper!=NULL; looper=looper->node_subsequent) {
		if (looper->above_group!=ingroup) continue;
		if (looper->node_layer!=inlayer) continue;
	
		if (looper->node_id!=NODE_LINE) continue;
		
		doline = GetLineData(looper);
		
		TryBBoxPoint(doline->x1,doline->y1);
		TryBBoxPoint(doline->x2,doline->y2);
	}
}

/* Look through layers for HATCH layer */
				
BOOL HatchBoundingBox(ingroup)
GROUP_A *ingroup;
{

	LAYER_A *looper;
	STDFLT dx,dy;
	
	bboxany = FALSE;			/* Activity flag */
	
	bboxmaxy = bboxmaxx = -MAXBIG;	/* Prep max min fun */
	bboxminy = bboxminx = MAXBIG;

	for (looper=Root_Layer; looper!=NULL; looper=looper->layer_next)
		if (match(looper->Layer_Name,"HATCH")) 
			BBoxBounds(ingroup,looper);
			
	if (!bboxany) {
		complain("Continue Next",NULL,3,3,3,3,
					"Hatch table pattern",
					ingroup->Group_Name,
					"had no drawing elements?",
					NULL);
	
		return(FALSE);	/* Didn't find anything? */
	}
	
	dx = bboxmaxx-bboxminx;
	dy = bboxmaxy-bboxminy;
		     
	bscale = max(dx,dy);
	
	FtoPORT(bscale,&(obuf2.Hatch_Scale));	/* Fill output buffer */
	FtoPORT(bboxminx,&(obuf2.BaseX));
	FtoPORT(bboxminy,&(obuf2.BaseY));
		
		
	return(TRUE);	/* Looks good - got something at least */
}

/*---------------Find the users boundary box layer corners----------------*/

int gotbounds=0;
STDFLT boundsx[4],boundsy[4];

VOID TryBoxPoint(inx,iny)
STDFLT inx,iny;
{

	int lp,cmax;
	
	cmax = gotbounds;
	if (cmax>=4) cmax = 4;
	
	for (lp=0; lp<cmax; lp++) {
		if (boundsx[lp]==inx &&
		    boundsy[lp]==iny) return;
	}
		    
	if (gotbounds<4) {
	    	boundsx[gotbounds] = inx;
		boundsy[gotbounds] = iny;
	}
		    
	gotbounds++;

}
			
VOID BoxBounds(ingroup,inlayer)
GROUP_A *ingroup;
LAYER_A *inlayer;
{

	O_LINE *doline;
	
	D_NODE *looper;
	
	/* Eliminate all but proper pieces */
	for (looper=Root_Node; looper!=NULL; looper=looper->node_subsequent) {
		if (looper->above_group!=ingroup) continue;
		if (looper->node_layer!=inlayer) continue;
	
		if (looper->node_id!=NODE_LINE) continue;
		
		doline = GetLineData(looper);

		TryBoxPoint(doline->x1,doline->y1);
		TryBoxPoint(doline->x2,doline->y2);
	}
}
				
BOOL BoundaryBox(ingroup)
GROUP_A *ingroup;
{

	LAYER_A *looper;
	int i,j,oorder[4],a;
	STDFLT tmps[4],tang[4],cx,cy,b;
	
	gotbounds = 0;				/* No bounds seen yet */
	
	for (looper=Root_Layer; looper!=NULL; looper=looper->layer_next)
		if (match(looper->Layer_Name,"BOUNDARY")) 
			BoxBounds(ingroup,looper);
			
	if (gotbounds!=4) {
		complain("Continue Next",NULL,3,3,3,3,
					"Hatch table pattern",
					ingroup->Group_Name,
					"BOUNDARY layer not set up",
					"correctly. -- See Manual");
		return(FALSE);	/* Not Fatal Error */
	}
	
	/* Sort the boundary box corners clockwise */

	cx = cy = 0.0;	
	for (i=0; i<4; i++) {
		cx += boundsx[i];
		cy += boundsy[i];
		oorder[i] = i;
	}
	
	cx /= 4.0;
	cy /= 4.0;		/* Calc center of p-gram */
	
	for (i=0; i<4; i++)
		tang[i] = atan2(boundsy[i]-cy, boundsx[i]-cx);
	
	/* A quick bubble sort on these - yuck! but only for 4? */	
	
	for (i=0; i<4; i++) 
	  for (j=0; j<3; j++) {
	  	if (tang[j]<tang[j+1]) {
			a = oorder[j];			/* Twiddle orders */
			oorder[j] = oorder[j+1];
			oorder[j+1] = a;
			
			b = tang[j];
			tang[j] = tang[j+1];
			tang[j+1] = b;
		}
	  }
	
	/* Twiddle the original coordinates */
	
	for (i=0; i<4; i++)	FtoPORT(boundsx[oorder[i]],&(obuf2.PGramX[i]));
	for (i=0; i<4; i++)	FtoPORT(boundsy[oorder[i]],&(obuf2.PGramY[i]));

	return(TRUE);
}
	
/*--------------------------Write Acutal Data-------------------------------*/

/* Search through drawing elements for target pieces */

VOID GetElements(ingroup,inlayer)
GROUP_A *ingroup;
LAYER_A *inlayer;
{

	STDFLT tx,ty;
	ULONG xp,yp,stage;

	O_LINE *doline;
	
	D_NODE *looper;
	
	/* Eliminate all but proper pieces */
	for (looper=Root_Node; looper!=NULL; looper=looper->node_subsequent) {
		if (looper->above_group!=ingroup) continue;
		if (looper->node_layer!=inlayer) continue;
	
		if (looper->node_id!=NODE_LINE) continue;
		
		doline = GetLineData(looper);
		
		if (doline->x1!=lastxelement ||
		    doline->y1!=lastyelement) {
		    
			tx = doline->x1 - bboxminx;
			ty = doline->y1 - bboxminy;
			
			tx /= bscale;
			ty /= bscale;
			
			tx *= 32767.0;
			ty *= 32767.0;
			
			xp = tx;
			yp = ty;
			
			stage = (1<<31) | (xp<<15) | yp;
			
			bigtab[population++] = stage;
		}
		
		tx = doline->x2 - bboxminx;
		ty = doline->y2 - bboxminy;
			
		tx /= bscale;
		ty /= bscale;
			
		tx *= 32767.0;
		ty *= 32767.0;
			
		xp = tx;
		yp = ty;
			
		stage = (xp<<15) | yp;		/* High bit not set */
			
		bigtab[population++] = stage;

		lastxelement = doline->x2;
		lastyelement = doline->y2;
		
	}
}

/* Look through layers for HATCH layer */
				
BOOL HatchElements(ingroup)
GROUP_A *ingroup;
{

	LAYER_A *looper;

	lastxelement = lastyelement = MAXBIG;
	population = 0;
			
	for (looper=Root_Layer; looper!=NULL; looper=looper->layer_next)
		if (match(looper->Layer_Name,"HATCH")) 
			GetElements(ingroup,looper);
			
	return(TRUE);			
}


BOOL TraverseGroups()
{

	GROUP_A *looper;
	int loop;
	
	obuf1.Hatch_Magic = HAT_MAGIC_NUM;
	obuf1.Hatch_Version = CUR_HATCH_VER;

	fwrite(&obuf1, sizeof (struct Hatch_Header), 1, hatfile);
			
	loop = 0;
	
	anythinghappen = FALSE;
	
	looper = Root_Group->descendants;
	
	if (looper==NULL) {
		complain("Recover",NULL,3,3,3,3,
				"The group structure in your",
				"file is not set up correctly",
				"for the creation of hatching",
				"pattern tables. -- See Manual");
						
		return(FALSE);
	}
	
	for (; looper!=NULL; looper=looper->next) {
		if (looper->descendants!=NULL) {
			complain("Recover",NULL,3,3,3,3,
				"The group structure in your",
				"file is not set up correctly",
				"for the creation of hatching",
				"pattern tables. -- See Manual");
		
			return(FALSE);
		}

		strcpy(obuf2.File_Name,hatchfile);
		strcpy(obuf2.Hatch_Name,looper->Group_Name);
						
		if (HatchBoundingBox(looper)) 
		   if (BoundaryBox(looper))
		      if (!HatchElements(looper)) continue;

		loop++;
		      
		obuf2.next = (struct Hatch_Entry *) loop;
		obuf2.prev = NULL;
		      
		obuf2.Pattern_Size = population * sizeof(ULONG);
		      
		obuf2.Pattern = NULL;
		           		      
		fwrite(&obuf2, sizeof (struct Hatch_Entry), 1, hatfile);
		      
		fwrite(&bigtab[0], population * sizeof(ULONG), 1, hatfile);
		      
		
	}
	
      	obuf2.next = (struct Hatch_Entry *) 0;		/* End of file */
	obuf2.prev = NULL;
	
      	fwrite(&obuf2, sizeof (struct Hatch_Entry), 1, hatfile);
		      
	if (!anythinghappen) {	
		complain("Recover",NULL,3,3,3,3,
			"I couldn't find anything to",
			"convert!  Are you sure the",
			"file is set up for hatching",
			"pattern tables? -- See Manual");

		return(FALSE);
	}
	
	return(TRUE);		
}
		
/*-------------------------Hatching file handling---------------------------*/

BOOL puthat(path,file,iflnm,afln)
UBYTE *iflnm,*afln,*path,*file;
{

	long	errret;
	UBYTE 	ptitle[80];
	UBYTE	ftmp[120],ftmp2[120];

	hatchfile = file;

	sprintf(ftmp,"%s(uhat-temporary).uhat",iflnm);
	sprintf(ftmp2,"%s%s",iflnm,afln);
	hatfile=fopen(ftmp,"w");
	if (hatfile==NULL) {
		printf("Cannot save %s\n",afln);
		InfoMess(SAVE_MODE,SAVE_MESS_ERROR);
		return(FALSE);
	}

	bigtab = (ULONG *) rmalloc(bufsiz);
	if (bigtab==NULL) return(FALSE);	/* Oops */
	
	if (!TraverseGroups()) {	/* Just leave as temp file */
		rmfree(bigtab,bufsiz);
		fclose(hatfile);
		return(FALSE);
	}
	
	rmfree(bigtab,bufsiz);
	fclose(hatfile);

	DeleteFile(ftmp2);	/* Delete if exists or not */
	if (!Rename(ftmp,ftmp2)) {
		printf("Couldn't rename temporary to your output name!\n");
		return(FALSE);
	}

	return(TRUE);
}
