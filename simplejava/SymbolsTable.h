#ifndef SYMBOLSTABLE_H_INCLUDED
#define SYMBOLSTABLE_H_INCLUDED
#include <string>
#include <vector>

namespace SymbolsTable{
  using std::vector;
  using std::string;
  struct CVarInfo{
      string name;
      string type;
      CVarInfo(const string& _name, const string& _type):
        name(_name), type(_type){}
  };

  struct CMethodInfo{
      string name;
      vector<CVarInfo> vars;
      vector<CVarInfo> params;
      string returnType;
      CMethodInfo(const string& _name, const string& _returnType):
        name(_name), returnType(_returnType), vars(), params(){}
  };

  struct CClassInfo{
      string name;
	  string parent;
      vector<CVarInfo> vars;
      vector<CMethodInfo> methods;
      CClassInfo(const string& _name):
        name(_name), vars(), methods(){}
  };

  struct CTable{
      vector<CClassInfo> classInfo;
      CTable(): classInfo(){}
  };
}

#endif
