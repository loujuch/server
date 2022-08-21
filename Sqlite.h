#ifndef _SQLITE_HPP_
#define _SQLITE_HPP_

#include <sqlite3.h>
#include <string>
#include <vector>
#include <utility>

typedef int (*callback)(void *data, int argc, char **ColValue, char **ColName);

class Sqlite {
	static pthread_mutex_t dbMutex;
	static sqlite3* db;

	Sqlite();
	Sqlite(const Sqlite&);
	Sqlite& operator=(const Sqlite&);
public:
	static bool init(const std::string& db_name);
	static void close();

	static bool createTable(const std::string& tableName, const std::vector<std::string>& domain);

	static bool insertItem(const std::string& tableName, 
				const std::vector<std::pair<std::string, std::string> >& key_value);
	// static bool updateItem(const std::string& tableName, 
	// 			const std::vector<std::pair<std::string, std::string> >& key_value, 
	// 			const std::string& condition = "");
	// static bool deleteItem(const std::string& tableName, const std::string& condition);
	static int countItem(const std::string& tableName, const std::string& condition);

	static bool selectItem(const std::string& exec, callback cb, void* data);
};

#endif