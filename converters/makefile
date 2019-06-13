.c.o8:
	cc $(CFLAG8) -o $@ $*.c
	
.c.oi:
	cc $(CFLAGi) -o $@ $*.c

.c.of:
	cc $(CFLAGf) -o $@ $*.c

all: /CADVerter /CADVerter_020_881

BASICS8=/ucadsrc/memutils.o8 /ucadsrc/amigahooks.o8 /ucadsrc/attrib_basics.o8 \
 /ucadsrc/pools.o8 /ucadsrc/portfloat.o8 /ucadsrc/globsect.o8 \
 /ucadsrc/stylehandle.o8 /ucadsrc/layerhandle.o8 /ucadsrc/grouphandle.o8 \
 /ucadsrc/penhandle.o8 /ucadsrc/loadtext.o8 /ucadsrc/painting.o8 \
 /ucadsrc/renderers.o8 /ucadsrc/scroller.o8 /ucadsrc/palette.o8 \
 /ucadsrc/marginreq.o8 /ucadsrc/units.o8 /ucadsrc/hatchhandle.o8 \
 /ucadsrc/loadhatch.o8 /ucadsrc/alert.o8 /ucadsrc/file_handle.o8
 
READCODE8=/ucadsrc/iffr.o8 /ucadsrc/iffget0.o8 dxf-reader.o8 intro-reader.o8 \
  aegis-reader.o8 dxy-reader.o8 hpgl-reader.o8 plotfuns.o8

ILBM8=ilbmw.o8 packer.o8 putpict.o8

WRITECODE8=/ucadsrc/iffw.o8 /ucadsrc/iffput0.o8 $(ILBM8) puthatch.o8 \
  dxf-writer.o8

ICONS8=/ucadsrc/miscicons.o8

CADVERT8=cadverter.o8 cadvertmenu.o8 menuhandle.o8 filereq.o8 startdefs.o8

BASICSi=/ucadsrc/memutils.oi /ucadsrc/amigahooks.oi /ucadsrc/attrib_basics.oi \
 /ucadsrc/pools.oi /ucadsrc/portfloat.oi /ucadsrc/globsect.oi \
 /ucadsrc/stylehandle.oi /ucadsrc/layerhandle.oi /ucadsrc/grouphandle.oi \
 /ucadsrc/penhandle.oi /ucadsrc/loadtext.oi /ucadsrc/painting.oi \
 /ucadsrc/renderers.oi /ucadsrc/scroller.oi /ucadsrc/palette.oi \
 /ucadsrc/marginreq.oi /ucadsrc/units.oi /ucadsrc/hatchhandle.oi \
 /ucadsrc/loadhatch.oi /ucadsrc/alert.oi /ucadsrc/file_handle.oi
 
READCODEi=/ucadsrc/iffr.oi /ucadsrc/iffget0.oi dxf-reader.oi intro-reader.oi \
  aegis-reader.oi dxy-reader.oi hpgl-reader.oi plotfuns.oi

ILBMi=ilbmw.oi packer.oi putpict.oi

WRITECODEi=/ucadsrc/iffw.oi /ucadsrc/iffput0.oi $(ILBMi) puthatch.oi \
  dxf-writer.oi

ICONSi=/ucadsrc/miscicons.oi

CADVERTi=cadverter.oi cadvertmenu.oi menuhandle.oi filereq.oi startdefs.oi

BASICSf=/ucadsrc/memutils.of /ucadsrc/amigahooks.of /ucadsrc/attrib_basics.of \
 /ucadsrc/pools.of /ucadsrc/portfloat.of /ucadsrc/globsect.of \
 /ucadsrc/stylehandle.of /ucadsrc/layerhandle.of /ucadsrc/grouphandle.of \
 /ucadsrc/penhandle.of /ucadsrc/loadtext.of /ucadsrc/painting.of \
 /ucadsrc/renderers.of /ucadsrc/scroller.of /ucadsrc/palette.of \
 /ucadsrc/marginreq.of /ucadsrc/units.of /ucadsrc/hatchhandle.of \
 /ucadsrc/loadhatch.of /ucadsrc/alert.of /ucadsrc/file_handle.of
 
READCODEf=/ucadsrc/iffr.of /ucadsrc/iffget0.of dxf-reader.of intro-reader.of \
  aegis-reader.of dxy-reader.of hpgl-reader.of plotfuns.of

ILBMf=ilbmw.of packer.of putpict.of

WRITECODEf=/ucadsrc/iffw.of /ucadsrc/iffput0.of $(ILBMi) puthatch.of \
  dxf-writer.of

ICONSf=/ucadsrc/miscicons.of

CADVERTf=cadverter.of cadvertmenu.of menuhandle.of filereq.of startdefs.of

CFLAG8=-L50 +D +C -B +L +F8 +X3,5 -I/ucaddefs -Iiff -dF_881
CFLAGi=-L50 +D +C -B +L +FI +X3,5 -I/ucaddefs -Iiff -dF_IEEE
CFLAGf=-L50 +D +C -B +L +FF +X3,5 -I/ucaddefs -Iiff -dF_FFP

DEBFLG=-q -g -w
NORFLG=-v +ss

/CADVerter_020_881: $(CADVERT8) $(BASICS8) $(READCODE8) $(WRITECODE8) $(ICONS8) makefile
 ln $(NORFLG) -o ram:CadVerter $(CADVERT8) $(BASICS8) $(READCODE8) \
 $(WRITECODE8) +Cd $(ICONS8) -lm8l32 -lcl32
 copy ram:CadVerter /CADVerter_020_881
 delete ram:CadVerter

/CADVerter: $(CADVERTi) $(BASICSi) $(READCODEi) $(WRITECODEi) $(ICONSi) makefile
 ln $(NORFLG) -o ram:CadVerter $(CADVERTi) $(BASICSi) $(READCODEi) \
 $(WRITECODEi) +Cd $(ICONSi) -lmal32 -lcl32
 copy ram:CadVerter /CADVerter
 delete ram:CadVerter

/CADVerterFFP: $(CADVERTf) $(BASICSf) $(READCODEf) $(WRITECODEf) $(ICONSf) makefile
 ln $(NORFLG) -o ram:CadVerter $(CADVERTf) $(BASICSf) $(READCODEf) \
 $(WRITECODEf) +Cd $(ICONSi) -lml32 -lcl32
 copy ram:CadVerter /CADVerterFFP
 delete ram:CadVerter
