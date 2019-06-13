/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	filereq - File requester
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
 *	1	28-Jun-87 SH	Begin writing
 *	2	22-Aug-87 SH	Insure that libraries get rid of their 
 *				 context on a save, so that the new file
 *				 will appear the next time requester used
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "plotsetup.h"
#include <libraries/dos.h>
#include <libraries/dosextens.h>

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

BOOL CheckAbort();
#define MemRem(size,rem) AllocRemember(rem,size,0)

/*-------------------------External Symbol Definitions-----------------------*/

BOOL Scroller();

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

/*------------------------Set up scroller for styles------------------------*/

/* Disk device node structure */
struct DeviceNode
{
  BPTR dn_Next;
  ULONG dn_Type;
  struct MsgPort *dn_Task;
  BPTR dn_Lock;
  BSTR dn_Handler;
  ULONG dn_StackSize;
  LONG dn_Priority;
  BPTR dn_Startup;
  BPTR dn_SegList;
  BPTR dn_GlobalVec;
  BSTR dn_Name;
};

BOOL RootDisk = FALSE;			/* Is this dir the root */

struct File_Entry {
	UBYTE	*FileName;		/* File name alstr'd must free */
	LONG	byteslong;		/* Length of file in bytes */
	LONG	blockslong;		/* Length of file in blocks */
	LONG	agedays;		/* How old file is in days */
	BOOL	directory;		/* Is this a directory node? */
	struct	File_Entry *prev_file;	/* Backwards linked list */
	struct	File_Entry *next_file;	/* Next file in linked list */
} *Root_File=NULL,*Tail_File=NULL;

UBYTE	*Current_Path;			/* Location of path in file */
BOOL	allowparent,allowsubs;
struct FileInfoBlock *finfo;
struct Remember **userem;
UBYTE *filepath;
VOID ClearGet();
VOID ClearList();
VOID GetDevNames();

struct File_Entry *File_Parent(current_in,inmask,descend)
struct File_Entry *current_in;
UBYTE *inmask;
BOOL descend;
{

	int a;

	if (!allowparent) return(NULL);	/* No path manipulation allowed */

	a = strlen(filepath)-1;
	if (a>=0&&filepath[a]==':') return(NULL);	/* No parent */
	if (descend) {
		if (a<0) return(NULL);
		while (a>=0) {
			if (filepath[a]=='/') {
				filepath[a]='\0';
				ClearGet();
				return(NULL);
			}
			if (filepath[a]==':') {
				ClearGet();
				return(NULL);
			}
			filepath[a]='\0';
			a--;
		}
	}
	return((struct File_Entry *) 1); /* Flag 1 will specify parent */

}

struct File_Entry *File_Next(current_in,inmask)
struct File_Entry *current_in;
UBYTE *inmask;
{

	if (current_in==2) {
		current_in=Root_File;
		if (current_in->directory) return(current_in);	
		if (match(inmask,current_in->FileName))
			return(current_in);
	}

	FOREVER {
		current_in=current_in->next_file;
		if (current_in==NULL) return(NULL);

		if (current_in->directory) return(current_in);	
		if (match(inmask,current_in->FileName))
			return(current_in);

	}
}

struct File_Entry *File_Prev(current_in,inmask)
struct File_Entry *current_in;
UBYTE *inmask;
{

	if (current_in==2) return(NULL);
	
	FOREVER {
		current_in=current_in->prev_file;
		if (current_in==NULL) return(2);
	
		if (current_in->directory) return(current_in);	
		if (match(inmask,current_in->FileName))
			return(current_in);

	}
}

struct File_Entry *File_First(current_in,inmask,descend)
struct File_Entry *current_in;
UBYTE *inmask;
BOOL descend;
{

	if (descend) ClearGet();

	return(2);

/*	current_in = Root_File;

	if (current_in->directory) return(current_in);	

	if (match(inmask,current_in->FileName))
		return(current_in);

	return(File_Next(Root_File,inmask)); */

}

struct File_Entry *File_Search(insearch)
UBYTE *insearch;
{

	struct File_Entry *looper;

	for (looper=Root_File; looper!=NULL; looper=looper->next_file) 
		if (match(insearch,looper->FileName)) return(looper);

	return(NULL);

}

struct File_Entry *File_Descend(current_in,inmask,descend)
struct File_Entry *current_in;
UBYTE *inmask;
BOOL descend;
{

	UBYTE newbuf[101];
	int a;

	if ((current_in==2)&&(!descend)) return(-2);
	if (!current_in->directory) return(NULL);

	if (descend) {
		
		if (current_in==2) {
			ClearList();
			return(NULL);
		}
		
		if ((a=strlen(filepath))==0||
				strchr(current_in->FileName,':')!=NULL)
			strcpy(newbuf,current_in->FileName);
		else {
			if (filepath[a-1]==':') 
				sprintf(newbuf,"%s%s",filepath,
					current_in->FileName);
			else sprintf(newbuf,"%s/%s",filepath,
				current_in->FileName);
		}
		strcpy(filepath,newbuf);
		ClearGet();

		return(NULL);
	}
			
	return((struct File_Entry *)(- ((int) current_in)));

}

VOID File_Form(current_in,outform)
struct File_Entry *current_in;
UBYTE *outform;
{

	UBYTE *st;
		
	if (current_in==NULL) return;

	if (((int) current_in)==1) {
		sprintf(outform,"/ Parent");
		return;
	}

	if (((int) current_in)==2) {
		sprintf(outform," [List Devices]");
		return;
	}

	for (st=current_in->FileName; *st==' '; st++);

	if (current_in->directory) {
		sprintf(outform," (%s)",st);
		return;
	}

	sprintf(outform,"  %-32.32s %6d %4d  %4d",
		st,
		current_in->byteslong,
		current_in->blockslong,
		current_in->agedays);

}


VOID File_Name(current_in,outform)
struct File_Entry *current_in;
UBYTE *outform;
{

	outform[0] = '\0';
	if (current_in==NULL) return;
	if (current_in==1) return;
	if (current_in==2) return;

	strcpy(outform,current_in->FileName);

}
UBYTE *alget(instr)
UBYTE *instr;
{

	UBYTE *c1;

	c1 = MemRem(strlen(instr)+1,userem);

	if (c1==NULL) return(NULL);

	strcpy(c1,instr);
	return(c1);

}

int asccmp(in1,in2)
char *in1,*in2;
{

	FOREVER {
		if (*in1!='\0'&&*in2=='\0') return(1);
		if (*in2!='\0'&&*in1=='\0') return(-1);
		if (*in2=='\0'&&*in1=='\0') return(0);
		if (toupper(*in1)>toupper(*in2)) return(1);
		if (toupper(*in2)>toupper(*in1)) return(-1);
		in1++; in2++;
	}
}

int cmpname(in1,in2)
struct File_Entry *in1,*in2;
{

	if (in1->directory==in2->directory)
		return(asccmp(in1->FileName,in2->FileName));

	if (in1->directory) return(-1);
	return(1);
}

VOID alphainsert(instr)
struct File_Entry *instr;
{

	struct File_Entry *loop,*inent;

	inent = (struct File_Entry *) 
		MemRem(sizeof(struct File_Entry),userem);
	if (inent==NULL) return;

	*inent = *instr;
	
	inent->prev_file = NULL;
	inent->next_file = NULL;

	if (Root_File==NULL) {
		Root_File = Tail_File = inent;
		return;
	}

	for (loop=Root_File; loop!=NULL; loop=loop->next_file)
		if (cmpname(inent,loop)<0) break;

	if (loop==NULL) {
		Tail_File->next_file = inent;
		inent->prev_file = Tail_File;
		Tail_File = inent;
		return;
	}
	
	if (loop->prev_file==NULL) {
		Root_File = inent;
		loop->prev_file = inent;
		inent->next_file = loop;
		return;
	}

	loop->prev_file->next_file = inent;
	inent->next_file = loop;
	inent->prev_file = loop->prev_file;
	loop->prev_file = inent;

	return;
}

UBYTE *path_setup(inpath)
UBYTE *inpath;
{

	UBYTE nambuf[101],nambf2[101],*thepath;
	BOOL firstone=TRUE;
	struct FileLock *cur_lock,*par_lock;

	if (!allowparent) return(inpath);

	thepath = inpath;
	while (*thepath==' ') thepath++;

	nambuf[0]='\0';
	cur_lock = Lock(thepath, ACCESS_READ);	
	FOREVER {
		if (cur_lock==NULL) break;
		if (!Examine(cur_lock, finfo)) break;
		par_lock = ParentDir(cur_lock);
		UnLock(cur_lock);
		if (finfo->fib_DirEntryType>0) {
		  if (par_lock==NULL) 
			sprintf(nambf2,"%s:%s",finfo->fib_FileName,nambuf);
		  else if (firstone) {
			strcpy(nambf2,finfo->fib_FileName);
			firstone=FALSE;
		  } else  sprintf(nambf2,"%s/%s",finfo->fib_FileName,nambuf);
		  strcpy(nambuf,nambf2);
		}
		cur_lock = par_lock;
	}

	strcpy(inpath,nambuf);
	return(inpath);
}	
					
filereq_setup()
{
	finfo = DrawMem((sizeof (struct FileInfoBlock)));
	if (finfo==NULL) getout("Not enough memory");
}

VOID dir_files()
{

	struct File_Entry efile;
	struct FileLock *cur_lock;
	struct DateStamp NowTime;
	UBYTE *stoname;

	if ((cur_lock = Lock(path_setup(filepath), ACCESS_READ)) !=NULL) {
		if (Examine(cur_lock, finfo)) {
			FOREVER {
				ExNext(cur_lock, finfo);
				if (IoErr() == ERROR_NO_MORE_ENTRIES) break;

				stoname = alget(finfo->fib_FileName);
				if (stoname ==NULL) break;
				efile.FileName = stoname;
				efile.byteslong = finfo->fib_Size;
				efile.blockslong = finfo->fib_NumBlocks;
				DateStamp(&NowTime);

				efile.agedays = NowTime.ds_Days - 
					finfo->fib_Date.ds_Days;
				if (efile.agedays<0) efile.agedays = 0;
				efile.directory = (finfo->fib_DirEntryType>0)?
					TRUE:FALSE;
				if (efile.directory&&!allowsubs) continue;
				efile.next_file=NULL;
				efile.prev_file=NULL;

				alphainsert(&efile);

			}	
		}
		UnLock(cur_lock);
	}
}	

VOID ClearGet()
{

	if (*userem!=NULL) FreeRemember(userem,TRUE);

	Root_File=Tail_File = NULL;
	*userem=NULL;

	dir_files();
}

VOID ClearList()
{

	if (*userem!=NULL) FreeRemember(userem,TRUE);

	Root_File=Tail_File = NULL;
	*userem=NULL;

	GetDevNames();
}
	
BOOL File_Sc(chgpath,loadstr,inrem,inmask,title,
	keep_root,keep_tail,path,subs,Holf)
UBYTE *loadstr,*inmask,*title,*path;
struct Remember **inrem;
struct File_Entry **keep_root,**keep_tail;
BOOL subs,chgpath;
struct File_Entry **Holf;
{

	BOOL ret;

	userem = inrem; 

	filepath = path;
	allowsubs = subs;
	allowparent = chgpath;

	Root_File = *keep_root;
	Tail_File = *keep_tail;

	if (Root_File==NULL) dir_files();

	ret=Scroller(File_First,
		File_Next,
		File_Prev,
		File_Search,
		File_Parent,
		File_Descend,
		File_Form,
		File_Name,
		chgpath,
		TRUE,
		FALSE,
		*Holf,
		loadstr,
		"  Cancel  ",
		title,
		"File Name---------------------------Bytes-Blks--Age (Days)",
		Holf,
		inmask,
		path);

	if (TotAvail()<200000) {
		Root_File=Tail_File = NULL;
		FreeRemember(userem,TRUE);
		*inrem=NULL;
	} 

	*keep_root = Root_File;
	*keep_tail = Tail_File;

	return(ret);

}

/* -------------------------------------------------------------------- */
/* getdevs.h - structures and includes.
/* -------------------------------------------------------------------- */

/* Extern data */
extern struct DosLibrary *DOSBase;

BYTE RamDirNameStr[]	= "RAM:";	/* Incase RAM wasn't found, we supply	*/

/* -------------------------------------------------------------------- */
/* Constructs a circular double-linked list of device name nodes. 	*/
/* Returns FALSE if there was a allocation failure, TRUE if all went	*/
/* as planned. 								*/
/* --------------------------------------------------------------------	*/
VOID GetDevNames()
{
  	int i;
  	struct RootNode *root;
  	struct DosInfo *di;
  	UBYTE bufarea[100],*strarea;
  	register struct DeviceNode *dn;
  	register UBYTE *bstring;
	register WORD blen;
  	register WORD ramflag = 0;	/* Flag whether RAM: device found */
  	struct File_Entry efile;
  
	efile.FileName = NULL;
	efile.byteslong = 0;
	efile.blockslong = 0;
	efile.agedays = 0;
	efile.directory = TRUE;
	efile.next_file=NULL;
	efile.prev_file=NULL;

/* Grab pointer to DosInfo device root */

  	root = (struct RootNode *)DOSBase->dl_Root;
  	di = (struct DosInfo *)BADDR(root->rn_Info);
  	dn = (struct DeviceNode *) BADDR (di->di_DevInfo);

/* Don't let other process muck with us during this crucial phase */

  	Forbid();

/* Starting with root devinfo, set devnames till out of devices */

  	for (; dn != 0L; dn = (struct DeviceNode *)BADDR(dn->dn_Next)) {

    		if (dn->dn_Task && dn->dn_Name) {

			bufarea[0] = ' ';
			strarea = &bufarea[0];
			if (dn->dn_Type==0) strarea = &bufarea[1];
			

		        bstring = (UBYTE *)BADDR(dn->dn_Name);	/* Pointer to BSTR name */
		        blen = (WORD)bstring[0];		/* Length of BSTR	*/
			for (i=0; i<blen; i++) strarea[i] = bstring[i+1];
			strarea[blen] = ':';
			strarea[blen+1] = '\0';
		        if ( strcmp(strarea, RamDirNameStr) ==0 )   
				ramflag = 1;

			efile.FileName = alget(bufarea);

			alphainsert(&efile);
    		}
	}
	
  	Permit();

  	if (!ramflag) {	 /* Ram device wasn't out there, add it to list */

		efile.FileName = alget(RamDirNameStr);

		alphainsert(&efile);
  	}
}
