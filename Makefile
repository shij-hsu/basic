lex:basic.l basic.y func.h func.c
	bison -d basic.y --report=solved
	flex -o basic.lex.c basic.l
	cc -o $@ *.c -lfl -lm
clean:
	rm -rf basic.lex.c basic.tab.h basic.tab.c lex
