#ifndef COMPILERS_INTERFERENCEGRAPH_H
#define COMPILERS_INTERFERENCEGRAPH_H
#include "../Structs/Graph.h"
#include "../Structs/Temp.h"
#include "../Structs/FlowGraph.h"
namespace RegAlloc {
	using namespace Temp;
	using namespace FlowGraph;
	class CInterferenceGraph : public CGraph<int, const CTemp*> {
	public:
		CInterferenceGraph(){}
		void Build(CFlowGraph& flowGraph);
	private:
		map<int, int> colors;
		map<int, set<AMOVE*>> movesAssociated;

		set<int> precolored; // machine registers, preassigned a color
		set<int> initial; // temporary registers, not precolored and not yet processed.

		set<int> spilledNodes; // nodes marked for spilling during this round; initially empty.
		map<int, int> coloredNodes; // nodes successfully colored
		// registers that have been coalesced; when u ← v is coalesced, v is
		// added to this set and u put back on some work list (or vice versa).
		map<int, int> coalescedNodes;

		set<int> simpilfyWorklist; // list of low-degree non-move-related nodes
		set<int> freezeWorklist; // low-degree move-related nodes
		set<int> spillWorklist; // high-degree nodes

		list<int> selectStack; // stack containing temporaries removed from the graph

		set<int> moveRelated;

		set<AMOVE*> coalescedMoves; // moves that have been coalesced.
		set<AMOVE*> constrainedMoves; // moves whose source and target interfere.
		set<AMOVE*> frozenMoves; // moves that will no longer be considered for coalescing.
		set<AMOVE*> worklistMoves; // moves enabled for possible coalescing.
		set<AMOVE*> activeMoves; // moves not yet ready for coalescing.
	};
}
#endif //COMPILERS_INTERFERENCEGRAPH_H
