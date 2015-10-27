#ifndef CPRINTVISITOR_H_INCLUDED
#define CPRINTVISITOR_H_INCLUDED
#include <iostream>
#include <typeinfo>
#include "CVisitor.h"
#include "ast.h"
using namespace std;

class CPrintVisitor : public CVisitor{
    int counter = 0;
    void print_tabs(int num){
      for (int i=0; i<num; i++)
        cout << "\t";
    }
public:
  	void visit(const CProgramRuleNode* node){
      print_tabs(counter++);
  		cout << "Program" << endl;
  		node->mainClass->accept(this);
      if (node->decl != 0)
  		  node->decl->accept(this);
      --counter;
    }

    void visit(const CMainClassDeclarationRuleNode* node){
      print_tabs(counter++);
  		cout << "Main class(" << node->className << ")" << endl;
  		node->stmt->accept(this);
      --counter;
    }

    void visit(const CDeclarationsListNode* node){
      if (node->decl != 0)
  		  node->decl->accept(this);
      if (node->cl != 0)
  		  node->cl->accept(this);
    }

    void visit(const CClassDeclarationRuleNode* node){
      print_tabs(counter++);
  		cout << "Class(" << node->ident << ")" << endl;
      if (node->extDecl != 0)
  		  node->extDecl->accept(this);
      if (node->vars != 0)
  		  node->vars->accept(this);
		  node->method->accept(this);
      --counter;
    }

    void visit(const CExtendDeclarationRuleNode* node){
  		cout << "CExtendDeclarationRuleNode" << endl;
    }

    void visit(const CVarDeclarationsListNode* node){
  		//cout << "CVarDeclarationsListNode\n\t\t";
      if (node->list != 0)
  		  node->list->accept(this);
		  node->item->accept(this);
    }


    void visit(const CMethodDeclarationsListNode* node){
  		//cout << "CMethodDeclarationsListNode\n\t";
      if (node->list != 0)
  		  node->list->accept(this);
  		node->item->accept(this);
    }


    void visit(const CVarDeclarationRuleNode* node){
    	node->type->accept(this);
      print_tabs(counter++);
  		cout << "id(" << node->ident << ")" << endl;
      --counter;
    }

    void visit(const CMethodDeclarationRuleNode* node){
    	node->type->accept(this);
  		cout << "function(" << node->ident << ")" << endl;
      counter++;
      if (node->param_arg != 0)
  		  node->param_arg->accept(this);
      if (node->method_body != 0)
  		  node->method_body->accept(this);
  		node->return_exp->accept(this);
      --counter;
    }

    void visit(const CVarsDecListNode* node){
      print_tabs(counter);
  		cout << typeid(*node).name() << endl;
    }

    void visit(const CVarsDecFirstNode* node){
      print_tabs(counter);
  		cout << typeid(*node).name() << endl;
    }

    void visit(const CStatsFirstNode* node){
      print_tabs(counter);
  		cout << typeid(*node).name() << endl;
    }

    void visit(const CStatsListNode* node){
      print_tabs(counter);
  		cout << typeid(*node).name() << endl;
    }

  	void visit(const CMethodBodyVarsNode* node){
      print_tabs(counter);
  		cout << typeid(*node).name() << endl;
  	}

  	void visit(const CMethodBodyStatsNode* node){
      print_tabs(counter);
      cout << typeid(*node).name() << endl;
  	}

  	void visit(const CMethodBodyAllNode* node){
      print_tabs(counter);
      cout << typeid(*node).name() << endl;
  	}

  	void visit(const CParamArgListNode* node){
  		node->params->accept(this);
  	}

  	void visit(const CParamsOneNode* node){
  		node->param->accept(this);
  	}

  	void visit(const CParamsTwoNode* node){
  	}

  	void visit(const CParamRuleNode* node){
  		node->type->accept(this);
  		cout << "param(" << node->ident << ")" << endl;
  	}

  	void visit(const CTypeRuleNode* node){
      print_tabs(counter);
  		cout << "type(" << node->type << ") ";
  	}

  	void visit(const CNumerousStatementsNode* node){
      print_tabs(counter);
  		cout << "CNumerousStatementsNode" << endl;
  	}

  	void visit(const CBracedStatementNode* node){
      print_tabs(counter);
  		cout << "CBracedStatementNode" << endl;
  	}

  	void visit(const CIfStatementNode* node){
      print_tabs(counter);
		  cout << "IF" << endl;
  	}

  	void visit(const CWhileStatementNode* node){
      print_tabs(counter);
		    cout << "WHILE" << endl;
  	}

  	void visit(const CPrintStatementNode* node){
      print_tabs(counter);
  		cout << "Print" << endl;
  		node->expression->accept(this);
  	}

  	void visit(const CAssignStatementNode* node){
      print_tabs(counter);
  		cout << "=" << endl;
  	}
  	void visit(const CInvokeExpressionStatementNode* node){
      print_tabs(counter);
  		cout << "CInvokeExpressionStatementNode" << endl;
  	}

  	void visit(const CInvokeExpressionNode* node){
      print_tabs(counter);
  		cout << "CInvokeExpressionNode" << endl;
  	}
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

  	void visit(const CInvokeMethodExpressionNode* node){
      print_tabs(counter);
  		cout << "CInvokeMethodExpressionNode" << endl;
  	}
  	void visit(const CFewArgsExpressionNode* node){}

  	void visit(const CListExpressionNode* node){}
  	void visit(const CLastListExpressionNode* node){}
};

#endif
