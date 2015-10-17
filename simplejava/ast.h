#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED
#include <string>
#include "CPrintVisitor.h"
using std::string;

class CProgramRuleNode;
class CMainClassDeclarationRuleNode;
class CDeclarationsRuleNode;
class CClassDeclarationRuleNode;
class CExtendDeclarationRuleNode;
class CVarDeclarationsListNode;
class CVarDeclarationsEmptyNode;
class CMethodDeclarationsListNode;
class CMethodDeclarationsEmptyNode;
class CVarDeclarationRuleNode;
class CMethodDeclarationRuleNode;
class CVarsDecListNode;
class CVarsDecFirstNode;
class CStatsTailNode;
class CStatsFirstNode;
class CMethodBodyVarsNode;
class CMethodBodyStatsNode;
class CMethodBodyAllNode;
class CParamArgListNode;
class CParamArgEmptyNode;
class CParamsOneNode;
class CParamsTwoNode;
class CParamRuleNode;
class CTypeRuleNode;
class CNumerousStatementsNode;
class CEmtptyStatementsNode;
class CBracedStatementNode;
class CIfStatementNodeNode;
class CWhileStatementNode;
class CPrintStatementNode;
class CAssignStatementNode;
class CInvokeExpressionStatementNode;
class CInvokeExpressionNode;
class CLengthExpressionNode;
class CAriphmeticExpressionNode;
class CCompareExpressionNode;
class CNotExpressionNode;
class CNewArrayExpressionNode;
class CNewObjectExpressionNode;
class CIntExpressionNode;
class CBooleanExpressionNode;
class CIdentExpressionNode;
class CThisExpressionNode;
class CParenExpressionNode;
class CInvokeMethodExpressionNode;
class CFewArgsExpressionNode;
class CEmptyArgsExpression;
class CListExpressionNode;
class CLastListExpressionNode;

struct CProgramNode {
  virtual void accept(const CVisitor*) = 0;
};

struct CMainClassNode {
  virtual void accept(const CVisitor*) = 0;
};

struct CDeclarationsNode {
  virtual void accept(const CVisitor*) = 0;
};

struct CClassDeclarationNode {
  virtual void accept(const CVisitor*) = 0;
};

struct CExtendDeclarationNode {
  virtual void accept(const CVisitor*) = 0;
};

struct CVarDeclarationsNode {
  virtual void accept(const CVisitor*) = 0;
};

struct CMethodDeclarationsNode {
  virtual void accept(const CVisitor*) = 0;
};

struct CVarDeclarationNode {
  virtual void accept(const CVisitor*) = 0;
};

struct CMethodDeclarationNode {
  virtual void accept(const CVisitor*) = 0;
};

struct CVarsDecNode {
  virtual void accept(const CVisitor*) = 0;
};

struct CStatsNode {
  virtual void accept(const CVisitor*) = 0;
};

struct CMethodBodyNode {
  virtual void accept(const CVisitor*) = 0;
};

struct CParamArgNode {
  virtual void accept(const CVisitor*) = 0;
};

struct CParamsNode {
  virtual void accept(const CVisitor*) = 0;
};

struct CParamNode {
  virtual void accept(const CVisitor*) = 0;
};

struct CTypeNode {
  virtual void accept(const CVisitor*) = 0;
};

struct CStatementsNode {
  virtual void accept(const CVisitor*) = 0;
};

struct CStatementNode {
  virtual void accept(const CVisitor*) = 0;
};

struct CExpressionNode {
  virtual void accept(const CVisitor*) = 0;
};

struct CExpArgNode {
  virtual void accept(const CVisitor*) = 0;
};

struct CExpressionsNode {
  virtual void accept(const CVisitor*) = 0;
};


class CProgramRuleNode: public CProgramNode {
public:
	CProgramRuleNode( const CMainClassNode* _mainClass, const CDeclarationsNode* _decl) :
		mainClass(_mainClass), decl(_decl) {}

	void accept( const CVisitor* v ) {
		v->visit( this );
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
		v->visit( this );
	}

private:
	string className;
	string argNames;
	const CStatementNode* stmt;
};

class CDeclarationsRuleNode: public CDeclarationsNode {
public:
	CDeclarationsRuleNode( const CDeclarationsNode* _decl, const CClassDeclarationNode* _cl ) :
		decl(_decl), cl(_cl) {}

	void accept( const CVisitor* v ) {
		v->visit( this );
	}
private:
	const CDeclarationsNode* decl;
	const CClassDeclarationNode* cl;
};

class CClassDeclarationRuleNode: public CClassDeclarationNode {
public:
	CClassDeclarationRuleNode( const char* _ident, const CExtendDeclarationNode* _extDecl,
    const CVarDeclarationsNode* _vars, const CMethodDeclarationsNode* _method ) :
		ident(_ident), extDecl(_extDecl), vars(_vars), method(_method) {}

	void accept( const CVisitor* v ) {
		v->visit( this );
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
		v->visit( this );
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
  const CVarDeclarationsNode* list;
  const CVarDeclarationNode* item;
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
  const CMethodDeclarationsNode* list;
  const CMethodDeclarationNode* item;
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
  const CTypeNode* type;
  string ident;
};

class CMethodDeclarationRuleNode : CMethodDeclarationNode{
public:
  CMethodDeclarationRuleNode(CTypeNode* _type, CParamArgNode* _param_arg,
    CMethodBodyNode* _method_body, CExpressionNode* _return_exp):
    type(_type), param_arg(_param_arg), method_body(_method_body),
    return_exp(_return_exp){}
  void accept( const CVisitor* v ){
    v->visit(this);
  }
private:
  const CTypeNode* type;
  const CParamArgNode* param_arg;
  const CMethodBodyNode* method_body;
  const CExpressionNode* return_exp;
};

class CVarsDecListNode : public CVarsDecNode{
public:
  CVarsDecListNode(CVarsDecNode* _list, CVarDeclarationNode* _next):
    list(_list), next(_next){}
  void accept( const CVisitor* v ){
    v->visit(this);
  }
private:
  const CVarsDecNode* list;
  const CVarDeclarationNode* next;
};

class CVarsDecFirstNode : public CVarsDecNode{
public:
  CVarsDecFirstNode(CVarDeclarationNode* _first):
    first(_first){}
  void accept( const CVisitor* v ){
    v->visit(this);
  }
private:
  const CVarDeclarationNode* first;
};

class CStatsTailNode: public CStatsNode {
public:
    CStatsTailNode(CStatementNode* _stm) : stm(_stm) {}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    const CStatementNode* stm;
};

class CStatsFirstNode: public CStatsNode {
public:
    CStatsFirstNode(CStatsNode* _firstStats,  CStatementNode* _stm) : firstStats(_firstStats), stm(_stm) {}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    const CStatsNode* firstStats;
    const CStatementNode* stm;
};

class CMethodBodyVarsNode: public CMethodBodyNode {
public:
    CMethodBodyVarsNode(CVarsDecNode* _vars) : vars(_vars){}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    const CVarsDecNode* vars;
};

class CMethodBodyStatsNode: public CMethodBodyNode {
public:
    CMethodBodyStatsNode(CStatsNode* _stats) : stats(_stats){}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    const CStatsNode* stats;
};

class CMethodBodyAllNode: public CMethodBodyNode {
public:
    CMethodBodyAllNode(CVarsDecNode* _vars, CStatsNode* _stats) :
    vars(_vars), stats(_stats){}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    const CVarsDecNode* vars;
    const CStatsNode* stats;
};

class CParamArgListNode: public CParamArgNode {
public:
    CParamArgListNode(CParamsNode* _params) : params(_params){}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    const CParamsNode* params;
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
    const CParamNode* param;
};

class CParamsTwoNode: public CParamsNode {
public:
    CParamsTwoNode(CParamNode* _first, CParamNode* _second):
      first(_first), second(_second){}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    const CParamNode* first;
    const CParamNode* second;
};

class CParamRuleNode: public CParamNode {
public:
    CParamRuleNode(CTypeNode* _type, char* _ident) :
    type(_type), ident(string(_ident)){}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    const CTypeNode* type;
    string ident;
};

class CTypeRuleNode: public CTypeNode {
public:
    CTypeRuleNode(const char* _type): type(_type){}
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
		CNumerousStatementsNode(CStatementsNode* _statements, CStatementNode* _statement):
    statements(_statements), statement(_statement){}

private:
		const CStatementsNode* statements;
		const CStatementNode* statement;
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
		const CStatementsNode* statements;
};

class CIfStatementNode : public CStatementNode{
public:
		void accept( const CVisitor* v ){
			v->visit(this);
		}
		CIfStatementNode(CExpressionNode* _expression, CStatementNode* _thenStatement, CStatementNode* _elseStatement):expression(_expression), thenStatement(_thenStatement), elseStatement(_elseStatement){}
private:
		const CExpressionNode* expression;
		const CStatementNode* thenStatement;
		const CStatementNode* elseStatement;

};

class CWhileStatementNode : public CStatementNode{
public:
		void accept( const CVisitor* v ){
			v->visit(this);
		}
		CWhileStatementNode(CExpressionNode* _expression, CStatementNode* _statement):expression(_expression), statement(_statement){}
private:
		const CExpressionNode* expression;
		const CStatementNode* statement;

};

class CPrintStatementNode : public CStatementNode{
public:
		void accept( const CVisitor* v ){
			v->visit(this);
		}
		CPrintStatementNode(CExpressionNode* _expression):expression(_expression){}
private:
		const CExpressionNode* expression;

};

class CAssignStatementNode : public CStatementNode{
public:
		void accept( const CVisitor* v ){
			v->visit(this);
		}

		CAssignStatementNode(CExpressionNode* _expression, const char* ident):expression(_expression), identifier(string(ident)){}
private:
		const CExpressionNode* expression;
		string identifier;
};

class CInvokeExpressionStatementNode : public CStatementNode{
public:
		void accept( const CVisitor* v ){
			v->visit(this);
		}

		CInvokeExpressionStatementNode(CExpressionNode* _firstexpression, CExpressionNode* _secondexpression, const char* ident):
    firstexpression(_firstexpression), secondexpression(_secondexpression), identifier(string(ident)){}
private:
		const CExpressionNode* firstexpression;
		const CExpressionNode* secondexpression;
		string identifier;

};


/// Expressions begin

class CInvokeExpressionNode: public CExpressionNode {
public:
    CInvokeExpressionNode(CExpressionNode* _firstExp, CExpressionNode* _secondExp) : firstExp(_firstExp), secondExp(_secondExp) {}
    void accept(const CVisitor* v) {
       v->visit(this);
    }
private:
    const CExpressionNode* firstExp;
    const CExpressionNode* secondExp;
};

class CLengthExpressionNode: public CExpressionNode {
public:
    CLengthExpressionNode(CExpressionNode* _exp) : exp(_exp) {}
    void accept(const CVisitor* v) {
       v->visit(this);
    }
private:
    const CExpressionNode* exp;
};

enum ArithmeticOpType {
    PLUS_OP, MINUS_OP, MULT_OP, DIV_OP
};

class CArithmeticExpressionNode: public CExpressionNode {
public:
    CArithmeticExpressionNode(CExpressionNode* _firstExp, CExpressionNode* _secondExp, ArithmeticOpType _opType) :
    firstExp(_firstExp), secondExp(_secondExp), opType(_opType) {}
    void accept(const CVisitor* v) {
       v->visit(this);
    }
private:
    const CExpressionNode* firstExp;
    const CExpressionNode* secondExp;
    const ArithmeticOpType opType;
};

class CCompareExpressionNode: public CExpressionNode {
public:
    CCompareExpressionNode(CExpressionNode* _firstExp, CExpressionNode* _secondExp) : firstExp(_firstExp), secondExp(_secondExp) {}
    void accept(const CVisitor* v) {
        v->visit(this);
    }
private:
    const CExpressionNode* firstExp;
    const CExpressionNode* secondExp;
};

class CNotExpressionNode: public CExpressionNode {
public:
    CNotExpressionNode(CExpressionNode* _exp) : exp(_exp) {}
    void accept(const CVisitor* v) {
        v->visit(this);
    }
private:
    const CExpressionNode* exp;
};

class CNewArrayExpressionNode: public CExpressionNode {
public:
    CNewArrayExpressionNode(CExpressionNode* _exp) : exp(_exp) {}
    void accept(const CVisitor* v) {
        v->visit(this);
    }
private:
    const CExpressionNode* exp;
};

class CNewObjectExpressionNode: public CExpressionNode {
public:
    CNewObjectExpressionNode(const char* _objType) : objType(_objType) {}
    void accept(const CVisitor* v) {
        v->visit(this);
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
    CBooleanExpressionNode(bool _value) : value(_value) {}
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
    const CExpressionNode* exp;
};

class CInvokeMethodExpressionNode: public CExpressionNode {
public:
    CInvokeMethodExpressionNode(CExpressionNode* _exp, const char* _name, CExpArgNode* _args):
                                                    exp(_exp), name(_name), args(_args) {}
    void accept(const CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    const CExpressionNode* exp;
    string name;
    const CExpArgNode* args;
};


class CFewArgsExpressionNode: public CExpArgNode {
public:
    CFewArgsExpressionNode(CExpressionsNode* _exp) : exp(_exp) {}
    void accept(const CVisitor* visitor) {
       visitor->visit(this);
    }
private:
    const CExpressionsNode* exp;
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
    const CExpressionNode* prevExps;
    const CExpressionNode* nextExp;
};

class CLastListExpressionNode: public CExpressionsNode {
public:
    CLastListExpressionNode(CExpressionNode* _exp) : exp(_exp) {}
    void accept(const CVisitor* visitor) {
      visitor->visit(this);
    }
private:
    const CExpressionNode* exp;
};

#endif
