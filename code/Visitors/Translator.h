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
	CExpConverter(const IRTree::IExp* _expr) : expr(_expr) {}

	const IRTree::IExp* ToExp() const {
		return expr;
	}

	const IRTree::IStm* ToStm() const {
		return new IRTree::EXP(expr);
	}

	const IRTree::IStm* ToConditional(const Temp::CLabel* t,const Temp::CLabel* f) const {
		return new IRTree::CJUMP(IRTree::EQ, expr, new IRTree::CONST(0), f, t);
	}

private:
	const IRTree::IExp* expr;
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

	virtual const IRTree::IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const = 0;

	const IRTree::IStm* ToStm() const {
		Temp::CLabel* jmp = new Temp::CLabel();

		return new IRTree::SEQ( ToConditional(jmp, jmp), new IRTree::LABEL(jmp) );
	}
};

class CRelativeCmpWrapper : public CConditionalWrapper {
public:
	CRelativeCmpWrapper(CJUMP_OP op, const IExp* _first, const IExp* _second) : first(_first), second(_second) {}
	const IRTree::IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const {
		return new CJUMP(op, first, second, t, f);
	}
private:
	const IExp* first;
	const IExp* second;
	CJUMP_OP op;
};

class CFromAndConverter : public CConditionalWrapper {
public:
	CFromAndConverter(const IExp* _leftArg, const IExp* _rightArg) : leftArg(_leftArg), rightArg(_rightArg) {}
	const IRTree::IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const {
		const Temp::CLabel* z = new Temp::CLabel();
		return new SEQ( new CJUMP(LT, leftArg, new CONST(1), f, z), 
			new SEQ(new LABEL(z), new CJUMP(LT, rightArg, new CONST(1), f, t)));
	}
private:
	const IExp* leftArg;
	const IExp* rightArg;
};

class CFromOrConverter : public CConditionalWrapper {
public:
	CFromOrConverter(const IExp* _leftArg, const IExp* _rightArg) : leftArg(_leftArg), rightArg(_rightArg) {}
	const IRTree::IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const {
		const CLabel* z = new CLabel();
		return new SEQ(new CJUMP(GT, leftArg, new CONST(1), t, z), 
			new SEQ(new LABEL(z), new CJUMP(LT, rightArg, new CONST(1), f, t)));
	}
private:
	const IExp* leftArg;
	const IExp* rightArg;
};

class CTranslator: public CVisitor {
	CStorage* symbolsStorage;
	CTable table;
	SymbolsTable::CClassInfo* current_class;
	SymbolsTable::CMethodInfo* current_method;
	CFrame* current_frame;
	std::shared_ptr<ISubtreeWrapper> current_node;
	std::vector<const INode*> trees;
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
		current_frame->allocFormal(); // this
		for (int i = 0; i < current_method->params.size(); i++){
			current_frame->allocFormal();
		}
		for (int i = 0; i < current_method->vars.size(); i++){
			current_frame->allocLocal();
		}
		for (int i = 0; i < current_class->vars.size(); i++){
			current_frame->allocVar();
		}
		node->type->accept(this);
		if (node->param_arg != 0)
			node->param_arg->accept(this);

		if (node->method_body != 0)
			node->method_body->accept(this);
		const IStm* arg1 = current_node->ToStm();

		node->return_exp->accept(this);
		const IExp* arg2 = current_node->ToExp();
		const IExp* res = new ESEQ(arg1, arg2);
		trees.push_back(res);

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

	//TODO
	void visit(const CIfStatementNode* node){
		node->expression->accept(this);
		const IExp* expr = current_node->ToExp();
		node->thenStatement->accept(this);
		const IStm* thenStatement = current_node->ToStm();
		if (node->elseStatement != 0){
			node->elseStatement->accept(this);
		}
		const IStm* elseStatement = current_node->ToStm();
		const CLabel* t = new CLabel();
		const CLabel* f = new CLabel();
		const IStm* res;
		current_node = std::shared_ptr<CStmConverter>(new CStmConverter(res));
	}

	void visit(const CWhileStatementNode* node){
		node->expression->accept(this);
		const IExp* expr = current_node->ToExp();
		node->statement->accept(this);
		const IStm* statement = current_node->ToStm();
		const CLabel* f = new CLabel();
		const CLabel* z = new CLabel();
		const IStm* res = new SEQ(new SEQ(new CJUMP(LT, expr, new CONST(1), f, z), 
			new SEQ(new SEQ(new LABEL(z), statement), new CJUMP(LT, expr, new CONST(1), f, z))), 
			new LABEL(f));
		current_node = std::shared_ptr<CStmConverter>(new CStmConverter(res));

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
		const IExp* arg1 = current_node->ToExp();
		node->secondExp->accept(this);
		const IExp* arg2 = current_node->ToExp();
		const IExp* res;
		const CConditionalWrapper* converter;
		switch (node->opType) {
			case AND_OP:
				converter = new CFromAndConverter(
					arg1,
					arg2);
				res = converter->ToExp();
				delete converter;
				break;
			case OR_OP:
				converter = new CFromOrConverter(
					arg1,
					arg2);
				res = converter->ToExp();
				delete converter;
				break;
			default:
				res = new BINOP(node->opType,
					arg1,
					arg2);
				break;
		}

		current_node = std::shared_ptr<CExpConverter>(new CExpConverter(res));
	}

	void visit(const CUnaryExpressionNode* node){
		node->expr->accept(this);
		const IExp* arg = current_node->ToExp();
		const IExp* res;// = new BINOP(node->op, new CONST(0), arg);
		current_node = std::shared_ptr<CExpConverter>(new CExpConverter(res));
	}

	void visit(const CCompareExpressionNode* node){
		node->firstExp->accept(this);
		const IExp* arg1 = current_node->ToExp();
		node->secondExp->accept(this);
		const IExp* arg2 = current_node->ToExp();

		const CConditionalWrapper* cmpWrapper = new CRelativeCmpWrapper(LT, 
			arg1, arg2);
		current_node = std::shared_ptr<CExpConverter>(new CExpConverter(cmpWrapper->ToExp()));
		
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
		current_node = std::shared_ptr<CExpConverter>(
			new CExpConverter(new CONST(node->value)));
	}

	void visit(const CBooleanExpressionNode* node){
	}

	void visit(const CIdentExpressionNode* node){
		try{
			int index = current_method->getLocalIndex(node->name);
			IExp* result = current_frame->getLocal(index)->getExp();
			current_node =std::shared_ptr<CExpConverter>(new CExpConverter(result));
		}
		catch (std::out_of_range* oor){
			delete oor;
			try{
				int index = current_method->getFormalIndex(node->name);
				IExp* result = current_frame->getFormal(index)->getExp();
				current_node = std::shared_ptr<CExpConverter>(new CExpConverter(result));
			}
			catch (std::out_of_range* oor){
				delete oor;
				try{
					int index = current_class->getVarIndex(node->name);
					IExp* result = current_frame->getVar(index)->getExp();
					current_node = std::shared_ptr<CExpConverter>(new CExpConverter(result));
				}
				catch (std::out_of_range* oor){
					cout<<oor->what()<< " "<<node->name->getString()<<endl;
				}
			}
		}
	}

	void visit(const CThisExpressionNode* node){
		current_node = std::shared_ptr<CExpConverter>(
			new CExpConverter(current_frame->getFormal(0)->getExp()));
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
