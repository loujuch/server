#include "listener.hpp"
#include "globe.hpp"

#include <pthread.h>

int main(int argc, char* argv[]) {
	Globe::init();
	pthread_t t;
	pthread_create(&t, NULL, Globe::serve, NULL);
	Listener listener(8080);
	listener.listen(Globe::slove);
	Globe::destory();
	return 0;
}