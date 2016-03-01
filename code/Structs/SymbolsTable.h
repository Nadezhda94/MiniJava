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
    int getLocalIndex(const CSymbol* name){
        for (int i = 0; i < vars.size(); i++)
            if (name == vars[i].name)
                return i;
        throw new std::out_of_range("Local not found");
    }
    int getFormalIndex(const CSymbol* name){
        for (int i = 0; i < params.size(); i++)
            if (name == params[i].name)
                return i;
        throw new std::out_of_range("Formal not found");
    }
};

struct CClassInfo{
    const CSymbol* name;
    const CSymbol* parent;
    vector<CVarInfo> vars;
    vector<CMethodInfo> methods;
    CClassInfo(const CSymbol* _name):
        name(_name), vars(), methods(){}
    CMethodInfo& getMethodInfo(const CSymbol* name){
        for (int i = 0; i < methods.size(); i++)
            if (name->getString() == methods[i].name->getString())
                return methods[i];
    }
    int getVarIndex(const CSymbol* name){
        for (int i = 0; i < vars.size(); i++)
            if (name == vars[i].name)
                return i;
        throw new std::out_of_range("Var in class not found");
    }
};

struct CTable{
    vector<CClassInfo> classInfo;
    CTable(): classInfo(){}
    CClassInfo& getClassInfo(const CSymbol* name){
        for (int i = 0; i < classInfo.size(); i++)
            if (name->getString() == classInfo[i].name->getString())
                return classInfo[i];
    }
};
}

#endif
