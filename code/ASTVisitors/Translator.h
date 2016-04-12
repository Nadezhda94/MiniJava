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
	virtual IExp* ToExp() const = 0;
	virtual IStm* ToStm() const = 0;
	virtual IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const = 0;
};

class CExpConverter : public ISubtreeWrapper {
public:
	CExpConverter(IExp* _expr) : expr(_expr) {}

	IExp* ToExp() const {
		return expr;
	}

	IStm* ToStm() const {
		return new EXP(expr);
	}

	IStm* ToConditional(const Temp::CLabel* t,const Temp::CLabel* f) const {
		return new CJUMP(EQ, expr, new CONST(0), f, t);
	}

private:
	IExp* expr;
};

class CStmConverter : public ISubtreeWrapper {
public:
	CStmConverter(IStm* _stm): stm(_stm) {}

	IExp* ToExp() const {
		assert(0);
	}

	IStm* ToStm() const {
		return stm;
	}

	IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const {
		assert(0);
	}

private:
	IStm* stm;
};

class CConditionalWrapper : public ISubtreeWrapper {
public:
	IExp* ToExp() const {
		shared_ptr<CTemp> r = shared_ptr<CTemp>( new Temp::CTemp() );
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

	virtual IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const = 0;

	IStm* ToStm() const {
		Temp::CLabel* jmp = new Temp::CLabel();
		return new SEQ( ToConditional(jmp, jmp), new LABEL(jmp) );
	}
};

class CRelativeCmpWrapper : public CConditionalWrapper {
public:
	CRelativeCmpWrapper(CJUMP_OP _op, IExp* _first, IExp* _second) :
		op(_op), first(_first), second(_second) {}
	IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const {
		return new CJUMP(op, first, second, t, f);
	}
private:
	IExp* first;
	IExp* second;
	CJUMP_OP op;
};

class CFromAndConverter : public CConditionalWrapper {
public:
	CFromAndConverter(IExp* _leftArg, IExp* _rightArg) :
		leftArg(_leftArg), rightArg(_rightArg) {}
	IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const {
		const Temp::CLabel* z = new Temp::CLabel();
		return new SEQ( new CJUMP(LT, leftArg, new CONST(1), f, z),
						new SEQ(new LABEL(z), new CJUMP(LT, rightArg, new CONST(1), f, t)));
	}
private:
	IExp* leftArg;
	IExp* rightArg;
};

class CFromOrConverter : public CConditionalWrapper {
public:
	CFromOrConverter(IExp* _leftArg, IExp* _rightArg) : leftArg(_leftArg), rightArg(_rightArg) {}
	IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const {
		const CLabel* z = new CLabel();
		return new SEQ(new CJUMP(GT, leftArg, new CONST(1), t, z),
					   new SEQ(new LABEL(z), new CJUMP(LT, rightArg, new CONST(1), f, t)));
	}
private:
	IExp* leftArg;
	IExp* rightArg;
};



class CTranslator: public CVisitor {
	CStorage* symbolsStorage;
	CTable table;
	map<string, shared_ptr<CLabel>> functionalLabels;

	CClassInfo* currentClass;
	CMethodInfo* currentMethod;
	shared_ptr<CFrame> currentFrame;
	shared_ptr<ISubtreeWrapper> currentNode;
	shared_ptr<ExpList> arguments;

	const CSymbol* getMallocFuncName() {
		return symbolsStorage->get("#malloc");
	}

	const CSymbol* getPrintFuncName() {
		return symbolsStorage->get("#print");
	}

public:
	vector<INode*> trees;

	CTranslator(CStorage* _symbols, CTable& _table):
		symbolsStorage(_symbols), table(_table),
		currentClass(&table.classInfo[0]),
		currentMethod(&table.classInfo[0].methods[0]) {
		for (int i=0; i<table.classInfo.size(); i++) {
			CClassInfo cl = table.classInfo[i];
			for (int j=0; j<cl.methods.size(); j++) {
				string name = cl.methods[j].name->getString();
				functionalLabels[name] = shared_ptr<CLabel>(new CLabel(name));
			}
		}
	}

	void visit(const CProgramRuleNode* node) {
		node->mainClass->accept(this);
		if (node->decl != 0)
			node->decl->accept(this);
	}

	void visit(const CMainClassDeclarationRuleNode* node) {
		const CSymbol* methodName = symbolsStorage->get("main");
		currentMethod = &(currentClass->getMethodInfo(methodName));
		currentFrame = shared_ptr<CFrame>( new CFrame(methodName) );
		currentFrame->allocFormal(symbolsStorage->get("this")); // this
		for (int i = 0; i < currentMethod->params.size(); i++) {
			currentFrame->allocFormal(currentMethod->params[i].name);
		}
		for (int i = 0; i < currentMethod->vars.size(); i++) {
			currentFrame->allocLocal(currentMethod->vars[i].name);
		}
		for (int i = 0; i < currentClass->vars.size(); i++) {
			currentFrame->allocVar(currentClass->vars[i].name);
		}

		if (node->stmt != 0) {
			node->stmt->accept(this);
			trees.push_back(currentNode->ToStm());
		}
	}

	void visit(const CDeclarationsListNode* node) {
		if (node->decl != 0)
			node->decl->accept(this);
		if (node->cl != 0)
			node->cl->accept(this);
	}

	void visit(const CClassDeclarationRuleNode* node) {
		currentClass = &table.getClassInfo(node->ident);
		if (node->extDecl != 0)
			node->extDecl->accept(this);
		if (node->vars != 0)
			node->vars->accept(this);
		if (node->method != 0)
			node->method->accept(this);
	}

	void visit(const CExtendDeclarationRuleNode* node) {
		// TODO: realize
	}

	void visit(const CVarDeclarationsListNode* node) {
		if (node->list != 0)
			node->list->accept(this);

		if (node->item != 0)
			node->item->accept(this);
	}

	void visit(const CMethodDeclarationsListNode* node) {
		if (node->list != 0)
			node->list->accept(this);
		node->item->accept(this);
	}

	void visit(const CVarDeclarationRuleNode* node) {}

	void visit(const CMethodDeclarationRuleNode* node) {
		currentMethod = &(currentClass->getMethodInfo(node->ident));
		currentFrame = shared_ptr<CFrame>( new CFrame(node->ident) );
		currentFrame->allocFormal(symbolsStorage->get("this")); // this
		for (int i = 0; i < currentMethod->params.size(); i++) {
			currentFrame->allocFormal(currentMethod->params[i].name);
		}
		for (int i = 0; i < currentMethod->vars.size(); i++) {
			currentFrame->allocLocal(currentMethod->vars[i].name);
		}
		vector <CVarInfo> parentVars = table.getParentVars(currentClass->name);
		for (int i = 0; i < parentVars.size(); i++) {
			currentFrame->allocVar(parentVars[i].name);
		}
		for (int i = 0; i < currentClass->vars.size(); i++) {
			currentFrame->allocVar(currentClass->vars[i].name);
		}

		node->return_exp->accept(this);
		IExp* res = currentNode->ToExp();

		if (node->method_body != 0) {
			node->method_body->accept(this);
			IStm* arg1 = currentNode->ToStm();
			res = new ESEQ(arg1, res);
		}

		trees.push_back(res);
	}

	void visit(const CVarsDecListNode* node) {
		if (node->list != 0)
			node->list->accept(this);
		if (node->next != 0)
			node->next->accept(this);
	}

	void visit(const CVarsDecFirstNode* node) {
		if (node->first != 0)
			node->first->accept(this);
	}

	void visit(const CStatsFirstNode* node) {
		if (node->stm != 0)
			node->stm->accept(this);
	}

	void visit(const CStatsListNode* node) {
		IStm* inner_seq = 0;
		if (node->list != 0) {
			node->list->accept(this);
			inner_seq = currentNode->ToStm();
		}

		IStm* res = currentNode->ToStm();
		if (node->stm != 0) {
			node->stm->accept(this);
			if (inner_seq != 0)
				res = new SEQ( inner_seq, currentNode->ToStm() );
		}
		currentNode = shared_ptr<CStmConverter>( new CStmConverter(res) );
	}

	void visit(const CMethodBodyVarsNode* node) {}

	void visit(const CMethodBodyStatsNode* node) {
		node->stats->accept(this);
	}

	void visit(const CMethodBodyAllNode* node) {
		node->stats->accept(this);
	}

	void visit(const CParamArgListNode* node) {}
	void visit(const CParamsOneNode* node) {}
	void visit(const CParamsTwoNode* node) {}
	void visit(const CParamRuleNode* node) {}
	void visit(const CTypeRuleNode* node) {}

	void visit(const CNumerousStatementsNode* node) {
		IStm* inner_seq = 0;
		if (node->statements != 0) {
			node->statements->accept(this);
			inner_seq = currentNode->ToStm();
		}
		node->statement->accept(this);
		IStm* res = currentNode->ToStm();
		if (inner_seq != 0)
			res = new SEQ( inner_seq, currentNode->ToStm() );
		currentNode = shared_ptr<CStmConverter>( new CStmConverter(res) );
	}

	void visit(const CBracedStatementNode* node) {
		if (node->statements != 0)
			node->statements->accept(this);
	}

	void visit(const CIfStatementNode* node) {
		node->expression->accept(this);
		const CLabel* t = new CLabel();
		const CLabel* f = new CLabel();
		const CLabel* e = new CLabel();
		IStm* stm = currentNode->ToConditional(t, f);

		node->thenStatement->accept(this);

		IStm* thenStatement = currentNode->ToStm();
		thenStatement = new SEQ( new SEQ( new LABEL(t), thenStatement ), new JUMP(e) );
		if (node->elseStatement != 0) {
			node->elseStatement->accept(this);
		}
		IStm* elseStatement = currentNode->ToStm();
		elseStatement = new SEQ( new SEQ( new LABEL(f), elseStatement ), new LABEL(e) );

		IStm* res = new SEQ( new SEQ( stm, thenStatement ), elseStatement );
		currentNode = shared_ptr<CStmConverter>( new CStmConverter(res) );
	}

	void visit(const CWhileStatementNode* node) {
		node->expression->accept(this);

		IExp* expr = currentNode->ToExp();
		node->statement->accept(this);
		IStm* statement = currentNode->ToStm();
		const CLabel* f = new CLabel();
		const CLabel* t = new CLabel();

		IStm* res = new SEQ(new SEQ(new SEQ(new SEQ(
											  new CJUMP(EQ, expr, new CONST(0), f, t),
											  new LABEL(t)),
										  statement),
										  new CJUMP(EQ, expr, new CONST(0), f, t)),
								  new LABEL(f));

		currentNode = shared_ptr<CStmConverter>(new CStmConverter(res));
	}

	void visit(const CPrintStatementNode* node) {
		node->expression->accept(this);
		IExp* exp = currentNode->ToExp();
		shared_ptr<ExpList> args = shared_ptr<ExpList>( new ExpList(exp, 0) );
		IExp* printCall = currentFrame->externalCall(getPrintFuncName()->getString(), args);
		const CExpConverter* res = new CExpConverter(printCall);
		currentNode = std::shared_ptr<CStmConverter>(new CStmConverter(res->ToStm()));
	}

	void visit(const CAssignStatementNode* node) {
		node->expression->accept(this);
		IStm* res = new MOVE( currentFrame->findByName(node->identifier), currentNode->ToExp() );
		currentNode = shared_ptr<CStmConverter>(new CStmConverter(res));
	}
	void visit(const CInvokeExpressionStatementNode* node) {
		node->firstexpression->accept(this);
		node->secondexpression->accept(this);
	}

	void visit(const CInvokeExpressionNode* node) {
		node->firstExp->accept(this);
		node->secondExp->accept(this);
	}

	void visit(const CLengthExpressionNode* node) {
		node->expr->accept(this);
	}

	void visit(const CArithmeticExpressionNode* node) {
		node->firstExp->accept(this);
		IExp* arg1 = currentNode->ToExp();
		node->secondExp->accept(this);
		IExp* arg2 = currentNode->ToExp();
		IExp* res;
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

		currentNode = shared_ptr<CExpConverter>(new CExpConverter(res));
	}

	void visit(const CUnaryExpressionNode* node) {
		node->expr->accept(this);
		IExp* arg = currentNode->ToExp();
		IExp* res = new BINOP(node->op, new CONST(0), arg);
		currentNode = std::shared_ptr<CExpConverter>(new CExpConverter(res));
	}

	void visit(const CCompareExpressionNode* node) {
		node->firstExp->accept(this);
		IExp* arg1 = currentNode->ToExp();
		node->secondExp->accept(this);
		IExp* arg2 = currentNode->ToExp();
		const CConditionalWrapper* cmpWrapper = new CRelativeCmpWrapper(LT, arg1, arg2);
		currentNode = shared_ptr<CExpConverter>(new CExpConverter(cmpWrapper->ToExp()));

	}

	void visit(const CNotExpressionNode* node) {
		node->expr->accept(this);
		IExp* arg = currentNode->ToExp();
		const CConditionalWrapper* cmpWrapper = new CRelativeCmpWrapper(EQ, arg, new CONST(0));
		currentNode = std::shared_ptr<CExpConverter>(new CExpConverter(cmpWrapper->ToExp()) );
	}

	void visit(const CNewArrayExpressionNode* node) {
		node->expr->accept(this);
		IExp* arg = currentNode->ToExp();
		shared_ptr<CTemp> arrSize = shared_ptr<CTemp>( new CTemp() );
		IExp* calcArrSize = new BINOP(PLUS_OP, arg, new CONST(1));
		IStm* storeArrSize = new MOVE (new TEMP(arrSize), calcArrSize);
		IExp* sizeInBytes = new BINOP(MULT_OP, new MEM( new TEMP(arrSize)), new CONST(4));

		shared_ptr<ExpList> args = shared_ptr<ExpList>(new ExpList(sizeInBytes, 0));
		IExp* memCall = currentFrame->externalCall(getMallocFuncName()->getString(), args);
		shared_ptr<CTemp> temp = shared_ptr<CTemp>( new CTemp() );

		IStm* storeCalcRes = new MOVE( new TEMP(temp), memCall);
		IStm* storeLength = new MOVE( new MEM( new TEMP(temp) ), new MEM( new TEMP(arrSize) ) );

		IExp* res = new ESEQ( new SEQ( storeArrSize,
											 new SEQ(storeCalcRes,
													 storeLength)),
									new TEMP(temp)
								  );
		currentNode = shared_ptr<CExpConverter>(new CExpConverter(res));
	}

	void visit(const CNewObjectExpressionNode* node) {
		shared_ptr<CTemp> temp = shared_ptr<CTemp>(new CTemp());

		int varsSizeInBytes = CFrame::wordSize * table.getClassInfo(node->objType).vars.size();
		if (varsSizeInBytes < CFrame::wordSize)
			varsSizeInBytes = CFrame::wordSize;
		shared_ptr<ExpList> args = shared_ptr<ExpList>(new ExpList(new CONST(varsSizeInBytes), 0));
		IExp* memCall = currentFrame->externalCall(getMallocFuncName()->getString(), args);
		IStm* storeCalcRes = new MOVE( new TEMP(temp), memCall);
		IExp* res = new ESEQ( storeCalcRes,
									new TEMP(temp));

		currentNode = shared_ptr<CExpConverter>(new CExpConverter(res));
	}

	void visit(const CIntExpressionNode* node) {
		currentNode = shared_ptr<CExpConverter>(
						  new CExpConverter(new CONST(node->value)));
	}

	void visit(const CBooleanExpressionNode* node) {
		currentNode = shared_ptr<CExpConverter>(
						  new CExpConverter(new CONST(node->value)));
	}

	void visit(const CIdentExpressionNode* node) {
		IExp* result = currentFrame->findByName(node->name);
		currentNode = shared_ptr<CExpConverter>(new CExpConverter(result));
	}

	void visit(const CThisExpressionNode* node) {
		currentNode = shared_ptr<CExpConverter>(
						  new CExpConverter(currentFrame->getTP()->getExp()));
	}

	void visit(const CParenExpressionNode* node) {
		node->expr->accept(this);
	}

	void visit(const CInvokeMethodExpressionNode* node) {
		node->expr->accept(this);
		IExp* texp = currentNode->ToExp();
		if (node->args != 0)
			node->args->accept(this);
		// надеюсь, что после прохода по списку аргументов (экспрешнов) currentNode станет ExpList
		arguments = shared_ptr<ExpList>( new ExpList(texp, arguments) );  //надо как-то в список аргументов зацепить this
		IExp* name = new NAME(functionalLabels[node->name->getString()]);
		IExp* res = new CALL(name, arguments);
		currentNode = shared_ptr<CExpConverter>(new CExpConverter (res));
		arguments = 0; //сбрасываем старые аргументы
	}

	void visit(const CFewArgsExpressionNode* node) {
		node->expr->accept(this);
	}

	void visit(const CListExpressionNode* node) {
		node->prevExps->accept(this);
		node->nextExp->accept(this);
		arguments = shared_ptr<ExpList>( new ExpList(currentNode->ToExp(), arguments) );
	}

	void visit(const CLastListExpressionNode* node) {
		node->expr->accept(this);
		arguments = shared_ptr<ExpList>( new ExpList(currentNode->ToExp(), arguments) );
	}
};

}
#endif
