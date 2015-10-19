#ifndef CVISITOR_H_INCLUDED
#define CVISITOR_H_INCLUDED
#include "ast.h"

class CProgramRuleNode;
class CMainClassDeclarationRuleNode;
class CDeclarationsListNode;
class CDeclarationsEmptyNode;
class CClassDeclarationRuleNode;
class CExtendDeclarationRuleNode;
class CExtendDeclarationEmptyNode;
class CVarDeclarationsListNode;
class CVarDeclarationsEmptyNode;
class CMethodDeclarationsListNode;
class CMethodDeclarationsEmptyNode;
class CVarDeclarationRuleNode;
class CMethodDeclarationRuleNode;
class CVarsDecListNode;
class CVarsDecFirstNode;
class CStatsFirstNode;
class CStatsListNode;
class CMethodBodyVarsNode;
class CMethodBodyStatsNode;
class CMethodBodyAllNode;
class CMethodBodyEmptyNode;
class CParamArgListNode;
class CParamArgEmptyNode;
class CParamsOneNode;
class CParamsTwoNode;
class CParamRuleNode;
class CTypeRuleNode;
class CNumerousStatementsNode;
class CEmptyStatementsNode;
class CBracedStatementNode;
class CIfStatementNode;
class CWhileStatementNode;
class CPrintStatementNode;
class CAssignStatementNode;
class CInvokeExpressionStatementNode;
class CInvokeExpressionNode;
class CLengthExpressionNode;
class CArithmeticExpressionNode;
class CUnaryExpressionNode;
class CCompareExpressionNode;
class CNotExpressionNode;
class CNewArrayExpressionNode;
class CNewObjectExpressionNode;
class CIntExpressionNode;
class CBooleanExpressionNode;
class CIdentExpressionNode;
class CThisExpressionNode;
class CParenExpressionNode;
class CInvokeMethodExpressionNode;
class CFewArgsExpressionNode;
class CEmptyArgsExpression;
class CListExpressionNode;
class CLastListExpressionNode;

class CVisitor{
public:
  virtual void visit(const CProgramRuleNode* node) const = 0;
  virtual void visit(const CMainClassDeclarationRuleNode* node) const = 0;
  virtual void visit(const CDeclarationsListNode* node) const = 0;
  virtual void visit(const CDeclarationsEmptyNode* node) const = 0;
  virtual void visit(const CClassDeclarationRuleNode* node) const = 0;
  virtual void visit(const CExtendDeclarationRuleNode* node) const = 0;
  virtual void visit(const CExtendDeclarationEmptyNode* node) const = 0;
  virtual void visit(const CVarDeclarationsListNode* node) const = 0;
  virtual void visit(const CVarDeclarationsEmptyNode* node) const = 0;
  virtual void visit(const CMethodDeclarationsListNode* node) const = 0;
  virtual void visit(const CMethodDeclarationsEmptyNode* node) const = 0;
  virtual void visit(const CVarDeclarationRuleNode* node) const = 0;
  virtual void visit(const CMethodDeclarationRuleNode* node) const = 0;
  virtual void visit(const CVarsDecListNode* node) const = 0;
  virtual void visit(const CVarsDecFirstNode* node) const = 0;
  virtual void visit(const CStatsFirstNode* node) const = 0;
  virtual void visit(const CStatsListNode* node) const = 0;
  virtual void visit(const CMethodBodyVarsNode* node) const = 0;
  virtual void visit(const CMethodBodyStatsNode* node) const = 0;
  virtual void visit(const CMethodBodyAllNode* node) const = 0;
  virtual void visit(const CMethodBodyEmptyNode* node) const = 0;
  virtual void visit(const CParamArgListNode* node) const = 0;
  virtual void visit(const CParamArgEmptyNode* node) const = 0;
  virtual void visit(const CParamsOneNode* node) const = 0;
  virtual void visit(const CParamsTwoNode* node) const = 0;
  virtual void visit(const CParamRuleNode* node) const = 0;
  virtual void visit(const CTypeRuleNode* node) const = 0;

  virtual void visit(const CNumerousStatementsNode* node) const = 0;
  virtual void visit(const CEmptyStatementsNode* node) const = 0;
  virtual void visit(const CBracedStatementNode* node) const = 0;
  virtual void visit(const CIfStatementNode* node) const = 0;
  virtual void visit(const CWhileStatementNode* node) const = 0;
  virtual void visit(const CPrintStatementNode* node) const = 0;
  virtual void visit(const CAssignStatementNode* node) const = 0;
  virtual void visit(const CInvokeExpressionStatementNode* node) const = 0;

  virtual void visit(const CInvokeExpressionNode* node) const = 0;
  virtual void visit(const CLengthExpressionNode* node) const = 0;
  virtual void visit(const CArithmeticExpressionNode* node) const = 0;
  virtual void visit(const CUnaryExpressionNode* node) const = 0;
  virtual void visit(const CCompareExpressionNode* node) const = 0;
  virtual void visit(const CNotExpressionNode* node) const = 0;
  virtual void visit(const CNewArrayExpressionNode* node) const = 0;
  virtual void visit(const CNewObjectExpressionNode* node) const = 0;
  virtual void visit(const CIntExpressionNode* node) const = 0;
  virtual void visit(const CBooleanExpressionNode* node) const = 0;
  virtual void visit(const CIdentExpressionNode* node) const = 0;
  virtual void visit(const CThisExpressionNode* node) const = 0;
  virtual void visit(const CParenExpressionNode* node) const = 0;
  virtual void visit(const CInvokeMethodExpressionNode* node) const = 0;
  virtual void visit(const CFewArgsExpressionNode* node) const = 0;

  virtual void visit(const CEmptyArgsExpression* node) const = 0;
  virtual void visit(const CListExpressionNode* node) const = 0;
  virtual void visit(const CLastListExpressionNode* node) const = 0;
};

#endif
