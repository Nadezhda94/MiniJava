#ifndef CANONIZER_H_INCLUDED
#define CANONIZER_H_INCLUDED
#include "../IRVisitors/Visitor.h"

using namespace IRTree;

class Canonizer: public CIRVisitor {
public:
    virtual void Visit(MOVE* node);
    virtual void Visit(EXP* node);
    virtual void Visit(JUMP* node);
    virtual void Visit(CJUMP* node);
    virtual void Visit(SEQ* node);
    virtual void Visit(LABEL* node);
    virtual void Visit(CONST* node);
    virtual void Visit(NAME* node);
    virtual void Visit(TEMP* node);
    virtual void Visit(BINOP* node);
    virtual void Visit(MEM* node);
    virtual void Visit(CALL* node);
    virtual void Visit(ExpList* node, ExpList*& newNode);
    virtual void Visit(ESEQ* node);
    virtual void Visit(IRTree::MoveCall* node);
  	virtual void Visit(IRTree::ExpCall* node);

    INode* current_node;
};

#endif
