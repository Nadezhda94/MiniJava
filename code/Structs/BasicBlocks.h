#ifndef BASICBLOCKS_H_INCLUDED
#define BASICBLOCKS_H_INCLUDED

#include "../IRVisitors/Printer.h"
using namespace IRTree;
using namespace Temp;

namespace Canon {


class StmtListList {
public:
    shared_ptr<StmtList> head;
    StmtListList* tail;
    StmtListList(shared_ptr<StmtList> _head, StmtListList* _tail) : head(_head), tail(_tail) {}
};

class BasicBlocks{
public:
    StmtListList* blocks;
    const CLabel* done;
    StmtListList* lastBlock;
    shared_ptr<StmtList> lastStm;
    CIRPrinter ir_print_vis;

    void addStm(const IStm* s) {
        lastStm->tail = make_shared<const StmtList>(s, nullptr);
        lastStm = const_pointer_cast<StmtList>(lastStm->tail);
    }

    void doStms(shared_ptr<const StmtList> l) {
        if ( l == nullptr ) {
            doStms(make_shared<const StmtList>(new JUMP(done), nullptr));
        } else {
            const JUMP* jump = dynamic_cast<const JUMP*>(l->head);
            const CJUMP* cjump = dynamic_cast<const CJUMP*>(l->head);
            if ( (jump != 0) || (cjump != 0) ){
                addStm(l->head);
                mkBlocks(l->tail);
            } else {
                const LABEL* label = dynamic_cast<const LABEL*>(l->head);
                if (label != 0) {
                    doStms(make_shared<StmtList>(new JUMP(label->label), l));
                } else {
                    addStm(l->head);
                    doStms(l->tail);
                }
            }
        }
    }

    void mkBlocks(shared_ptr<const StmtList> l) {
        if (l == nullptr) {
            return;
        } else {
            const LABEL* label = dynamic_cast<const LABEL*>(l->head);
            if (label != 0) {
                //std::cerr << "LABEL" << std::endl;
                lastStm = make_shared<StmtList>(StmtList(l->head, nullptr));
                if (lastBlock == nullptr) {
                    blocks = new StmtListList(lastStm, nullptr);
                    lastBlock = blocks;
                } else {
                    lastBlock->tail = new StmtListList(lastStm, nullptr);
                    lastBlock = lastBlock->tail;
                }
                doStms(l->tail);
            } else {
                mkBlocks(make_shared<StmtList>(new LABEL(new CLabel()), l));
            }

        }
    }

    BasicBlocks(shared_ptr<const StmtList> stms) {
        done = new CLabel();
        mkBlocks(stms);
    }
};

}

#endif