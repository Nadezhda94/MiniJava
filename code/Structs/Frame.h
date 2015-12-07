#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED
#include "../Structs/IRTree.h"
namespace Frame {
using namespace Temp;
using namespace IRTree;
// Переменная фрейма
class IAccess {
public:
    virtual IExp* getExp(CTemp* fp) const = 0;
    virtual ~IAccess() {}
};

class CFrameAccess : public IAccess {
public:
    CFrameAccess(int _offset) : offset(_offset){}
    IExp* getExp(CTemp* fp) const{
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
private:
    CTemp temp;
};

class CFrame {
// Класс-контейнер с платформо-зависимой информацией о функции
public:
    static const int wordSize = 4;
    CFrame( const Symbol::CSymbol* _name, int _formalsCount ):
        name(_name), formals(_formalsCount), localOffset(0), framePointer()
    {
        for (int i=0; i<_formalsCount; i++){
            formals[i] = new CFrameAccess(i*wordSize);
        }
    }

    int formalsCount() const {
        return formals.size();
    }

    CTemp& getFP(){
        return framePointer;
    }

    const IAccess* getFormal( size_t index ) const {
        return formals[index];
    }
    const IAccess* getLocal( size_t index ) const {
        return locals[index];
    }

    IAccess* allocLocal() {
        locals.push_back(new CFrameAccess(localOffset));
        localOffset -= wordSize;
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
    vector<IAccess*> formals;
    vector<IAccess*> locals;
};
}

#endif
