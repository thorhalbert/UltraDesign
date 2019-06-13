
/*  A Bison parser, made from dialog.yacc  */

#define	SLASH_LOG	3
#define	SLASH_NOLOG	4
#define	SLASH_QUIET	5
#define	SLASH_NOQUIET	6
#define	SLASH_CONFIRM	7
#define	SLASH_NOCONFIRM	8
#define	SLASH_POPOUT	9
#define	SLASH_NOPOPOUT	10
#define	SLASH_VIEW	11
#define	SLASH_LIBRARY	12
#define	SLASH_FONT	13
#define	SLASH_HATCH	14
#define	SLASH_GROUP	15
#define	SLASH_LAYER	16
#define	SLASH_PEN	17
#define	SLASH_STYLE	18
#define	SLASH_ROOT	19
#define	SLASH_CURRENT	20
#define	SLASH_ASSEMBLY	21
#define	SLASH_NOASSEMBLY	22
#define	SLASH_LOCK	23
#define	SLASH_NOLOCK	24
#define	SLASH_GHOST	25
#define	SLASH_NOGHOST	26
#define	SLASH_PLOT	27
#define	SLASH_NOPLOT	28
#define	SLASH_DISPLAY	29
#define	SLASH_NODISPLAY	30
#define	SLASH_COMMENT	31
#define	SLASH_COLOR	32
#define	SLASH_HEIGHT	33
#define	SLASH_SHEET	34
#define	SLASH_SLANT	35
#define	SLASH_ASPECT	36
#define	SLASH_TEMPLATE	37
#define	SLASH_HARDCOLOR	38
#define	SLASH_QUIT	39
#define	SLASH_ROTATE	40
#define	SLASH_START	41
#define	SLASH_END	42
#define	SLASH_CONT	43
#define	SLASH_FLIP	44
#define	SLASH_HANDLE	45
#define	SLASH_FILL	46
#define	SLASH_PATTERN	47
#define	SLASH_FILE	48
#define	HATCH	49
#define	TEXTC	50
#define	EQUALS	51
#define	LOAD	52
#define	SAVE	53
#define	MERGE	54
#define	USE	55
#define	NEW	56
#define	SET	57
#define	GROUP	58
#define	LAYER	59
#define	LINE	60
#define	ELLIPSE	61
#define	PEN	62
#define	STYLE	63
#define	RENAME	64
#define	EXPUNGE	65
#define	DELETE	66
#define	COLOR	67
#define	CREATE	68
#define	CURRENT	69
#define	TAKE	70
#define	EXIT	71
#define	COMMA	72
#define	SLASH_ABS	73
#define	SLASH_REL	74
#define	SLASH_ANGLE	75
#define	SLASH_TURTLE	76
#define	SLASH_TOARROW	77
#define	SLASH_FROMARROW	78
#define	STRING	79
#define	SIMPLE_NUMBER	80
#define	FLOAT_NUMBER	81
#define	HEX_NUMBER	82
#define	TEXT_STRING	83
#define	ERROR_TEXT	84
#define	AT_SIGN	85
#define	VARIABLE	86
#define	CLIP	87
#define	SLASH_DRAG	88
#define	SLASH_OFFSET	89
#define	SLASH_CLONE	90
#define	SLASH_FROM	91
#define	SLASH_TO	92
#define	SLASH_STEADY	93
#define	SLASH_SCALE	94
#define	SLASH_FACTOR	95
#define	SLASH_X	96
#define	SLASH_Y	97
#define	SLASH_CENTER	98

#line 1 "dialog.yacc"


#include "ucad.h"
#include "dialog.h"

/*
 *     +++
 *     +++
 *  +++++++++	The variable type binding
 *  +++++++++
 *     +++
 *     +++
 */

#define YYSTYPE union __yystype

YYSTYPE {
	void	*voidt;
	char	*ctype;
	STDFLT	ftype;
	LONG	itype;
	struct	coordset coord;
};

BOOL	parerr=FALSE;

STDFLT ParseLinear();
STDFLT ParseAngular();
#define dolin(a,b) ParseLinear(a)
#define doang(a,b) ParseAngular(a)


#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#define	YYACCEPT	return(0)
#define	YYABORT	return(1)
#define	YYERROR	return(1)
#ifndef YYSTYPE
#define YYSTYPE int
#endif
#include <stdio.h>

#ifdef __STDC__
#define CONST const
#else
#define CONST
#endif



#define	YYFINAL		409
#define	YYFLAG		-32768
#define	YYNTBASE	99

#define YYTRANSLATE(x) (x)

static CONST short yyrline[] = {     0,
   169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
   179,   180,   181,   182,   183,   184,   185,   186,   199,   201,
   203,   205,   209,   211,   213,   215,   219,   221,   225,   229,
   233,   237,   239,   241,   243,   245,   249,   251,   253,   255,
   257,   261,   263,   265,   267,   269,   273,   275,   277,   282,
   284,   286,   290,   298,   305,   310,   315,   318,   322,   326,
   330,   333,   337,   341,   345,   349,   353,   358,   362,   366,
   370,   375,   379,   383,   387,   403,   404,   406,   408,   410,
   412,   414,   416,   418,   431,   432,   433,   435,   437,   439,
   452,   453,   454,   456,   458,   460,   473,   475,   477,   479,
   492,   494,   496,   498,   513,   515,   517,   519,   523,   524,
   525,   527,   540,   542,   544,   546,   561,   563,   577,   579,
   581,   595,   599,   600,   601,   604,   608,   609,   610,   612,
   616,   617,   618,   619,   620,   622,   624,   626,   628,   630,
   632,   634,   636,   639,   643,   647,   648,   649,   650,   651,
   652,   653,   654,   655,   658,   662,   666,   670,   674,   678,
   682,   685,   688,   691,   694,   697,   700,   703,   706,   709,
   712,   715,   718,   721,   724,   727,   741,   754,   757,   760,
   763,   766,   769,   772,   774,   777,   780,   783,   786,   789,
   792,   795,   797,   800,   802,   804,   806,   808,   810,   823,
   825,   827,   829,   831,   833,   835,   837,   850,   853,   854,
   856,   858,   860,   864,   866,   879,   880,   882,   884,   888,
   894,   900,   906,   924,   925,   927,   929,   931,   935,   937,
   950,   951,   953,   955,   957,   962,   967,   982,   984,   986,
   988,  1001,  1003,  1005,  1007,  1009,  1011,  1024,  1028,  1040,
  1043,  1047,  1051,  1053,  1055,  1058,  1061,  1064,  1067,  1069,
  1071,  1073
};

static CONST char * yytname[] = {     0,
"error","$illegal.","SLASH_LOG","SLASH_NOLOG","SLASH_QUIET","SLASH_NOQUIET","SLASH_CONFIRM","SLASH_NOCONFIRM","SLASH_POPOUT","SLASH_NOPOPOUT",
"SLASH_VIEW","SLASH_LIBRARY","SLASH_FONT","SLASH_HATCH","SLASH_GROUP","SLASH_LAYER","SLASH_PEN","SLASH_STYLE","SLASH_ROOT","SLASH_CURRENT",
"SLASH_ASSEMBLY","SLASH_NOASSEMBLY","SLASH_LOCK","SLASH_NOLOCK","SLASH_GHOST","SLASH_NOGHOST","SLASH_PLOT","SLASH_NOPLOT","SLASH_DISPLAY","SLASH_NODISPLAY",
"SLASH_COMMENT","SLASH_COLOR","SLASH_HEIGHT","SLASH_SHEET","SLASH_SLANT","SLASH_ASPECT","SLASH_TEMPLATE","SLASH_HARDCOLOR","SLASH_QUIT","SLASH_ROTATE",
"SLASH_START","SLASH_END","SLASH_CONT","SLASH_FLIP","SLASH_HANDLE","SLASH_FILL","SLASH_PATTERN","SLASH_FILE","HATCH","TEXTC",
"EQUALS","LOAD","SAVE","MERGE","USE","NEW","SET","GROUP","LAYER","LINE",
"ELLIPSE","PEN","STYLE","RENAME","EXPUNGE","DELETE","COLOR","CREATE","CURRENT","TAKE",
"EXIT","COMMA","SLASH_ABS","SLASH_REL","SLASH_ANGLE","SLASH_TURTLE","SLASH_TOARROW","SLASH_FROMARROW","STRING","SIMPLE_NUMBER",
"FLOAT_NUMBER","HEX_NUMBER","TEXT_STRING","ERROR_TEXT","AT_SIGN","VARIABLE","CLIP","SLASH_DRAG","SLASH_OFFSET","SLASH_CLONE",
"SLASH_FROM","SLASH_TO","SLASH_STEADY","SLASH_SCALE","SLASH_FACTOR","SLASH_X","SLASH_Y","SLASH_CENTER","mainloop"
};

static CONST short yyr1[] = {     0,
    99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
    99,    99,    99,    99,    99,    99,    99,    99,   100,   100,
   100,   100,   101,   101,   101,   101,   102,   102,   103,   104,
   105,   106,   106,   106,   106,   106,   107,   107,   107,   107,
   107,   108,   108,   108,   108,   108,   109,   109,   109,   110,
   110,   110,   111,   112,   113,   113,   114,   114,   114,   114,
   114,   114,   114,   115,   115,   115,   115,   116,   116,   116,
   116,   117,   117,   117,   117,   118,   118,   118,   118,   118,
   118,   118,   118,   118,   119,   119,   119,   119,   119,   119,
   120,   120,   120,   120,   120,   120,   121,   121,   121,   121,
   122,   122,   122,   122,   123,   123,   123,   123,   124,   124,
   124,   124,   125,   125,   125,   125,   126,   126,   127,   127,
   127,   128,   129,   129,   129,   130,   131,   131,   131,   131,
   132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
   132,   132,   132,   133,   134,   135,   135,   135,   135,   135,
   135,   135,   135,   135,   136,   137,   138,   139,   140,   141,
   142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
   142,   142,   142,   142,   142,   142,   143,   144,   144,   144,
   144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
   144,   144,   144,   144,   144,   144,   144,   144,   144,   145,
   145,   145,   145,   145,   145,   145,   145,   146,   147,   147,
   147,   147,   147,   148,   148,   149,   149,   149,   149,   150,
   150,   150,   150,   151,   151,   151,   151,   151,   152,   152,
   153,   153,   153,   153,   153,   154,   154,   155,   155,   155,
   155,   156,   156,   156,   156,   156,   156,   157,   158,   159,
   159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
   159,   159
};

static CONST short yyr2[] = {     0,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
     1,     1,     3,     1,     3,     1,     1,     4,     3,     3,
     2,     2,     1,     4,     3,     3,     2,     4,     3,     3,
     2,     4,     3,     3,     2,     2,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     2,     1,     1,     1,     1,
     1,     2,     1,     1,     1,     1,     4,     3,     3,     2,
     4,     3,     3,     2,     4,     3,     3,     2,     1,     2,
     1,     1,     4,     3,     3,     2,     2,     1,     5,     4,
     4,     3,     1,     2,     1,     3,     1,     2,     1,     1,
     1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     3,     3,     1,     2,     1,     1,     1,
     1,     1,     1,     1,     3,     3,     3,     3,     3,     2,
     4,     3,     3,     2,     4,     3,     3,     2,     4,     3,
     3,     2,     4,     3,     3,     2,     2,     6,     5,     5,
     5,     4,     4,     3,     4,     6,     5,     5,     5,     4,
     4,     3,     4,     2,     1,     4,     3,     3,     2,     7,
     6,     7,     5,     7,     6,     7,     4,     1,     2,     1,
     1,     1,     1,     3,     3,     2,     3,     3,     3,     4,
     5,     6,     7,     2,     3,     3,     3,     3,     2,     2,
     2,     3,     3,     3,     3,     3,     4,     4,     3,     3,
     2,     4,     3,     3,     2,     2,     1,     1,     3,     5,
     8,     8,     5,     5,     8,     8,     8,     8,     5,     6,
     5,     6
};

static CONST short yydefact[] = {     0,
     0,     0,     0,     0,     0,     0,   118,     0,     0,     0,
     0,   195,     0,     0,     0,   247,   248,     0,     8,     9,
    10,     5,     6,     7,     4,     3,     1,     2,    11,    12,
    13,    14,    15,    16,     0,    17,    18,     0,   229,     0,
     0,     0,   230,    37,    39,    41,    40,    38,    54,     0,
    56,     0,    77,    78,    80,    79,    81,    82,    83,    84,
    87,    88,    89,    90,    20,    21,    22,    19,   100,    85,
     0,   104,     0,    93,    94,    95,    96,   111,   112,    20,
    21,    22,    19,   108,     0,    85,   109,     0,     0,   116,
     0,   117,     0,     0,     0,     0,     0,     0,   160,     0,
    32,    33,    36,    35,    34,     0,    57,    63,   208,     0,
     0,    24,    25,    26,    23,     0,    91,     0,   194,   199,
     0,   177,   241,     0,   246,    29,   245,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   224,     0,    30,
   236,    98,    76,    99,    86,   102,   103,   106,   192,     0,
     0,    92,   107,   110,   114,   115,   129,   130,   135,   136,
   137,   138,   139,   140,   141,   142,     0,     0,     0,   164,
   131,   133,     0,   127,   134,   143,   168,     0,     0,   172,
     0,   125,   123,     0,     0,     0,     0,     0,   176,   146,
   149,   148,     0,   150,   151,   152,   153,   154,    31,     0,
     0,     0,     0,    57,   213,   210,   211,   212,   215,     0,
    61,    52,    51,    62,   214,     0,   184,     0,     0,   197,
   198,   240,   239,   243,   244,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   249,    42,    44,    46,    45,    43,
   227,   226,   225,    47,    49,    48,   228,    55,     0,     0,
     0,     0,   237,    97,   101,   190,   193,   191,     0,   105,
   113,     0,     0,     0,   163,   162,   128,   132,   167,   166,
     0,   171,   170,   124,     0,     0,     0,     0,     0,   175,
   174,   147,    27,    28,   207,     0,   120,   121,    67,    71,
    75,    62,   209,    53,    60,    59,    50,   220,   182,   185,
   183,     0,   196,   238,   242,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   231,   187,   188,   189,   144,   145,   122,   161,   165,   126,
   169,   155,   156,   157,   158,   159,   173,   203,     0,   119,
    66,    70,    74,    65,    69,    73,    58,     0,     0,     0,
     0,   221,   179,   180,   181,     0,     0,   259,     0,   261,
     0,   250,   253,   254,     0,     0,   233,   234,   232,   235,
   186,   201,    64,    68,    72,     0,     0,     0,   222,   216,
   178,     0,     0,   260,   262,     0,     0,     0,     0,   200,
   219,   217,   218,   223,     0,     0,     0,     0,     0,     0,
   258,   257,   251,   252,   255,   256,     0,     0,     0
};

static CONST short yydefgoto[] = {   407,
    69,    85,   285,   127,   141,   200,   106,    50,   241,   247,
   297,   204,    51,    52,   205,   206,   207,   208,   117,   145,
   152,    19,    20,    21,   154,    22,    23,    24,   172,   274,
   182,   267,   174,   175,   176,   282,   194,   195,   196,   197,
   198,    25,    99,    26,    27,    28,   110,   293,    29,   380,
    30,   138,    31,   321,    32,    33,    34,    35,    36,    37
};

static CONST short yypact[] = {   729,
    13,   210,   575,   575,   625,   575,   740,   314,   164,   210,
   724,   946,   724,   131,   351,   -18,-32768,    76,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,   -21,-32768,-32768,   -17,-32768,    -3,
    17,    26,    10,-32768,-32768,-32768,-32768,-32768,-32768,     7,
-32768,     5,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   740,   710,
   575,   740,   575,-32768,-32768,-32768,-32768,-32768,-32768,   423,
   428,   684,   694,   928,   724,   962,   740,   724,   678,   740,
   575,   740,   491,   590,   529,   643,   656,   724,-32768,   316,
-32768,-32768,-32768,-32768,-32768,    15,    61,-32768,-32768,   316,
    18,-32768,-32768,-32768,-32768,   724,   710,   724,   946,   946,
   724,-32768,   710,   351,    29,-32768,    46,   -55,   -74,    11,
   -50,   -67,   689,   746,   152,   152,    72,    10,   751,-32768,
    25,   740,   710,   740,   740,   740,   740,   928,   946,   724,
   724,   946,   928,   928,   740,   740,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,    51,    62,    64,   850,
   710,-32768,   491,   885,-32768,-32768,   885,   590,    70,   814,
   529,-32768,   885,    77,    80,    82,    84,    92,   893,   710,
-32768,-32768,   643,-32768,-32768,-32768,-32768,-32768,-32768,   -31,
   656,   946,   724,   350,-32768,-32768,-32768,-32768,   316,   689,
   -23,-32768,-32768,    90,   316,   751,   946,   724,   724,   946,
   946,   710,   710,    88,-32768,   111,   130,   -35,   -32,   133,
   135,   136,   137,   150,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   151,   153,
   157,   161,    25,   740,   740,   946,   946,   946,   724,   928,
   740,     5,   134,   152,   850,   850,   850,   885,   885,   885,
   -31,   814,   814,   814,   152,   751,   751,   746,    72,   893,
   893,   893,-32768,-32768,-32768,   460,   946,   946,   -23,   -23,
   -23,   735,   316,-32768,   -23,   -23,   -23,    -5,   946,   946,
   946,   724,   946,   710,-32768,   746,   210,   751,   162,   751,
   167,   210,   210,   210,   210,   751,   152,   746,    29,    29,
    25,   946,   946,   946,-32768,-32768,-32768,   850,   885,-32768,
   814,-32768,-32768,-32768,-32768,-32768,   893,-32768,   460,   946,
   -23,   -23,   -23,   -23,   -23,   -23,   -23,   168,   169,   170,
   751,   119,   946,   946,   946,   122,   148,-32768,   210,-32768,
   210,-32768,   144,   147,   132,   154,-32768,-32768,-32768,-32768,
   946,   710,   -23,   -23,   -23,   746,   746,   746,   119,   119,
   946,   199,   200,-32768,-32768,   202,   204,   207,   208,   710,
-32768,-32768,-32768,   119,   210,   746,   210,   210,   751,   210,
-32768,-32768,-32768,-32768,-32768,-32768,   260,   261,-32768
};

static CONST short yypgoto[] = {-32768,
    41,   229,  -259,  -114,     3,  -187,  -129,   -99,  -254,   -15,
    22,   257,     1,   -10,   264,-32768,-32768,-32768,     2,   331,
    20,-32768,-32768,-32768,    -2,-32768,-32768,-32768,   -83,   -77,
-32768,   -92,   275,   -52,-32768,   -81,-32768,-32768,-32768,-32768,
-32768,-32768,   265,-32768,-32768,-32768,-32768,   -71,-32768,  -296,
-32768,   279,-32768,   140,-32768,-32768,-32768,-32768,-32768,-32768
};


#define	YYLAST		980


static CONST short yytable[] = {   111,
   173,   352,    89,   235,    70,    70,    86,    70,    70,   108,
   224,   330,   191,   286,   193,   308,   124,   181,   310,   226,
   125,   228,   229,   335,    88,   233,   338,   234,   209,   133,
   118,   119,   121,   134,   348,   349,   350,   227,   215,   248,
   231,   232,   249,   192,    72,    84,    90,   135,   283,    38,
   284,   356,    38,   212,   213,   123,    40,    41,    39,    40,
    41,   126,   309,   368,   250,   311,   351,   136,   251,   252,
    70,   143,    70,    70,    70,   326,   137,   265,   139,   372,
   294,   148,   394,   140,   225,    70,   210,   143,    70,   216,
    70,    70,    70,    70,   171,   171,   171,   190,   201,   230,
   108,   262,   272,    42,   150,   191,    42,   280,   126,   191,
   108,   144,   263,   147,   264,   128,   298,   203,   143,   129,
   271,   391,   392,   393,   222,   143,   305,   275,   214,   153,
   276,   156,   277,   211,   278,   218,   192,   212,   213,   220,
   192,   402,   279,    70,   143,    70,    70,    70,    70,    70,
   260,   244,   245,   246,    70,    70,    70,    70,   348,   349,
   350,   306,   296,   130,   223,   131,   212,   213,   256,   132,
   259,   171,   143,   328,   171,   171,   333,   334,   171,   171,
   307,   171,   171,   312,   171,   313,   314,   315,    93,    94,
   190,   143,    95,    96,   190,   331,   191,   191,   191,   337,
   316,   317,   143,   318,   369,   370,   100,   319,   358,   108,
   360,   320,   359,   199,   382,   108,   366,   361,   376,   377,
   378,   287,   383,   143,   304,   292,   388,   192,   192,   192,
   112,   113,   295,   114,   115,   386,   299,   387,   302,   116,
   303,   120,   101,   102,   103,   104,   105,   389,    49,   395,
   396,   379,   397,   191,   398,    70,    70,   399,   400,   408,
   409,    70,    70,   336,   325,   107,   171,   171,   171,   171,
   171,   171,   109,   171,   171,   171,   322,   323,   122,    43,
   253,   190,   190,   190,   192,     0,     0,   339,    44,    45,
    46,    47,    48,   108,    49,     0,   357,     0,     0,   405,
     0,   362,   363,   364,   365,   143,     0,   340,     0,     0,
   341,   342,   343,   149,     0,     0,   151,   347,     0,   353,
   354,   170,   177,   180,   189,     0,   202,     0,     0,   171,
   171,     0,   171,    71,    73,    87,    91,    92,   190,     0,
   143,     0,     0,   371,   217,     0,   219,     0,   384,   221,
   385,     0,     0,    53,    54,    55,    56,    57,    58,    59,
    60,     0,     0,   242,   243,   373,   374,   375,   178,   183,
     0,    93,    94,   390,   381,    95,    96,     0,   257,   258,
    97,     0,    98,     0,   401,     0,   403,   404,     0,   406,
     0,   143,     0,     0,   101,   102,   103,   104,   105,   142,
    49,   266,   146,     0,     0,     0,   270,     0,     0,   273,
     0,     0,     0,     0,    87,     0,     0,     0,     0,    87,
   155,   281,   211,   289,   290,   291,   212,   213,     0,    65,
    66,   288,    67,    68,     0,     0,     0,   -24,   -24,   -24,
   -24,     0,   -25,   -25,   -25,   -25,   300,   301,   268,     0,
     0,   269,   268,     0,   183,   183,     0,   268,     0,     0,
     0,     0,    53,    54,    55,    56,    57,    58,    59,    60,
     0,     0,     0,     0,   254,     0,     0,   255,    87,     0,
     0,     0,     0,    87,    87,     0,   261,   324,     0,     0,
     0,     0,   327,    53,    54,    55,    56,    57,    58,    59,
    60,   -24,   -24,   332,   -24,   -24,   -25,   -25,     0,   -25,
   -25,   157,   158,   159,   160,   161,   162,   163,   164,   165,
   166,   167,   168,     0,     0,     0,     0,   169,     0,     0,
   355,    53,    54,    55,    56,    57,    58,    59,    60,   283,
     0,   284,   268,   268,   329,   367,   183,   183,   183,     0,
     0,   159,   160,   161,   162,   163,   164,   165,   166,   167,
   168,     0,     0,     0,     0,   169,   179,     0,     0,   112,
   113,     0,   114,   115,     0,     0,     0,    53,    54,    55,
    56,    57,    58,    59,    60,    61,    62,    63,    64,     0,
    87,     0,    53,    54,    55,    56,    57,    58,    59,    60,
     0,     0,     0,   268,     0,   183,     0,   112,   113,     0,
   114,   115,   159,   160,   161,   162,   163,   164,   165,   166,
   167,   168,     0,     0,     0,     0,   169,    53,    54,    55,
    56,    57,    58,    59,    60,    61,    62,    63,    64,    74,
    75,    76,    77,    78,    79,    53,    54,    55,    56,    57,
    58,    59,    60,    65,    66,   184,    67,    68,    53,    54,
    55,    56,    57,    58,    59,    60,     0,     0,   112,   113,
     0,   114,   115,   167,     0,   185,   186,   187,   188,   169,
    53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
    63,    64,     0,     0,     0,     0,    78,    79,   -26,   -26,
   -26,   -26,     0,    80,    81,     0,    82,    83,   -23,   -23,
   -23,   -23,    53,    54,    55,    56,    57,    58,    59,    60,
     0,   112,   113,     0,   114,   115,    53,    54,    55,    56,
    57,    58,    59,    60,     0,   199,     0,     0,    74,    75,
    76,    77,    53,    54,    55,    56,    57,    58,    59,    60,
    61,    62,    63,    64,     0,     0,    65,    66,     0,    67,
    68,     0,   -26,   -26,     0,   -26,   -26,   101,   102,   103,
   104,   105,   -23,   -23,     0,   -23,   -23,     1,     2,     0,
     3,     4,     5,     6,     7,     8,     0,     0,     9,    10,
     0,     0,    11,    12,    13,     0,    14,     0,    15,    16,
     0,     0,   112,   113,     0,   114,   115,   296,   344,   345,
   346,   212,   213,     0,    17,    18,    53,    54,    55,    56,
    57,    58,    59,    60,   236,   237,   238,   239,   240,    44,
    45,    46,    47,    48,     0,     0,   159,   160,   161,   162,
   163,   164,   165,   166,   167,   168,     0,     0,     0,     0,
   169,   179,    53,    54,    55,    56,    57,    58,    59,    60,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
   167,   168,     0,     0,     0,     0,   169,    53,    54,    55,
    56,    57,    58,    59,    60,    53,    54,    55,    56,    57,
    58,    59,    60,     0,     0,   184,     0,   159,   160,   161,
   162,   163,   164,   165,   166,   167,   168,     0,     0,     0,
     0,   169,     0,   167,     0,   185,   186,   187,   188,   169,
    53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
    63,    64,     0,     0,     0,     0,    78,    79,    53,    54,
    55,    56,    57,    58,    59,    60,     0,     0,     0,     0,
    74,    75,    76,    77,    53,    54,    55,    56,    57,    58,
    59,    60,     0,     0,     0,     0,   -91,   -91,   -91,   -91
};

static CONST short yycheck[] = {    10,
    93,   298,     5,   133,     3,     4,     5,     6,     7,     9,
   125,   271,    96,   201,    96,    51,    15,    95,    51,    75,
    39,    96,    97,   278,     5,    93,   286,    95,   100,    51,
    11,    12,    13,    51,    40,    41,    42,    93,   110,   139,
    91,    92,    18,    96,     4,     5,     6,    51,    80,    40,
    82,   306,    40,    77,    78,    15,    47,    48,    46,    47,
    48,    80,    98,   318,    40,    98,    72,    51,    44,    45,
    69,    70,    71,    72,    73,   263,    51,   170,    72,   339,
   210,    84,   379,    79,    39,    84,    72,    86,    87,    72,
    89,    90,    91,    92,    93,    94,    95,    96,    97,    89,
   100,    51,   180,    94,    85,   189,    94,   189,    80,   193,
   110,    71,    51,    73,    51,    40,   216,    98,   117,    44,
    51,   376,   377,   378,   123,   124,    39,    51,   107,    89,
    51,    91,    51,    73,    51,   116,   189,    77,    78,   120,
   193,   396,    51,   142,   143,   144,   145,   146,   147,   148,
   153,    80,    81,    82,   153,   154,   155,   156,    40,    41,
    42,    51,    73,    88,   124,    90,    77,    78,   149,    94,
   151,   170,   171,   266,   173,   174,   276,   277,   177,   178,
    51,   180,   181,    51,   183,    51,    51,    51,    58,    59,
   189,   190,    62,    63,   193,   273,   280,   281,   282,   281,
    51,    51,   201,    51,   319,   320,    43,    51,   308,   209,
   310,    51,    51,    80,    93,   215,   316,    51,    51,    51,
    51,   202,    75,   222,   223,   204,    95,   280,   281,   282,
    79,    80,   211,    82,    83,    92,   217,    91,   219,    11,
   221,    13,    79,    80,    81,    82,    83,    94,    85,    51,
    51,   351,    51,   337,    51,   254,   255,    51,    51,     0,
     0,   260,   261,   279,   262,     9,   265,   266,   267,   268,
   269,   270,     9,   272,   273,   274,   257,   258,    14,     1,
   141,   280,   281,   282,   337,    -1,    -1,   286,    79,    80,
    81,    82,    83,   293,    85,    -1,   307,    -1,    -1,   399,
    -1,   312,   313,   314,   315,   304,    -1,   288,    -1,    -1,
   289,   290,   291,    85,    -1,    -1,    88,   296,    -1,   300,
   301,    93,    94,    95,    96,    -1,    98,    -1,    -1,   328,
   329,    -1,   331,     3,     4,     5,     6,     7,   337,    -1,
   339,    -1,    -1,   324,   116,    -1,   118,    -1,   359,   121,
   361,    -1,    -1,     3,     4,     5,     6,     7,     8,     9,
    10,    -1,    -1,   135,   136,   344,   345,   346,    94,    95,
    -1,    58,    59,   372,   355,    62,    63,    -1,   150,   151,
    67,    -1,    69,    -1,   395,    -1,   397,   398,    -1,   400,
    -1,   390,    -1,    -1,    79,    80,    81,    82,    83,    69,
    85,   173,    72,    -1,    -1,    -1,   178,    -1,    -1,   181,
    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    89,
    90,   193,    73,    74,    75,    76,    77,    78,    -1,    79,
    80,   203,    82,    83,    -1,    -1,    -1,    15,    16,    17,
    18,    -1,    15,    16,    17,    18,   218,   219,   174,    -1,
    -1,   177,   178,    -1,   180,   181,    -1,   183,    -1,    -1,
    -1,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
    -1,    -1,    -1,    -1,   144,    -1,    -1,   147,   148,    -1,
    -1,    -1,    -1,   153,   154,    -1,   156,   259,    -1,    -1,
    -1,    -1,   264,     3,     4,     5,     6,     7,     8,     9,
    10,    79,    80,   275,    82,    83,    79,    80,    -1,    82,
    83,    21,    22,    23,    24,    25,    26,    27,    28,    29,
    30,    31,    32,    -1,    -1,    -1,    -1,    37,    -1,    -1,
   302,     3,     4,     5,     6,     7,     8,     9,    10,    80,
    -1,    82,   268,   269,   270,   317,   272,   273,   274,    -1,
    -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
    32,    -1,    -1,    -1,    -1,    37,    38,    -1,    -1,    79,
    80,    -1,    82,    83,    -1,    -1,    -1,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    -1,
   260,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
    -1,    -1,    -1,   329,    -1,   331,    -1,    79,    80,    -1,
    82,    83,    23,    24,    25,    26,    27,    28,    29,    30,
    31,    32,    -1,    -1,    -1,    -1,    37,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,     3,     4,     5,     6,     7,
     8,     9,    10,    79,    80,    13,    82,    83,     3,     4,
     5,     6,     7,     8,     9,    10,    -1,    -1,    79,    80,
    -1,    82,    83,    31,    -1,    33,    34,    35,    36,    37,
     3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
    13,    14,    -1,    -1,    -1,    -1,    19,    20,    15,    16,
    17,    18,    -1,    79,    80,    -1,    82,    83,    15,    16,
    17,    18,     3,     4,     5,     6,     7,     8,     9,    10,
    -1,    79,    80,    -1,    82,    83,     3,     4,     5,     6,
     7,     8,     9,    10,    -1,    80,    -1,    -1,    15,    16,
    17,    18,     3,     4,     5,     6,     7,     8,     9,    10,
    11,    12,    13,    14,    -1,    -1,    79,    80,    -1,    82,
    83,    -1,    79,    80,    -1,    82,    83,    79,    80,    81,
    82,    83,    79,    80,    -1,    82,    83,    49,    50,    -1,
    52,    53,    54,    55,    56,    57,    -1,    -1,    60,    61,
    -1,    -1,    64,    65,    66,    -1,    68,    -1,    70,    71,
    -1,    -1,    79,    80,    -1,    82,    83,    73,    74,    75,
    76,    77,    78,    -1,    86,    87,     3,     4,     5,     6,
     7,     8,     9,    10,    79,    80,    81,    82,    83,    79,
    80,    81,    82,    83,    -1,    -1,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,
    37,    38,     3,     4,     5,     6,     7,     8,     9,    10,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
    31,    32,    -1,    -1,    -1,    -1,    37,     3,     4,     5,
     6,     7,     8,     9,    10,     3,     4,     5,     6,     7,
     8,     9,    10,    -1,    -1,    13,    -1,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    -1,    -1,    -1,
    -1,    37,    -1,    31,    -1,    33,    34,    35,    36,    37,
     3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
    13,    14,    -1,    -1,    -1,    -1,    19,    20,     3,     4,
     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    -1,
    15,    16,    17,    18,     3,     4,     5,     6,     7,     8,
     9,    10,    -1,    -1,    -1,    -1,    15,    16,    17,    18
};
#define YYPURE 1

#line 2 "bison.simple"

/* Skeleton output parser for bison,
   copyright (C) 1984 Bob Corbett and Richard Stallman

		       NO WARRANTY

  BECAUSE THIS PROGRAM IS LICENSED FREE OF CHARGE, WE PROVIDE ABSOLUTELY
NO WARRANTY, TO THE EXTENT PERMITTED BY APPLICABLE STATE LAW.  EXCEPT
WHEN OTHERWISE STATED IN WRITING, FREE SOFTWARE FOUNDATION, INC,
RICHARD M. STALLMAN AND/OR OTHER PARTIES PROVIDE THIS PROGRAM "AS IS"
WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY
AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE PROGRAM PROVE
DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR
CORRECTION.

 IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW WILL RICHARD M.
STALLMAN, THE FREE SOFTWARE FOUNDATION, INC., AND/OR ANY OTHER PARTY
WHO MAY MODIFY AND REDISTRIBUTE THIS PROGRAM AS PERMITTED BELOW, BE
LIABLE TO YOU FOR DAMAGES, INCLUDING ANY LOST PROFITS, LOST MONIES, OR
OTHER SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE
USE OR INABILITY TO USE (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR
DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY THIRD PARTIES OR
A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS) THIS
PROGRAM, EVEN IF YOU HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH
DAMAGES, OR FOR ANY CLAIM BY ANY OTHER PARTY.

		GENERAL PUBLIC LICENSE TO COPY

  1. You may copy and distribute verbatim copies of this source file
as you receive it, in any medium, provided that you conspicuously and
appropriately publish on each copy a valid copyright notice "Copyright
(C) 1985 Free Software Foundation, Inc."; and include following the
copyright notice a verbatim copy of the above disclaimer of warranty
and of this License.  You may charge a distribution fee for the
physical act of transferring a copy.

  2. You may modify your copy or copies of this source file or
any portion of it, and copy and distribute such modifications under
the terms of Paragraph 1 above, provided that you also do the following:

    a) cause the modified files to carry prominent notices stating
    that you changed the files and the date of any change; and

    b) cause the whole of any work that you distribute or publish,
    that in whole or in part contains or is a derivative of this
    program or any part thereof, to be licensed at no charge to all
    third parties on terms identical to those contained in this
    License Agreement (except that you may choose to grant more
    extensive warranty protection to third parties, at your option).

    c) You may charge a distribution fee for the physical act of
    transferring a copy, and you may at your option offer warranty
    protection in exchange for a fee.

  3. You may copy and distribute this program or any portion of it in
compiled, executable or object code form under the terms of Paragraphs
1 and 2 above provided that you do the following:

    a) cause each such copy to be accompanied by the
    corresponding machine-readable source code, which must
    be distributed under the terms of Paragraphs 1 and 2 above; or,

    b) cause each such copy to be accompanied by a
    written offer, with no time limit, to give any third party
    free (except for a nominal shipping charge) a machine readable
    copy of the corresponding source code, to be distributed
    under the terms of Paragraphs 1 and 2 above; or,

    c) in the case of a recipient of this program in compiled, executable
    or object code form (without the corresponding source code) you
    shall cause copies you distribute to be accompanied by a copy
    of the written offer of source code which you received along
    with the copy you received.

  4. You may not copy, sublicense, distribute or transfer this program
except as expressly provided under this License Agreement.  Any attempt
otherwise to copy, sublicense, distribute or transfer this program is void and
your rights to use the program under this License agreement shall be
automatically terminated.  However, parties who have received computer
software programs from you with this License Agreement will not have
their licenses terminated so long as such parties remain in full compliance.

  5. If you wish to incorporate parts of this program into other free
programs whose distribution conditions are different, write to the Free
Software Foundation at 675 Mass Ave, Cambridge, MA 02139.  We have not yet
worked out a simple rule that can be stated here, but we will often permit
this.  We will be guided by the two goals of preserving the free status of
all derivatives of our free software and of promoting the sharing and reuse of
software.


In other words, you are welcome to use, share and improve this program.
You are forbidden to forbid anyone else to use, share and improve
what you give them.   Help stamp out software-hoarding!  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYFAIL		goto yyerrlab;

#define YYTERROR	1

#ifndef YYIMPURE
#define YYLEX		yylex()
#endif

#ifndef YYPURE
#define YYLEX		yylex(&yylval, &yylloc)
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYIMPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/

int yydebug = 0;		/*  nonzero means print parse trace	*/

#endif  /* YYIMPURE */


/*  YYMAXDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYMAXDEPTH
#define YYMAXDEPTH 200
#endif

/*  YYMAXLIMIT is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#ifndef YYMAXLIMIT
#define YYMAXLIMIT 10000
#endif

extern void *alloca();

#line 87 "bison.simple"
int
yyparse()
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  YYLTYPE *yylsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYMAXDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYMAXDEPTH];	/*  the semantic value stack		*/
  YYLTYPE yylsa[YYMAXDEPTH];	/*  the location stack			*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */
  YYLTYPE *yyls = yylsa;

  int yymaxdepth = YYMAXDEPTH;

#ifndef YYPURE

  int yychar;
  YYSTYPE yylval;
  YYLTYPE yylloc;

  extern int yydebug;

#endif


  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

  if (yydebug)
    fprintf(stderr, "Starting parse\n");

  yystate = 0;
  yyerrstatus = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
  yylsp = yyls;

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yymaxdepth - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      YYLTYPE *yyls1 = yyls;
      short *yyss1 = yyss;

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yymaxdepth);

      yyss = yyss1; yyvs = yyvs1; yyls = yyls1;
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yymaxdepth >= YYMAXLIMIT)
	yyerror("parser stack overflow");
      yymaxdepth *= 2;
      if (yymaxdepth > YYMAXLIMIT)
	yymaxdepth = YYMAXLIMIT;
      yyss = (short *) alloca ((unsigned)yymaxdepth * sizeof (*yyssp));
      bcopy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      yyls = (YYLTYPE *) alloca ((unsigned)yymaxdepth * sizeof (*yylsp));
      bcopy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
      yyvs = (YYSTYPE *) alloca ((unsigned)yymaxdepth * sizeof (*yyvsp));
      bcopy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yylsp = yyls + size - 1;
      yyvsp = yyvs + size - 1;

      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yymaxdepth);

      if (yyssp >= yyss + yymaxdepth - 1)
	YYERROR;
    }

  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
yyresume:

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

      if (yydebug)
	fprintf(stderr, "Parsing next token; it is %d (%s)\n", yychar, yytname[yychar1]);
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
  *++yylsp = yylloc;

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  yyval = yyvsp[1-yylen]; /* implement default value of the action */

  if (yydebug)
    {
      if (yylen == 1)
	fprintf (stderr, "Reducing 1 value via line %d, ",
		 yyrline[yyn]);
      else
	fprintf (stderr, "Reducing %d values via line %d, ",
		 yylen, yyrline[yyn]);
    }


  switch (yyn) {

case 19:
#line 200 "dialog.yacc"
{ yyval.ctype = Stostring1(yyvsp[0].ctype); ;
    break;}
case 20:
#line 202 "dialog.yacc"
{ yyval.ctype = Stostring2(yyvsp[0].ctype); ;
    break;}
case 21:
#line 204 "dialog.yacc"
{ yyval.ctype = Stostring1(yyvsp[0].ctype); ;
    break;}
case 22:
#line 206 "dialog.yacc"
{ yyval.ctype = Stostring1(yyvsp[0].ctype); ;
    break;}
case 23:
#line 210 "dialog.yacc"
{ yyval.ctype = Stostring1(yyvsp[0].ctype); ;
    break;}
case 24:
#line 212 "dialog.yacc"
{ yyval.ctype = Stostring2(yyvsp[0].ctype); ;
    break;}
case 25:
#line 214 "dialog.yacc"
{ yyval.ctype = Stostring1(yyvsp[0].ctype); ;
    break;}
case 26:
#line 216 "dialog.yacc"
{ yyval.ctype = Stostring1(yyvsp[0].ctype); ;
    break;}
case 27:
#line 220 "dialog.yacc"
{ yyval.itype = Stostring1(yyvsp[0].ctype); ;
    break;}
case 28:
#line 222 "dialog.yacc"
{ yyval.itype = Stostring1(yyvsp[0].ctype); ;
    break;}
case 29:
#line 226 "dialog.yacc"
{ sscanf(yyvsp[0].ctype,"%d",&yyval.itype); ;
    break;}
case 30:
#line 230 "dialog.yacc"
{ yyval.ctype = Stostring2(yyvsp[0].ctype); ;
    break;}
case 31:
#line 234 "dialog.yacc"
{ yyval.itype = atoi(yyvsp[0].ctype); ;
    break;}
case 32:
#line 238 "dialog.yacc"
{ yyval.ctype = Stostring2(yyvsp[0].ctype); ;
    break;}
case 33:
#line 240 "dialog.yacc"
{ yyval.ctype = Stostring1(yyvsp[0].ctype); ;
    break;}
case 34:
#line 242 "dialog.yacc"
{ yyval.ctype = Stostring1(yyvsp[0].ctype); ;
    break;}
case 35:
#line 244 "dialog.yacc"
{ yyval.ctype = Stostring1(yyvsp[0].ctype); ;
    break;}
case 36:
#line 246 "dialog.yacc"
{ yyval.ctype = Stostring1(yyvsp[0].ctype); ;
    break;}
case 37:
#line 250 "dialog.yacc"
{ yyval.ftype = dolin(Stostring2(yyvsp[0].ctype), &parerr); ;
    break;}
case 38:
#line 252 "dialog.yacc"
{ yyval.ftype = dolin(yyvsp[0].ctype, &parerr); ;
    break;}
case 39:
#line 254 "dialog.yacc"
{ yyval.ftype = dolin(yyvsp[0].ctype, &parerr); ;
    break;}
case 40:
#line 256 "dialog.yacc"
{ yyval.ftype = dolin(yyvsp[0].ctype, &parerr); ;
    break;}
case 41:
#line 258 "dialog.yacc"
{ yyval.ftype = dolin(yyvsp[0].ctype, &parerr); ;
    break;}
case 42:
#line 262 "dialog.yacc"
{ yyval.ftype = doang(Stostring2(yyvsp[0].ctype), &parerr); ;
    break;}
case 43:
#line 264 "dialog.yacc"
{ yyval.ftype = doang(yyvsp[0].ctype, &parerr); ;
    break;}
case 44:
#line 266 "dialog.yacc"
{ yyval.ftype = doang(yyvsp[0].ctype, &parerr); ;
    break;}
case 45:
#line 268 "dialog.yacc"
{ yyval.ftype = doang(yyvsp[0].ctype, &parerr); ;
    break;}
case 46:
#line 270 "dialog.yacc"
{ yyval.ftype = doang(yyvsp[0].ctype, &parerr); ;
    break;}
case 47:
#line 274 "dialog.yacc"
{ sscanf(yyvsp[0].ctype, "%f", &yyval.ftype); ;
    break;}
case 48:
#line 276 "dialog.yacc"
{ sscanf(yyvsp[0].ctype, "%f", &yyval.ftype); ;
    break;}
case 49:
#line 278 "dialog.yacc"
{ sscanf(yyvsp[0].ctype, "%f", &yyval.ftype); ;
    break;}
case 50:
#line 283 "dialog.yacc"
{ yyval.itype = yyvsp[-1].itype | yyvsp[0].itype ;
    break;}
case 51:
#line 285 "dialog.yacc"
{ yyval.itype = COORD_FROM; ;
    break;}
case 52:
#line 287 "dialog.yacc"
{ yyval.itype = COORD_TO; ;
    break;}
case 53:
#line 291 "dialog.yacc"
{ yyval.coord.a = yyvsp[-2].ctype; 
			  yyval.coord.b = yyvsp[0].ctype;
			  yyval.coord.flags = 0;
			  yyval.coord.ftype = 0; ;
    break;}
case 54:
#line 299 "dialog.yacc"
{ 
			  DM_GetMouse(&yyval.coord); 
			  yyval.coord.flags = 0;
			  yyval.coord.ftype = COORD_ABS; ;
    break;}
case 55:
#line 306 "dialog.yacc"
{ yyval.coord.x = yyvsp[-2].ftype; 
			  yyval.coord.y = yyvsp[0].ftype;
			  yyval.coord.flags = 0;
			  yyval.coord.ftype = COORD_ABS; ;
    break;}
case 56:
#line 311 "dialog.yacc"
{
			  yyval.coord = yyvsp[0].coord; ;
    break;}
case 57:
#line 316 "dialog.yacc"
{ yyval.coord = yyvsp[0].coord;
			  yyval.coord.ftype = COORD_ABS; ;
    break;}
case 58:
#line 319 "dialog.yacc"
{ yyval.coord = yyvsp[-3].coord;
			  yyval.coord.flags = yyvsp[-2].itype | yyvsp[0].itype;
			  yyval.coord.ftype = COORD_ABS; ;
    break;}
case 59:
#line 323 "dialog.yacc"
{ yyval.coord = yyvsp[-2].coord;
			  yyval.coord.flags = yyvsp[-1].itype;
			  yyval.coord.ftype = COORD_ABS; ;
    break;}
case 60:
#line 327 "dialog.yacc"
{ yyval.coord = yyvsp[-2].coord;
			  yyval.coord.flags = yyvsp[0].itype;
			  yyval.coord.ftype = COORD_ABS; ;
    break;}
case 61:
#line 331 "dialog.yacc"
{ yyval.coord = yyvsp[-1].coord;
			  yyval.coord.ftype = COORD_ABS; ;
    break;}
case 62:
#line 334 "dialog.yacc"
{ yyval.coord = yyvsp[-1].coord;
			  yyval.coord.flags = yyvsp[0].itype;
			  yyval.coord.ftype = COORD_ABS; ;
    break;}
case 63:
#line 338 "dialog.yacc"
{ yyval.coord = yyvsp[0].coord; ;
    break;}
case 64:
#line 342 "dialog.yacc"
{ yyval.coord = yyvsp[-3].coord;
			  yyval.coord.flags = yyvsp[-2].itype | yyvsp[0].itype;
			  yyval.coord.ftype = COORD_REL; ;
    break;}
case 65:
#line 346 "dialog.yacc"
{ yyval.coord = yyvsp[-2].coord;
			  yyval.coord.flags = yyvsp[-1].itype;
			  yyval.coord.ftype = COORD_REL; ;
    break;}
case 66:
#line 350 "dialog.yacc"
{ yyval.coord = yyvsp[-2].coord;
			  yyval.coord.flags = yyvsp[0].itype;
			  yyval.coord.ftype = COORD_REL; ;
    break;}
case 67:
#line 354 "dialog.yacc"
{ yyval.coord = yyvsp[-1].coord;
			  yyval.coord.ftype = COORD_REL; ;
    break;}
case 68:
#line 359 "dialog.yacc"
{ yyval.coord = yyvsp[-3].coord;
			  yyval.coord.flags = yyvsp[-2].itype | yyvsp[0].itype;
			  yyval.coord.ftype = COORD_AANG; ;
    break;}
case 69:
#line 363 "dialog.yacc"
{ yyval.coord = yyvsp[-2].coord;
			  yyval.coord.flags = yyvsp[-1].itype;
			  yyval.coord.ftype = COORD_AANG; ;
    break;}
case 70:
#line 367 "dialog.yacc"
{ yyval.coord = yyvsp[-2].coord;
			  yyval.coord.flags = yyvsp[0].itype;
			  yyval.coord.ftype = COORD_AANG; ;
    break;}
case 71:
#line 371 "dialog.yacc"
{ yyval.coord = yyvsp[-1].coord;
			  yyval.coord.ftype = COORD_AANG; ;
    break;}
case 72:
#line 376 "dialog.yacc"
{ yyval.coord = yyvsp[-3].coord;
			  yyval.coord.flags = yyvsp[-2].itype | yyvsp[0].itype;
			  yyval.coord.ftype = COORD_RANG; ;
    break;}
case 73:
#line 380 "dialog.yacc"
{ yyval.coord = yyvsp[-2].coord;
			  yyval.coord.flags = yyvsp[-1].itype;
			  yyval.coord.ftype = COORD_RANG; ;
    break;}
case 74:
#line 384 "dialog.yacc"
{ yyval.coord = yyvsp[-2].coord;
			  yyval.coord.flags = yyvsp[0].itype;
			  yyval.coord.ftype = COORD_RANG; ;
    break;}
case 75:
#line 388 "dialog.yacc"
{ yyval.coord = yyvsp[-1].coord;
			  yyval.coord.ftype = COORD_RANG; ;
    break;}
case 77:
#line 405 "dialog.yacc"
{ __info.log = SLASH_LOG_TYPE; ;
    break;}
case 78:
#line 407 "dialog.yacc"
{ __info.log = SLASH_NOLOG_TYPE; ;
    break;}
case 79:
#line 409 "dialog.yacc"
{ __info.log = SLASH_LOG_TYPE; ;
    break;}
case 80:
#line 411 "dialog.yacc"
{ __info.log = SLASH_NOLOG_TYPE; ;
    break;}
case 81:
#line 413 "dialog.yacc"
{ __info.conf = SLASH_CONFIRM_TYPE; ;
    break;}
case 82:
#line 415 "dialog.yacc"
{ __info.conf = SLASH_NOCONFIRM_TYPE; ;
    break;}
case 83:
#line 417 "dialog.yacc"
{ __info.conf = SLASH_POPOUT_TYPE; ;
    break;}
case 84:
#line 419 "dialog.yacc"
{ __info.conf = SLASH_NOCONFIRM_TYPE; ;
    break;}
case 87:
#line 434 "dialog.yacc"
{ __info.type = SLASH_VIEW_TYPE; ;
    break;}
case 88:
#line 436 "dialog.yacc"
{ __info.type = SLASH_LIBRARY_TYPE; ;
    break;}
case 89:
#line 438 "dialog.yacc"
{ __info.type = SLASH_FONT_TYPE; ;
    break;}
case 90:
#line 440 "dialog.yacc"
{ __info.type = SLASH_HATCH_TYPE; ;
    break;}
case 93:
#line 455 "dialog.yacc"
{ __info.attrib = SLASH_GROUP_TYPE; ;
    break;}
case 94:
#line 457 "dialog.yacc"
{ __info.attrib = SLASH_LAYER_TYPE; ;
    break;}
case 95:
#line 459 "dialog.yacc"
{ __info.attrib = SLASH_PEN_TYPE; ;
    break;}
case 96:
#line 461 "dialog.yacc"
{ __info.attrib = SLASH_STYLE_TYPE; ;
    break;}
case 97:
#line 474 "dialog.yacc"
{ DM_LoadFile(yyvsp[-1].ctype, __info); ;
    break;}
case 98:
#line 476 "dialog.yacc"
{ DM_LoadFile(yyvsp[-1].ctype, __info); ;
    break;}
case 99:
#line 478 "dialog.yacc"
{ DM_LoadFile(yyvsp[0].ctype, __info); ;
    break;}
case 100:
#line 480 "dialog.yacc"
{ DM_LoadFile(yyvsp[0].ctype, __info); ;
    break;}
case 101:
#line 493 "dialog.yacc"
{ DM_SaveFile(yyvsp[-1].ctype, __info); ;
    break;}
case 102:
#line 495 "dialog.yacc"
{ DM_SaveFile(yyvsp[-1].ctype, __info); ;
    break;}
case 103:
#line 497 "dialog.yacc"
{ DM_SaveFile(yyvsp[0].ctype, __info); ;
    break;}
case 104:
#line 499 "dialog.yacc"
{ DM_SaveFile(yyvsp[0].ctype, __info); ;
    break;}
case 105:
#line 514 "dialog.yacc"
{ DM_MergeFile(yyvsp[-1].ctype, __info); ;
    break;}
case 106:
#line 516 "dialog.yacc"
{ DM_MergeFile(yyvsp[-1].ctype, __info); ;
    break;}
case 107:
#line 518 "dialog.yacc"
{ DM_MergeFile(yyvsp[0].ctype, __info); ;
    break;}
case 108:
#line 520 "dialog.yacc"
{ DM_MergeFile(yyvsp[0].ctype, __info); ;
    break;}
case 111:
#line 526 "dialog.yacc"
{ __info.where = SLASH_ROOT_TYPE; ;
    break;}
case 112:
#line 528 "dialog.yacc"
{ __info.where = SLASH_CURRENT_TYPE; ;
    break;}
case 113:
#line 541 "dialog.yacc"
{ DM_UseFile(yyvsp[-1].ctype, __info); ;
    break;}
case 114:
#line 543 "dialog.yacc"
{ DM_UseFile(yyvsp[-1].ctype, __info); ;
    break;}
case 115:
#line 545 "dialog.yacc"
{ DM_UseFile(yyvsp[0].ctype, __info); ;
    break;}
case 116:
#line 547 "dialog.yacc"
{ DM_UseFile(yyvsp[0].ctype, __info); ;
    break;}
case 117:
#line 562 "dialog.yacc"
{ DM_NewBuffer(__info); ;
    break;}
case 118:
#line 564 "dialog.yacc"
{ DM_NewBuffer(__info); ;
    break;}
case 119:
#line 578 "dialog.yacc"
{ DM_SetCurrent(yyvsp[-1].ctype, __info); ;
    break;}
case 120:
#line 580 "dialog.yacc"
{ DM_SetCurrent(yyvsp[-1].ctype, __info); ;
    break;}
case 121:
#line 582 "dialog.yacc"
{ DM_SetCurrent(yyvsp[0].ctype, __info); ;
    break;}
case 122:
#line 596 "dialog.yacc"
{ __info.template = yyvsp[0].ctype; ;
    break;}
case 126:
#line 605 "dialog.yacc"
{ __info.atflag.hardcolor = yyvsp[0].itype; ;
    break;}
case 129:
#line 611 "dialog.yacc"
{ __info.atflag.assembly = 2; ;
    break;}
case 130:
#line 613 "dialog.yacc"
{ __info.atflag.assembly = 1; ;
    break;}
case 135:
#line 621 "dialog.yacc"
{ __info.atflag.lock = 2; ;
    break;}
case 136:
#line 623 "dialog.yacc"
{ __info.atflag.lock = 1; ;
    break;}
case 137:
#line 625 "dialog.yacc"
{ __info.atflag.ghost = 2; ;
    break;}
case 138:
#line 627 "dialog.yacc"
{ __info.atflag.ghost = 1; ;
    break;}
case 139:
#line 629 "dialog.yacc"
{ __info.atflag.plot = 2; ;
    break;}
case 140:
#line 631 "dialog.yacc"
{ __info.atflag.plot = 1; ;
    break;}
case 141:
#line 633 "dialog.yacc"
{ __info.atflag.display = 2; ;
    break;}
case 142:
#line 635 "dialog.yacc"
{ __info.atflag.display = 1; ;
    break;}
case 144:
#line 640 "dialog.yacc"
{ __info.atflag.comment = yyvsp[0].ctype; ;
    break;}
case 145:
#line 644 "dialog.yacc"
{ __info.atflag.color = yyvsp[0].itype; ;
    break;}
case 155:
#line 659 "dialog.yacc"
{ __info.atflag.font = yyvsp[0].ctype; ;
    break;}
case 156:
#line 663 "dialog.yacc"
{ __info.atflag.height = yyvsp[0].ftype; ;
    break;}
case 157:
#line 667 "dialog.yacc"
{ __info.atflag.sheet = yyvsp[0].ftype; ;
    break;}
case 158:
#line 671 "dialog.yacc"
{ __info.atflag.slant = yyvsp[0].ftype; ;
    break;}
case 159:
#line 675 "dialog.yacc"
{ __info.atflag.aspect = yyvsp[0].ftype; ;
    break;}
case 160:
#line 679 "dialog.yacc"
{ DM_SetAttrib(yyvsp[0].ctype, __info); ;
    break;}
case 161:
#line 683 "dialog.yacc"
{ yyval.ctype = yyvsp[-1].ctype;
			  __info.maintype = GROUP_TYPE; ;
    break;}
case 162:
#line 686 "dialog.yacc"
{ yyval.ctype = yyvsp[0].ctype;
			  __info.maintype = GROUP_TYPE; ;
    break;}
case 163:
#line 689 "dialog.yacc"
{ yyval.ctype = yyvsp[-1].ctype;
			  __info.maintype = GROUP_TYPE; ;
    break;}
case 164:
#line 692 "dialog.yacc"
{ yyval.ctype = yyvsp[0].ctype;
			  __info.maintype = GROUP_TYPE; ;
    break;}
case 165:
#line 695 "dialog.yacc"
{ yyval.ctype = yyvsp[-1].ctype;
			  __info.maintype = LAYER_TYPE; ;
    break;}
case 166:
#line 698 "dialog.yacc"
{ yyval.ctype = yyvsp[0].ctype;
			  __info.maintype = LAYER_TYPE; ;
    break;}
case 167:
#line 701 "dialog.yacc"
{ yyval.ctype = yyvsp[-1].ctype;
			  __info.maintype = LAYER_TYPE; ;
    break;}
case 168:
#line 704 "dialog.yacc"
{ yyval.ctype = yyvsp[0].ctype;
			  __info.maintype = LAYER_TYPE; ;
    break;}
case 169:
#line 707 "dialog.yacc"
{ yyval.ctype = yyvsp[-1].ctype;
			  __info.maintype = PEN_TYPE; ;
    break;}
case 170:
#line 710 "dialog.yacc"
{ yyval.ctype = yyvsp[0].ctype;
			  __info.maintype = PEN_TYPE; ;
    break;}
case 171:
#line 713 "dialog.yacc"
{ yyval.ctype = yyvsp[-1].ctype;
			  __info.maintype = PEN_TYPE; ;
    break;}
case 172:
#line 716 "dialog.yacc"
{ yyval.ctype = yyvsp[0].ctype;
			  __info.maintype = PEN_TYPE; ;
    break;}
case 173:
#line 719 "dialog.yacc"
{ yyval.ctype = yyvsp[-1].ctype;
			  __info.maintype = STYLE_TYPE; ;
    break;}
case 174:
#line 722 "dialog.yacc"
{ yyval.ctype = yyvsp[0].ctype;
			  __info.maintype = STYLE_TYPE; ;
    break;}
case 175:
#line 725 "dialog.yacc"
{ yyval.ctype = yyvsp[-1].ctype;
			  __info.maintype = STYLE_TYPE; ;
    break;}
case 176:
#line 728 "dialog.yacc"
{ yyval.ctype = yyvsp[0].ctype;
			  __info.maintype = STYLE_TYPE; ;
    break;}
case 177:
#line 742 "dialog.yacc"
{ DM_CreateAttrib(yyvsp[0].ctype, __info); ;
    break;}
case 178:
#line 756 "dialog.yacc"
{ DM_RenameAttrib(yyvsp[-3].ctype, yyvsp[-1].ctype, __info); ;
    break;}
case 179:
#line 759 "dialog.yacc"
{ DM_RenameAttrib(yyvsp[-3].ctype, yyvsp[-1].ctype, __info); ;
    break;}
case 180:
#line 762 "dialog.yacc"
{ DM_RenameAttrib(yyvsp[-2].ctype, yyvsp[-1].ctype, __info); ;
    break;}
case 181:
#line 765 "dialog.yacc"
{ DM_RenameAttrib(yyvsp[-2].ctype, yyvsp[0].ctype, __info); ;
    break;}
case 182:
#line 768 "dialog.yacc"
{ DM_RenameAttrib(yyvsp[-2].ctype, yyvsp[-1].ctype, __info); ;
    break;}
case 183:
#line 771 "dialog.yacc"
{ DM_RenameAttrib(yyvsp[-1].ctype, yyvsp[0].ctype, __info); ;
    break;}
case 184:
#line 773 "dialog.yacc"
{ DM_RenameAttrib(yyvsp[-1].ctype, yyvsp[0].ctype, __info); ;
    break;}
case 185:
#line 776 "dialog.yacc"
{ DM_RenameAttrib(yyvsp[-2].ctype, yyvsp[0].ctype, __info); ;
    break;}
case 186:
#line 779 "dialog.yacc"
{ DM_MergeAttrib(yyvsp[-3].ctype, yyvsp[-1].ctype, __info); ;
    break;}
case 187:
#line 782 "dialog.yacc"
{ DM_MergeAttrib(yyvsp[-3].ctype, yyvsp[-1].ctype, __info); ;
    break;}
case 188:
#line 785 "dialog.yacc"
{ DM_MergeAttrib(yyvsp[-2].ctype, yyvsp[-1].ctype, __info); ;
    break;}
case 189:
#line 788 "dialog.yacc"
{ DM_MergeAttrib(yyvsp[-2].ctype, yyvsp[0].ctype, __info); ;
    break;}
case 190:
#line 791 "dialog.yacc"
{ DM_MergeAttrib(yyvsp[-2].ctype, yyvsp[-1].ctype, __info); ;
    break;}
case 191:
#line 794 "dialog.yacc"
{ DM_MergeAttrib(yyvsp[-1].ctype, yyvsp[0].ctype, __info); ;
    break;}
case 192:
#line 796 "dialog.yacc"
{ DM_MergeAttrib(yyvsp[-1].ctype, yyvsp[0].ctype, __info); ;
    break;}
case 193:
#line 799 "dialog.yacc"
{ DM_MergeAttrib(yyvsp[-2].ctype, yyvsp[0].ctype, __info); ;
    break;}
case 194:
#line 801 "dialog.yacc"
{ DM_ExpungeAttrib(__info); ;
    break;}
case 195:
#line 803 "dialog.yacc"
{ DM_ExpungeAttrib(__info); ;
    break;}
case 196:
#line 805 "dialog.yacc"
{ DM_DeleteAttrib(yyvsp[-1].ctype, __info); ;
    break;}
case 197:
#line 807 "dialog.yacc"
{ DM_DeleteAttrib(yyvsp[-1].ctype, __info); ;
    break;}
case 198:
#line 809 "dialog.yacc"
{ DM_DeleteAttrib(yyvsp[0].ctype, __info); ;
    break;}
case 199:
#line 811 "dialog.yacc"
{ DM_DeleteAttrib(yyvsp[0].ctype, __info); ;
    break;}
case 200:
#line 824 "dialog.yacc"
{ DM_SetColor(yyvsp[-3].itype,yyvsp[-1].itype); ;
    break;}
case 201:
#line 826 "dialog.yacc"
{ DM_SetColor(yyvsp[-2].itype,yyvsp[0].itype); ;
    break;}
case 202:
#line 828 "dialog.yacc"
{ DM_SetColor(yyvsp[-3].itype,yyvsp[-1].itype); ;
    break;}
case 203:
#line 830 "dialog.yacc"
{ DM_SetColor(yyvsp[-1].itype,yyvsp[0].itype); ;
    break;}
case 204:
#line 832 "dialog.yacc"
{ DM_SetColor(yyvsp[-3].itype,yyvsp[-1].itype); ;
    break;}
case 205:
#line 834 "dialog.yacc"
{ DM_SetColor(yyvsp[-2].itype,yyvsp[0].itype); ;
    break;}
case 206:
#line 836 "dialog.yacc"
{ DM_SetColor(yyvsp[-3].itype,yyvsp[-1].itype); ;
    break;}
case 207:
#line 838 "dialog.yacc"
{ DM_SetColor(yyvsp[-1].itype,yyvsp[0].itype); ;
    break;}
case 208:
#line 851 "dialog.yacc"
{ DM_StartLine(yyvsp[0].coord); ;
    break;}
case 210:
#line 855 "dialog.yacc"
{ DM_MoreLine(yyvsp[0].coord); ;
    break;}
case 211:
#line 857 "dialog.yacc"
{ DM_MoreLine(yyvsp[0].coord); ;
    break;}
case 212:
#line 859 "dialog.yacc"
{ DM_MoreLine(yyvsp[0].coord); ;
    break;}
case 213:
#line 861 "dialog.yacc"
{ DM_MoreLine(yyvsp[0].coord); ;
    break;}
case 214:
#line 865 "dialog.yacc"
{ ;
    break;}
case 215:
#line 867 "dialog.yacc"
{ ;
    break;}
case 217:
#line 881 "dialog.yacc"
{ __obj.sa = yyvsp[0].ftype; ;
    break;}
case 218:
#line 883 "dialog.yacc"
{ __obj.ea = yyvsp[0].ftype; ;
    break;}
case 219:
#line 885 "dialog.yacc"
{ __obj.rot = yyvsp[0].ftype; ;
    break;}
case 220:
#line 889 "dialog.yacc"
{ __obj.x = yyvsp[-2].coord.x;
		  __obj.y = yyvsp[-2].coord.y;
		  __obj.xr = yyvsp[0].ftype;
		  __obj.yr = yyvsp[0].ftype;
		  DM_Ellipse(__obj); ;
    break;}
case 221:
#line 895 "dialog.yacc"
{ __obj.x = yyvsp[-3].coord.x;
		  __obj.y = yyvsp[-3].coord.y;
		  __obj.xr = yyvsp[-1].ftype;
		  __obj.yr = yyvsp[-1].ftype;
		  DM_Ellipse(__obj); ;
    break;}
case 222:
#line 901 "dialog.yacc"
{ __obj.x = yyvsp[-4].coord.x;
		  __obj.y = yyvsp[-4].coord.y;
		  __obj.xr = yyvsp[-2].ftype;
		  __obj.yr = yyvsp[0].ftype;
		  DM_Ellipse(__obj); ;
    break;}
case 223:
#line 907 "dialog.yacc"
{ __obj.x = yyvsp[-5].coord.x;
		  __obj.y = yyvsp[-5].coord.y;
		  __obj.xr = yyvsp[-3].ftype;
		  __obj.yr = yyvsp[-1].ftype;
		  DM_Ellipse(__obj); ;
    break;}
case 225:
#line 926 "dialog.yacc"
{ __obj.file = yyvsp[0].ctype; ;
    break;}
case 226:
#line 928 "dialog.yacc"
{ __obj.pattern = yyvsp[0].ctype; ;
    break;}
case 227:
#line 930 "dialog.yacc"
{ __obj.rot = yyvsp[0].ftype; ;
    break;}
case 228:
#line 932 "dialog.yacc"
{ __obj.scale = yyvsp[0].ftype; ;
    break;}
case 229:
#line 936 "dialog.yacc"
{ DM_HatchFill(); ;
    break;}
case 230:
#line 938 "dialog.yacc"
{ DM_HatchPattern(__obj); ;
    break;}
case 232:
#line 952 "dialog.yacc"
{ __obj.flags |= (yyvsp[0].itype & 0x7); ;
    break;}
case 233:
#line 954 "dialog.yacc"
{ __obj.stylename = yyvsp[0].ctype; ;
    break;}
case 234:
#line 956 "dialog.yacc"
{ __obj.rot = yyvsp[0].ftype; ;
    break;}
case 235:
#line 958 "dialog.yacc"
{ __obj.flags &= ~(3<<8);	/* Mask off */
		  __obj.flags |= (yyvsp[0].itype & 0x3) <<8; ;
    break;}
case 236:
#line 963 "dialog.yacc"
{ __obj.x = yyvsp[-1].coord.x;
		  __obj.y = yyvsp[-1].coord.y;
		  __obj.tstring = yyvsp[0].ctype;
		  DM_Text(__obj); ;
    break;}
case 237:
#line 968 "dialog.yacc"
{ __obj.x = yyvsp[-2].coord.x;
		  __obj.y = yyvsp[-2].coord.y;
		  __obj.tstring = yyvsp[-1].ctype;
		  DM_Text(__obj); ;
    break;}
case 238:
#line 983 "dialog.yacc"
{ DM_TakeFile(__info, yyvsp[-1].ctype); ;
    break;}
case 239:
#line 985 "dialog.yacc"
{ DM_TakeFile(__info, yyvsp[0].ctype); ;
    break;}
case 240:
#line 987 "dialog.yacc"
{ DM_TakeFile(__info, yyvsp[-1].ctype); ;
    break;}
case 241:
#line 989 "dialog.yacc"
{ DM_TakeFile(__info, yyvsp[0].ctype); ;
    break;}
case 242:
#line 1002 "dialog.yacc"
{ DM_Exit(1,yyvsp[-1].itype); ;
    break;}
case 243:
#line 1004 "dialog.yacc"
{ DM_Exit(1,yyvsp[0].itype); ;
    break;}
case 244:
#line 1006 "dialog.yacc"
{ DM_Exit(1,yyvsp[-1].itype); ;
    break;}
case 245:
#line 1008 "dialog.yacc"
{ DM_Exit(0,yyvsp[0].itype); ;
    break;}
case 246:
#line 1010 "dialog.yacc"
{ DM_Exit(1,0); ;
    break;}
case 247:
#line 1012 "dialog.yacc"
{ DM_Exit(0,0); ;
    break;}
case 248:
#line 1025 "dialog.yacc"
{ yyval.ctype = Stostring1(yyvsp[0].ctype); ;
    break;}
case 249:
#line 1029 "dialog.yacc"
{ DM_SetVariable(yyvsp[-2].ctype, yyvsp[0].ctype); ;
    break;}
case 250:
#line 1042 "dialog.yacc"
{ DM_ClipDrag(yyvsp[0].coord.x,yyvsp[0].coord.y); ;
    break;}
case 251:
#line 1045 "dialog.yacc"
{ DM_ClipCloneFrom(yyvsp[-3].coord.x,yyvsp[-3].coord.y);
			  DM_ClipCloneTo(yyvsp[0].coord.x,yyvsp[0].coord.y); ;
    break;}
case 252:
#line 1049 "dialog.yacc"
{ DM_ClipCloneFrom(yyvsp[0].coord.x,yyvsp[0].coord.y);
			  DM_ClipCloneTo(yyvsp[-3].coord.x,yyvsp[-3].coord.y); ;
    break;}
case 253:
#line 1052 "dialog.yacc"
{ DM_ClipCloneFrom(yyvsp[0].coord.x,yyvsp[0].coord.y); ;
    break;}
case 254:
#line 1054 "dialog.yacc"
{ DM_ClipCloneTo(yyvsp[0].coord.x,yyvsp[0].coord.y); ;
    break;}
case 255:
#line 1057 "dialog.yacc"
{ DM_ClipScale(yyvsp[-3].coord.x,yyvsp[-3].coord.y,yyvsp[0].ftype); ;
    break;}
case 256:
#line 1060 "dialog.yacc"
{ DM_ClipScale(yyvsp[0].coord.x,yyvsp[0].coord.y,yyvsp[-3].ftype); ;
    break;}
case 257:
#line 1063 "dialog.yacc"
{ DM_ClipRotate(yyvsp[-3].coord.x,yyvsp[-3].coord.y,yyvsp[0].ftype); ;
    break;}
case 258:
#line 1066 "dialog.yacc"
{ DM_ClipRotate(yyvsp[0].coord.x,yyvsp[0].coord.y,yyvsp[-3].ftype); ;
    break;}
case 259:
#line 1068 "dialog.yacc"
{ DM_ClipFlipX(yyvsp[0].ftype); ;
    break;}
case 260:
#line 1070 "dialog.yacc"
{ DM_ClipFlipX(yyvsp[0].coord.x); ;
    break;}
case 261:
#line 1072 "dialog.yacc"
{ DM_ClipFlipY(yyvsp[0].ftype); ;
    break;}
case 262:
#line 1074 "dialog.yacc"
{ DM_ClipFlipY(yyvsp[0].coord.y); ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 303 "bison.simple"

  yyvsp -= yylen;
  yylsp -= yylen;
  yyssp -= yylen;

  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now", yyssp-yyss);
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }

  *++yyvsp = yyval;

  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      yyerror("parse error");
    }

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYERROR;

      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYERROR;
  yyvsp--;
  yylsp--;
  yystate = *--yyssp;

  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now", yyssp-yyss);
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

  if (yydebug)
    fprintf(stderr, "Shifting error token, ");

  *++yyvsp = yylval;
  *++yylsp = yylloc;

  yystate = yyn;
  goto yynewstate;
}
#line 1278 "dialog.yacc"


#include "lex.yy.c"

