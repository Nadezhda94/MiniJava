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

	shared_ptr<const CTemp> CInstr::getTemp(CTempList* l, int tempNumber) {
		if (l == 0) {
			std::cerr << tempNumber << std::endl;
			//return  std::make_shared<CTemp>();
		}
		if (tempNumber == 0) {
			return l->head;
		} else {
			if (l->tail == 0) {
				std::cerr << tempNumber << std::endl;
			}
			return getTemp(l->tail, tempNumber - 1);
		}
	}

	const CLabel* CInstr::getLabel(CLabelList* l, int tempNumber) {
		if (l == 0) {
			//std::cerr << tempNumber << std::endl;
			//return new CLabel();
		}
		if (tempNumber == 0) {
			return l->head;
		} else {
			if (l->tail == 0) {
				//std::cerr << tempNumber << std::endl;
			}
			return getLabel(l->tail, tempNumber - 1);
		}
	}

	std::string CInstr::format(CTempMap* m) {
		CTempList* dst = def();
		CTempList* src = use();
		CTargets* j = jumps();
		CLabelList* jump = ( j == nullptr) ? nullptr : j->labels;
		std::string s;
		//std::cerr << assemCmd << std::endl;
		int len = assemCmd.length();
		for (int i = 0; i < len; ++i) {
			if (assemCmd[i] == '`') {

				int n = 0;
				switch(assemCmd[++i]) {
					case 's':
						n = std::stoi(&assemCmd[++i]);
						s.append(m->tempMap(getTemp(src, n)));
						break;
					case 'd':
						n = std::stoi(&assemCmd[++i]);
						s.append(m->tempMap(getTemp(dst, n)));
						break;
					case 'j':
						n = std::stoi(&assemCmd[++i]);
						s.append(getLabel(jump, n)->Name());
						break;
					case '`':
						s.append("`");
						break;
					default:
						std::cerr << "ERROR" << std::endl;
						std::cerr << assemCmd << std::endl;
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
	
	ALABEL::ALABEL(const std::string& a, const CLabel* l) : CInstr(a), label(l) {}

	CTempList* ALABEL::use() {
		return nullptr;
	}

	CTempList* ALABEL::def() {
		return nullptr;
	}

	CTargets* ALABEL::jumps() {
		return nullptr;
	}

	void ALABEL::Print(ostream& s){
		s<<"ALABEL";
	}


	//--------------------------------------------------------------------------------------------------------------
	// AMOVE
	//--------------------------------------------------------------------------------------------------------------
	
	AMOVE::AMOVE(const std::string& a, shared_ptr<const CTemp> d, shared_ptr<const CTemp> s) : CInstr(a), dst(d), src(s) {}	

	CTempList* AMOVE::use() {
		return new CTempList(src, nullptr);
	}

	CTempList* AMOVE::def() {
		return new CTempList(dst, nullptr);
	}
	CTargets* AMOVE::jumps() {
		return nullptr;
	}

	void AMOVE::Print(ostream& s){
		s<<"AMOVE";
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

	void AOPER::Print(ostream& s){
		s<<"AOPER";
	}

}