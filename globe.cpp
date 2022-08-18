#include "globe.hpp"

#include <netdb.h>
#include <unistd.h>

#include <thread>
#include <algorithm>

std::atomic<int> Globe::messageNum(0);
std::mutex Globe::userMutex;
std::queue<std::string> Globe::message;
std::list<Globe::User> Globe::user;
std::unordered_map<int, std::list<Globe::User>::iterator> Globe::table;

Globe::User::User(const Link& link):link(link) {
}

void Globe::init() {
	messageNum=0;
	userMutex.unlock();
	while(!user.empty())user.pop_back();
	while(!message.empty())message.pop();
}

void Globe::slove(int other, struct sockaddr_in otherSocket) {
	Link link(other);
	std::string s("");
	insertUser(link);
	while(1) {
		link.read(s);
		insertMessage(s);
	}
	deleteUser(link);
}

void Globe::serve() {
	std::string s;
	bool flag;
	while(1) {
		flag=takeMessage(s);
		if(flag)broadcast(s);
	}
}

void Globe::broadcast(const std::string& s) {
	bool flag;
	userMutex.lock();
	for(auto&link:user) {
		flag=link.link.write(s);
	}
	userMutex.unlock();
}

bool Globe::insertUser(const Link& link) {
	userMutex.lock();
	user.push_back(link);
	auto i=user.end();
	table.emplace(link.getSocketId(), --i);
	userMutex.unlock();
	return true;
}

bool Globe::deleteUser(const Link& link) {
	bool u=true;
	userMutex.lock();
	u=(table.count(link.getSocketId())>0);
	if(u) {
		user.erase(table[link.getSocketId()]);
		table.erase(link.getSocketId());
	}
	userMutex.unlock();
	return u;
}

void Globe::insertMessage(const std::string& s) {
	message.push(s);
	++messageNum;
}

bool Globe::takeMessage(std::string& s) {
	while(messageNum<=0)std::this_thread::yield();
	s=message.front();
	message.pop();
	--messageNum;
	return true;
}