LEXER = flex

CC = cc
CCFLAGS = -Wimplicit-function-declaration

YACC = yacc
YACCFLAGS = -d

main: y.tab.c lex.yy.c
	$(CC) $(CCFLAGS) -o $@ $^

y.tab.c y.tab.h: parser.y
	$(YACC) $(YACCFLAGS) $^

lex.yy.c: parser.l y.tab.h
	$(LEXER) parser.l

clean:
	rm -f lex.yy.c y.tab.c y.tab.h

test: main test1.input test2.input
	./main test1.input
	./main test2.input