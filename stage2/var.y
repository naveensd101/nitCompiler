%{
	#include<stdlib.h>
	#include<stdio.h>
	#include "var.h"
	#include "var.c"
	int yylex();
	int yyerror(char const *);
	tnode *ast;
	int _t;
%}

%union {
	struct tnode *node;
}
%type <node> Program StmtList Stmt Input Output Assign E
%token <node> NUM VAR Read Write
%left '+' '-'
%left '*' '/'
%%

Program: '{' StmtList '}'
	   {
	   ast = makeTree(-1, NULL, connector_node, $2, NULL);
	   magic();
	   _t = codeGen(ast);
	   sexyExit();
	   putBrk();
	   exit(0);
	   }
	   | '{' '}'
	   {
	   exit(0);
	   }
	   ;

StmtList: StmtList Stmt  { $$ = makeTree(-1, NULL, connector_node, $1, $2); }
		| Stmt           { $$ = $1; }
		;

Stmt: Input  { $$ = $1; }
	| Output { $$ = $1; }
	| Assign { $$ = $1; }
	;

Input: Read '(' VAR ')' ';' { $$ = makeTree(-1, NULL, read_node, $3, NULL); }
	 ;

Output: Write '(' E ')' ';' { $$ = makeTree(-1, NULL, write_node, $3, NULL); }
	  ;

Assign: VAR '=' E ';' { $$ = makeTree(-1, NULL, assign_node, $1, $3); }
	  ;

E: E '+' E   { $$ = makeTree(-1, "+" , math_node, $1, $3); }
 | E '-' E   { $$ = makeTree(-1, "-" , math_node, $1, $3); }
 | E '*' E   { $$ = makeTree(-1, "*" , math_node, $1, $3); }
 | E '/' E   { $$ = makeTree(-1, "/" , math_node, $1, $3); }
 | '(' E ')' { $$ = $2; }
 | VAR       { $$ = $1; }
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

