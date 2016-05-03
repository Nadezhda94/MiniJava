#include "Printer.h"

void CPrinter::printTabs( int num ) {
	for ( int i = 0; i < num; i++ ) {
		out << "  ";
	}
}

void CPrinter::Visit( const CProgramRuleNode* node ) {
	printTabs( counter++ );
	out << "Program" << endl;
	node->mainClass->accept( this );
	if ( node->decl != 0 ) {
		node->decl->accept( this );
	}
	--counter;
}

void CPrinter::Visit( const CMainClassDeclarationRuleNode* node ) {
	printTabs( counter++ );
	out << "Main class(" << node->className << ")" << endl;
	node->stmt->accept( this );
	--counter;
}

void CPrinter::Visit( const CDeclarationsListNode* node ) {
	if ( node->decl != 0 ) {
		node->decl->accept( this );
	}
	if ( node->cl != 0 ) {
		node->cl->accept( this );
	}
}

void CPrinter::Visit( const CClassDeclarationRuleNode* node ) {
	printTabs( counter++ );
	out << "Class(" << node->ident << ")" << endl;
	if ( node->extDecl != 0 ) {
		node->extDecl->accept( this );
	}
	if ( node->vars != 0 ) {
		node->vars->accept( this );
	}
	node->method->accept( this );
	--counter;
}

void CPrinter::Visit( const CExtendDeclarationRuleNode* node ) {
	out << "CExtendDeclarationRuleNode" << endl;
	out << node->ident << endl;
}

void CPrinter::Visit( const CVarDeclarationsListNode* node ) {
	//out << "CVarDeclarationsListNode\n\t\t";
	if ( node->list != 0 ) {
		node->list->accept( this );
	}
	node->item->accept( this );
}

void CPrinter::Visit( const CMethodDeclarationsListNode* node ) {
	//out << "CMethodDeclarationsListNode\n\t";
	if ( node->list != 0 ) {
		node->list->accept( this );
	}
	node->item->accept( this );
}

void CPrinter::Visit( const CVarDeclarationRuleNode* node ) {
	printTabs( counter++ );
	node->type->accept( this );
	out << "id(" << node->ident << ")" << endl;
	--counter;
}

void CPrinter::Visit( const CMethodDeclarationRuleNode* node ) {
	printTabs( counter++ );
	node->type->accept( this );
	out << "function(" << node->ident << ")" << endl;
	if ( node->param_arg != 0 ) {
		node->param_arg->accept( this );
	}
	if ( node->method_body != 0 ) {
		node->method_body->accept( this );
	}
	printTabs( counter++ );
	out << "RETURN" << endl;
	node->return_exp->accept( this );
	--counter;
	--counter;
}

void CPrinter::Visit( const CVarsDecListNode* node ) {
	if ( node->list != 0 ) {
		node->list->accept( this );
	}
	if ( node->next != 0 ) {
		node->next->accept( this );
	}
}

void CPrinter::Visit( const CVarsDecFirstNode* node ) {
	if ( node->first != 0 ) {
		node->first->accept( this );
	}
}

void CPrinter::Visit( const CStatsFirstNode* node ) {
	if ( node->stm != 0 ) {
		node->stm->accept( this );
	}
}

void CPrinter::Visit( const CStatsListNode* node ) {
	if ( node->list != 0 ) {
		node->list->accept( this );
	}
	if ( node->stm != 0 ) {
		node->stm->accept( this );
	}
}

void CPrinter::Visit( const CMethodBodyVarsNode* node ) {
	if ( node->vars != 0 ) {
		node->vars->accept( this );
	}
}

void CPrinter::Visit( const CMethodBodyStatsNode* node ) {
	node->stats->accept( this );
}

void CPrinter::Visit( const CMethodBodyAllNode* node ) {
	node->vars->accept( this );
	node->stats->accept( this );
}

void CPrinter::Visit( const CParamArgListNode* node ) {
	node->params->accept( this );
}

void CPrinter::Visit( const CParamsOneNode* node ) {
	if ( node->param != 0 ) {
		node->param->accept( this );
	}
}

void CPrinter::Visit( const CParamsTwoNode* node ) {
	if ( node->first != 0 ) {
		node->first->accept( this );
	}
	if ( node->second != 0 ) {
		node->second->accept( this );
	}
}

void CPrinter::Visit( const CParamRuleNode* node ) {
	printTabs( counter++ );
	node->type->accept( this );
	out << "param(" << node->ident << ")" << endl;
	--counter;
}

void CPrinter::Visit( const CTypeRuleNode* node ) {
	out << "type(" << node->type << ") ";
}

void CPrinter::Visit( const CNumerousStatementsNode* node ) {
	if ( node->statements != 0 ) {
		node->statements->accept( this );
	}
	node->statement->accept( this );
}

void CPrinter::Visit( const CBracedStatementNode* node ) {
	if ( node->statements != 0 ) {
		node->statements->accept( this );
	}
}

void CPrinter::Visit( const CIfStatementNode* node ) {
	printTabs( counter++ );
	out << "IF" << endl;
	node->expression->accept( this );
	--counter;
	printTabs( counter++ );
	out << "THEN" << endl;
	node->thenStatement->accept( this );
	--counter;
	if ( node->elseStatement != 0 ) {
		printTabs( counter++ );
		out << "ELSE" << endl;
		node->elseStatement->accept( this );
		--counter;
	}
}

void CPrinter::Visit( const CWhileStatementNode* node ) {
	printTabs( counter++ );
	out << "WHILE" << endl;
	node->expression->accept( this );
	node->statement->accept( this );
	--counter;
}

void CPrinter::Visit( const CPrintStatementNode* node ) {
	printTabs( counter++ );
	out << "PRINT" << endl;
	node->expression->accept( this );
	--counter;
}

void CPrinter::Visit( const CAssignStatementNode* node ) {
	printTabs( counter++ );
	out << "id(" << node->identifier << ")";
	out << "'='" << endl;
	node->expression->accept( this );
	--counter;
}

void CPrinter::Visit( const CInvokeExpressionStatementNode* node ) {
	printTabs( counter++ );
	out << node->identifier << endl;
	node->firstexpression->accept( this );
	node->secondexpression->accept( this );
	--counter;
}

void CPrinter::Visit( const CInvokeExpressionNode* node ) {
	node->firstExp->accept( this );
	node->secondExp->accept( this );
}

void CPrinter::Visit( const CLengthExpressionNode* node ) {
	printTabs( counter );
	out << "LENGTH" << endl;
	node->expr->accept( this );
}

void CPrinter::Visit( const CArithmeticExpressionNode* node ) {
	printTabs( counter++ );
	switch ( node->opType ) {
		case PLUS_OP:
			out << "'+'" << endl;
			break;
		case MINUS_OP:
			out << "'-'" << endl;
			break;
		case MULT_OP:
			out << "'*'" << endl;
			break;
		case DIV_OP:
			out << "'/'" << endl;
			break;
		case AND_OP:
			out << "'and'" << endl;
			break;
	}

	node->firstExp->accept( this );
	node->secondExp->accept( this );
	--counter;
}

void CPrinter::Visit( const CUnaryExpressionNode* node ) {
	printTabs( counter++ );
	switch ( node->op ) {
		case PLUS_OP:
			out << "'+'" << endl;
			break;
		case MINUS_OP:
			out << "'-'" << endl;
			break;
	}

	node->expr->accept( this );
	--counter;
}

void CPrinter::Visit( const CCompareExpressionNode* node ) {
	printTabs( counter++ );
	out << "'<'" << endl;
	node->firstExp->accept( this );
	node->secondExp->accept( this );
	--counter;
}

void CPrinter::Visit( const CNotExpressionNode* node ) {
	printTabs( counter++ );
	out << "'not'" << endl;
	node->expr->accept( this );
	--counter;
}

void CPrinter::Visit( const CNewArrayExpressionNode* node ) {
	printTabs( counter++ );
	out << "NEWARRAY" << endl;
	node->expr->accept( this );
	--counter;
}

void CPrinter::Visit( const CNewObjectExpressionNode* node ) {
	printTabs( counter );
	out << "NewObject(" << node->objType << ")" << endl;
}

void CPrinter::Visit( const CIntExpressionNode* node ) {
	printTabs( counter++ );
	out << "int(" << node->value << ")" << endl;
	--counter;
}

void CPrinter::Visit( const CBooleanExpressionNode* node ) {
	printTabs( counter++ );
	out << "bool(" << node->value << ")" << endl;
	--counter;
}

void CPrinter::Visit( const CIdentExpressionNode* node ) {
	printTabs( counter++ );
	out << "id(" << node->name << ")" << endl;
	--counter;
}

void CPrinter::Visit( const CThisExpressionNode* node ) {
	printTabs( counter );
	out << "THIS " << node->name << endl;
}

void CPrinter::Visit( const CParenExpressionNode* node ) {
	printTabs( counter++ );
	out << "'()'" << endl;
	node->expr->accept( this );
	--counter;
}

void CPrinter::Visit( const CInvokeMethodExpressionNode* node ) {
	printTabs( counter++ );
	out << "function call(" << node->name << ")" << endl;
	node->expr->accept( this );
	if ( node->args != 0 ) {
		node->args->accept( this );
	}
	--counter;
}

void CPrinter::Visit( const CFewArgsExpressionNode* node ) {
	node->expr->accept( this );
}

void CPrinter::Visit( const CListExpressionNode* node ) {
	printTabs( counter );
	out << "CListExpressionNode" << endl;
	node->prevExps->accept( this );
	node->nextExp->accept( this );
}

void CPrinter::Visit( const CLastListExpressionNode* node ) {
	node->expr->accept( this );
}