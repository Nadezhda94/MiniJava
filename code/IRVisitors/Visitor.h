#ifndef IRVISITOR_H_INCLUDED
#define IRVISITOR_H_INCLUDED

#include "../Structs/IRTree.h"

namespace IRTree {
  class MOVE;
  class EXP;
  class JUMP;
  class CJUMP;
  class SEQ;
  class LABEL;
  class CONST;
  class NAME;
  class TEMP;
  class BINOP;
  class MEM;
  class CALL;
  class ESEQ;

};


class CIRVisitor {
public:
  virtual void visit(const IRTree::MOVE* node) const = 0;
  virtual void visit(const IRTree::EXP* node) const = 0;
  virtual void visit(const IRTree::JUMP* node) const = 0;
  virtual void visit(const IRTree::CJUMP* node) const = 0;
  virtual void visit(const IRTree::SEQ* node) const = 0;
  virtual void visit(const IRTree::LABEL* node) const  = 0;
  virtual void visit(const IRTree::CONST* node)const = 0;
  virtual void visit(const IRTree::NAME* node) const= 0;
  virtual void visit(const IRTree::TEMP* node) const = 0;
  virtual void visit(const IRTree::BINOP* node) const = 0;
  virtual void visit(const IRTree::MEM* node)const = 0;
  virtual void visit(const IRTree::CALL* node) const = 0;
  virtual void visit(const IRTree::ESEQ* node)const = 0;

};

#endif
