#include "Canonizer.h"

void Canonizer::Visit(MOVE* node) {
	node->dst->accept(this);
	IExp* arg1 = dynamic_cast<IExp*>(current_node);
	node->src->accept(this);
	IExp* arg2 = dynamic_cast<IExp*>(current_node);
	current_node = (doStm(new MOVE(arg1, arg2)));
}

void Canonizer::Visit(EXP* node) {
	node->exp->accept(this);
	IExp* arg = dynamic_cast<IExp*>(current_node);
	current_node = doStm(new EXP(arg));
}

void Canonizer::Visit(JUMP* node) {
	if (node->exp != 0) {
		node->exp->accept(this);
		IExp* arg = dynamic_cast<IExp*>(current_node);
		current_node = doStm(new JUMP(arg, node->target));
	} else {
		current_node = doStm(new JUMP(node->target));
	}
}

void Canonizer::Visit(CJUMP* node) {
	node->left->accept(this);
	IExp* arg1 = dynamic_cast<IExp*>(current_node);
	node->right->accept(this);
	IExp* arg2 = dynamic_cast<IExp*>(current_node);
	current_node = doStm(new CJUMP(node->relop, arg1, arg2, node->iftrue, node->iffalse));
}

void Canonizer::Visit(SEQ* node) {
	node->left->accept(this);
	IStm* arg1 = dynamic_cast<IStm*>(current_node);
	node->right->accept(this);
	IStm* arg2 = dynamic_cast<IStm*>(current_node);
	current_node = doStm(new SEQ(arg1, arg2));
}

void Canonizer::Visit(LABEL* node) {
	current_node = node;
}

void Canonizer::Visit(CONST* node) {
	current_node = node;
}

void Canonizer::Visit(NAME* node){
	current_node = node;
}

void Canonizer::Visit(TEMP* node) {
	current_node = node;
}

void Canonizer::Visit(BINOP* node) {
	node->left->accept(this);
	IExp* arg1 = dynamic_cast<IExp*>(current_node);
	node->right->accept(this);
	IExp* arg2 = dynamic_cast<IExp*>(current_node);
	current_node = doExp(new BINOP(node->binop, arg1, arg2));
}

void Canonizer::Visit(MEM* node) {
	node->exp->accept(this);
	IExp* arg = dynamic_cast<IExp*>(current_node);
	current_node = doExp(new MEM(arg));
}

void Canonizer::Visit(CALL* node) {
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

void Canonizer::Visit(ExpList* node, ExpList*& newNode) {
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

void Canonizer::Visit(ESEQ* node)  {
	node->stm->accept(this);
	IStm* arg1 = dynamic_cast<IStm*>(current_node);
	node->exp->accept(this);

	IExp* arg2 = dynamic_cast<IExp*>(current_node);
	current_node = doExp(new ESEQ(arg1, arg2));

}

void Canonizer::Visit(IRTree::MoveCall* node) {
	assert(0);
}

void Canonizer::Visit(IRTree::ExpCall* node) {
	assert(0);
}
