#ifndef COMPILERS_FLOWGRAPH_H
#define COMPILERS_FLOWGRAPH_H
#include "../common.h"
#include "../Structs/Graph.h"
#include "../Structs/Temp.h"
#include "../Structs/Assembler.h"

namespace FlowGraph {
	using namespace Assembler;
	using namespace Temp;
	class CFlowGraph : public CGraph<int, CInstr*> {
	public:
		CFlowGraph(const CInstrList* instructions);
		CInstr* GetInstr(int node);
		set<const CTemp*> GetDef( int node );
		set<const CTemp*> GetUse( int node );
		bool isMove( int node );
	};
}

#endif //COMPILERS_FLOWGRAPH_H
