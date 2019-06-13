#ifndef GIO_H
#define GIO_H
/*----------------------------------------------------------------------*/
/* GIO.H  defs for Generic I/O Speed Up Package.   	        1/23/86 */
/* See GIOCall.C for an example of usage.				*/
/* Read not speeded-up yet.  Only one Write file buffered at a time.	*/
/*									*/
/* Note: The speed-up provided is ONLY significant for code such as IFF */
/* which does numerous small Writes and Seeks.				*/
/*									*/
/* WARNING: If gio reports an error to you and you care what specific	*/
/* Dos error was, you must call IoErr() BEFORE calling any other gio	*/
/* functions.								*/
/*                                                                      */ 
/* By Jerry Morrison and Steve Shaw, Electronic Arts.                   */ 
/* This software is in the public domain.                               */ 
/*                                                                      */ 
/* This version for the Commodore-Amiga computer.                       */
/*                                                                      */ 
/*----------------------------------------------------------------------*/

/* Use this file interface in place of ALL Open,Close,Read,Write,Seek DOS
 * calls for an optional i/o speed-up via buffering.  You must use ONLY
 * these G routines for a file that is being buffered; e.g., call GClose
 * to Close the file, etc.
 * It is harmless though not necessary to use G routines for a file that
 * is not being buffered; e.g., GClose and Close are equivalent in that
 * case.
 * This Version only buffers one file at a time, and only for writing.
 * If you call GWriteDeclare for a second file before the first file
 * is GClosed, the first file becomes unbuffered.  This is harmless, no
 * data is lost, the first file is simply no longer speeded-up.
 */

/* Before compiling any modules that make G calls, or compiling gio.c,
 * you must set the GIO_ACTIVE flag below.
 *
 * To omit the speed-up code,
 *    #define GIO_ACTIVE 0
 *
 * To make the speed-up happen:
 * 1. #define GIO_ACTIVE 1
 * 2. link gio.o into your progrm
 * 3. GWriteDeclare(file, buffer, size)
 *    after GOpening the file and before doing
 *    any writing.
 * 4. ONLY use GRead, GWrite, GSeek, GClose -- do not use the DOS i/o
 *    routines directly.
 * 5. When done, do GClose.  Or to stop buffering without closing the
 *    file, do GWriteUndeclare(file).
 */
#define GIO_ACTIVE 0

#ifndef COMPILER_H
#include "compiler.h"
#endif

#ifndef LIBRARIES_DOS_H
#include "libraries/dos.h"
#endif

#ifndef OFFSET_BEGINNING
#define OFFSET_BEGINNING OFFSET_BEGINING
#endif

#if GIO_ACTIVE

#ifdef FDwAT  /* Compiler handles Function Declaration with Argument Types */

/* Present for completeness in the interface.
 * "openmode" is either MODE_OLDFILE to read/write an existing file, or
 * MODE_NEWFILE to write a new file.
 * RETURNs a "file" pointer to a system-supplied structure that describes
 * the open file.  This pointer is passed in to the other routines below.*/
extern BPTR GOpen(char * /*filename*/, LONG /*openmode*/);

/* NOTE: Flushes & Frees the write buffer.
 * Returns -1 on error from Write.*/
extern LONG GClose(BPTR /*file*/);

/* Read not speeded-up yet.
 * GOpen the file, then do GReads to get successive chunks of data in
 * the file.  Assumes the system can handle any number of bytes in each
 * call, regardless of any block-structure of the device being read from.
 * When done, GClose to free any system resources associated with an
 * open file.*/
extern LONG GRead(BPTR /*file*/, BYTE * /*buffer*/, LONG /*nBytes*/);

/* Writes out any data in write buffer for file.
 * NOTE WHEN have Seeked into middle of buffer:
 * GWriteFlush causes current position to be the end of the data written.
 * -1 on error from Write.*/
extern LONG GWriteFlush(BPTR /*file*/);

/* Sets up variables to describe a write buffer for the file.*/
/* If the buffer already has data in it from an outstanding GWriteDeclare,
 * then that buffer must first be flushed.
 * RETURN -1 on error from Write for that previous buffer flush.
 * See also "GWriteUndeclare".*/
extern LONG GWriteDeclare(BPTR /*file*/, BYTE * /*buffer*/, LONG /*nBytes*/);

/* ANY PROGRAM WHICH USES "GWrite" MUST USE "GSeek" rather than "Seek"
 * TO SEEK ON A FILE BEING WRITTEN WITH "GWrite".
 * "Write" with Generic speed-up.
 * -1 on error from Write.  else returns # bytes written to disk.
 * Call GOpen, then do successive GWrites with GSeeks if required,
 * then GClose when done.  (IFF does require GSeek.)*/
extern LONG GWrite(BPTR /*file*/, BYTE * /*buffer*/, LONG /*nBytes*/);

/* "Seek" with Generic speed-up, for a file being written with GWrite.*/
/* Returns what Seek returns, which appears to be the position BEFORE
 * seeking, though the documentation says it returns the NEW position.
 * In fact, the code now explicitly returns the OLD position when
 * seeking within the buffer.
 * Eventually, will support two independent files, one being read, the
 * other being written.  Or could support even more.  Designed so is safe
 * to call even for files which aren't being buffered.*/
extern LONG GSeek(BPTR /*file*/, LONG /*position*/, LONG /*mode*/);

#else /*not FDwAT*/

extern BPTR GOpen();
extern LONG GClose();
extern LONG GRead();
extern LONG GWriteFlush();
extern LONG GWriteDeclare();
extern LONG GWrite();
extern LONG GSeek();

#endif FDwAT

#else /* not GIO_ACTIVE */

#define GOpen(filename, openmode)    	    Open(filename, openmode)
#define GClose(file)  			    Close(file)
#define GRead(file, buffer, nBytes)	    Read(file, buffer, nBytes)
#define GWriteFlush(file)		    (0)
#define GWriteDeclare(file, buffer, nBytes) (0)
#define GWrite(file, buffer, nBytes)	    Write(file, buffer, nBytes)
#define GSeek(file, position, mode)	    Seek(file, position, mode)

#endif GIO_ACTIVE


/* Release the buffer for that file, flushing it to disk if it has any
 * contents.  GWriteUndeclare(NULL) to release ALL buffers.
 * Currently, only one file can be buffered at a time anyway.*/
#define GWriteUndeclare(file)  GWriteDeclare(file, NULL, 0)


#endif
