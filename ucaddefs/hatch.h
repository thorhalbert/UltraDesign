#ifndef HATCH_INFO
#define HATCH_INFO

#define HATCH_EXT ".uhat"

/* Hatching tile database */

struct Hatch_Header {

	WORD	Hatch_Magic;	/* Magic number for stroke hatch data */
	WORD	Hatch_Version;	/* Version number of this hatch */
};

struct Hatch_Entry {

	struct Hatch_Entry *next;	/* On disk is the count of items */
					/* zero denotes an end of file */
	struct Hatch_Entry *prev;	/* Previous entry */
	
	UBYTE	Hatch_Name[40];	/* Name of this hatch pattern */
	UBYTE	File_Name[40];	/* File info came from */
	
	PORTFLOAT Hatch_Scale;	/* How wide is a step (pt*scale=unit) */
	
	PORTFLOAT PGramX[4];	/* The four corners of the parallelogram */
	PORTFLOAT PGramY[4];	/* Sorted Clockwise */
	
	PORTFLOAT BaseX,BaseY;	/* The origin of the pattern table */
	
	ULONG	Pattern_Size;	/* Size of the table in bytes */
	ULONG	*Pattern;	/* Pointer to line patterns - used in mem */
	
	UWORD	Hatch_ID;	/* Cross reference ID */
	UWORD	Use_Count;	/* How may times used? */
	
};

/* This structure is intended to be used both on disk and in memory.  It has
   a view fields which are not useful on disk.
   
   The file contains data in the following order:
   
   	1. Hatch_Header
	2. Hatch_Entry 1
	3. Hatch Data for 1
	4. Hatch_Entry 2
	      .
	      .
	      .
	n. Hatch_Entry with next set to zero
	
   The hatch data is a table of ULONGs each representing an X,Y pair in the
   form:	
   		pattern = (sbit*(1<<31)) | (x<<15) | y;
		sbit is set to 1 if a move 0 if a draw
		x is 0-32k y is 0-32k
		
   These data describe a tile pattern which is replicated based on a 
   paralellogram described by arrays PGramX and PGramY.  These really
   don't have be be parallel but the results might be interesting. 
		
*/

#define CUR_HATCH_VER 10	/* Version 1.0 of the hatch data */
#define HAT_MAGIC_NUM 26927	/* Hatch magic number */
#endif
