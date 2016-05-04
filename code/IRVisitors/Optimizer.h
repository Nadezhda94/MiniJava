#ifndef COMPILERS_CJUMPOPTIMIZER_H
#define COMPILERS_CJUMPOPTIMIZER_H

#include "../common.h"
#include "../Structs/IRTree.h"
using namespace IRTree;

namespace Canon {
	void Canonize(vector<INode*>& trees, vector<IStm*>& canonized_trees);
	void Linearize(vector<IStm*>& trees, vector<shared_ptr<StmtList>>& result);
	void Trace(vector<shared_ptr<StmtList>>& stmts, vector<shared_ptr<StmtList>>& result);
	void Print(ostream& out, vector<INode*>& trees);
	void Print(ostream& out, vector<IStm*>& trees);
	void Print(ostream& out, shared_ptr<StmtList>& stmts);
	void Print(ostream& out, vector<shared_ptr<StmtList>>& stmts);
}


#endif //COMPILERS_CJUMPOPTIMIZER_H
