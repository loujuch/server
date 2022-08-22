#include "MessageQueue.h"

MessageQueue::MessageQueue() {
	pthread_cond_init(&messageCond, NULL);
	pthread_mutex_init(&messageMutex, NULL);
	while(!message.empty())message.pop();
}

MessageQueue::~MessageQueue() {
	pthread_cond_destroy(&messageCond);
	pthread_mutex_destroy(&messageMutex);
}

// 将消息加入消息队列
void MessageQueue::insertMessage(const Message& s) {
	pthread_mutex_lock(&messageMutex);
	message.push(s);
	pthread_mutex_unlock(&messageMutex);
	pthread_cond_signal(&messageCond);
}

// 将消息从消息队列中取出
Message MessageQueue::takeMessage() {
	pthread_mutex_lock(&messageMutex);
	while(message.empty())pthread_cond_wait(&messageCond, &messageMutex);
	const Message out(message.front());
	message.pop();
	pthread_mutex_unlock(&messageMutex);
	return out;
}