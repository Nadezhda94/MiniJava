#ifndef IRVISITOR_H_INCLUDED
#define IRVISITOR_H_INCLUDED

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
	int count = 0;
	virtual void Visit(IRTree::MOVE* node) = 0;
	virtual void Visit(IRTree::EXP* node) = 0;
	virtual void Visit(IRTree::JUMP* node) = 0;
	virtual void Visit(IRTree::CJUMP* node) = 0;
	virtual void Visit(IRTree::SEQ* node) = 0;
	virtual void Visit(IRTree::LABEL* node) = 0;
	virtual void Visit(IRTree::CONST* node)= 0;
	virtual void Visit(IRTree::NAME* node) = 0;
	virtual void Visit(IRTree::TEMP* node) = 0;
	virtual void Visit(IRTree::BINOP* node) = 0;
	virtual void Visit(IRTree::MEM* node)= 0;
	virtual void Visit(IRTree::CALL* node) = 0;
	virtual void Visit(IRTree::ESEQ* node) = 0;
	virtual void Visit(IRTree::MoveCall* node) = 0;
	virtual void Visit(IRTree::ExpCall* node) = 0;
};

#include "../Structs/IRTree.h"

#endif
