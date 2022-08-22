#include "User.h"

#include <cstdio>

User::User(int id, const std::string& name, const Link& link, MessageQueue& messageQueue):
	id(id), name(name), link(link), messageQueue(messageQueue) {
}

int User::readInt32(int& value) {
	return link.readInt32(value);
}

int User::readString(std::string& s, int n) {
	return link.readString(s, n);
}

int User::writeInt32(int value) {
	return 0; //link.writeInt32(value);
}

int User::writeString(const std::string& s) {
	return 0; //link.writeString(s);
}

int User::writeBuffer(const Buffer& buffer) {
	return link.writeBuffer(buffer);
}

void User::insertMessage(const Message& s) {
	return messageQueue.insertMessage(s);
}

Message User::takeMessage() {
	return messageQueue.takeMessage();
}

int User::getId() const {
	return id;
}

std::string User::getName() const {
	return name;
}