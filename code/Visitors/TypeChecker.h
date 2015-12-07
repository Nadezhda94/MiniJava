#ifndef TYPECHECKER_H_INCLUDED
#define TYPECHECKER_H_INCLUDED
#include "../common.h"
#include "../Visitors/Visitor.h"
#include "../Structs/Ast.h"
#include "../Structs/SymbolsTable.h"
using namespace SymbolsTable;
using namespace Symbol;

class CTypeChecker : public CVisitor{
public:
	CTable table;
	const CSymbol* lastTypeValue;
	CStorage* symbolsStorage;
	int classPos = 0;
	int methodPos = -1;

	bool inMethod;
	CTypeChecker(CStorage* _symbols, CTable& _table): symbolsStorage(_symbols), table(_table){}

	bool checkClassExistence(const CSymbol* name){
		bool flag = false;
		int i = 0;
		while (!flag && (i < table.classInfo.size())){
			flag = flag || (table.classInfo[i].name == name);
			i++;
		}

		if (!flag)
			return false;
		else
			return true;

	}

	bool assignType(const CSymbol* name, const CSymbol*& type){
		bool flagDec = false;
		for (int j = 0; j < table.classInfo[this->classPos].methods.size(); j++){
			for (int i = 0; i < table.classInfo[this->classPos].methods[j].vars.size(); i++){
				flagDec = flagDec || (name == table.classInfo[this->classPos].methods[j].vars[i].name);
				if (flagDec)
				{
					type = table.classInfo[this->classPos].methods[j].vars[i].type;
					return true;
				}
			}

			if (!flagDec){
				for (int i = 0; i < table.classInfo[this->classPos].methods[j].params.size(); i++){
				 	flagDec = flagDec || (name == table.classInfo[this->classPos].methods[j].params[i].name);
					if (flagDec){
						type = table.classInfo[this->classPos].methods[j].params[i].type;
						return true;
					}
				}
			}

			if (!flagDec){
				for (int i = 0; i < table.classInfo[this->classPos].vars.size(); i++){
					flagDec = flagDec || (name == table.classInfo[this->classPos].vars[i].name);
					if (flagDec){
						type = table.classInfo[this->classPos].vars[i].type;
						return true;
					}
				}
			}
		}
	}

	const CSymbol* checkAssignment(const CSymbol* name){
		const CSymbol* type;
		bool declared = assignType(name, type);


		if (!declared){
			cout << "Variable not declared " << name << endl;
			return symbolsStorage->get( "" );
		}

		if (type != lastTypeValue) {
			cout << "Cannot assign " << lastTypeValue << " to " << type << endl;
	}

		return type;
	}

	void visit(const CProgramRuleNode* node){
		if (node->mainClass != 0)
		node->mainClass->accept(this);

		if (node->decl != 0)
		node->decl->accept(this);
	}

	void visit(const CMainClassDeclarationRuleNode* node){
		if (node->stmt != 0)
		node->stmt->accept(this);
	}
	void visit(const CDeclarationsListNode* node){
		if (node->decl != 0)
			node->decl->accept(this);

		node->cl->accept(this);
	}

	void visit(const CClassDeclarationRuleNode* node){
		this->classPos++;
		if (node->extDecl != 0)
			node->extDecl->accept(this);

		if (node->vars != 0)
			node->vars->accept(this);

		if (node->method != 0)
			node->method->accept(this);
	}

	void visit(const CExtendDeclarationRuleNode* node){
		const CSymbol* parent = node->ident;
		do{
			int i = 0;
			while ((table.classInfo[i].name != parent) && (i < table.classInfo.size()))
				i++;

			if (i == table.classInfo.size())
				parent = symbolsStorage->get("");
			else
				parent = table.classInfo[i].parent;
		} while ((parent != symbolsStorage->get("")) || (parent != node->ident));

		if (parent != symbolsStorage->get(""))
			cout << "Cyclic inheritance with " << node->ident << endl;
	}

	void visit(const CVarDeclarationsListNode* node){
		if (node->list != 0)
			node->list->accept(this);

		if (node->item != 0)
			node->item->accept(this);
	}

	void visit(const CMethodDeclarationsListNode* node){
		if (node->list != 0) {
			node->list->accept(this);
			}
		node->item->accept(this);
	}

	void visit(const CVarDeclarationRuleNode* node){
		CTypeRuleNode* tmp = dynamic_cast<CTypeRuleNode*>(node->type);
		if ((tmp->type != symbolsStorage->get("bool") ) && (tmp->type != symbolsStorage->get("int") && (tmp->type != symbolsStorage->get("int[]")))) {
			if (!checkClassExistence(tmp->type)) {
				cout << "No such type: " << tmp->type << endl;
			}
		}
	}

	void visit(const CMethodDeclarationRuleNode* node){
		this->methodPos++;
		CTypeRuleNode* tmp = dynamic_cast<CTypeRuleNode*>(node->type);
		if ((tmp->type != symbolsStorage->get("bool") ) && (tmp->type != symbolsStorage->get("int"))
		&& (tmp->type != symbolsStorage->get("int[]"))) {
			bool flag = false;
			for (int i = 0; i < table.classInfo.size(); i++){
				flag = flag || (table.classInfo[i].name == tmp->type);
			}

			if (!flag)
			cout << "No such type: " << tmp->type << endl;
		}
		if (node->param_arg != 0)
			node->param_arg->accept(this);
		if (node->method_body != 0)
			node->method_body->accept(this);
		if (node->return_exp != 0)
			node->return_exp->accept(this);
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
		if (node->stats != 0)
			node->stats->accept(this);
	}

	void visit(const CMethodBodyAllNode* node){
		if (node->vars != 0)
			node->vars->accept(this);

		if (node->stats != 0)
			node->stats->accept(this);
	}

	void visit(const CParamArgListNode* node){
		if (node->params != 0)
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
		if (node->type != 0)
			node->type->accept(this);
	}

	void visit(const CTypeRuleNode* node){
		lastTypeValue = node->type;
	}

	void visit(const CNumerousStatementsNode* node){
		if (node->statements != 0)
			node->statements->accept(this);
		if (node->statement != 0)
			node->statement->accept(this);
	}

	void visit(const CBracedStatementNode* node){
		if (node->statements != 0)
			node->statements->accept(this);
	}

	void visit(const CIfStatementNode* node){
		node->expression->accept(this);
		if (lastTypeValue != symbolsStorage->get("bool"))
			cout << "Error in if/else statement expression" << endl;
	}

	void visit(const CWhileStatementNode* node){
		node->expression->accept(this);
		if (lastTypeValue != symbolsStorage->get("bool"))
			cout << "Error in while statement expression" << lastTypeValue << endl;
	}

	void visit(const CPrintStatementNode* node){
		node->expression->accept(this);
		if (lastTypeValue != symbolsStorage->get("int"))
			cout << "Error in print expression" << endl;
	}

	void visit(const CAssignStatementNode* node){
	if (node->expression != 0)
		node->expression->accept(this);

		checkAssignment(node->identifier);
	}

	void visit(const CInvokeExpressionStatementNode* node){
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

	void visit(const CInvokeExpressionNode* node){
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

	void visit(const CLengthExpressionNode* node){

	if (node->expr != 0)
		node->expr->accept(this);
		lastTypeValue = symbolsStorage->get("int");
	}

	void visit(const CArithmeticExpressionNode* node){
		node->firstExp->accept(this);
		if ((lastTypeValue != symbolsStorage->get("int")) && (lastTypeValue != symbolsStorage->get("bool")))
			cout << "Error in arithmetic expression" << endl;

		node->secondExp->accept(this);
		if ((lastTypeValue != symbolsStorage->get("int")) && (lastTypeValue != symbolsStorage->get("bool")))
			cout << "Error in arithmetic expression" << endl;
	}

	void visit(const CUnaryExpressionNode* node){
		node->expr->accept(this);

		if (lastTypeValue != symbolsStorage->get("int"))
			cout << "Error in unary expression" << endl;

		lastTypeValue = symbolsStorage->get("int");
	}

	void visit(const CCompareExpressionNode* node){
		node->firstExp->accept(this);

		if (lastTypeValue != symbolsStorage->get("int"))
			cout << "Error in compare expression" << endl;

		node->secondExp->accept(this);
		if (lastTypeValue != symbolsStorage->get("int"))
			cout << "Error in compare expression" << endl;

		lastTypeValue = symbolsStorage->get("bool");
	}

	void visit(const CNotExpressionNode* node){
		node->expr->accept(this);

		if (lastTypeValue != symbolsStorage->get("bool"))
			cout << "Error in NOT expression" << endl;

		lastTypeValue = symbolsStorage->get("bool");
	}

	void visit(const CNewArrayExpressionNode* node){
		if (node->expr != 0)
			node->expr->accept(this);

		if (lastTypeValue != symbolsStorage->get("int"))
			cout << "Array index is not int in new array" << lastTypeValue << endl;
		lastTypeValue = symbolsStorage->get("int[]");
	}

	void visit(const CNewObjectExpressionNode* node){
		if ((node->objType != symbolsStorage->get("int")) && (node->objType != symbolsStorage->get("bool")))
			if (!checkClassExistence(node->objType))
				cout << "No such type: " << node->objType << endl;
			else
				lastTypeValue = node->objType;

	}

	void visit(const CIntExpressionNode* node){
		lastTypeValue = symbolsStorage->get("int");
	}

	void visit(const CBooleanExpressionNode* node){
		lastTypeValue = symbolsStorage->get("bool");
	}

	void visit(const CIdentExpressionNode* node){
		const CSymbol* tmp;
		assignType(node->name, tmp);
		if (tmp != symbolsStorage->get("")) {
			lastTypeValue = tmp;
		}
	}

	void visit(const CThisExpressionNode* node){
		lastTypeValue = table.classInfo[this->classPos].name;
	}

	void visit(const CParenExpressionNode* node){
		if (node->expr != 0)
			node->expr->accept(this);

		if ((lastTypeValue != symbolsStorage->get("int")) && (lastTypeValue != symbolsStorage->get("bool")))
			cout << "Expression in brackets is not valid" << endl;
	}
	void visit(const CInvokeMethodExpressionNode* node){
		if (node->args != 0)
			node->args->accept(this);
		if (node->expr != 0)
			node->expr->accept(this);

		bool declaredClass = false;
		bool declaredMethod = false;
		for (int i = 0; i < table.classInfo.size(); i++){
			declaredClass = declaredClass || (table.classInfo[i].name == lastTypeValue);
			if (declaredClass) {
				for (int j = 0; j < table.classInfo[i].methods.size(); j++) {
					declaredMethod = declaredMethod || (table.classInfo[i].methods[j].name == node->name);
					if (declaredMethod) {
						lastTypeValue = table.classInfo[i].methods[j].returnType;
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
	}
	void visit(const CFewArgsExpressionNode* node){
		if (node->expr != 0)
			node->expr->accept(this);
	}

	void visit(const CListExpressionNode* node){
		if (node->prevExps != 0)
			node->prevExps->accept(this);
		if (node->nextExp != 0)
			node->nextExp->accept(this);
	}
	void visit(const CLastListExpressionNode* node){
		if (node->expr != 0)
			node->expr->accept(this);
	}
};
#endif
