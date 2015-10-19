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
		cout << "Program" << "\n\t";
		node->mainClass->accept(dynamic_cast<const CVisitor* const>(self));
		cout << "\n\t";
		node->decl->accept(dynamic_cast<const CVisitor* const>(self));
  	}

  	void visit(const CMainClassDeclarationRuleNode* node) const {
		const CPrintVisitor* const self = this;
		cout << "Main class(" << node->className << ")\n\t\t";
		node->stmt->accept(dynamic_cast<const CVisitor* const>(self));
  	}

  	void visit(const CDeclarationsListNode* node) const {
		const CPrintVisitor* const self = this;
		//cout << "DeclarationsNode" << "\n\t";
		node->decl->accept(dynamic_cast<const CVisitor* const>(self));
		node->cl->accept(dynamic_cast<const CVisitor* const>(self));
  	}

  	void visit(const CDeclarationsEmptyNode* node) const {
		//cout << "CDeclarationsEmptyNode\n\t";
  	}

  	void visit(const CClassDeclarationRuleNode* node) const {
		const CPrintVisitor* const self = this;
		cout << "Class(" << node->ident << ")\n\t\t";
		node->extDecl->accept(dynamic_cast<const CVisitor* const>(self));
		node->vars->accept(dynamic_cast<const CVisitor* const>(self));
		node->method->accept(dynamic_cast<const CVisitor* const>(self));
  	}

  	void visit(const CExtendDeclarationRuleNode* node) const {
		cout << "CExtendDeclarationRuleNode\n\t\t";
  	}

  	void visit(const CExtendDeclarationEmptyNode* node) const {
		//cout << "CExtendDeclarationEmptyNode\n\t\t";
  	}

  	void visit(const CVarDeclarationsListNode* node) const {
  		const CPrintVisitor* const self = this;
		//cout << "CVarDeclarationsListNode\n\t\t";
		node->list->accept(dynamic_cast<const CVisitor* const>(self));
		node->item->accept(dynamic_cast<const CVisitor* const>(self));
  	}

  	void visit(const CVarDeclarationsEmptyNode* node) const {
		//cout << "CVarDeclarationsEmptyNode\n\t";
  	}

  	void visit(const CMethodDeclarationsListNode* node) const {
  		const CPrintVisitor* const self = this;
		//cout << "CMethodDeclarationsListNode\n\t";
		node->list->accept(dynamic_cast<const CVisitor* const>(self));
		node->item->accept(dynamic_cast<const CVisitor* const>(self));
  	}

  	void visit(const CMethodDeclarationsEmptyNode* node) const {
		//cout << "CMethodDeclarationsEmptyNode\n\t";
 	}

  	void visit(const CVarDeclarationRuleNode* node) const {
  		const CPrintVisitor* const self = this;
  		node->type->accept(dynamic_cast<const CVisitor* const>(self));
		cout << "id(" << node->ident << ")\n\t\t";
  	}

  	void visit(const CMethodDeclarationRuleNode* node) const {
  		const CPrintVisitor* const self = this;
  		node->type->accept(dynamic_cast<const CVisitor* const>(self));
		cout << "function(" << node->ident << ")\n\t\t";
		node->param_arg->accept(dynamic_cast<const CVisitor* const>(self));
		cout << "\t\t";
		node->method_body->accept(dynamic_cast<const CVisitor* const>(self));
		node->return_exp->accept(dynamic_cast<const CVisitor* const>(self));
  	}

  	void visit(const CVarsDecListNode* node) const {
		cout << "CVarsDecListNode\n\t";
  	}

  	void visit(const CVarsDecFirstNode* node) const {
		cout << "CVarsDecFirstNode\n\t";
  	}

  	void visit(const CStatsFirstNode* node) const {
		cout << "CStatsFirstNode\n\t";
  	}

  	void visit(const CStatsListNode* node) const {
  		cout << "CStatsListNode\n\t";
  	}

  	void visit(const CMethodBodyVarsNode* node) const {
  		cout << "CMethodBodyVarsNode\n\t";
  	}

  	void visit(const CMethodBodyStatsNode* node) const {
  		cout << "CMethodBodyStatsNode\n\t";
  	}

  	void visit(const CMethodBodyAllNode* node) const {
  		cout << "CMethodBodyAllNode\n\t\t";
  	}

  	void visit(const CMethodBodyEmptyNode* node) const {
  		//cout << "CMethodBodyEmptyNode\n\t";
  	}

  	void visit(const CParamArgListNode* node) const {
  		const CPrintVisitor* const self = this;
  		//cout << "CParamArgListNode\n\t\t";
  		node->params->accept(dynamic_cast<const CVisitor* const>(self));
  	}

  	void visit(const CParamArgEmptyNode* node) const {
  		//cout << "CParamArgEmptyNode\n\t";
  	}

  	void visit(const CParamsOneNode* node) const {
  		const CPrintVisitor* const self = this;
  		//cout << "CParamsOneNode\n\t\t";
  		node->param->accept(dynamic_cast<const CVisitor* const>(self));
  	}

  	void visit(const CParamsTwoNode* node) const {
  		//cout << "CParamsTwoNode\n\t\t";
  	}

  	void visit(const CParamRuleNode* node) const {
  		const CPrintVisitor* const self = this;
  		node->type->accept(dynamic_cast<const CVisitor* const>(self));
  		cout << "param(" << node->ident << ")\n\t\t";
  		
  	}

  	void visit(const CTypeRuleNode* node) const {
  		const CPrintVisitor* const self = this;
  		cout << "type(" << node->type << ") ";
  	}

  	void visit(const CNumerousStatementsNode* node) const {
  		cout << "CNumerousStatementsNode\n\t";
  	}

  	void visit(const CEmptyStatementsNode* node) const {
  		//cout << "CEmptyStatementsNode\n\t";
  	}

  	void visit(const CBracedStatementNode* node) const {
  		cout << "CBracedStatementNode\n\t";
  	}

  	void visit(const CIfStatementNode* node) const {
		cout << "IF\n\t";
  	}

  	void visit(const CWhileStatementNode* node) const {
		cout << "WHILE\n\t";
  	}

  	void visit(const CPrintStatementNode* node) const {
  		const CPrintVisitor* const self = this;
		cout << "Print" << "\n\t\t";
		node->expression->accept(dynamic_cast<const CVisitor* const>(self));
  	}

  	void visit(const CAssignStatementNode* node) const {
  		cout << "=\n\t";
  	}
  	void visit(const CInvokeExpressionStatementNode* node) const {
  		cout << "CInvokeExpressionStatementNode\n\t";
  	}

  	void visit(const CInvokeExpressionNode* node) const {
  		cout << "CInvokeExpressionNode\n\t";
  	}
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

  	void visit(const CInvokeMethodExpressionNode* node) const {
  		cout << "CInvokeMethodExpressionNode\n\t";
  	}
  	void visit(const CFewArgsExpressionNode* node) const {}

  	void visit(const CEmptyArgsExpression* node) const {}
  	void visit(const CListExpressionNode* node) const {}
  	void visit(const CLastListExpressionNode* node) const {}
};

#endif
