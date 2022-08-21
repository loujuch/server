#include "UserSQL.h"

#include <cstdlib>

bool UserSQL::create() {
	std::vector<std::string> typeUser;
	typeUser.push_back("id INTEGER PRIMARY KEY AUTOINCREMENT");
	typeUser.push_back("account CHAR(32) NOT NULL");
	typeUser.push_back("passwd CHAR(32) NOT NULL");
	typeUser.push_back("name CHAR(16) NOT NULL");
	return Sqlite::createTable("user", typeUser);
}

bool UserSQL::insert(const std::string& accunt, const std::string& passwd, const std::string& name) {
	std::vector<std::pair<std::string, std::string> > insert;
	insert.push_back(make_pair("account", "'"+accunt+"'"));
	insert.push_back(make_pair("passwd", "'"+passwd+"'"));
	insert.push_back(make_pair("name", "'"+name+"'"));
	return Sqlite::insertItem("user", insert);
}

//if id = -1, not one
bool UserSQL::search(int& id, const std::string& account, const std::string& passwd, std::string& name) {
	std::string exec="SELECT id, name FROM user where account='"+account+"' and passwd='"+passwd+"';";
	std::pair<int, std::string>data;
	data.first=-1;
	bool b=Sqlite::selectItem(exec, select, &data);
	if(!b)return false;
	id=data.first;
	name=data.second;
	return b;
}

int UserSQL::select(void *data, int argc, char **ColValue, char **ColName) {
	if(argc!=2)return -1;
	std::pair<int, std::string>* pdata=(std::pair<int, std::string>*)data;
	pdata->first=atoi(ColValue[0]);
	pdata->second=ColValue[1];
	return 0;
}