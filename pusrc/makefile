CFLAG8=-L50 +D +C -B +L +F8 +X3,5 -I/pudefs
CFLAGI=-L50 +D +C -B +L +FI +X3,5 -I/pudefs
CFLAGF=-L50 +D +C -B +L +FF +X3,5 -I/pudefs
DEBFLG=-q -g -w
NORFLG=-V +SS

.c.o8:
	cc $(CFLAG8) -o $@ $*.c -dF_881
	
.c.oi:
	cc $(CFLAGI) -o $@ $*.c -dF_IEEE

.c.of:
	cc $(CFLAGF) -o $@ $*.c -dF_FFP

all: /PasteUp /PasteUp_020_881

PASTEOBJ8=pasteup.o8 pastemenu.o8 menuhandle.o8 pglobsect.o8 filereq.o8 \
 scroller.o8 drivreq.o8 units.o8 presets.o8 paintdisplay.o8 iffload.o8 \
 renderers.o8 avail.o8 loadtext.o8 iffr.o8 filesaves.o8 defsreq.o8 \
 startdefs.o8 hatchhandle.o8 loadhatch.o8 rendhatch.o8 alert.o8 portfloat.o8\
 file_handle.o8
 
PASTEICONS8=pasteicons.o8

pglobsect.o8: /pudefs/pglobals.h /pudefs/pcleartext.h

/PasteUp_020_881: $(PASTEOBJ8) $(PASTEICONS8) makefile
 ln $(NORFLG) -o ram:PasteTemp $(PASTEOBJ8) +Cd $(PASTEICONS8) -lm8l32 -lcl32
 copy ram:PasteTemp /PasteUp_020_881
 delete ram:PasteTemp
 ls -l /PasteUp_020_881

PASTEOBJi=pasteup.oi pastemenu.oi menuhandle.oi pglobsect.oi filereq.oi \
 scroller.oi drivreq.oi units.oi presets.oi paintdisplay.oi iffload.oi \
 renderers.oi avail.oi loadtext.oi iffr.oi filesaves.oi defsreq.oi \
 startdefs.oi hatchhandle.oi loadhatch.oi rendhatch.oi alert.oi portfloat.oi\
 file_handle.oi
 
PASTEICONSi=pasteicons.oi

pglobsect.oi: /pudefs/pglobals.h /pudefs/pcleartext.h

/PasteUp: $(PASTEOBJi) $(PASTEICONSi) makefile
 ln $(NORFLG) -o ram:PasteTemp $(PASTEOBJi) +Cd $(PASTEICONSi) -lmal32 -lcl32
 copy ram:PasteTemp /PasteUp
 delete ram:PasteTemp
 ls -l /PasteUp

PASTEOBJf=pasteup.of pastemenu.of menuhandle.of pglobsect.of filereq.of \
 scroller.of drivreq.of units.of presets.of paintdisplay.of iffload.of \
 renderers.of avail.of loadtext.of iffr.of filesaves.of defsreq.of \
 startdefs.of hatchhandle.of loadhatch.of rendhatch.of alert.of portfloat.of\
 file_handle.oi
 
PASTEICONSf=pasteicons.of

pglobsect.of: /pudefs/pglobals.h /pudefs/pcleartext.h

/PasteUpFFP: $(PASTEOBJf) $(PASTEICONSf) makefile
 ln $(NORFLG) -o ram:PasteTemp $(PASTEOBJf) +Cd $(PASTEICONSf) -lml32 -lcl32
 copy ram:PasteTemp /PasteUpFFP
 delete ram:PasteTemp
 ls -l /PasteUpFFP
