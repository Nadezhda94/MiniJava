#include "../Structs/FlowGraph.h"

namespace FlowGraph {
	void CFlowGraph::Build(const CInstrList* instructions){
		map<const CLabel*, int> labelToNode;
		
		// Первый проход - сбор ALABEL и их соответствий с CLabel*, добавление вершин
		const CInstrList* cur = instructions;
		while ( cur != 0 ){
			CInstr* node = cur->head;
			assert( node != 0 );
			int index = addNode(node);
			ALABEL* label = dynamic_cast<ALABEL*>(node);
			if ( label != 0 ){
				labelToNode[label->label] = index;
			}
			cur = cur->tail;
		}

		// Второй проход - добавление рёбер
		cur = instructions;
		while ( cur != 0 ){
			CInstr* node = cur->head;
			assert( node != 0 );
			CTargets* targets = node->jumps();
			if ( targets != 0 ) {
				CLabelList* labels = targets->labels;
				while ( labels != 0 ) {
					const CLabel* label = labels->head;
					assert( label != 0 );
					addEdge( getNode(node).index, labelToNode[label] );
					labels = labels->tail;
				}
			} else {
				if ( (cur->tail != 0) && (cur->tail->head != 0) ) {
					CInstr* second = cur->tail->head;
					addEdge( getNode(node).index, getNode(second).index );
				}
			}
			cur = cur->tail;
		}
	}

	CInstr* CFlowGraph::GetInstr(int node){
		return getNode(node).value;
	}

	set<const CTemp*> CFlowGraph::GetDef( int node ){
		return getNode(node).value->def()->GetSet();
	}

	set<const CTemp*> CFlowGraph::GetUse( int node ){
		return getNode(node).value->use()->GetSet();
	}

	bool CFlowGraph::isMove( int node ){
		return (dynamic_cast<AMOVE*>(getNode(node).value) != 0);
	}
}