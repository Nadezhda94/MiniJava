#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED
#include "../Structs/IRTree.h"
namespace Frame {
using namespace Temp;
using namespace IRTree;
// Переменная фрейма
class IAccess {
public:
    virtual IExp* getExp(CTemp* fp) = 0;
    virtual ~IAccess() {}
};

class CFrameAccess : public IAccess {
public:
    CFrameAccess(int _offset) : offset(_offset){}
    IExp* getExp(CTemp* fp){
        return new MEM(new BINOP(ArithmeticOpType::PLUS_OP,
            static_cast<IExp*>(new TEMP(fp)),
            static_cast<IExp*>(new CONST(offset))));
    }
private:
    int offset;
};

class CRegAccess : public IAccess {
public:
    CRegAccess(): temp(){}
    IExp* getExp(CTemp* fp){
        return new TEMP(&temp);
    }
private:
    CTemp temp;
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

    IAccess* getLocal(size_t index){
        return locals[index];
    }
    IAccess* getFormal(size_t index){
        return formals[index];
    }

    void allocLocal() {
        locals.push_back(new CFrameAccess(localOffset));
        localOffset -= wordSize;
    }
    void allocFormal() {
        formals.push_back(new CFrameAccess(formalOffset));
        formalOffset += wordSize;
    }

    ~CFrame(){
        for (int i=0; i<formals.size(); i++)
            delete formals[i];
        for (int i=0; i<locals.size(); i++)
            delete locals[i];
    }
private:
    const Symbol::CSymbol* name;
    CTemp framePointer;
    int localOffset;
    int formalOffset;
    vector<IAccess*> formals;
    vector<IAccess*> locals;
};
}

#endif
