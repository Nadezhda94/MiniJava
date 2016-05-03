#ifndef TRANSLATOR_H_INCLUDED
#define TRANSLATOR_H_INCLUDED

#include "../common.h"
#include "../ASTVisitors/Visitor.h"
#include "../Structs/IRTree.h"
#include "../Structs/Temp.h"
#include "../Structs/Frame.h"
#include "../Structs/SymbolsTable.h"

namespace Translate {
	using namespace IRTree;
	using namespace Temp;
	using namespace Frame;
	using namespace SymbolsTable;

	class ISubtreeWrapper {
	public:
		virtual ~ISubtreeWrapper() { }
		virtual IExp* ToExp() const = 0;
		virtual IStm* ToStm() const = 0;
		virtual IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const = 0;
	};

	class CExpConverter : public ISubtreeWrapper {
	public:
		CExpConverter(IExp* _expr);
		IExp* ToExp() const;
		IStm* ToStm() const;
		IStm* ToConditional(const Temp::CLabel* t,const Temp::CLabel* f) const;
	private:
		IExp* expr;
	};

	class CStmConverter : public ISubtreeWrapper {
	public:
		CStmConverter(IStm* _stm);
		IExp* ToExp() const;
		IStm* ToStm() const;
		IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const;
	private:
		IStm* stm;
	};

	class CConditionalWrapper : public ISubtreeWrapper {
	public:
		IExp* ToExp() const;
		virtual IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const = 0;
		IStm* ToStm() const;
	};

	class CRelativeCmpWrapper : public CConditionalWrapper {
	public:
		CRelativeCmpWrapper(CJUMP_OP _op, IExp* _first, IExp* _second);
		IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const;
	private:
		IExp* first;
		IExp* second;
		CJUMP_OP op;
	};

	class CFromAndConverter : public CConditionalWrapper {
	public:
		CFromAndConverter(IExp* _leftArg, IExp* _rightArg);
		IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const;
	private:
		IExp* leftArg;
		IExp* rightArg;
	};

	class CFromOrConverter : public CConditionalWrapper {
	public:
		CFromOrConverter(IExp* _leftArg, IExp* _rightArg);
		IStm* ToConditional(const Temp::CLabel* t, const Temp::CLabel* f) const;
	private:
		IExp* leftArg;
		IExp* rightArg;
	};

	class CTranslator : public CVisitor {
	public:
		vector<INode*> trees;

		CTranslator( CStorage* _symbols, CTable &_table );
		void Visit( const CProgramRuleNode* node );
		void Visit( const CMainClassDeclarationRuleNode* node );
		void Visit( const CDeclarationsListNode* node );
		void Visit( const CClassDeclarationRuleNode* node );
		void Visit( const CExtendDeclarationRuleNode* node ) {}
		void Visit( const CVarDeclarationsListNode* node );
		void Visit( const CMethodDeclarationsListNode* node );
		void Visit( const CVarDeclarationRuleNode* node ){}
		void Visit( const CMethodDeclarationRuleNode* node );
		void Visit( const CVarsDecListNode* node );
		void Visit( const CVarsDecFirstNode* node );
		void Visit( const CStatsFirstNode* node );
		void Visit( const CStatsListNode* node );
		void Visit( const CMethodBodyVarsNode* node ) {}
		void Visit( const CMethodBodyStatsNode* node );
		void Visit( const CMethodBodyAllNode* node );
		void Visit( const CParamArgListNode* node ){}
		void Visit( const CParamsOneNode* node ){}
		void Visit( const CParamsTwoNode* node ){}
		void Visit( const CParamRuleNode* node ){}
		void Visit( const CTypeRuleNode* node ){}
		void Visit( const CNumerousStatementsNode* node );
		void Visit( const CBracedStatementNode* node );
		void Visit( const CIfStatementNode* node );
		void Visit( const CWhileStatementNode* node );
		void Visit( const CPrintStatementNode* node );
		void Visit( const CAssignStatementNode* node );
		void Visit( const CInvokeExpressionStatementNode* node );
		void Visit( const CInvokeExpressionNode* node );
		void Visit( const CLengthExpressionNode* node );
		void Visit( const CArithmeticExpressionNode* node );
		void Visit( const CUnaryExpressionNode* node );
		void Visit( const CCompareExpressionNode* node );
		void Visit( const CNotExpressionNode* node );
		void Visit( const CNewArrayExpressionNode* node );
		void Visit( const CNewObjectExpressionNode* node );
		void Visit( const CIntExpressionNode* node );
		void Visit( const CBooleanExpressionNode* node );
		void Visit( const CIdentExpressionNode* node );
		void Visit( const CThisExpressionNode* node );
		void Visit( const CParenExpressionNode* node );
		void Visit( const CInvokeMethodExpressionNode* node );
		void Visit( const CFewArgsExpressionNode* node );
		void Visit( const CListExpressionNode* node );
		void Visit( const CLastListExpressionNode* node );
	private:
		CStorage* symbolsStorage;
		CTable table;
		map<string, shared_ptr<CLabel>> functionalLabels;
		CClassInfo* currentClass;
		CMethodInfo* currentMethod;
		shared_ptr<CFrame> currentFrame;
		shared_ptr<ISubtreeWrapper> currentNode;
		shared_ptr<ExpList> arguments;

		const CSymbol* getMallocFuncName();
		const CSymbol* getPrintFuncName();
	};
}
#endif
