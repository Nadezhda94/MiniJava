#include "../Structs/InterferenceGraph.h"

namespace RegAlloc {
	void CInterferenceGraph::Build( CFlowGraph& flowGraph ){
		map< int, set<const CTemp*> > in;
		map< int, set<const CTemp*> > inOld;
		map< int, set<const CTemp*> > out;
		map< int, set<const CTemp*> > outOld;

		bool cycled;
		list<CGraphNode<CInstr*>*> ordered;
		flowGraph.DFS(cycled, ordered);
		set<const CTemp*> inDiff;
		set<const CTemp*> outDiff;
		do {
			inDiff.clear();
			outDiff.clear();
			for (auto it = ordered.begin(); it != ordered.end(); it++){
				CGraphNode<CInstr*>* node = (*it);
				int index = node->index;
				inOld[index] = in[index];
				outOld[index] = out[index];
				in[index].clear();
				out[index].clear();

				set<const CTemp*> use = flowGraph.GetUse(index);
				set<const CTemp*> def = flowGraph.GetDef(index);
				set<const CTemp*> inter;
				set_intersection(out[index].begin(), out[index].end(),
								 def.begin(), def.end(),
								 inserter(inter, inter.begin()));
				set_union(use.begin(), use.end(),
						  inter.begin(), inter.end(),
						  inserter(in[index], in[index].begin()));

				set<int> succ = flowGraph.getNodesIndexFromNode(index);
				for (auto i = succ.begin(); i != succ.end(); i++){
					int succIndex = (*i);
					out[index].insert(in[succIndex].begin(), in[succIndex].end());
				}

				set_intersection(in[index].begin(), in[index].end(),
								 inOld[index].begin(), inOld[index].end(),
								 inserter(inDiff, inDiff.begin()));
				set_intersection(out[index].begin(), out[index].end(),
								 outOld[index].begin(), outOld[index].end(),
								 inserter(outDiff, outDiff.begin()));
			}
		} while( !( inDiff.empty() && outDiff.empty() ) );

		set<const CTemp*> allTemps;
		for (auto it = ordered.begin(); it != ordered.end(); it++) {
			CGraphNode<CInstr*>* node = (*it);
			set<const CTemp*> defs = flowGraph.GetDef(node->index);
			allTemps.insert( defs.begin(), defs.end() );
		}
		for (auto it = allTemps.begin(); it != allTemps.end(); it++) {
			addNode( (*it) );
		}

		for (auto i = ordered.begin(); i != ordered.end(); i++) {
			CGraphNode<CInstr*>* node = (*i);
			int index = node->index;
			AMOVE* move = dynamic_cast<AMOVE*>(node->value);
			if ( move != 0 ){
				set<const CTemp*> use = flowGraph.GetUse(index);
				set<const CTemp*> all = flowGraph.GetDef(index);
				all.insert(use.begin(), use.end());
				for (auto j = all.begin(); j != all.end(); j++) {
					movesAssociated[getNode(*j).index].insert(move);
					movesAssociated[getNode(*j).index].insert(move);
				}
				worklistMoves.insert(move);
			} else {
				set<const CTemp*> def = flowGraph.GetDef(index);
				for (auto j = def.begin(); j != def.end(); j++) {
					for (auto k = out[index].begin(); k != out[index].end(); k++) {
						addBiEdge( getNode((*k)).index, getNode((*j)).index );
					}
				}
			}

		}
	}
	void Simplify(){

	}
}