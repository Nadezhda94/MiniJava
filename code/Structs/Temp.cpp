#include "Temp.h"

namespace Temp {
	//--------------------------------------------------------------------------------------------------------------
	// CLabel
	//--------------------------------------------------------------------------------------------------------------
	CLabel::CLabel(): name("label" + std::to_string(nextUniqueId)) {
		++nextUniqueId;
	}

	CLabel::CLabel(const std::string& uniqueName) : name(uniqueName) {}

	const std::string& CLabel::Name() const {
		return name;
	}

	//--------------------------------------------------------------------------------------------------------------
	// CTemp
	//--------------------------------------------------------------------------------------------------------------
	CTemp::CTemp(): name("temp" + std::to_string(nextUniqueId)) {
		++nextUniqueId;
	}

	CTemp::CTemp(const std::string& uniqueName) : name(uniqueName) {}

	CTemp::~CTemp() {}

	const std::string& CTemp::Name() const {
		return name;
	}

	int CLabel::nextUniqueId = 0;
	int CTemp::nextUniqueId = 0;


	CTempList::CTempList(shared_ptr<const CTemp> _head, CTempList* _tail) : head(_head), tail(_tail) {}

	CLabelList::CLabelList(const CLabel* _head, CLabelList* _tail) : head(_head), tail(_tail) {}
}