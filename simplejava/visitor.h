struct CVisitor{
  virtual void visit(const CAriphmeticExpressionNode* node) = 0;
  virtual void visit(const CCompareExpressionNode* node) = 0;
  virtual void visit(const CNotExpressionNode* node) = 0;
  virtual void visit(const CNewArrayExpressionNode* node) = 0;
  virtual void visit(const CObjectExpressionNode* node) = 0;
  virtual void visit(const CIdentExpressionNode* node) = 0;
  virtual void visit(const CThisExpressionNode* node) = 0;
  virtual void visit(const CParenExpressionNode* node) = 0;
  virtual void visit(const CTailStatsNode* node) = 0;
  virtual void visit(const CFirstStatsNode* node) = 0;

  virtual void visit(const CMainClassDeclarationRuleNode* node) = 0;
  virtual void visit(const CClassDeclarationRuleNode* node) = 0;
  virtual void visit(const CProgramRuleNode* node) = 0;
  virtual void visit(const CExtendDeclarationRuleNode* node) = 0;
  virtual void visit(const CDeclarationsRuleNode* node) = 0;

  virtual void visit(const CVarDeclarationsRuleNode* node) = 0;
  virtual void visit(const CVarDeclarationsEmptyRuleNode* node) = 0;
  virtual void visit(const CMethodDeclarationsRuleNode* node) = 0;
  virtual void visit(const CMethodDeclarationsEmptyRuleNode* node) = 0;
  virtual void visit(const CDeclarationsRuleNode* node) = 0;
  virtual void visit(const CVarDeclarationRuleNode* node) = 0;
  virtual void visit(const CMethodDeclarationRuleNode* node) = 0;



  void visit(const CNumerousStatementsRuleNode* node);
  void visit(const CEmtptyStatementsRuleNode* node);
}
