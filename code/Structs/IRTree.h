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
	virtual void accept(const CIRVisitor* visitor) const = 0;
	virtual ~INode() {}
};

template<class TARGET, class INTERFACE>
class CAcceptsIRVisitor : public INTERFACE {
public:
	virtual void accept(const CIRVisitor* visitor) const {
		visitor->visit( static_cast<const TARGET*> (this) );
	}
};

class ExpList;

class IExp : public INode {
public:
	virtual  shared_ptr<const ExpList> kids() const = 0;
	virtual  const IExp* build(shared_ptr<const ExpList> kids) const = 0;
};

class IStm : public INode {
public:
	virtual  shared_ptr<const ExpList> kids() const = 0;
	virtual  const IStm* build(shared_ptr<const ExpList> kids) const = 0;
};



class ExpList {
public:
	ExpList(const IExp* _head, shared_ptr<const ExpList> _tail) : head(_head), tail(_tail) {}
	const IExp* head;
	shared_ptr<const ExpList> tail;
};


class MEM: public CAcceptsIRVisitor<MEM, IExp> {
public:
	MEM(const IExp* _exp): exp(_exp) {}

	shared_ptr<const ExpList> kids() const {
		return make_shared<const ExpList>(exp, nullptr);
	}

	const IExp* build(shared_ptr<const ExpList> kids) const {
		return new MEM(kids->head);
	}

	const IExp* exp;
};

class MOVE: public CAcceptsIRVisitor<MOVE, IStm> {
public:
	MOVE(const IExp* _dst, const IExp* _src): dst(_dst), src(_src) {}

	shared_ptr<const ExpList> kids() const {
		const MEM* memDst = dynamic_cast<const MEM*>(dst);
		if (memDst != 0) {
			return make_shared<const ExpList>(memDst->exp, make_shared<const ExpList>(src, nullptr));
		} else {
			return make_shared<const ExpList>(src, nullptr);
		}
	}

	const IStm* build(shared_ptr<const ExpList> kids) const {

		const MEM* memDst = dynamic_cast<const MEM*>(dst);
		if (memDst != 0) {
			return new MOVE(new MEM(kids->head), kids->tail.get()->head);
		} else {
			return new MOVE(dst, kids->head);
		}
	}

	const IExp* dst;
	const IExp* src;
};

class EXP: public CAcceptsIRVisitor<EXP, IStm> {
public:
	EXP(const IExp* _exp): exp(_exp) {}

	shared_ptr<const ExpList> kids() const {
		return make_shared<const ExpList>(exp, nullptr);
	}

	const IStm* build(shared_ptr<const ExpList> kids) const {
		return new EXP(kids->head);
	}

	const IExp* exp;
};


class JUMP: public CAcceptsIRVisitor<JUMP, IStm> {
public:
	JUMP(const IExp* _exp, const Temp::CLabel* _target) : exp(_exp), target(_target) {}
	JUMP(const Temp::CLabel* _target) : exp(0), target(_target) {}

	shared_ptr<const ExpList> kids() const {
		if (exp == 0) {
			return 0;
		} else {
			return make_shared<const ExpList>(exp, nullptr);
		}
	}

	const IStm* build(shared_ptr<const ExpList> kids) const {
		if (kids != 0) {
			return new JUMP(kids->head, target);
		} else {
			return new JUMP(target);
		}
	}

	const IExp* exp;
	const Temp::CLabel* target;
};

class CJUMP: public CAcceptsIRVisitor<CJUMP, IStm> {
public:
	CJUMP(CJUMP_OP _relop, const IExp* _left, const IExp* _right, const Temp::CLabel* _iftrue, const Temp::CLabel* _iffalse):
		relop(_relop), left(_left), right(_right), iftrue(_iftrue), iffalse(_iffalse) {}

	shared_ptr<const ExpList> kids() const {
		return make_shared<const ExpList>(left,make_shared<const ExpList>(right, nullptr));
	}

	const IStm* build(shared_ptr<const ExpList> kids) const {

		return new CJUMP(relop, kids->head, kids->tail.get()->head, iftrue, iffalse);
	}

	CJUMP_OP relop;
	const IExp* left;
	const IExp* right;
	const Temp::CLabel* iftrue;
	const Temp::CLabel* iffalse;
};

class SEQ: public CAcceptsIRVisitor<SEQ, IStm> {
public:
	SEQ(const IStm* _left, const IStm* _right): left(_left), right(_right) {}

	shared_ptr<const ExpList> kids() const {
		assert(0);
		return 0;
	}

	const IStm* build(shared_ptr<const ExpList> kids) const {
		assert(0);
		return 0;
	}

	const IStm* left;
	const IStm* right;
};

class LABEL: public CAcceptsIRVisitor<LABEL, IStm> {
public:
	LABEL(const Temp::CLabel* _label):label(_label) {}

	shared_ptr<const ExpList> kids() const {
		return 0;
	}

	const IStm* build(shared_ptr<const ExpList> kids) const {
		return this;
	}

	const Temp::CLabel* label;
};


class CONST: public CAcceptsIRVisitor<CONST, IExp> {
public:
	CONST(int _value): value(_value) {}

	shared_ptr<const ExpList> kids() const {
		return 0;
	}

	const IExp* build(shared_ptr<const ExpList> kids) const {
		return this;
	}

	int value;
};

class NAME : public CAcceptsIRVisitor<NAME, IExp> {
public:

	shared_ptr<const ExpList> kids() const {
		return 0;
	}

	const IExp* build(shared_ptr<const ExpList> kids) const {
		return this;
	}

	NAME(shared_ptr<Temp::CLabel> _label): label(_label) {}
	shared_ptr<Temp::CLabel> label;
};

class TEMP: public CAcceptsIRVisitor<TEMP, IExp> {
public:

	shared_ptr<const ExpList> kids() const {
		return 0;
	}

	const IExp* build(shared_ptr<const ExpList> kids) const {
		return this;
	}

	TEMP(shared_ptr<const Temp::CTemp> _temp): temp(_temp) {}
	shared_ptr<const Temp::CTemp> temp;
};

class BINOP: public CAcceptsIRVisitor<BINOP, IExp> {
public:
	BINOP(ArithmeticOpType _binop, const IExp* _left, const IExp* _right): binop(_binop), left(_left), right(_right) {}

	shared_ptr<const ExpList> kids() const {
		return make_shared<const ExpList>(left, make_shared<const ExpList>(right, nullptr));
	}

	const IExp* build(shared_ptr<const ExpList> kids) const {
		return new BINOP(binop, kids->head, kids->tail.get()->head);
	}

	ArithmeticOpType binop;
	const IExp* left;
	const IExp* right;
};



class CALL: public CAcceptsIRVisitor<CALL, IExp> {
public:

	shared_ptr<const ExpList> kids() const {
		return make_shared<const ExpList>(func, args);
	}

	const IExp* build(shared_ptr<const ExpList> kids) const {
		return new CALL(kids->head, kids->tail);
	}

	CALL(const IExp* _func, shared_ptr<const ExpList> _args): func(_func), args(_args) {}
	const IExp* func;
	shared_ptr<const ExpList> args;
};

class ESEQ: public CAcceptsIRVisitor<ESEQ, IExp> {
public:
	ESEQ(const IStm* _stm, const IExp* _exp): stm(_stm), exp(_exp) {}

	shared_ptr<const ExpList> kids() const {
		assert(0);
		return 0;
	}

	const IExp* build(shared_ptr<const ExpList> kids) const {
		assert(0);
		return 0;
	}

	const IStm* stm;
	const IExp* exp;
};

class MoveCall: public CAcceptsIRVisitor<MoveCall, IStm> {
public:
	const TEMP* dst;
	const CALL* src;
	MoveCall(const TEMP* _dst, const CALL* _src) : dst(_dst), src(_src) {}
	shared_ptr<const ExpList> kids() const {
		return src->kids();
	}
	const IStm* build(shared_ptr<const ExpList> kids) const {
		return new MOVE(dst, src->build(kids));
	}
};

class ExpCall: public CAcceptsIRVisitor<ExpCall, IStm> {
public:
	const CALL* call;
	ExpCall(const CALL* _call) : call(_call) {}
	shared_ptr<const ExpList> kids() const {
		return call->kids();
	}
	const IStm* build(shared_ptr<const ExpList> kids) const {
		return new EXP(call->build(kids));
	}
};

bool isNop(const IStm* stm) {
	const EXP* exp = dynamic_cast<const EXP*>(stm);
	return (exp != 0) && (dynamic_cast<const CONST*>(exp->exp) != 0);
}

bool commute(const IStm* stm, const IExp* exp) {
	return isNop(stm) || (dynamic_cast<const EXP*>(exp) != 0) || (dynamic_cast<const NAME*>(exp) != 0);
}


const IStm* seq(const IStm* arg1, const IStm* arg2) {
	if (isNop(arg1)) {
		return arg2;
	} else {
		if (isNop(arg2)) {
			return arg1;
		} else {
			return new SEQ(arg1, arg2);
		}
	}
}



class StmExpList {
public:

	StmExpList(const IStm* _stm, shared_ptr<const ExpList> _exps) : stm(_stm), exps(_exps) {}

	const IStm* stm;
	shared_ptr<const ExpList> exps;
};

const StmExpList* reorder(shared_ptr<const ExpList> list);

const ESEQ* reorderExp(const IExp* exp) {

	const StmExpList* l = reorder(exp->kids());


	return new ESEQ(l->stm, exp->build(l->exps));
}

const IStm* reorderStm(const IStm* stm) {
	const StmExpList* list = reorder(stm->kids());
	return seq(list->stm, stm->build(list->exps));
}

const ESEQ* doExp(const ESEQ* exp);

const ESEQ* doExp(const IExp* exp) {

	const ESEQ* eseq = dynamic_cast<const ESEQ*>(exp);
	if ( eseq != 0) {
		return doExp(eseq);
	} else {
		return reorderExp(exp);
	}
}


const IStm* doStm(const IStm* stm);
const IStm* doStm(const SEQ* s) {
	return seq(doStm(s->left), doStm(s->right));
}


const IStm* doStm(const MOVE* s) {

	const TEMP* temp = dynamic_cast<const TEMP*>(s->dst);
	const CALL* call = dynamic_cast<const CALL*>(s->src);
	if ( (temp != 0) && (call != 0)  ) {
		return reorderStm(new MoveCall(temp, call));
	} else {
		const ESEQ* eseq = dynamic_cast<const ESEQ*>(s->dst);
		if (eseq != 0) {
			return doStm(new SEQ( eseq->stm, new MOVE(eseq->exp, s->src)));
		} else {
			return reorderStm(s);
		}
	}

}

const IStm* doStm(const EXP* s) {

	const CALL* call = dynamic_cast<const CALL*>(s->exp);
	if (call != 0) {
		return reorderStm(new ExpCall(call));
	} else {
		return reorderStm(s);
	}
}


const IStm* doStm(const IStm* stm) {

	const SEQ* seq = dynamic_cast<const SEQ*>(stm);
	if (seq != 0) {
		return doStm( seq );
	} else {
		const MOVE* move = dynamic_cast<const MOVE*>(stm);
		if (move != 0) {
			return doStm(move);
		} else {
			const EXP* exp = dynamic_cast<const EXP*>(stm);
			if (exp != 0) {
				return doStm(exp);

			} else {
				return reorderStm(stm);
			}
		}
	}
}

const ESEQ* doExp(const ESEQ* exp) {
	if (exp == 0) {
		exit(1);
	}

	const IStm* stms = doStm(exp->stm);

	const ESEQ* eseq = doExp(exp->exp);
	return new ESEQ(seq(stms, eseq->stm), eseq->exp);
}



const StmExpList* reorder(shared_ptr<const ExpList> list) {

	if (list == nullptr) {
		return new StmExpList(new EXP(new CONST(0)), 0);
	} else {

		const IExp* head = list.get()->head;
		const CALL* call = dynamic_cast<const CALL*>(head);

		if (call != 0) {

			shared_ptr<const Temp::CTemp> t(new Temp::CTemp());
			const IExp* eseq = new ESEQ(new MOVE(new TEMP(t), head), new TEMP(t));
			return reorder(make_shared<const ExpList>(eseq, list->tail));
		} else {
			const ESEQ* eseq = doExp(head);
			const StmExpList* stmtList = reorder(list->tail);
			if (commute(stmtList->stm, eseq->exp)) {
				return new StmExpList(seq(eseq->stm, stmtList->stm),
									  make_shared<const ExpList>(eseq->exp, stmtList->exps)
									 );
			} else {
				shared_ptr<const Temp::CTemp> t(new Temp::CTemp());
				return new StmExpList(seq(eseq->stm,
										  seq(new MOVE(new TEMP(t), eseq->exp),
											  stmtList->stm
											 )
										 ),
									  make_shared<const ExpList>(new TEMP(t), stmtList->exps)
									 );
			}
		}
	}

}


};




#endif
