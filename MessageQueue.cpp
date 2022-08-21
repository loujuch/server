#include "MessageQueue.h"

pthread_cond_t MessageQueue::messageCond;
pthread_mutex_t MessageQueue::messageMutex;
std::queue<Message> MessageQueue::message;

void MessageQueue::init() {
	pthread_cond_init(&messageCond, NULL);
	pthread_mutex_init(&messageMutex, NULL);
	while(!message.empty())message.pop();
}

void MessageQueue::destory() {
	pthread_cond_destroy(&messageCond);
	pthread_mutex_destroy(&messageMutex);
}

void MessageQueue::insertMessage(const Message& s) {
	pthread_mutex_lock(&messageMutex);
	message.push(s);
	pthread_cond_signal(&messageCond);
	pthread_mutex_unlock(&messageMutex);
}

Message MessageQueue::takeMessage() {
	pthread_mutex_lock(&messageMutex);
	while(message.empty())pthread_cond_wait(&messageCond, &messageMutex);
	const Message out(message.front());
	message.pop();
	pthread_mutex_unlock(&messageMutex);
	return out;
}