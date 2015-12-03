#ifndef IRTREE_H_INCLUDED
#define IRTREE_H_INCLUDED
#include "../Structs/Temp.h"
#include "../Structs/Ast.h"

namespace IRTree {

enum CJUMP_OP{
	EQ, NE, LT, GT, LE, GE, ULT, ULE, UGT, UGE
};

class INode{

};

class IExp : public INode {

};

class IStm : public INode {

};

class ExpList;

class ExpList {
public:
	ExpList(IExp* _head, ExpList* _tail) : head(_head), tail(_tail) {}
private:
	IExp* head;
	ExpList* tail;
};

class MOVE: public IStm {
public:
	MOVE(IExp* _dst, IExp* _src): dst(_dst), src(_src) {}
private:
	IExp* dst;
	IExp* src;
};

class EXP: public IStm {
public:
	EXP(IExp* _exp): exp(_exp) {}
private:
	IExp* exp;
};


class JUMP: public IStm {
public:
	JUMP(IExp* _exp, Temp::CLabel* _target) : exp(_exp), target(_target) {}
private:
	IExp* exp;
	Temp::CLabel* target;
};

class CJUMP: public IStm {
public:
	CJUMP(CJUMP_OP _relop, IExp* _left, IExp* _right, Temp::CLabel* _iftrue, Temp::CLabel* _iffalse):
						relop(_relop), left(_left), right(_right), iftrue(_iftrue), iffalse(_iffalse) {}
private:
	CJUMP_OP relop;
	IExp* left;
	IExp* right;
	Temp::CLabel* iftrue;
	Temp::CLabel* iffalse;
};

class SEQ: public IStm {
public:
	SEQ(IStm* _left, IStm* _right): left(_left), right(_right) {}
private:
	IStm* left;
	IStm* right;
};

class LABEL: public IStm {
public:
	LABEL(Temp::CLabel* _label):label(_label) {}
private:
	Temp::CLabel* label;
};


class CONST: public IExp {
public:
	CONST(int _value): value(_value){}
private:
	int value;
};

class NAME : public IExp {
public:
	NAME(LABEL* _label): label(_label) {}
private:
	LABEL* label;
};

class TEMP: public IExp {
public:
	TEMP(Temp::CTemp* _temp): temp(_temp) {}
private:
	Temp::CTemp* temp;
};

class BINOP: public IExp {
public:
	BINOP(ArithmeticOpType _binop, IExp* _left, IExp* _right): binop(_binop), left(_left), right(_right) {}
private:
	ArithmeticOpType binop;
	IExp* left;
	IExp* right;
};

class MEM: public IExp {
public:
	MEM(IExp* _exp): exp(_exp) {}
private:
	IExp* exp;
};

class CALL: public IExp {
public:
	CALL(IExp* _func, ExpList* _args): func(_func), args(_args) {}
private:
	IExp* func;
	ExpList* args;
};

class ESEQ: public IExp {
public:
	ESEQ(IStm* _stm, IExp* _exp): stm(_stm), exp(_exp) {}
private:
	IStm* stm;
	IExp* exp;
};

}

#endif
