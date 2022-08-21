#ifndef _USER_SQL_HPP_
#define _USER_SQL_HPP_

#include "Sqlite.h"

class UserSQL {
	static int select(void *data, int argc, char **ColValue, char **ColName);
public:
	static bool create();
	static bool insert(const std::string& account, const std::string& passwd, const std::string& name);
	static bool search(int& id, const std::string& account, const std::string& passwd, std::string& name);
};

#endif