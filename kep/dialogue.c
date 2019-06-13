/*
 *	UltraCAD - WURBsoft Inc., Integrated Drafting/Composition Package
 *
 *	UCAD - Drawing/Library Editor
 *
 *	File:	dialogue.c - The macro input system
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
 *	1	 9-Oct-88 SH	Initial dialogue input system
 *	2	10-Mar-89 SH	Add the parser
 *
 */

/*-------------------------------Include Files-------------------------------*/

#include "ucad.h"
#include <devices/console.h>

/*----------------------------------Macros-----------------------------------*/

/*-----------------------------Forward References----------------------------*/

/*-------------------------External Symbol Definitions-----------------------*/

/*-------------------------Global Variable Definitions-----------------------*/

/*-------------------------------Code Section--------------------------------*/

struct NewWindow newdial = {
   50, 300,                          	/* start position                */
   300, 75,                 		/* width, height                 */
   3, 2,                        	/* detail pen, block pen         */
   CLOSEWINDOW,	                   	/* IDCMP flags                   */
   WINDOWCLOSE | ACTIVATE | WINDOWSIZING | WINDOWDRAG | SMART_REFRESH,
                                  	/* window flags                  */
   NULL,	           		/* pointer to first user gadget  */
   NULL,                          	/* pointer to user checkmark     */
   "Dialogue Window",  		  	/* window title                  */
   NULL,                          	/* pointer to screen (set below) */
   NULL,                          	/* pointer to superbitmap        */
   200, 60, 200, 300,			/* Max resizing box size	 */
   CUSTOMSCREEN                   	/* type of screen desired        */
};

BOOL ucadscreen;

struct IOStdReq *dial_writemsg=NULL,*dial_readmsg=NULL;
struct MsgPort *dial_writeport=NULL,*dial_readport=NULL;

int dialmode;
int dialstep;

#define DIALO(ins) ConPutStr(dial_writemsg,ins)
#define DIALOCRLF DIALO("\n\r");

int OpenConsole(writerequest,readrequest,window)
struct IOStdReq *writerequest;
struct IOStdReq *readrequest;
struct Window *window;
{

	int error;
	
	writerequest->io_Data = (void *) window;
	writerequest->io_Length = sizeof (struct Window *);
	error = OpenDevice("console.device",0,writerequest,0);
	readrequest->io_Device = writerequest->io_Device;
	readrequest->io_Unit = writerequest->io_Unit;
	return(error);
}

int lchr=0;
UBYTE ibuf[2],obuf[2],linbuf[121];

int QueueRead(request)
struct IOStdReq *request;
{

	request->io_Command = CMD_READ;
	request->io_Data = (void *) &ibuf[0];
	request->io_Length = 1;
	SendIO(request);
	return(0);
}

int ConMayGetChar(request,requestPort)
struct IOStdReq *request;
struct MsgPort *requestPort;
{

	register int temp;
	
	if (GetMsg(requestPort)==NULL) return(-1);
	temp = ibuf;
	QueueRead(request);
	return(temp);
	
}

prompt()
{
	dialmode = 0;
	dialstep = 0;
	DIALO("([L]ine or [E]llipse) >> ");
}

BOOL ParsePair(inbuf,x,y)
UBYTE *inbuf;
STDFLT *x,*y;
{

	UBYTE buf1[30],buf2[30],*a,*b;
	BOOL error;
	
	a = inbuf;
	b = buf1;
	
	while (*a!=','&& *a!='\0') *b++ = *a++;
	*b++ = '\0';
	
	if (*a=='\0') return(TRUE);
	
	a++;
	b = buf2;

	while (*a!='\0') *b++ = *a++;
	*b++ = '\0';
	
	printf("First=%s Second=%s\n",buf1,buf2);
	
	*x = ParLin(buf1,&error);
	if (error) return(TRUE);
	
	*y = ParLin(buf2,&error);
	if (error) return(TRUE);
	
	return(FALSE);
}

BOOL ParseAngles(inbuf,ang,dis)
UBYTE *inbuf;
STDFLT *ang,*dis;
{

	UBYTE buf1[30],buf2[30],*a,*b;
	BOOL error;
	
	a = inbuf;
	b = buf1;
	
	while (*a!=','&& *a!='\0') *b++ = *a++;
	*b++ = '\0';
	
	if (*a=='\0') return(TRUE);
	
	a++;
	b = buf2;

	while (*a!='\0') *b++ = *a++;
	*b++ = '\0';
	
/*	printf("First=%s Second=%s\n",buf1,buf2);	 */
	
	*ang = ParAng(buf1,&error);
	if (error) return(TRUE);
	
	*dis = ParLin(buf2,&error);
	if (error) return(TRUE);
	
	return(FALSE);
}

LOCAL STDFLT linx,liny,slinx,sliny,ang,dist;
LOCAL STDFLT lastang=0.0;

LineIn(inbuf)
UBYTE *inbuf;
{
	UBYTE *fcar;
	UBYTE *ptype;
	
	if (inbuf==NULL) {
		lastang = 0.0;
		DIALO("Start X,Y: ");
		return;
	}	
	
	if (*inbuf=='\0') {
		prompt();
		return;
	}
	
	FOREVER {
		if (dialstep==0) {
			if (ParsePair(inbuf,&linx,&liny)) {
				DIALO("?Bad coordinates: ");
				DIALO(inbuf);
				DIALOCRLF;
				break;
			}
			dialstep = 1;
			break;
		}
	
		ptype = 'L';
		fcar = toupper(*inbuf);
		switch ((int) fcar) {
		case 'A':			/* Absolute */
			ptype = 'L';
			inbuf++;
			break;
		case 'R':			/* Relative */
			ptype = 'L';
			inbuf++;
			break;
		case 'P':			/* Polar ang,distance */
			ptype = 'A';
			inbuf++;
			break;
		case 'T':			/* Turtle relang,distance */
			ptype = 'A';
			inbuf++;
			break;
		default:
			ptype = 'L';
			fcar = 'A';
			break;

		}

		if (ptype=='L') {
						
			if (ParsePair(inbuf,&slinx,&sliny)) {
				DIALO("?Bad Coordinates: ");
				DIALO(inbuf);
				DIALOCRLF;
				break;	
			}
			
			if (fcar=='R') {
				slinx += linx;
				sliny += liny;
			}
		} else if (ptype=='A') {
		
			if (ParseAngles(inbuf,&ang,&dist)) {
				DIALO("?Bad Angle/Distance: ");
				DIALO(inbuf);
				DIALOCRLF;
				break;	
			}
			
			if (fcar=='T') ang += lastang;

			slinx = linx + (cos(ang)*dist);
			sliny = liny + (sin(ang)*dist);
			lastang = ang;
			normalize(lastang);

		} else break;
		
		CreateLine(linx,liny,slinx,sliny,0);
		linx = slinx;
		liny = sliny;
	
		break;		
	}
	
	if (dialstep>0) 	DIALO(" Next X,Y: ");
	else			DIALO("Start X,Y: ");
	
	return;
}

LOCAL STDFLT xrad,yrad,sang,eang,erot;

EllipIn(inbuf)
UBYTE *inbuf;
{

	UBYTE *fcar;
	UBYTE *ptype;
	BOOL  error;
	
	if (inbuf==NULL) dialstep = 0;

	switch (dialstep) {
	case 1:		/* X,Y center */
		if (ParsePair(inbuf,&linx,&liny)) {
			DIALO("?Bad Coordinates: ");
			DIALO(inbuf);
			DIALOCRLF;
		} else dialstep++;
		break;
	case 2:		/* X radius */
		xrad = ParLin(inbuf,&error);
		if (error||xrad<=0.0) {
			DIALO("?Bad Radius: ");
			DIALO(inbuf);
			DIALOCRLF;
		} else dialstep++;
		break;
	case 3:		/* Y radius */
		if (*inbuf=='\0') {
			yrad = xrad;
			error = FALSE;
		} else yrad = ParLin(inbuf,&error);
		if (error||yrad<=0.0) {
			DIALO("?Bad Radius: ");
			DIALO(inbuf);
			DIALOCRLF;
		} else dialstep++;
		break;
	case 4:		/* Start angle */
		if (*inbuf=='\0') {
			sang = 0.0;
			error = FALSE;
		} else sang = ParAng(inbuf,&error);
		if (error) {
			DIALO("?Bad Starting Angle: ");
			DIALO(inbuf);
			DIALOCRLF;
		} else dialstep++;
		break;
	case 5:		/* End angle */
		if (*inbuf=='\0') {
			eang = TWOPI;
			error = FALSE;
		} else eang = ParAng(inbuf,&error);
		if (error) {
			DIALO("?Bad Ending Angle: ");
			DIALO(inbuf);
			DIALOCRLF;
		} else dialstep++;
		break;
	case 6:		/* Ellipse rotation */
		if (*inbuf=='\0') {
			erot = 0.0;
			error = FALSE;
		} else erot = ParAng(inbuf,&error);
		if (error) {
			DIALO("?Bad Rotation Angle: ");
			DIALO(inbuf);
			DIALOCRLF;
		} else {
			dialstep=0;
			normalize(erot);
			normalize(sang);
			normalize(eang);
			CreateEllipse(linx,liny,xrad,yrad,
				erot,sang,eang);
		}
		break;
	}
		
	if (inbuf==NULL) dialstep = 1;

	switch (dialstep) {
	case 0:
		prompt();
		break;
	case 1:
		DIALO("   Center X,Y: ");
		break;
	case 2: 
		DIALO("     X Radius: ");
		break;
	case 3:
		DIALO("Y Rad <X rad>: ");
		break;
	case 4:
		DIALO("Start Ang <0>: ");
		break;
	case 5:
		DIALO("End Ang <360>: ");
		break;
	case 6:
		DIALO(" Rotation <0>: ");
		break;
	}
	
}

char *rdit;
BOOL hiteof=FALSE;

int getybuf(buf,max_size)
char *buf;
int max_size;
{

	if (hiteof) return(0);

	buf[0] = *rdit++;
	
	if (buf[0]=='\n'||buf[0]=='\) {
		hiteof = TRUE;
		buf[0] = '\0';
		return(0);
	}

	return(1);
}


StuffLine() 
{
	UBYTE *linin;
	
	linin=linbuf;
	
	while (linin==' ') linin++;
	
	switch(dialmode) {
	case 1:
		LineIn(linin);
		return;
	case 2:
		EllipIn(linin);
		return;
	}

	dialstep = 0;
	switch(toupper(*linin)) {
	case 'L':
		dialmode = 1;
		LineIn(NULL);
		return;
	case 'E':
		dialmode = 2;
		EllipIn(NULL);
		return;
	}

	if (*linin!='\0') {	
		DIALO("?Illegal command: ");
		DIALO(linin);
		DIALOCRLF;
	}
		
	prompt();

}

StuffChar()
{

	int temp;
	
	temp = ibuf[0];
	QueueRead(dial_readmsg);

	obuf[0] = ibuf[0];	
	obuf[1] = '\0';

	if (temp=='\177'||temp=='\010') {
		if (lchr>0) {
			DIALO("\010 \010");	/* Erase on screen */
			lchr--;
		}
		return;
	}
	
	if (temp=='\n'||temp=='\r') {
		DIALOCRLF;
		linbuf[lchr++] = '\0';
		StuffLine();
		lchr = 0;
		return;
	}
	
	DIALO(obuf);
	
	if (lchr<119) linbuf[lchr++] = temp;
	
}

int ConPutStr(request,string)
struct IOStdReq *request;
char *string;
{

	request->io_Command = CMD_WRITE;
	request->io_Data = (void *) string;
	request->io_Length = -1;
	
	DoIO(request);
	return(0);
}

VOID closedial()
{


	if (dial_writemsg!=NULL)	CloseDevice(dial_writemsg);

	if (dial_writemsg!=NULL) 	DeleteStdIO(dial_writemsg);

	if (dial_writeport!=NULL) 	DeletePort(dial_writeport);

	if (dial_readmsg!=NULL) 	DeleteStdIO(dial_readmsg);

	if (dial_readport!=NULL) 	DeletePort(dial_readport);

   	if (dialwin != NULL) {
		PurgeIDCMP(dialwin->UserPort);
		newdial.LeftEdge = dialwin->LeftEdge;	/* Sto for next */
		newdial.TopEdge = dialwin->TopEdge;
		newdial.Width = dialwin->Width;
		newdial.Height = dialwin->Height;
		CloseWindow(dialwin);
	}
	
	dialwin = NULL;
	dial_writemsg = NULL;
	dial_writeport = NULL;
	dial_readmsg = NULL;
	dial_readport = NULL;
	dialmode = 0;
	
	ScreenToFront(screen);
	
}


VOID OpenDialog(ucsc)
BOOL ucsc;
{

	int error;
	UBYTE *errormsg;
	
	if (dialwin!=NULL) {	/* Find dialog and put it up front */
	
		if (ucadscreen) ScreenToFront(screen);
		else	WBenchToFront();
	
		return;	/* We're here already */
	}
	
	errormsg = NULL;
	
	ucadscreen = ucsc;
	
   	if (ucsc) {
		newdial.Screen = screen;
		newdial.Type = CUSTOMSCREEN;
		newdial.MinWidth = 100;
		newdial.MinHeight = 40;
		newdial.MaxWidth = 200;
		newdial.MaxHeight = 300;
	} else {
		newdial.Screen = NULL;
		newdial.Type = WBENCHSCREEN;
		newdial.MinWidth = 100;
		newdial.MinHeight = 40;
		newdial.MaxWidth = -1;
		newdial.MaxHeight = -1;
	}
   

	FOREVER {	/* Cheat so we can get a structured exit */

	   	dialwin = OpenWindow(&newdial);
	   	if (dialwin == NULL) {
			errormsg = "Cannot open dialogue window";
	      		break;
	   	}

		dial_writeport = CreatePort("ucad.con.write",0);
		if (dial_writeport==NULL) {
			errormsg = "Cannot open dialogue write port";
			break;
		}
		
		dial_writemsg = CreateStdIO(dial_writeport);
		if (dial_writemsg==NULL) {
			errormsg = "Cannot open dialogue write msg";
			break;
		}
		
		dial_readport = CreatePort("ucad.con.read",0);
		if (dial_readport==NULL) {
			errormsg = "Cannot open dialogue read port";
			break;
		}
		
		dial_readmsg = CreateStdIO(dial_readport);
		if (dial_readmsg==NULL) {
			errormsg = "Cannot open dialogue read msg";
			break;
		}
		
		error = OpenConsole(dial_writemsg,dial_readmsg,dialwin);
		if (error!=NULL) {
			errormsg = "Cannot open console device";
			break;
		}
		
		DIALO(ucadname);
		DIALO("\n\rDialog window\r\n\n");
		
		if (ucadscreen) ScreenToFront(screen);
		else	WBenchToFront();
		
		QueueRead(dial_readmsg);
		
		prompt();
		
		break;
	}

	if (errormsg!=NULL) {
		closedial();
		return;
	}
	

}

VOID OpenDialogUS()
{

	OpenDialog(TRUE);
}

VOID OpenDialogWB()
{

	OpenDialog(FALSE);
	
}

/*--------------------------Parser Support Code-------------------------*/

/*
main()
{

	hiteof = FALSE;
	yyparse();	

}
*/

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
