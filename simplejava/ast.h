class CNumerousStatementsNode : public CStatementsNode{
public:
		void accept( CVisitor* v ){
			v->visit(this);
		}
		CNumerousStatementNode(CStatementsNode* _statements, CStatementNode* _statement):statements(_statements), statement(_statement){}
		
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

