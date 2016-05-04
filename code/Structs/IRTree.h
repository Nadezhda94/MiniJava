#ifndef IRTREE_H_INCLUDED
#define IRTREE_H_INCLUDED
#include "../Structs/Temp.h"
#include "../Structs/Ast.h"
#include "../IRVisitors/Visitor.h"

namespace IRTree {

enum CJUMP_OP {
	EQ, NE, LT, GT, LE, GE, ULT, ULE, UGT, UGE
};

struct INode {
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

struct ExpList;

struct IExp : public INode {
	virtual shared_ptr<ExpList> kids() = 0;
	virtual IExp* build(shared_ptr<ExpList> kids) = 0;
};

struct IStm : public INode {
	virtual shared_ptr<ExpList> kids() = 0;
	virtual IStm* build(shared_ptr<ExpList> kids) = 0;
};

struct ExpList {
	ExpList(IExp* _head, shared_ptr<ExpList> _tail) : head(_head), tail(_tail) {}
	IExp* head;
	shared_ptr<ExpList> tail;
};

struct StmtList {
	StmtList(IStm* _head, shared_ptr<StmtList> _tail);
	StmtList(vector<IStm*>& list);
	IStm* head;
	shared_ptr<StmtList> tail;

	void toVector(vector<IStm*>& list);
};

struct StmExpList {
	StmExpList(IStm* _stm, shared_ptr<ExpList> _exps);
	IStm* stm;
	shared_ptr<ExpList> exps;
};

struct MEM: public CAcceptsIRVisitor<MEM, IExp> {
	MEM(IExp* _exp);
	shared_ptr<ExpList> kids();
	IExp* build(shared_ptr<ExpList> kids);

	IExp* exp;
};

struct MOVE: public CAcceptsIRVisitor<MOVE, IStm> {
	MOVE(IExp* _dst, IExp* _src);
	shared_ptr<ExpList> kids();
	IStm* build(shared_ptr<ExpList> kids);

	IExp* dst;
	IExp* src;
};

struct EXP: public CAcceptsIRVisitor<EXP, IStm> {
	EXP(IExp* _exp);
	shared_ptr<ExpList> kids();
	IStm* build(shared_ptr<ExpList> kids);

	IExp* exp;
};


struct JUMP: public CAcceptsIRVisitor<JUMP, IStm> {
	JUMP(IExp* _exp, const Temp::CLabel* _target);
	JUMP(const Temp::CLabel* _target);
	shared_ptr<ExpList> kids();
	IStm* build(shared_ptr<ExpList> kids);

	IExp* exp;
	const Temp::CLabel* target;
};

struct CJUMP: public CAcceptsIRVisitor<CJUMP, IStm> {
	CJUMP(CJUMP_OP _relop, IExp* _left, IExp* _right, const Temp::CLabel* _iftrue, const Temp::CLabel* _iffalse);
	shared_ptr<ExpList> kids();
	IStm* build(shared_ptr<ExpList> kids);

	CJUMP_OP relop;
	IExp* left;
	IExp* right;
	const Temp::CLabel* iftrue;
	const Temp::CLabel* iffalse;
};

struct SEQ: public CAcceptsIRVisitor<SEQ, IStm> {
	SEQ(IStm* _left, IStm* _right);
	shared_ptr<ExpList> kids();
	IStm* build(shared_ptr<ExpList> kids);

	IStm* left;
	IStm* right;
};

struct LABEL: public CAcceptsIRVisitor<LABEL, IStm> {
	LABEL(const Temp::CLabel* _label);
	shared_ptr<ExpList> kids();
	IStm* build(shared_ptr<ExpList> kids);

	const Temp::CLabel* label;
};


struct CONST: public CAcceptsIRVisitor<CONST, IExp> {
	CONST(int _value);
	shared_ptr<ExpList> kids();
	IExp* build(shared_ptr<ExpList> kids);

	int value;
};

struct NAME : public CAcceptsIRVisitor<NAME, IExp> {
	NAME(shared_ptr<Temp::CLabel> _label);
	shared_ptr<ExpList> kids();
	IExp* build(shared_ptr<ExpList> kids);

	shared_ptr<Temp::CLabel> label;
};

struct TEMP: public CAcceptsIRVisitor<TEMP, IExp> {
	TEMP(shared_ptr<const Temp::CTemp> _temp);
	shared_ptr<ExpList> kids();
	IExp* build(shared_ptr<ExpList> kids);

	shared_ptr<const Temp::CTemp> temp;
};

struct BINOP: public CAcceptsIRVisitor<BINOP, IExp> {
	BINOP(ArithmeticOpType _binop, IExp* _left, IExp* _right);
	shared_ptr<ExpList> kids();
	IExp* build(shared_ptr<ExpList> kids);

	ArithmeticOpType binop;
	IExp* left;
	IExp* right;
};

struct CALL: public CAcceptsIRVisitor<CALL, IExp> {
	CALL(IExp* _func, shared_ptr<ExpList> _args);
	shared_ptr<ExpList> kids();
	IExp* build(shared_ptr<ExpList> kids);

	IExp* func;
	shared_ptr<ExpList> args;
};

struct ESEQ: public CAcceptsIRVisitor<ESEQ, IExp> {
	ESEQ(IStm* _stm, IExp* _exp);
	shared_ptr<ExpList> kids();
	IExp* build(shared_ptr<ExpList> kids);

	IStm* stm;
	IExp* exp;
};

struct MoveCall: public CAcceptsIRVisitor<MoveCall, IStm> {
	MoveCall(TEMP* _dst, CALL* _src);
	shared_ptr<ExpList> kids();
	IStm* build(shared_ptr<ExpList> kids);

	TEMP* dst;
	CALL* src;
};

struct ExpCall: public CAcceptsIRVisitor<ExpCall, IStm> {
	ExpCall(CALL* _call);
	shared_ptr<ExpList> kids();
	IStm* build(shared_ptr<ExpList> kids);

	CALL* call;
};

};





#endif
