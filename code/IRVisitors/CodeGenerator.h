#ifndef COMPILERS_CODEGENERATOR_H
#define COMPILERS_CODEGENERATOR_H

#include "../common.h"
#include "../Structs/Codegen.h"

namespace CodeGenerator {
	using namespace IRTree;
	using namespace Assembler;
	void GenerateCode( ostream &out, const vector<shared_ptr<StmtList>> &blocks,
					   vector<shared_ptr<CInstrList>> &blockInstructions );
}

#endif