#include "link.hpp"

#include <unistd.h>

#include <cstdio>

const int Link::bufferSize = 1024;
const int Link::maxWrite = 3;

Link::Link(): socketId(-1), vaild(false) {
}

Link::Link(int socketId): socketId(socketId), vaild(true) {
}

Link::~Link() {
	if(!vaild)return;
	close(socketId);
}

int Link::getSocketId() const {
	if(!vaild)return -1;
	return this->socketId;
}

void Link::setSocketId(int socket) {
	socketId=socket;
	vaild=true;
}

int Link::read(std::string& s) const {
	if(!vaild)return false;
	s.clear();
	int len, size, tmp;
	char buffer[bufferSize];
	if(buffer==NULL)perror("buffer");
	len=::read(socketId, &size, sizeof(int));
	if(len<0) {
		perror("read len error");
		return false;
	}
	size=le32toh(size);
	tmp=size;
	while(size>0) {
		len=std::min(bufferSize, size);
		len=::read(socketId, buffer, len);
		if(len<0) {
			perror("read data error");
			continue;
		}
		size-=len;
		s.append(buffer, len);
	}
	return tmp;
}

bool Link::write(const std::string& s) const {
	if(!vaild||s.empty())return false;
	const char* sp = s.c_str();
	int size=s.size(), len, writeNum=0;
	len=htole32(size);
	len=::write(socketId, &len, sizeof(int));
	if(len<0) {
		perror("write len error");
		return false;
	}
	while(size>0) {
		len=::write(socketId, sp, size);
		if(len<0) {
			perror("write data error");
			if((++writeNum)==maxWrite)return false;
			continue;
		}
		writeNum=0;
		sp+=len;
		size-=len;
	}
	return true;
}