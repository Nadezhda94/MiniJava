#include "BasicBlocks.h"

namespace Canon {
	StmtListList::StmtListList( shared_ptr <StmtList> _head, StmtListList* _tail ) : head( _head ), tail( _tail ) { }

	BasicBlocks::BasicBlocks( shared_ptr <StmtList> stms ) {
		done = new CLabel();
		mkBlocks( stms );
	}

	void BasicBlocks::addStm( IStm* s ) {
		lastStm->tail = make_shared<StmtList>( s, nullptr );
		lastStm = lastStm->tail;
	}

	void BasicBlocks::doStms( shared_ptr <StmtList> l ) {
		if ( l == nullptr ) {
			doStms( make_shared<StmtList>( new JUMP( done ), nullptr ));
		} else {
			JUMP* jump = dynamic_cast<JUMP*>(l->head);
			CJUMP* cjump = dynamic_cast<CJUMP*>(l->head);
			if (( jump != 0 ) || ( cjump != 0 )) {
				addStm( l->head );
				mkBlocks( l->tail );
			} else {
				LABEL* label = dynamic_cast<LABEL*>(l->head);
				if ( label != 0 ) {
					doStms( make_shared<StmtList>( new JUMP( label->label ), l ));
				} else {
					addStm( l->head );
					doStms( l->tail );
				}
			}
		}
	}

	void BasicBlocks::mkBlocks( shared_ptr <StmtList> l ) {
		if ( l == nullptr ) {
			return;
		} else {
			LABEL* label = dynamic_cast<LABEL*>(l->head);
			if ( label != 0 ) {
				//std::cerr << "LABEL" << std::endl;
				lastStm = make_shared<StmtList>( StmtList( l->head, nullptr ));
				if ( lastBlock == nullptr ) {
					blocks = new StmtListList( lastStm, nullptr );
					lastBlock = blocks;
				} else {
					lastBlock->tail = new StmtListList( lastStm, nullptr );
					lastBlock = lastBlock->tail;
				}
				doStms( l->tail );
			} else {
				mkBlocks( make_shared<StmtList>( new LABEL( new CLabel()), l ));
			}

		}
	}
}