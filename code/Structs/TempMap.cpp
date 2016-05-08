#include "TempMap.h"

namespace Temp {
	//--------------------------------------------------------------------------------------------------------------
	// CCombineMap
	//--------------------------------------------------------------------------------------------------------------
	
	CCombineMap::CCombineMap(CTempMap* _tmapFirst, CTempMap* _tmapSecond)
	: tmapFirst(_tmapFirst), tmapSecond(_tmapSecond) {}


	const std::string& CCombineMap::tempMap(shared_ptr<CTemp> t) {
		const std::string& s = tmapFirst->tempMap(t);
		if ( !s.empty() ) {
			return s;
		} else {
			return tmapSecond->tempMap(t);
		}
	}
	
	//--------------------------------------------------------------------------------------------------------------
	// CDefaultMap
	//--------------------------------------------------------------------------------------------------------------

	const std::string& CDefaultMap::tempMap(shared_ptr<CTemp> t) {
		return t->Name();
	}
}