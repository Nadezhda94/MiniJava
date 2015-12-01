#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED
#include "../common.h"
#include "../Visitors/Visitor.h"
#include "../Structs/Symbol.h"
using namespace Symbol;
extern Symbol::CStorage symbolsStorage;

struct Location
{
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
    ~CProgramRuleNode(){
        delete mainClass;
        delete decl;
    }

	CMainClassNode* mainClass;
	CDeclarationsNode* decl;
};

class CMainClassDeclarationRuleNode: public CAcceptsVisitor<CMainClassDeclarationRuleNode, CMainClassNode> {
public:
	CMainClassDeclarationRuleNode( const char* _className, const char* _argNames, CStatementNode* _stmt ) :
		className(symbolsStorage.get(_className)), argNames(symbolsStorage.get(_argNames)), stmt(_stmt) {}
    ~CMainClassDeclarationRuleNode(){
        delete stmt;
    }
	const CSymbol* className;
	const CSymbol* argNames;
	CStatementNode* stmt;
};

class CDeclarationsListNode: public CAcceptsVisitor<CDeclarationsListNode, CDeclarationsNode> {
public:
	CDeclarationsListNode( CDeclarationsNode* _decl, CClassDeclarationNode* _cl ) :
		decl(_decl), cl(_cl) {}
    ~CDeclarationsListNode(){
        delete decl;
        delete cl;
    }

	CDeclarationsNode* decl;
	CClassDeclarationNode* cl;
};

class CClassDeclarationRuleNode: public CAcceptsVisitor<CClassDeclarationRuleNode, CClassDeclarationNode> {
public:
	CClassDeclarationRuleNode( const char* _ident, CExtendDeclarationNode* _extDecl,
    CVarDeclarationsNode* _vars, CMethodDeclarationsNode* _method ) :
		ident(symbolsStorage.get(_ident)), extDecl(_extDecl), vars(_vars), method(_method) {}
    ~CClassDeclarationRuleNode(){
        delete extDecl;
        delete vars;
        delete method;
    }

	const CSymbol* ident;
	CExtendDeclarationNode *extDecl;
	CVarDeclarationsNode *vars;
	CMethodDeclarationsNode *method;
};

class CExtendDeclarationRuleNode: public CAcceptsVisitor<CExtendDeclarationRuleNode, CExtendDeclarationNode> {
public:
	CExtendDeclarationRuleNode( const char* _ident ) : ident(symbolsStorage.get(_ident)) {}
	const CSymbol* ident;
};

class CVarDeclarationsListNode : public CAcceptsVisitor<CVarDeclarationsListNode, CVarDeclarationsNode>{
public:
    CVarDeclarationsListNode(CVarDeclarationsNode* _first, CVarDeclarationNode* _second):
        list(_first), item(_second) {}
    ~CVarDeclarationsListNode(){
        delete list;
        delete item;
    }

    CVarDeclarationsNode* list;
    CVarDeclarationNode* item;
};

class CMethodDeclarationsListNode : public CAcceptsVisitor<CMethodDeclarationsListNode, CMethodDeclarationsNode>{
public:
    CMethodDeclarationsListNode(CMethodDeclarationsNode* _first, CMethodDeclarationNode* _second):
        list(_first), item(_second) {}
    ~CMethodDeclarationsListNode(){
        delete list;
        delete item;
    }

    CMethodDeclarationsNode* list;
    CMethodDeclarationNode* item;
};

class CVarDeclarationRuleNode : public CAcceptsVisitor<CVarDeclarationRuleNode, CVarDeclarationNode>{
public:
    CVarDeclarationRuleNode(CTypeNode* _type, const char* _ident): type(_type), ident(symbolsStorage.get(_ident)){}
    ~CVarDeclarationRuleNode(){
        delete type;
    }

    CTypeNode* type;
    const CSymbol* ident;
};

class CMethodDeclarationRuleNode : public CAcceptsVisitor<CMethodDeclarationRuleNode, CMethodDeclarationNode>{
public:
    CMethodDeclarationRuleNode(CTypeNode* _type, const char* _ident,
    CParamArgNode* _param_arg, CMethodBodyNode* _method_body, CExpressionNode* _return_exp):
        type(_type), ident(symbolsStorage.get(_ident)), param_arg(_param_arg),
        method_body(_method_body), return_exp(_return_exp){}
    ~CMethodDeclarationRuleNode(){
        delete type;
        delete param_arg;
        delete method_body;
        delete return_exp;
    }

    CTypeNode* type;
    const CSymbol* ident;
    CParamArgNode* param_arg;
    CMethodBodyNode* method_body;
    CExpressionNode* return_exp;
};

class CVarsDecListNode : public CAcceptsVisitor<CVarsDecListNode, CVarsDecNode>{
public:
    CVarsDecListNode(CVarsDecNode* _list, CVarDeclarationNode* _next):
        list(_list), next(_next){}
    ~CVarsDecListNode(){
        delete list;
        delete next;
    }

    CVarsDecNode* list;
    CVarDeclarationNode* next;
};

class CVarsDecFirstNode : public CAcceptsVisitor<CVarsDecFirstNode, CVarsDecNode>{
public:
    CVarsDecFirstNode(CVarDeclarationNode* _first):
        first(_first){}
    ~CVarsDecFirstNode(){
        delete first;
    }

    CVarDeclarationNode* first;
};

class CStatsFirstNode: public CAcceptsVisitor<CStatsFirstNode, CStatsNode> {
public:
    CStatsFirstNode(CStatementNode* _stm) : stm(_stm) {}
    ~CStatsFirstNode(){
        delete stm;
    }

    CStatementNode* stm;
};

class CStatsListNode: public CAcceptsVisitor<CStatsListNode, CStatsNode> {
public:
    CStatsListNode(CStatsNode* _list,  CStatementNode* _stm) : list(_list), stm(_stm) {}
    ~CStatsListNode(){
        delete stm;
        delete list;
    }

    CStatsNode* list;
    CStatementNode* stm;
};

class CMethodBodyVarsNode: public CAcceptsVisitor<CMethodBodyVarsNode, CMethodBodyNode> {
public:
    CMethodBodyVarsNode(CVarsDecNode* _vars) : vars(_vars){}
    ~CMethodBodyVarsNode(){
        delete vars;
    }

    CVarsDecNode* vars;
};

class CMethodBodyStatsNode: public CAcceptsVisitor<CMethodBodyStatsNode, CMethodBodyNode> {
public:
    CMethodBodyStatsNode(CStatsNode* _stats) : stats(_stats){}
    ~CMethodBodyStatsNode(){
        delete stats;
    }

    CStatsNode* stats;
};

class CMethodBodyAllNode: public CAcceptsVisitor<CMethodBodyAllNode, CMethodBodyNode> {
public:
    CMethodBodyAllNode(CVarsDecNode* _vars, CStatsNode* _stats) :
        vars(_vars), stats(_stats){}
    ~CMethodBodyAllNode(){
        delete vars;
        delete stats;
    }

    CVarsDecNode* vars;
    CStatsNode* stats;
};

class CParamArgListNode: public CAcceptsVisitor<CParamArgListNode, CParamArgNode> {
public:
    CParamArgListNode(CParamsNode* _params) : params(_params){}
    ~CParamArgListNode(){
        delete params;
    }

    CParamsNode* params;
};

class CParamsOneNode: public CAcceptsVisitor<CParamsOneNode, CParamsNode> {
public:
    CParamsOneNode(CParamNode* _param) : param(_param){}
    ~CParamsOneNode(){
        delete param;
    }

    CParamNode* param;
};

class CParamsTwoNode: public CAcceptsVisitor<CParamsTwoNode, CParamsNode> {
public:
    CParamsTwoNode(CParamsNode* _first, CParamNode* _second):
        first(_first), second(_second){}
    ~CParamsTwoNode(){
        delete first;
        delete second;
    }

    CParamsNode* first;
    CParamNode* second;
};

class CParamRuleNode: public CAcceptsVisitor<CParamRuleNode, CParamNode> {
public:
    CParamRuleNode(CTypeNode* _type, const char* _ident) :
        type(_type), ident(symbolsStorage.get(_ident)){}
    ~CParamRuleNode(){
        delete type;
    }

    CTypeNode* type;
    const CSymbol* ident;
};

class CTypeRuleNode: public CAcceptsVisitor<CTypeRuleNode, CTypeNode> {
public:
    CTypeRuleNode(const char* _type): type(symbolsStorage.get(_type)){}

    const CSymbol* type;
};

/// Statements begin
class CNumerousStatementsNode : public CAcceptsVisitor<CNumerousStatementsNode, CStatementsNode>{
public:
    CNumerousStatementsNode(CStatementsNode* _statements, CStatementNode* _statement):
        statements(_statements), statement(_statement){}
    ~CNumerousStatementsNode(){
        delete statements;
        delete statement;
    }

    CStatementsNode* statements;
    CStatementNode* statement;
};

class CBracedStatementNode : public CAcceptsVisitor<CBracedStatementNode, CStatementNode>{
public:
    CBracedStatementNode(CStatementsNode* _statements):statements(_statements){}
    ~CBracedStatementNode(){
        delete statements;
    }

    CStatementsNode* statements;
};

class CIfStatementNode : public CAcceptsVisitor<CIfStatementNode, CStatementNode>{
public:
    CIfStatementNode(CExpressionNode* _expression, CStatementNode* _thenStatement, CStatementNode* _elseStatement):
        expression(_expression), thenStatement(_thenStatement), elseStatement(_elseStatement){}
    ~CIfStatementNode(){
        delete expression;
        delete thenStatement;
        delete elseStatement;
    }

    CExpressionNode* expression;
    CStatementNode* thenStatement;
    CStatementNode* elseStatement;

};

class CWhileStatementNode : public CAcceptsVisitor<CWhileStatementNode, CStatementNode>{
public:
    CWhileStatementNode(CExpressionNode* _expression, CStatementNode* _statement):expression(_expression), statement(_statement){}
    ~CWhileStatementNode(){
        delete expression;
        delete statement;
    }

    CExpressionNode* expression;
    CStatementNode* statement;
};

class CPrintStatementNode : public CAcceptsVisitor<CPrintStatementNode, CStatementNode>{
public:
    CPrintStatementNode(CExpressionNode* _expression):expression(_expression){}
    ~CPrintStatementNode(){
        delete expression;
    }

    CExpressionNode* expression;
};

class CAssignStatementNode : public CAcceptsVisitor<CAssignStatementNode, CStatementNode>{
public:
    CAssignStatementNode(CExpressionNode* _expression, const char* ident):expression(_expression),
        identifier(symbolsStorage.get(ident)){}
    ~CAssignStatementNode(){
        delete expression;
    }

    CExpressionNode* expression;
    const CSymbol* identifier;
};

class CInvokeExpressionStatementNode : public CAcceptsVisitor<CInvokeExpressionStatementNode, CStatementNode>{
public:
    CInvokeExpressionStatementNode(CExpressionNode* _firstexpression, CExpressionNode* _secondexpression,
        const char* ident): firstexpression(_firstexpression), secondexpression(_secondexpression),
        identifier(symbolsStorage.get(ident)){}
    ~CInvokeExpressionStatementNode(){
        delete firstexpression;
        delete secondexpression;
    }

    CExpressionNode* firstexpression;
    CExpressionNode* secondexpression;
    const CSymbol* identifier;
};


/// Expressions begin

class CInvokeExpressionNode: public CAcceptsVisitor<CInvokeExpressionNode, CExpressionNode> {
public:
    CInvokeExpressionNode(CExpressionNode* _firstExp, CExpressionNode* _secondExp) : firstExp(_firstExp), secondExp(_secondExp) {}
    ~CInvokeExpressionNode(){
        delete firstExp;
        delete secondExp;
    }
    CExpressionNode* firstExp;
    CExpressionNode* secondExp;
};

class CLengthExpressionNode: public CAcceptsVisitor<CLengthExpressionNode, CExpressionNode> {
public:
    CLengthExpressionNode(CExpressionNode* _exp) : expr(_exp) {}
    ~CLengthExpressionNode(){
        delete expr;
    }

    CExpressionNode* expr;
};

enum ArithmeticOpType {
    PLUS_OP, MINUS_OP, MULT_OP, DIV_OP, AND_OP
};

class CArithmeticExpressionNode: public CAcceptsVisitor<CArithmeticExpressionNode, CExpressionNode> {
public:
    CArithmeticExpressionNode(CExpressionNode* _firstExp, CExpressionNode* _secondExp, ArithmeticOpType _opType) :
        firstExp(_firstExp), secondExp(_secondExp), opType(_opType) {}
    ~CArithmeticExpressionNode(){
        delete firstExp;
        delete secondExp;
    }

    CExpressionNode* firstExp;
    CExpressionNode* secondExp;
    ArithmeticOpType opType;
};

enum UnaryOpType {
    UPLUS_OP, UMINUS_OP
};

class CUnaryExpressionNode: public CAcceptsVisitor<CUnaryExpressionNode, CExpressionNode> {
public:
    CUnaryExpressionNode(CExpressionNode* _exp, UnaryOpType _op) : expr(_exp), op(_op){}
    ~CUnaryExpressionNode(){
        delete expr;
    }

    CExpressionNode* expr;
    UnaryOpType op;
};

class CCompareExpressionNode: public CAcceptsVisitor<CCompareExpressionNode, CExpressionNode> {
public:
    CCompareExpressionNode(CExpressionNode* _firstExp, CExpressionNode* _secondExp) : firstExp(_firstExp), secondExp(_secondExp) {}
    ~CCompareExpressionNode(){
        delete firstExp;
        delete secondExp;
    }

    CExpressionNode* firstExp;
    CExpressionNode* secondExp;
};

class CNotExpressionNode: public CAcceptsVisitor<CNotExpressionNode, CExpressionNode> {
public:
    CNotExpressionNode(CExpressionNode* _exp) : expr(_exp) {}
    ~CNotExpressionNode(){
        delete expr;
    }

    CExpressionNode* expr;
};

class CNewArrayExpressionNode: public CAcceptsVisitor<CNewArrayExpressionNode, CExpressionNode> {
public:
    CNewArrayExpressionNode(CExpressionNode* _exp) : expr(_exp) {}
    ~CNewArrayExpressionNode(){
        delete expr;
    }

    CExpressionNode* expr;
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
    ~CParenExpressionNode(){
        delete expr;
    }

    CExpressionNode* expr;
};

class CInvokeMethodExpressionNode: public CAcceptsVisitor<CInvokeMethodExpressionNode, CExpressionNode> {
public:
    CInvokeMethodExpressionNode(CExpressionNode* _exp, const char* _name, CExpArgNode* _args):
        expr(_exp), name(symbolsStorage.get(_name)), args(_args) {}
    ~CInvokeMethodExpressionNode(){
        delete expr;
        delete args;
    }

    CExpressionNode* expr;
    const CSymbol* name;
    CExpArgNode* args;
};


class CFewArgsExpressionNode: public CAcceptsVisitor<CFewArgsExpressionNode, CExpArgNode> {
public:
    CFewArgsExpressionNode(CExpressionsNode* _exp) : expr(_exp) {}
    ~CFewArgsExpressionNode(){
        delete expr;
    }

    CExpressionsNode* expr;
};

class CListExpressionNode: public CAcceptsVisitor<CListExpressionNode, CExpressionsNode> {
public:
    CListExpressionNode(CExpressionsNode* _prevExps, CExpressionNode* _nextExp):
                        prevExps(_prevExps), nextExp(_nextExp) {}
    ~CListExpressionNode(){
        delete prevExps;
        delete nextExp;
    }

    CExpressionsNode* prevExps;
    CExpressionNode* nextExp;
};

class CLastListExpressionNode: public CAcceptsVisitor<CLastListExpressionNode, CExpressionsNode> {
public:
    CLastListExpressionNode(CExpressionNode* _exp) : expr(_exp) {}
    ~CLastListExpressionNode(){
        delete expr;
    }

    CExpressionNode* expr;
};

#endif
