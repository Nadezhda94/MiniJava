#include "IRTree.h"
#include "CVisitor.h"
#include "Temp.h"

namespace Translate {

class ISubtreeWrapper {
public:
	virtual ~ISubtreeWrapper() { }
	virtual const IRTree::IExp* ToExp() const = 0; 
	virtual const IRTree::IStm* ToStm() const = 0; 
	virtual const IRTree::IStm* ToConditional(
                         const Temp::CLabel* t, const Temp::CLabel* f) const = 0;
};

class CExpConverter : public ISubtreeWrapper {
public:
	CExpConverter(IRTree::IExp* _expr) : expr(_expr) {}

	const IRTree::IExp* ToExp() const {
		return expr;
	}

	const IRTree::IStm* ToStm() const { 
		new IRTree::EXP(expr);
	}

	const IRTree::IStm* ToConditional(Temp::CLabel* t, Temp::CLabel* f) const {
		return new IRTree::CJUMP(IRTree::GT, expr, new IRTree::CONST(1), t, f);
	}

private:
	IRTree::IExp* expr;
};

class CStmConverter : public ISubtreeWrapper {
public:
	CStmConverter(const IRTree::IStm* _stm): stm(_stm) {}

	const IRTree::IExp* ToExp() const {

	}

	const IRTree::IStm* ToStm() const {
		return stm;
	}

	const IRTree::IStm* ToConditional(
                         const Temp::CLabel* t, const Temp::CLabel* f) const {

	}
private:
	const IRTree::IStm* stm;
};

class CTranslatorVisitor: public CVisitor {
public:
  virtual void visit(const CProgramRuleNode* node) {}
  virtual void visit(const CMainClassDeclarationRuleNode* node) {}
  virtual void visit(const CDeclarationsListNode* node) {}
  virtual void visit(const CClassDeclarationRuleNode* node) {}
  virtual void visit(const CExtendDeclarationRuleNode* node) {}
  virtual void visit(const CVarDeclarationsListNode* node) {}
  virtual void visit(const CMethodDeclarationsListNode* node) {}
  virtual void visit(const CVarDeclarationRuleNode* node) {}
  virtual void visit(const CMethodDeclarationRuleNode* node) {}
  virtual void visit(const CVarsDecListNode* node) {}
  virtual void visit(const CVarsDecFirstNode* node) {}
  virtual void visit(const CStatsFirstNode* node) {}
  virtual void visit(const CStatsListNode* node) {}
  virtual void visit(const CMethodBodyVarsNode* node) {}
  virtual void visit(const CMethodBodyStatsNode* node) {}
  virtual void visit(const CMethodBodyAllNode* node) {}
  virtual void visit(const CParamArgListNode* node) {}
  virtual void visit(const CParamsOneNode* node) {}
  virtual void visit(const CParamsTwoNode* node) {}
  virtual void visit(const CParamRuleNode* node) {}
  virtual void visit(const CTypeRuleNode* node) {}
  virtual void visit(const CNumerousStatementsNode* node) {}
  virtual void visit(const CBracedStatementNode* node) {}
  virtual void visit(const CIfStatementNode* node) {}
  virtual void visit(const CWhileStatementNode* node) {}
  virtual void visit(const CPrintStatementNode* node) {}
  virtual void visit(const CAssignStatementNode* node) {}
  virtual void visit(const CInvokeExpressionStatementNode* node) {}
  virtual void visit(const CInvokeExpressionNode* node) {}
  virtual void visit(const CLengthExpressionNode* node) {}
  virtual void visit(const CArithmeticExpressionNode* node) {}
  virtual void visit(const CUnaryExpressionNode* node) {}
  virtual void visit(const CCompareExpressionNode* node) {}
  virtual void visit(const CNotExpressionNode* node) {}
  virtual void visit(const CNewArrayExpressionNode* node) {}
  virtual void visit(const CNewObjectExpressionNode* node) {}
  virtual void visit(const CIntExpressionNode* node) {}
  virtual void visit(const CBooleanExpressionNode* node) {}
  virtual void visit(const CIdentExpressionNode* node) {}
  virtual void visit(const CThisExpressionNode* node) {}
  virtual void visit(const CParenExpressionNode* node) {}
  virtual void visit(const CInvokeMethodExpressionNode* node) {}
  virtual void visit(const CFewArgsExpressionNode* node) {}
  virtual void visit(const CListExpressionNode* node) {}
  virtual void visit(const CLastListExpressionNode* node) {}
};

}