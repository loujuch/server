#ifndef _MESSAGE_QUEUE_HPP_
#define _MESSAGE_QUEUE_HPP_

#include "Message.h"

#include <queue>

class MessageQueue {
	pthread_cond_t messageCond;
	pthread_mutex_t messageMutex;
	std::queue<Message>message;

	MessageQueue(const MessageQueue&);
	MessageQueue& operator=(const MessageQueue&);
public:
	MessageQueue();
	~MessageQueue();

	void insertMessage(const Message& s);
	Message takeMessage();
};

#endif