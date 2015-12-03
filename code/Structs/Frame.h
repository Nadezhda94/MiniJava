#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED
namespace Frame {
// Переменная фрейма
class IAccess {
public:
    virtual ~IAccess() {}
};

// Класс-контейнер с платформо-зависимой информацией о функции
class CFrame {
public:
    CFrame( const Symbol::CSymbol* _name, int _formalsCount ):
        name(_name), formalsCount(_formalsCount){}
    // Доступ к формальным параметрам
    int FormalsCount() const { return formals.size(); }
    const IAccess* Formal( size_t index ) const;

private:
    const Symbol::CSymbol* name;
    int formalsCount;
};
}

#endif
