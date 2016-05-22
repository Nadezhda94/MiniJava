#ifndef COMPILERS_COLORING_H
#define COMPILERS_COLORING_H
#include "../Structs/FlowGraph.h"
namespace Coloring {
	using namespace FlowGraph;
	void BuildFlowGraph( ostream &out, vector<shared_ptr<CInstrList>> &blockInstructions,
						 vector<shared_ptr<CFlowGraph>> graphs );
}

#endif //COMPILERS_COLORING_H
