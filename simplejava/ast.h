#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED
#include <string>
#include "CVisitor.h"
using std::string;

struct CNode{
  virtual void accept(CVisitor*)= 0;
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


class CProgramRuleNode: public CProgramNode {
public:
	CProgramRuleNode( CMainClassNode* _mainClass, CDeclarationsNode* _decl) :
		mainClass(_mainClass), decl(_decl) {}
  ~CProgramRuleNode(){
    delete mainClass;
    delete decl;
  }
	void accept( CVisitor* v ){
		v->visit( this );
	}

	CMainClassNode* mainClass;
	CDeclarationsNode* decl;
};

class CMainClassDeclarationRuleNode: public CMainClassNode {
public:
	CMainClassDeclarationRuleNode( const char* _className, const char* _argNames, CStatementNode* _stmt ) :
		className(_className), argNames(_argNames), stmt(_stmt) {}
  ~CMainClassDeclarationRuleNode(){
    delete stmt;
  }
	void accept( CVisitor* v ){
		v->visit( this );
	}

	string className;
	string argNames;
	CStatementNode* stmt;
};

class CDeclarationsListNode: public CDeclarationsNode {
public:
	CDeclarationsListNode( CDeclarationsNode* _decl, CClassDeclarationNode* _cl ) :
		decl(_decl), cl(_cl) {}
  ~CDeclarationsListNode(){
    delete decl;
    delete cl;
  }
	void accept( CVisitor* v ){
		v->visit( this );
	}

	CDeclarationsNode* decl;
	CClassDeclarationNode* cl;
};

class CDeclarationsEmptyNode: public CDeclarationsNode {
public:
	CDeclarationsEmptyNode(){}
	void accept( CVisitor* v ){
		v->visit( this );
	}
};

class CClassDeclarationRuleNode: public CClassDeclarationNode {
public:
	CClassDeclarationRuleNode( const char* _ident, CExtendDeclarationNode* _extDecl,
    CVarDeclarationsNode* _vars, CMethodDeclarationsNode* _method ) :
		ident(_ident), extDecl(_extDecl), vars(_vars), method(_method) {}
  ~CClassDeclarationRuleNode(){
    delete extDecl;
    delete vars;
    delete method;
  }
	void accept( CVisitor* v ){
		v->visit( this );
	}


	string ident;
	CExtendDeclarationNode *extDecl;
	CVarDeclarationsNode *vars;
	CMethodDeclarationsNode *method;
};

class CExtendDeclarationRuleNode: public CExtendDeclarationNode {
public:
	CExtendDeclarationRuleNode( const char* _ident ) : ident(_ident) {}

	void accept( CVisitor* v ){
		v->visit( this );
	}

	string ident;
};

class CExtendDeclarationEmptyNode: public CExtendDeclarationNode {
public:
  CExtendDeclarationEmptyNode(){}
  void accept( CVisitor* v ){
     v->visit(this);
  }
};

class CVarDeclarationsListNode : public CVarDeclarationsNode{
public:
  CVarDeclarationsListNode(CVarDeclarationsNode* _first, CVarDeclarationNode* _second):
    list(_first), item(_second) {}
  ~CVarDeclarationsListNode(){
    delete list;
    delete item;
  }
  void accept( CVisitor* v ){
    v->visit(this);
  }

  CVarDeclarationsNode* list;
  CVarDeclarationNode* item;
};

class CVarDeclarationsEmptyNode : public CVarDeclarationsNode{
public:
  CVarDeclarationsEmptyNode(){}
  void accept( CVisitor* v ){
    v->visit(this);
  }
};

class CMethodDeclarationsListNode : public CMethodDeclarationsNode{
public:
  CMethodDeclarationsListNode(CMethodDeclarationsNode* _first, CMethodDeclarationNode* _second):
    list(_first), item(_second) {}
  ~CMethodDeclarationsListNode(){
    delete list;
    delete item;
  }
  void accept( CVisitor* v ){
    v->visit(this);
  }

  CMethodDeclarationsNode* list;
  CMethodDeclarationNode* item;
};

class CMethodDeclarationsEmptyNode : public CMethodDeclarationsNode{
public:
  CMethodDeclarationsEmptyNode(){}
  void accept( CVisitor* v ){
    v->visit(this);
  }
};

class CVarDeclarationRuleNode : public CVarDeclarationNode{
public:
  CVarDeclarationRuleNode(CTypeNode* _type, const char* _ident): type(_type), ident(string(_ident)){}
  ~CVarDeclarationRuleNode(){
    delete type;
  }
  void accept( CVisitor* v ){
    v->visit(this);
  }

  CTypeNode* type;
  string ident;
};

class CMethodDeclarationRuleNode : public CMethodDeclarationNode{
public:
  CMethodDeclarationRuleNode(CTypeNode* _type, const char* _ident,
    CParamArgNode* _param_arg, CMethodBodyNode* _method_body,
    CExpressionNode* _return_exp):
    type(_type), ident(string(_ident)), param_arg(_param_arg),
    method_body(_method_body), return_exp(_return_exp){}
  ~CMethodDeclarationRuleNode(){
    delete type;
    delete param_arg;
    delete method_body;
    delete return_exp;
  }
  void accept( CVisitor* v ){
    v->visit(this);
  }

  CTypeNode* type;
  string ident;
  CParamArgNode* param_arg;
  CMethodBodyNode* method_body;
  CExpressionNode* return_exp;
};

class CVarsDecListNode : public CVarsDecNode{
public:
  CVarsDecListNode(CVarsDecNode* _list, CVarDeclarationNode* _next):
    list(_list), next(_next){}
  ~CVarsDecListNode(){
    delete list;
    delete next;
  }
  void accept( CVisitor* v ){
    v->visit(this);
  }

  CVarsDecNode* list;
  CVarDeclarationNode* next;
};

class CVarsDecFirstNode : public CVarsDecNode{
public:
  CVarsDecFirstNode(CVarDeclarationNode* _first):
    first(_first){}
  ~CVarsDecFirstNode(){
    delete first;
  }
  void accept( CVisitor* v ){
    v->visit(this);
  }

  CVarDeclarationNode* first;
};

class CStatsFirstNode: public CStatsNode {
public:
    CStatsFirstNode(CStatementNode* _stm) : stm(_stm) {}
    ~CStatsFirstNode(){
      delete stm;
    }
    void accept( CVisitor* v){
        v->visit(this);
    }

    CStatementNode* stm;
};

class CStatsListNode: public CStatsNode {
public:
    CStatsListNode(CStatsNode* _list,  CStatementNode* _stm) : list(_list), stm(_stm) {}
    ~CStatsListNode(){
      delete stm;
      delete list;
    }
    void accept( CVisitor* v){
        v->visit(this);
    }

    CStatsNode* list;
    CStatementNode* stm;
};

class CMethodBodyVarsNode: public CMethodBodyNode {
public:
    CMethodBodyVarsNode(CVarsDecNode* _vars) : vars(_vars){}
    ~CMethodBodyVarsNode(){
      delete vars;
    }
    void accept( CVisitor* v){
        v->visit(this);
    }

    CVarsDecNode* vars;
};

class CMethodBodyStatsNode: public CMethodBodyNode {
public:
    CMethodBodyStatsNode(CStatsNode* _stats) : stats(_stats){}
    ~CMethodBodyStatsNode(){
      delete stats;
    }
    void accept( CVisitor* v){
        v->visit(this);
    }

    CStatsNode* stats;
};

class CMethodBodyAllNode: public CMethodBodyNode {
public:
    CMethodBodyAllNode(CVarsDecNode* _vars, CStatsNode* _stats) :
    vars(_vars), stats(_stats){}
    ~CMethodBodyAllNode(){
      delete vars;
      delete stats;
    }
    void accept( CVisitor* v){
        v->visit(this);
    }

    CVarsDecNode* vars;
    CStatsNode* stats;
};

class CMethodBodyEmptyNode: public CMethodBodyNode {
public:
    CMethodBodyEmptyNode(){}
    void accept( CVisitor* v){
        v->visit(this);
    }
};

class CParamArgListNode: public CParamArgNode {
public:
    CParamArgListNode(CParamsNode* _params) : params(_params){}
    ~CParamArgListNode(){
      delete params;
    }
    void accept( CVisitor* v){
        v->visit(this);
    }

    CParamsNode* params;
};

class CParamArgEmptyNode: public CParamArgNode {
public:
    CParamArgEmptyNode(){}
    void accept( CVisitor* v){
        v->visit(this);
    }
};

class CParamsOneNode: public CParamsNode {
public:
    CParamsOneNode(CParamNode* _param) : param(_param){}
    ~CParamsOneNode(){
      delete param;
    }
    void accept( CVisitor* v){
        v->visit(this);
    }

    CParamNode* param;
};

class CParamsTwoNode: public CParamsNode {
public:
    CParamsTwoNode(CParamsNode* _first, CParamNode* _second):
      first(_first), second(_second){}
    ~CParamsTwoNode(){
      delete first;
      delete second;
    }
    void accept( CVisitor* v){
        v->visit(this);
    }

    CParamsNode* first;
    CParamNode* second;
};

class CParamRuleNode: public CParamNode {
public:
    CParamRuleNode(CTypeNode* _type, const char* _ident) :
        type(_type), ident(string(_ident)){}
    ~CParamRuleNode(){
      delete type;
    }
    void accept( CVisitor* v){
        v->visit(this);
    }

    CTypeNode* type;
    string ident;
};

class CTypeRuleNode: public CTypeNode {
public:
    CTypeRuleNode(const char* _type): type(_type){}
    void accept( CVisitor* v){
        v->visit(this);
    }

    string type;
};

/// Statements begin
class CNumerousStatementsNode : public CStatementsNode{
public:
		void accept( CVisitor* v ){
			v->visit(this);
		}
		CNumerousStatementsNode(CStatementsNode* _statements, CStatementNode* _statement):
        statements(_statements), statement(_statement){}
    ~CNumerousStatementsNode(){
      delete statements;
      delete statement;
    }


		CStatementsNode* statements;
		CStatementNode* statement;
};

class CEmptyStatementsNode : public CStatementsNode{
public:
		void accept( CVisitor* v ){
			v->visit(this);
		}
};

class CBracedStatementNode : public CStatementNode{
public:
		void accept( CVisitor* v ){
			v->visit(this);
		}
		CBracedStatementNode(CStatementsNode* _statements):statements(_statements){}
    ~CBracedStatementNode(){
      delete statements;
    }


		CStatementsNode* statements;
};

class CIfStatementNode : public CStatementNode{
public:
		void accept( CVisitor* v ){
			v->visit(this);
		}
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

class CWhileStatementNode : public CStatementNode{
public:
		void accept( CVisitor* v ){
			v->visit(this);
		}
		CWhileStatementNode(CExpressionNode* _expression, CStatementNode* _statement):expression(_expression), statement(_statement){}
    ~CWhileStatementNode(){
      delete expression;
      delete statement;
    }

		CExpressionNode* expression;
		CStatementNode* statement;

};

class CPrintStatementNode : public CStatementNode{
public:
		void accept( CVisitor* v ){
			v->visit(this);
		}
		CPrintStatementNode(CExpressionNode* _expression):expression(_expression){}
    ~CPrintStatementNode(){
      delete expression;
    }

		CExpressionNode* expression;

};

class CAssignStatementNode : public CStatementNode{
public:
		void accept( CVisitor* v ){
			v->visit(this);
		}

		CAssignStatementNode(CExpressionNode* _expression, const char* ident):expression(_expression), identifier(string(ident)){}
    ~CAssignStatementNode(){
      delete expression;
    }

		CExpressionNode* expression;
		string identifier;
};

class CInvokeExpressionStatementNode : public CStatementNode{
public:
		void accept( CVisitor* v ){
			v->visit(this);
		}

		CInvokeExpressionStatementNode(CExpressionNode* _firstexpression, CExpressionNode* _secondexpression, const char* ident):
          firstexpression(_firstexpression), secondexpression(_secondexpression), identifier(string(ident)){}
    ~CInvokeExpressionStatementNode(){
      delete firstexpression;
      delete secondexpression;
    }

		CExpressionNode* firstexpression;
		CExpressionNode* secondexpression;
		string identifier;

};


/// Expressions begin

class CInvokeExpressionNode: public CExpressionNode {
public:
    CInvokeExpressionNode(CExpressionNode* _firstExp, CExpressionNode* _secondExp) : firstExp(_firstExp), secondExp(_secondExp) {}
    ~CInvokeExpressionNode(){
        delete firstExp;
        delete secondExp;
    }
    void accept( CVisitor* v ){
        v->visit(this);
    }

    CExpressionNode* firstExp;
    CExpressionNode* secondExp;
};

class CLengthExpressionNode: public CExpressionNode {
public:
    CLengthExpressionNode(CExpressionNode* _exp) : expr(_exp) {}
    ~CLengthExpressionNode(){
        delete expr;
    }
    void accept( CVisitor* v ){
        v->visit(this);
    }

    CExpressionNode* expr;
};

enum ArithmeticOpType {
    PLUS_OP, MINUS_OP, MULT_OP, DIV_OP, AND_OP
};

class CArithmeticExpressionNode: public CExpressionNode {
public:
    CArithmeticExpressionNode(CExpressionNode* _firstExp, CExpressionNode* _secondExp, ArithmeticOpType _opType) :
        firstExp(_firstExp), secondExp(_secondExp), opType(_opType) {}
    ~CArithmeticExpressionNode(){
        delete firstExp;
        delete secondExp;
    }
    void accept( CVisitor* v ){
        v->visit(this);
    }

    CExpressionNode* firstExp;
    CExpressionNode* secondExp;
    ArithmeticOpType opType;
};

enum UnaryOpType {
    UPLUS_OP, UMINUS_OP
};

class CUnaryExpressionNode: public CExpressionNode {
public:
    CUnaryExpressionNode(CExpressionNode* _exp, UnaryOpType _op) : expr(_exp), op(_op){}
    ~CUnaryExpressionNode(){
        delete expr;
    }
    void accept( CVisitor* v ){
        v->visit(this);
    }

    CExpressionNode* expr;
    UnaryOpType op;
};

class CCompareExpressionNode: public CExpressionNode {
public:
    CCompareExpressionNode(CExpressionNode* _firstExp, CExpressionNode* _secondExp) : firstExp(_firstExp), secondExp(_secondExp) {}
    ~CCompareExpressionNode(){
        delete firstExp;
        delete secondExp;
    }
    void accept( CVisitor* v ){
        v->visit(this);
    }

    CExpressionNode* firstExp;
    CExpressionNode* secondExp;
};

class CNotExpressionNode: public CExpressionNode {
public:
    CNotExpressionNode(CExpressionNode* _exp) : expr(_exp) {}
    ~CNotExpressionNode(){
        delete expr;
    }
    void accept( CVisitor* v ){
        v->visit(this);
    }

    CExpressionNode* expr;
};

class CNewArrayExpressionNode: public CExpressionNode {
public:
    CNewArrayExpressionNode(CExpressionNode* _exp) : expr(_exp) {}
    ~CNewArrayExpressionNode(){
        delete expr;
    }
    void accept( CVisitor* v ){
        v->visit(this);
    }

    CExpressionNode* expr;
};

class CNewObjectExpressionNode: public CExpressionNode {
public:
    CNewObjectExpressionNode(const char* _objType) : objType(_objType) {}
    void accept( CVisitor* v ){
        v->visit(this);
    }

    string objType;
};

class CIntExpressionNode: public CExpressionNode {
public:
    CIntExpressionNode(int _value) : value(_value) {}
    void accept( CVisitor* v){
        v->visit(this);
    }

    int value;
};

class CBooleanExpressionNode: public CExpressionNode {
public:
    CBooleanExpressionNode(bool _value) : value(_value) {}
    void accept( CVisitor* v){
        v->visit(this);
    }

    bool value;
};

class CIdentExpressionNode: public CExpressionNode {
public:
    CIdentExpressionNode(const char* _name) : name(_name) {}
    void accept( CVisitor* v){
        v->visit(this);
    }

    string name;
};

class CThisExpressionNode: public CExpressionNode {
public:
    CThisExpressionNode(const char* _name) : name(_name) {}
    void accept( CVisitor* v){
        v->visit(this);
    }

    string name;
};

class CParenExpressionNode: public CExpressionNode {
public:
    CParenExpressionNode(CExpressionNode* _exp) : expr(_exp) {}
    ~CParenExpressionNode(){
        delete expr;
    }
    void accept( CVisitor* v){
        v->visit(this);
    }

    CExpressionNode* expr;
};

class CInvokeMethodExpressionNode: public CExpressionNode {
public:
    CInvokeMethodExpressionNode(CExpressionNode* _exp, const char* _name, CExpArgNode* _args):
                                                    expr(_exp), name(_name), args(_args) {}
    ~CInvokeMethodExpressionNode(){
        delete expr;
        delete args;
    }
    void accept( CVisitor* v){
        v->visit(this);
    }

    CExpressionNode* expr;
    string name;
    CExpArgNode* args;
};


class CFewArgsExpressionNode: public CExpArgNode {
public:
    CFewArgsExpressionNode(CExpressionsNode* _exp) : expr(_exp) {}
    ~CFewArgsExpressionNode(){
        delete expr;
    }
    void accept( CVisitor* v){
       v->visit(this);
    }

    CExpressionsNode* expr;
};

class CEmptyArgsExpression: public CExpArgNode {
public:
    CEmptyArgsExpression() {}
    void accept( CVisitor* v){
      v->visit(this);
    }
};

class CListExpressionNode: public CExpressionsNode {
public:
    CListExpressionNode(CExpressionsNode* _prevExps, CExpressionNode* _nextExp):
                        prevExps(_prevExps), nextExp(_nextExp) {}
    ~CListExpressionNode(){
        delete prevExps;
        delete nextExp;
    }
    void accept( CVisitor* v){
      v->visit(this);
    }

    CExpressionsNode* prevExps;
    CExpressionNode* nextExp;
};

class CLastListExpressionNode: public CExpressionsNode {
public:
    CLastListExpressionNode(CExpressionNode* _exp) : expr(_exp) {}
    ~CLastListExpressionNode(){
        delete expr;
    }
    void accept( CVisitor* v){
        v->visit(this);
    }

    CExpressionNode* expr;
};

#endif
