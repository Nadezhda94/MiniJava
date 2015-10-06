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

%token <intValue> INT
%token <boolValue> BOOLEAN
%token <identName> IDENT 
%token <str> INT_TYPE BOOLEAN_TYPE EQ PLUS IF ELSE WHILE  RETURN  PUBLIC CLASS STATIC  VOID MAIN STRING PRINT  THIS NEW LENGTH ARRAY LBRACE  RBRACE  LPAREN RPAREN LBRACK RBRACK LEQ AND MINUS MULT SEMCOL COMMA BANG DOT 

%%
superterm
    : superterm INT {
            std::cout << $2;
        }
    | IDENT { std::cout << yylval.identName;}
    |  EQ { std::cout << $1;}
    |  PLUS  { std::cout << $1;}
;
%%

    

int main(int argc, char** argv) {
    FILE* progrFile;
    //progrFile = fopen(argv[1], "r");
    //yyin = progrFile;
    yyparse();
    return 0;
}