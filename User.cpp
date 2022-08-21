#include "User.h"

#include <cstdio>

User::User(int id, const std::string& name, const Link& link):id(id), name(name), link(link) {
}

int User::readInt32(int& value) {
	return link.readInt32(value);
}

int User::readString(std::string& s, int n) {
	return link.readString(s, n);
}

int User::writeInt32(int value) {
	return link.writeInt32(value);
}

int User::writeString(const std::string& s) {
	return link.writeString(s);
}

int User::getId() const {
	return id;
}

std::string User::getName() const {
	return name;
}