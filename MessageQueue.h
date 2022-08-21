#ifndef _MESSAGE_QUEUE_HPP_
#define _MESSAGE_QUEUE_HPP_

#include "Message.h"

#include <queue>

class MessageQueue {
	static pthread_cond_t messageCond;
	static pthread_mutex_t messageMutex;
	static std::queue<Message>message;

	MessageQueue();
	MessageQueue(const MessageQueue&);
	MessageQueue& operator=(const MessageQueue&);
public:
	static void init();
	static void destory();

	static void insertMessage(const Message& s);
	static Message takeMessage();
};

#endif