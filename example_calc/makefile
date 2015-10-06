main:	flex.l bison.y
	bison -d bison.y
	flex flex.l
	g++ -o bin.o bison.tab.c lex.yy.c -lfl
