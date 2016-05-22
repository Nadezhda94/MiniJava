#include "RegAlloc.h"

template <>
ostream& operator<< <Assembler::CInstr*> (ostream& s, CGraphNode<Assembler::CInstr*> const & rhs){
	s<<rhs.index<<" ";
	(*rhs.value).Print(s);
	return s;
}

namespace RegAlloc {
	void BuildFlowGraph( ostream &out, vector<shared_ptr<CInstrList>> &blockInstructions,
						 vector<shared_ptr<CFlowGraph>> graphs )
	{
		for (int i = 0; i < blockInstructions.size(); i++) {
			graphs.push_back(make_shared<CFlowGraph>());
			graphs.back()->Build(blockInstructions[i].get());
			out<<(*(graphs.back().get()));
		}
	}

	void BuildInterferenceGraph( ostream &out, vector<shared_ptr<CFlowGraph>> flowGraphs,
						 vector<shared_ptr<CInterferenceGraph>> interferenceGraphs )
	{
		for (int i = 0; i < flowGraphs.size(); i++) {
			interferenceGraphs.push_back(make_shared<CInterferenceGraph>());
			interferenceGraphs.back()->Build(*flowGraphs[i]);
			out<<(*(interferenceGraphs.back().get()));
		}
	}
}
