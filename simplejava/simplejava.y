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
%token <str> INT_TYPE BOOLEAN_TYPE EXTENDS EQ PLUS IF ELSE WHILE  RETURN  PUBLIC CLASS STATIC  VOID MAIN STRING PRINT  THIS NEW LENGTH ARRAY LBRACE  RBRACE  LPAREN RPAREN LBRACK RBRACK LEQ AND MINUS MULT DIV SEMCOL COMMA BANG DOT 

%left PLUS MINUS 
%left MULT DIV
%left AND

%nonassoc BANG DOT
%nonassoc LBRACK RBRACK 
%nonassoc UMINUS UPLUS

%%


program
        : main_class declarations
        ;

main_class
        : CLASS IDENT LBRACE PUBLIC STATIC VOID MAIN LPAREN STRING LBRACK RBRACK IDENT RPAREN LBRACE statement RBRACE RBRACE
        ;
declarations
        : declarations class_declaration
        | 
        ;
class_declaration
        : CLASS IDENT extend_declaration LBRACE var_declarations method_declarations RBRACE
        ;
extend_declaration
        : extend_declaration EXTENDS IDENT
        | EXTENDS IDENT
        ;
        
var_declarations
        : var_declarations var_declaration
        | 
        ;
method_declarations
        : method_declarations method_declaration
        |
        ;
var_declaration
        : type IDENT SEMCOL
        ;
method_declaration
        : PUBLIC type IDENT LPAREN params RPAREN LBRACE method_body RETURN expression SEMCOL RBRACE
        ;
vars_dec
: vars_dec var_declaration
| var_declaration
;
stats
: stats statement
| statement
;
method_body
        : vars_dec
        | stats
        | vars_dec stats
        |
        ;
params
        : param
        | params COMMA param
        ;
param
        : type IDENT
        ;
type
        : ARRAY
        | BOOLEAN_TYPE
        | INT_TYPE
        | IDENT
        ;

statements
        : statements statement
        |
        ;
statement
        : LBRACE statements RBRACE
        | if_statement
        | while_statement
        | print_statement
        | assign_statement
        | invoke_exp_statement
        ;
        
if_statement
        : IF LPAREN expression RPAREN statement ELSE statement
        ;
while_statement
        : WHILE LPAREN expression RPAREN statement
        ;
print_statement
        : PRINT LPAREN expression RPAREN SEMCOL
        ;
assign_statement
        : IDENT EQ expression SEMCOL
        ;
invoke_exp_statement
        : IDENT LBRACK expression RBRACK EQ expression SEMCOL
        ;
binop
    : expression PLUS expression { }
    | expression MINUS expression { }
    | expression MULT expression { }
    | expression AND expression {  }
    | expression DIV expression
    | PLUS expression %prec UPLUS {}
    | MINUS expression %prec UMINUS
    ;


expression
        : invoke_expression
        | length_expression
        | INT
        | BOOLEAN
        | IDENT
        | THIS
        | NEW INT_TYPE LBRACK expression RBRACK
        | NEW IDENT LPAREN RPAREN
        | BANG expression
        | LPAREN expression RPAREN
        | binop
        ;
invoke_expression
        : expression LBRACK expression RBRACK
        ;
length_expression
        : expression DOT LENGTH
        ;
%%
