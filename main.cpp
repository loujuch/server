#include "listener.h"
#include "globe.h"
#include "MessageQueue.h"
#include "OnlineUserList.h"
#include "Sqlite.h"

#include <pthread.h>

int main(int argc, char* argv[]) {
	Sqlite::init("test.db");
	MessageQueue::init();
	OnlineUserList::init();
	pthread_t t;
	pthread_create(&t, NULL, Globe::server, NULL);
	Listener listener(8080);
	listener.listen(Globe::slove);
	OnlineUserList::destory();
	MessageQueue::destory();
	Sqlite::close();
	return 0;
}