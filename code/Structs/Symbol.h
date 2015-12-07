#ifndef SYMBOL_H_INCLUDED
#define SYMBOL_H_INCLUDED
#include "../common.h"

namespace Symbol {
class CStorage;

class CSymbol{
public:
	CSymbol(const std::string& _name): name(_name) {}
	const std::string& getString() const {
		return name;
	}

	friend std::ostream& operator<<(std::ostream& ostr, const CSymbol* symbol) {
		ostr << symbol->name;
		return ostr;
	}
private:
	std::string name;
	static CStorage* symbolsStorage;
};

class CStorage {
public:
	const CSymbol* get(const std::string& s) {
		std::unordered_map<std::string, std::unique_ptr<CSymbol>>::iterator it = symbols.find(s);
		if (it == symbols.end() ) {
			it = symbols.emplace(s, std::unique_ptr<CSymbol>(new CSymbol(s))).first;
		}
		return (it->second).get();
	}
	std::unordered_map<std::string, std::unique_ptr<CSymbol>> symbols;
};

}

#endif
