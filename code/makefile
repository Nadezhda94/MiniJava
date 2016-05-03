all: bison flex build

flex:
	flex -o "lex.yy.cpp" simplejava.l

bison:
	bison -o "simplejava.tab.cpp" -d --report all --report-file "report.txt"  simplejava.y

build:
	mkdir -p Logs
	g++ -std=c++11 lex.yy.cpp simplejava.tab.cpp Structs/*.cpp ASTVisitors/*.cpp IRVisitors/*cpp main.cpp
	rm -f simplejava.tab.cpp simplejava.tab.hpp lex.yy.cpp ast.h.gch CVisitor.h.gch CPrintVisitor.h.gch
