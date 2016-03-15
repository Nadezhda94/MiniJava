#ifndef VISITOR_H_INCLUDED
#define VISITOR_H_INCLUDED
#include "../Structs/Ast.h"

class CProgramRuleNode;
class CMainClassDeclarationRuleNode;
class CDeclarationsListNode;
class CClassDeclarationRuleNode;
class CExtendDeclarationRuleNode;
class CVarDeclarationsListNode;
class CMethodDeclarationsListNode;
class CVarDeclarationRuleNode;
class CMethodDeclarationRuleNode;
class CVarsDecListNode;
class CVarsDecFirstNode;
class CStatsFirstNode;
class CStatsListNode;
class CMethodBodyVarsNode;
class CMethodBodyStatsNode;
class CMethodBodyAllNode;
class CParamArgListNode;
class CParamsOneNode;
class CParamsTwoNode;
class CParamRuleNode;
class CTypeRuleNode;
class CNumerousStatementsNode;
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
class CListExpressionNode;
class CLastListExpressionNode;

class CVisitor{
public:
  virtual void visit(const CProgramRuleNode* node) = 0;
  virtual void visit(const CMainClassDeclarationRuleNode* node) = 0;
  virtual void visit(const CDeclarationsListNode* node) = 0;
  virtual void visit(const CClassDeclarationRuleNode* node) = 0;
  virtual void visit(const CExtendDeclarationRuleNode* node) = 0;
  virtual void visit(const CVarDeclarationsListNode* node) = 0;
  virtual void visit(const CMethodDeclarationsListNode* node) = 0;
  virtual void visit(const CVarDeclarationRuleNode* node) = 0;
  virtual void visit(const CMethodDeclarationRuleNode* node) = 0;
  virtual void visit(const CVarsDecListNode* node) = 0;
  virtual void visit(const CVarsDecFirstNode* node) = 0;
  virtual void visit(const CStatsFirstNode* node) = 0;
  virtual void visit(const CStatsListNode* node) = 0;
  virtual void visit(const CMethodBodyVarsNode* node) = 0;
  virtual void visit(const CMethodBodyStatsNode* node) = 0;
  virtual void visit(const CMethodBodyAllNode* node) = 0;
  virtual void visit(const CParamArgListNode* node) = 0;
  virtual void visit(const CParamsOneNode* node) = 0;
  virtual void visit(const CParamsTwoNode* node) = 0;
  virtual void visit(const CParamRuleNode* node) = 0;
  virtual void visit(const CTypeRuleNode* node) = 0;
  virtual void visit(const CNumerousStatementsNode* node) = 0;
  virtual void visit(const CBracedStatementNode* node) = 0;
  virtual void visit(const CIfStatementNode* node) = 0;
  virtual void visit(const CWhileStatementNode* node) = 0;
  virtual void visit(const CPrintStatementNode* node) = 0;
  virtual void visit(const CAssignStatementNode* node) = 0;
  virtual void visit(const CInvokeExpressionStatementNode* node) = 0;
  virtual void visit(const CInvokeExpressionNode* node) = 0;
  virtual void visit(const CLengthExpressionNode* node) = 0;
  virtual void visit(const CArithmeticExpressionNode* node) = 0;
  virtual void visit(const CUnaryExpressionNode* node) = 0;
  virtual void visit(const CCompareExpressionNode* node) = 0;
  virtual void visit(const CNotExpressionNode* node) = 0;
  virtual void visit(const CNewArrayExpressionNode* node) = 0;
  virtual void visit(const CNewObjectExpressionNode* node) = 0;
  virtual void visit(const CIntExpressionNode* node) = 0;
  virtual void visit(const CBooleanExpressionNode* node) = 0;
  virtual void visit(const CIdentExpressionNode* node) = 0;
  virtual void visit(const CThisExpressionNode* node) = 0;
  virtual void visit(const CParenExpressionNode* node) = 0;
  virtual void visit(const CInvokeMethodExpressionNode* node) = 0;
  virtual void visit(const CFewArgsExpressionNode* node) = 0;
  virtual void visit(const CListExpressionNode* node) = 0;
  virtual void visit(const CLastListExpressionNode* node) = 0;
};






#endif
