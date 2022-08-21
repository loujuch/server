#ifndef _LINK_HPP_
#define _LINK_HPP_

#include <string>

class Link {
	static const int bufferSize;
	static const int maxWrite;
	bool vaild;
	int socketId;

	Link(const Link&);
	Link& operator=(const Link&);
public:
	Link();
	Link(int socketId);

	int getSocketId() const;
	void setSocketId(int socket);

	int readInt32(int& value) const;
	int writeInt32(int value) const;

	int readString(std::string& s, int num) const;
	bool writeString(const std::string& s) const;

	~Link();
};

#endif