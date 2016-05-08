#include "../Structs/Assembler.h"


namespace Assembler {
	//--------------------------------------------------------------------------------------------------------------
	// CTargets
	//--------------------------------------------------------------------------------------------------------------
	
	CTargets::CTargets(CLabelList* _labels) : labels(_labels) {}

	//--------------------------------------------------------------------------------------------------------------
	// CInstr
	//--------------------------------------------------------------------------------------------------------------
	
	CInstr::CInstr(const std::string& a) : assemCmd(a) {}

	shared_ptr<CTemp> CInstr::getTemp(CTempList* l, int tempNumber) {
		if (tempNumber == 0) {
			return l->head;
		} else {
			return getTemp(l->tail, tempNumber - 1);
		}
	}

	CLabel* CInstr::getLabel(CLabelList* l, int tempNumber) {
		if (tempNumber == 0) {
			return l->head;
		} else {
			return getLabel(l->tail, tempNumber - 1);
		}
	}

	std::string CInstr::format(CTempMap* m) {
		CTempList* dst = def();
		CTempList* src = use();
		CTargets* j = jumps();
		CLabelList* jump = ( j == nullptr) ? nullptr : j->labels;
		std::string s;
		int len = assemCmd.length();
		for (int i = 0; i < len; ++i) {
			if (assemCmd[i] = '`') {
				int n = std::stoi(&assemCmd[++i]);
				switch(assemCmd[++i]) {
					case 's':
						s.append(m->tempMap(getTemp(src, n)));
						break;
					case 'd':
						s.append(m->tempMap(getTemp(dst, n)));
						break;
					case 'j':
						s.append(getLabel(jump, n)->Name());
						break;
					case '`':
						s.append("`");
						break;
					default:
						throw new std::invalid_argument("assembler syntax error");
				}
			} else {
				s += assemCmd[i];
			}
		}
		return s;
	}

	//--------------------------------------------------------------------------------------------------------------
	// CInstrList
	//--------------------------------------------------------------------------------------------------------------
	
	CInstrList::CInstrList(CInstr* _head, CInstrList* _tail): head(_head), tail(_tail) {}

	//--------------------------------------------------------------------------------------------------------------
	// ALABEL
	//--------------------------------------------------------------------------------------------------------------
	
	ALABEL::ALABEL(const std::string& a, CLabel* l) : CInstr(a), label(l) {}

	CTempList* ALABEL::use() {
		return nullptr;
	}

	CTempList* ALABEL::def() {
		return nullptr;
	}

	CTargets* ALABEL::jumps() {
		return nullptr;
	}

	//--------------------------------------------------------------------------------------------------------------
	// AMOVE
	//--------------------------------------------------------------------------------------------------------------
	
	AMOVE::AMOVE(const std::string& a, shared_ptr<CTemp> d, shared_ptr<CTemp> s) : CInstr(a), dst(d), src(s) {}	

	CTempList* AMOVE::use() {
		return new CTempList(src, nullptr);
	}

	CTempList* AMOVE::def() {
		return new CTempList(dst, nullptr);
	}
	CTargets* AMOVE::jumps() {
		return nullptr;
	}

	//--------------------------------------------------------------------------------------------------------------
	// AOPER
	//--------------------------------------------------------------------------------------------------------------
	
	AOPER::AOPER(const std::string& a, CTempList* d, CTempList* s, CLabelList* j) 
	: CInstr(a), dst(d), src(s), jump(new CTargets(j)) {}

	AOPER::AOPER(const std::string& a, CTempList* d, CTempList* s) 
	: CInstr(a), dst(d), src(s), jump(nullptr) {}


	CTempList* AOPER::use() {
		return src;
	}

	CTempList* AOPER::def() {
		return dst;
	}

	CTargets* AOPER::jumps() {
		return jump;
	}

}