#ifndef TRANSLATOR_H_INCLUDED
#define TRANSLATOR_H_INCLUDED
#include "../common.h"
#include "../Visitors/Visitor.h"
#include "../Structs/IRTree.h"
#include "../Structs/Temp.h"
#include "../Structs/Frame.h"


namespace Translate {

using namespace IRTree;
using namespace Temp;
using namespace Frame;

class ISubtreeWrapper {
public:
	virtual ~ISubtreeWrapper() { }
	virtual const IRTree::IExp* ToExp() const = 0;
	virtual const IRTree::IStm* ToStm() const = 0;
	virtual const IRTree::IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const = 0;
};

class CExpConverter : public ISubtreeWrapper {
public:
	CExpConverter(IRTree::IExp* _expr) : expr(_expr) {}

	const IRTree::IExp* ToExp() const {
		return expr;
	}

	const IRTree::IStm* ToStm() const {
		return new IRTree::EXP(expr);
	}

	const IRTree::IStm* ToConditional(Temp::CLabel* t, Temp::CLabel* f) const {
		return new IRTree::CJUMP(IRTree::EQ, expr, new IRTree::CONST(0), f, t);
	}

private:
	IRTree::IExp* expr;
};

class CStmConverter : public ISubtreeWrapper {
public:
	CStmConverter(const IRTree::IStm* _stm): stm(_stm) {}

	const IRTree::IExp* ToExp() const {
		assert(0);
	}

	const IRTree::IStm* ToStm() const {
		return stm;
	}

	const IRTree::IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const {
		assert(0);
	}

private:
	const IRTree::IStm* stm;
};

/*class CConditionalWrapper : public ISubtreeWrapper {
public:
    virtual const IRTree::IExp* ToExp() const { /* return CONST(1) or CONST(0); }
    virtual const IRTree::IStm* ToStm() const { /* return stm; }
    virtual const IRTree::IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const = 0;
};

class CRelativeCmpWrapper : public CConditionalWrapper {
public:
	CRelativeCmpWrapper ( op, e1, e2 );
	virtual const IRTree::IStm* ToConditional( t, f ) const { return new IRTree::CJUMP(...); }
};*/

class CConditionalWrapper : public ISubtreeWrapper {
public:
	const IRTree::IExp* ToExp() const {
		Temp::CTemp* r = new Temp::CTemp();
		Temp::CLabel* t = new Temp::CLabel();
		Temp::CLabel* f = new Temp::CLabel();
		return new IRTree::ESEQ( new IRTree::SEQ( new IRTree::MOVE( new IRTree::TEMP(r), new IRTree::CONST(1) ),
			new IRTree::SEQ( ToConditional(t, f), new IRTree::SEQ( new IRTree::LABEL(f),
				new IRTree::SEQ( new IRTree::MOVE( new IRTree::TEMP(r), new IRTree::CONST(0) ),
					new IRTree::LABEL(t) ) ) ) ),
			new IRTree::TEMP(r) );
	}

	/*virtual*/ IRTree::IStm* ToConditional(Temp::CLabel* t, Temp::CLabel* f) const {
		return NULL;
		}
};

class CTranslator: public CVisitor {
	CStorage* symbolsStorage;
	CTable table;
	SymbolsTable::CClassInfo* current_class;
	SymbolsTable::CMethodInfo* current_method;
	CFrame* current_frame;
	std::vector<INode*> trees;
	std::vector<INode*> children;
public:
	CTranslator(CStorage* _symbols, CTable& _table):
		symbolsStorage(_symbols), table(_table),
		current_class(&table.classInfo[0]),
		current_method(&table.classInfo[0].methods[0])
	{}
	void visit(const CProgramRuleNode* node){
		node->mainClass->accept(this);
		if (node->decl != 0)
			node->decl->accept(this);
		for (int i=0; i<children.size();i++)
			cout<<typeid(*children[i]).name()<<endl;
	}

	void visit(const CMainClassDeclarationRuleNode* node){
		node->stmt->accept(this);
	}

	void visit(const CDeclarationsListNode* node){
		if (node->decl != 0)
			node->decl->accept(this);
		if (node->cl != 0)
			node->cl->accept(this);
	}

	void visit(const CClassDeclarationRuleNode* node){
		current_class = &table.getClassInfo(node->ident);
		if (node->extDecl != 0)
			node->extDecl->accept(this);
		if (node->vars != 0)
			node->vars->accept(this);
		node->method->accept(this);
	}

	void visit(const CExtendDeclarationRuleNode* node){
	}

	void visit(const CVarDeclarationsListNode* node){
		if (node->list != 0)
			node->list->accept(this);
		node->item->accept(this);
	}

	void visit(const CMethodDeclarationsListNode* node){
		if (node->list != 0)
			node->list->accept(this);
		node->item->accept(this);
	}

	void visit(const CVarDeclarationRuleNode* node){
	}

	void visit(const CMethodDeclarationRuleNode* node){
		current_method = &(current_class->getMethodInfo(node->ident));
		current_frame = new CFrame(node->ident);
		for (int i = 0; i < current_method->params.size(); i++){
			current_frame->allocFormal();
		}
		for (int i = 0; i < current_method->vars.size(); i++){
			current_frame->allocLocal();
		}

		node->type->accept(this);
		if (node->param_arg != 0)
			node->param_arg->accept(this);
		if (node->method_body != 0)
			node->method_body->accept(this);
		node->return_exp->accept(this);
		delete current_frame;
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
		node->type->accept(this);
	}

	void visit(const CTypeRuleNode* node){
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
		node->expression->accept(this);
		node->thenStatement->accept(this);
		if (node->elseStatement != 0){
			node->elseStatement->accept(this);
		}
	}

	void visit(const CWhileStatementNode* node){
		node->expression->accept(this);
		node->statement->accept(this);
	}

	void visit(const CPrintStatementNode* node){
		node->expression->accept(this);
	}

	void visit(const CAssignStatementNode* node){
		node->expression->accept(this);
	}
	void visit(const CInvokeExpressionStatementNode* node){
		node->firstexpression->accept(this);
		node->secondexpression->accept(this);
	}

	void visit(const CInvokeExpressionNode* node){
		node->firstExp->accept(this);
		node->secondExp->accept(this);
	}

	void visit(const CLengthExpressionNode* node){
		node->expr->accept(this);
	}

	void visit(const CArithmeticExpressionNode* node) {
		node->firstExp->accept(this);
		node->secondExp->accept(this);
		IExp* res = new BINOP(node->opType,
			static_cast<IExp*>(children[children.size()-2]),
			static_cast<IExp*>(children[children.size()-1]));
		children.pop_back();
		children.pop_back();
		children.push_back(res);
	}

	void visit(const CUnaryExpressionNode* node){
		node->expr->accept(this);
	}

	void visit(const CCompareExpressionNode* node){
		node->firstExp->accept(this);
		node->secondExp->accept(this);
	}

	void visit(const CNotExpressionNode* node){
		node->expr->accept(this);
	}

	void visit(const CNewArrayExpressionNode* node){
		node->expr->accept(this);
	}

	void visit(const CNewObjectExpressionNode* node){
	}

	void visit(const CIntExpressionNode* node) {
		children.push_back(new CONST(node->value));
	}

	void visit(const CBooleanExpressionNode* node){
	}

	void visit(const CIdentExpressionNode* node){
		try{
			int index = current_method->getLocalIndex(node->name);
			IExp* result = current_frame->getLocal(index)->getExp(current_frame->getFP());
			children.push_back(result);
		}
		catch (std::out_of_range* oor){
			delete oor;
			try{
				int index = current_method->getFormalIndex(node->name);
				IExp* result = current_frame->getFormal(index)->getExp(current_frame->getFP());
				children.push_back(result);
			}
			catch (std::out_of_range* oor){
				cout<<oor->what()<< " "<<node->name->getString()<<endl;
			}
		}
	}

	void visit(const CThisExpressionNode* node){
	}

	void visit(const CParenExpressionNode* node){
		node->expr->accept(this);
	}

	void visit(const CInvokeMethodExpressionNode* node){
		node->expr->accept(this);
		if (node->args != 0)
			node->args->accept(this);
	}

	void visit(const CFewArgsExpressionNode* node){
		node->expr->accept(this);
	}

	void visit(const CListExpressionNode* node){
		node->prevExps->accept(this);
		node->nextExp->accept(this);
	}

	void visit(const CLastListExpressionNode* node){
		node->expr->accept(this);
	}
};

}
#endif
