#ifndef PRINTIRVISITOR_H_INCLUDED
#define PRINTIRVISITOR_H_INCLUDED
#include "../common.h"
#include "../Structs/IRTree.h"
#include "../IRVisitors/Visitor.h"
using namespace IRTree;
static const char * ArithmeticOpStrings[] = { "+", "-", "*", "/", "&&", "||", "<<", ">>", "!" };
static const char * CJumpOpStrings[] = { "=", "!=", "<", ">", "<=", ">=", "u<", "u<=", "u>", "u>=" };

class CIRPrinter: public CIRVisitor {
	bool withoutSEQ;
	int counter = 0;
	void print_tabs(int num) {
		for (int i = 0; i < num; i++)
			cout << "| ";
	}
public:
	CIRPrinter() {}
	CIRPrinter(bool _withoutSEQ) : withoutSEQ(_withoutSEQ) {}
	virtual void visit(MOVE* node) {
		print_tabs(counter++);
		cout << "MOVE" << endl;
		node->dst->accept(this);
		node->src->accept(this);
		--counter;
	}

	virtual void visit(EXP* node) {
		print_tabs(counter++);
		cout << "EXP" << endl;
		node->exp->accept(this);
		--counter;
	}

	virtual void visit(JUMP* node) {
		print_tabs(counter++);
		cout << "JUMP "<< node->target->Name() << endl;
		if (node->exp != 0)
			node->exp->accept(this);
		--counter;
	}

	virtual void visit(CJUMP* node) {
		print_tabs(counter++);
		cout << "CJUMP " << CJumpOpStrings[node->relop] << " " <<
			 node->iftrue->Name()<< " " << node->iffalse->Name() << " " << endl;
		node->left->accept(this);
		node->right->accept(this);
		--counter;
	}

	virtual void visit(SEQ* node) {
		if (!withoutSEQ) {
			print_tabs(counter++);
			cout << "SEQ" << endl;
		}
		node->left->accept(this);
		node->right->accept(this);
		if (!withoutSEQ)
			--counter;
	}

	virtual void visit(LABEL* node) {
		print_tabs(counter++);
		cout << "LABEL "<< node->label->Name() << endl;
		--counter;
	}

	virtual void visit(CONST* node) {
		print_tabs(counter++);
		cout << "CONST " << node->value << endl;
		--counter;
	}

	virtual void visit(NAME* node) {
		print_tabs(counter++);
		cout << "NAME " << node->label->Name() << endl;
		--counter;
	}

	virtual void visit(TEMP* node) {
		print_tabs(counter++);
		cout << "TEMP " << node->temp->Name() << endl;
		//node->temp->accept(this);
		--counter;
	}

	virtual void visit(BINOP* node) {
		print_tabs(counter++);
		cout << "BINOP " << ArithmeticOpStrings[node->binop] << endl;
		node->left->accept(this);
		node->right->accept(this);
		--counter;
	}

	virtual void visit(MEM* node) {
		print_tabs(counter++);
		cout << "MEM" << endl;
		node->exp->accept(this);
		--counter;
	}

	virtual void visit(CALL* node) {
		print_tabs(counter++);
		cout << "CALL" << endl;
		node->func->accept(this);
		shared_ptr<ExpList> cur = node->args;
		while(cur) {
			cur->head->accept(this);
			cur = cur->tail;
		}
		--counter;
	}

	virtual void visit(ESEQ* node) {
		print_tabs(counter++);
		cout << "ESEQ" << endl;
		node->stm->accept(this);
		node->exp->accept(this);
		--counter;
	}

	virtual void visit(IRTree::MoveCall* node) {
		assert(0);
	}
	virtual void visit(IRTree::ExpCall* node) {
		assert(0);
	}
};

#endif
