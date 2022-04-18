%{
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
	#include "y.tab.h"
	#include "nitc.h"
	int number;
	char *name;
%}

startcomment \/\*\*
endcomment \*\*\/

%%
"write"      { return Write; }
"read"       { return Read; }
"If"         { return If; }
"else"       { return Else; }
"FUN"        { return Fun; }
"return"     { return Return; }

[0-9]+ {
	number = atoi(yytext);
	yylval.node = makeTree(number, NULL, number_node, NULL, NULL);
	return NUM;
}
[a-zA-Z][a-zA-Z0-9]* {
	name = strdup(yytext);
	yylval.node = makeTree(-1, name, variable_node, NULL, NULL);
	return VAR;
}

\+|\*|\-|\/|\= { return yytext[0]; }
[ \t\r\n]      { ; }
{startcomment}.*{endcomment} { ; }
\(             { return yytext[0]; }
\)             { return yytext[0]; }
[{}]           { return yytext[0]; }
[;]            { return yytext[0]; }
">"            { return yytext[0]; }
"<"            { return yytext[0]; }
"<="           { return 'l'; }
">="           { return 'g'; }
"=="           { return 'e'; }
"!="           { return 'n'; }
.              { printf("lex error: %s\n", yytext); exit(1); }
%%

int yywrap() { return 1; }
