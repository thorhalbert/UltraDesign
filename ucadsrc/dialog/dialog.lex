%{

#undef YY_INPUT

#define YY_INPUT(buf,result,max_size)	result = getybuf(buf,max_size)

extern int deblen,debinc;

#define DEBECHO deblen+=debinc; debinc=strlen(yytext)

%}

int	{ return(TOKEN_INT); }

W	[ \t]*
S	"/"
X	[EeDd]{W}[-+]?{W}[0-9]+

%%

"="{W}?					{ DEBECHO; return(EQUALS); }

","{W}?					{ DEBECHO; return(COMMA); }

"$"[\$A-Za-z0-9_]+{W}?			{ yylval.ctype = yytext; 
					  DEBECHO; return(VARIABLE); }

[-+]?{W}?[0-9]+{W}?			{ yylval.ctype = yytext;
					  DEBECHO; return(SIMPLE_NUMBER); }

[-+]?{W}?[0-9]+"."[0-9]*({X})?{W}? |
[-+]?{W}?[0-9]*"."[0-9]+({X})?{W}? |
[-+]?{W}?[0-9]+{X}{W}?			{ yylval.ctype = yytext;
					  DEBECHO; return(FLOAT_NUMBER); }

 + .1 e - 12
1.
1.0


[0-9A-Fa-f]+{W}?			{ yylval.ctype = yytext;
					  DEBECHO; return(HEX_NUMBER); }

\".*\"{W}?				{ yylval.ctype = yytext;
					  DEBECHO; return(STRING); }

[^ \t\"\/=\,\@\$]+{W}?			{ yylval.ctype = yytext;
					  DEBECHO; return(TEXT_STRING);	}
					
\n				|
.					{ DEBECHO; return(ERROR_TEXT); }

										
%%

void resetparse()
{

	yy_init = 1;
	
	yy_saw_eof = 0;
	
}
