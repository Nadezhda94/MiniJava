#ifndef PRINTIRVISITOR_H_INCLUDED
#define PRINTIRVISITOR_H_INCLUDED
#include "../common.h"
#include "../Structs/IRTree.h"
#include "../Visitors/IRVisitor.h"


class CIRPrint: public CIRVisitor {
  public:
  virtual void visit(const IRTree::MOVE* node) const {
    print_tabs(++counter);
    cout << "MOVE" << endl;
    node->dst->accept(this);
    node->src->accept(this);
    --counter;
  }

  virtual void visit(const IRTree::EXP* node) const {
    print_tabs(++counter);
    cout << "EXP" << endl;
    node->exp->accept(this);
    --counter;
  }

  virtual void visit(const IRTree::JUMP* node) const {
    print_tabs(counter++);
    cout << "JUMP" << endl;
    node->exp->accept(this);
    //TODO
  --counter;
  }

  virtual void visit(const IRTree::CJUMP* node) const {
    print_tabs(counter++);
    cout << "CJUMP" << endl;
    node->left->accept(this);
  node->right->accept(this);
  --counter;
  }

  virtual void visit(const IRTree::SEQ* node) const {
    print_tabs(counter++);
    cout << "SEQ" << endl;
    node->left->accept(this);
  node->right->accept(this);
  --counter;
  }

  virtual void visit(const IRTree::LABEL* node) const {
    print_tabs(counter++);
    cout << "LABEL" << endl;
    //TODO
  --counter;
  }

  virtual void visit(const IRTree::CONST* node) const {
    print_tabs(counter++);
    cout << "CONST " << node->value << endl;
  --counter;
  }

  virtual void visit(const IRTree::NAME* node) const {
    print_tabs(counter++);
    cout << "NAME" << endl;
    //TODO
  --counter;
  }

  virtual void visit(const IRTree::TEMP* node) const {
    print_tabs(counter++);
    cout << "TEMP" << endl;
    //node->temp->accept(this);
  --counter;
  }

  virtual void visit(const IRTree::BINOP* node) const {
    print_tabs(counter++);
    cout << "BINOP" << endl;
    node->left->accept(this);
  node->right->accept(this);
  --counter;
  }

  virtual void visit(const IRTree::MEM* node) const {
    print_tabs(counter++);
    cout << "MEM" << endl;
  node->exp->accept(this);
  --counter;
  }

  virtual void visit(const IRTree::CALL* node) const {
    print_tabs(counter++);
    cout << "CALL" << endl;
    node->func->accept(this);
  --counter;
  }

  virtual void visit(const IRTree::ESEQ* node) const {
    print_tabs(counter++);
    cout << "ESEQ" << endl;
    node->stm->accept(this);
  node->exp->accept(this);
  --counter;
  }
private:
  mutable int counter = 0;
    void print_tabs(int num) const {
      for (int i = 0; i < num; i++)
        cout << "  ";
    }
};

#endif