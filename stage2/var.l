%{
	#include<stdio.h>
	#include<stdlib.h>
	#include "y.tab.h"
	#include "var.h"
	int number;
	char name[1];
%}

%%
"write" { return Write; }
"read"  { return Read; }
[0-9]+ {
	number = atoi(yytext);
	yylval.node = makeTree(number, NULL, number_node, NULL, NULL);
	return NUM;
}
[a-z] {
	name[0] = yytext[0];
	yylval.node = makeTree(-1, name, variable_node, NULL, NULL);
	return VAR;
}
\+|\*|\-|\/|\= { return yytext[0]; }
[ \t\n]  { ; }
\(    { return yytext[0]; }
\)    { return yytext[0]; }
[{}]    { return yytext[0]; }
[;]     { return yytext[0]; }
.       { printf("lex error: %s\n", yytext); exit(1); }
%%

int yywrap() { return 1; }
