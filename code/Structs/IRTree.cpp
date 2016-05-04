#include "IRTree.h"

namespace IRTree {
	//--------------------------------------------------------------------------------------------------------------
	// MEM
	//--------------------------------------------------------------------------------------------------------------
	MEM::MEM(IExp* _exp): exp(_exp) {}
	shared_ptr<ExpList> MEM::kids() {
		return make_shared<ExpList>(exp, nullptr);
	}
	IExp* MEM::build(shared_ptr<ExpList> kids) {
		return new MEM(kids->head);
	}

	//--------------------------------------------------------------------------------------------------------------
	// MOVE
	//--------------------------------------------------------------------------------------------------------------
	MOVE::MOVE(IExp* _dst, IExp* _src): dst(_dst), src(_src) {}

	shared_ptr<ExpList> MOVE::kids() {
		const MEM* memDst = dynamic_cast<const MEM*>(dst);
		if (memDst != 0)
			return make_shared<ExpList>(memDst->exp, make_shared<ExpList>(src, nullptr));
		return make_shared<ExpList>(src, nullptr);
	}

	IStm* MOVE::build(shared_ptr<ExpList> kids) {
		const MEM* memDst = dynamic_cast<const MEM*>(dst);
		if (memDst != 0)
			return new MOVE(new MEM(kids->head), kids->tail.get()->head);
		return new MOVE(dst, kids->head);
	}

	//--------------------------------------------------------------------------------------------------------------
	// EXP
	//--------------------------------------------------------------------------------------------------------------
	EXP::EXP(IExp* _exp): exp(_exp) {}

	shared_ptr<ExpList> EXP::kids() {
		return make_shared<ExpList>(exp, nullptr);
	}

	IStm* EXP::build(shared_ptr<ExpList> kids) {
		return new EXP(kids->head);
	}

	//--------------------------------------------------------------------------------------------------------------
	// JUMP
	//--------------------------------------------------------------------------------------------------------------
	JUMP::JUMP(IExp* _exp, const Temp::CLabel* _target) : exp(_exp), target(_target) {}

	JUMP::JUMP(const Temp::CLabel* _target) : exp(0), target(_target) {}

	shared_ptr<ExpList> JUMP::kids() {
		if (exp == 0) return 0;
		return make_shared<ExpList>(exp, nullptr);
	}

	IStm* JUMP::build(shared_ptr<ExpList> kids) {
		if (kids != 0) {
			return new JUMP(kids->head, target);
		} else {
			return new JUMP(target);
		}
	}

	//--------------------------------------------------------------------------------------------------------------
	// CJUMP
	//--------------------------------------------------------------------------------------------------------------
	CJUMP::CJUMP(CJUMP_OP _relop, IExp* _left, IExp* _right, const Temp::CLabel* _iftrue, const Temp::CLabel* _iffalse):
			relop(_relop), left(_left), right(_right), iftrue(_iftrue), iffalse(_iffalse) {}

	shared_ptr<ExpList> CJUMP::kids() {
		return make_shared<ExpList>(left,make_shared<ExpList>(right, nullptr));
	}

	IStm* CJUMP::build(shared_ptr<ExpList> kids) {
		return new CJUMP(relop, kids->head, kids->tail.get()->head, iftrue, iffalse);
	}

	//--------------------------------------------------------------------------------------------------------------
	// SEQ
	//--------------------------------------------------------------------------------------------------------------
	SEQ::SEQ(IStm* _left, IStm* _right): left(_left), right(_right) {}

	shared_ptr<ExpList> SEQ::kids() {
		assert(0);
		return 0;
	}

	IStm* SEQ::build(shared_ptr<ExpList> kids) {
		assert(0);
		return 0;
	}

	//--------------------------------------------------------------------------------------------------------------
	// LABEL
	//--------------------------------------------------------------------------------------------------------------
	LABEL::LABEL(const Temp::CLabel* _label):label(_label) {}

	shared_ptr<ExpList> LABEL::kids() {
		return 0;
	}

	IStm* LABEL::build(shared_ptr<ExpList> kids) {
		return this;
	}

	//--------------------------------------------------------------------------------------------------------------
	// CONST
	//--------------------------------------------------------------------------------------------------------------
	CONST::CONST(int _value): value(_value) {}

	shared_ptr<ExpList> CONST::kids() {
		return 0;
	}

	IExp* CONST::build(shared_ptr<ExpList> kids) {
		return this;
	}


	//--------------------------------------------------------------------------------------------------------------
	// NAME
	//--------------------------------------------------------------------------------------------------------------
	NAME::NAME(shared_ptr<Temp::CLabel> _label): label(_label) {}

	shared_ptr<ExpList> NAME::kids() {
		return 0;
	}

	IExp* NAME::build(shared_ptr<ExpList> kids) {
		return this;
	}

	//--------------------------------------------------------------------------------------------------------------
	// TEMP
	//--------------------------------------------------------------------------------------------------------------
	TEMP::TEMP(shared_ptr<const Temp::CTemp> _temp): temp(_temp) {}

	shared_ptr<ExpList> TEMP::kids() {
		return 0;
	}

	IExp* TEMP::build(shared_ptr<ExpList> kids) {
		return this;
	}

	//--------------------------------------------------------------------------------------------------------------
	// BINOP
	//--------------------------------------------------------------------------------------------------------------

	BINOP::BINOP(ArithmeticOpType _binop, IExp* _left, IExp* _right): binop(_binop), left(_left), right(_right) {}

	shared_ptr<ExpList> BINOP::kids() {
		return make_shared<ExpList>(left, make_shared<ExpList>(right, nullptr));
	}

	IExp* BINOP::build(shared_ptr<ExpList> kids) {
		return new BINOP(binop, kids->head, kids->tail.get()->head);
	}

	//--------------------------------------------------------------------------------------------------------------
	// CALL
	//--------------------------------------------------------------------------------------------------------------
	CALL::CALL(IExp* _func, shared_ptr<ExpList> _args): func(_func), args(_args) {}

	shared_ptr<ExpList> CALL::kids() {
		return make_shared<ExpList>(func, args);
	}

	IExp* CALL::build(shared_ptr<ExpList> kids) {
		return new CALL(kids->head, kids->tail);
	}

	//--------------------------------------------------------------------------------------------------------------
	// ESEQ
	//--------------------------------------------------------------------------------------------------------------
	ESEQ::ESEQ(IStm* _stm, IExp* _exp): stm(_stm), exp(_exp) {}

	shared_ptr<ExpList> ESEQ::kids() {
		assert(0);
		return 0;
	}

	IExp* ESEQ::build(shared_ptr<ExpList> kids) {
		assert(0);
		return 0;
	}

	//--------------------------------------------------------------------------------------------------------------
	// MoveCall
	//--------------------------------------------------------------------------------------------------------------
	MoveCall::MoveCall(TEMP* _dst, CALL* _src) : dst(_dst), src(_src) {}

	shared_ptr<ExpList> MoveCall::kids() {
		return src->kids();
	}

	IStm* MoveCall::build(shared_ptr<ExpList> kids) {
		return new MOVE(dst, src->build(kids));
	}


	//--------------------------------------------------------------------------------------------------------------
	// ExpCall
	//--------------------------------------------------------------------------------------------------------------
	ExpCall::ExpCall(CALL* _call) : call(_call) {}

	shared_ptr<ExpList> ExpCall::kids() {
		return call->kids();
	}

	IStm* ExpCall::build(shared_ptr<ExpList> kids) {
		return new EXP(call->build(kids));
	}

	//--------------------------------------------------------------------------------------------------------------
	// StmExpList
	//--------------------------------------------------------------------------------------------------------------
	StmExpList::StmExpList(IStm* _stm, shared_ptr<ExpList> _exps) : stm(_stm), exps(_exps) {}

	//--------------------------------------------------------------------------------------------------------------
	// StmtList
	//--------------------------------------------------------------------------------------------------------------
	StmtList::StmtList(IStm* _head, shared_ptr<StmtList> _tail) : head(_head), tail(_tail) {}

	StmtList::StmtList(vector<IStm*>& list) {
		if (list.empty()) {
			tail = 0;
		} else {
			head = list[0];
			list.erase( list.begin() );
			tail = make_shared<StmtList>( list );
		}
	}

	void StmtList::toVector(vector<IStm*>& list){
		if ( head != 0 )
			list.push_back(head);
		if ( tail !=0 )
			tail->toVector( list );
	}
}
