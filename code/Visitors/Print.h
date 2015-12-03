#ifndef PRINTVISITOR_H_INCLUDED
#define PRINTVISITOR_H_INCLUDED
#include "../common.h"
#include "../Structs/Ast.h"
#include "../Visitors/Visitor.h"

class CPrint : public CVisitor{
    int counter = 0;
    void print_tabs(int num){
      for (int i=0; i<num; i++)
        cout << "  ";
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
      cout << node->ident << endl;
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
      print_tabs(counter++);
    	node->type->accept(this);
  		cout << "id(" << node->ident << ")" << endl;
      --counter;
    }

    void visit(const CMethodDeclarationRuleNode* node){
      print_tabs(counter++);
    	node->type->accept(this);
  		cout << "function(" << node->ident << ")" << endl;
      if (node->param_arg != 0)
  		  node->param_arg->accept(this);
      if (node->method_body != 0)
  		  node->method_body->accept(this);
      print_tabs(counter++);
      cout<<"RETURN"<<endl;
  		node->return_exp->accept(this);
      --counter;
      --counter;
    }

    void visit(const CVarsDecListNode* node){
      if (node->list != 0)
        node->list->accept(this);
      if (node->next != 0)
        node->next->accept(this);
    }

    void visit(const CVarsDecFirstNode* node){
      if (node->first != 0)
        node->first->accept(this);
    }

    void visit(const CStatsFirstNode* node){
      if (node->stm != 0)
        node->stm->accept(this);
    }

    void visit(const CStatsListNode* node){
      if (node->list != 0)
        node->list->accept(this);
      if (node->stm != 0)
        node->stm->accept(this);
    }

  	void visit(const CMethodBodyVarsNode* node){
      if (node->vars != 0)
        node->vars->accept(this);
  	}

  	void visit(const CMethodBodyStatsNode* node){
      node->stats->accept(this);
  	}

  	void visit(const CMethodBodyAllNode* node){
      node->vars->accept(this);
      node->stats->accept(this);
  	}

  	void visit(const CParamArgListNode* node){
  		node->params->accept(this);
  	}

  	void visit(const CParamsOneNode* node){
      if (node->param != 0)
  		  node->param->accept(this);
  	}

  	void visit(const CParamsTwoNode* node){
      if (node->first != 0)
        node->first->accept(this);
      if (node->second != 0)
        node->second->accept(this);
  	}

  	void visit(const CParamRuleNode* node){
      print_tabs(counter++);
  		node->type->accept(this);
  		cout << "param(" << node->ident << ")" << endl;
      --counter;
  	}

  	void visit(const CTypeRuleNode* node){
  		cout << "type(" << node->type << ") ";
  	}

  	void visit(const CNumerousStatementsNode* node){
      if (node->statements != 0)
        node->statements->accept(this);
      node->statement->accept(this);
  	}

  	void visit(const CBracedStatementNode* node){
      if (node->statements != 0)
        node->statements->accept(this);
  	}

  	void visit(const CIfStatementNode* node){
      print_tabs(counter++);
		  cout << "IF" << endl;
      node->expression->accept(this);
      --counter;
      print_tabs(counter++);
      cout << "THEN" << endl;
      node->thenStatement->accept(this);
      --counter;
      if (node->elseStatement != 0){
        print_tabs(counter++);
        cout << "ELSE" << endl;
        node->elseStatement->accept(this);
        --counter;
      }
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
      cout << "id(" << node->identifier << ")";
  		cout << "'='"<<endl;
      node->expression->accept(this);
      --counter;
  	}
  	void visit(const CInvokeExpressionStatementNode* node){
      print_tabs(counter++);
      cout<<node->identifier<<endl;
      node->firstexpression->accept(this);
      node->secondexpression->accept(this);
      --counter;
  	}

  	void visit(const CInvokeExpressionNode* node){
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
      print_tabs(counter++);
  		cout << "function call(" << node->name << ")" << endl;
      node->expr->accept(this);
      if (node->args != 0)
        node->args->accept(this);
      --counter;
  	}

  	void visit(const CFewArgsExpressionNode* node){
      node->expr->accept(this);
    }

  	void visit(const CListExpressionNode* node){
      print_tabs(counter);
      cout << "CListExpressionNode" << endl;
      node->prevExps->accept(this);
      node->nextExp->accept(this);
    }

  	void visit(const CLastListExpressionNode* node){
      node->expr->accept(this);
    }
};

#endif
