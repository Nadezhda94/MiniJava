#include "SymbolTableBuilder.h"

template<typename T>
void CSymbolTableBuilder::checkItemAlreadyExists( const std::vector <T> &items, const CSymbol* itemName ) {
	typename std::vector<T>::const_iterator it = items.cbegin();
	typename std::vector<T>::const_iterator itEnd = items.cend();
	for ( ; it != itEnd; ++it ) {
		if ( it->name == itemName ) {
			std::cerr << "redefinition: " << itemName << std::endl;
		}
	}
}

CSymbolTableBuilder::CSymbolTableBuilder(CStorage* _symbolsStorage):
	symbolsStorage(_symbolsStorage), inMethod( 0 )
{
	symbolsStorage->get( "this" );
}

void CSymbolTableBuilder::Visit( const CProgramRuleNode* node ) {
	node->mainClass->accept( this );
	if ( node->decl != 0 ) {
		node->decl->accept( this );
	}
}

void CSymbolTableBuilder::Visit( const CMainClassDeclarationRuleNode* node ) {
	inMethod = 0;
	checkItemAlreadyExists<CClassInfo>( table.classInfo, node->className );

	table.classInfo.push_back( CClassInfo( node->className ));
	CMethodInfo mainMthd( symbolsStorage->get( "main" ), symbolsStorage->get( "void" ));
	mainMthd.params.push_back( CVarInfo( node->argNames, symbolsStorage->get( "String[]" )));

	table.classInfo.back().methods.push_back( mainMthd );
	if ( node->stmt != 0 ) {
		node->stmt->accept( this );
	}
}

void CSymbolTableBuilder::Visit( const CDeclarationsListNode* node ) {
	if ( node->decl != 0 ) {
		node->decl->accept( this );
	}
	node->cl->accept( this );
}

void CSymbolTableBuilder::Visit( const CClassDeclarationRuleNode* node ) {
	inMethod = 0;
	checkItemAlreadyExists<CClassInfo>( table.classInfo, node->ident );
	table.classInfo.push_back( CClassInfo( node->ident ));
	if ( node->extDecl != 0 ) {
		node->extDecl->accept( this );
	}
	if ( node->method != 0 ) {
		node->method->accept( this );
	}
	if ( node->vars != 0 ) {
		node->vars->accept( this );
	}
}

void CSymbolTableBuilder::Visit( const CExtendDeclarationRuleNode* node ) {
	table.classInfo.back().parent = node->ident;
}

void CSymbolTableBuilder::Visit( const CVarDeclarationsListNode* node ) {
	if ( node->list != 0 ) {
		node->list->accept( this );
	}
	node->item->accept( this );
}

void CSymbolTableBuilder::Visit( const CMethodDeclarationsListNode* node ) {
	if ( node->list != 0 ) {
		node->list->accept( this );
	}
	node->item->accept( this );
}

void CSymbolTableBuilder::Visit( const CVarDeclarationRuleNode* node ) {
	node->type->accept( this );
	if ( inMethod ) {
		checkItemAlreadyExists<CVarInfo>( table.classInfo.back().methods.back().vars, node->ident );
		table.classInfo.back().methods.back().vars.push_back( CVarInfo( node->ident, lastTypeValue ));
	} else {
		checkItemAlreadyExists<CVarInfo>( table.classInfo.back().vars, node->ident );
		table.classInfo.back().vars.push_back( CVarInfo( node->ident, lastTypeValue ));
	}
}

void CSymbolTableBuilder::Visit( const CMethodDeclarationRuleNode* node ) {
	node->type->accept( this );
	checkItemAlreadyExists<CMethodInfo>( table.classInfo.back().methods, node->ident );
	table.classInfo.back().methods.push_back( CMethodInfo( node->ident, lastTypeValue ));
	if ( node->param_arg != 0 ) {
		node->param_arg->accept( this );
	}
	if ( node->method_body != 0 ) {
		inMethod = 1;
		node->method_body->accept( this );
		inMethod = 0;
	}
}

void CSymbolTableBuilder::Visit( const CVarsDecListNode* node ) {

	node->list->accept( this );
	node->next->accept( this );

}

void CSymbolTableBuilder::Visit( const CVarsDecFirstNode* node ) {
	node->first->accept( this );
}

void CSymbolTableBuilder::Visit( const CStatsFirstNode* node ) {

}

void CSymbolTableBuilder::Visit( const CStatsListNode* node ) { }

void CSymbolTableBuilder::Visit( const CMethodBodyVarsNode* node ) {
	node->vars->accept( this );
}

void CSymbolTableBuilder::Visit( const CMethodBodyStatsNode* node ) {

}

void CSymbolTableBuilder::Visit( const CMethodBodyAllNode* node ) {
	node->vars->accept( this );
}

void CSymbolTableBuilder::Visit( const CParamArgListNode* node ) {
	if ( node->params != 0 ) {
		node->params->accept( this );
	}
}

void CSymbolTableBuilder::Visit( const CParamsOneNode* node ) {
	node->param->accept( this );
}

void CSymbolTableBuilder::Visit( const CParamsTwoNode* node ) {
	node->first->accept( this );
	node->second->accept( this );
}

void CSymbolTableBuilder::Visit( const CParamRuleNode* node ) {
	node->type->accept( this );
	checkItemAlreadyExists<CVarInfo>( table.classInfo.back().methods.back().params, node->ident );
	table.classInfo.back().methods.back().params.push_back( CVarInfo( node->ident, lastTypeValue ));
}

void CSymbolTableBuilder::Visit( const CTypeRuleNode* node ) {
	lastTypeValue = node->type;
}

void CSymbolTableBuilder::Visit( const CNumerousStatementsNode* node ){}
void CSymbolTableBuilder::Visit( const CBracedStatementNode* node ){}
void CSymbolTableBuilder::Visit( const CIfStatementNode* node ){}
void CSymbolTableBuilder::Visit( const CWhileStatementNode* node ){}
void CSymbolTableBuilder::Visit( const CPrintStatementNode* node ){}
void CSymbolTableBuilder::Visit( const CAssignStatementNode* node ){}
void CSymbolTableBuilder::Visit( const CInvokeExpressionStatementNode* node ){}
void CSymbolTableBuilder::Visit( const CInvokeExpressionNode* node ){}
void CSymbolTableBuilder::Visit( const CLengthExpressionNode* node ){}
void CSymbolTableBuilder::Visit( const CArithmeticExpressionNode* node ){}
void CSymbolTableBuilder::Visit( const CUnaryExpressionNode* node ){}
void CSymbolTableBuilder::Visit( const CCompareExpressionNode* node ){}
void CSymbolTableBuilder::Visit( const CNotExpressionNode* node ){}
void CSymbolTableBuilder::Visit( const CNewArrayExpressionNode* node ){}
void CSymbolTableBuilder::Visit( const CNewObjectExpressionNode* node ){}
void CSymbolTableBuilder::Visit( const CIntExpressionNode* node ){}
void CSymbolTableBuilder::Visit( const CBooleanExpressionNode* node ){}
void CSymbolTableBuilder::Visit( const CIdentExpressionNode* node ){}
void CSymbolTableBuilder::Visit( const CThisExpressionNode* node ){}
void CSymbolTableBuilder::Visit( const CParenExpressionNode* node ){}
void CSymbolTableBuilder::Visit( const CInvokeMethodExpressionNode* node ){}
void CSymbolTableBuilder::Visit( const CFewArgsExpressionNode* node ){}
void CSymbolTableBuilder::Visit( const CListExpressionNode* node ){}
void CSymbolTableBuilder::Visit( const CLastListExpressionNode* node ){}


void testBuilder(ostream& out, CSymbolTableBuilder& table_vis) {
	auto itClass = table_vis.table.classInfo.begin();
	auto itClassEnd = table_vis.table.classInfo.end();
	out << "____________TABLE_BUILDER__________" << std::endl;
	for (; itClass != itClassEnd; ++itClass) {
		out << itClass -> name << std::endl;
		auto itVarsClass = itClass->vars.begin();
		auto itVarsClassEnd = itClass->vars.end();
		out<< "\tvars:" << std::endl;
		for (; itVarsClass != itVarsClassEnd; ++itVarsClass) {
			out << "\t\t" << itVarsClass->name << " " << itVarsClass->type << std::endl;
		}
		auto itMthd = itClass->methods.begin();
		auto itMthdEnd = itClass->methods.end();
		out << "\tmethods:" <<std::endl;
		for (; itMthd != itMthdEnd; ++itMthd) {
			out << "\t\t" << itMthd->name << std::endl;
			out << "\t\t" << "params:" << std::endl;
			auto itParams = itMthd->params.begin();
			auto itParamsEnd = itMthd->params.end();
			for (; itParams != itParamsEnd; ++itParams) {
				out << "\t\t\t" << itParams->name << " " << itParams->type << std::endl;
			}
			out << "\t\tvars:" << std::endl;
			auto itVars = itMthd->vars.begin();
			auto itVarsEnd = itMthd->vars.end();
			for (; itVars != itVarsEnd; ++itVars) {
				out << "\t\t\t" << itVars->name << " " << itVars->type << std::endl;
			}
			out << "\t\treturnType: " << itMthd->returnType << std::endl;
			out << "____________________________" << std::endl;

		}
	}
}