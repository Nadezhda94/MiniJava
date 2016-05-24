#ifndef COMPILERS_REGALLOC_H
#define COMPILERS_REGALLOC_H
#include "../Structs/FlowGraph.h"
#include "../Structs/InterferenceGraph.h"
namespace RegAlloc {
	using namespace Assembler;
	using namespace FlowGraph;
	void BuildFlowGraph( ostream &out, vector<shared_ptr<CInstrList>>& blockInstructions,
						 vector<shared_ptr<CFlowGraph>>& graphs );
	void BuildInterferenceGraph( ostream &out, vector<shared_ptr<CFlowGraph>>& flowGraphs,
								 vector<shared_ptr<CInterferenceGraph>>& interferenceGraphs );
}

#endif //COMPILERS_REGALLOC_H
