#ifndef CANONIZER_H_INCLUDED
#define CANONIZER_H_INCLUDED
#include "../IRVisitors/Printer.h"
#include "../Structs/IRTree.h"
#include "../IRVisitors/Visitor.h"

class CIRPrinter;

class Canonizer: public CIRVisitor {
public:
	CIRPrinter ir_print_vis;
	INode* current_node;

    virtual void visit(MOVE* node) {
        node->dst->accept(this);
        IExp* arg1 = dynamic_cast<IExp*>(current_node);
        node->src->accept(this);
        IExp* arg2 = dynamic_cast<IExp*>(current_node);
        current_node = (doStm(new MOVE(arg1, arg2)));
    }

    virtual void visit(EXP* node) {
        node->exp->accept(this);
        IExp* arg = dynamic_cast<IExp*>(current_node);
        current_node = doStm(new EXP(arg));
    }

    virtual void visit(JUMP* node) {
        if (node->exp != 0) {
            node->exp->accept(this);
            IExp* arg = dynamic_cast<IExp*>(current_node);
             current_node = doStm(new JUMP(arg, node->target));
        } else {
            current_node = doStm(new JUMP(node->target));
        }
    }

    virtual void visit(CJUMP* node) {
        node->left->accept(this);
        IExp* arg1 = dynamic_cast<IExp*>(current_node);
        node->right->accept(this);
        IExp* arg2 = dynamic_cast<IExp*>(current_node);
        current_node = doStm(new CJUMP(node->relop, arg1, arg2, node->iftrue, node->iffalse));
    }

    virtual void visit(SEQ* node) {
        node->left->accept(this);
        IStm* arg1 = dynamic_cast<IStm*>(current_node);
        node->right->accept(this);
        IStm* arg2 = dynamic_cast<IStm*>(current_node);
        current_node = doStm(new SEQ(arg1, arg2));
    }

    virtual void visit(LABEL* node) {
       	current_node = node;
    }

    virtual void visit(CONST* node) {
    	current_node = node;
    }

    virtual void visit(NAME* node){
    	current_node = node;
    }

    virtual void visit(TEMP* node) {
        current_node = node;
    }

    virtual void visit(BINOP* node) {
        node->left->accept(this);
        IExp* arg1 = dynamic_cast<IExp*>(current_node);
       	node->right->accept(this);
        IExp* arg2 = dynamic_cast<IExp*>(current_node);
        current_node = doExp(new BINOP(node->binop, arg1, arg2));
    }

    virtual void visit(MEM* node) {
        node->exp->accept(this);
        IExp* arg = dynamic_cast<IExp*>(current_node);
        current_node = doExp(new MEM(arg));
    }

    virtual void visit(CALL* node) {
        node->func->accept(this);
        IExp* func = dynamic_cast<IExp*>(current_node);
        IExp* arg = node->args.get()->head;

        ExpList* newNode = nullptr;
        if (node->args != nullptr) {
            visit(node->args.get(), newNode);
            shared_ptr<ExpList> ptr(newNode);
            current_node = doExp(new CALL(func, ptr));
        }
    }

    virtual void visit(ExpList* node, ExpList*& newNode) {
        if (node != nullptr){
            if (node->tail != nullptr){
                visit(node->tail.get(), newNode);
            }

            shared_ptr<ExpList> ptr(newNode);
            node->head->accept(this);
            IExp* arg = dynamic_cast<IExp*>(current_node);
            newNode = new ExpList(doExp(arg), ptr);
        } else {
            newNode = nullptr;
        }

    }


    virtual void visit(ESEQ* node)  {
        node->stm->accept(this);
        IStm* arg1 = dynamic_cast<IStm*>(current_node);
        node->exp->accept(this);

        IExp* arg2 = dynamic_cast<IExp*>(current_node);
        current_node = doExp(new ESEQ(arg1, arg2));

    }

    virtual void visit(IRTree::MoveCall* node) {
        assert(0);
    }

  	virtual void visit(IRTree::ExpCall* node) {
		assert(0);
  	}
};



#endif
