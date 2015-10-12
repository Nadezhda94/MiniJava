all: bison flex build

flex:
	win_flex -o "lex.yy.cpp" simplejava.l

bison:
	win_bison -o "simplejava.tab.cpp" -d simplejava.y 

build: 
	g++ -std=gnu++0x lex.yy.cpp simplejava.tab.cpp

