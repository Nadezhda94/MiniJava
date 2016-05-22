#include "../Structs/Codegen.h"

using namespace IRTree;
using namespace Frame;

CCodegen::CCodegen():  instrList(0), last(0) {}
	

void CCodegen::MunchStm(IStm* s) {
	SEQ* seq = dynamic_cast<SEQ*>(s);
	if ( seq != 0 ) {
		MunchStm(seq);
	} else {
		MOVE* move = dynamic_cast<MOVE*>(s);
		if (move != 0) {
			MunchMove(move->dst, move->src);
		} else {
			LABEL* label = dynamic_cast<LABEL*>(s);
			if (label != 0) {
				MunchStm(label);
			} else {
				EXP* exp = dynamic_cast<EXP*>(s);
				if (exp != 0) {
					CALL* call = dynamic_cast<CALL*>(exp->exp);
					if (call != 0) {
						MunchExpCall(call);
					}
				} else {
					JUMP* jmp = dynamic_cast<JUMP*>(s);
					if (jmp != 0) {
						emit(new AOPER("jmp `j0\n", 
								nullptr, 
								nullptr, 
								
								new CLabelList(jmp->target, nullptr)		)
							);
					}
					CJUMP* cjump = dynamic_cast<CJUMP*>(s);
					if (cjump != 0) {
						emit(new AOPER("cmp `s0, `s1\n", nullptr, 
										new CTempList( 
													MunchExp(cjump->left), 
													new CTempList(MunchExp(cjump->right), nullptr)
													)
										)
							);
						std::string jumpCmdName;
						if (cjump->relop == EQ) {
							jumpCmdName = "je";
						} else if (cjump->relop == LT) {
							jumpCmdName = "jl";
						}
						emit(new AOPER(jumpCmdName + "   `j0\n", nullptr, nullptr,
							new CLabelList(cjump->iftrue, new CLabelList(cjump->iffalse, nullptr))));
					}

				}
			}
		}
	}
}

void CCodegen::MunchExpCall(CALL* call) {
	NAME* name = dynamic_cast<NAME*>(call->func);
	if (name != 0) {
		CTempList* l = MunchArgs(call->args);
		emit(new AOPER("CALL " + name->label->Name() + "\n", CFrame::callDefs, l)); 
	} else {
		shared_ptr<const CTemp> r = MunchExp(call->func);
		CTempList* l = MunchArgs(call->args);
		emit(new AOPER("CALL `s0\n", CFrame::callDefs, new CTempList(r, l))); 
	}
}

CTempList* CCodegen::MunchArgs(shared_ptr<ExpList> args) {
	CTempList* l = nullptr;
	while(args != 0) {
		if (l != nullptr) {
			l->tail = new CTempList(std::make_shared<CTemp>(), nullptr);
			l = l->tail;
		} else {
			l = new CTempList(std::make_shared<CTemp>(), nullptr);
		}
		emit(new AOPER("push `s0\n", nullptr, new CTempList(MunchExp(args->head), nullptr)
						)
			);
		args = args->tail;
	}
}


/*munchStm(EXP(CALL(e,args)))
{Temp r = munchExp(e); TempList l = munchArgs(0,args);
emit(new OPER("CALL 's0\n",calldefs,L(r,l)));}*/

void CCodegen::MunchMove(IExp* dst, IExp* src) {
	MEM* mem = dynamic_cast<MEM*>(dst);
	if (mem != 0) {
		MunchMove(mem, src);
	} else {
		TEMP* temp = dynamic_cast<TEMP*>(dst);
		if (temp != 0)  {
			//MOVE(TEMP(i), e2)
			MunchMove(temp, src);
		}
	}
}

void CCodegen::MunchMove(MEM* dst, IExp* src) {
	
	BINOP* binop = dynamic_cast<BINOP*>(dst->exp);
	if (binop != 0) {

		CONST* cst = dynamic_cast<CONST*>(binop->right);
		if (cst != 0) {
			//MOVE(MEM(BINOP(PLUS,e1,CONST(i))),e2)

			emit(new AOPER("mov [`s0 " + CCodegen::opSymbols[binop->binop] + std::to_string(cst->value) + "],	`s1\n", 
				nullptr, new CTempList( MunchExp(binop->left), 
										new CTempList(MunchExp(src), nullptr)
										)
							)
				);
			return;
		} 

		cst = dynamic_cast<CONST*>(binop->left);
		if (cst != 0) {
			//MOVE(MEM(BINOP(PLUS,CONST(i),e1)),e2)
			emit(new AOPER("mov [`s0 " + CCodegen::opSymbols[binop->binop] + std::to_string(cst->value) + "],	`s1\n", 
			nullptr, new CTempList( MunchExp(binop->right), 
									new CTempList(MunchExp(src), nullptr)
									)
						)
			);
			return;
		}

	
	}

	//???
	MEM* mem = dynamic_cast<MEM*>(src);
	if (mem != 0) {
		emit(new AMOVE("mov `d0, `s0\n", MunchExp(dst->exp), MunchExp(mem->exp)
					  )
			);
		return;
	}
	CONST* cst = dynamic_cast<CONST*>(dst->exp);
	if (cst != 0) {
		emit(new AOPER("mov [" + std::to_string(cst->value) + "], `s0\n", nullptr, 
						new CTempList( MunchExp(src), nullptr)));
		return;
	}

	emit(new AMOVE("mov `d0, `s0\n", MunchExp(dst->exp), MunchExp(src) 
				  )
		);

}


void CCodegen::MunchMove(TEMP* dst, IExp* src) {
	//MOVE(TEMP(i), e2)
	//???
	emit(new AOPER("mov 	`d0, `s0\n", new CTempList(dst->temp, nullptr), 
		new CTempList(MunchExp(src), nullptr)));
}

void CCodegen::MunchStm(SEQ* s) {
	MunchStm(s->left);
	MunchStm(s->right);
}


void CCodegen::MunchStm(LABEL* label) {
	emit(new ALABEL(label->label->Name() + ":\n", label->label));
}

shared_ptr<const Temp::CTemp>  CCodegen::MunchExp(IExp* exp) {
	MEM* mem = dynamic_cast<MEM*>(exp);
	if ( mem != 0) {
		BINOP* binop = dynamic_cast<BINOP*>(mem->exp);
		
			CONST* cst = dynamic_cast<CONST*>(binop->right);
			if (cst != 0) {
				//MEM(BINOP(PLUS,e1,CONST(i)))
				shared_ptr<const CTemp> r = make_shared<const CTemp>();
				emit(new AOPER("mov `d0, [`s0 " + CCodegen::opSymbols[binop->binop] + std::to_string(cst->value) + "]\n",
								new CTempList(r, nullptr), new CTempList(MunchExp(binop->left), nullptr)
								)
					);
				return r;
			} 
			cst = dynamic_cast<CONST*>(binop->left);
			if (cst != 0 ) {
				//MEM(BINOP(PLUS,CONST(i),e1))
				shared_ptr<const CTemp> r = make_shared<const CTemp>();
				emit(new AOPER("mov `d0, [`s0 " + CCodegen::opSymbols[binop->binop] + std::to_string(cst->value) + "]\n",
								new CTempList(r, nullptr), new CTempList(MunchExp(binop->right), nullptr)
								)
					);
				return r;
			}
		
		cst = dynamic_cast<CONST*>(mem->exp);
		if (cst != 0) {
			//MEM(CONST(i))
			shared_ptr<const CTemp> r = make_shared<const CTemp>();
			emit(new AOPER("mov `d0, [" + std::to_string(cst->value) + "]\n", 
					new CTempList(r, nullptr), nullptr));
			return r; 
		}
		shared_ptr<const CTemp> r = make_shared<const CTemp>();
		//MEM(e1)
		//?????
		emit(new AMOVE("mov `d0, `s0\n", r, MunchExp(mem->exp) 
						)
			);
		return r;
	}

	BINOP* binop = dynamic_cast<BINOP*>(exp);
	if ( (binop != 0) ) {
		std::cerr << "A djn z Z!" << std::endl;
		return MunchBinop(binop->left, binop->right, binop->binop);
	}
	CONST* cst = dynamic_cast<CONST*>(exp);
	if (cst != 0) {
		//CONST(i)
		shared_ptr<const CTemp> r = make_shared<const CTemp>();
		emit(new AOPER("mov `d0, " + std::to_string(cst->value) + "\n", new CTempList(r, nullptr), nullptr )
			);
		return r;
	}
	TEMP* t = dynamic_cast<TEMP*>(exp);
	if (t != 0) {
		std::cerr << "temp";
		return t->temp;
	}
	CALL* call = dynamic_cast<CALL*>(exp);
	if (call != 0) {
		MunchExpCall(call);
		return make_shared<const CTemp>("ecx");
	}
	



}

shared_ptr<const Temp::CTemp> CCodegen::MunchBinop(
	CONST* cst,
 	IExp* exp, 
 	ArithmeticOpType binop) 
{
	
	shared_ptr<const CTemp> r = make_shared<const CTemp>();
	emit(new AMOVE("move `d0, `s0\n", r, MunchExp(exp)
					)
		);
	emit(new AOPER(CCodegen::opNames[binop] + " `d0, " + std::to_string(cst->value) + "\n", new CTempList(r, nullptr),
						new CTempList(r, nullptr)
					)
		);
	return r;
}

shared_ptr<const Temp::CTemp> CCodegen::MunchBinop(
	IRTree::IExp* src, IRTree::IExp* exp, 
	ArithmeticOpType binop) 
{
	CONST* cst = dynamic_cast<CONST*>(src);
	if (cst != 0) {
		return MunchBinop(cst, exp, binop);
	} 
	cst = dynamic_cast<CONST*>(exp);
	if (cst != 0){
		return MunchBinop(cst, src, binop);
	}
	//BINOP(PLUS,e1,e2)
	shared_ptr<const CTemp> r = make_shared<const CTemp>();
	emit(new AMOVE("mov `d0, `s0\n", r, MunchExp(src)
					)
		);
	emit(new AOPER(CCodegen::opNames[binop] + " `d0, `s1\n", new CTempList(r, nullptr),
						new CTempList(r, new CTempList( MunchExp(exp), nullptr ))
					)
		);
	return r;
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


std::vector<std::string> CCodegen::initOpNames() {
	std::vector<std::string> names;
	names.push_back("add");
	names.push_back("sub");
	names.push_back("mul");
	names.push_back("div");
	return names;
}
std::vector<std::string> CCodegen::initOpSymbols() {
	std::vector<std::string> names;
	names.push_back("+");
	names.push_back("-");
	names.push_back("*");
	names.push_back("/");
	return names;
}

std::vector<std::string> CCodegen::opNames = CCodegen::initOpNames();
std::vector<std::string> CCodegen::opSymbols = CCodegen::initOpSymbols();