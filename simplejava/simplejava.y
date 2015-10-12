%{
    
#include <iostream>
#include <string>
#include <stdio.h>
#include "simplejava.tab.hpp"
extern FILE * yyin;
int yylex();
void yyerror(const char *){};

%}

%union { 
    int intValue; 
    bool boolValue;
    char identName[256];
    char str[256];
};

%type <str> superterm
%type <str> binop
%type <str> identifier
%token <intValue> INT
%token <boolValue> BOOLEAN
%token <identName> IDENT 
%token <str> INT_TYPE BOOLEAN_TYPE EQ PLUS IF ELSE WHILE  RETURN  PUBLIC CLASS STATIC  VOID MAIN STRING PRINT  THIS NEW LENGTH ARRAY LBRACE  RBRACE  LPAREN RPAREN LBRACK RBRACK LEQ AND MINUS MULT SEMCOL COMMA DOT BANG
%left PLUS MINUS 
%left MULT
%left AND
%nonassoc BANG DOT
%nonassoc LBRACK RBRACK

%%
superterm
    :	INT { $$ = yylval.intValue; }
    |	identifier { $$ = $1; }
	|	BOOLEAN { $$ = yylval.boolValue; }
	|	THIS { $$ = "this "; }
	|	BANG superterm { $$ = "!" + $2; }
	|	NEW identifier LPAREN RPAREN { $$ = "new " + $2 + "() "; }
	|	superterm DOT LENGTH { $$ = $1 + ".length "; }
	|	superterm LBRACK superterm RBRACK { $$ = $1 + "[" + $3 + "]"; }
	|	NEW INT_TYPE LBRACK superterm RBRACK { $$ = "new int [" + $4 + "]"; }
	|	LPAREN superterm RPAREN { $$ = "(" + $2 + ")"; }
	|	binop { $$ = $1; }
;

binop
	: superterm PLUS superterm { $$ = $1 + " plus " + $3; }
	| superterm MINUS superterm { $$ = $1 + " minus " + $3; }
	| superterm MULT superterm { $$ = $1 + " mult " + $3; }
	| superterm AND superterm { $$ = $1 + " and " + $3; }
;

identifier
	: IDENT { $$ = yylval.identName; }
;
%%

    

int main(int argc, char** argv) {
    FILE* progrFile;
    //progrFile = fopen(argv[1], "r");
    //yyin = progrFile;
    yyparse();
    return 0;
}