#define ARGS(type,name) name,
#define LARG(type,name) name
#define ARGCALL(type,name) name,
#define LARGCALL(type,name) name
TEMPNAME(TEMPARGS)
#undef ARGS
#undef LARG
#undef ARGCALL
#undef LARGCALL
#define ARGS(type,name) type name;
#define LARG(type,name) type name;
#define ARGCALL(type,name) type (*name)();
#define LARGCALL(type,name) type (*name)();
TEMPARGS
#undef ARGS
#undef LARG
#undef ARGCALL
#undef LARGCALL
#undef TEMPARGS
#undef TEMPNAME
