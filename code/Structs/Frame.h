#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED
#include "../Structs/IRTree.h"
namespace Frame {
using namespace Temp;
using namespace IRTree;
class CFrame;
// Переменная фрейма
class IAccess {
public:
    virtual IExp* getExp() = 0;
    virtual ~IAccess() {}
};

class CFrameAccess : public IAccess {
public:
    CFrameAccess(CFrame* _frame, int _offset) : frame(_frame), offset(_offset){}
    IExp* getExp();
private:
    CFrame* frame;
    int offset;
};

class CRegAccess : public IAccess {
public:
    CRegAccess(): temp(){}
    IExp* getExp(){
        return new TEMP(&temp);
    }
private:
    CTemp temp;
};

class CVarAccess : public IAccess {
public:
    CVarAccess(CFrame* _frame, int _offset) : frame(_frame), offset(_offset){}
    IExp* getExp();
private:
    CFrame* frame;
    int offset;
};

class CFrame {
// Класс-контейнер с платформо-зависимой информацией о функции
public:
    static const int wordSize = 4;
    CFrame( const Symbol::CSymbol* _name):
        name(_name), localOffset(0), formalOffset(0), framePointer()
    {}

    CTemp* getFP(){
        return &framePointer;
    }

    IAccess* getTP(){
        return formals[0];
    }

    IAccess* getLocal(size_t index){
        return locals[index];
    }
    IAccess* getFormal(size_t index){
        return formals[index];
    }
    IAccess* getVar(size_t index){
        return vars[index];
    }

    void allocLocal() {
        locals.push_back(new CFrameAccess(this, localOffset));
        localOffset -= wordSize;
    }
    void allocFormal() {
        formals.push_back(new CFrameAccess(this, formalOffset));
        formalOffset += wordSize;
    }
    void allocVar() {
        vars.push_back(new CVarAccess(this, varOffset));
        varOffset += wordSize;
    }

    ~CFrame(){
        for (int i=0; i<formals.size(); i++)
            delete formals[i];
        for (int i=0; i<locals.size(); i++)
            delete locals[i];
        for (int i=0; i<vars.size(); i++)
            delete vars[i];
    }
private:
    const Symbol::CSymbol* name;
    CTemp framePointer;
    int localOffset;
    int formalOffset;
    int varOffset;
    vector<IAccess*> formals;
    vector<IAccess*> locals;
    vector<IAccess*> vars;

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

}

#endif
