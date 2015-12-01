all: bison flex build

flex:
	flex -o "lex.yy.cpp" simplejava.l

bison:
	bison -o "simplejava.tab.cpp" -d --report all --report-file "report.txt"  simplejava.y

build:
	g++ -std=c++11 lex.yy.cpp simplejava.tab.cpp main.cpp
	rm -f simplejava.tab.cpp simplejava.tab.hpp lex.yy.cpp ast.h.gch CVisitor.h.gch CPrintVisitor.h.gch
