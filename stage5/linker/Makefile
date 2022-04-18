all:
	lex linker.l
	gcc -o linker lex.yy.c -Wshadow -Wall -O2 -Wno-unused-result
clean:
	rm lex.* linker
