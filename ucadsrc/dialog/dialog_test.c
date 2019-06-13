#include <stdio.h>

#include <exec/types.h>

/*
        alloca -- (mostly) portable public-domain implementation -- D A Gwyn

        last edit:      86/05/30        rms
           include config.h, since on VMS it renames some symbols.
           Use malloc instead of malloc.

        This implementation of the PWB library alloca() function,
        which is used to allocate space off the run-time stack so
        that it is automatically reclaimed upon procedure exit,
        was inspired by discussions with J. Q. Johnson of Cornell.

        It should work under any C implementation that uses an
        actual procedure stack (as opposed to a linked list of
        frames).  There are some preprocessor constants that can
        be defined when compiling for your specific system, for
        improved efficiency; however, the defaults should be okay.

        The general concept of this implementation is to keep
        track of all alloca()-allocated blocks, and reclaim any
        that are found to be deeper in the stack than the current
        invocation.  This heuristic does not reclaim storage as
        soon as it becomes invalid, but it will do so eventually.

        As a special case, alloca(0) reclaims storage without
        allocating any.  It is a good idea to use alloca(0) in
        your main control loop, etc. to force garbage collection.
*/

typedef char    *pointer;               /* generic pointer type */

extern void     free();
extern pointer  malloc();

/*
        Define STACK_DIRECTION if you know the direction of stack
        growth for your system; otherwise it will be automatically
        deduced at run-time.

        STACK_DIRECTION > 0 => grows toward higher addresses
        STACK_DIRECTION < 0 => grows toward lower addresses
        STACK_DIRECTION = 0 => direction of growth unknown
*/

#define STACK_DIRECTION -1

#define STACK_DIR       STACK_DIRECTION /* known at compile-time */

/*
        An "alloca header" is used to:
        (a) chain together all alloca()ed blocks;
        (b) keep track of stack depth.

        It is very important that sizeof(header) agree with malloc()
        alignment chunk size.  The following default should work okay.
*/

#ifndef ALIGN_SIZE
#define ALIGN_SIZE      sizeof(double)
#endif

typedef union hdr
{
  char  align[ALIGN_SIZE];      /* to force sizeof(header) */
  struct
    {
      union hdr *next;          /* for chaining headers */
      char *deep;               /* for stack depth measure */
    } h;
} header;

/*
        alloca( size ) returns a pointer to at least `size' bytes of
        storage which will be automatically reclaimed upon exit from
        the procedure that called alloca().  Originally, this space
        was supposed to be taken from the current stack frame of the
        caller, but that method cannot be made to work for some
        implementations of C, for example under Gould's UTX/32.
*/

static header *last_alloca_header = NULL; /* -> last alloca header */

pointer
alloca (size)                   /* returns pointer to storage */
     unsigned   size;           /* # bytes to allocate */
{
  auto char     probe;          /* probes stack depth: */
  register char *depth = &probe;

                                /* Reclaim garbage, defined as all alloca()ed storage that
                                   was allocated from deeper in the stack than currently. */

  {
    register header     *hp;    /* traverses linked list */

    for (hp = last_alloca_header; hp != NULL;)
      if (STACK_DIR > 0 && hp->h.deep > depth
          || STACK_DIR < 0 && hp->h.deep < depth)
        {
          register header       *np = hp->h.next;

          free ((pointer) hp);  /* collect garbage */

          hp = np;              /* -> next header */
        }
      else
        break;                  /* rest are not deeper */

    last_alloca_header = hp;    /* -> last valid storage */
  }

  if (size == 0)
    return NULL;                /* no allocation required */

  /* Allocate combined header + user data storage. */

  {
    register pointer    new = malloc (sizeof (header) + size);
    /* address of header */

    ((header *)new)->h.next = last_alloca_header;
    ((header *)new)->h.deep = depth;

    last_alloca_header = (header *)new;

    /* User storage begins just after header. */

    return (pointer)((char *)new + sizeof(header));
  }
}

void bcopy(a,b,l)
char *a,*b;
int l;
{

	register int i;
	
	for (i=0; i<l; i++) a[i] = b[i];
}

BOOL hiteof=FALSE;

int getybuf(buf,max_size)
char *buf;
int max_size;
{

	if (hiteof) return(0);

	buf[0] = getchar();
	
	if (buf[0]=='\n') {
		hiteof = TRUE;
		buf[0] = '\0';
		return(0);
	}

	return(1);
}

main()
{

	hiteof = FALSE;
	yyparse();	

}

void DM_LoadFile(file,info) 
{

	printf("DM_LoadFile called\n");

}
void DM_SaveFile(file,info) 
{

	printf("DM_SaveFile called\n");

}
void DM_MergeFile(file, info) 
{

	printf("DM_MergeFile called\n");

}
void DM_UseFile(file,info) 
{

	printf("DM_UseFile called\n");

}
void DM_NewBuffer(info) 
{

	printf("DM_NewBuffer called\n");

}
void DM_SetCurrent(attrib,info) 
{

	printf("DM_SetCurrent called\n");

}
void DM_SetAttrib(attrib,info) 
{

	printf("DM_SetAttrib called\n");

}
void DM_CreateAttrib(attrib,info) 
{

	printf("DM_CreateAttrib called\n");

}
void DM_RenameAttrib(from,to,info) 
{

	printf("DM_RenameAttrib called\n");

}
void DM_MergeAttrib(from,to,info) 
{

	printf("DM_MergeAttrib called\n");

}
void DM_ExpungeAttrib(from,info) 
{

	printf("DM_ExpungeAttrib called\n");

}
void DM_DeleteAttrib(attrib,info) 
{

	printf("DM_DeleteAttrib called\n");

}
void DM_SetColor(what,to) 
{

	printf("DM_SetColor called\n");

}

void yyerror(a)
char *a;
{
	printf("Error: %s\n",a);
}

char *Stostring1(in) {}
char *Stostring2(in) {}

char *ParLin(in,err) {}
char *ParAng(in,err) {}

