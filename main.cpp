#include "listener.h"
#include "globe.h"
#include "MessageQueue.h"
#include "OnlineUserList.h"
#include "Sqlite.h"
#include "UserSQL.h"

#include <pthread.h>

int main(int argc, char* argv[]) {
	Sqlite::init("test.db");
	OnlineUserList::init();
	UserSQL::create();
	Listener listener(8080);
	listener.listen(Globe::slove);
	OnlineUserList::destory();
	Sqlite::close();
	return 0;
}