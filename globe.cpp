#include "globe.h"
#include "MessageQueue.h"
#include "OnlineUserList.h"
#include "ControlCode.h"
#include "LogUtils.h"

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
	if(waitLog(link, id, name)) {
		return NULL;
	}
	MessageQueue messageQueue;
	User user(id, name, link, messageQueue);
	pthread_t t;
	pthread_create(&t, NULL, userWrite, &user);
	pthread_detach(t);
	OnlineUserList::insertUser(user);
	OnlineUserList::sendAllIdentityIn(user);
	int sim=0;
	bool logout=false, has=false;
	MessageType type;
	int target, len;
	std::string s("");
	while(!logout) {
		if(!user.readInt32(sim))break;
		switch(sim) {
			case LogOut:
				user.logOut();
				logout=true;
				has=false;
				break;
			case AloneText:
				type=AloneText;
				tmp=user.readInt32(target);
				tmp=user.readInt32(len);
				tmp=user.readString(s, len);
				has=true;
				break;
			case GroubText:
				type=GroubText;
				tmp=user.readInt32(len);
				tmp=user.readString(s, len);
				has=true;
				break;
			default:
				has=false;
				break;
		}
		if(!tmp)break;
		if(has)user.insertMessage(Message(type, user.getId(), target, s));
	}
	OnlineUserList::deleteUser(user);
	OnlineUserList::sendAllIdentityOut(user);
	return NULL;
}

void* Globe::userWrite(void* pUser) {
	User* user=(User*)pUser;
	bool logout=false;
	while(logout) {
		Message s(user->takeMessage());
		switch (s.getType()) {
		case LogOut:
			logout=true;
			break;
		case AloneText :
			OnlineUserList::sendInt32(AloneText, s.getTarget());
			OnlineUserList::sendInt32(s.getSource(), s.getTarget());
			OnlineUserList::send(s.getContent(), s.getTarget());
			break;
		case GroubText:
			OnlineUserList::broadcastInt32(GroubText);
			OnlineUserList::broadcastInt32(s.getSource());
			OnlineUserList::broadcast(s.getContent());
			break;
		case AddIdentity:
			OnlineUserList::sendInt32(AddIdentity, s.getTarget());
			OnlineUserList::sendInt32(s.getSource(), s.getTarget());
			OnlineUserList::send(s.getContent(), s.getTarget());
			break;
		case SubIdentity:
			OnlineUserList::sendInt32(SubIdentity, s.getTarget());
			OnlineUserList::sendInt32(s.getSource(), s.getTarget());
			OnlineUserList::send(s.getContent(), s.getTarget());
		default:
			break;
		}
	}
	return NULL;
}