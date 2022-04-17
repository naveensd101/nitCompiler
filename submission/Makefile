all:
	gcc y.tab.c lex.nitc.c -o nitc
	./nitc <example.c >example.xsm
	gcc lex.linker.c -o linker
	./linker <example.xsm >B190707CS.xsm

clean:
	rm nitc *.xsm linker

build:
	bison --yacc -d nitc.y
	lex nitc.l
	mv lex.yy.c lex.nitc.c
	lex linker.l
	mv lex.yy.c lex.linker.c

back:
	bison --yacc -d nitc.y
	lex nitc.l
	gcc y.tab.c lex.yy.c -o nitc
	rm lex* y.tab*
	./nitc <example.c >example.xsm
	lex linker.l
	gcc lex.yy.c -o linker
	./linker <example.xsm >B190707CS.xsm
