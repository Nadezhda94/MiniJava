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
    return_exp(_return_exp):
  void accept( CVisitor* v ){
    v->visit(this);
  }
private:
  CTypeNode* type;
  CParamArgNode* param_arg;
  CMethodBodyNode* method_body;
  CExpressionNode* return_exp;
};
