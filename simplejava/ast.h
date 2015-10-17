struct CNode {
  virtual void accept() = 0;
};

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

	void accept( CVisitor* v ) {
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

	void accept( CVisitor* v ) {
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

	void accept( CVisitor* v ) {
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

	void accept( CVisitor* v ) {
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

	void accept( CVisitor* v ) {
		v->visit( this )
	}
private:
	string ident;
};

class CVarDeclarationsRuleNode{
public:
  CVarDeclarationsRuleNode(CVarDeclarationsNode* _first, CVarDeclarationNode* _second):
    list(_first), item(_second) {}
  void accept( CVisitor* v ){
    v->visit(this);
  }
private:
  CVarDeclarationsNode* list;
  CVarDeclarationNode* item;
};

class CVarDeclarationsEmptyRuleNode{
public:
   CVarDeclarationsEmptyRuleNode(){}
  void accept( CVisitor* v ){
    v->visit(this);
  }
};

class CMethodDeclarationsRuleNode{
public:
  CMethodDeclarationsRuleNode(CMethodDeclarationsNode* _first, CMethodDeclarationNode* _second):
    list(_first), item(_second) {}
  void accept( CVisitor* v ){
    v->visit(this);
  }
private:
  CMethodDeclarationsNode* list;
  CMethodDeclarationNode* item;
};

class CMethodDeclarationsEmptyRuleNode{
public:
  CMethodDeclarationsEmptyRuleNode(){}
  void accept( CVisitor* v ){
    v->visit(this);
  }
};

class CVarDeclarationRuleNode{
public:
  CVarDeclarationRuleNode(CTypeNode* _type, const char* _ident): type(_type), ident(string(_ident)){}
  void accept( CVisitor* v ){
    v->visit(this);
  }
private:
  CTypeNode* type;
  string ident;
};

class CMethodDeclarationRuleNode{
public:
  CVarDeclarationRuleNode(CTypeNode* _type, CParamArgNode* _param_arg,
    CMethodBodyNode* _method_body, CExpressionNode* _return_exp):
    type(_type), param_arg(_param_arg), method_body(_method_body),
    return_exp(_return_exp): param_arg(_param_arg), type(_type),
    method_body(_method_body), return_exp(_return_exp){}
  void accept( CVisitor* v ){
    v->visit(this);
  }
private:
  CTypeNode* type;
  CParamArgNode* param_arg;
  CMethodBodyNode* method_body;
  CExpressionNode* return_exp;
};

class CVarsDecListRuleNode{
public:
  CVarDeclarationRuleNode(CVarsDec* _list, CVarDeclarationNode* _next):
    list(_list), next(_next){}
  void accept( CVisitor* v ){
    v->visit(this);
  }
private:
  CVarsDec* list;
  CVarDeclarationNode* next;
};

class CVarsDecFirstRuleNode{
public:
  CVarDeclarationRuleNode(CVarDeclarationNode* _first):
    first(_first){}
  void accept( CVisitor* v ){
    v->visit(this);
  }
private:
  CVarDeclarationNode* first;
};

class CNumerousStatementsNode : public CStatementsNode{
public:
		void accept( CVisitor* v ){
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

private:
		CStatementsNode* statements;
};

class CIfStatementNode : public CStatementNode{
public:
		void accept( CVisitor* v ){
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
		void accept( CVisitor* v ){
			v->visit(this);
		}
		CWhileStatementNode(CExpressionNode* _expression, CStatementNode* _statement):expression(_expression), statement(_statement){}
private:
		CExpressionNode* expression;
		CStatementNode* statement;

};

class CPrintStatementNode : public CStatementNode{
public:

		void accept( CVisitor* v ){
			v->visit(this);
		}
		CPrintStatementNode(CExpressionNode* _expression):expression(_expression){}
private:
		CExpressionNode* expression;

};

class CAssignStatementNode : public CStatementNode{
public:

		void accept( CVisitor* v ){
			v->visit(this);
		}

		CAssignStatementNode(CExpressionNode* _expression, char* ident):expression(_expression), identifier(string(ident)){}
private:
		CExpressionNode* expression;
		string identifier;

};

class CInvokeExpressionStatementNode : public CStatementNode{
public:

		void accept( CVisitor* v ){
			v->visit(this);
		}

		CInvokeExpressionStatementNode(CExpressionNode* _firstexpression, CExpressionNode* _secondexpression, char* ident):firstexpression(_firstexpression), secondexpression(_secondexpression), identifier(string(ident)){}
private:
		CExpressionNode* firstexpression;
		CExpressionNode* secondexpression;
		string identifier;

};
