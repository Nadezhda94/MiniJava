#ifndef TRACESHEDULE_H_INCLUDED
#define TRACESHEDULE_H_INCLUDED

#include "../common.h"
#include "BasicBlocks.h"

namespace Canon {
class TraceShedule {
public:
	shared_ptr<StmtList> stms;
	BasicBlocks* blocks;
	std::unordered_map<const Temp::CLabel*, shared_ptr<StmtList>> table;

	shared_ptr<StmtList> getLast(shared_ptr<StmtList> block) {
		shared_ptr<StmtList> l = block;
		while (l->tail->tail != nullptr) {
			l = const_pointer_cast<StmtList>(l->tail);
		}
		return l;
	}

	void trace(shared_ptr<StmtList> l) {
		for (;;) {
			const LABEL* label = static_cast<const LABEL*>(l->head);
			table.erase(label->label);
			shared_ptr<StmtList> last = getLast(l);
			const IStm* s = last->tail->head;
			const JUMP* jump = dynamic_cast<const JUMP*>(s);
			if (jump != 0) {
				std::unordered_map<const Temp::CLabel*, shared_ptr<StmtList>>::iterator it = table.find(jump->target);
				if (it != table.end()) {
					last->tail = it->second;
					l = it->second;
				} else {
					const_pointer_cast<StmtList>(last->tail)->tail = getNext();
					return;
				}

			} else {
				const CJUMP* cjump = dynamic_cast<const CJUMP*>(s);
				if (cjump != 0) {
					std::unordered_map<const Temp::CLabel*, shared_ptr<StmtList>>::iterator itTrue = table.find(cjump->iftrue);
					std::unordered_map<const Temp::CLabel*, shared_ptr<StmtList>>::iterator itFalse = table.find(cjump->iffalse);
					if ( itFalse != table.end() ) {
						cout << last->tail->tail << endl;
						const_pointer_cast<StmtList>(last->tail)->tail = const_pointer_cast<StmtList>(itFalse->second);
						cout << last->tail->tail << endl;
						l = itFalse->second;
						cout << "ohoho" << endl;
					} else if ( itTrue != table.end() ) {
						const_pointer_cast<StmtList>(last->tail)->head = new CJUMP(cjump->relop, cjump->left, cjump->right, cjump->iffalse, cjump->iftrue);
						const_pointer_cast<StmtList>(last->tail)->tail = const_pointer_cast<StmtList>(itTrue->second);
						l = itTrue->second;
					} else {
						const CLabel* ff = new CLabel();
						const_pointer_cast<StmtList>(last->tail)->head = new CJUMP(cjump->relop, cjump->left, cjump->right, cjump->iffalse, cjump->iftrue);
						const_pointer_cast<StmtList>(last->tail)->tail = make_shared<const StmtList>(new LABEL(ff), 
							make_shared<StmtList>(new JUMP(cjump->iffalse), getNext()));
						return;
					}
				} else {
					std::cerr << "BOOOOOOOOOOOM" << std::endl;
				}


			}
		} 
	}

	shared_ptr<StmtList> getNext() {
		if (blocks->blocks == nullptr) {
			return make_shared<StmtList>(new LABEL(blocks->done), nullptr);
		} else {
			shared_ptr<StmtList> s = blocks->blocks->head;
			const LABEL* label = static_cast<const LABEL*>(s->head);
			std::unordered_map<const Temp::CLabel*, shared_ptr<StmtList>>::iterator itTrue = table.find(label->label);
			if (itTrue != table.end()) {
				trace(s);
				return s;
			} else {
				blocks->blocks = blocks->blocks->tail;
				return getNext();
			}
		}
	}

	TraceShedule(BasicBlocks* b) {
		blocks = b;
		for (StmtListList* l = b->blocks; l != nullptr; l = l->tail) {
			if (static_cast<const LABEL*>(l->head->head)->label == 0) {
				cerr << "BAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa" << endl;
				exit(1);
			}
			table[static_cast<const LABEL*>(l->head->head)->label] = const_pointer_cast<StmtList>(l->head);
			cout << "1" << endl;
		}
		stms = getNext();
		//table = nullptr;
	}
 	
};

}

#endif

