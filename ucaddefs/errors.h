#ifndef DIALOG_ERRORS

#define DIALOG_ERRORS

/*---------------------------Error Returns----------------------------------*/

#define DIALOG_OK	0		/* Command parsed and executed */
#define DIALOG_REXX	10		/* Like OK, arexx argument returned */

/* Bad Syntax/Context Class Errors */
#define DIALOG_PARSER		100	/* Error parsing input line */
#define DIALOG_BADFILETYPE	101	/* File type non-sequiteur */
#define DIALOG_BADCOLOR		102	/* Hex color string bad */
#define DIALOG_NOWILDS		103	/* No Wildcards allowed here */
#define DIALOG_NOVARIABLE	104	/* No variable of that name exists */
#define DIALOG_BADVALUE		105	/* Value has bad format */
#define DIALOG_MISSING		106	/* Required switch missing */

/* Bad Data Errors */
#define DIALOG_DOSERROR		200	/* Got a dos error - bad read, etc */
#define DIALOG_NOFILE		201	/* Could not find requested file */
#define DIALOG_CLIENT		202	/* Some programming error */
#define DIALOG_BADFORM		203	/* Bad IFF form error */
#define DIALOG_SHORTCHUNK	204	/* Bad IFF chunk error */
#define DIALOG_BADIFF		205	/* Mal-formed IFF file */
#define DIALOG_NOTIFF		206	/* This is not an IFF file */
#define DIALOG_UNKNOWNIFF	209	/* Unknown iff error */

/* Search Problems */
#define DIALOG_NOLAYER		250	/* Layer not found */
#define DIALOG_NOPEN		251	/* Pen not found */
#define DIALOG_NOGROUP		252	/* Group not found */
#define DIALOG_NOSTYLE		253	/* Style not found */
#define DIALOG_NOHATCH		254	/* Hatch pattern not found */
#define DIALOG_NOFONT		255	/* Font not found */
#define DIALOG_NOOBJECTS	256	/* No drawing objects to affect */

#define DIALOG_GOTLAYER		260	/* Layer already here */
#define DIALOG_GOTPEN		261	/* Pen already here */
#define DIALOG_GOTGROUP		262	/* Group already here */
#define DIALOG_GOTSTYLE		263	/* Style already here */
#define DIALOG_GOTHATCH		264	/* Hatch already here */
#define DIALOG_GOTFONT		265	/* Font already here */

/* System Problems */
#define DIALOG_NOINTUI		300	/* No Intution library */
#define DIALOG_NOGRAPH		301	/* No graphics library */
#define DIALOG_NOREXSYS		302	/* No rexx system library */
#define DIALOG_NOICON		303	/* No icon system library */
#define DIALOG_NOFONTS		304	/* No diskfont system library */
#define DIALOG_NOREXPORT	320	/* No system rexx port */
#define DIALOG_NOTIMER		321	/* Cannot open timer */
#define DIALOG_NOSTROKE		322	/* Cannot load stroke font */
#define DIALOG_NOWINDOWS	323	/* Cannot open windows */
#define DIALOG_REXXERR		324	/* Problem talking to rexx */
#define DIALOG_NOUPORT		325	/* Ultradesign has no port? */

#endif
