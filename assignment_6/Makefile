CXX = g++
CXXFLAGS = -Wno-write-strings
LEXER = flex

main: parser.cpp lex.yy.c
	$(CXX) $(CXXFLAGS) -o $@ $^

lex.yy.c: parser.l
	$(LEXER) $^

clean:
	rm -f lex.yy.c

test: main test1.input test2.input
	./main test1.input
	./main test2.input