#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED
#include "../Structs/Temp.h"
namespace Frame {
using namespace Temp;
// Переменная фрейма
class IAccess {
public:
    virtual ~IAccess() {}
};

class CFrameAccess : public IAccess {
public:
    CFrameAccess(int _offset) : offset(_offset){}
private:
    int offset;
};

class CRegAccess : public IAccess {
public:
    CRegAccess(): temp(){}
private:
    CTemp temp;
};

// Класс-контейнер с платформо-зависимой информацией о функции
class CFrame {
public:
    static const int wordSize = 4;
    CFrame( const Symbol::CSymbol* _name, int _formalsCount ):
        name(_name), formals(_formalsCount), local_offset(0){
        for (int i=0; i<_formalsCount; i++){
            formals[i] = new CFrameAccess(i*wordSize);
        }
    }

    int formalsCount() const {
        return formals.size();
    }

    const IAccess* getFormal( size_t index ) const {
        return formals[index];
    }
    const IAccess* getLocal( size_t index ) const {
        return locals[index];
    }

    IAccess* allocLocal() {
        locals.push_back(new CFrameAccess(local_offset));
        local_offset -= wordSize;
    }

    ~CFrame(){
        for (int i=0; i<formals.size(); i++)
            delete formals[i];
        for (int i=0; i<locals.size(); i++)
            delete locals[i];
    }
private:
    int local_offset;
    vector<IAccess*> formals;
    vector<IAccess*> locals;
    const Symbol::CSymbol* name;
};
}

#endif
