#include "Buffer.h"

#include <arpa/inet.h>

Buffer::Buffer():s("") {
}

void Buffer::addInt32(int n) {
	n=htonl(n);
	s.append((char*)&n, 4);
}

void Buffer::addString(const std::string& s) {
	addInt32(s.size());
	this->s+=s;
}

void Buffer::addHead() {
	int n=s.size()+4;
	n=htonl(n);
	s=std::string((char*)&n, 4)+s;
}

const std::string& Buffer::data() const {
	return s;
}

int Buffer::size() const {
	return s.size();
}