#include "TypeChecker.h"

CTypeChecker::CTypeChecker(CStorage* _symbols, CTable& _table): symbolsStorage(_symbols), table(_table) {}

bool CTypeChecker::checkClassExistence(const CSymbol* name) {
	bool flag = false;
	int i = 0;
	while (!flag && (i < table.classInfo.size())) {
		flag = flag || (table.classInfo[i].name == name);
		i++;
	}
	return flag;
}

bool CTypeChecker::assignType(const CSymbol* name, const CSymbol*& type) {
	bool flagDec = false;
	for (int j = 0; j < table.classInfo[this->classPos].methods.size(); j++) {
		for (int i = 0; i < table.classInfo[this->classPos].methods[j].vars.size(); i++) {
			flagDec = flagDec || (name == table.classInfo[this->classPos].methods[j].vars[i].name);
			if (flagDec) {
				type = table.classInfo[this->classPos].methods[j].vars[i].type;
				return true;
			}
		}

		if (!flagDec) {
			for (int i = 0; i < table.classInfo[this->classPos].methods[j].params.size(); i++) {
				flagDec = flagDec || (name == table.classInfo[this->classPos].methods[j].params[i].name);
				if (flagDec) {
					type = table.classInfo[this->classPos].methods[j].params[i].type;
					return true;
				}
			}
		}

		if (!flagDec) {
			for (int i = 0; i < table.classInfo[this->classPos].vars.size(); i++) {
				flagDec = flagDec || (name == table.classInfo[this->classPos].vars[i].name);
				if (flagDec) {
					type = table.classInfo[this->classPos].vars[i].type;
					return true;
				}
			}
		}
	}

	const CSymbol* first = table.classInfo[this->classPos].name;
	const CSymbol* parent = table.classInfo[this->classPos].name;
	do{
		int i = 0;
		while ((table.classInfo[i].name != parent) && (i < table.classInfo.size()))
			i++;

		if (i == table.classInfo.size())
			parent = symbolsStorage->get("");
		else
			parent = table.classInfo[i].parent;

		if (parent != symbolsStorage->get(""))
			for (int k = 0; k < table.classInfo[i].vars.size(); k++){
				flagDec = flagDec || (name == table.classInfo[i].vars[k].name);
				if (flagDec){
					type = table.classInfo[i].vars[k].type;
					return true;
				}
			}

	} while ((parent != symbolsStorage->get("")) && (parent != first));
}

const CSymbol* CTypeChecker::checkAssignment(const CSymbol* name) {
	const CSymbol* type;

	bool declared = assignType(name, type);
	if (!declared) {
		cout << "Variable not declared " << name << endl;
		return symbolsStorage->get( "" );
	}

	if (type != lastTypeValue) {
		cout << "Cannot assign " << lastTypeValue << " to " << type << endl;
	}

	return type;
}

void CTypeChecker::Visit(const CProgramRuleNode* node) {
	if (node->mainClass != 0)
		node->mainClass->accept(this);

	if (node->decl != 0)
		node->decl->accept(this);
}

void CTypeChecker::Visit(const CMainClassDeclarationRuleNode* node) {
	if (node->stmt != 0)
		node->stmt->accept(this);
}
void CTypeChecker::Visit(const CDeclarationsListNode* node) {
	if (node->decl != 0)
		node->decl->accept(this);

	node->cl->accept(this);
}

void CTypeChecker::Visit(const CClassDeclarationRuleNode* node) {
	this->classPos++;
	if (node->extDecl != 0)
		node->extDecl->accept(this);

	if (node->vars != 0)
		node->vars->accept(this);

	if (node->method != 0)
		node->method->accept(this);
}

void CTypeChecker::Visit(const CExtendDeclarationRuleNode* node) {
	const CSymbol* parent = node->ident;
	do {
		int i = 0;
		while ((table.classInfo[i].name != parent) && (i < table.classInfo.size()))
			i++;

		if (i == table.classInfo.size())
			parent = symbolsStorage->get("");

		else
			parent = table.classInfo[i].parent;

	} while ((parent != symbolsStorage->get("")) && (parent != node->ident));

	if (parent != symbolsStorage->get(""))
		cout << "Cyclic inheritance with " << node->ident << endl;
}

void CTypeChecker::Visit(const CVarDeclarationsListNode* node) {
	if (node->list != 0)
		node->list->accept(this);

	if (node->item != 0)
		node->item->accept(this);
}

void CTypeChecker::Visit(const CMethodDeclarationsListNode* node) {
	if (node->list != 0) {
		node->list->accept(this);
	}
	node->item->accept(this);
}

void CTypeChecker::Visit(const CVarDeclarationRuleNode* node) {
	CTypeRuleNode* tmp = dynamic_cast<CTypeRuleNode*>(node->type.get());
	if ((tmp->type != symbolsStorage->get("boolean") ) && (tmp->type != symbolsStorage->get("int") && (tmp->type != symbolsStorage->get("int[]")))) {
		if (!checkClassExistence(tmp->type)) {
			cout << "No such type: " << tmp->type << endl;
		}
	}
	node->type->accept(this);
}

void CTypeChecker::Visit(const CMethodDeclarationRuleNode* node) {
	CTypeRuleNode* tmp = dynamic_cast<CTypeRuleNode*>(node->type.get());
	if ((tmp->type != symbolsStorage->get("boolean") ) && (tmp->type != symbolsStorage->get("int"))
		&& (tmp->type != symbolsStorage->get("int[]"))) {
		bool flag = false;
		for (int i = 0; i < table.classInfo.size(); i++) {
			flag = flag || (table.classInfo[i].name == tmp->type);
		}

		if (!flag)
			cout << "No such type: " << tmp->type << endl;
	}
	node->type->accept(this);
	if (node->param_arg != 0)
		node->param_arg->accept(this);
	if (node->method_body != 0)
		node->method_body->accept(this);
	if (node->return_exp != 0)
		node->return_exp->accept(this);

	for (int i = 0; i < table.classInfo.size(); i++)
		for (int j = 0; j < table.classInfo[i].methods.size(); j++) {
			if (table.classInfo[i].methods[j].name == node->ident) {
				if (table.classInfo[i].methods[j].returnType != lastTypeValue) {
					cout<< "Return types does not match" << endl;
				}
			}
		}
}

void CTypeChecker::Visit(const CVarsDecListNode* node) {
	if (node->list != 0)
		node->list->accept(this);

	if (node->next != 0)
		node->next->accept(this);
}

void CTypeChecker::Visit(const CVarsDecFirstNode* node) {
	if (node->first != 0)
		node->first->accept(this);
}

void CTypeChecker::Visit(const CStatsFirstNode* node) {
	if (node->stm != 0)
		node->stm->accept(this);
}

void CTypeChecker::Visit(const CStatsListNode* node) {
	if (node->list != 0)
		node->list->accept(this);
	if (node->stm != 0)
		node->stm->accept(this);
}

void CTypeChecker::Visit(const CMethodBodyVarsNode* node) {
	if (node->vars != 0)
		node->vars->accept(this);
}

void CTypeChecker::Visit(const CMethodBodyStatsNode* node) {
	if (node->stats != 0)
		node->stats->accept(this);
}

void CTypeChecker::Visit(const CMethodBodyAllNode* node) {
	if (node->vars != 0)
		node->vars->accept(this);

	if (node->stats != 0)
		node->stats->accept(this);
}

void CTypeChecker::Visit(const CParamArgListNode* node) {
	if (node->params != 0)
		node->params->accept(this);
}

void CTypeChecker::Visit(const CParamsOneNode* node) {
	if (node->param != 0)
		node->param->accept(this);
}

void CTypeChecker::Visit(const CParamsTwoNode* node) {
	if (node->first != 0)
		node->first->accept(this);
	if (node->second != 0)
		node->second->accept(this);
}

void CTypeChecker::Visit(const CParamRuleNode* node) {
	if (node->type != 0)
		node->type->accept(this);
}

void CTypeChecker::Visit(const CTypeRuleNode* node) {
	lastTypeValue = node->type;
}

void CTypeChecker::Visit(const CNumerousStatementsNode* node) {
	if (node->statements != 0)
		node->statements->accept(this);
	if (node->statement != 0)
		node->statement->accept(this);
}

void CTypeChecker::Visit(const CBracedStatementNode* node) {
	if (node->statements != 0)
		node->statements->accept(this);
}

void CTypeChecker::Visit(const CIfStatementNode* node) {
	node->expression->accept(this);
	if (lastTypeValue != symbolsStorage->get("boolean"))
		cout << "Error in if/else statement expression" << endl;
	node->thenStatement->accept(this);
	if (node->elseStatement != 0) {
		node->elseStatement->accept(this);
	}
}

void CTypeChecker::Visit(const CWhileStatementNode* node) {
	node->expression->accept(this);
	if (lastTypeValue != symbolsStorage->get("boolean"))
		cout << "Error in while statement expression" << lastTypeValue << endl;
	node->statement->accept(this);
}

void CTypeChecker::Visit(const CPrintStatementNode* node) {
	node->expression->accept(this);
	if (lastTypeValue != symbolsStorage->get("int"))
		cout << "Error in print expression" << endl;
}

void CTypeChecker::Visit(const CAssignStatementNode* node) {
	if (node->expression != 0)
		node->expression->accept(this);

	checkAssignment(node->identifier);
}

void CTypeChecker::Visit(const CInvokeExpressionStatementNode* node) {
	if (node->firstexpression != 0)
		node->firstexpression->accept(this);

	if (lastTypeValue != symbolsStorage->get("int"))
		cout << "Array index is not int in " << node->identifier << endl;

	if (node->secondexpression != 0)
		node->secondexpression->accept(this);

	const CSymbol* type;
	assignType(node->identifier, type);
	if (!((type == symbolsStorage->get("int[]")) && (lastTypeValue == symbolsStorage->get("int"))))
		cout << "Cannot assign " << lastTypeValue << " to " << type << endl;

}

void CTypeChecker::Visit(const CInvokeExpressionNode* node) {
	if (node->firstExp != 0)
		node->firstExp->accept(this);

	if (lastTypeValue != symbolsStorage->get("int[]"))
		cout << "Trying to access non-existent array" << endl;

	if (node->secondExp != 0)
		node->secondExp->accept(this);

	if (lastTypeValue != symbolsStorage->get("int"))
		cout << "Array index is not int" << endl;

	lastTypeValue = symbolsStorage->get("int");
}

void CTypeChecker::Visit(const CLengthExpressionNode* node) {

	if (node->expr != 0)
		node->expr->accept(this);
	lastTypeValue = symbolsStorage->get("int");
}

void CTypeChecker::Visit(const CArithmeticExpressionNode* node) {
	node->firstExp->accept(this);
	if ((lastTypeValue != symbolsStorage->get("int")) && (lastTypeValue != symbolsStorage->get("boolean")))
		cout << "Error in arithmetic expression" << endl;

	node->secondExp->accept(this);
	if ((lastTypeValue != symbolsStorage->get("int")) && (lastTypeValue != symbolsStorage->get("boolean")))
		cout << "Error in arithmetic expression" << endl;
}

void CTypeChecker::Visit(const CUnaryExpressionNode* node) {
	node->expr->accept(this);

	if (lastTypeValue != symbolsStorage->get("int"))
		cout << "Error in unary expression" << endl;

	lastTypeValue = symbolsStorage->get("int");
}

void CTypeChecker::Visit(const CCompareExpressionNode* node) {
	node->firstExp->accept(this);

	if (lastTypeValue != symbolsStorage->get("int"))
		cout << "Error in compare expression" << endl;

	node->secondExp->accept(this);
	if (lastTypeValue != symbolsStorage->get("int"))
		cout << "Error in compare expression" << endl;

	lastTypeValue = symbolsStorage->get("boolean");
}

void CTypeChecker::Visit(const CNotExpressionNode* node) {
	node->expr->accept(this);

	if (lastTypeValue != symbolsStorage->get("boolean"))
		cout << "Error in NOT expression" << endl;

	lastTypeValue = symbolsStorage->get("boolean");
}

void CTypeChecker::Visit(const CNewArrayExpressionNode* node) {
	if (node->expr != 0)
		node->expr->accept(this);

	if (lastTypeValue != symbolsStorage->get("int"))
		cout << "Array index is not int in new array" << lastTypeValue << endl;
	lastTypeValue = symbolsStorage->get("int[]");
}

void CTypeChecker::Visit(const CNewObjectExpressionNode* node) {
	if ((node->objType != symbolsStorage->get("int")) && (node->objType != symbolsStorage->get("boolean")))
	if (!checkClassExistence(node->objType))
		cout << "No such type: " << node->objType << endl;
	else
		lastTypeValue = node->objType;

}

void CTypeChecker::Visit(const CIntExpressionNode* node) {
	lastTypeValue = symbolsStorage->get("int");
}

void CTypeChecker::Visit(const CBooleanExpressionNode* node) {
	lastTypeValue = symbolsStorage->get("boolean");
}

void CTypeChecker::Visit(const CIdentExpressionNode* node) {
	const CSymbol* tmp;
	if (assignType(node->name, tmp)) {
		if (tmp != symbolsStorage->get("")) {
			lastTypeValue = tmp;
		}
	} else {
		cout << "Invaid ident " << node->name << endl;
	}
}

void CTypeChecker::Visit(const CThisExpressionNode* node) {
	lastTypeValue = table.classInfo[this->classPos].name;
}

void CTypeChecker::Visit(const CParenExpressionNode* node) {
	if (node->expr != 0)
		node->expr->accept(this);

	if ((lastTypeValue != symbolsStorage->get("int")) && (lastTypeValue != symbolsStorage->get("boolean")))
		cout << "Expression in brackets is not valid" << endl;
}
void CTypeChecker::Visit(const CInvokeMethodExpressionNode* node) {

	if (node->expr != 0)
		node->expr->accept(this);

	bool declaredClass = false;
	bool declaredMethod = false;
	const CSymbol* ret;
	for (int i = 0; i < table.classInfo.size(); i++) {
		declaredClass = declaredClass || (table.classInfo[i].name == lastTypeValue);
		if (declaredClass) {
			for (int j = 0; j < table.classInfo[i].methods.size(); j++) {
				declaredMethod = declaredMethod || (table.classInfo[i].methods[j].name == node->name);
				if (declaredMethod) {
					argNum = table.classInfo[i].methods[j].params.size();
					ret = table.classInfo[i].methods[j].returnType;
					break;
				}
			}
			if (!declaredMethod)
				cout << "Method not declared: " << node->name << endl;
			break;
		}
	}
	if (!declaredClass)
		cout << "Class not declared: " << lastTypeValue <<node->name << endl;

	if (node->args != 0)
		node->args->accept(this);
	else {
		if (argNum != 0)
			cout << "Arguments number in declaration and in usage does not match" << endl;
	}
	lastTypeValue = ret;
}
void CTypeChecker::Visit(const CFewArgsExpressionNode* node) {
	if (node->expr != 0)
		node->expr->accept(this);
}

void CTypeChecker::Visit(const CListExpressionNode* node) {
	argCount++;
	if (node->prevExps != 0)
		node->prevExps->accept(this);
	if (node->nextExp != 0)
		node->nextExp->accept(this);
}
void CTypeChecker::Visit(const CLastListExpressionNode* node) {
	argCount++;
	if (argCount != argNum)
		cout << "Arguments number in declaration and in usage does not match" << endl;
	argCount  = 0;
	if (node->expr != 0)
		node->expr->accept(this);
}