#include "Message.h"

Message::Message(enum MessageType type, int source, int target, const std::string& s):
	type(type), source(source), target(target), content(s) {
}

Message::Message(const Message& o):type(o.type), source(o.source), target(o.target), content(o.content) {
}

int Message::getTarget() const {
	return target;
}

const std::string& Message::getContent() const {
	return content;
}

int Message::getType() const {
	return type;
}

int Message::getSource() const {
	return source;
}