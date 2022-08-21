#include "LogUtils.h"
#include "UserSQL.h"
#include "ControlCode.h"

bool LogUtils::LogIn(const Link& link, int& id, std::string& name) {
	std::string accunt, passwd;
	link.readString(accunt, 32);
	link.readString(passwd, 32);
	if(Sqlite::countItem("user", "account='"+accunt+"' OR name='"+name+"';")!=1) {
		return false;
	}
	if(!UserSQL::search(id, accunt, passwd, name)) {
		return false;
	}
	link.writeInt32(LogSuccess);
	link.writeInt32(id);
	link.writeString(name);
	return true;
}

bool LogUtils::LogUp(const Link& link, int& id, std::string& name) {
	int n;
	std::string accunt, passwd;
	link.readString(accunt, 32);
	link.readString(passwd, 32);
	link.readInt32(n);
	link.readString(name, n);
	if(Sqlite::countItem("user", "account='"+accunt+"' OR name='"+name+"';")) {
		return false;
	}
	UserSQL::insert(accunt, passwd, name);
	if(!UserSQL::search(id, accunt, passwd, name)) {
		return false;
	}
	link.writeInt32(LogSuccess);
	link.writeInt32(id);
	link.writeString(name);
	return true;
}