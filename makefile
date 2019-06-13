UCAD:
	cd ucadsrc
	-make
	cd /

test:
	cd ucadsrc
	make test
	cd /

others:
	cd pusrc
	-make
	cd /
	cd pdrsrc
	-make
	cd /
	cd converters
	-make
	cd /

update:
	zoo a update.zoo UltraDesign Pasteup Cadverter UPDATES.README
	zoo a update_020_881.zoo UltraDesign_020_881 Pasteup_020_881\
	 Cadverter_020_881 UPDATES.README

IEEE:
	-delete UcadIEEE.zoo
	zoo a UcadIEEE.zoo UltraDesignIEEE* CadverterIEEE* PasteUpIEEE* \
	  drivers/* ultradesign.config \
	  hatch_lib/* stroke_lib/*

881:
	-delete Ucad881.zoo
	zoo a Ucad881.zoo UltraDesign881* Cadverter881* PasteUp881* \
	  drivers/* ultradesign.config \
	  hatch_lib/* stroke_lib/*

UCADBINS=UltraDesign \
 UltraDesign.info \
 PasteUp \
 PasteUp.info \
 CADVerter \
 CADVerter.info

UCAD020BINS=UltraDesign_020_881 \
 UltraDesign_020_881.info \
 PasteUp_020_881 \
 PasteUp_020_881.info \
 CADVerter_020_881 \
 CADVerter_020_881.info

# UltraDesign.menus 
UCADSTDS=UltraDesign.config \
 Defaults.uvw \
 Cadverter.rgb \
 definitions/Paste-Startup.upd

zoo:	ucad.zoo ucad020.zoo

ucad.zoo:	$(UCADBINS) $(UCADSTDS)
	zoo un ucad.zoo $(UCADBINS) $(UCADSTDS) drivers/* stroke_lib/* readme*
	
ucad020.zoo:	$(UCAD020BINS) $(UCADSTDS)
	zoo un ucad020.zoo $(UCAD020BINS) $(UCADSTDS) drivers/* stroke_lib/* reame*

BACKS=makefile \
 ultradesign.config \
 ultradesign.menus \
 ucaddefs/*.h \
 ucadsrc/*.c \
 ucadsrc/makefile \
 ucadsrc/dialog/*.yacc \
 ucadsrc/dialog/*.lex \
 ucadsrc/dialog/makefile \
 converters/makefile \
 converters/*.c \
 converters/*.h \
 pdrsrc/*.asm \
 pdrsrc/*.c \
 pdrsrc/makefile \
 pusrc/*.c \
 pusrc/makefile \
 pudefs/*.h

bak:
	zoo un ucadsrc.zoo $(BACKS)


TARS=	UltraDesign Ultradesign.config Ultradesign.menus \
	Cadverter Cadverter.rgb \
	Pasteup
	
TARS020=	UltraDesign_020_881 Ultradesign.config Ultradesign.menus \
	Cadverter_020_881 Cadverter.rgb \
	Pasteup_020_881
	
tar:	tardist tar020

tardist: $(TARS)
	tar -cvf ram:ucad.tar $(TARS) hatch_lib stroke_lib drivers
	compress ram:ucad.tar
	copy ram:ucad.tar.Z ucaddist.tar.Z
	delete ram:ucad.tar#?

tar020: $(TARS020)
	tar -cvf ram:ucad.tar $(TARS020) hatch_lib stroke_lib drivers
	compress ram:ucad.tar
	copy ram:ucad.tar.Z ucaddist020.tar.Z
	delete ram:ucad.tar#?
