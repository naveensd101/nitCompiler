%{
	#include<stdlib.h>
	#include<stdio.h>
	#include "math.h"
	#include "math.c"
	int yylex();
	int yyerror(char const *);
%}

%union {
	struct tnode *node;
}
%type <node> S E
%token <node> NUM END
%left '+' '-'
%left '*' '/'
%%
S: E END {
 $$ = $1; magic();
 putBrk();
 codeGen($1);
 putBrk();
 return 0;
 }
 ;
E: E '+' E   { $$ = makeOperatorNode('+', $1, $3); }
 | E '-' E   { $$ = makeOperatorNode('-', $1, $3); }
 | E '*' E   { $$ = makeOperatorNode('*', $1, $3); }
 | E '/' E   { $$ = makeOperatorNode('/', $1, $3); }
 | '(' E ')' { $$ = $2; }
 | NUM       { $$ = $1; }
 ;
%%

int yyerror(char const *s) {
	fprintf(stderr, "Error: %s\n", s);
	exit(1);
}
int main() {
	yyparse();
	return 0;
}

