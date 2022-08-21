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


// 插入指定用户，返回插入是否成功
bool OnlineUserList::insertUser(const User& user) {
	bool u=true;
	pthread_mutex_lock(&userMutex);
	u=(table.count(user.getId())==0);
	if(u) {
		userList.push_front(user);
		table.insert(make_pair(user.getId(), userList.begin()));
	}
	pthread_mutex_unlock(&userMutex);
	return u;
}

// 删除指定用户，返回删除是否成功
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

// 向所有用户广播一个整数
void OnlineUserList::broadcastInt32(int s) {
	pthread_mutex_lock(&userMutex);
	std::list<User>::iterator it = userList.begin();
	while(it!=userList.end()) {
		// it->writeInt32(s);
		++it;
	}
	pthread_mutex_unlock(&userMutex);
}

// 向指定用户发送一个整数，返回是否成功
bool OnlineUserList::sendInt32(int s, int to) {
	bool u;
	pthread_mutex_lock(&userMutex);
	u=(table.count(to)>0);
	// if(u)u=table[to]->writeInt32(s);
	pthread_mutex_unlock(&userMutex);
	return u;
}

void OnlineUserList::broadcast(const std::string& s) {
	pthread_mutex_lock(&userMutex);
	std::list<User>::iterator it = userList.begin();
	while(it!=userList.end()) {
		// it->writeString(s);
		++it;
	}
	pthread_mutex_unlock(&userMutex);
}

// 向所有用户广播一个字符串
bool OnlineUserList::send(const std::string& s, int to) {
	bool u;
	pthread_mutex_lock(&userMutex);
	u=(table.count(to)>0);
	// if(u)u=table[to]->writeString(s);
	pthread_mutex_unlock(&userMutex);
	return u;
}

void OnlineUserList::broadcastBuffer(const Buffer& buffer) {
	pthread_mutex_lock(&userMutex);
	std::list<User>::iterator it = userList.begin();
	while(it!=userList.end()) {
		it->writeBuffer(buffer);
		++it;
	}
	pthread_mutex_unlock(&userMutex);
}

bool OnlineUserList::sendBuffer(const Buffer& buffer, int to, int from) {
	bool u;
	pthread_mutex_lock(&userMutex);
	u=(table.count(to)>0);
	if(u) {
		u=table[to]->writeBuffer(buffer);
		if(from>=0&&from!=to)u&=table[from]->writeBuffer(buffer);
	}
	pthread_mutex_unlock(&userMutex);
	return u;
}


// 向指定用户发送当前所有在线人员信息，相当前在线人员广播指定用户信息（着重关注是否死锁）
bool OnlineUserList::sendAllIdentityIn(User& s) {
	pthread_mutex_lock(&userMutex);
	std::list<User>::iterator it = userList.begin();
	while(it!=userList.end()) {
		it->insertMessage(Message(AddIdentity, s.getId(), it->getId(), s.getName()));
		if(it->getId()!=s.getId())s.insertMessage(Message(AddIdentity, it->getId(), s.getId(), it->getName()));
		++it;
	}
	pthread_mutex_unlock(&userMutex);
	return true;
}

// 相当前在线人员广播指定用户信息（着重关注是否死锁）
bool OnlineUserList::sendAllIdentityOut(const User& s) {
	pthread_mutex_lock(&userMutex);
	std::list<User>::iterator it = userList.begin();
	while(it!=userList.end()) {
		it->insertMessage(Message(SubIdentity, s.getId(), it->getId(), s.getName()));
		++it;
	}
	pthread_mutex_unlock(&userMutex);
	return true;
}