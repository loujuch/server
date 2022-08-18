#include "listener.hpp"
#include "globe.hpp"

#include <thread>

int main(int argc, char* argv[]) {
	Globe::init();
	std::thread serve(Globe::serve);
	Listener listener(8080);
	listener.listen(Globe::slove);
	return 0;
}