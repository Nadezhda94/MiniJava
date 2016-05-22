#ifndef COMPILERS_INTERFERENCEGRAPH_H
#define COMPILERS_INTERFERENCEGRAPH_H
#include "../Structs/Graph.h"
#include "../Structs/Temp.h"
#include "../Structs/FlowGraph.h"
namespace RegAlloc {
	using namespace Temp;
	using namespace FlowGraph;
	class CInterferenceGraph : public CGraph<int, const CTemp*> {
		CInterferenceGraph(CFlowGraph& flowGraph);
	};
}
#endif //COMPILERS_INTERFERENCEGRAPH_H
