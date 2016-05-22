#include "Frame.h"

namespace Frame {

	IExp* CFrameAccess::getExp() {
		return new MEM( new BINOP( ArithmeticOpType::PLUS_OP,
								   static_cast<IExp*>(new TEMP( frame->getFP())),
								   static_cast<IExp*>(new CONST( offset ))));
	}

	IExp* CVarAccess::getExp() {
		return new MEM( new BINOP( ArithmeticOpType::PLUS_OP,
								   static_cast<IExp*>( frame->getTP()->getExp()),
								   static_cast<IExp*>(new CONST( offset ))));
	}

	IExp* CRegAccess::getExp() {
		return new TEMP( temp );
	}

	CFrame::CFrame( const Symbol::CSymbol* _name):
			name(_name), varOffset(0), localOffset(0), formalOffset(-4), framePointer(new CTemp()) {
		//TODO: заполнить регистры
	}

	const std::string& CFrame::tempMap(shared_ptr<const CTemp> t) {
		return t->Name();
	}

	shared_ptr<CTemp> CFrame::getFP() {
		return framePointer;
	}

	shared_ptr<IAccess> CFrame::getTP() {
		return formals[0];
	}

	shared_ptr<IAccess> CFrame::getLocal(const CSymbol* name) {
		for (int i = 0; i < locals.size(); i++)
			if (name == locals[i]->getName())
				return locals[i];
		return 0;
	}
	shared_ptr<IAccess> CFrame::getFormal(const CSymbol* name) {
		for (int i = 0; i < formals.size(); i++)
			if (name == formals[i]->getName())
				return formals[i];
		return 0;
	}

	shared_ptr<IAccess> CFrame::getVar(const CSymbol* name) {
		for (int i = 0; i < vars.size(); i++)
			if (name == vars[i]->getName())
				return vars[i];
		return 0;
	}

	IExp* CFrame::findByName(const CSymbol* name) {
		shared_ptr<IAccess> l = getLocal(name);
		shared_ptr<IAccess> f = getFormal(name);
		shared_ptr<IAccess> v = getVar(name);
		if (l!=0) return l->getExp();
		if (f!=0) return f->getExp();
		if (v!=0) return v->getExp();
		throw new std::out_of_range("Ident not found in findByName");
	}

	void CFrame::allocLocal(const CSymbol* name) {
		locals.push_back(shared_ptr<IAccess>(new CFrameAccess(name, this, localOffset)));
		localOffset += wordSize;
	}
	void CFrame::allocFormal(const CSymbol* name) {
		formals.push_back(shared_ptr<IAccess>(new CFrameAccess(name, this, formalOffset)));
		formalOffset -= wordSize;
	}
	void CFrame::allocVar(const CSymbol* name) {
		vars.push_back(shared_ptr<IAccess>(new CVarAccess(name, this, varOffset)));
		varOffset += wordSize;
	}

	IExp* CFrame::externalCall(const std::string& funcName, shared_ptr<ExpList> args) {
		return new CALL(new NAME(shared_ptr<CLabel>(new CLabel(funcName))), args);
	}

	CTempList* CFrame::callDefsInit() {
		CTempList* l = new CTempList(std::make_shared<CTemp>("esp"), 
										new CTempList( std::make_shared<CTemp>("ebp"), 
														new CTempList(std::make_shared<CTemp>("ecx"), 
															nullptr)) );

	}

	CTempList* CFrame::registersInit() {
		CTempList* l = new CTempList(std::make_shared<const CTemp>("eax"), 
										new CTempList( std::make_shared<const CTemp>("ebx"), 
														new CTempList(std::make_shared<const CTemp>("ecx"), 
																		new CTempList(std::make_shared<const CTemp>("edx"), 
																			nullptr))) );

	}

	CTempList* CFrame::callDefs = CFrame::callDefsInit();

	CTempList* CFrame::registers = CFrame::registersInit();
}