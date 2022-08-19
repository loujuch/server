#ifndef _GLOBE_HPP_
#define _GLOBE_HPP_

#include <string>
#include <list>
#include <queue>
#include <map>

#include "link.hpp"

class Globe {
	struct User {
		const Link& link;

		User(const Link& link);
	};

	static pthread_cond_t messageCond;
	static pthread_mutex_t messageMutex;
	static pthread_mutex_t userMutex;
	static std::queue<std::string>message;
	static std::list<User>user;
	static std::map<int, std::list<User>::iterator> table;

	Globe();
	Globe(const Globe&);
	Globe& operator=(const Globe&);
public:
	static void init();

	static void* slove(void* other);

	static void* serve(void* null);

	static void broadcast(const std::string& s);

	static bool insertUser(const Link& link);
	static bool deleteUser(const Link& link);

	static void insertMessage(const std::string& s);
	static bool takeMessage(std::string& s);

	static void destory();
};

#endif