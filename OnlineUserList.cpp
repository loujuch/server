#include "OnlineUserList.h"
#include "MessageQueue.h"

pthread_mutex_t OnlineUserList::userMutex;
std::list<User> OnlineUserList::userList;
std::map<int, std::list<User>::iterator> OnlineUserList::table;

void OnlineUserList::init() {
	pthread_mutex_init(&userMutex, NULL);
	while(!userList.empty())userList.pop_back();
}

void OnlineUserList::destory() {
	pthread_mutex_destroy(&userMutex);
}

bool OnlineUserList::insertUser(const User& user) {
	pthread_mutex_lock(&userMutex);
	userList.push_front(user);
	table.insert(make_pair(user.getId(), userList.begin()));
	pthread_mutex_unlock(&userMutex);
	return true;
}

bool OnlineUserList::deleteUser(const User& user) {
	bool u=true;
	pthread_mutex_lock(&userMutex);
	u=(table.count(user.getId())>0);
	if(u) {
		userList.erase(table[user.getId()]);
		table.erase(user.getId());
	}
	pthread_mutex_unlock(&userMutex);
	return u;
}

void OnlineUserList::broadcastInt32(int s) {
	pthread_mutex_lock(&userMutex);
	std::list<User>::iterator it = userList.begin();
	while(it!=userList.end()) {
		it->writeInt32(s);
		++it;
	}
	pthread_mutex_unlock(&userMutex);
}

bool OnlineUserList::sendInt32(int s, int to) {
	bool u;
	pthread_mutex_lock(&userMutex);
	u=(table.count(to)>0);
	if(u)u=table[to]->writeInt32(s);
	pthread_mutex_unlock(&userMutex);
	return u;
}

void OnlineUserList::broadcast(const std::string& s) {
	pthread_mutex_lock(&userMutex);
	std::list<User>::iterator it = userList.begin();
	while(it!=userList.end()) {
		it->writeString(s);
		++it;
	}
	pthread_mutex_unlock(&userMutex);
}

bool OnlineUserList::send(const std::string& s, int to) {
	bool u;
	pthread_mutex_lock(&userMutex);
	u=(table.count(to)>0);
	if(u)u=table[to]->writeString(s);
	pthread_mutex_unlock(&userMutex);
	return u;
}


//maybe problem
bool OnlineUserList::sendAllIdentityIn(const User& s) {
	pthread_mutex_lock(&userMutex);
	std::list<User>::iterator it = userList.begin();
	while(it!=userList.end()) {
		MessageQueue::insertMessage(Message(AddIdentity, s.getId(), it->getId(), s.getName()));
		MessageQueue::insertMessage(Message(AddIdentity, it->getId(), s.getId(), it->getName()));
		++it;
	}
	pthread_mutex_unlock(&userMutex);
	return true;
}

//maybe problem
bool OnlineUserList::sendAllIdentityOut(const User& s) {
	pthread_mutex_lock(&userMutex);
	std::list<User>::iterator it = userList.begin();
	while(it!=userList.end()) {
		MessageQueue::insertMessage(Message(SubIdentity, s.getId(), it->getId(), s.getName()));
		++it;
	}
	pthread_mutex_unlock(&userMutex);
	return true;
}