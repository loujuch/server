#include "globe.hpp"

#include <cstring>
#include <netdb.h>
#include <unistd.h>

pthread_cond_t Globe::messageCond;
pthread_mutex_t Globe::messageMutex;
pthread_mutex_t Globe::userMutex;
std::queue<std::string> Globe::message;
std::list<Globe::User> Globe::user;
std::map<int, std::list<Globe::User>::iterator> Globe::table;

Globe::User::User(const Link& link):link(link) {
}

void Globe::init() {
	pthread_cond_init(&messageCond, NULL);
	pthread_mutex_init(&messageMutex, NULL);
	pthread_mutex_init(&userMutex, NULL);
	while(!user.empty())user.pop_back();
	while(!message.empty())message.pop();
}

void* Globe::slove(void* other) {
	int tmp;
	memcpy(&tmp, &other, sizeof(int));
	Link link(tmp);
	std::string s("");
	int len=0;
	insertUser(link);
	while(1) {
		if(!link.read(s))break;
		insertMessage(s);
	}
	deleteUser(link);
	return NULL;
}

void* Globe::serve(void* null) {
	std::string s;
	bool flag;
	while(1) {
		flag=takeMessage(s);
		if(flag)broadcast(s);
	}
	return NULL;
}

void Globe::broadcast(const std::string& s) {
	bool flag;
	pthread_mutex_lock(&userMutex);
	std::list<User>::iterator it = user.begin();
	while(it!=user.end()) {
		flag=it->link.write(s);
		++it;
	}
	pthread_mutex_unlock(&userMutex);
}

bool Globe::insertUser(const Link& link) {
	pthread_mutex_lock(&userMutex);
	user.push_front(link);
	table.insert(make_pair(link.getSocketId(), user.begin()));
	pthread_mutex_unlock(&userMutex);
	return true;
}

bool Globe::deleteUser(const Link& link) {
	bool u=true;
	pthread_mutex_lock(&userMutex);
	u=(table.count(link.getSocketId())>0);
	if(u) {
		user.erase(table[link.getSocketId()]);
		table.erase(link.getSocketId());
	}
	pthread_mutex_unlock(&userMutex);
	return u;
}

void Globe::insertMessage(const std::string& s) {
	if(s.empty())return;
	pthread_mutex_lock(&messageMutex);
	message.push(s);
	pthread_cond_signal(&messageCond);
	pthread_mutex_unlock(&messageMutex);
}

bool Globe::takeMessage(std::string& s) {
	pthread_mutex_lock(&messageMutex);
	while(message.empty())pthread_cond_wait(&messageCond, &messageMutex);
	s=message.front();
	message.pop();
	pthread_mutex_unlock(&messageMutex);
	return true;
}

void Globe::destory() {
	pthread_cond_destroy(&messageCond);
	pthread_mutex_destroy(&messageMutex);
	pthread_mutex_destroy(&userMutex);
}