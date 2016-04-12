#ifndef IRTREE_H_INCLUDED
#define IRTREE_H_INCLUDED
#include "../Structs/Temp.h"
#include "../Structs/Ast.h"
#include "../IRVisitors/Visitor.h"

namespace IRTree {

enum CJUMP_OP {
	EQ, NE, LT, GT, LE, GE, ULT, ULE, UGT, UGE
};

class INode {
public:
	virtual void accept(CIRVisitor* visitor) = 0;
	virtual ~INode() {}
};

template<class TARGET, class INTERFACE>
class CAcceptsIRVisitor : public INTERFACE {
public:
	virtual void accept(CIRVisitor* visitor) {
		visitor->visit( static_cast<TARGET*> (this) );
	}
};

class ExpList;

class IExp : public INode {
public:
	virtual  shared_ptr<ExpList> kids() = 0;
	virtual  IExp* build(shared_ptr<ExpList> kids) = 0;
};

class IStm : public INode {
public:
	virtual  shared_ptr<ExpList> kids() = 0;
	virtual  IStm* build(shared_ptr<ExpList> kids) = 0;
};



class ExpList {
public:
	ExpList(IExp* _head, shared_ptr<ExpList> _tail) : head(_head), tail(_tail) {}
	IExp* head;
	shared_ptr<ExpList> tail;
};

class StmtList {
public:
	StmtList(IStm* _head, shared_ptr<StmtList> _tail) : head(_head), tail(_tail) {}
	IStm* head;
	shared_ptr<StmtList> tail;
};

class MEM: public CAcceptsIRVisitor<MEM, IExp> {
public:
	MEM(IExp* _exp): exp(_exp) {}

	shared_ptr<ExpList> kids() {
		return make_shared<ExpList>(exp, nullptr);
	}

	IExp* build(shared_ptr<ExpList> kids) {
		return new MEM(kids->head);
	}

	IExp* exp;
};

class MOVE: public CAcceptsIRVisitor<MOVE, IStm> {
public:
	MOVE(IExp* _dst, IExp* _src): dst(_dst), src(_src) {}

	shared_ptr<ExpList> kids() {
		const MEM* memDst = dynamic_cast<const MEM*>(dst);
		if (memDst != 0) {
			return make_shared<ExpList>(memDst->exp, make_shared<ExpList>(src, nullptr));
		} else {
			return make_shared<ExpList>(src, nullptr);
		}
	}

	IStm* build(shared_ptr<ExpList> kids) {

		const MEM* memDst = dynamic_cast<const MEM*>(dst);
		if (memDst != 0) {
			return new MOVE(new MEM(kids->head), kids->tail.get()->head);
		} else {
			return new MOVE(dst, kids->head);
		}
	}

	IExp* dst;
	IExp* src;
};

class EXP: public CAcceptsIRVisitor<EXP, IStm> {
public:
	EXP(IExp* _exp): exp(_exp) {}

	shared_ptr<ExpList> kids() {
		return make_shared<ExpList>(exp, nullptr);
	}

	IStm* build(shared_ptr<ExpList> kids) {
		return new EXP(kids->head);
	}

	IExp* exp;
};


class JUMP: public CAcceptsIRVisitor<JUMP, IStm> {
public:
	JUMP(IExp* _exp, const Temp::CLabel* _target) : exp(_exp), target(_target) {}
	JUMP(const Temp::CLabel* _target) : exp(0), target(_target) {}

	shared_ptr<ExpList> kids() {
		if (exp == 0) {
			return 0;
		} else {
			return make_shared<ExpList>(exp, nullptr);
		}
	}

	IStm* build(shared_ptr<ExpList> kids) {
		if (kids != 0) {
			return new JUMP(kids->head, target);
		} else {
			return new JUMP(target);
		}
	}

	IExp* exp;
	const Temp::CLabel* target;
};

class CJUMP: public CAcceptsIRVisitor<CJUMP, IStm> {
public:
	CJUMP(CJUMP_OP _relop, IExp* _left, IExp* _right, const Temp::CLabel* _iftrue, const Temp::CLabel* _iffalse):
		relop(_relop), left(_left), right(_right), iftrue(_iftrue), iffalse(_iffalse) {}

	shared_ptr<ExpList> kids() {
		return make_shared<ExpList>(left,make_shared<ExpList>(right, nullptr));
	}

	IStm* build(shared_ptr<ExpList> kids) {

		return new CJUMP(relop, kids->head, kids->tail.get()->head, iftrue, iffalse);
	}

	CJUMP_OP relop;
	IExp* left;
	IExp* right;
	const Temp::CLabel* iftrue;
	const Temp::CLabel* iffalse;
};

class SEQ: public CAcceptsIRVisitor<SEQ, IStm> {
public:
	SEQ(IStm* _left, IStm* _right): left(_left), right(_right) {}

	shared_ptr<ExpList> kids() {
		assert(0);
		return 0;
	}

	IStm* build(shared_ptr<ExpList> kids) {
		assert(0);
		return 0;
	}

	IStm* left;
	IStm* right;
};

class LABEL: public CAcceptsIRVisitor<LABEL, IStm> {
public:
	LABEL(const Temp::CLabel* _label):label(_label) {}

	shared_ptr<ExpList> kids() {
		return 0;
	}

	IStm* build(shared_ptr<ExpList> kids) {
		return this;
	}

	const Temp::CLabel* label;
};


class CONST: public CAcceptsIRVisitor<CONST, IExp> {
public:
	CONST(int _value): value(_value) {}

	shared_ptr<ExpList> kids() {
		return 0;
	}

	IExp* build(shared_ptr<ExpList> kids) {
		return this;
	}

	int value;
};

class NAME : public CAcceptsIRVisitor<NAME, IExp> {
public:

	shared_ptr<ExpList> kids() {
		return 0;
	}

	IExp* build(shared_ptr<ExpList> kids) {
		return this;
	}

	NAME(shared_ptr<Temp::CLabel> _label): label(_label) {}
	shared_ptr<Temp::CLabel> label;
};

class TEMP: public CAcceptsIRVisitor<TEMP, IExp> {
public:

	shared_ptr<ExpList> kids() {
		return 0;
	}

	IExp* build(shared_ptr<ExpList> kids) {
		return this;
	}

	TEMP(shared_ptr<const Temp::CTemp> _temp): temp(_temp) {}
	shared_ptr<const Temp::CTemp> temp;
};

class BINOP: public CAcceptsIRVisitor<BINOP, IExp> {
public:
	BINOP(ArithmeticOpType _binop, IExp* _left, IExp* _right): binop(_binop), left(_left), right(_right) {}

	shared_ptr<ExpList> kids() {
		return make_shared<ExpList>(left, make_shared<ExpList>(right, nullptr));
	}

	IExp* build(shared_ptr<ExpList> kids) {
		return new BINOP(binop, kids->head, kids->tail.get()->head);
	}

	ArithmeticOpType binop;
	IExp* left;
	IExp* right;
};



class CALL: public CAcceptsIRVisitor<CALL, IExp> {
public:

	shared_ptr<ExpList> kids() {
		return make_shared<ExpList>(func, args);
	}

	IExp* build(shared_ptr<ExpList> kids) {
		return new CALL(kids->head, kids->tail);
	}

	CALL(IExp* _func, shared_ptr<ExpList> _args): func(_func), args(_args) {}
	IExp* func;
	shared_ptr<ExpList> args;
};

class ESEQ: public CAcceptsIRVisitor<ESEQ, IExp> {
public:
	ESEQ(IStm* _stm, IExp* _exp): stm(_stm), exp(_exp) {}

	shared_ptr<ExpList> kids() {
		assert(0);
		return 0;
	}

	IExp* build(shared_ptr<ExpList> kids) {
		assert(0);
		return 0;
	}

	IStm* stm;
	IExp* exp;
};

class MoveCall: public CAcceptsIRVisitor<MoveCall, IStm> {
public:
	TEMP* dst;
	CALL* src;
	MoveCall(TEMP* _dst, CALL* _src) : dst(_dst), src(_src) {}
	shared_ptr<ExpList> kids() {
		return src->kids();
	}
	IStm* build(shared_ptr<ExpList> kids) {
		return new MOVE(dst, src->build(kids));
	}
};

class ExpCall: public CAcceptsIRVisitor<ExpCall, IStm> {
public:
	CALL* call;
	ExpCall(CALL* _call) : call(_call) {}
	shared_ptr<ExpList> kids() {
		return call->kids();
	}
	IStm* build(shared_ptr<ExpList> kids) {
		return new EXP(call->build(kids));
	}
};

bool isNop(IStm* stm) {
	EXP* exp = dynamic_cast<EXP*>(stm);
	return (exp != 0) && (dynamic_cast<const CONST*>(exp->exp) != 0);
}

bool commute(IStm* stm, IExp* exp) {
	return isNop(stm) || (dynamic_cast<EXP*>(exp) != 0) || (dynamic_cast<const NAME*>(exp) != 0);
}


IStm* seq(IStm* arg1, IStm* arg2) {
	if (isNop(arg1)) {
		return arg2;
	} else {
		if (isNop(arg2)) {
			return arg1;
		} else {
			//cerr << "SEQ" << endl;
			return new SEQ(arg1, arg2);
		}
	}
}



class StmExpList {
public:

	StmExpList(IStm* _stm, shared_ptr<ExpList> _exps) : stm(_stm), exps(_exps) {}

	IStm* stm;
	shared_ptr<ExpList> exps;
};

StmExpList* reorder(shared_ptr<ExpList> list);

ESEQ* reorderExp(IExp* exp) {

	StmExpList* l = reorder(exp->kids());


	return new ESEQ(l->stm, exp->build(l->exps));
}

IStm* reorderStm(IStm* stm) {
	StmExpList* list = reorder(stm->kids());
	return seq(list->stm, stm->build(list->exps));
}

ESEQ* doExp(ESEQ* exp);

ESEQ* doExp(IExp* exp) {

	ESEQ* eseq = dynamic_cast<ESEQ*>(exp);
	if ( eseq != 0) {
		return doExp(eseq);
	} else {
		return reorderExp(exp);
	}
}


IStm* doStm(IStm* stm);
IStm* doStm(SEQ* s) {
	return seq(doStm(s->left), doStm(s->right));
}


IStm* doStm(MOVE* s) {
	TEMP* temp = dynamic_cast<TEMP*>(s->dst);
	CALL* call = dynamic_cast<CALL*>(s->src);
	if ( (temp != 0) && (call != 0)  ) {
		return reorderStm(new MoveCall(temp, call));
	} else {
		ESEQ* eseq = dynamic_cast<ESEQ*>(s->dst);
		if (eseq != 0) {
			return doStm(new SEQ( eseq->stm, new MOVE(eseq->exp, s->src)));
		} else {
			return reorderStm(s);
		}
	}

}

IStm* doStm(EXP* s) {
	CALL* call = dynamic_cast<CALL*>(s->exp);
	if (call != 0) {
		return reorderStm(new ExpCall(call));
	} else {
		return reorderStm(s);
	}
}


IStm* doStm(IStm* stm) {
	SEQ* seq = dynamic_cast<SEQ*>(stm);
	if (seq != 0) {
		//cerr << "seq" << endl;
		return doStm( seq );
	} else {
		MOVE* move = dynamic_cast<MOVE*>(stm);
		if (move != 0) {
			return doStm(move);
		} else {
			EXP* exp = dynamic_cast<EXP*>(stm);
			if (exp != 0) {
				return doStm(exp);

			} else {
				return reorderStm(stm);
			}
		}
	}
}

ESEQ* doExp(ESEQ* exp) {
	if (exp == 0) {
		exit(1);
	}

	IStm* stms = doStm(exp->stm);

	ESEQ* eseq = doExp(exp->exp);
	return new ESEQ(seq(stms, eseq->stm), eseq->exp);
}



StmExpList* reorder(shared_ptr<ExpList> list) {

	if (list == nullptr) {
		return new StmExpList(new EXP(new CONST(0)), 0);
	} else {

		IExp* head = list.get()->head;
		CALL* call = dynamic_cast<CALL*>(head);

		if (call != 0) {

			shared_ptr<const Temp::CTemp> t = make_shared<const Temp::CTemp>();
			IExp* eseq = new ESEQ(new MOVE(new TEMP(t), head), new TEMP(t));
			return reorder(make_shared<ExpList>(eseq, list->tail));
		} else {
			ESEQ* eseq = doExp(head);
			StmExpList* stmtList = reorder(list->tail);
			if (commute(stmtList->stm, eseq->exp)) {
				return new StmExpList(seq(eseq->stm, stmtList->stm),
									  make_shared<ExpList>(eseq->exp, stmtList->exps)
									 );
			} else {
				shared_ptr<const Temp::CTemp> t = make_shared<const Temp::CTemp>();
				return new StmExpList(seq(eseq->stm,
										  seq(new MOVE(new TEMP(t), eseq->exp),
											  stmtList->stm
											 )
										 ),
									  make_shared<ExpList>(new TEMP(t), stmtList->exps)
									 );
			}
		}
	}

}

shared_ptr<StmtList> linear(IStm* s, shared_ptr<StmtList> l);

shared_ptr<StmtList> linear(SEQ* s, shared_ptr<StmtList> l) {
	return linear(s->left, linear(s->right, l));
}

shared_ptr<StmtList> linear(IStm* s, shared_ptr<StmtList> l) {
	SEQ* seq = dynamic_cast<SEQ*>(s);
	if (seq != 0 ) {
		return linear(seq, l);
	} else {
		return make_shared<StmtList>(s, l);
	}
}

shared_ptr<StmtList> linearize(IStm* s) {
	return linear(doStm(s), nullptr);
}

};





#endif
