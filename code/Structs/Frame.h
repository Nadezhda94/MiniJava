#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED

#include "../Structs/IRTree.h"
#include "../Structs/Temp.h"
#include "../Structs/TempMap.h"

using namespace Temp;
using namespace IRTree;
namespace Frame {
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
		name(_name), frame(_frame), offset(_offset) {}
	IExp* getExp();
	const CSymbol* getName() {
		return name;
	}
private:
	const CSymbol* name;
	CFrame* frame;
	int offset;
};

class CRegAccess : public IAccess {
public:
	CRegAccess(const CSymbol* _name): name(_name), temp(new CTemp()) {}
	IExp* getExp();
	const CSymbol* getName() {
		return name;
	}
private:
	const CSymbol* name;
	shared_ptr<CTemp> temp;
};

class CVarAccess : public IAccess {
public:
	CVarAccess(const CSymbol* _name, CFrame* _frame, int _offset) :
		name(_name), frame(_frame), offset(_offset) {}
	IExp* getExp();
	const CSymbol* getName() {
		return name;
	}
private:
	const CSymbol* name;
	CFrame* frame;
	int offset;
};

class CFrame: public CTempMap {
// Класс-контейнер с платформо-зависимой информацией о функции
public:
	static const int wordSize = 4;
	static CTempList* callDefs;
	/*static*/CTempList* registers;
	CFrame( const Symbol::CSymbol* _name);
	/*static*/CTempList* Registers();
	/*static*/const std::string& tempMap(shared_ptr<const CTemp> t);
	shared_ptr<CTemp> getFP();
	shared_ptr<IAccess> getTP();
	shared_ptr<IAccess> getLocal(const CSymbol* name);
	shared_ptr<IAccess> getFormal(const CSymbol* name);
	shared_ptr<IAccess> getVar(const CSymbol* name);
	IExp* findByName(const CSymbol* name);
	void allocLocal(const CSymbol* name);
	void allocFormal(const CSymbol* name);
	void allocVar(const CSymbol* name);
	IExp* externalCall(const std::string& funcName, shared_ptr<ExpList> args);
	~CFrame() {}
private:
	static CTempList* callDefsInit();

	const Symbol::CSymbol* name;
	shared_ptr<CTemp> framePointer;
	
	
	int localOffset;
	int formalOffset;
	int varOffset;
	vector<shared_ptr<IAccess>> formals;
	vector<shared_ptr<IAccess>> locals;
	vector<shared_ptr<IAccess>> vars;
};

}

#endif
