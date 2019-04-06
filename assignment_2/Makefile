LEXER = flex

CC = cc
CCFLAGS = -Wimplicit-function-declaration

YACC = yacc
YACCFLAGS = -d

calc: y.tab.c lex.yy.c
	$(CC) $(CCFLAGS) -o $@ $^

y.tab.c y.tab.h: calc.y
	$(YACC) $(YACCFLAGS) $^

lex.yy.c: calc.l y.tab.h
	$(LEXER) calc.l

clean:
	rm -f lex.yy.c y.tab.c y.tab.h