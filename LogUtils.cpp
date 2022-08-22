#include "LogUtils.h"
#include "UserSQL.h"
#include "ControlCode.h"
#include "Buffer.h"
#include "OnlineUserList.h"

#include <cstdio>

bool LogUtils::LogIn(const Link& link, int& id, std::string& name) {
	printf("LogUtils::LogIn will read user mseeage\n");
	int n;
	std::string accunt, passwd;
	link.readInt32(n);
	link.readString(accunt, n);
	link.readInt32(n);
	link.readString(passwd, n);
	printf("LogUtils::LogIn finish read user mseeage account:%s, passwd:%s\n", 
		accunt.c_str(), passwd.c_str());
	Buffer buffer;
	if((n=Sqlite::countItem("user", "account='"+accunt+"' AND passwd='"+passwd+"';"))!=1) {
		printf("LogUtils::LogIn will send error message: user is not exist!\n");
		buffer.addInt32(Error);
		buffer.addString("user is not exist!\n");
		link.writeBuffer(buffer);
		printf("LogUtils::LogIn finish send error message: user is not exist!\n");
		return false;
	}
	if(!UserSQL::search(id, accunt, passwd, name)||id<=0) {
		printf("LogUtils::LogIn will send error message: select error!\n");
		buffer.addInt32(Error);
		buffer.addString("select error!\n");
		link.writeBuffer(buffer);
		printf("LogUtils::LogIn finish send error message: select error!\n");
		return false;
	}
	if(OnlineUserList::existUser(id)) {
		printf("LogUtils::LogIn will send error message: user had login!\n");
		buffer.addInt32(Error);
		buffer.addString("user had login!\n");
		link.writeBuffer(buffer);
		printf("LogUtils::LogIn finish send error message: user had login!\n");
		return false;
	}
	printf("LogUtils::LogIn will write user mseeage id:%d, account:%s, passwd:%s, name:%s\n", 
		id, accunt.c_str(), passwd.c_str(), name.c_str());
	buffer.addInt32(LogSuccess);
	buffer.addInt32(id);
	buffer.addString(name);
	link.writeBuffer(buffer);
	printf("LogUtils::LogIn finish write user mseeage id:%d, account:%s, passwd:%s, name:%s\n", 
		id, accunt.c_str(), passwd.c_str(), name.c_str());
	return true;
}

bool LogUtils::LogUp(const Link& link, int& id, std::string& name) {
	printf("LogUtils::LogUp will read user mseeage");
	int n;
	std::string accunt, passwd;
	link.readInt32(n);
	link.readString(accunt, n);
	link.readInt32(n);
	link.readString(passwd, n);
	link.readInt32(n);
	link.readString(name, n);
	printf("LogUtils::LogUp finish read user mseeage account:%s, passwd:%s, name:%s\n", 
		accunt.c_str(), passwd.c_str(), name.c_str());
	Buffer buffer;
	if(Sqlite::countItem("user", "account='"+accunt+"' OR name='"+name+"';")) {
		printf("LogUtils::LogUp will send error message: user is exist!\n");
		buffer.addInt32(Error);
		buffer.addString("user is exist!\n");
		link.writeBuffer(buffer);
		printf("LogUtils::LogUp finish send error message: user is exist!\n");
		return false;
	}
	UserSQL::insert(accunt, passwd, name);
	if(!UserSQL::search(id, accunt, passwd, name)||id<=0) {
		printf("LogUtils::LogUp will send error message: select error!\n");
		buffer.addInt32(Error);
		buffer.addString("select error!\n");
		link.writeBuffer(buffer);
		printf("LogUtils::LogUp finish send error message: select error!\n");
		return false;
	}
	printf("LogUtils::LogUp will write user mseeage id:%d, account:%s, passwd:%s, name:%s\n", 
		id, accunt.c_str(), passwd.c_str(), name.c_str());
	buffer.addInt32(LogSuccess);
	buffer.addInt32(id);
	buffer.addString(name);
	link.writeBuffer(buffer);
	printf("LogUtils::LogUp finish write user mseeage id:%d, account:%s, passwd:%s, name:%s\n", 
		id, accunt.c_str(), passwd.c_str(), name.c_str());
	return true;
}