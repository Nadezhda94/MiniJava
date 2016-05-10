#ifndef PRINTIRVISITOR_H_INCLUDED
#define PRINTIRVISITOR_H_INCLUDED
#include "../common.h"
#include "../IRVisitors/Visitor.h"
using namespace IRTree;

class CIRPrinter: public CIRVisitor {
public:
	~CIRPrinter();
	CIRPrinter(ostream& _out, ostream& _gv, bool _withoutSEQ = false);
	void Visit(MOVE* node);
	void Visit(EXP* node);
	void Visit(JUMP* node);
	void Visit(CJUMP* node);
	void Visit(SEQ* node);
	void Visit(LABEL* node);
	void Visit(CONST* node);
	void Visit(NAME* node);
	void Visit(TEMP* node);
	void Visit(BINOP* node);
	void Visit(MEM* node);
	void Visit(CALL* node);
	void Visit(ESEQ* node);
	void Visit(IRTree::MoveCall* node);
	void Visit(IRTree::ExpCall* node);
private:
	ostream& gv;
	ostream& out;
	bool withoutSEQ;
	int counter = 0;
	void print_tabs(int num);
};

#endif
