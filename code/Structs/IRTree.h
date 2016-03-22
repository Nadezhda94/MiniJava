#ifndef IRTREE_H_INCLUDED
#define IRTREE_H_INCLUDED
#include "../Structs/Temp.h"
#include "../Structs/Ast.h"
#include "../IRVisitors/Visitor.h"

namespace IRTree {

enum CJUMP_OP{
	EQ, NE, LT, GT, LE, GE, ULT, ULE, UGT, UGE
};

class INode{
public:
	virtual void accept(const CIRVisitor* visitor) const = 0;
	virtual ~INode(){}
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
	virtual  const ExpList* kids() const = 0;
	virtual  const IExp* build(const ExpList* kids) const = 0;
};

class IStm : public INode {
public:
	virtual  const ExpList* kids() const = 0;
	virtual  const IStm* build(const ExpList* kids) const = 0;
};



class ExpList {
public:
	ExpList(const IExp* _head, const ExpList* _tail) : head(_head), tail(_tail) {}

	const IExp* head;
	const ExpList* tail;
};


class MEM: public CAcceptsIRVisitor<MEM, IExp> {
public:
	MEM(const IExp* _exp): exp(_exp) {}

	const ExpList* kids() const {
		return new ExpList(exp, 0);
	}

	const IExp* build(const ExpList* kids) const {
		return new MEM(kids->head);
	}

	const IExp* exp;
};

class MOVE: public CAcceptsIRVisitor<MOVE, IStm> {
public:
	MOVE(const IExp* _dst, const IExp* _src): dst(_dst), src(_src) {}

	const ExpList* kids() const {
		const MEM* memDst = dynamic_cast<const MEM*>(dst);
		if (memDst != 0) {
			return new ExpList(memDst->exp, new ExpList(src, 0));
		} else {
			return new ExpList(src, 0);
		}
	}

	const IStm* build(const ExpList* kids) const {
		
		const MEM* memDst = dynamic_cast<const MEM*>(dst);
		if (memDst != 0) {
			return new MOVE(new MEM(kids->head), kids->tail->head);
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

	const ExpList* kids() const {
		return new ExpList(exp, 0);
	}
	
	const IStm* build(const ExpList* kids) const {
		return new EXP(kids->head);
	}

	const IExp* exp;
};


class JUMP: public CAcceptsIRVisitor<JUMP, IStm> {
public:
	JUMP(const IExp* _exp, const Temp::CLabel* _target) : exp(_exp), target(_target) {}
	JUMP(const Temp::CLabel* _target) : exp(0), target(_target) {}

	const ExpList* kids() const {
		return new ExpList(exp, 0);
	}
	
	const IStm* build(const ExpList* kids) const {
		return new JUMP(kids->head, target);
	}

	const IExp* exp;
	const Temp::CLabel* target;
};

class CJUMP: public CAcceptsIRVisitor<CJUMP, IStm> {
public:
	CJUMP(CJUMP_OP _relop, const IExp* _left, const IExp* _right, const Temp::CLabel* _iftrue, const Temp::CLabel* _iffalse):
						relop(_relop), left(_left), right(_right), iftrue(_iftrue), iffalse(_iffalse) {}
	
	const ExpList* kids() const {
		return new ExpList(left, new ExpList(right, 0));
	}
	
	const IStm* build(const ExpList* kids) const {
		return new CJUMP(relop, kids->head, kids->tail->head, iftrue, iffalse);
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

	const ExpList* kids() const {
		assert(0);
		return 0;
	}
	
	const IStm* build(const ExpList* kids) const {
		assert(0);
		return 0;
	}

	const IStm* left;
	const IStm* right;
};

class LABEL: public CAcceptsIRVisitor<LABEL, IStm> {
public:
	LABEL(const Temp::CLabel* _label):label(_label) {}

	const ExpList* kids() const {
		return 0;
	}
	
	const IStm* build(const ExpList* kids) const {
		return this;
	}

	const Temp::CLabel* label;
};


class CONST: public CAcceptsIRVisitor<CONST, IExp> {
public:
	CONST(int _value): value(_value){}

	const ExpList* kids() const {
		return 0;
	}

	const IExp* build(const ExpList* kids) const {
		return this;
	}

	int value;
};

class NAME : public CAcceptsIRVisitor<NAME, IExp> {
public:
	NAME(const Temp::CLabel* _label): label(_label) {}

	const ExpList* kids() const {
		return 0;
	}

	const IExp* build(const ExpList* kids) const {
		return this;
	}


	const Temp::CLabel* label;
};

class TEMP: public CAcceptsIRVisitor<TEMP, IExp> {
public:
	TEMP(const Temp::CTemp* _temp): temp(_temp) {}

	const ExpList* kids() const {
		return 0;
	}

	const IExp* build(const ExpList* kids) const {
		return this;
	}


	const Temp::CTemp* temp;
};

class BINOP: public CAcceptsIRVisitor<BINOP, IExp> {
public:
	BINOP(ArithmeticOpType _binop, const IExp* _left, const IExp* _right): binop(_binop), left(_left), right(_right) {}
	
	const ExpList* kids() const {
		return new ExpList(left, new ExpList(right, 0));
	}

	const IExp* build(const ExpList* kids) const {
		return new BINOP(binop, kids->head, kids->tail->head);
	}

	ArithmeticOpType binop;
	const IExp* left;
	const IExp* right;
};



class CALL: public CAcceptsIRVisitor<CALL, IExp> {
public:
	CALL(const IExp* _func, const ExpList* _args): func(_func), args(_args) {}

	const ExpList* kids() const {
		return new ExpList(func, args);
	}

	const IExp* build(const ExpList* kids) const {
		return new CALL(kids->head, kids->tail);
	}

	const IExp* func;
	const ExpList* args;
};

class ESEQ: public CAcceptsIRVisitor<ESEQ, IExp> {
public:
	ESEQ(const IStm* _stm, const IExp* _exp): stm(_stm), exp(_exp) {}

	const ExpList* kids() const {
		assert(0);
		return 0;
	}

	const IExp* build(const ExpList* kids) const {
		assert(0);
		return 0;
	}

	const IStm* stm;
	const IExp* exp;
};

bool isNop(const IStm* stm) {
	const EXP* exp = dynamic_cast<const EXP*>(stm);
	return (exp != 0) && (dynamic_cast<const CONST*>(exp->exp) != 0);
}

bool commute(const IStm* stm, const IExp* exp) {
	return isNop(stm) || (dynamic_cast<const EXP*>(exp) != 0) || (dynamic_cast<const NAME*>(exp) != 0);
}



class StmExpList {
public:

	StmExpList(const IStm* _stm, const ExpList* _exps) : stm(_stm), exps(_exps) {}

	const IStm* stm;
	const ExpList* exps;
};

const StmExpList* reorder(const ExpList* list);

const ESEQ* reorderExp(const IExp* exp) {
	const StmExpList* l = reorder(exp->kids());
	return new ESEQ(l->stm, exp->build(l->exps));
}

const IStm* reorderStm(const IStm* stm) {
	const StmExpList* list = reorder(stm->kids());
	return new SEQ(list->stm, stm->build(list->exps));
}

const ESEQ* doExp(const IExp* exp) {
	const ESEQ* eseq = dynamic_cast<const ESEQ*>(exp);
	if ( eseq != 0) {
		return eseq;
	} else {
		return reorderExp(exp);
	}
}

const IStm* doStm(const IStm* stm);
const IStm* doStm(const SEQ* s) {
	return new SEQ(doStm(s->left), doStm(s->right));
}

 /*const IStm* doStm(const MOVE* s) {
 	
 }*/



const IStm* doStm(const EXP* s) {
	/*const CALL* call = dynamic_cast<const CALL*>(s);
	if (call != 0) {
		return reorderStm()
	}*/
}


const IStm* doStm(const IStm* stm) {
	const SEQ* seq = dynamic_cast<const SEQ*>(stm);
	if (stm != 0) {
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
	const IStm* stms = doStm(exp->stm);
	const ESEQ* eseq = doExp(exp->exp);
	return new ESEQ(new SEQ(stms, eseq->stm), eseq->exp);
}



const StmExpList* reorder(const ExpList* list) {
	if (list == 0) {
		return new StmExpList(new EXP(new CONST(0)), 0);
	}
	const IExp* head = list->head;					
	const ESEQ* eseq = doExp(head);
	const StmExpList* stmtList = reorder(list->tail);
	if (commute(stmtList->stm, eseq->exp)) {
		return new StmExpList(new SEQ(eseq->stm, stmtList->stm), 
								new ExpList(eseq->exp, stmtList->exps)
							);
	} else {
		const Temp::CTemp* t = new Temp::CTemp();
		return new StmExpList(new SEQ(eseq->stm, 
										new SEQ(new MOVE(new TEMP(t), eseq->exp), 
												stmtList->stm
												)
									), 
								new ExpList(new TEMP(t), stmtList->exps)
							);
	}

}


};




#endif
