%{
#include <iostream>
#include "simplejava.tab.hpp"
#include "ast.h"
#include "CSymbol.h"
using std::cout;
using std::endl;
extern FILE * yyin;
CProgramRuleNode* root;
Symbol::CStorage symbolsStorage;

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
    struct CProgramNode* programNode;
    struct CMainClassNode* mainClassNode;
    struct CDeclarationsNode* declarationsNode;
    struct CClassDeclarationNode* classDeclarationNode;
    struct CExtendDeclarationNode* extendDeclarationNode;
    struct CVarDeclarationsNode* varDeclarationsNode;
    struct CMethodDeclarationsNode* methodDeclarationsNode;
    struct CVarDeclarationNode* varDeclarationNode;
    struct CMethodDeclarationNode* methodDeclarationNode;
    struct CVarsDecNode* varsDecNode;
    struct CStatsNode* statsNode;
    struct CMethodBodyNode* methodBodyNode;
    struct CParamArgNode* paramArgNode;
    struct CParamsNode* paramsNode;
    struct CParamNode* paramNode;
    struct CTypeNode* typeNode;
    struct CStatementsNode* statementsNode;
    struct CStatementNode* statementNode;
    struct CExpressionNode* expressionNode;
    struct CExpArgNode* expArgNode;
    struct CExpressionsNode* expressionsNode;
};

%token <intValue> INT
%token <boolValue> BOOLEAN
%token <str> IDENT INT_TYPE BOOLEAN_TYPE EXTENDS EQ PLUS IF ELSE WHILE  RETURN  PUBLIC CLASS STATIC  VOID MAIN STRING PRINT  THIS NEW LENGTH ARRAY LBRACE  RBRACE  LPAREN RPAREN LBRACK RBRACK LEQ AND MINUS MULT DIV SEMCOL COMMA BANG DOT

%right EQ
%left AND
%left LEQ

%left PLUS MINUS
%left MULT DIV
%left BANG UMINUS UPLUS
%left DOT

%nonassoc  IF ELSE
%nonassoc LBRACK RBRACK

%type <programNode> program
%type <mainClassNode> main_class
%type <declarationsNode> declarations
%type <classDeclarationNode> class_declaration
%type <extendDeclarationNode> extend_declaration
%type <varDeclarationsNode> var_declarations
%type <methodDeclarationsNode> method_declarations
%type <varDeclarationNode> var_declaration
%type <methodDeclarationNode> method_declaration
%type <varsDecNode> vars_dec
%type <statsNode> stats
%type <methodBodyNode> method_body
%type <paramArgNode> param_arg
%type <paramsNode> params
%type <paramNode> param
%type <typeNode> type
%type <statementsNode> statements
%type <statementNode> statement
%type <expressionNode> expression
%type <expArgNode> exp_arg
%type <expressionsNode> expressions

%%


program

        : main_class declarations {
            root = new CProgramRuleNode($1, $2);
            $$ = root;
          }
        ;

main_class
        : CLASS IDENT LBRACE PUBLIC STATIC VOID MAIN LPAREN STRING LBRACK RBRACK IDENT RPAREN LBRACE statement RBRACE RBRACE {
            $$ = new CMainClassDeclarationRuleNode(symbolsStorage.get(std::string($2)), symbolsStorage.get(std::string($12)), $15);
          }
        ;

declarations
        : declarations class_declaration {
            $$ = new CDeclarationsListNode($1, $2);
          }
        | {$$ = 0;}
        ;

class_declaration
        : CLASS IDENT extend_declaration LBRACE var_declarations method_declarations RBRACE {
            $$ = new CClassDeclarationRuleNode(symbolsStorage.get(std::string($2)), $3, $5, $6);
          }
        ;

extend_declaration
        : EXTENDS IDENT {$$ = new CExtendDeclarationRuleNode(symbolsStorage.get(std::string($2)));}
        | {$$ = 0;}
        ;

var_declarations
        : var_declarations var_declaration  {
            $$ = new CVarDeclarationsListNode($1, $2);
          }
        | {$$ = 0;}
        ;

method_declarations
        : method_declarations method_declaration {
            $$ = new CMethodDeclarationsListNode($1, $2);
          }
        | {$$ = 0;}
        ;

var_declaration
        : type IDENT SEMCOL {$$ = new CVarDeclarationRuleNode($1, symbolsStorage.get(std::string($2)));}
        ;

method_declaration
        : PUBLIC type IDENT LPAREN param_arg RPAREN LBRACE method_body RETURN expression SEMCOL RBRACE {
            $$ = new CMethodDeclarationRuleNode($2, symbolsStorage.get(std::string($3)), $5, $8, $10);
          }
        ;

vars_dec
        : vars_dec var_declaration {
            $$ = new CVarsDecListNode($1, $2);
          }
        | var_declaration {$$ = new CVarsDecFirstNode($1);}
        ;

stats
        : stats statement {
            $$ = new CStatsListNode($1, $2);
          }
        | statement {$$ = new CStatsFirstNode($1);}
        ;

method_body
        : vars_dec {$$ = new CMethodBodyVarsNode($1);}
        | stats {$$ = new CMethodBodyStatsNode($1);}
        | vars_dec stats {
            $$ = new CMethodBodyAllNode($1, $2);
          }
        | {$$ = 0;}
        ;

param_arg
        : params {$$ = new CParamArgListNode($1);}
        | {$$ = 0;}
        ;

params
        : param {$$ = new CParamsOneNode($1);}
        | params COMMA param {
            $$ = new CParamsTwoNode($1, $3);
          }
        ;

param
        : type IDENT {$$ = new CParamRuleNode($1, symbolsStorage.get(std::string($2)));}
        ;

type
        : ARRAY {$$ = new CTypeRuleNode(symbolsStorage.get("int[]"));}
        | BOOLEAN_TYPE {$$ = new CTypeRuleNode(symbolsStorage.get("bool"));}
        | INT_TYPE {$$ = new CTypeRuleNode(symbolsStorage.get("int"));}
        | IDENT {$$ = new CTypeRuleNode(symbolsStorage.get(std::string($1)));}
        ;

statements
        : statements statement {
            $$ = new CNumerousStatementsNode($1, $2);
          }
        | {$$ = 0;}
        ;

statement
        : LBRACE statements RBRACE {$$ = new CBracedStatementNode($2);}
        | IF LPAREN expression RPAREN statement ELSE statement {
            $$ = new CIfStatementNode($3, $5, $7);
          }
        | WHILE LPAREN expression RPAREN statement {
            $$ = new CWhileStatementNode($3, $5);
          }
        | PRINT LPAREN expression RPAREN SEMCOL {$$ = new CPrintStatementNode($3);}
        | IDENT EQ expression SEMCOL {$$ = new CAssignStatementNode($3, symbolsStorage.get(std::string($1)));}
        | IDENT LBRACK expression RBRACK EQ expression SEMCOL {
            $$ = new CInvokeExpressionStatementNode($3, $6, symbolsStorage.get(std::string($1)));
          }
        ;

expression
        : expression LBRACK expression RBRACK {
            $$ = new CInvokeExpressionNode($1, $3);
          }
        | expression DOT LENGTH {$$ = new CLengthExpressionNode($1);}
        | INT {$$ = new CIntExpressionNode(yylval.intValue);}
        | BOOLEAN {$$ = new CBooleanExpressionNode(yylval.boolValue);}
        | IDENT {$$ = new CIdentExpressionNode(symbolsStorage.get(std::string(yylval.str)));}
        | THIS {$$ = new CThisExpressionNode(symbolsStorage.get("this"));}
        | NEW INT_TYPE LBRACK expression RBRACK {
            $$ = new CNewArrayExpressionNode($4);
          }
        | NEW IDENT LPAREN RPAREN {
            $$ = new CNewObjectExpressionNode(symbolsStorage.get(std::string($2)));
          }
        | BANG expression %prec BANG {
            $$ = new CNotExpressionNode($2);
          }
        | LPAREN expression RPAREN {
            $$ = new CParenExpressionNode($2);
          }
        | expression PLUS expression {
            $$ = new CArithmeticExpressionNode($1, $3, PLUS_OP);
          }
        | expression MINUS expression {
            $$ = new CArithmeticExpressionNode($1, $3, MINUS_OP);
          }
        | expression MULT expression {
            $$ = new CArithmeticExpressionNode($1, $3, MULT_OP);
          }
        | expression AND expression {
            $$ = new CArithmeticExpressionNode($1, $3, AND_OP);
          }
        | expression DIV expression {
            $$ = new CArithmeticExpressionNode($1, $3, DIV_OP);
          }
        | expression LEQ expression {
            $$ = new CCompareExpressionNode($1, $3);
          }
        | PLUS expression %prec UPLUS {$$ = new CUnaryExpressionNode($2, UPLUS_OP);}
        | MINUS expression %prec UMINUS {$$ = new CUnaryExpressionNode($2, UMINUS_OP);}
        | expression DOT IDENT LPAREN exp_arg RPAREN {
            $$ = new CInvokeMethodExpressionNode($1, symbolsStorage.get(std::string($3)), $5);
          }
        ;

exp_arg
        : expressions {$$ = new CFewArgsExpressionNode($1);}
        | {$$ = 0;}
        ;
expressions
        : expression  {$$ = new CLastListExpressionNode($1);}
        | expressions COMMA expression {
            $$ = new CListExpressionNode($1, $3);
          }
        ;

%%
