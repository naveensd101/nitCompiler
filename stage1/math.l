%{
	#include<stdio.h>
	#include<stdlib.h>
	#include "y.tab.h"
	#include "math.h"
	int number;
%}

%%
[0-9]+ { number=atoi(yytext); yylval.node = makeLeafNode(number);return NUM; }
\+|\*|\-|\/    { return yytext[0]; }
[ \t]  { ; }
[()]   { return yytext[0]; }
[\n]   { return END; }
.      { printf("lex error: %s\n", yytext); exit(1); }
%%

int yywrap() { return 1; }
