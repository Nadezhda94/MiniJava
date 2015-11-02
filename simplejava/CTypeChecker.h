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
  void visit(const CProgramRuleNode* node){
	  if (node->mainClass != NULL)
		node->mainClass->accept(this);
	
	  if (node->decl != NULL)
		node->decl->accept(this);
  }
  void visit(const CMainClassDeclarationRuleNode* node){
	  if (node->stmt != NULL)
		node->stmt->accept(this);
  }
  void visit(const CDeclarationsListNode* node){
	  if (node->decl != NULL)
		  node->decl->accept(this);
	  
	  node->cl->accept(this);
  }
  
  void visit(const CClassDeclarationRuleNode* node){
	  if (node->extDecl != NULL)
		  node->extDecl->accept(this);
	  
	  if (node->vars != NULL)
		  node->vars->accept(this);
	  
	  if (node->method != NULL)
		  node->method->accept(this);
  }
  
  void visit(const CExtendDeclarationRuleNode* node){
	  string parent = node->ident;
	  do{
		  int i = 0;
		  while ((table.classInfo[i].name != parent) && (i < table.classInfo.size()))
			  i++;
		  
		  if (i == table.classInfo.size())
			  parent = "";
		  else
			  parent = table.classInfo[i].parent;
	  } while ((parent != "") || (parent != node->ident));
		  
	  if (parent != "")
		  cout << "Cyclic inheritance with " << node->ident << endl;
  }
  
  void visit(const CVarDeclarationsListNode* node){
	  if (node->list != NULL)
		  node->list->accept(this);
	  
	  if (node->item != NULL)
		  node->item->accept(this);
  }
  
  void visit(const CMethodDeclarationsListNode* node){
	if (node->list != NULL) {
		node->list->accept(this);
	}
	node->item->accept(this);
  }
  
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
  void visit(const CMethodDeclarationRuleNode* node){
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
  void visit(const CVarsDecListNode* node){
	  if (node->list != NULL)
		  node->list->accept(this);
	  
	  if (node->next != NULL)
		  node->next->accept(this);
  }
  
  void visit(const CVarsDecFirstNode* node){
	  if (node->first != NULL)
		  node->first->accept(this);
  }
  
  void visit(const CStatsFirstNode* node){
	  if (node->stm != NULL)
		  node->stm->accept(this);
  }
  
  void visit(const CStatsListNode* node){
	  if (node->list != NULL)
		  node->list->accept(this);
	  if (node->stm != NULL)
		  node->stm->accept(this);
	  }
  
  void visit(const CMethodBodyVarsNode* node){
	  if (node->vars != NULL)
		  node->vars->accept(this);
  }
  
  void visit(const CMethodBodyStatsNode* node){
	  if (node->stats != NULL)
		  node->stats->accept(this);
  }
  
  void visit(const CMethodBodyAllNode* node){
	  if (node->vars != NULL)
		  node->vars->accept(this);
	  
	  if (node->stats != NULL)
		  node->stats->accept(this);
  }
  
  void visit(const CParamArgListNode* node){
	  if (node->params != NULL)
		  node->params->accept(this);
  }
  
  void visit(const CParamsOneNode* node){
	  if (node->param != NULL)
		  node->param->accept(this);
  }
  
  void visit(const CParamsTwoNode* node){
	  if (node->first != NULL)
		  node->first->accept(this);
	  if (node->second != NULL)
		  node->second->accept(this);
	  }
  
  void visit(const CParamRuleNode* node){
	  if (node->type != NULL)
		  node->type->accept(this);
  }
  
  void visit(const CTypeRuleNode* node){
	  lastTypeValue = node->type;
  }

  void visit(const CNumerousStatementsNode* node){
	  if (node->statements != NULL)
		  node->statements->accept(this);
	  if (node->statement != NULL)
		  node->statement->accept(this);
  }
  
  void visit(const CBracedStatementNode* node){
	  if (node->statements != NULL)
		  node->statements->accept(this);
  }
  
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
  
  void visit(const CAssignStatementNode* node){
	  //сложновато
	  if (node->expression != NULL)
		  node->expression->accept(this);
  }
  
  void visit(const CInvokeExpressionStatementNode* node){
	  if (node->firstexpression != NULL)
		  node->firstexpression->accept(this);
	  if (lastTypeValue != "int")
		  cout << "Array index is not int in " << node->identifier << endl;
	  
	  
	  if (node->secondexpression != NULL)
		  node->secondexpression->accept(this);
	  
	  //тут тоже присваивание проверить
  }

  void visit(const CInvokeExpressionNode* node){
	  
	  if (node->firstExp != NULL)
		  node->firstExp->accept(this);;
	  
	  
	  if (node->secondExp != NULL)
		  node->secondExp->accept(this);
  }
  
  void visit(const CLengthExpressionNode* node){
	  
	if (node->expr != NULL)
		node->expr->accept(this);
	}
  
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
  
  void visit(const CNewArrayExpressionNode* node){
	  if (node->expr != NULL)
		  node->expr->accept(this);
	  
	  if (lastTypeValue != "int")
		  cout << "Array index is not int in new array" << endl;
  }
  void visit(const CNewObjectExpressionNode* node){
	  //и тут тоже
  }
  
  void visit(const CIntExpressionNode* node){
	  lastTypeValue = "int";
  
  }
  void visit(const CBooleanExpressionNode* node){
	  lastTypeValue = "bool";
  }
  
  void visit(const CIdentExpressionNode* node){
	  //nothing to do here
  }
  void visit(const CThisExpressionNode* node){
	  //and here
  }
  void visit(const CParenExpressionNode* node){
	  if (node->expr != NULL)
		  node->expr->accept(this);
	  
	  if ((lastTypeValue != "int") || (lastTypeValue != "bool"))
		  cout << "Expression in brackets is not valid" << endl;
  }
  void visit(const CInvokeMethodExpressionNode* node){
	  //сложновато
  }
  void visit(const CFewArgsExpressionNode* node){
	  if (node->expr != NULL)
		  node->expr->accept(this);
  }
  
  void visit(const CListExpressionNode* node){
	  if (node->prevExps != NULL)
		  node->prevExps->accept(this);
	  if (node->nextExp != NULL)
		  node->nextExp->accept(this);
  }
  void visit(const CLastListExpressionNode* node){
	  if (node->expr != NULL)
		  node->expr->accept(this);
  }
};
#endif
