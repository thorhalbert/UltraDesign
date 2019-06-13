lc:lc -iinclude:lattice/ -iinclude: -r -cs uprint.c
lc:blink from lib:c.obj+uprint.o to uprint library lib:lc.lib+lib:amiga.lib verbose
list uprint
copy uprint lc:
delete uprint
delete uprint.o
