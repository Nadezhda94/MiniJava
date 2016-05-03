#ifndef TRACESHEDULE_H_INCLUDED
#define TRACESHEDULE_H_INCLUDED

#include "../common.h"
#include "BasicBlocks.h"

namespace Canon {
class TraceShedule {
public:
	shared_ptr<StmtList> stms;
	BasicBlocks* blocks;
	std::unordered_map<const Temp::CLabel*, shared_ptr<StmtList>> table;

	TraceShedule(BasicBlocks* b);
	shared_ptr<StmtList> getLast(shared_ptr<StmtList> block);
	void trace(shared_ptr<StmtList> l);
	shared_ptr<StmtList> getNext();
};

}

#endif
