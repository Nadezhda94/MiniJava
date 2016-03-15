#ifndef IRTREE_H_INCLUDED
#define IRTREE_H_INCLUDED
#include "../Structs/Temp.h"
#include "../Structs/Ast.h"
#include "../Visitors/IRVisitor.h"

namespace IRTree {

enum CJUMP_OP{
	EQ, NE, LT, GT, LE, GE, ULT, ULE, UGT, UGE
};

class INode{
public:
	virtual void accept(const CIRVisitor* visitor) const = 0;
	virtual ~INode(){}
};

template<class TARGET, class INTERFACE>
class CAcceptsIRVisitor : public INTERFACE {
public:
    virtual void accept(const CIRVisitor* visitor) const {
        visitor->visit( static_cast<const TARGET*> (this) );
    }
};

class IExp : public INode {
};

class IStm : public INode {
};

class ExpList;

class ExpList {
public:
	ExpList(const IExp* _head, const ExpList* _tail) : head(_head), tail(_tail) {}
private:
	const IExp* head;
	const ExpList* tail;
};

class MOVE: public CAcceptsIRVisitor<MOVE, IStm> {
public:
	MOVE(const IExp* _dst, const IExp* _src): dst(_dst), src(_src) {}
	const IExp* dst;
	const IExp* src;
};

class EXP: public CAcceptsIRVisitor<EXP, IStm> {
public:
	EXP(const IExp* _exp): exp(_exp) {}
	const IExp* exp;
};


class JUMP: public CAcceptsIRVisitor<JUMP, IStm> {
public:
	JUMP(const IExp* _exp, const Temp::CLabel* _target) : exp(_exp), target(_target) {}
	const IExp* exp;
	const Temp::CLabel* target;
};

class CJUMP: public CAcceptsIRVisitor<CJUMP, IStm> {
public:
	CJUMP(CJUMP_OP _relop, const IExp* _left, const IExp* _right, const Temp::CLabel* _iftrue, const Temp::CLabel* _iffalse):
						relop(_relop), left(_left), right(_right), iftrue(_iftrue), iffalse(_iffalse) {}
	CJUMP_OP relop;
	const IExp* left;
	const IExp* right;
	const Temp::CLabel* iftrue;
	const Temp::CLabel* iffalse;
};

class SEQ: public CAcceptsIRVisitor<SEQ, IStm> {
public:
	SEQ(const IStm* _left, const IStm* _right): left(_left), right(_right) {}
	const IStm* left;
	const IStm* right;
};

class LABEL: public CAcceptsIRVisitor<LABEL, IStm> {
public:
	LABEL(const Temp::CLabel* _label):label(_label) {}
	const Temp::CLabel* label;
};


class CONST: public CAcceptsIRVisitor<CONST, IExp> {
public:
	CONST(int _value): value(_value){}
	int value;
};

class NAME : public CAcceptsIRVisitor<NAME, IExp> {
public:
	NAME(LABEL* _label): label(_label) {}
	LABEL* label;
};

class TEMP: public CAcceptsIRVisitor<TEMP, IExp> {
public:
	TEMP(const Temp::CTemp* _temp): temp(_temp) {}
	const Temp::CTemp* temp;
};

class BINOP: public CAcceptsIRVisitor<BINOP, IExp> {
public:
	BINOP(ArithmeticOpType _binop, const IExp* _left, const IExp* _right): binop(_binop), left(_left), right(_right) {}
	ArithmeticOpType binop;
	const IExp* left;
	const IExp* right;
};

class MEM: public CAcceptsIRVisitor<MEM, IExp> {
public:
	MEM(const IExp* _exp): exp(_exp) {}
	const IExp* exp;
};

class CALL: public CAcceptsIRVisitor<CALL, IExp> {
public:
	CALL(const IExp* _func, const ExpList* _args): func(_func), args(_args) {}
	const IExp* func;
	const ExpList* args;
};

class ESEQ: public CAcceptsIRVisitor<ESEQ, IExp> {
public:
	ESEQ(const IStm* _stm, const IExp* _exp): stm(_stm), exp(_exp) {}
	const IStm* stm;
	const IExp* exp;
};

};

#endif
