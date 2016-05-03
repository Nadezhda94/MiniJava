#ifndef TYPECHECKER_H_INCLUDED
#define TYPECHECKER_H_INCLUDED
#include "../common.h"
#include "../ASTVisitors/Visitor.h"
#include "../Structs/SymbolsTable.h"
using namespace SymbolsTable;
using namespace Symbol;

class CTypeChecker : public CVisitor {
public:
	CTypeChecker(CStorage* _symbols, CTable& _table);
	void Visit(const CProgramRuleNode* node);
	void Visit(const CMainClassDeclarationRuleNode* node);
	void Visit(const CDeclarationsListNode* node);
	void Visit(const CClassDeclarationRuleNode* node);
	void Visit(const CExtendDeclarationRuleNode* node);
	void Visit(const CVarDeclarationsListNode* node);
	void Visit(const CMethodDeclarationsListNode* node);
	void Visit(const CVarDeclarationRuleNode* node);
	void Visit(const CMethodDeclarationRuleNode* node);
	void Visit(const CVarsDecListNode* node);
	void Visit(const CVarsDecFirstNode* node);
	void Visit(const CStatsFirstNode* node);
	void Visit(const CStatsListNode* node);
	void Visit(const CMethodBodyVarsNode* node);
	void Visit(const CMethodBodyStatsNode* node);
	void Visit(const CMethodBodyAllNode* node);
	void Visit(const CParamArgListNode* node);
	void Visit(const CParamsOneNode* node);
	void Visit(const CParamsTwoNode* node);
	void Visit(const CParamRuleNode* node);
	void Visit(const CTypeRuleNode* node);
	void Visit(const CNumerousStatementsNode* node);
	void Visit(const CBracedStatementNode* node);
	void Visit(const CIfStatementNode* node);
	void Visit(const CWhileStatementNode* node);
	void Visit(const CPrintStatementNode* node);
	void Visit(const CAssignStatementNode* node);
	void Visit(const CInvokeExpressionStatementNode* node);
	void Visit(const CInvokeExpressionNode* node);
	void Visit(const CLengthExpressionNode* node);
	void Visit(const CArithmeticExpressionNode* node);
	void Visit(const CUnaryExpressionNode* node);
	void Visit(const CCompareExpressionNode* node);
	void Visit(const CNotExpressionNode* node);
	void Visit(const CNewArrayExpressionNode* node);
	void Visit(const CNewObjectExpressionNode* node);
	void Visit(const CIntExpressionNode* node) ;
	void Visit(const CBooleanExpressionNode* node);
	void Visit(const CIdentExpressionNode* node);
	void Visit(const CThisExpressionNode* node);
	void Visit(const CParenExpressionNode* node);
	void Visit(const CInvokeMethodExpressionNode* node);
	void Visit(const CFewArgsExpressionNode* node);
	void Visit(const CListExpressionNode* node);
	void Visit(const CLastListExpressionNode* node);
private:
	CTable table;
	const CSymbol* lastTypeValue;
	CStorage* symbolsStorage;
	bool inMethod;
	int classPos = 0;
	int methodPos = -1;
	int argCount = 0;
	int argNum = 0;

	bool checkClassExistence(const CSymbol* name);
	bool assignType(const CSymbol* name, const CSymbol*& type);
	const CSymbol* checkAssignment(const CSymbol* name);
};
#endif
