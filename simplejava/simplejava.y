%{

#include <iostream>
#include <string>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include "simplejava.tab.hpp"
#include "ast.h"
#include "CPrintVisitor.h"
#include "CSymbolTableBuilder.h"
#include "CTypeChecker.h"
#include "Translator.h"

using std::cout;
using std::endl;
using std::strcpy;
extern FILE * yyin;
int yylex();
void yyerror(const char * s){
    std::cout << s << std::endl;
    std::cout << "line number: " << yylloc.first_line << std::endl;
    std:: cout << "position in line: " << yylloc.last_column << std::endl;
};

void testBuilder(CSymbolTableBuilder& table_vis){
    auto it = table_vis.table.classInfo[1].methods.begin();
    auto itEnd = table_vis.table.classInfo[1].methods.end();
    for ( ; it != itEnd; ++it) {
        std::cout << it->name << std::endl;
        auto innerIt = it->vars.begin();
        auto innerItEnd = it->vars.end();
        for (;innerIt != innerItEnd; ++innerIt) {
            std::cerr << innerIt -> name << " " << innerIt->type << std::endl;
        }
    }
    std::cout << table_vis.table.classInfo[1].name << std::endl;
}


%}
%locations

%union {
    int intValue;
    bool boolValue;
    char str[4096];
    struct CNode* node;
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

%type <node> expression statement extend_declaration param_arg var_declaration var_declarations class_declaration exp_arg program main_class
%type <node> expressions statements type param params method_body vars_dec stats method_declarations method_declaration declarations

%%


program

        : main_class declarations {
            CPrintVisitor print_vis;
            CSymbolTableBuilder table_vis;
            CTypeChecker checker_vis;
            CProgramRuleNode* ptr = new CProgramRuleNode(dynamic_cast<CMainClassNode*>($1), dynamic_cast<CDeclarationsNode*>($2));
            $$ = ptr;
            ptr->accept(&print_vis);
            cout<<endl;
            ptr->accept(&table_vis);
            checker_vis.table = table_vis.table;
            ptr->accept(&checker_vis);
//            testBuilder(table_vis);
            delete ptr;
          }
        ;

main_class
        : CLASS IDENT LBRACE PUBLIC STATIC VOID MAIN LPAREN STRING LBRACK RBRACK IDENT RPAREN LBRACE statement RBRACE RBRACE {
            $$ = new CMainClassDeclarationRuleNode($2, $12, dynamic_cast<CStatementNode*>($15));
          }
        ;

declarations
        : declarations class_declaration {
            $$ = new CDeclarationsListNode(dynamic_cast<CDeclarationsNode*>($1), dynamic_cast<CClassDeclarationNode*>($2));
          }
        | {$$ = 0;}
        ;

class_declaration
        : CLASS IDENT extend_declaration LBRACE var_declarations method_declarations RBRACE {
            $$ = new CClassDeclarationRuleNode($2, dynamic_cast<CExtendDeclarationNode*>($3),
                                               dynamic_cast<CVarDeclarationsNode*>($5),
                                               dynamic_cast<CMethodDeclarationsNode*>($6));
          }
        ;

extend_declaration
        : EXTENDS IDENT {$$ = new CExtendDeclarationRuleNode($2);}
        | {$$ = 0;}
        ;

var_declarations
        : var_declarations var_declaration  {
            $$ = new CVarDeclarationsListNode(dynamic_cast<CVarDeclarationsNode*>($1), dynamic_cast<CVarDeclarationNode*>($2));
          }
        | {$$ = 0;}
        ;

method_declarations
        : method_declarations method_declaration {
            $$ = new CMethodDeclarationsListNode(dynamic_cast<CMethodDeclarationsNode*>($1), dynamic_cast<CMethodDeclarationNode*>($2));
          }
        | {$$ = 0;}
        ;

var_declaration
        : type IDENT SEMCOL {$$ = new CVarDeclarationRuleNode(dynamic_cast<CTypeNode*>($1), $2);}
        ;

method_declaration
        : PUBLIC type IDENT LPAREN param_arg RPAREN LBRACE method_body RETURN expression SEMCOL RBRACE {
            $$ = new CMethodDeclarationRuleNode(dynamic_cast<CTypeNode*>($2), $3,
                                                dynamic_cast<CParamArgNode*>($5),
                                                dynamic_cast<CMethodBodyNode*>($8),
                                                dynamic_cast<CExpressionNode*>($10));
          }
        ;

vars_dec
        : vars_dec var_declaration {
            $$ = new CVarsDecListNode(dynamic_cast<CVarsDecNode*>($1), dynamic_cast<CVarDeclarationNode*>($2));
          }
        | var_declaration {$$ = new CVarsDecFirstNode(dynamic_cast<CVarDeclarationNode*>($1));}
        ;

stats
        : stats statement {
            $$ = new CStatsListNode(dynamic_cast<CStatsNode*>($1), dynamic_cast<CStatementNode*>($2));
          }
        | statement {$$ = new CStatsFirstNode(dynamic_cast<CStatementNode*>($1));}
        ;

method_body
        : vars_dec {$$ = new CMethodBodyVarsNode(dynamic_cast<CVarsDecNode*>($1));}
        | stats {$$ = new CMethodBodyStatsNode(dynamic_cast<CStatsNode*>($1));}
        | vars_dec stats {
            $$ = new CMethodBodyAllNode(dynamic_cast<CVarsDecNode*>($1), dynamic_cast<CStatsNode*>($2));
          }
        | {$$ = 0;}
        ;

param_arg
        : params {$$ = new CParamArgListNode(dynamic_cast<CParamsNode*>($1));}
        | {$$ = 0;}
        ;

params
        : param {$$ = new CParamsOneNode(dynamic_cast<CParamNode*>($1));}
        | params COMMA param {
            $$ = new CParamsTwoNode(dynamic_cast<CParamsNode*>($1), dynamic_cast<CParamNode*>($3));
          }
        ;

param
        : type IDENT {$$ = new CParamRuleNode(dynamic_cast<CTypeNode*>($1), $2);}
        ;

type
        : ARRAY {$$ = new CTypeRuleNode("int[]");}
        | BOOLEAN_TYPE {$$ = new CTypeRuleNode("bool");}
        | INT_TYPE {$$ = new CTypeRuleNode("int");}
        | IDENT {$$ = new CTypeRuleNode($1);}
        ;

statements
        : statements statement {
            $$ = new CNumerousStatementsNode(dynamic_cast<CStatementsNode*>($1), dynamic_cast<CStatementNode*>($2));
          }
        | {$$ = 0;}
        ;

statement
        : LBRACE statements RBRACE {$$ = new CBracedStatementNode(dynamic_cast<CStatementsNode*>($2));}
        | IF LPAREN expression RPAREN statement ELSE statement {
            $$ = new CIfStatementNode(dynamic_cast<CExpressionNode*>($3), dynamic_cast<CStatementNode*>($5), dynamic_cast<CStatementNode*>($7));
          }
        | WHILE LPAREN expression RPAREN statement {
            $$ = new CWhileStatementNode(dynamic_cast<CExpressionNode*>($3), dynamic_cast<CStatementNode*>($5));
          }
        | PRINT LPAREN expression RPAREN SEMCOL {$$ = new CPrintStatementNode(dynamic_cast<CExpressionNode*>($3));}
        | IDENT EQ expression SEMCOL {$$ = new CAssignStatementNode(dynamic_cast<CExpressionNode*>($3), $1);}
        | IDENT LBRACK expression RBRACK EQ expression SEMCOL {
            $$ = new CInvokeExpressionStatementNode(dynamic_cast<CExpressionNode*>($3), dynamic_cast<CExpressionNode*>($6), $1);
          }
        ;

expression
        : expression LBRACK expression RBRACK {
            $$ = new CInvokeExpressionNode(dynamic_cast<CExpressionNode*>($1), dynamic_cast<CExpressionNode*>($3));
          }
        | expression DOT LENGTH {$$ = new CLengthExpressionNode(dynamic_cast<CExpressionNode*>($1));}
        | INT {$$ = new CIntExpressionNode(yylval.intValue);}
        | BOOLEAN {$$ = new CBooleanExpressionNode(yylval.boolValue);}
        | IDENT {$$ = new CIdentExpressionNode(yylval.str);}
        | THIS {$$ = new CThisExpressionNode("this");}
        | NEW INT_TYPE LBRACK expression RBRACK {
            $$ = new CNewArrayExpressionNode(dynamic_cast<CExpressionNode*>($4));
          }
        | NEW IDENT LPAREN RPAREN {
            $$ = new CNewObjectExpressionNode($2);
          }
        | BANG expression %prec BANG {
            $$ = new CNotExpressionNode(dynamic_cast<CExpressionNode*>($2));
          }
        | LPAREN expression RPAREN {
            $$ = new CParenExpressionNode(dynamic_cast<CExpressionNode*>($2));
          }
        | expression PLUS expression {
            $$ = new CArithmeticExpressionNode(dynamic_cast<CExpressionNode*>($1), dynamic_cast<CExpressionNode*>($3), PLUS_OP);
          }
        | expression MINUS expression {
            $$ = new CArithmeticExpressionNode(dynamic_cast<CExpressionNode*>($1), dynamic_cast<CExpressionNode*>($3), MINUS_OP);
          }
        | expression MULT expression {
            $$ = new CArithmeticExpressionNode(dynamic_cast<CExpressionNode*>($1), dynamic_cast<CExpressionNode*>($3), MULT_OP);
          }
        | expression AND expression {
            $$ = new CArithmeticExpressionNode(dynamic_cast<CExpressionNode*>($1), dynamic_cast<CExpressionNode*>($3), AND_OP);
          }
        | expression DIV expression {
            $$ = new CArithmeticExpressionNode(dynamic_cast<CExpressionNode*>($1), dynamic_cast<CExpressionNode*>($3), DIV_OP);
          }
        | expression LEQ expression {
            $$ = new CCompareExpressionNode(dynamic_cast<CExpressionNode*>($1), dynamic_cast<CExpressionNode*>($3));
          }
        | PLUS expression %prec UPLUS {$$ = new CUnaryExpressionNode(dynamic_cast<CExpressionNode*>($2), UPLUS_OP);}
        | MINUS expression %prec UMINUS {$$ = new CUnaryExpressionNode(dynamic_cast<CExpressionNode*>($2), UMINUS_OP);}
        | expression DOT IDENT LPAREN exp_arg RPAREN {
            $$ = new CInvokeMethodExpressionNode(dynamic_cast<CExpressionNode*>($1), $3, dynamic_cast<CExpArgNode*>($5));
          }
        ;

exp_arg
        : expressions {$$ = new CFewArgsExpressionNode(dynamic_cast<CExpressionsNode*>($1));}
        | {$$ = 0;}
        ;
expressions
        : expression  {$$ = new CLastListExpressionNode(dynamic_cast<CExpressionNode*>($1));}
        | expressions COMMA expression {
            $$ = new CListExpressionNode(dynamic_cast<CExpressionsNode*>($1), dynamic_cast<CExpressionNode*>($3));
          }
        ;

%%
