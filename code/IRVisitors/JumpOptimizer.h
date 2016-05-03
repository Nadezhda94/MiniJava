#ifndef COMPILERS_CJUMPOPTIMIZER_H
#define COMPILERS_CJUMPOPTIMIZER_H

#include "../common.h"
#include "../Structs/IRTree.h"
using namespace IRTree;

namespace Canon {
	void printCanonizedTree(ostream& out, IStm* res);
	void optimize(ostream& out, Symbol::CStorage& symbolsStorage, std::vector<INode*>& traslator_trees);
}


#endif //COMPILERS_CJUMPOPTIMIZER_H
