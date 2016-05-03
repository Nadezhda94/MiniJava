#ifndef SYMBOL_H_INCLUDED
#define SYMBOL_H_INCLUDED
#include "../common.h"

namespace Symbol {
class CStorage;

class CSymbol {
public:
	CSymbol(const std::string& _name);
	const std::string& getString() const;

	friend std::ostream& operator<<(std::ostream& ostr, const CSymbol* symbol) {
		ostr << symbol->name;
		return ostr;
	}

	std::string name;
	static CStorage* symbolsStorage;
};

class CStorage {
public:
	const CSymbol* get(const std::string& s);

	std::unordered_map<std::string, std::unique_ptr<CSymbol>> symbols;
};

}

#endif
