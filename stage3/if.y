%{
	#include<stdlib.h>
	#include<stdio.h>
	#include "if.h"
	#include "if.c"
	int yylex();
	int yyerror(char const *);
	tnode *ast;
	int _t;
%}

%union {
	struct tnode *node;
}
%type <node> Program StmtList Stmt Input Output Assign E IfStmt IfElseStmt
%token <node> NUM VAR Read Write If Else
%left '<' '>' 'g' 'l' 'e' 'n'
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

Stmt: Input      { $$ = $1; }
	| Output     { $$ = $1; }
	| Assign     { $$ = $1; }
	| IfStmt     { $$ = $1; }
	| IfElseStmt { $$ = $1; }
	;

Input: Read '(' VAR ')' ';' { $$ = makeTree(-1, NULL, read_node, $3, NULL); }
	 ;

Output: Write '(' E ')' ';' { $$ = makeTree(-1, NULL, write_node, $3, NULL); }
	  ;

Assign: VAR '=' E ';' { $$ = makeTree(-1, NULL, assign_node, $1, $3); }
	  ;

IfStmt: If '(' E ')' '{' StmtList '}' { $$ = makeTree(-1, NULL, if_node, $3, $6); }
	  ;

IfElseStmt: If '(' E ')' '{' StmtList '}' Else '{' StmtList '}' { $$ = makeIfElseTree($3, $6, $10); }
	  ;

E: E '+' E   { $$ = makeTree(-1, "+" , math_node, $1, $3); }
 | E '-' E   { $$ = makeTree(-1, "-" , math_node, $1, $3); }
 | E '*' E   { $$ = makeTree(-1, "*" , math_node, $1, $3); }
 | E '/' E   { $$ = makeTree(-1, "/" , math_node, $1, $3); }
 | E '<' E   { $$ = makeTree(-1, "<" , bool_node, $1, $3); }
 | E '>' E   { $$ = makeTree(-1, ">" , bool_node, $1, $3); }
 | E 'l' E   { $$ = makeTree(-1, "l" , bool_node, $1, $3); }
 | E 'g' E   { $$ = makeTree(-1, "g" , bool_node, $1, $3); }
 | E 'e' E   { $$ = makeTree(-1, "e" , bool_node, $1, $3); }
 | E 'n' E   { $$ = makeTree(-1, "n" , bool_node, $1, $3); }
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

