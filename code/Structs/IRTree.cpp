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
		if (memDst != 0) {
			return make_shared<ExpList>(memDst->exp, make_shared<ExpList>(src, nullptr));
		} else {
			return make_shared<ExpList>(src, nullptr);
		}
	}

	IStm* MOVE::build(shared_ptr<ExpList> kids) {
		const MEM* memDst = dynamic_cast<const MEM*>(dst);
		if (memDst != 0) {
			return new MOVE(new MEM(kids->head), kids->tail.get()->head);
		} else {
			return new MOVE(dst, kids->head);
		}
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
		if (exp == 0) {
			return 0;
		} else {
			return make_shared<ExpList>(exp, nullptr);
		}
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

	bool isNop( IStm* stm ) {
		EXP* exp = dynamic_cast<EXP*>(stm);
		return ( exp != 0 ) && ( dynamic_cast<const CONST*>(exp->exp) != 0 );
	}

	bool commute( IStm* stm, IExp* exp ) {
		return isNop( stm ) || ( dynamic_cast<EXP*>(exp) != 0 ) || ( dynamic_cast<const NAME*>(exp) != 0 );
	}

	IStm* seq( IStm* arg1, IStm* arg2 ) {
		if ( isNop( arg1 )) {
			return arg2;
		} else {
			if ( isNop( arg2 )) {
				return arg1;
			} else {
				//cerr << "SEQ" << endl;
				return new SEQ( arg1, arg2 );
			}
		}
	}

	StmExpList* reorder( shared_ptr<ExpList> list ) {
		if ( list == nullptr ) {
			return new StmExpList( new EXP( new CONST( 0 )), 0 );
		} else {
			IExp* head = list.get()->head;
			CALL* call = dynamic_cast<CALL*>(head);

			if ( call != 0 ) {

				shared_ptr<const Temp::CTemp> t = make_shared<const Temp::CTemp>();
				IExp* eseq = new ESEQ( new MOVE( new TEMP( t ), head ), new TEMP( t ));
				return reorder( make_shared<ExpList>( eseq, list->tail ));
			} else {
				ESEQ* eseq = doExp( head );
				StmExpList* stmtList = reorder( list->tail );
				if ( commute( stmtList->stm, eseq->exp )) {
					return new StmExpList( seq( eseq->stm, stmtList->stm ),
										   make_shared<ExpList>( eseq->exp, stmtList->exps )
					);
				} else {
					shared_ptr<const Temp::CTemp> t = make_shared<const Temp::CTemp>();
					return new StmExpList( seq( eseq->stm,
												seq( new MOVE( new TEMP( t ), eseq->exp ),
													 stmtList->stm
												)
										   ),
										   make_shared<ExpList>( new TEMP( t ), stmtList->exps )
					);
				}
			}
		}
	}

	ESEQ* reorderExp( IExp* exp ) {
		StmExpList* l = reorder( exp->kids());
		return new ESEQ( l->stm, exp->build( l->exps ));
	}

	IStm* reorderStm( IStm* stm ) {
		StmExpList* list = reorder( stm->kids());
		return seq( list->stm, stm->build( list->exps ));
	}

	ESEQ* doExp( IExp* exp ) {
		ESEQ* eseq = dynamic_cast<ESEQ*>(exp);
		if ( eseq != 0 ) {
			return doExp( eseq );
		} else {
			return reorderExp( exp );
		}
	}

	IStm* doStm( SEQ* s ) {
		return seq( doStm( s->left ), doStm( s->right ));
	}

	IStm* doStm( MOVE* s ) {
		TEMP* temp = dynamic_cast<TEMP*>(s->dst);
		CALL* call = dynamic_cast<CALL*>(s->src);
		if (( temp != 0 ) && ( call != 0 )) {
			return reorderStm( new MoveCall( temp, call ));
		} else {
			ESEQ* eseq = dynamic_cast<ESEQ*>(s->dst);
			if ( eseq != 0 ) {
				return doStm( new SEQ( eseq->stm, new MOVE( eseq->exp, s->src )));
			} else {
				return reorderStm( s );
			}
		}
	}

	IStm* doStm( EXP* s ) {
		CALL* call = dynamic_cast<CALL*>(s->exp);
		if ( call != 0 ) {
			return reorderStm( new ExpCall( call ));
		} else {
			return reorderStm( s );
		}
	}

	IStm* doStm( IStm* stm ) {
		SEQ* seq = dynamic_cast<SEQ*>(stm);
		if ( seq != 0 ) {
			//cerr << "seq" << endl;
			return doStm( seq );
		} else {
			MOVE* move = dynamic_cast<MOVE*>(stm);
			if ( move != 0 ) {
				return doStm( move );
			} else {
				EXP* exp = dynamic_cast<EXP*>(stm);
				if ( exp != 0 ) {
					return doStm( exp );

				} else {
					return reorderStm( stm );
				}
			}
		}
	}

	ESEQ* doExp( ESEQ* exp ) {
		if ( exp == 0 ) {
			exit( 1 );
		}

		IStm* stms = doStm( exp->stm );

		ESEQ* eseq = doExp( exp->exp );
		return new ESEQ( seq( stms, eseq->stm ), eseq->exp );
	}

	shared_ptr<StmtList> linear( IStm* s, shared_ptr<StmtList> l ) {
		SEQ* seq = dynamic_cast<SEQ*>(s);
		if ( seq != 0 ) {
			return linear( seq, l );
		} else {
			return make_shared<StmtList>( s, l );
		}
	}

	shared_ptr<StmtList> linear( SEQ* s, shared_ptr<StmtList> l ) {
		return linear( s->left, linear( s->right, l ));
	}

	shared_ptr<StmtList> linearize( IStm* s ) {
		return linear( doStm( s ), nullptr );
	}
}
