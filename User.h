#ifndef _USER_HPP_
#define _USER_HPP_

#include "link.h"
#include "Message.h"

class User {
	int id;
	std::string name;
	const Link& link;
public:
	User(int id, const std::string& name, const Link& link);

	int readInt32(int& value);
	int readString(std::string& s, int n);

	int writeInt32(int value);
	int writeString(const std::string& s);

	int getId() const;
	std::string getName() const;
};

#endif