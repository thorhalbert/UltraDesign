.c.o8:
	cc $(CFLAG8) -o $@ $*.c
	
.c.oi:
	cc $(CFLAGI) -o $@ $*.c

.c.of:
	cc $(CFLAGF) -o $@ $*.c

.asm.o:
	as -C -D -o $@ $*.asm

CFLAG8=-L50 +D +C -B +L +F8 +X3,5 -I//ucaddefs -dF_881
CFLAGI=-L50 +D +C -B +L +FI +X3,5 -I//ucaddefs -dF_IEEE
CFLAGF=-L50 +D +C -B +L +FF +X3,5 -I//ucaddefs -dF_FFP

all:	/dialog.yacc.tab.o8 /dialog.yacc.tab.oi /dialog.yacc.tab.of

test:	/dialog.yacc.tab.o8

dialog_test.oi: dialog_test.c
	cc $(CFLAGI) -o dialog_test.oi dialog_test.c

dialog_test.o8: dialog_test.c
	cc $(CFLAG8) -o dialog_test.o8 dialog_test.c

dialog_test.of: dialog_test.c
	cc $(CFLAGF) -o dialog_test.of dialog_test.c

/dialog.yacc.tab.oi: lex.yy.c dialog.yacc.tab.c
	cc $(CFLAGI) -Y500 -o /dialog.yacc.tab.oi dialog.yacc.tab.c

/dialog.yacc.tab.o8: lex.yy.c dialog.yacc.tab.c
	cc $(CFLAG8) -Y500 -o /dialog.yacc.tab.o8 dialog.yacc.tab.c

/dialog.yacc.tab.of: lex.yy.c dialog.yacc.tab.c
	cc $(CFLAGF) -Y500 -o /dialog.yacc.tab.of dialog.yacc.tab.c

lex.yy.c:	dialog.lex 
	stack 40000
	flex dialog.lex
	
dialog.yacc.tab.c:	dialog.yacc 
	stack 40000
	bison dialog.yacc

