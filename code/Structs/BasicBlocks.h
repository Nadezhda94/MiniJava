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

    void addStm(IStm* s) {
        lastStm->tail = make_shared<StmtList>(s, nullptr);
        lastStm = lastStm->tail;
    }

    void doStms(shared_ptr<StmtList> l) {
        if ( l == nullptr ) {
            doStms(make_shared<StmtList>(new JUMP(done), nullptr));
        } else {
            JUMP* jump = dynamic_cast<JUMP*>(l->head);
            CJUMP* cjump = dynamic_cast<CJUMP*>(l->head);
            if ( (jump != 0) || (cjump != 0) ){
                addStm(l->head);
                mkBlocks(l->tail);
            } else {
                LABEL* label = dynamic_cast<LABEL*>(l->head);
                if (label != 0) {
                    doStms(make_shared<StmtList>(new JUMP(label->label), l));
                } else {
                    addStm(l->head);
                    doStms(l->tail);
                }
            }
        }
    }

    void mkBlocks(shared_ptr<StmtList> l) {
        if (l == nullptr) {
            return;
        } else {
            LABEL* label = dynamic_cast<LABEL*>(l->head);
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

    BasicBlocks(shared_ptr<StmtList> stms) {
        done = new CLabel();
        mkBlocks(stms);
    }
};

}

#endif
