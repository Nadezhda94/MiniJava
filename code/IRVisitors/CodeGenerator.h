#ifndef COMPILERS_CODEGENERATOR_H
#define COMPILERS_CODEGENERATOR_H

#include "../common.h"
#include "../Structs/Codegen.h"
using namespace IRTree;
using namespace Assembler;

void GenerateCode(ostream& out, const vector<shared_ptr<StmtList>>& blocks) {
	CCodegen generator;
	CInstrList* instructs = 0;
	CDefaultMap* defMap= new CDefaultMap();
	for (int i = 0; i < blocks.size(); ++i) {
		out << "===========================" << endl;
		shared_ptr<StmtList> curBlock = blocks[i];
		while (curBlock != 0) {
			instructs = 0;
			if (curBlock -> head != 0) {
				instructs = generator.Codegen(curBlock->head);
			}
			while (instructs != 0) {
				if (instructs->head  != 0) {
					//cout << "ohoho" << endl;
					out << instructs->head->format(defMap);
				} 
				instructs = instructs->tail;
			}
			
			curBlock = curBlock->tail;
		}
	}
}


#endif