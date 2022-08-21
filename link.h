#ifndef _LINK_HPP_
#define _LINK_HPP_

#include <string>
#include "Buffer.h"

class Link {
	static const int bufferSize;
	static const int maxWrite;
	bool vaild;
	int socketId;

	Link(const Link&);
	Link& operator=(const Link&);

	int writeInt32(int value) const;
	int writeString(const std::string& s, bool set = true) const;
public:
	Link();
	Link(int socketId);

	int getSocketId() const;
	void setSocketId(int socket);

	int readInt32(int& value) const;
	int readString(std::string& s, int num) const;
	

	int writeBuffer(const Buffer& buffer) const;

	~Link();
};

#endif