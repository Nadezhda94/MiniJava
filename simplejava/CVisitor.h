#ifndef CVISITOR_H_INCLUDED
#define CVISITOR_H_INCLUDED
#include "ast.h"
class CVisitor{
public:
  virtual void visit(const CProgramRuleNode* node) const {}
  virtual void visit(const CMainClassDeclarationRuleNode* node) const {}
  virtual void visit(const CDeclarationsRuleNode* node) const {}
  virtual void visit(const CClassDeclarationRuleNode* node) const {}
  virtual void visit(const CExtendDeclarationRuleNode* node) const {}
  virtual void visit(const CVarDeclarationsListNode* node) const {}
  virtual void visit(const CVarDeclarationsEmptyNode* node) const {}
  virtual void visit(const CMethodDeclarationsListNode* node) const {}
  virtual void visit(const CMethodDeclarationsEmptyNode* node) const {}
  virtual void visit(const CVarDeclarationRuleNode* node) const {}
  virtual void visit(const CMethodDeclarationRuleNode* node) const {}
  virtual void visit(const CVarsDecListNode* node) const {}
  virtual void visit(const CVarsDecFirstNode* node) const {}
  virtual void visit(const CStatsTailNode* node) const {}
  virtual void visit(const CStatsFirstNode* node) const {}
  virtual void visit(const CMethodBodyVarsNode* node) const {}
  virtual void visit(const CMethodBodyStatsNode* node) const {}
  virtual void visit(const CMethodBodyAllNode* node) const {}
  virtual void visit(const CParamArgListNode* node) const {}
  virtual void visit(const CParamArgEmptyNode* node) const {}
  virtual void visit(const CParamsOneNode* node) const {}
  virtual void visit(const CParamsTwoNode* node) const {}
  virtual void visit(const CParamRuleNode* node) const {}
  virtual void visit(const CTypeRuleNode* node) const {}

  virtual void visit(const CNumerousStatementsNode* node) const {}
  virtual void visit(const CEmtptyStatementsNode* node) const {}
  virtual void visit(const CBracedStatementNode* node) const {}
  virtual void visit(const CIfStatementNodeNode* node) const {}
  virtual void visit(const CWhileStatementNode* node) const {}
  virtual void visit(const CPrintStatementNode* node) const {}
  virtual void visit(const CAssignStatementNode* node) const {}
  virtual void visit(const CInvokeExpressionStatementNode* node) const {}

  virtual void visit(const CInvokeExpressionNode* node) const {}
  virtual void visit(const CLengthExpressionNode* node) const {}
  virtual void visit(const CAriphmeticExpressionNode* node) const {}
  virtual void visit(const CCompareExpressionNode* node) const {}
  virtual void visit(const CNotExpressionNode* node) const {}
  virtual void visit(const CNewArrayExpressionNode* node) const {}
  virtual void visit(const CNewObjectExpressionNode* node) const {}
  virtual void visit(const CIntExpressionNode* node) const {}
  virtual void visit(const CBooleanExpressionNode* node) const {}
  virtual void visit(const CIdentExpressionNode* node) const {}
  virtual void visit(const CThisExpressionNode* node) const {}
  virtual void visit(const CParenExpressionNode* node) const {}
  virtual void visit(const CInvokeMethodExpressionNode* node) const {}
  virtual void visit(const CFewArgsExpressionNode* node) const {}

  virtual void visit(const CEmptyArgsExpression* node) const {}
  virtual void visit(const CListExpressionNode* node) const {}
  virtual void visit(const CLastListExpressionNode* node) const {}
}

#endif
