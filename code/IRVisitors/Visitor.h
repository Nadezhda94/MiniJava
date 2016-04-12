#ifndef IRVISITOR_H_INCLUDED
#define IRVISITOR_H_INCLUDED

#include "../Structs/IRTree.h"

namespace IRTree {
	class MOVE;
	class EXP;
	class JUMP;
	class CJUMP;
	class SEQ;
	class LABEL;
	class CONST;
	class NAME;
	class TEMP;
	class BINOP;
	class MEM;
	class CALL;
	class ESEQ;
	class MoveCall;
	class ExpCall;
};

class CIRVisitor {
public:
	virtual void visit(IRTree::MOVE* node) = 0;
	virtual void visit(IRTree::EXP* node) = 0;
	virtual void visit(IRTree::JUMP* node) = 0;
	virtual void visit(IRTree::CJUMP* node) = 0;
	virtual void visit(IRTree::SEQ* node) = 0;
	virtual void visit(IRTree::LABEL* node) = 0;
	virtual void visit(IRTree::CONST* node)= 0;
	virtual void visit(IRTree::NAME* node) = 0;
	virtual void visit(IRTree::TEMP* node) = 0;
	virtual void visit(IRTree::BINOP* node) = 0;
	virtual void visit(IRTree::MEM* node)= 0;
	virtual void visit(IRTree::CALL* node) = 0;
	virtual void visit(IRTree::ESEQ* node) = 0;
	virtual void visit(IRTree::MoveCall* node) = 0;
	virtual void visit(IRTree::ExpCall* node) = 0;
};

#endif
