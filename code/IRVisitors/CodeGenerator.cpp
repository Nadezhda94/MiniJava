#include "CodeGenerator.h"
namespace CodeGenerator {
	void GenerateCode( ostream &out, const vector<shared_ptr<StmtList>> &blocks,
					   vector<shared_ptr<CInstrList>> &blockInstructions ) {
		CCodegen generator;
		CDefaultMap* defMap = new CDefaultMap();
		for ( int i = 0; i < blocks.size(); ++i ) {
			out << "===========================" << endl;
			shared_ptr<StmtList> curBlock = blocks[i];
			CInstrList* instructs = 0;
			CInstrList* blockInstructs = 0;
			while ( curBlock != 0 ) {
				assert( curBlock->head != 0 );
				if ( instructs == 0 ) {
					instructs = generator.Codegen( curBlock->head );
					blockInstructs = instructs;
				} else {
					while ( instructs->tail != 0 ) {
						instructs = instructs->tail;
					}
					instructs->tail = generator.Codegen( curBlock->head );
				}
				curBlock = curBlock->tail;
			}

			instructs = blockInstructs;
			while ( instructs != 0 ) {
				if ( instructs->head != 0 ) {
					out << instructs->head->format( defMap );
				}
				instructs = instructs->tail;
			}

			blockInstructions.push_back( shared_ptr<CInstrList>(blockInstructs) );
		}
	}
}