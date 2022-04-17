all:
	bison --yacc -d nitc.y -Wcounterexamples
	lex nitc.l
	gcc y.tab.c lex.yy.c -o nitc

clean:
	rm lex* y.tab* nitc *.xsm


