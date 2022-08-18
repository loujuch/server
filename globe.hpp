#ifndef _GLOBE_HPP_
#define _GLOBE_HPP_

#include <string>
#include <list>
#include <mutex>
#include <queue>
#include <unordered_map>

#include "link.hpp"

#include <atomic>

class Globe {
	struct User {
		const Link& link;

		User(const Link& link);
	};

	static std::atomic<int> messageNum;
	static std::mutex userMutex;
	static std::queue<std::string>message;
	static std::list<User>user;
	static std::unordered_map<int, std::list<User>::iterator> table;
public:
	Globe() = delete;
	Globe(const Globe&) = delete;
	Globe& operator=(const Globe&) = delete;

	static void init();

	static void slove(int other, struct sockaddr_in otherSocket);

	static void serve();

	static void broadcast(const std::string& s);

	static bool insertUser(const Link& link);
	static bool deleteUser(const Link& link);

	static void insertMessage(const std::string& s);
	static bool takeMessage(std::string& s);
};

#endif