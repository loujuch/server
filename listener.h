#ifndef _LISTENER_HPP_
#define _LISTENER_HPP_

typedef void* (*Event)(void* other);

class Listener {
	bool status;
	int listenSocket;
public:
	Listener(const short port);

	void listen(Event event) const;

	~Listener();
};

#endif