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
			l = l->tail;
		}
		return l;
	}

	void trace(shared_ptr<StmtList> l) {
		for (;;) {
			LABEL* label = static_cast<LABEL*>(l->head);
			table.erase(label->label);
			shared_ptr<StmtList> last = getLast(l);
			IStm* s = last->tail->head;
			JUMP* jump = dynamic_cast<JUMP*>(s);
			if (jump != 0) {
				std::unordered_map<const Temp::CLabel*, shared_ptr<StmtList>>::iterator it = table.find(jump->target);
				if (it != table.end()) {
					last->tail = it->second;
					l = it->second;
				} else {
					last->tail->tail = getNext();
					return;
				}

			} else {
				CJUMP* cjump = dynamic_cast<CJUMP*>(s);
				if (cjump != 0) {
					std::unordered_map<const Temp::CLabel*, shared_ptr<StmtList>>::iterator itTrue = table.find(cjump->iftrue);
					std::unordered_map<const Temp::CLabel*, shared_ptr<StmtList>>::iterator itFalse = table.find(cjump->iffalse);
					if ( itFalse != table.end() ) {
						cout << last->tail->tail << endl;
						last->tail->tail = itFalse->second;
						cout << last->tail->tail << endl;
						l = itFalse->second;
						cout << "ohoho" << endl;
					} else if ( itTrue != table.end() ) {
						last->tail->head = new CJUMP(cjump->relop, cjump->left, cjump->right, cjump->iffalse, cjump->iftrue);
						last->tail->tail = itTrue->second;
						l = itTrue->second;
					} else {
						const CLabel* ff = new CLabel();
						last->tail->head = new CJUMP(cjump->relop, cjump->left, cjump->right, cjump->iffalse, cjump->iftrue);
						last->tail->tail = make_shared<StmtList>(new LABEL(ff),
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
			LABEL* label = static_cast<LABEL*>(s->head);
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
			if (static_cast<LABEL*>(l->head->head)->label == 0) {
				cerr << "BAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa" << endl;
				exit(1);
			}
			table[static_cast<LABEL*>(l->head->head)->label] = l->head;
			cout << "1" << endl;
		}
		stms = getNext();
		//table = nullptr;
	}

};

}

#endif
