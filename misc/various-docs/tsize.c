/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* |_o_o|\\ Copyright (c) 1986 The Software Distillery.  All Rights Reserved */
/* |. o.| || This program may not be distributed without the permission of   */
/* | .  | || the authors.                                                    */
/* | o  | ||    Dave Baker     Ed Burnette  Stan Chow    Jay Denebeim        */
/* |  . |//     Gordon Keener  Jack Rouse   John Toebes  Doug Walker         */
/* ======          BBS:(919)-471-6436      VOICE:(919)-469-4210              */ 
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*--------------------------------------------------------------------------+
 | program:   tsize
 | purpose:   Find the size of a directory tree by adding the sizes
 |            of all files in the directory and all of its
 |            subdirectories.
 | author:    Edmund Burnette
 | usage:     tsize              -- find size of current directory
 |            tsize <name>       -- find size of named file or directory
 | history:   16apr86/ebb - version 1.0 created
 |            10jul86/ebb - added break handler, tsize of single file
 |            12jul86/ebb - changed break handling
 | notes:     1. When given a file, this program just returns the size of
 |               that file.
 +-------------------------------------------------------------------------*/

#include <libraries/dosextens.h>

char copyright[] =
"TSIZE v1.2 by Edmund Burnette - Copyright © 1986 The Software Distillery";

#define ERREXITRC(arcee) { rc = arcee; goto EXIT; }
#define ERREXIT goto EXIT;
#define ERRORRC(arcee, errmsg) { rc = arcee; printf errmsg; goto EXIT; }
#define ERROR(errmsg) { printf errmsg; goto EXIT; }

#define CTOB(cptr) ((BPTR)((unsigned long)(cptr) >> 2))
#define BTOC(bptr) ((char *)((unsigned long)(bptr) << 2))

/* exit codes */
#define TOO_MANY_NAMES 50    /* more than one file or directory specified  */

BPTR stdout;
char path[256];
char empty[]    = "";

BPTR Output();
int checkbreak();
long tsize(int, int);
int addname(char *, int);
int parse(int, char *[], int *);

main(argc, argv)
int argc;
char *argv[];
   {
   int rc = 0;               /* return code                                */
   int len;                  /* length of file name given on command line  */

   /* initialize standard out */
   stdout = Output();

   /* parse command line */
   if (rc = parse(argc, argv, &len)) ERREXIT

   /* print tree size information */
   (void)tsize(len, TRUE);

EXIT:
   return(rc);
   }

/*--------------------------------------------------------------------------+
 | name:      checkbreak
 | purpose:   Check for CTRL-C / CTRL-D brek
 | history:   12jul86/ebb - created
 | usage:     rc = checkbreak();
 |            int rc;                   flag="break detected"
 | notes:     1. Sets global abort flag.
 *-------------------------------------------------------------------------*/

#define SIGS (SIGBREAKF_CTRL_C | SIGBREAKF_CTRL_D)

int checkbreak()
   {
   static int abort = FALSE;

   /* print message and set abort flag if break occurred */
   if (!abort && (SetSignal(0,SIGS) & SIGS))
      {
      printf("**BREAK\n");
      abort = TRUE;
      }

   /* return break detected flag */
   return abort;
   }

/*--------------------------------------------------------------------------+
 | name:      tsize
 | purpose:   Find the size of a directory tree by adding the sizes
 |            of all files in the directory and all of its
 |            subdirectories.
 | usage:     size = tsize(len, prtsize)
 |            long size;         returned size of tree
 |            int len;           length of path name
 |            int prtsize;       flag to print size of tree even if file
 | history:   16apr86/ebb - created
 |            10jul86/ebb - handle printing size of single file
 |            12jul86/ebb - changed break handling
 | notes:     1. Uses the path name stored in an external variable to
 |               save stack space.
 |            2. 
 | future:    
 +-------------------------------------------------------------------------*/

long tsize(len, prtsize)
int len;
int prtsize;
   {
   struct FileInfoBlock *info = NULL;  /* pointer to file info             */
   long size = 0;                      /* running total of file size       */
   BPTR lock;                          /* pointer to file lock             */

   BPTR Lock();
   BOOL Examine();
   BOOL ExNext();

   /* try to obtain lock on current file */
   if ((lock = Lock(path, ACCESS_READ)) == NULL)
      ERROR(("Invalid file or directory, '%s'.\n", path))

   /* allocate an info block (must be longword alligned) */
   if ((info = (struct FileInfoBlock *)
               AllocMem(sizeof(struct FileInfoBlock), 0)) == NULL)
      ERROR(("Out of memory.\n"))

   /* get information on file or directory associated with lock */
   if (!Examine(lock, info))
      ERROR(("Error examining locked file or directory.\n"))

   /* what have we here... */
   if (info->fib_DirEntryType > 0)
      {
      /* lock is for a directory */
      while (ExNext(lock, info) && !checkbreak())
         size += tsize(addname(info->fib_FileName, len), FALSE);
      prtsize = TRUE;
      }
   else if (info->fib_DirEntryType < 0)
      /* lock is for a file */
      size = info->fib_Size;
   else
      /* invalid entry type */
      ERROR(("Invalid directory entry type of 0 for '%s'.\n", path))

   /* repair path and print statistics */
   if (prtsize && !checkbreak())
      {
      path[len] = '\0';
      printf("%s %ld\n", path, size);
      }
   
EXIT:
   if (info) FreeMem(info, sizeof(struct FileInfoBlock));
   if (lock) UnLock(lock);
   return(size);
   }

/*--------------------------------------------------------------------------+
 | name:      parse
 | purpose:   Recognize a file name or directory in the argument list.
 | usage:     rc = parse(argc, argv, len);
 |            int rc;            return code (0: ok, non-0: failed)
 |            int argc;          number of arguments
 |            char *argv[];      argument vector
 |            int len;           length of file or directory name
 | history:   16apr86/ebb - created
 | notes:     1. Stores the file or directory name in the external
 |               variable 'path' (null terminated).
 |            2. 
 | future:    
 +-------------------------------------------------------------------------*/

int parse(argc, argv, len)
int argc;
char *argv[];
int *len;
   {
   int rc = 0;               /* return code                                */
   int i;                    /* argument index                             */
   int found = 0;            /* flag="name found on argument list"         */
   register char *arg;       /* pointer to current argument                */

   /* process all arguments */
   for (i = 1; i < argc; i++)
      {
      arg = argv[i];
      /* check for name already specified */
      if (found)
         /* attempt to give more than one directory */
         ERRORRC(TOO_MANY_NAMES,
                 ("More than one file or directory specified.\n"))
      else
         {
         /* set name */
         found = 1;
         *len = addname(arg, 0);
         }
      } /* for */

   /* use current directory if no file or directory specified */
   if (!found)
      *len = addname(empty, 0);

EXIT:
   return rc;
   }

/*--------------------------------------------------------------------------+
 | name:      addname
 | purpose:   Add a name to the end of the current path.
 | usage:     newlen = addname(name, len);
 |            int newlen;        returned new length of resulting path
 |            char *name;        null terminated name to add
 |            int len;           length of current path
 | history:   16apr86/ebb - created
 | notes:     1. Modifies the external variable 'path'.
 |            2. 
 | future:    
 +-------------------------------------------------------------------------*/

int addname(name, len)
register char *name;
int len;
   {
   register char *p = path + len;  /* current position in path             */
   register int i = 0;             /* added length counter                 */
   char ch;                        /* last character of current path       */

   /* add a slash between path and name if legal */
   if (len != 0 && (ch = path[len-1]) != '/' && ch != ':')
      {
      *p++ = '/';
      ++i;
      }

   /* copy name to end of path (including null terminator) */
   for (;*p++ = *name++; ++i) /* nothing */ ;

   /* return new length of path */
   return len + i;
   }
