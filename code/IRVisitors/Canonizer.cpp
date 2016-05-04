#include "Canonizer.h"

void CCanonizer::Visit(MOVE* node) {
	node->dst->accept(this);
	IExp* arg1 = dynamic_cast<IExp*>(current_node);
	node->src->accept(this);
	IExp* arg2 = dynamic_cast<IExp*>(current_node);
	current_node = (doStm(new MOVE(arg1, arg2)));
}

void CCanonizer::Visit(EXP* node) {
	node->exp->accept(this);
	IExp* arg = dynamic_cast<IExp*>(current_node);
	current_node = doStm(new EXP(arg));
}

void CCanonizer::Visit(JUMP* node) {
	if (node->exp != 0) {
		node->exp->accept(this);
		IExp* arg = dynamic_cast<IExp*>(current_node);
		current_node = doStm(new JUMP(arg, node->target));
	} else {
		current_node = doStm(new JUMP(node->target));
	}
}

void CCanonizer::Visit(CJUMP* node) {
	node->left->accept(this);
	IExp* arg1 = dynamic_cast<IExp*>(current_node);
	node->right->accept(this);
	IExp* arg2 = dynamic_cast<IExp*>(current_node);
	current_node = doStm(new CJUMP(node->relop, arg1, arg2, node->iftrue, node->iffalse));
}

void CCanonizer::Visit(SEQ* node) {
	node->left->accept(this);
	IStm* arg1 = dynamic_cast<IStm*>(current_node);
	node->right->accept(this);
	IStm* arg2 = dynamic_cast<IStm*>(current_node);
	current_node = doStm(new SEQ(arg1, arg2));
}

void CCanonizer::Visit(LABEL* node) {
	current_node = node;
}

void CCanonizer::Visit(CONST* node) {
	current_node = node;
}

void CCanonizer::Visit(NAME* node){
	current_node = node;
}

void CCanonizer::Visit(TEMP* node) {
	current_node = node;
}

void CCanonizer::Visit(BINOP* node) {
	node->left->accept(this);
	IExp* arg1 = dynamic_cast<IExp*>(current_node);
	node->right->accept(this);
	IExp* arg2 = dynamic_cast<IExp*>(current_node);
	current_node = doExp(new BINOP(node->binop, arg1, arg2));
}

void CCanonizer::Visit(MEM* node) {
	node->exp->accept(this);
	IExp* arg = dynamic_cast<IExp*>(current_node);
	current_node = doExp(new MEM(arg));
}

void CCanonizer::Visit(CALL* node) {
	node->func->accept(this);
	IExp* func = dynamic_cast<IExp*>(current_node);
	IExp* arg = node->args.get()->head;

	ExpList* newNode = nullptr;
	if (node->args != nullptr) {
		Visit(node->args.get(), newNode);
		shared_ptr<ExpList> ptr(newNode);
		current_node = doExp(new CALL(func, ptr));
	}
}

void CCanonizer::Visit(ExpList* node, ExpList*& newNode) {
	if (node != nullptr){
		if (node->tail != nullptr){
			Visit(node->tail.get(), newNode);
		}

		shared_ptr<ExpList> ptr(newNode);
		node->head->accept(this);
		IExp* arg = dynamic_cast<IExp*>(current_node);
		newNode = new ExpList(doExp(arg), ptr);
	} else {
		newNode = nullptr;
	}

}

void CCanonizer::Visit(ESEQ* node)  {
	node->stm->accept(this);
	IStm* arg1 = dynamic_cast<IStm*>(current_node);
	node->exp->accept(this);

	IExp* arg2 = dynamic_cast<IExp*>(current_node);
	current_node = doExp(new ESEQ(arg1, arg2));

}

void CCanonizer::Visit(IRTree::MoveCall* node) {
	assert(0);
}

void CCanonizer::Visit(IRTree::ExpCall* node) {
	assert(0);
}

//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
bool isNop( IStm* stm ) {
	EXP* exp = dynamic_cast<EXP*>(stm);
	return ( exp != 0 ) && ( dynamic_cast<const CONST*>(exp->exp) != 0 );
}

bool commute( IStm* stm, IExp* exp ) {
	return isNop( stm ) || ( dynamic_cast<EXP*>(exp) != 0 ) || ( dynamic_cast<const NAME*>(exp) != 0 );
}

IStm* seq( IStm* arg1, IStm* arg2 ) {
	if (isNop(arg1)) return arg2;
	if (isNop(arg2)) return arg1;
	return new SEQ( arg1, arg2 );
}

// DoStm
IStm* doStm( SEQ* stm ) {
	return seq( doStm( stm->left ), doStm( stm->right ));
}

IStm* doStm( MOVE* stm ) {
	TEMP* temp = dynamic_cast<TEMP*>(stm->dst);
	CALL* call = dynamic_cast<CALL*>(stm->src);
	ESEQ* eseq = dynamic_cast<ESEQ*>(stm->dst);

	if (( temp != 0 ) && ( call != 0 ))
		return reorderStm( new MoveCall( temp, call ));
	if ( eseq != 0 )
		return doStm( new SEQ( eseq->stm, new MOVE( eseq->exp, stm->src )));
	return reorderStm( stm );
}

IStm* doStm( EXP* stm ) {
	CALL* call = dynamic_cast<CALL*>(stm->exp);

	if ( call != 0 )
		return reorderStm( new ExpCall( call ));
	return reorderStm( stm );
}

IStm* doStm( IStm* stm ) {
	SEQ* seq = dynamic_cast<SEQ*>(stm);
	MOVE* move = dynamic_cast<MOVE*>(stm);
	EXP* exp = dynamic_cast<EXP*>(stm);

	if ( seq != 0 ) return doStm( seq );
	if ( move != 0 ) return doStm( move );
	if ( exp != 0 ) return doStm( exp );
	return reorderStm( stm );
}

// DoExp
ESEQ* doExp( IExp* exp ) {
	ESEQ* eseq = dynamic_cast<ESEQ*>(exp);
	if ( eseq != 0 )
		return doExp( eseq );
	return reorderExp( exp );
}


ESEQ* doExp( ESEQ* exp ) {
	assert( exp != 0 );
	IStm* stms = doStm( exp->stm );
	ESEQ* eseq = doExp( exp->exp );
	return new ESEQ( seq( stms, eseq->stm ), eseq->exp );
}

// Reorder
StmExpList* reorder( shared_ptr<ExpList> list ) {
	if ( list == nullptr ) {
		return new StmExpList( new EXP( new CONST( 0 )), 0 );
	}
	IExp* head = list.get()->head;
	CALL* call = dynamic_cast<CALL*>(head);

	if ( call != 0 ) {
		shared_ptr<const Temp::CTemp> t = make_shared<const Temp::CTemp>();
		IExp* eseq = new ESEQ( new MOVE( new TEMP( t ), head ), new TEMP( t ));
		return reorder( make_shared<ExpList>( eseq, list->tail ));
	}

	ESEQ* eseq = doExp( head );
	StmExpList* stmtList = reorder( list->tail );
	if ( commute( stmtList->stm, eseq->exp )) {
		return new StmExpList( seq( eseq->stm, stmtList->stm ),
							   make_shared<ExpList>( eseq->exp, stmtList->exps ));
	} else {
		shared_ptr<const Temp::CTemp> t = make_shared<const Temp::CTemp>();
		return new StmExpList( seq( eseq->stm, seq( new MOVE( new TEMP( t ), eseq->exp ), stmtList->stm )),
							   make_shared<ExpList>( new TEMP( t ), stmtList->exps ));
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

// Linearize
shared_ptr<StmtList> linear( IStm* s, shared_ptr<StmtList> l ) {
	SEQ* seq = dynamic_cast<SEQ*>(s);
	if ( seq != 0 )
		return linear( seq, l );
	return make_shared<StmtList>( s, l );
}

shared_ptr<StmtList> linear( SEQ* s, shared_ptr<StmtList> l ) {
	return linear( s->left, linear( s->right, l ));
}

shared_ptr<StmtList> linearize( IStm* s ) {
	return linear( doStm( s ), nullptr );
}