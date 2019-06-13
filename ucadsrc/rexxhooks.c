/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	rexxhooks - calls to external arexx server
 *
 */

/*
 *	Copyright (C) 1986, 1987, 1988 by WURBsoft Inc., and Scott Halbert.
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
 *	1	 3-Apr-89 SH	Create Arexx hooks
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include "errors.h"

#include <libraries/dos.h>
#include <libraries/dosextens.h>

#include <rexx/rxslib.h>

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

extern LONG d_return;	/* Give dialog it's return value */

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

extern BOOL dialopen;
struct RexxLib *RexxSysBase = NULL;

struct MsgPort UcadPort,*RexxPort=NULL;
struct RexxMsg *rmptr = NULL;
BOOL UcadSetup=FALSE;
UBYTE portname[40];
extern struct FileInfoBlock *finfo;

#define REXPORT "REXX"

LONG OpenRexxLib()
{
   	RexxSysBase = OpenLibrary("rexxsyslib.library",0L);
   	if (RexxSysBase == NULL) return(DIALOG_NOREXSYS);
	/* printf("Loaded rexxsyslib.library ok!\n"); */
	return(DIALOG_OK);
}

LONG OpenRexxPort()
{
	if (RexxSysBase==NULL) return(DIALOG_NOREXSYS);
	RexxPort = FindPort(REXPORT);
	if (RexxPort==NULL) return(DIALOG_NOREXPORT);
	/* printf("Found REXX public port ok!\n"); */
	return(DIALOG_OK);
}

LONG MakeUcadPort()
{
	int i,j;
	struct MsgPort *temp;
	
	UcadSetup = FALSE;
		
	if (RexxSysBase==NULL) return(DIALOG_NOREXSYS);
	if (RexxPort==NULL) return(DIALOG_NOREXPORT);
	
	Forbid();	/* Lock everyone out */
	
	for (i=1; i <999; i++) {
	
		sprintf(portname,"UltraDesign_P%d",i);
		
		temp = FindPort(portname);
		
		if (temp!=NULL) continue;	/* We already got one */
		
		j = InitPort(&UcadPort,portname);
	
		AddPort(&UcadPort);
		
		printf("Create public port %s ok %d!\n",portname,j);
		
		UcadSetup = TRUE;
		
		break;
	}
	
	Permit();	/* Ok, you can come in now */
	
	UcadSetup = TRUE;
	
	return(DIALOG_OK);
}

VOID FreeRexx()
{
	if (!UcadSetup) return;
   	RemPort(&UcadPort);
   	FreePort(&UcadPort);
	if (RexxSysBase!=NULL) CloseLibrary(RexxSysBase);
}

extern LONG d_return;

UBYTE retans[80];

VOID ProcessRexxPacket(gotmsg)
struct RexxMsg *gotmsg;
{

	/* printf("Processing a message\n"); */

	if (!IsRexxMsg(gotmsg)) {
		printf("I got a bad message on the rexx port\n");
		
		ReplyMsg(gotmsg);	/* Better reply anyway */
	}
	
	strcpy(retans,"0");		/* Normally illegal */
	
	RexxLine(ARG0(gotmsg));		/* Parse this puppy */
	
	gotmsg->rm_Result1 = d_return;
	gotmsg->rm_Result2 = 0;
	
	if (gotmsg->rm_Action&RXFF_RESULT&&d_return==0) {
	
		printf("Set return string to %s\n",retans);
		gotmsg->rm_Result2 = CreateArgstring(retans, strlen(retans));
		
	}
	
	ReplyMsg(gotmsg);
	
}

char *ext = "uurx";

VOID DoRexxCommand(incmd,flag)
UBYTE *incmd;
LONG flag;
{

	struct MsgPort *rexxport;
	struct RexxMsg *rexxmsg,*ucadback;
	struct Process *thisproc;
	struct RexxArg *geterr=NULL;
	int i;
	
	UBYTE prtmess[257];

	d_return = DIALOG_OK;

	if (RexxSysBase==NULL) {
		d_return = DIALOG_NOREXSYS;
		return;
	}

	if (!UcadSetup) {
		d_return = DIALOG_NOUPORT;
		return;
	}
	
	/* According to the dosextens include, findtask returns a pointer
	   to the task structure for a process, but it so happens that DOS
	   processes are stored in a process stucture, so they are coincident,
	   this cast may not be as dangerous as it looks */
	   
	thisproc = (struct Process *) FindTask(0);	/* Find us */
	if (thisproc==NULL) {
		printf("Could not find our task structure???\n");
		d_return = DIALOG_REXXERR;		/* Huh? */
		return;
	}
	
	Forbid();		/* Turn off switching */
	
	FOREVER {
	
		rexxport = FindPort("REXX");	/* Search for port */
	
		if (rexxport==NULL) {
			d_return = DIALOG_NOREXPORT;
			break;
		}

		rexxmsg = CreateRexxMsg(&thisproc->pr_MsgPort, ext, portname);
		if (rexxmsg==NULL) {
			printf("Could not create a rexx message???\n");
			d_return = DIALOG_REXXERR;
			break;
		}
		
		rexxmsg->rm_Action = RXCOMM | flag;
		ARG0(rexxmsg) = incmd;
		rexxmsg->rm_Stdin = dialog_in;
		rexxmsg->rm_Stdout = dialog_out;
								
		if (!FillRexxMsg(rexxmsg, 1, 0)) {
			printf("Could not fill a rexx message\n");
			d_return = DIALOG_REXXERR;
			break;
		}
		
		PutMsg(rexxport, rexxmsg);

		Permit();

		FOREVER {

			Wait((1<<UcadPort.mp_SigBit)|(1<<thisproc->pr_MsgPort.mp_SigBit));
	
			rexxmsg = (struct RexxMsg *) GetMsg(&thisproc->pr_MsgPort);
			if (rexxmsg!=NULL) break;
			
			ucadback = (struct RexxMsg *) GetMsg(&UcadPort);
			if (ucadback!=NULL) ProcessRexxPacket(ucadback);
			
		}
		
		printf("Rexx returned %d/%d",
			rexxmsg->rm_Result1,
			rexxmsg->rm_Result2);

		if (ErrorMsg(rexxmsg->rm_Result2,&geterr)) {
			
			for (i=0; i<geterr->ra_Length; i++) 
				prtmess[i] = geterr->ra_Buff[i];
			prtmess[i] = '\0';
			
			printf(": %s",prtmess);
		}
		
		printf("\n");

		break;		
	}
	
	Permit();		/* Turn switching back on permanently */
		
	ClearRexxMsg(rexxmsg,1);	/* Give these back */
	DeleteRexxMsg(rexxmsg);
	
	return;
		
}

#define RXFF_NOIO /* (1<<RXFB_NOIO) */ 0

VOID ParseRexxCommand(inbuf)
UBYTE *inbuf;
{
	BOOL clos=FALSE;
	
	clos=FALSE;
	if (!dialopen) {
		popdefault();
		if (!dialopen) return;
		clos=TRUE;
	}
	

	while (*inbuf==' ') inbuf++;

	DoRexxCommand(inbuf,RXFF_NOIO|RXFF_STRING);
	if (clos) CloseDialog();

}

VOID ParseLaunchCommand(inbuf)
UBYTE *inbuf;
{
	struct FileLock *cur_lock,*rexx_lock;
	BOOL clos=FALSE;
	
	clos=FALSE;
	if (!dialopen) {
		popdefault();
		if (!dialopen) return;
		clos=TRUE;
	}
	
	
	while (*inbuf==' ') inbuf++;

	rexx_lock = Lock(RexxPath, ACCESS_READ);
	if (rexx_lock!=NULL) {
		if (!Examine(rexx_lock, finfo)) {
			UnLock(rexx_lock);
			rexx_lock = FALSE;
		} else {
			if (finfo<=0) {		/* Its not a dir */
				UnLock(rexx_lock);
				rexx_lock = FALSE;
			}
		}
	}
	
	cur_lock = NULL;
	if (rexx_lock!=NULL) cur_lock = CurrentDir(rexx_lock);			
				
	DoRexxCommand(inbuf,RXFF_NOIO);
	
	if (rexx_lock!=NULL) {
		(VOID) CurrentDir(cur_lock);
		UnLock(rexx_lock);
	}

	if (clos) CloseDialog();
}

VOID ParseResultCommand(inbuf)
UBYTE *inbuf;
{

	while (*inbuf==' ') inbuf++;

	CheckResult(inbuf, retans);

}

BOOL RexxParse(inbuf)
UBYTE *inbuf;
{

	UBYTE cbuf[40];
	int i;
	
	for (i=0; i<4; i++)
		cbuf[i] = toupper(inbuf[i]);
	cbuf[i] = '\0';
	
	if (strcmp(cbuf,"REXX")==0) {
		ParseRexxCommand(&inbuf[4]);
		return(TRUE);
	}
	
	for (i=0; i<6; i++)
		cbuf[i] = toupper(inbuf[i]);
	cbuf[i] = '\0';
	
	if (strcmp(cbuf,"LAUNCH")==0) {
		ParseLaunchCommand(&inbuf[6]);
		return(TRUE);
	}
	
	for (i=0; i<6; i++)
		cbuf[i] = toupper(inbuf[i]);
	cbuf[i] = '\0';
	
	if (strcmp(cbuf,"RESULT")==0) {
		ParseResultCommand(&inbuf[6]);
		return(TRUE);
	}
	
	return(FALSE);	/* No Rexx verb seen */
}
