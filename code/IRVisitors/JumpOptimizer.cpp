#include "JumpOptimizer.h"
#include "../Structs/BasicBlocks.h"
#include "../Structs/TraceShedule.h"
#include "../IRVisitors/Canonizer.h"
#include "../IRVisitors/Printer.h"

namespace Canon {

void printCanonizedTree(ostream& out, IStm* res) {
	CIRPrinter ir_print_vis(out, false);
	out << "linearization" << endl;
	shared_ptr<StmtList> listLin = linearize(res);
    shared_ptr<StmtList> list = listLin;
    while (list != nullptr) {
            list->head->accept(&ir_print_vis);
            list = list->tail;
    }
    out << "end linearization" << endl;
    BasicBlocks* blocks = new BasicBlocks(listLin);
    StmtListList* stmLL = blocks->blocks;
    TraceShedule* traceSh = new TraceShedule(blocks);
	shared_ptr<StmtList> stms = traceSh->stms;
	out << "=============cjump start============" << endl;
    while (stms != nullptr) {
    		//out << "===========one start===========" << endl;
            stms->head->accept(&ir_print_vis);
            stms = stms->tail;
            //out << "===========one end===========" << endl;
    }
    out << "=============cjump end============" << endl;


}

void optimize(ostream& out, Symbol::CStorage& symbolsStorage, std::vector<INode*>& traslator_trees) {

    CIRPrinter ir_print_vis(out, false);

    Canonizer canonizer;
    for (int i = 0; i < traslator_trees.size(); ++i ) {
        out << "=================================" << endl;
        out << "tree" << endl;
        traslator_trees[i]->accept(&ir_print_vis);

        traslator_trees[i]->accept(&canonizer);
        out << "=================================" << endl;
        out << "modified tree" << endl;
       	IExp* arg =  dynamic_cast<IExp*>(canonizer.current_node);
        if (arg != 0) {
            IExp* res = doExp(arg);
            res->accept(&ir_print_vis);
	        ESEQ* eseq = dynamic_cast<ESEQ*>(res);

		    SEQ* seq = dynamic_cast<SEQ*>(eseq->stm);
		    if (seq != 0) {
            	printCanonizedTree(out, eseq->stm);
            }

        } else {
           	IStm* res = doStm(dynamic_cast<IStm*>(canonizer.current_node));
            doStm(res)->accept(&ir_print_vis);
            printCanonizedTree(out, res);
        }

    }
}

}

