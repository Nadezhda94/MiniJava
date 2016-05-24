#ifndef TEMP_H_INCLUDED
#define TEMP_H_INCLUDED
#include "../common.h"

namespace Temp {
// Метка - точка перехода в коде
class CLabel {
public:
	// Создать метку с уникальным именем
	CLabel();
	CLabel(const string& uniqueName);
	const string& Name() const;
	
private:
	// Счётчик для создания уникальных идентификаторов
	static int nextUniqueId;
	string name;
};

// Временная переменная
class CTemp {
public:
	// Новая переменная с уникальным именем
	CTemp();
	CTemp(const std::string& uniqueName);
	~CTemp();
	const string& Name() const;

private:
	// Счётчик для создания уникальных имён
	static int nextUniqueId;
	string name;
};

class CTempList {
public:
	CTempList(shared_ptr<const CTemp> _head, CTempList* _tail);
	CTempList* Reverse();

	shared_ptr<const CTemp> head;
	CTempList* tail;

	set<const CTemp*> GetSet() {
		CTempList* cur = this;
		set<const CTemp*> res;
		while ( cur != 0 ) {
			assert( cur->head != 0 );
			res.insert(cur->head.get());
			cur = cur->tail;
		}
		return res;
	}
};

class CLabelList {
public:
	CLabelList(const CLabel* _head, CLabelList* _tail);

	const CLabel* head;
	CLabelList* tail;
};
}

#endif
