all:
	gcc y.tab.c lex.nitc.c -o nitc
	./nitc <$(input) >example.xsm
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

purge:
	rm lex.* y.tab.* nitc *.xsm linker
