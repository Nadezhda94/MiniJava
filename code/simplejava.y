%{
#include "common.h"
#include "simplejava.tab.hpp"
#include "Structs/Ast.h"
#include "Structs/Symbol.h"

#include <exception>
extern FILE * yyin;
CProgramRuleNode* root;


int yylex();
void yyerror(const char * s){
    std::cerr << s << std::endl;
    std::cerr << "line number: " << yylloc.first_line << std::endl;
    std:: cerr << "position in line: " << yylloc.last_column << std::endl;
    throw new std::invalid_argument("syntax error");
};

void setLocation(YYLTYPE& curLoc, YYLTYPE& firstLoc, YYLTYPE& lastLoc, CNode* node) {
    curLoc.first_column = firstLoc.first_column;
    curLoc.first_line = firstLoc.first_line;
    curLoc.last_column = lastLoc.last_column;
    curLoc.last_line = lastLoc.last_line;
    node->setLocation(curLoc.first_column, curLoc.first_line, curLoc.last_column, curLoc.last_line);
}

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
            setLocation(@$, @1, @2, $$);
        }
        ;

main_class
        : CLASS IDENT LBRACE PUBLIC STATIC VOID MAIN LPAREN STRING LBRACK RBRACK IDENT RPAREN LBRACE statement RBRACE RBRACE {
            $$ = new CMainClassDeclarationRuleNode($2, $12, $15);
            setLocation(@$, @1, @16, $$);
        }
        ;

declarations
        : declarations class_declaration {
            $$ = new CDeclarationsListNode($1, $2);
            setLocation(@$, @1, @2, $$);
        }
        | {$$ = 0;}
        ;

class_declaration
        : CLASS IDENT extend_declaration LBRACE var_declarations method_declarations RBRACE {
            $$ = new CClassDeclarationRuleNode($2, $3, $5, $6);
            setLocation(@$, @1, @7, $$);
        }
        ;

extend_declaration
        : EXTENDS IDENT {
            $$ = new CExtendDeclarationRuleNode($2);
            setLocation(@$, @1, @2, $$);
        }
        | {$$ = 0;}
        ;

var_declarations
        : var_declarations var_declaration  {
            $$ = new CVarDeclarationsListNode($1, $2);
            setLocation(@$, @1, @2, $$);
        }
        | {$$ = 0;}
        ;

method_declarations
        : method_declarations method_declaration {
            $$ = new CMethodDeclarationsListNode($1, $2);
            setLocation(@$, @1, @2, $$);
        }
        | {$$ = 0;}
        ;

var_declaration
        : type IDENT SEMCOL {
            $$ = new CVarDeclarationRuleNode($1, $2);
            setLocation(@$, @1, @3, $$);
        }
        ;

method_declaration
        : PUBLIC type IDENT LPAREN param_arg RPAREN LBRACE method_body RETURN expression SEMCOL RBRACE {
            $$ = new CMethodDeclarationRuleNode($2, $3, $5, $8, $10);
            setLocation(@$, @1, @12, $$);
        }
        ;

vars_dec
        : vars_dec var_declaration {
            $$ = new CVarsDecListNode($1, $2);
            setLocation(@$, @1, @2, $$);
        }
        | var_declaration {
            $$ = new CVarsDecFirstNode($1);
            setLocation(@$, @1, @1, $$);
        }
        ;

stats
        : stats statement {
            $$ = new CStatsListNode($1, $2);
            setLocation(@$, @1, @2, $$);
        }
        | statement {
            $$ = new CStatsFirstNode($1);
            setLocation(@$, @1, @1, $$);
        }
        ;

method_body
        : vars_dec {
            $$ = new CMethodBodyVarsNode($1);
            setLocation(@$, @1, @1, $$);
        }
        | stats {
            $$ = new CMethodBodyStatsNode($1);
            setLocation(@$, @1, @1, $$);
        }
        | vars_dec stats {
            $$ = new CMethodBodyAllNode($1, $2);
            setLocation(@$, @1, @2, $$);
        }
        | {$$ = 0;}
        ;

param_arg
        : params {
            $$ = new CParamArgListNode($1);
            setLocation(@$, @1, @1, $$);
        }
        | {$$ = 0;}
        ;

params
        : param {
            $$ = new CParamsOneNode($1);
            setLocation(@$, @1, @1, $$);
        }
        | params COMMA param {
            $$ = new CParamsTwoNode($1, $3);
            setLocation(@$, @1, @3, $$);
        }
        ;

param
        : type IDENT {
            $$ = new CParamRuleNode($1, $2);
            setLocation(@$, @1, @2, $$);
        }
        ;

type
        : ARRAY {
            $$ = new CTypeRuleNode("int[]");
            setLocation(@$, @1, @1, $$);
        }
        | BOOLEAN_TYPE {
            $$ = new CTypeRuleNode("bool");
            setLocation(@$, @1, @1, $$);
        }
        | INT_TYPE {
            $$ = new CTypeRuleNode("int");
            setLocation(@$, @1, @1, $$);
        }
        | IDENT {
            $$ = new CTypeRuleNode($1);
            setLocation(@$, @1, @1, $$);
        }
        ;

statements
        : statements statement {
            $$ = new CNumerousStatementsNode($1, $2);
            setLocation(@$, @1, @2, $$);
        }
        | {$$ = 0;}
        ;

statement
        : LBRACE statements RBRACE {
            $$ = new CBracedStatementNode($2);
            setLocation(@$, @1, @3, $$);
        }
        | IF LPAREN expression RPAREN statement ELSE statement {
            $$ = new CIfStatementNode($3, $5, $7);
            setLocation(@$, @1, @7, $$);
        }
        | WHILE LPAREN expression RPAREN statement {
            $$ = new CWhileStatementNode($3, $5);
            setLocation(@$, @1, @5, $$);
        }
        | PRINT LPAREN expression RPAREN SEMCOL {
            $$ = new CPrintStatementNode($3);
            setLocation(@$, @1, @5, $$);
        }
        | IDENT EQ expression SEMCOL {
            $$ = new CAssignStatementNode($3, $1);
            setLocation(@$, @1, @4, $$);
        }
        | IDENT LBRACK expression RBRACK EQ expression SEMCOL {
            $$ = new CInvokeExpressionStatementNode($3, $6, $1);
            setLocation(@$, @1, @7, $$);
        }
        ;

expression
        : expression LBRACK expression RBRACK {
            $$ = new CInvokeExpressionNode($1, $3);
            setLocation(@$, @1, @4, $$);
        }
        | expression DOT LENGTH {
            $$ = new CLengthExpressionNode($1);
            setLocation(@$, @1, @3, $$);
        }
        | INT {
            $$ = new CIntExpressionNode(yylval.intValue);
            setLocation(@$, @1, @1, $$);
        }
        | BOOLEAN {
            $$ = new CBooleanExpressionNode(yylval.boolValue);
            setLocation(@$, @1, @1, $$);
        }
        | IDENT {
            $$ = new CIdentExpressionNode(yylval.str);
            setLocation(@$, @1, @1, $$);
        }
        | THIS {
            $$ = new CThisExpressionNode("this");
            setLocation(@$, @1, @1, $$);
        }
        | NEW INT_TYPE LBRACK expression RBRACK {
            $$ = new CNewArrayExpressionNode($4);
            setLocation(@$, @1, @5, $$);
        }
        | NEW IDENT LPAREN RPAREN {
            $$ = new CNewObjectExpressionNode($2);
            setLocation(@$, @1, @4, $$);
        }
        | BANG expression %prec BANG {
            $$ = new CNotExpressionNode($2);
            setLocation(@$, @1, @2, $$);
        }
        | LPAREN expression RPAREN {
            $$ = new CParenExpressionNode($2);
            setLocation(@$, @1, @3, $$);
        }
        | expression PLUS expression {
            $$ = new CArithmeticExpressionNode($1, $3, PLUS_OP);
            setLocation(@$, @1, @3, $$);
        }
        | expression MINUS expression {
            $$ = new CArithmeticExpressionNode($1, $3, MINUS_OP);
            setLocation(@$, @1, @3, $$);
        }
        | expression MULT expression {
            $$ = new CArithmeticExpressionNode($1, $3, MULT_OP);
            setLocation(@$, @1, @3, $$);
        }
        | expression AND expression {
            $$ = new CArithmeticExpressionNode($1, $3, AND_OP);
            setLocation(@$, @1, @3, $$);
        }
        | expression DIV expression {
            $$ = new CArithmeticExpressionNode($1, $3, DIV_OP);
            setLocation(@$, @1, @3, $$);
        }
        | expression LEQ expression {
            $$ = new CCompareExpressionNode($1, $3);
            setLocation(@$, @1, @3, $$);
        }
        | PLUS expression %prec UPLUS {
            $$ = new CUnaryExpressionNode($2, UPLUS_OP);
            setLocation(@$, @1, @2, $$);
        }
        | MINUS expression %prec UMINUS {
            $$ = new CUnaryExpressionNode($2, UMINUS_OP);
            setLocation(@$, @1, @2, $$);
        }
        | expression DOT IDENT LPAREN exp_arg RPAREN {
            $$ = new CInvokeMethodExpressionNode($1, $3, $5);
            setLocation(@$, @1, @6, $$);
        }
        ;

exp_arg
        : expressions {
            $$ = new CFewArgsExpressionNode($1);
            setLocation(@$, @1, @1, $$);
        }
        | {$$ = 0;}
        ;
expressions
        : expression  {
            $$ = new CLastListExpressionNode($1);
            setLocation(@$, @1, @1, $$);
        }
        | expressions COMMA expression {
            $$ = new CListExpressionNode($1, $3);
            setLocation(@$, @1, @3, $$);
        }
        ;

%%
