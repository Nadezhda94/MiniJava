#include "../IRVisitors/Coloring.h"

namespace Coloring {
	void BuildFlowGraph( ostream &out, vector<shared_ptr<CInstrList>> &blockInstructions,
						 vector<shared_ptr<CFlowGraph>> graphs ){
		for (int i = 0; i < blockInstructions.size(); i++) {
			graphs.push_back(make_shared<CFlowGraph>());
			graphs.back()->Build(blockInstructions[i].get());
			out<<graphs[i];
		}
	}
}
