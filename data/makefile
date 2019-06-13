.c.o8:
	cc $(CFLAG8) -o $@ $*.c
	
all: eatit

CFLAG8=-L50 -Z4000 +D +C -B +L +F8 +X3,5 -dF_881
DEBFLG=-q -g -w
NORFLG=-V +SS -T

eatit: eatit.o8
 ln $(NORFLG) -o eatit eatit.o8 -lm8l32 -lcl32
