#ifndef _LOG_UTILS_
#define _LOG_UTILS_

#include "link.h"

class LogUtils {
public:
	static bool LogIn(const Link& link, int& id, std::string& name);
	static bool LogUp(const Link& link, int& id, std::string& name);
};

#endif