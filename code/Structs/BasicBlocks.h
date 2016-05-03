#ifndef BASICBLOCKS_H_INCLUDED
#define BASICBLOCKS_H_INCLUDED

#include "../common.h"
#include "../Structs/IRTree.h"

using namespace IRTree;
using namespace Temp;

namespace Canon {
class StmtListList {
public:
    shared_ptr<StmtList> head;
    StmtListList* tail;
    StmtListList(shared_ptr<StmtList> _head, StmtListList* _tail);
};

class BasicBlocks{
public:
	StmtListList* blocks;
	const CLabel* done;
	StmtListList* lastBlock;
	shared_ptr<StmtList> lastStm;

    BasicBlocks(shared_ptr<StmtList> stms);
    void addStm(IStm* s);
    void doStms(shared_ptr<StmtList> l) ;
    void mkBlocks(shared_ptr<StmtList> l);
};

}

#endif
