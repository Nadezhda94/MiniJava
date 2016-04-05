#ifndef CANONIZER_H_INCLUDED
#define CANONIZER_H_INCLUDED
#include "../IRVisitors/Printer.h"
#include "../Structs/IRTree.h"
#include "../IRVisitors/Visitor.h"

class CIRPrinter;

class Canonizer: public CIRVisitor {
public:
	CIRPrinter ir_print_vis;
	mutable const INode* current_node;
	virtual void visit(const MOVE* node) const {
		node->dst->accept(this);
		const IExp* arg1 = dynamic_cast<const IExp*>(current_node);
		node->src->accept(this);
		const IExp* arg2 = dynamic_cast<const IExp*>(current_node);
		current_node = (doStm(new MOVE(arg1, arg2)));
	}

	virtual void visit(const EXP* node) const {
		node->exp->accept(this);
		const IExp* arg = dynamic_cast<const IExp*>(current_node);
		current_node = doStm(new EXP(arg));
	}

	virtual void visit(const JUMP* node) const {
		if (node->exp != 0) {
			node->exp->accept(this);
			const IExp* arg = dynamic_cast<const IExp*>(current_node);
			current_node = doStm(new JUMP(arg, node->target));
		} else {
			current_node = doStm(new JUMP(node->target));
		}



	}

	virtual void visit(const CJUMP* node) const {
		node->left->accept(this);
		const IExp* arg1 = dynamic_cast<const IExp*>(current_node);
		node->right->accept(this);
		const IExp* arg2 = dynamic_cast<const IExp*>(current_node);
		current_node = doStm(new CJUMP(node->relop, arg1, arg2, node->iftrue, node->iffalse));
	}

	virtual void visit(const SEQ* node) const {
		node->left->accept(this);
		const IStm* arg1 = dynamic_cast<const IStm*>(current_node);
		node->right->accept(this);
		const IStm* arg2 = dynamic_cast<const IStm*>(current_node);
		current_node = doStm(new SEQ(arg1, arg2));
	}

	virtual void visit(const LABEL* node) const {
		current_node = node;
	}

	virtual void visit(const CONST* node) const {
		current_node = node;
	}

	virtual void visit(const NAME* node) const {
		current_node = node;
	}

	virtual void visit(const TEMP* node) const {
		current_node = node;
	}

	virtual void visit(const BINOP* node) const {
		node->left->accept(this);
		const IExp* arg1 = dynamic_cast<const IExp*>(current_node);
		node->right->accept(this);
		const IExp* arg2 = dynamic_cast<const IExp*>(current_node);
		current_node = doExp(new BINOP(node->binop, arg1, arg2));
	}

	virtual void visit(const MEM* node) const {
		node->exp->accept(this);
		const IExp* arg = dynamic_cast<const IExp*>(current_node);
		current_node = doExp(new MEM(arg));
	}

	virtual void visit(const CALL* node) const {
		node->func->accept(this);
		const IExp* func = dynamic_cast<const IExp*>(current_node);
		const IExp* arg = node->args.get()->head;

		const ExpList* newNode = nullptr;
		if (node->args != nullptr) {
			visit(node->args.get(), newNode);

			shared_ptr<const ExpList> ptr(newNode);

			current_node = doExp(new CALL(func, ptr));
		}

	}

	virtual void visit(const ExpList* node, const ExpList*& newNode) const {
		if (node != nullptr) {
			if (node->tail != nullptr) {
				visit(node->tail.get(), newNode);
			}

			shared_ptr<const ExpList> ptr(newNode);
			node->head->accept(this);
			const IExp* arg = dynamic_cast<const IExp*>(current_node);
			newNode = new ExpList(doExp(arg), ptr);
		} else {
			newNode = nullptr;
		}

	}


	virtual void visit(const ESEQ* node) const {
		node->stm->accept(this);
		const IStm* arg1 = dynamic_cast<const IStm*>(current_node);
		node->exp->accept(this);

		const IExp* arg2 = dynamic_cast<const IExp*>(current_node);
		current_node = doExp(new ESEQ(arg1, arg2));

	}

	virtual void visit(const IRTree::MoveCall* node)const {

	}

	virtual void visit(const IRTree::ExpCall* node)const {

	}

private:
};



#endif