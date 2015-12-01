#ifndef TEMP_H_INCLUDED
#define TEMP_H_INCLUDED
#include <string>
#include "CSymbol.h"

namespace Temp {
// Метка - точка перехода в коде
class CLabel {
public:
	// Создать метку с уникальным именем
	CLabel(): name("label" + std::to_string(nextUniqueId)) {
		++nextUniqueId;
	}

	const std::string& Name() const { 
		return name; 
	}

private:
	// Счётчик для создания уникальных идентификаторов
	static int nextUniqueId;
	std::string name;
};

int CLabel::nextUniqueId = 0;

// Временная переменная
class CTemp {
public:
	// Новая переменная с уникальным именем
	CTemp(): name("temp" + std::to_string(nextUniqueId)) {
		++nextUniqueId;
	}
	// Новая переменная с заданным именем
	explicit CTemp(const Symbol::CSymbol* symbol );
	~CTemp() {}

	const std::string& Name() const { 
		return name; 
	}

private:
	// Счётчик для создания уникальных имён
	static int nextUniqueId;
	std::string name;
};

int CTemp::nextUniqueId = 0;

} 

#endif
