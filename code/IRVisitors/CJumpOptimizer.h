#ifndef CJUMPOPTIMIZER_H_INCLUDED
#define CJUMPOPTIMIZER_H_INCLUDED
#include "../Structs/BasicBlocks.h"
#include "../common.h"
#include "../IRVisitors/Printer.h"
#include "../IRVisitors/Canonizer.h"
#include "../Structs/TraceShedule.h"

namespace Canon {

void printCanonizedTree(const IStm* res) {
	CIRPrinter ir_print_vis(false);
	cout << "linearization" << endl;
	shared_ptr<const StmtList> listLin = linearize(res); 
    shared_ptr<const StmtList> list = listLin;
    while (list != nullptr) {
            list->head->accept(&ir_print_vis);
            list = list->tail;
    }
    cout << "end linearization" << endl;
    BasicBlocks* blocks = new BasicBlocks(listLin);
    StmtListList* stmLL = blocks->blocks;
    TraceShedule* traceSh = new TraceShedule(blocks);
	shared_ptr<const StmtList> stms = traceSh->stms;
	cout << "=============cjump start============" << endl;
    while (stms != nullptr) {
    		//cout << "===========one start===========" << endl;
            stms->head->accept(&ir_print_vis);
            stms = stms->tail;
            //cout << "===========one end===========" << endl;
    }
    cout << "=============cjump end============" << endl;

   
}

void optimize(Symbol::CStorage& symbolsStorage, std::vector<const INode*>& traslator_trees) {

    CIRPrinter ir_print_vis(false);

    Canonizer canonizer;
    for (int i = 0; i < traslator_trees.size(); ++i ) {
        cout << "=================================" << endl;
        cout << "tree" << endl;
        traslator_trees[i]->accept(&ir_print_vis);
           
        traslator_trees[i]->accept(&canonizer);
        cout << "=================================" << endl;
        cout << "modified tree" << endl;
       	const IExp* arg =  dynamic_cast<const IExp*>(canonizer.current_node);
        if (arg != 0) {
            const IExp* res = doExp(arg);
            res->accept(&ir_print_vis);
	        const ESEQ* eseq = dynamic_cast<const ESEQ*>(res);
		    
		    const SEQ* seq = dynamic_cast<const SEQ*>(eseq->stm);
		    if (seq != 0) {	
            	printCanonizedTree(eseq->stm);
            }
	        
        } else {
           	const IStm* res = doStm(dynamic_cast<const IStm*>(canonizer.current_node));
            doStm(res)->accept(&ir_print_vis); 
            printCanonizedTree(res);
            
        }

    }
}

}

#endif