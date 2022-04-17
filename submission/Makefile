all:
	bison --yacc -d nitc.y
	lex nitc.l
	gcc y.tab.c lex.yy.c -o nitc
	rm lex* y.tab*
	./nitc <example.nitc >example.xsm
	lex linker.l
	gcc lex.yy.c -o linker
	./linker <example.xsm >B190707CS.xsm

clean:
	rm lex* nitc *.xsm linker


