all: bison flex build

flex:
	flex -o "lex.yy.cpp" simplejava.l
 
bison:
	bison -o "simplejava.tab.cpp" -d --report all --report-file "report.txt"  simplejava.y 

build:
	g++ -std=c++11 lex.yy.cpp simplejava.tab.cpp main.cpp
	rm simplejava.tab.cpp simplejava.tab.hpp lex.yy.cpp
