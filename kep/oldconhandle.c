#ifdef OLDCONSOLEHANDLER	/* Yuck */

#define DIALO(ins) ConPutStr(dial_writemsg,ins)

SHORT	WBLeftEdge = 0,
	WBTopEdge = 20,
	WBWidth = 640,
	WBHeight = 100,
	USLeftEdge = 50,
	USTopEdge = 300,
	USWidth = 300,
	USHeight = 75;

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

struct IOStdReq *dial_writemsg=NULL,*dial_readmsg=NULL;
struct MsgPort *dial_writeport=NULL,*dial_readport=NULL;

int OpenConsole(writerequest,readrequest,window)
struct IOStdReq *writerequest;
struct IOStdReq *readrequest;
struct Window *window;
{

	int error;
	
	writerequest->io_Data = (VOID *) window;
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
	request->io_Data = (VOID *) &ibuf[0];
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
	
	if (obuf[0]=='\t') obuf[0] = ' ';
	
	if (temp=='\n'||temp=='\r') {
		DIALOCRLF;
		linbuf[lchr++] = '\0';
		ProcessLine(linbuf,DIALOG_HOOK);
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
	request->io_Data = (VOID *) string;
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
		if (ucadscreen) {
			USLeftEdge = dialwin->LeftEdge;	/* Sto for next */
			USTopEdge = dialwin->TopEdge;
			USWidth = dialwin->Width;
			USHeight = dialwin->Height;
		} else {
			WBLeftEdge = dialwin->LeftEdge;	/* Sto for next */
			WBTopEdge = dialwin->TopEdge;
			WBWidth = dialwin->Width;
			WBHeight = dialwin->Height;
		}
		CloseWindow(dialwin);
	}
	
	dialwin = NULL;
	dial_writemsg = NULL;
	dial_writeport = NULL;
	dial_readmsg = NULL;
	dial_readport = NULL;
	
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
		newdial.LeftEdge = USLeftEdge;
		newdial.TopEdge = USTopEdge;
		newdial.Width = USWidth;
		newdial.Height = USHeight;
		newdial.MinWidth = 100;		/*Not enough chip */
		newdial.MinHeight = 40;
		newdial.MaxWidth = 320;
		newdial.MaxHeight = 200;
	} else {
		newdial.Screen = NULL;
		newdial.Type = WBENCHSCREEN;
		newdial.LeftEdge = WBLeftEdge;
		newdial.TopEdge = WBTopEdge;
		newdial.Width = WBWidth;
		newdial.Height = WBHeight;
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

#endif /* OLDCONSOLEHANDLER */
