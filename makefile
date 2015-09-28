all: flex build

flex:
	flex -o "lex.yy.cpp" simplejava.l
build: 
	g++ -std=c++11 lex.yy.cpp

