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
      CVarInfo(const char* _name, const char* _type):
        name(_name), type(_type){}
  };

  struct CMethodInfo{
      string name;
      vector<CVarInfo> vars;
      vector<CVarInfo> params;
      CMethodInfo(const char* _name):
        name(_name), vars(), params(){}
  };

  struct CClassInfo{
      string name;
      vector<CVarInfo> vars;
      vector<CMethodInfo> methods;
      CClassInfo(const char* _name):
        name(_name), vars(), methods(){}
  };

  struct CTable{
      vector<CClassInfo> class_info;
      CTable(): class_info(){}
  };
}

#endif
