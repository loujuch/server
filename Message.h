#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

#include <string>
#include "ControlCode.h"

class Message {
	enum MessageType type;
	int source;
	int target;
	const std::string content;
public:
	Message(enum MessageType type, int source, int target, const std::string& s);
	Message(const Message& o);

	int getType() const;
	int getTarget() const;
	int getSource() const;
	const std::string& getContent() const;
};

#endif