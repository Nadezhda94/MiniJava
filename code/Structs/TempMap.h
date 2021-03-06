#ifndef TEMPMAP_H_INCLUDED
#define TEMPMAP_H_INCLUDED

#include "../Structs/Temp.h"


namespace Temp {


class CTempMap {
public:
	virtual const std::string& tempMap(shared_ptr<const CTemp> t) = 0;
};



class CCombineMap: public CTempMap {
public:

	CCombineMap(CTempMap* _tmapFirst, CTempMap* _tmapSecond);

	CTempMap* tmapFirst;
	CTempMap* tmapSecond;

	const std::string& tempMap(shared_ptr<const CTemp> t);

};

class CDefaultMap: public CTempMap {
public:
	const std::string& tempMap(shared_ptr<const CTemp> t);
};

}

#endif