#ifndef CSYMBOLTABLEBUILDER_H_INCLUDED
#define CSYMBOLTABLEBUILDER_H_INCLUDED
#include <iostream>
#include "CVisitor.h"
#include "ast.h"
#include "SymbolsTable.h"
using namespace SymbolsTable;
using std::vector;
using std::cout;
using std::endl;
using std::string;

class CSymbolTableBuilder : public CVisitor{
public:
  CTable table;
  bool inMethod;
  string lastTypeValue;
  CSymbolTableBuilder(): inMethod(0) {}
  void visit(const CProgramRuleNode* node){
    node->mainClass->accept(this);
    node->decl->accept(this);
  }
  void visit(const CMainClassDeclarationRuleNode* node){
    inMethod = 0;
    table.classInfo.push_back(CClassInfo(node->className));
    CMethodInfo mainMthd("main", "void");
    mainMthd.params.push_back(CVarInfo(node->argNames, "String[]"));
    table.classInfo.back().methods.push_back(mainMthd);
    node->stmt->accept(this);
  }
  void visit(const CDeclarationsListNode* node){
    node->decl->accept(this);
    node->cl->accept(this);
  }
  void visit(const CClassDeclarationRuleNode* node){
    inMethod = 0;
    table.classInfo.push_back(CClassInfo(node->ident));
    node->extDecl->accept(this);
    node->method->accept(this);
    node->vars->accept(this);
  }
  void visit(const CExtendDeclarationRuleNode* node){
    table.classInfo.back().parent = node->ident;
  }

  void visit(const CVarDeclarationsListNode* node){
    node->list->accept(this);
    node->item->accept(this);
  }
  void visit(const CMethodDeclarationsListNode* node){
    node->list->accept(this);
    node->item->accept(this);
  }
  void visit(const CVarDeclarationRuleNode* node){
    node->type->accept(this);
    if (inMethod) {
      table.classInfo.back().methods.back().vars.push_back(CVarInfo(node->ident, lastTypeValue));
    } else {
      table.classInfo.back().vars.push_back(CVarInfo(node->ident, lastTypeValue));
    }
  }
  void visit(const CMethodDeclarationRuleNode* node){
    node->type->accept(this);
    table.classInfo.back().methods.push_back(CMethodInfo(node->ident, lastTypeValue));
    node->param_arg->accept(this);
    inMethod = 1;
    node->method_body->accept(this);
    inMethod = 0;
  }
  void visit(const CVarsDecListNode* node){
    
    node->list->accept(this);
    node->next->accept(this);

  }
  void visit(const CVarsDecFirstNode* node){
    node->first->accept(this);
  }
  void visit(const CStatsFirstNode* node){

  }
  void visit(const CStatsListNode* node){}
  void visit(const CMethodBodyVarsNode* node){
    node->vars->accept(this);
  }
  void visit(const CMethodBodyStatsNode* node){

  }
  void visit(const CMethodBodyAllNode* node){
    node->vars->accept(this);
  }
  void visit(const CParamArgListNode* node){
    node->params->accept(this);
  }
  void visit(const CParamsOneNode* node){
    node->param->accept(this);
  }
  void visit(const CParamsTwoNode* node){
    node->first->accept(this);
    node->second->accept(this);
  }
  void visit(const CParamRuleNode* node){
    node->type->accept(this);
    table.classInfo.back().methods.back().params.push_back(CVarInfo(node->ident, lastTypeValue));
  }
  void visit(const CTypeRuleNode* node){
    lastTypeValue = node->type;
  }

  void visit(const CNumerousStatementsNode* node){}
  void visit(const CBracedStatementNode* node){}
  void visit(const CIfStatementNode* node){}
  void visit(const CWhileStatementNode* node){}
  void visit(const CPrintStatementNode* node){}
  void visit(const CAssignStatementNode* node){}
  void visit(const CInvokeExpressionStatementNode* node){}

  void visit(const CInvokeExpressionNode* node){}
  void visit(const CLengthExpressionNode* node){}
  void visit(const CArithmeticExpressionNode* node){}
  void visit(const CUnaryExpressionNode* node){}
  void visit(const CCompareExpressionNode* node){}
  void visit(const CNotExpressionNode* node){}
  void visit(const CNewArrayExpressionNode* node){}
  void visit(const CNewObjectExpressionNode* node){}
  void visit(const CIntExpressionNode* node){}
  void visit(const CBooleanExpressionNode* node){}
  void visit(const CIdentExpressionNode* node){}
  void visit(const CThisExpressionNode* node){}
  void visit(const CParenExpressionNode* node){}
  void visit(const CInvokeMethodExpressionNode* node){}
  void visit(const CFewArgsExpressionNode* node){}

  void visit(const CListExpressionNode* node){}
  void visit(const CLastListExpressionNode* node){}
};
#endif
