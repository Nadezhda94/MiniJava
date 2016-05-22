#include "CodeGenerator.h"
namespace CodeGenerator {
	void GenerateCode( ostream &out, const vector<shared_ptr<StmtList>> &blocks,
					   vector<shared_ptr<CInstrList>> &blockInstructions ) {
		CCodegen generator;
		CDefaultMap* defMap = new CDefaultMap();
		for ( int i = 0; i < blocks.size(); ++i ) {
			CInstrList* instructs = 0;
			out << "===========================" << endl;
			shared_ptr<StmtList> curBlock = blocks[i];
			while ( curBlock != 0 ) {
				instructs = 0;
				if ( curBlock->head != 0 ) {
					instructs = generator.Codegen( curBlock->head );
				}
				while ( instructs != 0 ) {
					if ( instructs->head != 0 ) {
						out << instructs->head->format( defMap );
					}
					instructs = instructs->tail;
				}
				curBlock = curBlock->tail;
			}
			blockInstructions.push_back( shared_ptr<CInstrList>(instructs) );
		}
	}
}