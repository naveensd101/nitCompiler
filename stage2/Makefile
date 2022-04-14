all:
	bison --yacc -d var.y -Wcounterexamples
	lex var.l
	gcc y.tab.c lex.yy.c -o nitc

clean:
	rm lex* y.tab* nitc


