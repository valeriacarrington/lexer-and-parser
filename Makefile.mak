# Makefile

all: parser

parser: lexer.o parser.o main.o
	g++ -o parser lexer.o parser.o main.o -lfl -std=c++11

lexer.o: lexer.l parser.tab.h
	flex lexer.l
	g++ -c lex.yy.c -o lexer.o

parser.o: parser.y
	bison -d parser.y
	g++ -c parser.tab.c -o parser.o

main.o: main.cpp ast.h
	g++ -c main.cpp -o main.o

clean:
	rm -f parser lex.yy.c parser.tab.c parser.tab.h *.o
