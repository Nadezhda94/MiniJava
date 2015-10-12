all: bison flex build

flex:
	flex -o "lex.yy.cpp" simplejava.l

bison:
	bison -o "simplejava.tab.cpp" -d --report all --report-file "f.txt"  simplejava.y 

build: 
	g++ -std=c++11 lex.yy.cpp simplejava.tab.cpp main.cpp -ll

