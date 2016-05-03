#include "Symbol.h"

namespace Symbol {
	CSymbol::CSymbol(const std::string& _name): name(_name) {}

	const std::string& CSymbol::getString() const {
		return name;
	}

	const CSymbol* CStorage::get(const std::string& s) {
		std::unordered_map<std::string, std::unique_ptr<CSymbol>>::iterator it = symbols.find(s);
		if (it == symbols.end() ) {
			it = symbols.emplace(s, std::unique_ptr<CSymbol>(new CSymbol(s))).first;
		}
		return (it->second).get();
	}
}

void storagePrinter(ostream& out, Symbol::CStorage& symbolsStorage) {
	out << "________________Storage_____________" << std::endl;
	auto it = symbolsStorage.symbols.begin();
	auto itEnd = symbolsStorage.symbols.end();
	for (; it !=itEnd; ++it) {
		out << it->first << " " << it->second.get() << std::endl;
	}
}