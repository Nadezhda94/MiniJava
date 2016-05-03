#ifndef IRTREE_H_INCLUDED
#define IRTREE_H_INCLUDED
#include "../Structs/Temp.h"
#include "../Structs/Ast.h"
#include "../IRVisitors/Visitor.h"

namespace IRTree {

enum CJUMP_OP {
	EQ, NE, LT, GT, LE, GE, ULT, ULE, UGT, UGE
};

class INode {
public:
	virtual void accept(CIRVisitor* Visitor) = 0;
	virtual ~INode() {}
};

template<class TARGET, class INTERFACE>
class CAcceptsIRVisitor : public INTERFACE {
public:
	virtual void accept(CIRVisitor* visitor) {
		visitor->Visit( static_cast<TARGET*> (this) );
	}
};

class ExpList;

class IExp : public INode {
public:
	virtual  shared_ptr<ExpList> kids() = 0;
	virtual  IExp* build(shared_ptr<ExpList> kids) = 0;
};

class IStm : public INode {
public:
	virtual  shared_ptr<ExpList> kids() = 0;
	virtual  IStm* build(shared_ptr<ExpList> kids) = 0;
};

struct ExpList {
	ExpList(IExp* _head, shared_ptr<ExpList> _tail) : head(_head), tail(_tail) {}
	IExp* head;
	shared_ptr<ExpList> tail;
};

struct StmtList {
	StmtList(IStm* _head, shared_ptr<StmtList> _tail) : head(_head), tail(_tail) {}
	IStm* head;
	shared_ptr<StmtList> tail;
};

struct StmExpList {
	StmExpList(IStm* _stm, shared_ptr<ExpList> _exps);
	IStm* stm;
	shared_ptr<ExpList> exps;
};

class MEM: public CAcceptsIRVisitor<MEM, IExp> {
public:
	MEM(IExp* _exp);
	shared_ptr<ExpList> kids();
	IExp* build(shared_ptr<ExpList> kids);

	IExp* exp;
};

class MOVE: public CAcceptsIRVisitor<MOVE, IStm> {
public:
	MOVE(IExp* _dst, IExp* _src);
	shared_ptr<ExpList> kids();
	IStm* build(shared_ptr<ExpList> kids);

	IExp* dst;
	IExp* src;
};

class EXP: public CAcceptsIRVisitor<EXP, IStm> {
public:
	EXP(IExp* _exp);
	shared_ptr<ExpList> kids();
	IStm* build(shared_ptr<ExpList> kids);

	IExp* exp;
};


class JUMP: public CAcceptsIRVisitor<JUMP, IStm> {
public:
	JUMP(IExp* _exp, const Temp::CLabel* _target);
	JUMP(const Temp::CLabel* _target);
	shared_ptr<ExpList> kids();
	IStm* build(shared_ptr<ExpList> kids);

	IExp* exp;
	const Temp::CLabel* target;
};

class CJUMP: public CAcceptsIRVisitor<CJUMP, IStm> {
public:
	CJUMP(CJUMP_OP _relop, IExp* _left, IExp* _right, const Temp::CLabel* _iftrue, const Temp::CLabel* _iffalse);
	shared_ptr<ExpList> kids();
	IStm* build(shared_ptr<ExpList> kids);

	CJUMP_OP relop;
	IExp* left;
	IExp* right;
	const Temp::CLabel* iftrue;
	const Temp::CLabel* iffalse;
};

class SEQ: public CAcceptsIRVisitor<SEQ, IStm> {
public:
	SEQ(IStm* _left, IStm* _right);
	shared_ptr<ExpList> kids();
	IStm* build(shared_ptr<ExpList> kids);

	IStm* left;
	IStm* right;
};

class LABEL: public CAcceptsIRVisitor<LABEL, IStm> {
public:
	LABEL(const Temp::CLabel* _label);
	shared_ptr<ExpList> kids();
	IStm* build(shared_ptr<ExpList> kids);

	const Temp::CLabel* label;
};


class CONST: public CAcceptsIRVisitor<CONST, IExp> {
public:
	CONST(int _value);
	shared_ptr<ExpList> kids();
	IExp* build(shared_ptr<ExpList> kids);

	int value;
};

class NAME : public CAcceptsIRVisitor<NAME, IExp> {
public:
	NAME(shared_ptr<Temp::CLabel> _label);
	shared_ptr<ExpList> kids();
	IExp* build(shared_ptr<ExpList> kids);

	shared_ptr<Temp::CLabel> label;
};

class TEMP: public CAcceptsIRVisitor<TEMP, IExp> {
public:
	TEMP(shared_ptr<const Temp::CTemp> _temp);
	shared_ptr<ExpList> kids();
	IExp* build(shared_ptr<ExpList> kids);

	shared_ptr<const Temp::CTemp> temp;
};

class BINOP: public CAcceptsIRVisitor<BINOP, IExp> {
public:
	BINOP(ArithmeticOpType _binop, IExp* _left, IExp* _right);
	shared_ptr<ExpList> kids();
	IExp* build(shared_ptr<ExpList> kids);

	ArithmeticOpType binop;
	IExp* left;
	IExp* right;
};

class CALL: public CAcceptsIRVisitor<CALL, IExp> {
public:
	CALL(IExp* _func, shared_ptr<ExpList> _args);
	shared_ptr<ExpList> kids();
	IExp* build(shared_ptr<ExpList> kids);

	IExp* func;
	shared_ptr<ExpList> args;
};

class ESEQ: public CAcceptsIRVisitor<ESEQ, IExp> {
public:
	ESEQ(IStm* _stm, IExp* _exp);
	shared_ptr<ExpList> kids();
	IExp* build(shared_ptr<ExpList> kids);

	IStm* stm;
	IExp* exp;
};

class MoveCall: public CAcceptsIRVisitor<MoveCall, IStm> {
public:
	MoveCall(TEMP* _dst, CALL* _src);
	shared_ptr<ExpList> kids();
	IStm* build(shared_ptr<ExpList> kids);

	TEMP* dst;
	CALL* src;
};

class ExpCall: public CAcceptsIRVisitor<ExpCall, IStm> {
public:

	ExpCall(CALL* _call);
	shared_ptr<ExpList> kids();
	IStm* build(shared_ptr<ExpList> kids);

	CALL* call;
};

bool isNop(IStm* stm);
bool commute(IStm* stm, IExp* exp);
IStm* seq(IStm* arg1, IStm* arg2);
StmExpList* reorder(shared_ptr<ExpList> list);
ESEQ* reorderExp(IExp* exp);
IStm* reorderStm(IStm* stm);
ESEQ* doExp(ESEQ* exp);
ESEQ* doExp(IExp* exp);
IStm* doStm(IStm* stm);
IStm* doStm(SEQ* s);
IStm* doStm(MOVE* s);
IStm* doStm(EXP* s);
shared_ptr<StmtList> linear(IStm* s, shared_ptr<StmtList> l);
shared_ptr<StmtList> linear(SEQ* s, shared_ptr<StmtList> l);
shared_ptr<StmtList> linearize(IStm* s);

};





#endif
