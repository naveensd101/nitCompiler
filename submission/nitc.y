%{
	#include<stdlib.h>
	#include<stdio.h>
	#include<string.h>
	#include "nitc.h"
	#include "nitc.c"
	int yylex();
	int yyerror(char const *);
	tnode *ast;
	int _t;
%}

%union {
	struct tnode *node;
}
%type <node> Program StmtList Stmt Input Output Assign E IfStmt IfElseStmt RetStmt
%token <node> NUM VAR Read Write If Else Return Fun
%left '<' '>' 'g' 'l' 'e' 'n'
%left '+' '-'
%left '*' '/'
%%

Program: Read '(' VAR ')' ';' VAR '=' Fun '(' E ')' '{' StmtList '}' Write '(' VAR ')' ';'
	   {
		   //ast = makeTree(-1, NULL, connector_node, $2, NULL);
		   magic();
		   ast = initTree($13);
		   _t = codeGen(ast, NULL);
		   //sexyExit();
		   exit(0);
	   }
	   ;

StmtList: StmtList Stmt  { $$ = makeTree(-1, NULL, connector_node, $1, $2); }
		| Stmt           { $$ = $1; }
		;

Stmt: Input      { $$ = $1; }
	| Output     { $$ = $1; }
	| Assign     { $$ = $1; }
	| IfElseStmt { $$ = $1; }
	| IfStmt     { $$ = $1; }
	| RetStmt    { $$ = $1; }
	;

Input: Read '(' VAR ')' ';' { $$ = makeTree(-1, NULL, read_node, $3, NULL); }
	 ;

Output: Write '(' E ')' ';' { $$ = makeTree(-1, NULL, write_node, $3, NULL); }
	  ;

Assign: VAR '=' E ';' { $$ = makeTree(-1, NULL, assign_node, $1, $3); }
	  ;

IfElseStmt: If '(' E ')' '{' StmtList '}' Else '{' StmtList '}' { $$ = makeIfElseTree($3, $6, $10); }
	  ;

IfStmt: If '(' E ')' '{' StmtList '}' { $$ = makeTree(-1, NULL, if_node, $3, $6); }
	  ;

RetStmt : Return E ';' { $$ = makeTree(-1, NULL, return_node, $2, NULL); }
	  ;

E: E '+' E       { $$ = makeTree(-1, "+" , math_node, $1, $3); }
 | E '-' E       { $$ = makeTree(-1, "-" , math_node, $1, $3); }
 | E '*' E       { $$ = makeTree(-1, "*" , math_node, $1, $3); }
 | E '/' E       { $$ = makeTree(-1, "/" , math_node, $1, $3); }
 | E '<' E       { $$ = makeTree(-1, "<" , bool_node, $1, $3); }
 | E '>' E       { $$ = makeTree(-1, ">" , bool_node, $1, $3); }
 | E 'l' E       { $$ = makeTree(-1, "l" , bool_node, $1, $3); }
 | E 'g' E       { $$ = makeTree(-1, "g" , bool_node, $1, $3); }
 | E 'e' E       { $$ = makeTree(-1, "e" , bool_node, $1, $3); }
 | E 'n' E       { $$ = makeTree(-1, "n" , bool_node, $1, $3); }
 | '(' E ')'     { $$ = $2; }
 | VAR           { $$ = $1; }
 | NUM           { $$ = $1; }
 | Fun '(' E ')' { $$ = makeTree(-1, NULL, fun_call_node, $3, NULL); }
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

