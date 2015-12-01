#ifndef SYMBOLSTABLE_H_INCLUDED
#define SYMBOLSTABLE_H_INCLUDED
#include "../common.h"
#include "../Structs/Symbol.h"

namespace SymbolsTable{
  using std::vector;
  using std::string;

  struct CVarInfo{
      const CSymbol* name;
      const CSymbol* type;
      CVarInfo(const CSymbol* _name, const CSymbol* _type):
        name(_name), type(_type){}
  };

  struct CMethodInfo{
      const CSymbol* name;
      vector<CVarInfo> vars;
      vector<CVarInfo> params;
      const CSymbol* returnType;
      CMethodInfo(const CSymbol* _name, const CSymbol* _returnType):
        name(_name), returnType(_returnType), vars(), params(){}
  };

  struct CClassInfo{
      const CSymbol* name;
      const CSymbol* parent;
      vector<CVarInfo> vars;
      vector<CMethodInfo> methods;
      CClassInfo(const CSymbol* _name):
        name(_name), vars(), methods(){}
  };

  struct CTable{
      vector<CClassInfo> classInfo;
      CTable(): classInfo(){}
  };
}

#endif
