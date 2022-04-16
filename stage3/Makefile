all:
	bison --yacc -d if.y -Wcounterexamples
	lex if.l
	gcc y.tab.c lex.yy.c -o nitc

clean:
	rm lex* y.tab* nitc *.xsm


