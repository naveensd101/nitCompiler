all:
	bison --yacc -d nitc.y
	lex -i nitc.l
	mv lex.yy.c lex.nitc.c
	lex linker.l
	mv lex.yy.c lex.linker.c
	gcc y.tab.c lex.nitc.c -o nitc
	./nitc <$(input) >example.xsm
	gcc lex.linker.c -o linker
	./linker <example.xsm >B190707CS.xsm

clean:
	rm nitc *.xsm linker
	rm lex.* rm y.tab.*

bak:
	bison --yacc -d nitc.y
	lex -i nitc.l
	mv lex.yy.c lex.nitc.c
	lex linker.l
	mv lex.yy.c lex.linker.c
	gcc y.tab.c lex.nitc.c -o nitc
	./nitc <example.c >example.xsm
	gcc lex.linker.c -o linker
	./linker <example.xsm >B190707CS.xsm
