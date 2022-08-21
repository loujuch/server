#include "globe.h"
#include "MessageQueue.h"
#include "OnlineUserList.h"
#include "ControlCode.h"
#include "LogUtils.h"

#include <cstdio>
#include <cstring>
#include <netdb.h>
#include <unistd.h>

bool Globe::waitLog(const Link& link, int& id, std::string& name) {
	bool log=false;
	int code, len;
	while(!log) {
		len=link.readInt32(code);
		if(!len)break;
		switch(code) {
			case LogUp :
				log=LogUtils::LogUp(link, id, name);
				break;
			case LogIn :
				log=LogUtils::LogIn(link, id, name);
				break;
			default:
				break;
		}
	}
	return log;
}

void* Globe::slove(void* other) {
	int tmp, id;
	std::string name;
	memcpy(&tmp, &other, sizeof(int));
	Link link(tmp);
	printf("Globe::slove new Link create: %d\n", tmp);
	while(!waitLog(link, id, name));
	printf("Globe::slove log success: id: %d, name: %s\n", id, name.c_str());
	MessageQueue messageQueue;
	User user(id, name, link, messageQueue);
	pthread_t t;
	printf("Globe::slove will write thread create\n");
	pthread_create(&t, NULL, userWrite, &user);
	pthread_detach(t);
	printf("Globe::slove finish write thread create\n");
	OnlineUserList::insertUser(user);
	printf("Globe::slove user into list\n");
	OnlineUserList::sendAllIdentityIn(user);
	printf("Globe::slove userlist send\n");
	int sim=0;
	bool logout=false, has=false;
	MessageType type;
	int target, len;
	while(!logout) {
		if(!user.readInt32(sim))break;
		std::string s("");
		switch(sim) {
			case LogOut:
				printf("Globe::slove will LogOut id: %d, name: %s\n", user.getId(), user.getName().c_str());
				logout=true;
				has=false;
				printf("Globe::slove finish LogOut id: %d, name: %s\n", user.getId(), user.getName().c_str());
				break;
			case AloneText:
				printf("Globe::slove will read AloneText id: %d, name: %s\n",
					user.getId(), user.getName().c_str());
				type=AloneText;
				tmp=user.readInt32(target);
				tmp=user.readInt32(len);
				tmp=user.readString(s, len);
				has=true;
				printf("Globe::slove finish read AloneText id: %d, name: %s, target: %d, content: %s\n", 
					user.getId(), user.getName().c_str(), target, s.c_str());
				break;
			case GroupText:
				printf("Globe::slove will read GroupText id: %d, name: %s\n",
					user.getId(), user.getName().c_str());
				type=GroupText;
				tmp=user.readInt32(len);
				tmp=user.readString(s, len);
				has=true;
				printf("Globe::slove finish read GroupText id: %d, name: %s, content: %s\n", 
					user.getId(), user.getName().c_str(), s.c_str());
				break;
			default:
				has=false;
				break;
		}
		if(!tmp)break;
		if(has)user.insertMessage(Message(type, user.getId(), target, s));
	}
	user.logOut();
	OnlineUserList::deleteUser(user);
	OnlineUserList::sendAllIdentityOut(user);
	printf("Globe::slove finish\n");
	return NULL;
}

void* Globe::userWrite(void* pUser) {
	User* user=(User*)pUser;
	printf("Globe::userWrite write thread create id:%d, user: %s\n", 
		user->getId(), user->getName().c_str());
	bool logout=false;
	while(!logout) {
		printf("Globe::userWrite will read message id:%d, user: %s\n",
			user->getId(), user->getName().c_str());
		Message s(user->takeMessage());
		printf("Globe::userWrite will read message id:%d, user: %s\n",
			user->getId(), user->getName().c_str());
		Buffer buffer;
		buffer.addInt32(s.getType());
		buffer.addInt32(s.getSource());
		buffer.addString(s.getContent());
		switch (s.getType()) {
		case LogOut:
			printf("Globe::userWrite will LogOut id:%d, user: %s\n",
				user->getId(), user->getName().c_str());
			logout=true;
			printf("Globe::userWrite finish LogOut id:%d, user: %s\n",
				user->getId(), user->getName().c_str());
			break;
		case AloneText :
			printf("Globe::userWrite will write AloneText id:%d, user: %s, source: %d, content: %s\n",
				user->getId(), user->getName().c_str(), s.getSource(), s.getContent().c_str());
			OnlineUserList::sendBuffer(buffer, s.getTarget(), s.getSource());
			printf("Globe::userWrite finish write AloneText id:%d, user: %s, source: %d, content: %s\n",
				user->getId(), user->getName().c_str(), s.getSource(), s.getContent().c_str());
			break;
		case AddIdentity:
			printf("Globe::userWrite will write AddIdentity id:%d, user: %s, oid: %d, oname: %s\n",
				user->getId(), user->getName().c_str(), s.getSource(), s.getContent().c_str());
			OnlineUserList::sendBuffer(buffer, s.getTarget());
			printf("Globe::userWrite finish write AddIdentity id:%d, user: %s, oid: %d, oname: %s\n",
				user->getId(), user->getName().c_str(), s.getSource(), s.getContent().c_str());
			break;
		case SubIdentity:
			printf("Globe::userWrite will write SubIdentity id:%d, user: %s, oid: %d, oname: %s\n",
				user->getId(), user->getName().c_str(), s.getSource(), s.getContent().c_str());
			OnlineUserList::sendBuffer(buffer, s.getTarget());
			printf("Globe::userWrite finish write SubIdentity id:%d, user: %s, oid: %d, oname: %s\n",
				user->getId(), user->getName().c_str(), s.getSource(), s.getContent().c_str());
			break;
		case GroupText:
			printf("Globe::userWrite will write GroupText id:%d, user: %s, source: %d, content: %s\n",
				user->getId(), user->getName().c_str(), s.getSource(), s.getContent().c_str());
			OnlineUserList::broadcastBuffer(buffer);
			printf("Globe::userWrite finish write GroupText id:%d, user: %s, source: %d, content: %s\n",
				user->getId(), user->getName().c_str(), s.getSource(), s.getContent().c_str());
			break;
		default:
			break;
		}
		printf("Globe::userWrite finishid:%d, user: %s\n", user->getId(), user->getName().c_str());
	}
	return NULL;
}