.c.o8:
	cc $(CFLAG8) -o $@ $*.c
	
.c.oi:
	cc $(CFLAGI) -o $@ $*.c

.c.of:
	cc $(CFLAGF) -o $@ $*.c

.asm.o:
	as -C -D -o $@ $*.asm

all: dialogstuff /UltraDesign_020_881 /UltraDesign /UltraDesignIEEE

test: testdialog /UltraDesign_020_881

UCADOBJ8=amigahooks.o8 memutils.o8 clipboard.o8 clonedrag.o8 dimsmode.o8\
 dimsreq.o8 menumapper.o8 ellipses.o8 fancys.o8 filereq.o8\
 globsect.o8 groupreq.o8 iffget0.o8 ifflib0.o8 iffput0.o8 iffr.o8 iffw.o8\
 layerreq.o8 linemode.o8 loadtext.o8 marginreq.o8 origins.o8\
 palette.o8 rexxhooks.o8 alert.o8 file_handle.o8\
 penreq.o8 pointers.o8 pools.o8 portfloat.o8 razor.o8 renderers.o8 rollem.o8\
 screens.o8 screensaver.o8 scroller.o8 searcher.o8 snaps.o8 stylereq.o8\
 textmode.o8 ucad1.o8 units.o8 virwindow.o8 zooms.o8 interrupts.o8 startdefs.o8\
 dialogue.o8 attrib_basics.o8 stylehandle.o8 layerhandle.o8 grouphandle.o8\
 penhandle.o8 painting.o8 hatchmode.o8 hatchhandle.o8 loadhatch.o8 \
 dialog.yacc.tab.o8 rexxglue.o boxreq.o8 object_editor.o8

ICONOBJ8=dimsicons.o8 att_icons.o8 eli_icons.o8 lineicons.o8 miscicons.o8\
 texticons.o8 hatchicons.o8 objecticons.o8

UCADOBJI=amigahooks.oi memutils.oi clipboard.oi clonedrag.oi dimsmode.oi\
 dimsreq.oi menumapper.oi ellipses.oi fancys.oi filereq.oi\
 globsect.oi groupreq.oi iffget0.oi ifflib0.oi iffput0.oi iffr.oi iffw.oi\
 layerreq.oi linemode.oi loadtext.oi marginreq.oi origins.oi\
 palette.oi rexxhooks.oi alert.oi file_handle.oi\
 penreq.oi pointers.oi pools.oi portfloat.oi razor.oi renderers.oi rollem.oi\
 screens.oi screensaver.oi scroller.oi searcher.oi snaps.oi stylereq.oi\
 textmode.oi ucad1.oi units.oi virwindow.oi zooms.oi interrupts.oi startdefs.oi\
 dialogue.oi attrib_basics.oi stylehandle.oi layerhandle.oi grouphandle.oi\
 penhandle.oi painting.oi hatchmode.oi hatchhandle.oi loadhatch.oi \
 dialog.yacc.tab.oi rexxglue.o boxreq.oi object_editor.oi

ICONOBJI=dimsicons.oi att_icons.oi eli_icons.oi lineicons.oi miscicons.oi\
 texticons.oi hatchicons.oi objecticons.oi

UCADOBJF=amigahooks.of memutils.of clipboard.of clonedrag.of dimsmode.of\
 dimsreq.of menumapper.of ellipses.of fancys.of filereq.of\
 globsect.of groupreq.of iffget0.of ifflib0.of iffput0.of iffr.of iffw.of\
 layerreq.of linemode.of loadtext.of marginreq.of origins.of\
 palette.of rexxhooks.of alert.of file_handle.of\
 penreq.of pointers.of pools.of portfloat.of razor.of renderers.of rollem.of\
 screens.of screensaver.of scroller.of searcher.of snaps.of stylereq.of\
 textmode.of ucad1.of units.of virwindow.of zooms.of interrupts.of startdefs.of\
 dialogue.of attrib_basics.of stylehandle.of layerhandle.of grouphandle.of\
 penhandle.of painting.of hatchmode.of hatchhandle.of loadhatch.of \
 dialog.yacc.tab.of rexxglue.o boxreq.of object_editor.of 
 
ICONOBJF=dimsicons.of att_icons.of eli_icons.of lineicons.of miscicons.of\
 texticons.of hatchicons.of objecticons.of
 
CFLAG8=-L50 -Z4000 +D +C -B +L +F8 +X3,5 -I/ucaddefs -dF_881
CFLAGI=-L50 -Z4000 +D +C -B +L +FI +X3,5 -I/ucaddefs -dF_IEEE
CFLAGF=-L50 -Z4000 +D +C -B +L +FF +X3,5 -I/ucaddefs -dF_FFP
DEBFLG=-q -g -w
NORFLG=-V +SS -T

dialogstuff:
 cd dialog
 make
 cd /
 
testdialog:
 cd dialog
 make test
 cd /

/UltraDesign_020_881: $(UCADOBJ8) $(ICONOBJ8) makefile
 ln $(NORFLG) -o ram:UcadTemp $(UCADOBJ8) +Cd $(ICONOBJ8) -lm8l32 -lcl32
 copy ram:UcadTemp /UltraDesign_020_881
 delete ram:UcadTemp
 ls -l /UltraDesign_020_881

/UltraDesignIEEE: $(UCADOBJI) $(ICONOBJI) makefile
 ln $(NORFLG) -o ram:UcadTemp $(UCADOBJI) +Cd $(ICONOBJI) -lmal32 -lcl32
 copy ram:UcadTemp /UltraDesignIEEE
 delete ram:UcadTemp
 ls -l /UltraDesignIEEE

/UltraDesign: $(UCADOBJF) $(ICONOBJF) makefile
 ln $(NORFLG) -o ram:UcadTemp $(UCADOBJF) +Cd $(ICONOBJF) -lml32 -lcl32
 copy ram:UcadTemp /UltraDesign
 delete ram:UcadTemp
 ls -l /UltraDesign

globsect.o8: /ucaddefs/globals.h /ucaddefs/cleartext.h
fancys.o8 screens.o8 virwindow.o8: /ucaddefs/versions.h

globsect.oi: /ucaddefs/globals.h /ucaddefs/cleartext.h
fancys.oi screens.oi virwindow.oi: /ucaddefs/versions.h

globsect.of: /ucaddefs/globals.h /ucaddefs/cleartext.h
fancys.of screens.of virwindow.of: /ucaddefs/versions.h

menumapper.o8: /ucaddefs/keymapping.h /ucaddefs/internalfun.h
menumapper.oi: /ucaddefs/keymapping.h /ucaddefs/internalfun.h
menumapper.of: /ucaddefs/keymapping.h /ucaddefs/internalfun.h
