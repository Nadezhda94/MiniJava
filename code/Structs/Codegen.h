#ifndef CODEGEN_H_INCLUDED
#define CODEGEN_H_INCLUDED
#include "IRTree.h"
#include "Assembler.h"
#include "../Structs/Frame.h"

using namespace Assembler;

class CCodegen {
public:
	shared_ptr<Frame::CFrame> frame;

	CCodegen(shared_ptr<Frame::CFrame> f);
	

	void MunchStm(IRTree::IStm* s);

	void MunchStm(IRTree::SEQ* s);

	shared_ptr<Temp::CTemp>  MunchExp(IRTree::IExp* exp);

	CInstrList* Codegen(IRTree::IStm* s);
private:
	CInstrList* instrList;
	CInstrList* last;

	void emit(CInstr* instr);
};



#endif