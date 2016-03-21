#ifndef TRANSLATOR_H_INCLUDED
#define TRANSLATOR_H_INCLUDED
#include "../common.h"
#include "../ASTVisitors/Visitor.h"
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
	virtual const IExp* ToExp() const = 0;
	virtual const IStm* ToStm() const = 0;
	virtual const IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const = 0;
};

class CExpConverter : public ISubtreeWrapper {
public:
	CExpConverter(const IExp* _expr) : expr(_expr) {}

	const IExp* ToExp() const {
		return expr;
	}

	const IStm* ToStm() const {
		return new EXP(expr);
	}

	const IStm* ToConditional(const Temp::CLabel* t,const Temp::CLabel* f) const {
		return new CJUMP(EQ, expr, new CONST(0), f, t);
	}

private:
	const IExp* expr;
};

class CStmConverter : public ISubtreeWrapper {
public:
	CStmConverter(const IStm* _stm): stm(_stm) {}

	const IExp* ToExp() const {
		assert(0);
	}

	const IStm* ToStm() const {
		return stm;
	}

	const IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const {
		assert(0);
	}

private:
	const IStm* stm;
};

class CConditionalWrapper : public ISubtreeWrapper {
public:
	const IExp* ToExp() const {
		Temp::CTemp* r = new Temp::CTemp();
		Temp::CLabel* t = new Temp::CLabel();
		Temp::CLabel* f = new Temp::CLabel();
		return new ESEQ(
			   new SEQ( new MOVE( new TEMP(r), new CONST(1) ),
			   new SEQ( ToConditional(t, f),
		 	   new SEQ( new LABEL(f),
			   new SEQ( new MOVE( new TEMP(r), new CONST(0) ),
						 new LABEL(t) ) ) ) ),
			   new TEMP(r) );
	}

	virtual const IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const = 0;

	const IStm* ToStm() const {
		Temp::CLabel* jmp = new Temp::CLabel();
		return new SEQ( ToConditional(jmp, jmp), new LABEL(jmp) );
	}
};

class CRelativeCmpWrapper : public CConditionalWrapper {
public:
	CRelativeCmpWrapper(CJUMP_OP _op, const IExp* _first, const IExp* _second) :
		op(_op), first(_first), second(_second) {}
	const IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const {
		return new CJUMP(op, first, second, t, f);
	}
private:
	const IExp* first;
	const IExp* second;
	CJUMP_OP op;
};

class CFromAndConverter : public CConditionalWrapper {
public:
	CFromAndConverter(const IExp* _leftArg, const IExp* _rightArg) :
		leftArg(_leftArg), rightArg(_rightArg) {}
	const IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const {
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
	const IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const {
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
	ExpList* arguments = 0;
	std::shared_ptr<ISubtreeWrapper> current_node;
	
	const CSymbol* getMallocFuncName() {
		return symbolsStorage->get("#malloc");
	}

	const CSymbol* getPrintFuncName() {
		return symbolsStorage->get("#print");
	}

public:
	std::vector<const INode*> trees;

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
		const CSymbol* methodName = symbolsStorage->get("main");
		current_method = &(current_class->getMethodInfo(methodName));
		current_frame = new CFrame(methodName);
		current_frame->allocFormal(symbolsStorage->get("this")); // this
		for (int i = 0; i < current_method->params.size(); i++){
			current_frame->allocFormal(current_method->params[i].name);
		}
		for (int i = 0; i < current_method->vars.size(); i++){
			current_frame->allocLocal(current_method->vars[i].name);
		}
		for (int i = 0; i < current_class->vars.size(); i++){
			current_frame->allocVar(current_class->vars[i].name);
		}

		if (node->stmt != 0){
			//node->stmt->accept(this);
			//trees.push_back(current_node->ToStm());
		}

		delete current_frame;
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
		// TODO: realize
	}

	void visit(const CVarDeclarationsListNode* node){
	}

	void visit(const CMethodDeclarationsListNode* node){
		if (node->list != 0)
			node->list->accept(this);
		node->item->accept(this);
	}

	void visit(const CVarDeclarationRuleNode* node){}

	void visit(const CMethodDeclarationRuleNode* node){
		current_method = &(current_class->getMethodInfo(node->ident));
		current_frame = new CFrame(node->ident);
		current_frame->allocFormal(symbolsStorage->get("this")); // this
		for (int i = 0; i < current_method->params.size(); i++){
			current_frame->allocFormal(current_method->params[i].name);
		}
		for (int i = 0; i < current_method->vars.size(); i++){
			current_frame->allocLocal(current_method->vars[i].name);
		}
		for (int i = 0; i < current_class->vars.size(); i++){
			current_frame->allocVar(current_class->vars[i].name);
		}

		if (node->method_body != 0){
			node->method_body->accept(this);
			const IStm* arg1 = current_node->ToStm();
			node->return_exp->accept(this);
			const IExp* arg2 = current_node->ToExp();
			const IExp* res = new ESEQ(arg1, arg2);
			trees.push_back(res);
		}
		else 
		{
			node->return_exp->accept(this);
			const IExp* arg2 = current_node->ToExp();
			//const IExp* res = arg2;
			trees.push_back(arg2);
		}

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
		const IStm* inner_seq = 0;
		if (node->list != 0){
			node->list->accept(this);
			inner_seq = current_node->ToStm();
		}

		const IStm* res = current_node->ToStm();
		if (node->stm != 0){
			node->stm->accept(this);
			if (inner_seq != 0)
				res = new SEQ( inner_seq, current_node->ToStm() );
		}
		current_node = std::shared_ptr<CStmConverter>( new CStmConverter(res) );
	}

	void visit(const CMethodBodyVarsNode* node){}

	void visit(const CMethodBodyStatsNode* node){
		node->stats->accept(this);
	}

	void visit(const CMethodBodyAllNode* node){
		node->stats->accept(this);
	}

	void visit(const CParamArgListNode* node){}
	void visit(const CParamsOneNode* node){}
	void visit(const CParamsTwoNode* node){}
	void visit(const CParamRuleNode* node){}
	void visit(const CTypeRuleNode* node){}

	void visit(const CNumerousStatementsNode* node){
		const IStm* inner_seq = 0;
		if (node->statements != 0){
			node->statements->accept(this);
			inner_seq = current_node->ToStm();
		}
		node->statement->accept(this);
		const IStm* res = current_node->ToStm();
		if (inner_seq != 0)
			res = new SEQ( inner_seq, current_node->ToStm() );
		current_node = std::shared_ptr<CStmConverter>( new CStmConverter(res) );
	}

	void visit(const CBracedStatementNode* node){
		if (node->statements != 0)
			node->statements->accept(this);
	}

	void visit(const CIfStatementNode* node){
		node->expression->accept(this);
		const CLabel* t = new CLabel();
		const CLabel* f = new CLabel();
		const CLabel* e = new CLabel();
		const IStm* stm = current_node->ToConditional(t, f);

		node->thenStatement->accept(this);

		const IStm* thenStatement = current_node->ToStm();
		thenStatement = new SEQ( new SEQ( new LABEL(t), thenStatement ), new JUMP(e) );
		if (node->elseStatement != 0){
			node->elseStatement->accept(this);
		}
		const IStm* elseStatement = current_node->ToStm();
		elseStatement = new SEQ( new SEQ( new LABEL(f), elseStatement ), new LABEL(e) );

		const IStm* res = new SEQ( new SEQ( stm, thenStatement ), elseStatement );
		current_node = std::shared_ptr<CStmConverter>( new CStmConverter(res) );
	}

	void visit(const CWhileStatementNode* node){
		node->expression->accept(this);

		const IExp* expr = current_node->ToExp();
		node->statement->accept(this);
		const IStm* statement = current_node->ToStm();
		const CLabel* f = new CLabel();
		const CLabel* t = new CLabel();

		const IStm* res = new SEQ(new SEQ(new SEQ(new SEQ(
							  new CJUMP(EQ, expr, new CONST(0), f, t),
						  	  new LABEL(t)),
					  		  statement),
							  new CJUMP(EQ, expr, new CONST(0), f, t)),
					  		  new LABEL(f));

		current_node = std::shared_ptr<CStmConverter>(new CStmConverter(res));
	}

	void visit(const CPrintStatementNode* node){
		node->expression->accept(this);
		const IExp* exp = current_node->ToExp();
		const ExpList* args = new ExpList(exp, 0);
		const IExp* printCall = current_frame->externalCall(getPrintFuncName()->getString(), args);
		const CExpConverter* res = new CExpConverter(printCall);
		current_node = std::shared_ptr<CStmConverter>(new CStmConverter(res->ToStm()));
	}

	void visit(const CAssignStatementNode* node){
		node->expression->accept(this);
		const IStm* res = new MOVE( current_frame->findByName(node->identifier), current_node->ToExp() );
		current_node = std::shared_ptr<CStmConverter>(new CStmConverter(res));
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
				converter = new CFromAndConverter( arg1, arg2 );
				res = converter->ToExp();
				break;
			case OR_OP:
				converter = new CFromOrConverter( arg1, arg2 );
				res = converter->ToExp();
				break;
			default:
				res = new BINOP( node->opType, arg1, arg2 );
				break;
		}

		current_node = std::shared_ptr<CExpConverter>(new CExpConverter(res));
	}

	void visit(const CUnaryExpressionNode* node){
		node->expr->accept(this);
		const IExp* arg = current_node->ToExp();
		const IExp* res = new BINOP(node->op, new CONST(0), arg);
		current_node = std::shared_ptr<CExpConverter>(new CExpConverter(res));
	}

	void visit(const CCompareExpressionNode* node){
		node->firstExp->accept(this);
		const IExp* arg1 = current_node->ToExp();
		node->secondExp->accept(this);
		const IExp* arg2 = current_node->ToExp();
		const CConditionalWrapper* cmpWrapper = new CRelativeCmpWrapper(LT, arg1, arg2);
		current_node = std::shared_ptr<CExpConverter>(new CExpConverter(cmpWrapper->ToExp()));

	}

	void visit(const CNotExpressionNode* node){
		node->expr->accept(this);
		const IExp* arg = current_node->ToExp();
		const CConditionalWrapper* cmpWrapper = new CRelativeCmpWrapper(EQ, arg, new CONST(0));
		current_node = std::shared_ptr<CExpConverter>(new CExpConverter(cmpWrapper->ToExp()) );
	}

	void visit(const CNewArrayExpressionNode* node){
		node->expr->accept(this);
		const IExp* arg = current_node->ToExp();
		const CTemp* arrSize = new CTemp();
		const IExp* calcArrSize = new BINOP(PLUS_OP, arg, new CONST(1));
		const IStm* storeArrSize = new MOVE (new TEMP(arrSize), calcArrSize);
		const IExp* sizeInBytes = new BINOP(MULT_OP, new MEM( new TEMP(arrSize)), new CONST(4));
		
		
		const ExpList* args = new ExpList(sizeInBytes, 0);
		const IExp* memCall = current_frame->externalCall(getMallocFuncName()->getString(), args);
		const CTemp* temp = new CTemp();

		const IStm* storeCalcRes = new MOVE( new TEMP(temp), memCall);
		const IStm* storeLength = new MOVE( new MEM( new TEMP(temp) ), new MEM( new TEMP(arrSize) ) );
		//const CTemp* curVar = new CTemp();
		//const IStm* writeZero = new CJUMP(LT,  )
		
		const IExp* res = new ESEQ(  new SEQ( storeArrSize, 
											new SEQ(storeCalcRes, 
													storeLength 
													) 
											), 
									new TEMP(temp)
									);
		current_node = std::shared_ptr<CExpConverter>(new CExpConverter(res));
	}

	void visit(const CNewObjectExpressionNode* node){
		const CTemp* temp = new CTemp();

		int varsSizeInBytes = CFrame::wordSize * table.getClassInfo(node->objType).vars.size();
		const ExpList* args = new ExpList(new CONST(varsSizeInBytes), 0);
		const IExp* memCall = current_frame->externalCall(getMallocFuncName()->getString(), args);
		const IStm* storeCalcRes = new MOVE( new TEMP(temp), memCall);
		const IExp* res = new ESEQ( storeCalcRes, 
									new TEMP(temp));

		current_node = std::shared_ptr<CExpConverter>(new CExpConverter(res));
	}

	void visit(const CIntExpressionNode* node) {
		current_node = std::shared_ptr<CExpConverter>(
			new CExpConverter(new CONST(node->value)));
	}

	void visit(const CBooleanExpressionNode* node){
		current_node = std::shared_ptr<CExpConverter>(
			new CExpConverter(new CONST(node->value)));
	}

	void visit(const CIdentExpressionNode* node){
		IExp* result = current_frame->findByName(node->name);
		current_node = std::shared_ptr<CExpConverter>(new CExpConverter(result));
	}

	void visit(const CThisExpressionNode* node){
		current_node = std::shared_ptr<CExpConverter>(
			new CExpConverter(current_frame->getTP()->getExp()));
	}

	void visit(const CParenExpressionNode* node){
		node->expr->accept(this);
	}

	void visit(const CInvokeMethodExpressionNode* node){
		//node->expr->accept(this);
		//if (node->args != 0)
		//	node->args->accept(this);
		//ExpList* arguments = new ExpList(current_frame->findByName(symbolsStorage->get("this")), arguments);  //надо как-то в список аргументов зацепить this
		//current_node = std::shared_ptr<CExpConverter>(new CExpConverter (new CALL(new NAME(/*метка для конкретной функции!*/0), arguments))); // надеюсь, что после прохода по списку аргументов (экспрешнов) current_node станет ExpList
		//arguments = 0; //сбрасываем старые аргументы
	}

	void visit(const CFewArgsExpressionNode* node){
		node->expr->accept(this);
	}

	void visit(const CListExpressionNode* node){
		node->prevExps->accept(this);
		node->nextExp->accept(this);
		ExpList* arguments = new ExpList(current_node->ToExp(), arguments);
	}

	void visit(const CLastListExpressionNode* node){
		node->expr->accept(this);
		ExpList* arguments = new ExpList(current_node->ToExp(), arguments);
	}
};

}
#endif
