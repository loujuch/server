#include "Sqlite.h"

#include <cstdio>
#include <cstdlib>

sqlite3* Sqlite::db=NULL;
pthread_mutex_t Sqlite::dbMutex;

bool Sqlite::init(const std::string& db_name) {
	pthread_mutex_init(&dbMutex, NULL);
	int flag=sqlite3_open(db_name.c_str(), &db);
	return flag==SQLITE_OK;
}

void Sqlite::close() {
	pthread_mutex_destroy(&dbMutex);
	if(db==NULL)return;
	sqlite3_close(db);
}

bool Sqlite::createTable(const std::string& tableName, const std::vector<std::string>& domain) {
	if(domain.empty()||tableName.empty())return false;
	char* errmsg=NULL;
	std::string exec="CREATE TABLE IF NOT EXISTS '"+tableName+"'("+domain[0];
	for(int i=1;i<domain.size();++i)exec.append(", "+domain[i]);
	exec.append(");");
	pthread_mutex_lock(&dbMutex);
	int flag=sqlite3_exec(db, exec.c_str(), NULL, NULL, &errmsg);
	pthread_mutex_unlock(&dbMutex);
	if(flag!=SQLITE_OK || errmsg!=NULL) {
		fprintf(stderr, "Sqlite::createTable: create table false\n \
				error code: %d\n error message: %s\n", flag, errmsg);
		sqlite3_free(errmsg);
	}
	return flag==SQLITE_OK;
}

bool Sqlite::insertItem(const std::string& tableName, 
				const std::vector<std::pair<std::string, std::string> >& key_value) {
	if(key_value.empty()||tableName.empty())return false;
	char* errmsg=NULL;
	std::string execHead="INSERT INTO '"+tableName+"'("+key_value[0].first;
	std::string execEnd=") VALUES ("+key_value[0].second;
	for(int i=1;i<key_value.size();++i) {
		execHead.append(", "+key_value[i].first);
		execEnd.append(", "+key_value[i].second);
	}
	std::string exec=execHead+execEnd+");";
	pthread_mutex_lock(&dbMutex);
	int flag=sqlite3_exec(db, exec.c_str(), NULL, NULL, &errmsg);
	pthread_mutex_unlock(&dbMutex);
	if(flag!=SQLITE_OK || errmsg!=NULL) {
		fprintf(stderr, "Sqlite::insertItem: insert item false\n \
				error code: %d\n error message: %s\n", flag, errmsg);
		sqlite3_free(errmsg);
	}
	return flag==SQLITE_OK;
}

bool Sqlite::selectItem(const std::string& exec, callback cb, void* data) {
	if(exec.empty())return false;
	char* errmsg=NULL;
	pthread_mutex_lock(&dbMutex);
	int flag=sqlite3_exec(db, exec.c_str(), cb, data, &errmsg);
	pthread_mutex_unlock(&dbMutex);
	if(flag!=SQLITE_OK || errmsg!=NULL) {
		fprintf(stderr, "Sqlite::selectItem: select item false\n \
				error code: %d\n error message: %s\n", flag, errmsg);
		sqlite3_free(errmsg);
	}
	return flag==SQLITE_OK;
}

int Sqlite::countItem(const std::string& tableName, const std::string& condition) {
	if(tableName.empty()||condition.empty())return false;
	char* errmsg=NULL;
	std::string exec="SELECT COUNT(*) AS num From '"+tableName+"' WHERE "+condition+";";
	char **result; 
	int row, column;
	pthread_mutex_lock(&dbMutex);
	int flag=sqlite3_get_table(db, exec.c_str(), &result, &row, &column, &errmsg);
	pthread_mutex_unlock(&dbMutex);
	if(flag!=SQLITE_OK||errmsg!=NULL||row!=1||column!=1) {
		fprintf(stderr, "Sqlite::insertItem: insert item false\n \
				error code: %d\n error message: %s\n", flag, errmsg);
		sqlite3_free_table(result);
		sqlite3_free(errmsg);
		return -1;
	}
	int n=atoi(result[1]);
	sqlite3_free_table(result);
	return n;
}

// bool Sqlite::updateItem(const std::string& tableName, 
// 				const std::vector<std::pair<std::string, std::string> >& key_value, 
// 				const std::string& condition = "") {
// 	return true;
// }

// bool Sqlite::deleteItem(const std::string& tableName, const std::string& condition) {
// 	return true;
// }