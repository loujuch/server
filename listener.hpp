#ifndef _LISTENER_HPP_
#define _LISTENER_HPP_

#include <thread>

using Event = void (*)(int other, struct sockaddr_in otherSocket);

class Listener {
	bool status;
	int listenSocket;
public:
	Listener(const short port);

	void listen(Event event) const;

	~Listener();
};

#endif