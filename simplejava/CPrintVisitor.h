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
  		node->decl->accept(this);
  		node->cl->accept(this);
    }

    void visit(const CDeclarationsEmptyNode* node){
  		//cout << "CDeclarationsEmptyNode\n\t";
    }

    void visit(const CClassDeclarationRuleNode* node){
      print_tabs(counter++);
  		cout << "Class(" << node->ident << ")" << endl;
  		node->extDecl->accept(this);
  		node->vars->accept(this);
  		node->method->accept(this);
      --counter;
    }

    void visit(const CExtendDeclarationRuleNode* node){
  		cout << "CExtendDeclarationRuleNode" << endl;
      cout << node->ident << endl;
    }

    void visit(const CExtendDeclarationEmptyNode* node){
  		//cout << "CExtendDeclarationEmptyNode\n\t\t";
    }

    void visit(const CVarDeclarationsListNode* node){
  		//cout << "CVarDeclarationsListNode\n\t\t";
  		node->list->accept(this);
  		node->item->accept(this);
    }

    void visit(const CVarDeclarationsEmptyNode* node){
  		//cout << "CVarDeclarationsEmptyNode\n\t";
    }

    void visit(const CMethodDeclarationsListNode* node){
  		//cout << "CMethodDeclarationsListNode\n\t";
  		node->list->accept(this);
  		node->item->accept(this);
    }

    void visit(const CMethodDeclarationsEmptyNode* node){
  		//cout << "CMethodDeclarationsEmptyNode\n\t";
   	}

    void visit(const CVarDeclarationRuleNode* node){
    	node->type->accept(this);
  		cout << "id(" << node->ident << ")" << endl;
    }

    void visit(const CMethodDeclarationRuleNode* node){
      node->type->accept(this);
  		cout << "function(" << node->ident << ")" << endl;
  		node->param_arg->accept(this);
      print_tabs(counter++);
  		node->method_body->accept(this);
  		//cout << "return ";
      node->return_exp->accept(this);
      --counter;
    }

    void visit(const CVarsDecListNode* node){
      //print_tabs(counter);
  		//cout << "CVarsDecListNode" << endl;
      node->list->accept(this);
      node->next->accept(this);
    }

    void visit(const CVarsDecFirstNode* node){
      //print_tabs(counter);
  		//cout << typeid(*node).name() << endl;
      node->first->accept(this);
    }

    void visit(const CStatsFirstNode* node){
      print_tabs(counter);
  		//cout << typeid(*node).name() << endl;
      node->stm->accept(this);
    }

    void visit(const CStatsListNode* node){
      //print_tabs(counter);
  		//cout << "CStatsListNode" << endl;
      node->list->accept(this);
      node->stm->accept(this);
    }

  	void visit(const CMethodBodyVarsNode* node){
      //print_tabs(counter);
  		//cout << "CMethodBodyVarsNode" << endl;
      node->vars->accept(this);
  	}

  	void visit(const CMethodBodyStatsNode* node){
      //print_tabs(counter);
      //cout << "CMethodBodyStatsNode" << endl;
      node->stats->accept(this);
  	}

  	void visit(const CMethodBodyAllNode* node){
      //print_tabs(counter);
      //cout << typeid(*node).name() << endl;
      node->vars->accept(this);
      node->stats->accept(this);
  	}

  	void visit(const CMethodBodyEmptyNode* node){
  	}

  	void visit(const CParamArgListNode* node){
  		node->params->accept(this);
  	}

  	void visit(const CParamArgEmptyNode* node){
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
  		cout << "param(" << node->ident << ")" << endl;
  	}

  	void visit(const CTypeRuleNode* node){
      print_tabs(counter);
      //cout << counter;
  		cout << "type(" << node->type << ") ";
  	}

  	void visit(const CNumerousStatementsNode* node){
      //print_tabs(counter);
  		//cout << "CNumerousStatementsNode" << endl;
      node->statements->accept(this);
      node->statement->accept(this);
  	}

  	void visit(const CEmptyStatementsNode* node){
  		//cout << "CEmptyStatementsNode\n\t";
  	}

  	void visit(const CBracedStatementNode* node){
      //print_tabs(counter++);
  		//cout << "CBracedStatementNode" << endl;
      node->statements->accept(this);
      //--counter;
  	}

  	void visit(const CIfStatementNode* node){
      print_tabs(counter++);
		  cout << "IF" << endl;
      node->expression->accept(this);
      node->thenStatement->accept(this);
      node->elseStatement->accept(this);
      --counter;
  	}

  	void visit(const CWhileStatementNode* node){
        print_tabs(counter++);
		    cout << "WHILE" << endl;
        node->expression->accept(this);
        node->statement->accept(this);
        --counter;
  	}

  	void visit(const CPrintStatementNode* node){
      print_tabs(counter++);
  		cout << "PRINT" << endl;
  		node->expression->accept(this);
      --counter;
  	}

  	void visit(const CAssignStatementNode* node){
      print_tabs(counter++);
  		cout << "'='" << endl;
      print_tabs(counter);
      cout << "id(" << node->identifier << ")" <<endl;
      node->expression->accept(this);
      --counter;
  	}
  	void visit(const CInvokeExpressionStatementNode* node){
      //print_tabs(counter++);
  		//cout << node->identifier << "[" << endl;
      node->firstexpression->accept(this);
      node->secondexpression->accept(this);
      //--counter;
  	}

  	void visit(const CInvokeExpressionNode* node){
      //print_tabs(counter);
  		//cout << "CInvokeExpressionNode" << endl;
      node->firstExp->accept(this);
      node->secondExp->accept(this);
  	}

  	void visit(const CLengthExpressionNode* node){
      print_tabs(counter);
      cout << "LENGTH" << endl;
      node->expr->accept(this);
    }

  	void visit(const CArithmeticExpressionNode* node){
      print_tabs(counter++);
      switch(node->opType){
        case PLUS_OP:
          cout << "'+'" << endl;
          break;
        case MINUS_OP:
          cout << "'-'" << endl;
          break;
        case MULT_OP:
          cout << "'*'" << endl;
          break;
        case DIV_OP:
          cout << "'/'" << endl;
          break;
        case AND_OP:
          cout << "'and'" << endl;
          break;
      }
      
      node->firstExp->accept(this);
      node->secondExp->accept(this);
      --counter;
    }

  	void visit(const CUnaryExpressionNode* node){
      print_tabs(counter++);
      switch(node->op) {
        case UPLUS_OP:
          cout << "'+'" << endl;
          break;
        case UMINUS_OP:
          cout << "'-'" << endl;
          break;
      }
      
      node->expr->accept(this);
      --counter;
    }

  	void visit(const CCompareExpressionNode* node){
      print_tabs(counter++);
      cout << "'<'" << endl;
      node->firstExp->accept(this);
      node->secondExp->accept(this);
      --counter;
    }

  	void visit(const CNotExpressionNode* node){
      print_tabs(counter++);
      cout << "'not'" << endl;
      node->expr->accept(this);
      --counter;
    }

  	void visit(const CNewArrayExpressionNode* node){
      print_tabs(counter++);
      cout << "NEWARRAY" << endl;
      node->expr->accept(this);
      --counter;
    }

  	void visit(const CNewObjectExpressionNode* node){
      print_tabs(counter);
      cout << "NewObject(" << node->objType << ")" << endl;
    }

  	void visit(const CIntExpressionNode* node){
      print_tabs(counter++);
      cout << "int(" << node->value << ")" << endl;
      --counter;
    }

  	void visit(const CBooleanExpressionNode* node){
      print_tabs(counter++);
      cout << "bool(" << node->value << ")" << endl;
      --counter;
    }

  	void visit(const CIdentExpressionNode* node){
      print_tabs(counter++);
      cout << "id(" << node->name << ")" << endl;
      --counter;
    }

  	void visit(const CThisExpressionNode* node){
      print_tabs(counter);
      cout << "THIS " << node->name << endl;
    }

  	void visit(const CParenExpressionNode* node){
      print_tabs(counter++);
      cout << "'()'" << endl;
      node->expr->accept(this);
      --counter;
    }

  	void visit(const CInvokeMethodExpressionNode* node){
      print_tabs(counter);
  		cout << "function call(" << node->name << ")" << endl;
      node->expr->accept(this);
      node->args->accept(this);
  	}

  	void visit(const CFewArgsExpressionNode* node){
      //print_tabs(counter);
      //cout << "CFewArgsExpressionNode" << endl;
      node->expr->accept(this);
    }

  	void visit(const CEmptyArgsExpression* node){

    }

  	void visit(const CListExpressionNode* node){
      print_tabs(counter);
      cout << "CListExpressionNode" << endl;
      node->prevExps->accept(this);
      node->nextExp->accept(this);
    }

  	void visit(const CLastListExpressionNode* node){
      //print_tabs(counter);
      // cout << "CLastListExpressionNode" << endl;
      node->expr->accept(this);
    }
};

#endif
