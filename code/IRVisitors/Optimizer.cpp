#include "Optimizer.h"
#include "../Structs/BasicBlocks.h"
#include "../Structs/TraceShedule.h"
#include "../IRVisitors/Canonizer.h"
#include "../IRVisitors/Printer.h"

namespace Canon {
	void Canonize(vector<INode*>& trees, vector<IStm*>& canonized_trees){
		canonized_trees.clear();
		CCanonizer canonizer;
		for ( int i = 0; i < trees.size(); ++i ) {
			trees[i]->accept(&canonizer);

			INode* root = canonizer.current_node;
			IExp* exp = dynamic_cast<IExp*>(root);
			IStm* stm = dynamic_cast<IStm*>(root);
			IStm* result = 0;

			if (exp != 0) {
				IExp* res = doExp(exp);
				ESEQ* eseq = dynamic_cast<ESEQ*>(res);
				result = dynamic_cast<SEQ*>(eseq->stm);
			}
			if (stm !=0 ) {
				result = doStm(stm);
			}
			if (result != 0) {
				canonized_trees.push_back(result);
			}
		}
	}

	void Linearize(vector<IStm*>& trees, vector<shared_ptr<StmtList>>& result) {
		result.clear();
		for ( int i = 0; i < trees.size(); ++i ) {
			shared_ptr<StmtList> listLin = linearize( trees[i] );
			result.push_back(listLin);
		}
	}

	void Trace(vector<shared_ptr<StmtList>>& linearized, vector<shared_ptr<StmtList>>& result) {
		result.clear();
		for ( int i = 0; i < linearized.size(); ++i ) {
			shared_ptr<StmtList> listLin = linearized[i];
			BasicBlocks* blocks = new BasicBlocks( listLin );
			TraceShedule* traceSh = new TraceShedule( blocks );
			shared_ptr<StmtList> traced = traceSh->stms;
			result.push_back(traced);
		}
	}

	void Print(ostream& out, ostream& gv, vector<INode*>& trees) {
		CIRPrinter ir_print_vis( out, gv );
		for ( int i = 0; i < trees.size(); ++i ) {
			out << "=================================" << endl;
			out << i << " tree" << endl;
			out << "=================================" << endl;
			trees[i]->accept( &ir_print_vis );
		}
	}

	void Print(ostream& out, ostream& gv, vector<IStm*>& trees) {
		CIRPrinter ir_print_vis( out, gv );
		for ( int i = 0; i < trees.size(); ++i ) {
			out << "=================================" << endl;
			out << i << " tree" << endl;
			out << "=================================" << endl;
			trees[i]->accept( &ir_print_vis );
		}
	}

	void Print(ostream& out, ostream& gv, shared_ptr<StmtList> stmts) {

	}

	void Print(ostream& out, ostream& gv, vector<shared_ptr<StmtList>>& stmts) {
		CIRPrinter ir_print_vis( out, gv );
		for ( int i = 0; i < stmts.size(); ++i ) {
			out << "=================================" << endl;
			out << i << " tree" << endl;
			out << "=================================" << endl;
			vector<IStm*> trees;
			stmts[i]->toVector(trees);
			for ( int j = 0; j < trees.size(); ++j ) {
				trees[j]->accept( &ir_print_vis );
			}
		}
	}
}

