ROLANDOBJ=rolandbase.o roland.o plotcommon.o
HPGLOBJ=hpglbase.o hpgl.o plotcommon.o
DMPLOBJ=dmplbase.o dmpl.o plotcommon.o
EPSOBJ=psbase.o eps.o plotcommon.o
PPSOBJ=psbase.o pps.o plotcommon.o
PREFOBJ=prefbase.o preferences.o plotcommon.o initbases.o
DEFLIBS=-lmal32 -lcl32

CFLAGS=-L50 +D +C -B +B +L +FI -I/pudefs
AFLAGS=-C -D -I -V
LFLAGS= -v +ss -m

all: 	/drivers/Roland.Pdr /drivers/HPGL.Pdr \
	/drivers/DMPL.Pdr /drivers/Preferences.Pdr \
	/drivers/Encapsulated-PS.Pdr /drivers/Print-PS.Pdr

/drivers/Roland.Pdr: $(ROLANDOBJ)
 ln $(LFLAGS) -o /drivers/Roland.Pdr $(ROLANDOBJ) $(DEFLIBS)

/drivers/HPGL.Pdr: $(HPGLOBJ)
 ln $(LFLAGS) -o /drivers/HPGL.Pdr $(HPGLOBJ) $(DEFLIBS)

/drivers/DMPL.Pdr: $(DMPLOBJ)
 ln $(LFLAGS) -o /drivers/DMPL.Pdr $(DMPLOBJ) $(DEFLIBS)

/drivers/Encapsulated-PS.Pdr: $(EPSOBJ)
 ln $(LFLAGS) -o /drivers/Encapsulated-PS.Pdr $(EPSOBJ) $(DEFLIBS)

/drivers/Print-PS.Pdr: $(PPSOBJ)
 ln $(LFLAGS) -o /drivers/Print-PS.Pdr $(PPSOBJ) $(DEFLIBS)

/drivers/Preferences.Pdr: $(PREFOBJ)
 ln $(LFLAGS) -t -o /drivers/Preferences.Pdr $(PREFOBJ) $(DEFLIBS)

eps.o: pscommon.c
	cc $(CFLAGS) -dEPS -o eps.o pscommon.c

pps.o: pscommon.c
	cc $(CFLAGS) -dPPS -o pps.o pscommon.c
