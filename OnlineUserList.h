#ifndef _ONLINE_USER_LIST_HPP_
#define _ONLINE_USER_LIST_HPP_

#include "User.h"
#include "Message.h"

#include <map>
#include <list>

class OnlineUserList {
	static pthread_mutex_t userMutex;
	static std::list<User>userList;
	static std::map<int, std::list<User>::iterator> table;

	OnlineUserList();
	OnlineUserList(const OnlineUserList&);
	OnlineUserList& operator=(const OnlineUserList&);
public:
	static void init();
	static void destory();

	static bool insertUser(const User& user);
	static bool deleteUser(const User& user);

	static void broadcastInt32(int s);
	static bool sendInt32(int s, int to);

	static void broadcast(const std::string& s);
	static bool send(const std::string& s, int to);

	static bool sendAllIdentityIn(User& s);
	static bool sendAllIdentityOut(const User& s);
};

#endif