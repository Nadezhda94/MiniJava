#include "../Structs/Codegen.h"

using namespace IRTree;
using namespace Frame;

CCodegen::CCodegen(shared_ptr<CFrame> f): frame(f), instrList(0), last(0) {}
	

void CCodegen::MunchStm(IStm* s) {

}

void CCodegen::MunchStm(SEQ* s) {
	MunchStm(s->left);
	MunchStm(s->right);
}

shared_ptr<Temp::CTemp>  CCodegen::MunchExp(IExp* exp) {

}

CInstrList* CCodegen::Codegen(IStm* s) {
	CInstrList* l;
	MunchStm(s);
	l = instrList;
	last = 0;
	instrList = last;
	return l;
}

void CCodegen::emit(CInstr* instr) {
	if (last != nullptr) {
		last->tail = new CInstrList(instr, 0);
		last = last->tail;
	} else {
		instrList = new CInstrList(instr, 0);
		last = instrList;
	}
}