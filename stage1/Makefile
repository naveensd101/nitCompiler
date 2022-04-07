all:
	bison --yacc -d math.y -Wcounterexamples
	lex math.l
	gcc y.tab.c lex.yy.c -o nitc

clean:
	rm lex* y.tab* nitc


