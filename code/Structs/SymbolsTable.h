#ifndef SYMBOLSTABLE_H_INCLUDED
#define SYMBOLSTABLE_H_INCLUDED
#include "../common.h"
#include "../Structs/Symbol.h"

using namespace Symbol;
namespace SymbolsTable {

struct CVarInfo {
	CVarInfo(const CSymbol* _name, const CSymbol* _type);

	const CSymbol* name;
	const CSymbol* type;
};

struct CMethodInfo {
	CMethodInfo(const CSymbol* _name, const CSymbol* _returnType);

	const CSymbol* name;
	vector<CVarInfo> vars;
	vector<CVarInfo> params;
	const CSymbol* returnType;
};

struct CClassInfo {
	CClassInfo(const CSymbol* _name);
	CMethodInfo& getMethodInfo(const CSymbol* name);

	const CSymbol* name;
	const CSymbol* parent;
	vector<CVarInfo> vars;
	vector<CMethodInfo> methods;
};

struct CTable {
	CTable();
	CClassInfo& getClassInfo(const CSymbol* name);
	vector<CVarInfo> getParentVars(const CSymbol* name);

	vector<CClassInfo> classInfo;
};

}

#endif
