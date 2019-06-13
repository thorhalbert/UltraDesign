FDATOBJS=fdat_ufnt.o
DEFLIBS=-lmal32 -lcl32

CFLAGS=-L50 +D +C -B +L +FI 

all: /fdat_ufnt

/fdat_ufnt: $(FDATOBJS) Makefile
 ln -q -g -w -v -o /fdat_ufnt $(FDATOBJS) $(DEFLIBS)
