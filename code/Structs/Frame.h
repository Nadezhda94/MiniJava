#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED
#include "../Structs/IRTree.h"
#include "../Structs/Temp.h"
namespace Frame {
using namespace Temp;
using namespace IRTree;
class CFrame;
// Переменная фрейма
class IAccess {
public:
    virtual IExp* getExp() = 0;
    virtual const CSymbol* getName() = 0;
    virtual ~IAccess() {}
};

class CFrameAccess : public IAccess {
public:
    CFrameAccess(const CSymbol* _name, CFrame* _frame, int _offset) :
        name(_name), frame(_frame), offset(_offset){}
    IExp* getExp();
    const CSymbol* getName() { return name; }
private:
    const CSymbol* name;
    CFrame* frame;
    int offset;
};

class CRegAccess : public IAccess {
public:
    CRegAccess(const CSymbol* _name): name(_name), temp(new CTemp()){}
    IExp* getExp();
    const CSymbol* getName() { return name; }
private:
    const CSymbol* name;
    shared_ptr<CTemp> temp;
};

class CVarAccess : public IAccess {
public:
    CVarAccess(const CSymbol* _name, CFrame* _frame, int _offset) :
        name(_name), frame(_frame), offset(_offset){}
    IExp* getExp();
    const CSymbol* getName() { return name; }
private:
    const CSymbol* name;
    CFrame* frame;
    int offset;
};

class CFrame {
// Класс-контейнер с платформо-зависимой информацией о функции
public:
    static const int wordSize = 4;
    CFrame( const Symbol::CSymbol* _name):
        name(_name), localOffset(0), formalOffset(0), framePointer(new CTemp())
    {}

    shared_ptr<CTemp> getFP(){
        return framePointer;
    }

    shared_ptr<IAccess> getTP(){
        return formals[0];
    }

    shared_ptr<IAccess> getLocal(const CSymbol* name){
        for (int i = 0; i < locals.size(); i++)
            if (name == locals[i]->getName())
                return locals[i];
        return 0;
    }
    shared_ptr<IAccess> getFormal(const CSymbol* name){
        for (int i = 0; i < formals.size(); i++)
            if (name == formals[i]->getName())
                return formals[i];
        return 0;
    }

    shared_ptr<IAccess> getVar(const CSymbol* name){
        for (int i = 0; i < vars.size(); i++)
            if (name == vars[i]->getName())
                return vars[i];
        return 0;
    }

    IExp* findByName(const CSymbol* name){
		shared_ptr<IAccess> l = getLocal(name);
        shared_ptr<IAccess> f = getFormal(name);
        shared_ptr<IAccess> v = getVar(name);
        if (l!=0) return l->getExp();
        if (f!=0) return f->getExp();
        if (v!=0) return v->getExp();
		throw new std::out_of_range("Ident not found in findByName");
	}

    void allocLocal(const CSymbol* name) {
        locals.push_back(shared_ptr<IAccess>(new CFrameAccess(name, this, localOffset)));
        localOffset -= wordSize;
    }
    void allocFormal(const CSymbol* name) {
        formals.push_back(shared_ptr<IAccess>(new CFrameAccess(name, this, formalOffset)));
        formalOffset += wordSize;
    }
    void allocVar(const CSymbol* name) {
        vars.push_back(shared_ptr<IAccess>(new CVarAccess(name, this, varOffset)));
        varOffset += wordSize;
    }

    const IExp* externalCall(const std::string& funcName, shared_ptr<ExpList> args) {
        return new CALL(new NAME(shared_ptr<CLabel>(new CLabel(funcName))), args);
    }

    ~CFrame(){}
private:
    const Symbol::CSymbol* name;
    shared_ptr<CTemp> framePointer;
    int localOffset;
    int formalOffset;
    int varOffset;
    vector<shared_ptr<IAccess>> formals;
    vector<shared_ptr<IAccess>> locals;
    vector<shared_ptr<IAccess>> vars;

};

IExp* CFrameAccess::getExp(){
    return new MEM(new BINOP(ArithmeticOpType::PLUS_OP,
        static_cast<IExp*>(new TEMP(frame->getFP())),
        static_cast<IExp*>(new CONST(offset))));
}

IExp* CVarAccess::getExp(){
    return new MEM(new BINOP(ArithmeticOpType::PLUS_OP,
        static_cast<IExp*>(new MEM(frame->getTP()->getExp())),
        static_cast<IExp*>(new CONST(offset))));
}

IExp* CRegAccess::getExp(){
    return new TEMP(temp);
}

}

#endif
