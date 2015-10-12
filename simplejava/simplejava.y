%{

#include <iostream>
#include <string>
#include <stdio.h>
#include "simplejava.tab.h"
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

%token <intValue> INT
%token <boolValue> BOOLEAN
%token <identName> IDENT
%token <str> INT_TYPE BOOLEAN_TYPE EQ PLUS IF ELSE WHILE  RETURN  PUBLIC CLASS STATIC  VOID MAIN STRING PRINT THIS NEW LENGTH ARRAY LBRACE  RBRACE  LPAREN RPAREN LBRACK RBRACK LEQ AND MINUS MULT SEMCOL COMMA BANG DOT EXTENDS END_OF_FILE

%%
stm: ident EQ exp SEMCOL
;

exp: INT
;

goal:	main_class class_declaration END_OF_FILE
;

main_class:	CLASS ident LBRACE PUBLIC STATIC VOID MAIN LPAREN STRING LBRACK RBRACK ident RPAREN LBRACE stm RBRACE RBRACE
;

class_declaration: CLASS ident EXTENDS ident LBRACK var_declaration method_declaration RBRACK
  | CLASS ident LBRACK var_declaration method_declaration RBRACK
;

method_declaration: PUBLIC type ident LPAREN type ident RPAREN LBRACK var_declaration stm RETURN exp SEMCOL RBRACK
;

var_declaration: type ident SEMCOL { std::cout << ";" <<std::endl; }
;

ident: IDENT { std::cout << yylval.identName << " ";}
;

type:	INT_TYPE LBRACK RBRACK
	|	BOOLEAN_TYPE { std::cout << "bool" << " ";}
	|	INT_TYPE { std::cout << "int" << " ";}
	|	ident
;
%%



int main(int argc, char** argv) {
    FILE* progrFile;
    //progrFile = fopen(argv[1], "r");
    //yyin = progrFile;
    yyparse();
    return 0;
}
