#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED
#include "CPrintVisitor.h"

struct CProgramNode {
  virtual void accept() = 0;
};

struct CMainClassNode {
  virtual void accept() = 0;
};

struct CDeclarationsNode {
  virtual void accept() = 0;
};

struct CClassDeclarationNode {
  virtual void accept() = 0;
};

struct CExtendDeclarationNode {
  virtual void accept() = 0;
};

struct CVarDeclarationsNode {
  virtual void accept() = 0;
};

struct CMethodDeclarationsNode {
  virtual void accept() = 0;
};

struct CVarDeclarationNode {
  virtual void accept() = 0;
};

struct CMethodDeclarationNode {
  virtual void accept() = 0;
};

struct CVarsDecNode {
  virtual void accept() = 0;
};

struct CStatsNode {
  virtual void accept() = 0;
};

struct CMethodBodyNode {
  virtual void accept() = 0;
};

struct CParamArgNode {
  virtual void accept() = 0;
};

struct CParamsNode {
  virtual void accept() = 0;
};

struct CParamNode {
  virtual void accept() = 0;
};

struct CTypeNode {
  virtual void accept() = 0;
};

struct CStatemntsNode {
  virtual void accept() = 0;
};

struct CStatementNode {
  virtual void accept() = 0;
};

struct CIfStatementNode {
  virtual void accept() = 0;
};

struct CWhileStatementNode {
  virtual void accept() = 0;
};

struct CPrintStatementNode {
  virtual void accept() = 0;
};

struct CAssignStatementNode {
  virtual void accept() = 0;
};

struct CInvokeExpStatementNode {
  virtual void accept() = 0;
};

struct CBinOpNode {
  virtual void accept() = 0;
};

struct CExpressionNode {
  virtual void accept() = 0;
};

struct CExpArgNode {
  virtual void accept() = 0;
};

struct CExpressionsNode {
  virtual void accept() = 0;
};


class CProgramRuleNode: public CProgramNode {
public:
	CProgramRuleNode( const CMainClassNode* _mainClass, const CDeclarationsNode* _decl) :
		mainClass(_mainClass), decl(_decl) {}

	void accept( const CVisitor* v ) {
		v->visit( this )
	}

private:
	const CMainClassNode* mainClass;
	const CDeclarationsNode* decl;
};

class CMainClassDeclarationRuleNode: public CMainClassNode {
public:
	CMainClassDeclarationRuleNode( const char* _className, const char* _argNames, const CStatementNode* _stmt ) :
		className(_className), argNames(_argNames), stmt(_stmt) {}

	void accept( const CVisitor* v ) {
		v->visit( this )
	}

private:
	string className;
	string argNames;
	const CStatementNode* stmt;
};

class CDeclarationsRuleNode: public CDeclarationsNode {
public:
	CDeclarationsRuleNode( const CDeclarationsNode* _decl, const CClassNode* _cl ) :
		decl(_decl), cl(_cl) {}

	void accept( const CVisitor* v ) {
		v->visit( this )
	}
private:
	const CDeclarationsNode* decl;
	const CClassNode* cl;
};

class CClassDeclarationRuleNode: public CClassNode {
public:
	CClassDeclarationRuleNode( const char* _ident, const CExtendDeclarationNode* _extDecl, const CVarDeclarationsNode* vars, const CMethodDeclarationsNode* method ) :
		ident(_ident), extDecl(_extDecl), vars(_vars), method(_method) {}

	void accept( const CVisitor* v ) {
		v->visit( this )
	}

private:
	string ident;
	const CExtendDeclarationNode *extDecl;
	const CVarDeclarationsNode *vars;
	const CMethodDeclarationsNode *method;
};

class CExtendDeclarationRuleNode: public CExtendDeclarationNode {
public:
	CExtendDeclarationRuleNode( const char* _ident ) : ident(_ident) {}

	void accept( const CVisitor* v ) {
		v->visit( this )
	}
private:
	string ident;
};

class CVarDeclarationsListNode : public CVarDeclarationsNode{
public:
  CVarDeclarationsListNode(CVarDeclarationsNode* _first, CVarDeclarationNode* _second):
    list(_first), item(_second) {}
  void accept( const CVisitor* v ){
    v->visit(this);
  }
private:
  CVarDeclarationsNode* list;
  CVarDeclarationNode* item;
};

class CVarDeclarationsEmptyNode : public CVarDeclarationsNode{
public:
   CVarDeclarationsEmptyNode(){}
  void accept( const CVisitor* v ){
    v->visit(this);
  }
};

class CMethodDeclarationsListNode : public CMethodDeclarationsNode{
public:
  CMethodDeclarationsListNode(CMethodDeclarationsNode* _first, CMethodDeclarationNode* _second):
    list(_first), item(_second) {}
  void accept( const CVisitor* v ){
    v->visit(this);
  }
private:
  CMethodDeclarationsNode* list;
  CMethodDeclarationNode* item;
};

class CMethodDeclarationsEmptyNode : public CMethodDeclarationsNode{
public:
  CMethodDeclarationsEmptyNode(){}
  void accept( const CVisitor* v ){
    v->visit(this);
  }
};

class CVarDeclarationRuleNode : public CVarDeclarationNode{
public:
  CVarDeclarationRuleNode(CTypeNode* _type, const char* _ident): type(_type), ident(string(_ident)){}
  void accept( const CVisitor* v ){
    v->visit(this);
  }
private:
  CTypeNode* type;
  string ident;
};

class CMethodDeclarationRuleNode : CMethodDeclarationNode{
public:
  CVarDeclarationRuleNode(CTypeNode* _type, CParamArgNode* _param_arg,
    CMethodBodyNode* _method_body, CExpressionNode* _return_exp):
    type(_type), param_arg(_param_arg), method_body(_method_body),
    return_exp(_return_exp): param_arg(_param_arg), type(_type),
    method_body(_method_body), return_exp(_return_exp){}
  void accept( const CVisitor* v ){
    v->visit(this);
  }
private:
  CTypeNode* type;
  CParamArgNode* param_arg;
  CMethodBodyNode* method_body;
  CExpressionNode* return_exp;
};

class CVarsDecListNode : public CVarsDecNode{
public:
  CVarsDecListNode(CVarsDec* _list, CVarDeclarationNode* _next):
    list(_list), next(_next){}
  void accept( const CVisitor* v ){
    v->visit(this);
  }
private:
  CVarsDec* list;
  CVarDeclarationNode* next;
};

class CVarsDecFirstNode : public CVarsDecNode{
public:
  CVarsDecFirstNode(CVarDeclarationNode* _first):
    first(_first){}
  void accept( const CVisitor* v ){
    v->visit(this);
  }
private:
  CVarDeclarationNode* first;
};

class CStatsTailNode: public CStatsNode {
public:
    CStatsTailNode(CStatementNode* _stm) : stm(_stm) {}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    CStatementNode* stm;
};

class CStatsFirstNode: public CStatsNode {
public:
    CStatsFirstNode(CStatsNode* _firstStats,  CStatementNode* _stm) : firstStats(_firstStats), stm(_stm) {}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    CStatsNode* firstStats;
    CStatementNode* stm;
};

class CMethodBodyVarsNode: public CMethodBodyNode {
public:
    CMethodBodyVarsNode(CVarsDecNode* _vars) : vars(_vars){}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    CVarsDecNode* vars;
};

class CMethodBodyStatsNode: public CMethodBodyNode {
public:
    CMethodBodyStatsNode(CStatsNode* _stats) : stats(_stats){}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    CStatsNode* stats;
};

class CMethodBodyAllNode: public CMethodBodyNode {
public:
    CMethodBodyAllNode(CVarsDecNode* _vars, CStatsNode* _stats) :
    vars(_vars), stats(_stats){}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    CVarsDecNode* vars;
    CStatsNode* stats;
};

class CParamArgListNode: public CParamArgNode {
public:
    CParamArgListNode(CParamsNode* _params) : params(_params){}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    CParamsNode* params;
};

class CParamArgEmptyNode: public CParamArgNode {
public:
    CParamArgEmptyNode(){}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
};

class CParamsOneNode: public CParamsNode {
public:
    CParamsOneNode(CParamNode* _param) : param(_param){}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    CParamNode* param;
};

class CParamsTwoNode: public CParamsNode {
public:
    CParamsTwoNode(CParamNode* _first, CParamNode* _second):
      first(_first), second(_second){}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    CParamNode* first;
    CParamNode* second;
};

class CParamRuleNode: public CParamNode {
public:
    CParamRuleNode(CTypeNode* _type, char* _ident) :
    type(_type), ident(string(_ident)){}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    CTypeNode* type;
    string ident;
};

class CTypeRuleNode: public CTypeNode {
public:
    CTypeRuleNode(char* _type): type(_type){}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    string type;
};

/// Statements begin
class CNumerousStatementsNode : public CStatementsNode{
public:
		void accept( const CVisitor* v ){
			v->visit(this);
		}
		CNumerousStatementNode(CStatementsNode* _statements, CStatementNode* _statement):
    statements(_statements), statement(_statement){}

private:
		CStatementsNode* statements;
		CStatementNode* statement;
};

class CEmptyStatementsNode : public CStatementsNode{
public:
		void accept( const CVisitor* v ){
			v->visit(this);
		}
};

class CBracedStatementNode : public CStatementNode{
public:
		void accept( const CVisitor* v ){
			v->visit(this);
		}
		CBracedStatementNode(CStatementsNode* _statements):statements(_statements){}

private:
		CStatementsNode* statements;
};

class CIfStatementNode : public CStatementNode{
public:
		void accept( const CVisitor* v ){
			v->visit(this);
		}
		CIfStatementNode(CExpressionNode* _expression, CStatementNode* _thenStatement, CStatementNode* _elseStatement):expression(_expression), thenStatement(_thenStatement), elseStatement(_elseStatement){}
private:
		CExpressionNode* expression;
		CStatementNode* thenStatement;
		CStatementNode* elseStatement;

};

class CWhileStatementNode : public CStatementNode{
public:
		void accept( const CVisitor* v ){
			v->visit(this);
		}
		CWhileStatementNode(CExpressionNode* _expression, CStatementNode* _statement):expression(_expression), statement(_statement){}
private:
		CExpressionNode* expression;
		CStatementNode* statement;

};

class CPrintStatementNode : public CStatementNode{
public:
		void accept( const CVisitor* v ){
			v->visit(this);
		}
		CPrintStatementNode(CExpressionNode* _expression):expression(_expression){}
private:
		CExpressionNode* expression;

};

class CAssignStatementNode : public CStatementNode{
public:
		void accept( const CVisitor* v ){
			v->visit(this);
		}

		CAssignStatementNode(CExpressionNode* _expression, char* ident):expression(_expression), identifier(string(ident)){}
private:
		CExpressionNode* expression;
		string identifier;

};

class CInvokeExpressionStatementNode : public CStatementNode{
public:
		void accept( const CVisitor* v ){
			v->visit(this);
		}

		CInvokeExpressionStatementNode(CExpressionNode* _firstexpression, CExpressionNode* _secondexpression, char* ident):firstexpression(_firstexpression), secondexpression(_secondexpression), identifier(string(ident)){}
private:
		CExpressionNode* firstexpression;
		CExpressionNode* secondexpression;
		string identifier;

};


/// Expressions begin

class CInvokeExpressionNode: public CExpressionNode {
public:
    CInvokeExpressionNode(CExpressionNode* _firstExp, CExpressionNode* _secondExp) : firstExp(_firstExp), secondExp(_secondExp) {}
    void accept(const CVisitor* visit) {
       visitor->visit(this);
    }
private:
    CExpressionNode* firstExp;
    CExpressionNode* secondExp;
};

class CLengthExpressionNode: public CExpressionNode {
public:
    CLengthExpressionNode(CExpressionNode* _exp) : exp(_exp) {}
    void accept(const CVisitor* visitor) {
       visitor->visit(this);
    }
private:
    CExpressionNode* exp;
};

enum ArithmeticOpType {
    PLUS, MINUS, MULT, DIV
};

class CArithmeticExpressionNode: public CExpressionNode {
public:
    CArithmeticExpressionNode(CExpressionNode* _firstExp, CExpressionNode* _secondExp, ArithmeticOpType opType) : firstExp(_firstExp), secondExp(_secondExp) {}
    void accept(const CVisitor* visit) {
       visitor->visit(this);
    }
private:
    CExpressionNode* firstExp;
    CExpressionNode* secondExp;
    ArithmeticOpType opType;
};

class CCompareExpressionNode: public CExpressionNode {
public:
    CCompareExpressionNode(CExpressionNode* _firstExp, CExpressionNode* _secondExp) : firstExp(_firstExp), secondExp(_secondExp) {}
    void accept(const CVisitor* visit) {
        visitor->visit(this);
    }
private:
    CExpressionNode* firstExp;
    CExpressionNode* secondExp;
};

class CNotExpressionNode: public CExpressionNode {
public:
    CNotExpressionNode(CExpressionNode* _exp) : exp(_exp) {}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    CExpressionNode* exp;
};

class CNewArrayExpressionNode: public CExpressionNode {
public:
    CNewArrayExpressionNode(CExpressionNode* _exp) : exp(_exp) {}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    CExpressionNode* exp;
};

class CNewObjectExpressionNode: public CExpressionNode {
public:
    CNewObjectExpressionNode(const char* _objType) : objType(_objType) {}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    string objType;
};

class CIntExpressionNode: public CExpressionNode {
public:
    CIntExpressionNode(int _value) : value(_value) {}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    int value;
};

class CBooleanExpressionNode: public CExpressionNode {
public:
    CNewObjectExpressionNode(bool _value) : value(_value) {}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    bool value;
};

class CIdentExpressionNode: public CExpressionNode {
public:
    CIdentExpressionNode(char* _name) : name(_name) {}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    string name;
};

class CThisExpressionNode: public CExpressionNode {
public:
    CThisExpressionNode(const char* _name) : name(_name) {}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    string name;
};

class CParenExpressionNode: public CExpressionNode {
public:
    CParenExpressionNode(CExpressionNode* _exp) : exp(_exp) {}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    CExpressionNode* exp;
};

class CInvokeMethodExpressionNode: public CExpressionNode {
public:
    CInvokeExpressionNode(CExpressionNode* _exp, const char* _name, CExpArgNode* _args):
                                                    exp(_exp), name(_name), args(_args) {}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    CExpressionNode exp;
    string name;
    CExpArgNode args;
};


class CFewArgsExpressionNode: public CExpArgNode {
public:
    CFewArgsExpressionNode(CExpressionsNode* _exp) : exp(_exp) {}
    void accept(const CVisitor* visitor) {
       visitor->visit(this);
    }
private:
    CExpressionNode* exp;
};

class CEmptyArgsExpression: public CExpArgNode {
public:
    CEmptyArgsExpression() {}
    void accept(const CVisitor* visitor) {
      visitor->visit(this);
    }
};

class CListExpressionNode: public CExpressionsNode {
public:
    CListExpressionNode(CExpressionNode* _prevExps, CExpressionNode* _nextExp):
                                                            prevExps(_prevExps), nextExp(_nextExp) {}
    void accept(const CVisitor* visitor) {
      visitor->visit(this);
    }
private:
    CExpressionNode* prevExps;
    CExpressionNode* nextExp;
};

class CLastListExpressionNode: public CExpressionsNode {
public:
    CLastListExpressionNode(CExpressionNode* _exp) : exp(_exp) {}
    void accept(const CVisitor* visitor) {
      visitor->visit(this);
    }
private:
    CExpressionNode* exp;
};

#endif
