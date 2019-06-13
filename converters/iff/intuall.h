/*** intuall.h **********************************************************/
/*  intuall.h, Include lots of Amiga-provided header files.	1/22/86 */
/*  Plus the portability file "iff/compiler.h" which should be tailored */
/*  for your compiler.							*/
/*                                                                      */ 
/* By Jerry Morrison and Steve Shaw, Electronic Arts.                   */ 
/* This software is in the public domain.                               */ 
/*                                                                      */ 
/* This version for the Commodore-Amiga computer.                       */
/*                                                                      */ 
/************************************************************************/

#include "iff/compiler.h"	/* COMPILER-DEPENDENCIES */

/* Dummy definitions because some includes below are commented out.
 * This avoids 'undefined structure' warnings when compile.
 * This is safe as long as only use POINTERS to these structures.
 */

/* struct Region { int dummy; };
struct VSprite { int dummy; };
struct collTable { int dummy; };
struct CopList { int dummy; };
struct UCopList { int dummy; };
struct cprlist { int dummy; };
struct copinit { int dummy; };
struct TimeVal { int dummy; }; */

#include "exec/types.h"
#include "exec/nodes.h"
#include "exec/lists.h"
#include "exec/libraries.h"
#include "exec/ports.h"

#include "exec/tasks.h"
#include "exec/devices.h"


#include "exec/interrupts.h"

#include "exec/io.h"
#include "exec/memory.h"
#include "exec/alerts.h"

/* ALWAYS INCLUDE GFX.H before any other amiga includes */

#include "graphics/gfx.h"
/*#include "hardware/blit.h"*/

/*****
#include "graphics/collide.h"
#include "graphics/copper.h"
#include "graphics/display.h"
#include "hardware/dmabits.h" 
#include "graphics/gels.h"
***/

#include "graphics/clip.h"

#include "graphics/rastport.h"
#include "graphics/view.h"
#include "graphics/gfxbase.h"
/*#include "hardware/intbits.h"*/
#include "graphics/gfxmacros.h"

#include "graphics/layers.h"

#include "graphics/text.h"
#include "graphics/sprite.h"
/*#include "hardware/custom.h"*/

/*#include "libraries/dos.h"*/
/*#include "libraries/dosextens.h"*/

#include "devices/timer.h"
#include "devices/inputevent.h"
#include "devices/keymap.h"

#include "intuition/intuition.h"

/*#include "intuitionbase.h"*/
/*#include "intuinternal.h"*/

