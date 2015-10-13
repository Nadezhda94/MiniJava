
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



