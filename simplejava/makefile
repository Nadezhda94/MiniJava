all: bison flex build

flex:
	flex simplejava.l

bison:
	bison -d --report all --report-file simplejava.out simplejava.y

build:
	g++ -std=c++11 lex.yy.c simplejava.tab.c
	rm simplejava.tab.c simplejava.tab.h lex.yy.c
