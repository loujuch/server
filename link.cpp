#include "link.h"

#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>

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

int Link::readInt32(int& value) const {
	int size=4, tmp=size, len;
	char* p=(char*)&value;
	while(tmp>0) {
		len=::read(socketId, p, tmp);
		if(!len)return 0;
		if(len<0) {
			perror("Link::readInt32 read error!");
			continue;
		}
		tmp-=len;
		p+=len;
	}
	value=ntohl(value);
	return size-tmp;
}

int Link::writeInt32(int value) const {
	value=htonl(value);
	int size=4, tmp=size, len, writeNum=0;
	char* p=(char*)&value;
	while(tmp>0) {
		len=::write(socketId, p, tmp);
		if(len<=0) {
			perror("Link::writeInt32 write error!");
			if((++writeNum)==maxWrite)return false;
			continue;
		}
		writeNum=0;
		tmp-=len;
		p+=len;
	}
	return size-tmp;
}

int Link::readString(std::string& s, int size) const {
	if(!vaild)return false;
	s.clear();
	int len, tmp;
	char buffer[bufferSize];
	if(buffer==NULL)perror("buffer");
	tmp=size;
	while(tmp>0) {
		len=std::min(bufferSize, tmp);
		len=::read(socketId, buffer, len);
		if(len<0) {
			perror("Link::readString read data error");
			continue;
		}
		tmp-=len;
		s.append(buffer, len);
	}
	return size-tmp;
}

bool Link::writeString(const std::string& s) const {
	if(!vaild||s.empty())return false;
	const char* sp = s.c_str();
	int size=s.size(), len, writeNum=0;
	len=writeInt32(len);
	if(len<0) {
		perror("write len error");
		return false;
	}
	while(size>0) {
		len=::write(socketId, sp, size);
		if(len<=0) {
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