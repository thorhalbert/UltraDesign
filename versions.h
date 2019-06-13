/*----------------------------------------------------------------------*
 *	versions.h - screen version numbers				*
 *----------------------------------------------------------------------*/

/*
 *	Modification History
 *
 *	0(36)	 8-May-89 SH	Added report layer, report pen, move point,
 *				move point co-linear, draw thru, seek 
 *				intersection
 *	0(37)	13-Jun-89 SH	Work on dialog and reports
 *	0(38)	19-Jul-89 SH	Finish (in dialog)
 *				 delete group, hatch, rename all, run files
 *				 on boot
 *				Other modes
 *				 Report Group, Fix architectural, 
 *				 elliptical hatching, select hatch, box hatch
 *	0(39)	26-Aug-89 SH	Add new dialog console handler
 *	0(40)	10-Sep-89 SH	Split filereq into file_handler and filereq,
 *				 add device list to file_handler, add filet
 *				 command to searcher, and force intuition
 *				 requesters to ucad window instead of wb
 *				 Add minimum memory startup requirements
 *	0(41)	 9-Oct-89 SH	Tons of cleanup changes for V1 
 *				 Eliminate Lock icons from attributes
 *				 Get rid of the sheet height from text
 *				 X & Y flip now called horiz and vert flip
 *				 Do not save unused pattern references
 *				 Move dimensioning defaults to margins
 *				 Add rectangular lock to dimensioning
 *				 Put icon in dim req for angle lock
 *				 Clean up complain requester
 *				 Add pacman interface for ellipses
 *				 add various register lines to dim,ellip,text
 *				 Add dotted lines to pasteup remove sheet lock
 *				 Made some changes to cleartext
 *	0(42)	29-Oct-89 SH	Small changes for release
 *	1.0(44)	17-Dec-89 SH	The Release version	Version 1.0
 *	1.0(45)	18-Jan-90 SH	Post-Release Bug fixes	Not Released
 *				  1. iffput0 was writing files in group
 *				     order instead of paint order - also
 *				     affects cadverter
 *				  2. clonedrag was not rotating text
 *				     properly, full new flip algorithm and
 *				     move handle to center of box and back
 *				  3. painting - DoArrow would guru when
 *				     Arrow_Angle was 90 (/0 error)
 *				  4. pasteup - postscript printer driver
 *				     was putting parens around last showpage
 *				     command
 *	1.1(46)	 6-May-90 SH	Next Bug fix barrage	Version 1.1
 *				  1. Ellipse begin/end angles stopped working
 *				     at around 100 meters in FFP mode because
 *				     routine in renderers.c started to overflow.
 *				     Played games with math to fix.
 *				     Fixed on principal in pasteup even though
 *				     it is IEEE instead of FFP.
 *				  2. Added object editor, changed ud.menus
 *				  3. Minor speedups to setup_render in 
 * 				     painting.c
 *				  4. In dimsreq, aborting after the first
 *				     witness line leaves line hanging out
 *				  5. Double click in pasteup (iffload)
 *				     crashes in Load_View
 *				  6. String functions in clonedrag leave
 *				     bogus current items
 *				  7. Dialog @ command does not create
 *				     a coord pair, just bogus values.
 *				  8. @ commands do not properly paint
 *				     crosshairs because there is not always
 *				     an outstanding timer request.  Move
 *				     dialog check outside of timer loop.
 *				  9. Make the cancel in hatch and font load
 *				     do something.
 *				  10. Many suggestions by Larry Hanebrink
 *				      for Aegis Draw Loader:
 *					A. Aegis has odd text baseline,
 *					   make base 25% up, and char 70% high
 *					B. That odd value in the part insert
 *					   is the insertion angle.  Implement.
 *					C. Text in part was *2, make *1
 *					D. Sorry Larry, Simplex2 does not
 *					   have a %.  We will have to fix when
 *					   we redo fonts.
 *				  11. Rename in group_req or dialogue for
 *				      groups would jettison any descendants
 *			 	      for that group.  Add new arg to 
 *				      Link_Group() and groupinsert() to
 *				      control this -- change all calls.
 *	1.1(47)	20-May-90 SH	Minor fixes before release of V1.1
 *				  1.  DXF-Writer was not writing a 70
 *				      flag in LAYER table if it was 0,
 *				      which caused autocad to barf
 *				  2.  Add angle and length fields to 
 *				      line mode in the object editor
 *				  3.  Add alternate snap to pixel increment
 *				      to freehand - add field to margin req,
 *				      code to pointers to handle mouse and
 *				      code to linemode to tell us about 
 *				      freehand status
 *				  4.  Make freehand/boxmode mutually exclude
 *				      each other
 *				  5.  Change text in margin req from 'View'
 *				      to 'Draw' in req and report
 *				  6.  Forgot to implement $$DRAWING_UNIT as
 *				      synonym to $$VIEW_UNIT (dialogue.c)
 *				  7.  Double click in zoom would crash due
 *				      to /0 (zooms.c)
 *				  8.  Grids mode not always rendering all
 *				      of the division lines (painting.c)
 *				  9.  REPLACE the English Architectural and
 *				      English Feet/Inches mode -- we hope
 *				      that this is last time we need to
 *				      mess with this.
 *				 10.  Fixed EPSF header in EPS driver
 *				      in pasteup.
 *				 11.  Accidentally transposed the angular
 *				      units and linear units in dialog,
 *				      so when you set one, you set the other
 *				      instead (with possibly bogus results)
 *	1.1(48) 17-Jun-90 SH	Other minor fixes before V1.1
 *				  1.  DXF-Writer had a few other Flag bugs.
 *				      Autocad will not accept files with non
 *				      alphanumeric chars in layers/pens/styles,
 *				      replace bad chars with little ex 'x'.
 *				  2.  Rehash the text in margin req to conform
 *				      with manual
 *				  3.  Pasteup had error with fill scale that
 *				      kept the fill as hatch from working.
 *				      FILL_HATCH_SCALE is now also an absolute
 *				      scale in printer units.
 *				  4.  Do not allocate the memory for solid
 *				      filling until they are needed.  This saves
 *				      some 40-60K of chip memory for people
 *				      with 1M machines.  This can cause crash
 *				      conditions as machine nears low memory
 *				      and solid filling is needed, but we knew
 *				      this.
 *				  5.  Pattern select requester said 'font', not
 *				      'pattern'.
 *
 */

#define VERNAME "UltraDesign"

#ifdef F_FFP
#define VERDIS "UltraDesign V1.1(48)-F"
#define VERNUM "V1.1(48)-F"
#endif

#ifdef F_IEEE
#define VERDIS "UltraDesign V1.1(48)-I"
#define VERNUM "V1.1(48)-I"
#endif

#ifdef F_881
#define VERDIS "UltraDesign V1.1(48)-8"
#define VERNUM "V1.1(48)-8"
#endif

#define _TURNSY		/* Use the NOSYSI for requesters */
/* #define SCREEN_SAVER	*/ /* Do we configure a screen saver */

#define USE_AREXX	/* Turn on the arexx hooks */
