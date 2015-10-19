#ifndef CPRINTVISITOR_H_INCLUDED
#define CPRINTVISITOR_H_INCLUDED
#include <iostream>
#include "CVisitor.h"
#include "ast.h"
using namespace std;

class CPrintVisitor : public CVisitor{
public:
  void visit(const CProgramRuleNode* node) const {
      const CPrintVisitor* const self = this;
      cout<<"ProgramNode"<<endl<<"\t";
      node->mainClass->accept(dynamic_cast<const CVisitor* const>(self));
      cout<<endl<<"\t";
      node->decl->accept(dynamic_cast<const CVisitor* const>(self));
      cout<<endl;
  }
  void visit(const CMainClassDeclarationRuleNode* node) const {cout<<"MainClassNode";}
  void visit(const CDeclarationsListNode* node) const {cout<<"DeclarationsNode";}
  void visit(const CDeclarationsEmptyNode* node) const {}
  void visit(const CClassDeclarationRuleNode* node) const {}
  void visit(const CExtendDeclarationRuleNode* node) const {}
  void visit(const CExtendDeclarationEmptyNode* node) const {}
  void visit(const CVarDeclarationsListNode* node) const {}
  void visit(const CVarDeclarationsEmptyNode* node) const {}
  void visit(const CMethodDeclarationsListNode* node) const {}
  void visit(const CMethodDeclarationsEmptyNode* node) const {}
  void visit(const CVarDeclarationRuleNode* node) const {}
  void visit(const CMethodDeclarationRuleNode* node) const {}
  void visit(const CVarsDecListNode* node) const {}
  void visit(const CVarsDecFirstNode* node) const {}
  void visit(const CStatsFirstNode* node) const {}
  void visit(const CStatsListNode* node) const {}
  void visit(const CMethodBodyVarsNode* node) const {}
  void visit(const CMethodBodyStatsNode* node) const {}
  void visit(const CMethodBodyAllNode* node) const {}
  void visit(const CMethodBodyEmptyNode* node) const {}
  void visit(const CParamArgListNode* node) const {}
  void visit(const CParamArgEmptyNode* node) const {}
  void visit(const CParamsOneNode* node) const {}
  void visit(const CParamsTwoNode* node) const {}
  void visit(const CParamRuleNode* node) const {}
  void visit(const CTypeRuleNode* node) const {}

  void visit(const CNumerousStatementsNode* node) const {}
  void visit(const CEmptyStatementsNode* node) const {}
  void visit(const CBracedStatementNode* node) const {}
  void visit(const CIfStatementNode* node) const {}
  void visit(const CWhileStatementNode* node) const {}
  void visit(const CPrintStatementNode* node) const {}
  void visit(const CAssignStatementNode* node) const {}
  void visit(const CInvokeExpressionStatementNode* node) const {}

  void visit(const CInvokeExpressionNode* node) const {}
  void visit(const CLengthExpressionNode* node) const {}
  void visit(const CArithmeticExpressionNode* node) const {}
  void visit(const CUnaryExpressionNode* node) const {}
  void visit(const CCompareExpressionNode* node) const {}
  void visit(const CNotExpressionNode* node) const {}
  void visit(const CNewArrayExpressionNode* node) const {}
  void visit(const CNewObjectExpressionNode* node) const {}
  void visit(const CIntExpressionNode* node) const {}
  void visit(const CBooleanExpressionNode* node) const {}
  void visit(const CIdentExpressionNode* node) const {}
  void visit(const CThisExpressionNode* node) const {}
  void visit(const CParenExpressionNode* node) const {}
  void visit(const CInvokeMethodExpressionNode* node) const {}
  void visit(const CFewArgsExpressionNode* node) const {}

  void visit(const CEmptyArgsExpression* node) const {}
  void visit(const CListExpressionNode* node) const {}
  void visit(const CLastListExpressionNode* node) const {}
};

#endif
