#ifndef _GLOBE_HPP_
#define _GLOBE_HPP_

#include <list>
#include <map>

#include "link.h"

class Globe {
	Globe();
	Globe(const Globe&);
	Globe& operator=(const Globe&);
public:
	static void* slove(void* other);

	static bool waitLog(const Link& link, int& id, std::string& name);

	static void* userWrite(void* pUser);
};

#endif