#ifndef _USER_HPP_
#define _USER_HPP_

#include "link.h"
#include "MessageQueue.h"
#include "Buffer.h"

class User {
	bool vaild;
	int id;
	std::string name;
	const Link& link;
	MessageQueue& messageQueue;

	int writeInt32(int value);
	int writeString(const std::string& s);
public:
	User(int id, const std::string& name, const Link& link, MessageQueue& messageQueue);

	int readInt32(int& value);
	int readString(std::string& s, int n);

	int writeBuffer(const Buffer& buffer);

	void insertMessage(const Message& s);
	Message takeMessage();

	int getId() const;
	std::string getName() const;

	void logOut();
};

#endif