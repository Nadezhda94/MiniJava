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
  string lastTypeValue;
  bool inMethod;
  void visit(const CProgramRuleNode* node){}
  void visit(const CMainClassDeclarationRuleNode* node){}
  void visit(const CDeclarationsListNode* node){}
  void visit(const CClassDeclarationRuleNode* node){}
  void visit(const CExtendDeclarationRuleNode* node){}
  void visit(const CVarDeclarationsListNode* node){}
  void visit(const CMethodDeclarationsListNode* node){}
  void visit(const CVarDeclarationRuleNode* node){
	  CTypeRuleNode* tmp = dynamic_cast<CTypeRuleNode*>(node->type);
	  if ((tmp->type != "bool" ) && (tmp->type != "int") && (tmp->type != "int[]")) {
		bool flag = false;
		for (int i = 0; i < table.classInfo.size(); i++){
		  flag = flag || (table.classInfo[i].name == tmp->type);
		}
	  
		if (!flag)
		  cout << "No such type: " << tmp->type << endl;
	 }
	 
	 delete tmp;
  }
  void visit(const CMethodDeclarationRuleNode* node){}
  void visit(const CVarsDecListNode* node){}
  void visit(const CVarsDecFirstNode* node){}
  void visit(const CStatsFirstNode* node){}
  void visit(const CStatsListNode* node){}
  void visit(const CMethodBodyVarsNode* node){}
  void visit(const CMethodBodyStatsNode* node){}
  void visit(const CMethodBodyAllNode* node){}
  void visit(const CParamArgListNode* node){}
  void visit(const CParamsOneNode* node){}
  void visit(const CParamsTwoNode* node){}
  void visit(const CParamRuleNode* node){}
  void visit(const CTypeRuleNode* node){}

  void visit(const CNumerousStatementsNode* node){}
  void visit(const CBracedStatementNode* node){}
  
  void visit(const CIfStatementNode* node){
	  node->expression->accept(this);
	  if (lastTypeValue != "bool")
		  cout << "Error in if/else statement expression";
  }
  
  void visit(const CWhileStatementNode* node){
	  node->expression->accept(this);
	  if (lastTypeValue != "bool")
		  cout << "Error in while statement expression";
  }
  
  void visit(const CPrintStatementNode* node){
	  node->expression->accept(this);
	  if (lastTypeValue != "int")
		  cout << "Error in print expression";  
  }
  
  void visit(const CAssignStatementNode* node){}
  void visit(const CInvokeExpressionStatementNode* node){}

  void visit(const CInvokeExpressionNode* node){}
  void visit(const CLengthExpressionNode* node){}
  
  void visit(const CArithmeticExpressionNode* node){
	  node->firstExp->accept(this);
	  if ((lastTypeValue != "int") || (lastTypeValue != "bool"))
		  cout << "Error in arithmetic expression";
	  
	  node->secondExp->accept(this);
	  if ((lastTypeValue != "int") || (lastTypeValue != "bool"))
		  cout << "Error in arithmetic expression";
	  
	  lastTypeValue = "int";
  }
  void visit(const CUnaryExpressionNode* node){
	  node->expr->accept(this);
	  
	  if (lastTypeValue != "int")
		  cout << "Error in unary expression";
		  
	lastTypeValue = "int";
  }
  void visit(const CCompareExpressionNode* node){
	  node->firstExp->accept(this);
	  if ((lastTypeValue != "int") || (lastTypeValue != "bool"))
		  cout << "Error in compare expression";
	  
	  node->secondExp->accept(this);
	  if ((lastTypeValue != "int") || (lastTypeValue != "bool"))
		  cout << "Error in compare expression";
	  
	  lastTypeValue = "bool";
  }
  void visit(const CNotExpressionNode* node){ 
	  node->expr->accept(this);
	  
	  if (lastTypeValue != "int")
		  cout << "Error in NOT expression";
		  
	lastTypeValue = "bool";
  }
  
  void visit(const CNewArrayExpressionNode* node){}
  void visit(const CNewObjectExpressionNode* node){}
  
  void visit(const CIntExpressionNode* node){
	  lastTypeValue = "int";
  
  }
  void visit(const CBooleanExpressionNode* node){
	  lastTypeValue = "bool";
  }
  
  void visit(const CIdentExpressionNode* node){}
  void visit(const CThisExpressionNode* node){}
  void visit(const CParenExpressionNode* node){}
  void visit(const CInvokeMethodExpressionNode* node){
	  
  }
  void visit(const CFewArgsExpressionNode* node){}
  void visit(const CListExpressionNode* node){}
  void visit(const CLastListExpressionNode* node){}
};
#endif
