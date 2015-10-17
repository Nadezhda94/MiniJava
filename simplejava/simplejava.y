%{

#include <iostream>
#include <string>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include "simplejava.tab.hpp"
#include "ast.h"
using std::cout;
using std::endl;
using std::to_string;
using std::strcpy;
extern FILE * yyin;
int yylex();
void yyerror(const char * s){
    std::cout << s << std::endl;
    std::cout << "line number: " << yylloc.first_line << std::endl;
    std:: cout << "position in line: " << yylloc.last_column << std::endl;
};

%}
%locations

%union {
    int intValue;
    bool boolValue;
    char str[4096];
};

%token <intValue> INT
%token <boolValue> BOOLEAN
%token <str> IDENT INT_TYPE BOOLEAN_TYPE EXTENDS EQ PLUS IF ELSE WHILE  RETURN  PUBLIC CLASS STATIC  VOID MAIN STRING PRINT  THIS NEW LENGTH ARRAY LBRACE  RBRACE  LPAREN RPAREN LBRACK RBRACK LEQ AND MINUS MULT DIV SEMCOL COMMA BANG DOT

%left LEQ EQ
%left DOT
%left PLUS MINUS
%left MULT DIV AND

%nonassoc BANG  IF ELSE
%nonassoc LBRACK RBRACK
%nonassoc UMINUS UPLUS

%type <str> expression statement extend_declaration param_arg var_declaration var_declarations class_declaration exp_arg
%type <str> expressions statements type param params method_body vars_dec stats method_declarations method_declaration

%%


program
        : main_class declarations {cout<<"End"<<endl;}
        ;

main_class
        : CLASS IDENT LBRACE PUBLIC STATIC VOID MAIN LPAREN STRING LBRACK RBRACK IDENT RPAREN LBRACE statement RBRACE RBRACE {
          cout<<"Main class "<< $2 << " with arg " << $12 << " do"<<endl<<"\t"<<  $15<<endl;
        }
        ;

declarations
        : declarations class_declaration {cout<<$2<<endl;}
        | {cout<<endl;}
        ;

class_declaration
        : CLASS IDENT extend_declaration LBRACE var_declarations method_declarations RBRACE {
          strcpy($$, "Class declaration");
          strcat($$, $3);
          strcat($$, ": ");
          strcat($$, $5);
          strcat($$, $6);
        }
        ;

extend_declaration
        : EXTENDS IDENT { strcpy($$, " extends "); strcat($$, $2); }
        |  { strcpy($$, ""); }
        ;

var_declarations
        : var_declarations var_declaration  {
            strcpy($$, $1);
            strcat($$, "\n\t\t");
            strcat($$, $2);
          }
        | { strcpy($$, "\n\tVar declarations: "); }
        ;

method_declarations
        : method_declarations method_declaration { strcpy($$, $1); strcat($$, $2); }
        | { strcpy($$, "\n\tMethod declarations: "); }
        ;

var_declaration
        : type IDENT SEMCOL { strcpy($$, $1); strcat($$, $2); strcat($$, ";");}
        ;

method_declaration
        : PUBLIC type IDENT LPAREN param_arg RPAREN LBRACE method_body RETURN expression SEMCOL RBRACE {
                strcpy($$, "\n\t\tMethod ");
                strcat($$, $3);
                strcat($$, " declaration{ ");
                strcat($$, "\n\t\t\tType: ");
                strcat($$, $2);
                strcat($$, "\n\t\t\tParams: ");
                strcat($$, $5);
                strcat($$, "\n\t\t\tMethod body[ ");
                strcat($$, $8);
                strcat($$, "\n\t\t\t]");
                strcat($$, "\n\t\t\tReturn: ");
                strcat($$, $10);
                strcat($$, "\n\t\t}");
                }
        ;

vars_dec
        : vars_dec var_declaration  { strcpy($$, $1); strcat($$, " "); strcat($$, $2);}
        | var_declaration { strcpy($$, $1); }
        ;

stats
        : stats statement { strcpy($$, $1); strcat($$, " ; "); strcat($$, $2);}
        | statement { strcpy($$, $1); }
        ;

method_body
        : vars_dec { strcpy($$, "\n\t\t\t\tVars declaration: ");  strcat($$, $1);}
        | stats { strcpy($$, "\n\t\t\t\tStatements: "); strcat($$, $1);}
        | vars_dec stats {  strcpy($$, "\n\t\t\t\tVars declaration: ");  strcat($$, $1); strcat($$, "\n\t\t\t\tStatements: "); strcat($$, $2);}
        | {  strcpy($$, " No "); }
        ;

param_arg
        : params { strcpy($$, $1); }
        | { strcpy($$, ""); }
        ;

params
        : param { strcpy($$, $1);}
        | params COMMA param { strcpy($$, $1); strcat($$, " , "); strcat($$, $2); }
        ;

param

        : type IDENT { strcpy($$, $1);  strcat($$, $2); }
        ;

type
        : ARRAY { strcpy($$, "int[] "); }
        | BOOLEAN_TYPE { strcpy($$, "bool "); }
        | INT_TYPE { strcpy($$, "int "); }
        | IDENT { strcpy($$, $1); }
        ;

statements
        : statements statement { strcpy($$, $1); strcat($$, " ; "); strcat($$, $2);}
        | { strcpy($$, "Statements: ");}
        ;

statement
        : LBRACE statements RBRACE { strcpy($$, "{"); strcat($$, $2); strcat($$, "}"); }
        | IF LPAREN expression RPAREN statement ELSE statement {
            strcpy($$, "If "); strcat($$, $3);
            strcat($$, " then {"); strcat($$, $5); strcat($$, "}");
            strcat($$, " else {"); strcat($$, $7); strcat($$, "}");
          }
        | WHILE LPAREN expression RPAREN statement {strcpy($$, "While "); strcat($$, $3); strcat($$, " do "); strcat($$, $5);}
        | PRINT LPAREN expression RPAREN SEMCOL {strcpy($$, "Print "); strcat($$, $3);}
        | IDENT EQ expression SEMCOL { strcpy($$, "Assign "); strcat($$, $3); strcat($$, " to "); strcat($$, $1);}
        | IDENT LBRACK expression RBRACK EQ expression SEMCOL { strcpy($$, "Invoke "); strcat($$, $3); strcat($$, " and "); strcat($$, $6);}
        ;

expression
        :  expression LBRACK expression RBRACK {
            strcpy($$, $1);
            strcat($$, "(");
            strcat($$, $3);
            strcat($$, ")");
          }
        | expression DOT LENGTH { strcpy($$, $1); strcat($$, ".length");}
        | INT { strcpy($$, to_string(yylval.intValue).c_str()); }
        | BOOLEAN { strcpy($$, to_string(yylval.boolValue).c_str()); }
        | IDENT { strcpy($$, yylval.str); }
        | THIS { strcpy($$, "this"); }
        | NEW INT_TYPE LBRACK expression RBRACK {
            strcpy($$, "new int [");
            strcat($$, $4);
            strcat($$, "]");
          }
        | NEW IDENT LPAREN RPAREN {
            strcpy($$, "new ");
            strcat($$, $2);
            strcat($$, "(");
            strcat($$, ")");
          }
        | BANG expression %prec BANG { strcpy($$, "!"); strcat($$, $2); }
        | LPAREN expression RPAREN {
            strcpy($$, "(");
            strcat($$, $2);
            strcat($$, ")");
          }
        | expression PLUS expression { strcpy($$, $1); strcat($$, "+"); strcat($$, $3); }
        | expression MINUS expression { strcpy($$, $1); strcat($$, "-"); strcat($$, $3); }
        | expression MULT expression { strcpy($$, $1); strcat($$, "*"); strcat($$, $3); }
        | expression AND expression { strcpy($$, $1); strcat($$, "&&"); strcat($$, $3); }
        | expression DIV expression { strcpy($$, $1); strcat($$, "/"); strcat($$, $3); }
        | expression LEQ expression { strcpy($$, $1); strcat($$, "<"); strcat($$, $3); }
        | PLUS expression %prec UPLUS { strcpy($$, "+"); strcat($$, $2); }
        | MINUS expression %prec UMINUS { strcpy($$, "-"); strcat($$, $2); }
        | expression DOT IDENT LPAREN exp_arg RPAREN {
            strcpy($$, $1);
            strcat($$, ".");
            strcat($$, $3);
            strcat($$, "(");
            strcat($$, $5);
            strcat($$, ")");
          }
        ;

exp_arg
        : expressions { strcpy($$, $1); }
        | { strcpy($$, ""); }
        ;
expressions
        : expression  { strcpy($$, $1); }
        | expressions COMMA expression {
            strcpy($$, $1);
            strcat($$, " , ");
            strcat($$, $3);
          }
        ;

%%
