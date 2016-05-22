#ifndef CODEGEN_H_INCLUDED
#define CODEGEN_H_INCLUDED
#include "../Structs/IRTree.h"
#include "../Structs/Assembler.h"
#include "../Structs/Frame.h"

using namespace Assembler;

class CCodegen {
public:


	CCodegen();
	

	void MunchStm(IRTree::IStm* s);

	void MunchStm(IRTree::SEQ* s);


	void MunchMove(IRTree::IExp* dst, IRTree::IExp* src);
	void MunchMove(IRTree::MEM* dst, IRTree::IExp* src);
	void MunchMove(IRTree::TEMP* dst, IRTree::IExp* src);
	void MunchStm(IRTree::LABEL* label);
	void MunchExpCall(IRTree::CALL* call);
	CTempList* MunchArgs(shared_ptr<ExpList>);

	shared_ptr<const Temp::CTemp>  MunchExp(IRTree::IExp* exp);
	shared_ptr<const Temp::CTemp> MunchBinop(
		IRTree::CONST* cst, 
		IRTree::IExp* exp, 
		ArithmeticOpType binop);
	shared_ptr<const Temp::CTemp> MunchBinop(
		IRTree::IExp* src, 
		IRTree::IExp* exp, 
		ArithmeticOpType binop);

	CInstrList* Codegen(IRTree::IStm* s);
private:
	CInstrList* instrList;
	CInstrList* last;
	static std::vector<std::string> opNames;
	static std::vector<std::string> opSymbols;
	static std::vector<std::string> initOpNames();
	static std::vector<std::string> initOpSymbols();
	void emit(CInstr* instr);
};



#endif