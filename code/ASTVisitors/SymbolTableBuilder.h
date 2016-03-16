#ifndef SYMBOLTABLEBUILDER_H_INCLUDED
#define SYMBOLTABLEBUILDER_H_INCLUDED
#include "../common.h"
#include "../Structs/Ast.h"
#include "../Structs/SymbolsTable.h"
#include "../ASTVisitors/Visitor.h"

using namespace SymbolsTable;

class CSymbolTableBuilder : public CVisitor{
public:
  CTable table;
  CStorage* symbolsStorage;
  bool inMethod;
  const CSymbol* lastTypeValue;

  template <typename T>
  void checkItemAlreadyExists(const std::vector<T>& items, const CSymbol* itemName) {
    typename std::vector<T>::const_iterator it = items.cbegin();
    typename std::vector<T>::const_iterator itEnd = items.cend();
    for (; it != itEnd; ++it) {
      if (it->name == itemName) {
        std::cerr << "redefinition: " << itemName << std::endl;
      }
    }
  }

  CSymbolTableBuilder(CStorage* _symbolsStorage): symbolsStorage(_symbolsStorage), inMethod(0) {
      symbolsStorage->get("this");
  }

  void visit(const CProgramRuleNode* node){
    node->mainClass->accept(this);
    if (node->decl != 0) {
       node->decl->accept(this);
    }
  }

  void visit(const CMainClassDeclarationRuleNode* node){
    inMethod = 0;
    checkItemAlreadyExists<CClassInfo>(table.classInfo, node->className);

    table.classInfo.push_back(CClassInfo(node->className));
    CMethodInfo mainMthd(symbolsStorage->get("main"), symbolsStorage->get("void"));
    mainMthd.params.push_back(CVarInfo(node->argNames, symbolsStorage->get("String[]")));

    table.classInfo.back().methods.push_back(mainMthd);
    if (node->stmt != 0) {
      node->stmt->accept(this);
    }
  }

  void visit(const CDeclarationsListNode* node){
    if (node->decl != 0) {
      node->decl->accept(this);
    }
    node->cl->accept(this);
  }

  void visit(const CClassDeclarationRuleNode* node){
    inMethod = 0;
    checkItemAlreadyExists<CClassInfo>(table.classInfo, node->ident);
    table.classInfo.push_back(CClassInfo(node->ident));
    if (node->extDecl != 0) {
      node->extDecl->accept(this);
    }
    if (node->method != 0) {
      node->method->accept(this);
    }
    if (node->vars != 0) {
      node->vars->accept(this);
    }
  }

  void visit(const CExtendDeclarationRuleNode* node){
    table.classInfo.back().parent = node->ident;
  }

  void visit(const CVarDeclarationsListNode* node){
    if (node->list != 0) {
      node->list->accept(this);
    }
    node->item->accept(this);
  }

  void visit(const CMethodDeclarationsListNode* node){
    if (node->list != 0) {
      node->list->accept(this);
    }
    node->item->accept(this);
  }

  void visit(const CVarDeclarationRuleNode* node){
    node->type->accept(this);
    if (inMethod) {
      checkItemAlreadyExists<CVarInfo>(table.classInfo.back().methods.back().vars, node->ident);
      table.classInfo.back().methods.back().vars.push_back(CVarInfo(node->ident, lastTypeValue));
    } else {
      checkItemAlreadyExists<CVarInfo>(table.classInfo.back().vars, node->ident);
      table.classInfo.back().vars.push_back(CVarInfo(node->ident, lastTypeValue));
    }
  }

  void visit(const CMethodDeclarationRuleNode* node){
    node->type->accept(this);
    checkItemAlreadyExists<CMethodInfo>(table.classInfo.back().methods, node->ident);
    table.classInfo.back().methods.push_back(CMethodInfo(node->ident, lastTypeValue));
    if (node->param_arg != 0) {
      node->param_arg->accept(this);
    }
    if (node->method_body != 0) {
      inMethod = 1;
      node->method_body->accept(this);
      inMethod = 0;
    }
  }

  void visit(const CVarsDecListNode* node){

    node->list->accept(this);
    node->next->accept(this);

  }

  void visit(const CVarsDecFirstNode* node){
    node->first->accept(this);
  }

  void visit(const CStatsFirstNode* node){

  }

  void visit(const CStatsListNode* node){}

  void visit(const CMethodBodyVarsNode* node){
    node->vars->accept(this);
  }

  void visit(const CMethodBodyStatsNode* node){

  }

  void visit(const CMethodBodyAllNode* node){
    node->vars->accept(this);
  }

  void visit(const CParamArgListNode* node){
    if (node->params != 0) {
      node->params->accept(this);
    }
  }

  void visit(const CParamsOneNode* node){
    node->param->accept(this);
  }

  void visit(const CParamsTwoNode* node){
    node->first->accept(this);
    node->second->accept(this);
  }

  void visit(const CParamRuleNode* node){
    node->type->accept(this);
    checkItemAlreadyExists<CVarInfo>(table.classInfo.back().methods.back().params, node->ident);
    table.classInfo.back().methods.back().params.push_back(CVarInfo(node->ident, lastTypeValue));
  }

  void visit(const CTypeRuleNode* node){
    lastTypeValue = node->type;
  }

  void visit(const CNumerousStatementsNode* node){}
  void visit(const CBracedStatementNode* node){}
  void visit(const CIfStatementNode* node){}
  void visit(const CWhileStatementNode* node){}
  void visit(const CPrintStatementNode* node){}
  void visit(const CAssignStatementNode* node){}
  void visit(const CInvokeExpressionStatementNode* node){}

  void visit(const CInvokeExpressionNode* node){}
  void visit(const CLengthExpressionNode* node){}
  void visit(const CArithmeticExpressionNode* node){}
  void visit(const CUnaryExpressionNode* node){}
  void visit(const CCompareExpressionNode* node){}
  void visit(const CNotExpressionNode* node){}
  void visit(const CNewArrayExpressionNode* node){}
  void visit(const CNewObjectExpressionNode* node){}
  void visit(const CIntExpressionNode* node){}
  void visit(const CBooleanExpressionNode* node){}
  void visit(const CIdentExpressionNode* node){}
  void visit(const CThisExpressionNode* node){}
  void visit(const CParenExpressionNode* node){}
  void visit(const CInvokeMethodExpressionNode* node){}
  void visit(const CFewArgsExpressionNode* node){}

  void visit(const CListExpressionNode* node){}
  void visit(const CLastListExpressionNode* node){}
};
#endif
