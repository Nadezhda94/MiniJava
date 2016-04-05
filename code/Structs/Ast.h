#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED
#include "../common.h"
#include "../ASTVisitors/Visitor.h"
#include "../Structs/Symbol.h"
using namespace Symbol;
extern Symbol::CStorage symbolsStorage;

enum ArithmeticOpType {
	PLUS_OP, MINUS_OP, MULT_OP, DIV_OP, AND_OP, OR_OP, LSHIFT_OP, RSHIFT_OP, ARSHIFT_OP
};

struct Location {
	int firstColumn;
	int firstLine;
	int lastColumn;
	int lastLine;
};

struct CNode {
	virtual void accept(CVisitor*)= 0;
	void setLocation(int _firstColumn, int _firstLine, int _lastColumn, int _lastLine) {
		location.firstColumn = _firstColumn;
		location.firstLine = _firstLine;
		location.lastColumn = _lastColumn;
		location.lastLine = _lastLine;
	}
	Location location;
};

template<class TARGET, class INTERFACE>
class CAcceptsVisitor : public INTERFACE {
public:
	virtual void accept(CVisitor* visitor) {
		// Магический дебаг
		// cout<<typeid(TARGET).name()<<endl;
		visitor->visit( static_cast<TARGET*> (this) );
	}
};

struct CProgramNode : public CNode {};
struct CMainClassNode : public CNode {};
struct CDeclarationsNode : public CNode {};
struct CClassDeclarationNode : public CNode {};
struct CExtendDeclarationNode : public CNode {};
struct CVarDeclarationsNode : public CNode {};
struct CMethodDeclarationsNode : public CNode {};
struct CVarDeclarationNode : public CNode {};
struct CMethodDeclarationNode : public CNode {};
struct CVarsDecNode : public CNode {};
struct CStatsNode : public CNode {};
struct CMethodBodyNode : public CNode {};
struct CParamArgNode : public CNode {};
struct CParamsNode : public CNode {};
struct CParamNode : public CNode {};
struct CTypeNode : public CNode {};
struct CStatementsNode : public CNode {};
struct CStatementNode : public CNode {};
struct CExpressionNode : public CNode {};
struct CExpArgNode : public CNode {};
struct CExpressionsNode : public CNode {};

class CProgramRuleNode: public CAcceptsVisitor<CProgramRuleNode, CProgramNode> {
public:
	CProgramRuleNode( CMainClassNode* _mainClass, CDeclarationsNode* _decl) :
		mainClass(_mainClass), decl(_decl) {}

	shared_ptr<CMainClassNode> mainClass;
	shared_ptr<CDeclarationsNode> decl;
};

class CMainClassDeclarationRuleNode: public CAcceptsVisitor<CMainClassDeclarationRuleNode, CMainClassNode> {
public:
	CMainClassDeclarationRuleNode( const char* _className, const char* _argNames, CStatementNode* _stmt ) :
		className(symbolsStorage.get(_className)), argNames(symbolsStorage.get(_argNames)), stmt(_stmt) {}

	const CSymbol* className;
	const CSymbol* argNames;
	shared_ptr<CStatementNode> stmt;
};

class CDeclarationsListNode: public CAcceptsVisitor<CDeclarationsListNode, CDeclarationsNode> {
public:
	CDeclarationsListNode( CDeclarationsNode* _decl, CClassDeclarationNode* _cl ) :
		decl(_decl), cl(_cl) {}

	shared_ptr<CDeclarationsNode> decl;
	shared_ptr<CClassDeclarationNode> cl;
};

class CClassDeclarationRuleNode: public CAcceptsVisitor<CClassDeclarationRuleNode, CClassDeclarationNode> {
public:
	CClassDeclarationRuleNode( const char* _ident, CExtendDeclarationNode* _extDecl,
							   CVarDeclarationsNode* _vars, CMethodDeclarationsNode* _method ) :
		ident(symbolsStorage.get(_ident)), extDecl(_extDecl), vars(_vars), method(_method) {}

	const CSymbol* ident;
	shared_ptr<CExtendDeclarationNode> extDecl;
	shared_ptr<CVarDeclarationsNode> vars;
	shared_ptr<CMethodDeclarationsNode> method;
};

class CExtendDeclarationRuleNode: public CAcceptsVisitor<CExtendDeclarationRuleNode, CExtendDeclarationNode> {
public:
	CExtendDeclarationRuleNode( const char* _ident ) : ident(symbolsStorage.get(_ident)) {}

	const CSymbol* ident;
};

class CVarDeclarationsListNode : public CAcceptsVisitor<CVarDeclarationsListNode, CVarDeclarationsNode> {
public:
	CVarDeclarationsListNode(CVarDeclarationsNode* _first, CVarDeclarationNode* _second):
		list(_first), item(_second) {}

	shared_ptr<CVarDeclarationsNode> list;
	shared_ptr<CVarDeclarationNode> item;
};

class CMethodDeclarationsListNode : public CAcceptsVisitor<CMethodDeclarationsListNode, CMethodDeclarationsNode> {
public:
	CMethodDeclarationsListNode(CMethodDeclarationsNode* _first, CMethodDeclarationNode* _second):
		list(_first), item(_second) {}

	shared_ptr<CMethodDeclarationsNode> list;
	shared_ptr<CMethodDeclarationNode> item;
};

class CVarDeclarationRuleNode : public CAcceptsVisitor<CVarDeclarationRuleNode, CVarDeclarationNode> {
public:
	CVarDeclarationRuleNode(CTypeNode* _type, const char* _ident): type(_type), ident(symbolsStorage.get(_ident)) {}

	shared_ptr<CTypeNode> type;
	const CSymbol* ident;
};

class CMethodDeclarationRuleNode : public CAcceptsVisitor<CMethodDeclarationRuleNode, CMethodDeclarationNode> {
public:
	CMethodDeclarationRuleNode(CTypeNode* _type, const char* _ident,
		CParamArgNode* _param_arg, CMethodBodyNode* _method_body, CExpressionNode* _return_exp):
			type(_type), ident(symbolsStorage.get(_ident)), param_arg(_param_arg),
			method_body(_method_body), return_exp(_return_exp) {}

	shared_ptr<CTypeNode> type;
	const CSymbol* ident;
	shared_ptr<CParamArgNode> param_arg;
	shared_ptr<CMethodBodyNode> method_body;
	shared_ptr<CExpressionNode> return_exp;
};

class CVarsDecListNode : public CAcceptsVisitor<CVarsDecListNode, CVarsDecNode> {
public:
	CVarsDecListNode(CVarsDecNode* _list, CVarDeclarationNode* _next): list(_list), next(_next) {}

	shared_ptr<CVarsDecNode> list;
	shared_ptr<CVarDeclarationNode> next;
};

class CVarsDecFirstNode : public CAcceptsVisitor<CVarsDecFirstNode, CVarsDecNode> {
public:
	CVarsDecFirstNode(CVarDeclarationNode* _first):
		first(_first) {}

	shared_ptr<CVarDeclarationNode> first;
};

class CStatsFirstNode: public CAcceptsVisitor<CStatsFirstNode, CStatsNode> {
public:
	CStatsFirstNode(CStatementNode* _stm) : stm(_stm) {}

	shared_ptr<CStatementNode> stm;
};

class CStatsListNode: public CAcceptsVisitor<CStatsListNode, CStatsNode> {
public:
	CStatsListNode(CStatsNode* _list,  CStatementNode* _stm) : list(_list), stm(_stm) {}

	shared_ptr<CStatsNode> list;
	shared_ptr<CStatementNode> stm;
};

class CMethodBodyVarsNode: public CAcceptsVisitor<CMethodBodyVarsNode, CMethodBodyNode> {
public:
	CMethodBodyVarsNode(CVarsDecNode* _vars) : vars(_vars) {}

	shared_ptr<CVarsDecNode> vars;
};

class CMethodBodyStatsNode: public CAcceptsVisitor<CMethodBodyStatsNode, CMethodBodyNode> {
public:
	CMethodBodyStatsNode(CStatsNode* _stats) : stats(_stats) {}

	shared_ptr<CStatsNode> stats;
};

class CMethodBodyAllNode: public CAcceptsVisitor<CMethodBodyAllNode, CMethodBodyNode> {
public:
	CMethodBodyAllNode(CVarsDecNode* _vars, CStatsNode* _stats) :
		vars(_vars), stats(_stats) {}

	shared_ptr<CVarsDecNode> vars;
	shared_ptr<CStatsNode> stats;
};

class CParamArgListNode: public CAcceptsVisitor<CParamArgListNode, CParamArgNode> {
public:
	CParamArgListNode(CParamsNode* _params) : params(_params) {}

	shared_ptr<CParamsNode> params;
};

class CParamsOneNode: public CAcceptsVisitor<CParamsOneNode, CParamsNode> {
public:
	CParamsOneNode(CParamNode* _param) : param(_param) {}

	shared_ptr<CParamNode> param;
};

class CParamsTwoNode: public CAcceptsVisitor<CParamsTwoNode, CParamsNode> {
public:
	CParamsTwoNode(CParamsNode* _first, CParamNode* _second):
		first(_first), second(_second) {}

	shared_ptr<CParamsNode> first;
	shared_ptr<CParamNode> second;
};

class CParamRuleNode: public CAcceptsVisitor<CParamRuleNode, CParamNode> {
public:
	CParamRuleNode(CTypeNode* _type, const char* _ident) :
		type(_type), ident(symbolsStorage.get(_ident)) {}

	shared_ptr<CTypeNode> type;
	const CSymbol* ident;
};

class CTypeRuleNode: public CAcceptsVisitor<CTypeRuleNode, CTypeNode> {
public:
	CTypeRuleNode(const char* _type): type(symbolsStorage.get(_type)) {}

	const CSymbol* type;
};

/// Statements begin
class CNumerousStatementsNode : public CAcceptsVisitor<CNumerousStatementsNode, CStatementsNode> {
public:
	CNumerousStatementsNode(CStatementsNode* _statements, CStatementNode* _statement):
		statements(_statements), statement(_statement) {}

	shared_ptr<CStatementsNode> statements;
	shared_ptr<CStatementNode> statement;
};

class CBracedStatementNode : public CAcceptsVisitor<CBracedStatementNode, CStatementNode> {
public:
	CBracedStatementNode(CStatementsNode* _statements):statements(_statements) {}

	shared_ptr<CStatementsNode> statements;
};

class CIfStatementNode : public CAcceptsVisitor<CIfStatementNode, CStatementNode> {
public:
	CIfStatementNode(CExpressionNode* _expression, CStatementNode* _thenStatement, CStatementNode* _elseStatement):
		expression(_expression), thenStatement(_thenStatement), elseStatement(_elseStatement) {}

	shared_ptr<CExpressionNode> expression;
	shared_ptr<CStatementNode> thenStatement;
	shared_ptr<CStatementNode> elseStatement;

};

class CWhileStatementNode : public CAcceptsVisitor<CWhileStatementNode, CStatementNode> {
public:
	CWhileStatementNode(CExpressionNode* _expression, CStatementNode* _statement):expression(_expression), statement(_statement) {}

	shared_ptr<CExpressionNode> expression;
	shared_ptr<CStatementNode> statement;
};

class CPrintStatementNode : public CAcceptsVisitor<CPrintStatementNode, CStatementNode> {
public:
	CPrintStatementNode(CExpressionNode* _expression):expression(_expression) {}

	shared_ptr<CExpressionNode> expression;
};

class CAssignStatementNode : public CAcceptsVisitor<CAssignStatementNode, CStatementNode> {
public:
	CAssignStatementNode(CExpressionNode* _expression, const char* ident):expression(_expression),
		identifier(symbolsStorage.get(ident)) {}

	shared_ptr<CExpressionNode> expression;
	const CSymbol* identifier;
};

class CInvokeExpressionStatementNode : public CAcceptsVisitor<CInvokeExpressionStatementNode, CStatementNode> {
public:
	CInvokeExpressionStatementNode(CExpressionNode* _firstexpression, CExpressionNode* _secondexpression,
								   const char* ident): firstexpression(_firstexpression), secondexpression(_secondexpression),
		identifier(symbolsStorage.get(ident)) {}

	shared_ptr<CExpressionNode> firstexpression;
	shared_ptr<CExpressionNode> secondexpression;
	const CSymbol* identifier;
};


/// Expressions begin

class CInvokeExpressionNode: public CAcceptsVisitor<CInvokeExpressionNode, CExpressionNode> {
public:
	CInvokeExpressionNode(CExpressionNode* _firstExp, CExpressionNode* _secondExp) : firstExp(_firstExp), secondExp(_secondExp) {}

	shared_ptr<CExpressionNode> firstExp;
	shared_ptr<CExpressionNode> secondExp;
};

class CLengthExpressionNode: public CAcceptsVisitor<CLengthExpressionNode, CExpressionNode> {
public:
	CLengthExpressionNode(CExpressionNode* _exp) : expr(_exp) {}

	shared_ptr<CExpressionNode> expr;
};

class CArithmeticExpressionNode: public CAcceptsVisitor<CArithmeticExpressionNode, CExpressionNode> {
public:
	CArithmeticExpressionNode(CExpressionNode* _firstExp, CExpressionNode* _secondExp, ArithmeticOpType _opType) :
		firstExp(_firstExp), secondExp(_secondExp), opType(_opType) {}

	shared_ptr<CExpressionNode> firstExp;
	shared_ptr<CExpressionNode> secondExp;
	ArithmeticOpType opType;
};

class CUnaryExpressionNode: public CAcceptsVisitor<CUnaryExpressionNode, CExpressionNode> {
public:
	CUnaryExpressionNode(CExpressionNode* _exp, ArithmeticOpType _op) : expr(_exp), op(_op) {}

	shared_ptr<CExpressionNode> expr;
	ArithmeticOpType op;
};

class CCompareExpressionNode: public CAcceptsVisitor<CCompareExpressionNode, CExpressionNode> {
public:
	CCompareExpressionNode(CExpressionNode* _firstExp, CExpressionNode* _secondExp) : firstExp(_firstExp), secondExp(_secondExp) {}

	shared_ptr<CExpressionNode> firstExp;
	shared_ptr<CExpressionNode> secondExp;
};

class CNotExpressionNode: public CAcceptsVisitor<CNotExpressionNode, CExpressionNode> {
public:
	CNotExpressionNode(CExpressionNode* _exp) : expr(_exp) {}

	shared_ptr<CExpressionNode> expr;
};

class CNewArrayExpressionNode: public CAcceptsVisitor<CNewArrayExpressionNode, CExpressionNode> {
public:
	CNewArrayExpressionNode(CExpressionNode* _exp) : expr(_exp) {}

	shared_ptr<CExpressionNode> expr;
};

class CNewObjectExpressionNode: public CAcceptsVisitor<CNewObjectExpressionNode, CExpressionNode> {
public:
	CNewObjectExpressionNode(const char* _objType) : objType(symbolsStorage.get(_objType)) {}

	const CSymbol* objType;
};

class CIntExpressionNode: public CAcceptsVisitor<CIntExpressionNode, CExpressionNode> {
public:
	CIntExpressionNode(int _value) : value(_value) {}

	int value;
};

class CBooleanExpressionNode: public CAcceptsVisitor<CBooleanExpressionNode, CExpressionNode> {
public:
	CBooleanExpressionNode(bool _value) : value(_value) {}

	bool value;
};

class CIdentExpressionNode: public CAcceptsVisitor<CIdentExpressionNode, CExpressionNode> {
public:
	CIdentExpressionNode(const char* _name) : name(symbolsStorage.get(_name)) {}

	const CSymbol* name;
};

class CThisExpressionNode: public CAcceptsVisitor<CThisExpressionNode, CExpressionNode> {
public:
	CThisExpressionNode(const char* _name) : name(symbolsStorage.get(_name)) {}

	const CSymbol* name;
};

class CParenExpressionNode: public CAcceptsVisitor<CParenExpressionNode, CExpressionNode> {
public:
	CParenExpressionNode(CExpressionNode* _exp) : expr(_exp) {}

	shared_ptr<CExpressionNode> expr;
};

class CInvokeMethodExpressionNode: public CAcceptsVisitor<CInvokeMethodExpressionNode, CExpressionNode> {
public:
	CInvokeMethodExpressionNode(CExpressionNode* _exp, const char* _name, CExpArgNode* _args):
		expr(_exp), name(symbolsStorage.get(_name)), args(_args) {}
	~CInvokeMethodExpressionNode() {


	}

	shared_ptr<CExpressionNode> expr;
	const CSymbol* name;
	shared_ptr<CExpArgNode> args;
};


class CFewArgsExpressionNode: public CAcceptsVisitor<CFewArgsExpressionNode, CExpArgNode> {
public:
	CFewArgsExpressionNode(CExpressionsNode* _exp) : expr(_exp) {}

	shared_ptr<CExpressionsNode> expr;
};

class CListExpressionNode: public CAcceptsVisitor<CListExpressionNode, CExpressionsNode> {
public:
	CListExpressionNode(CExpressionsNode* _prevExps, CExpressionNode* _nextExp):
		prevExps(_prevExps), nextExp(_nextExp) {}

	shared_ptr<CExpressionsNode> prevExps;
	shared_ptr<CExpressionNode> nextExp;
};

class CLastListExpressionNode: public CAcceptsVisitor<CLastListExpressionNode, CExpressionsNode> {
public:
	CLastListExpressionNode(CExpressionNode* _exp) : expr(_exp) {}

	shared_ptr<CExpressionNode> expr;
};

#endif
