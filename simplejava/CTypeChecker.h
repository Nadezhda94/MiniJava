#ifndef CTYPECHECKER_H_INCLUDED
#define CTYPECHECKER_H_INCLUDED
#include <iostream>
#include "CVisitor.h"
#include "ast.h"
#include "SymbolsTable.h"
using namespace SymbolsTable;
using std::vector;
using std::cout;
using std::endl;
using std::string;

class CTypeChecker : public CVisitor{
public:
  CTable table;
  void visit(const CProgramRuleNode* node){}
  void visit(const CMainClassDeclarationRuleNode* node){}
  void visit(const CDeclarationsListNode* node){}
  void visit(const CDeclarationsEmptyNode* node){}
  void visit(const CClassDeclarationRuleNode* node){}
  void visit(const CExtendDeclarationRuleNode* node){}
  void visit(const CExtendDeclarationEmptyNode* node){}
  void visit(const CVarDeclarationsListNode* node){}
  void visit(const CVarDeclarationsEmptyNode* node){}
  void visit(const CMethodDeclarationsListNode* node){}
  void visit(const CMethodDeclarationsEmptyNode* node){}
  void visit(const CVarDeclarationRuleNode* node){}
  void visit(const CMethodDeclarationRuleNode* node){}
  void visit(const CVarsDecListNode* node){}
  void visit(const CVarsDecFirstNode* node){}
  void visit(const CStatsFirstNode* node){}
  void visit(const CStatsListNode* node){}
  void visit(const CMethodBodyVarsNode* node){}
  void visit(const CMethodBodyStatsNode* node){}
  void visit(const CMethodBodyAllNode* node){}
  void visit(const CMethodBodyEmptyNode* node){}
  void visit(const CParamArgListNode* node){}
  void visit(const CParamArgEmptyNode* node){}
  void visit(const CParamsOneNode* node){}
  void visit(const CParamsTwoNode* node){}
  void visit(const CParamRuleNode* node){}
  void visit(const CTypeRuleNode* node){}

  void visit(const CNumerousStatementsNode* node){}
  void visit(const CEmptyStatementsNode* node){}
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

  void visit(const CEmptyArgsExpression* node){}
  void visit(const CListExpressionNode* node){}
  void visit(const CLastListExpressionNode* node){}
};
#endif
