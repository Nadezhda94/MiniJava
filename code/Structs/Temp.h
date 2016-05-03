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
	~CTemp();
	const string& Name() const;

private:
	// Счётчик для создания уникальных имён
	static int nextUniqueId;
	string name;
};

}

#endif
