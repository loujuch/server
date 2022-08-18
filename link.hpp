#ifndef _LINK_HPP_
#define _LINK_HPP_

#include <string>

class Link {
	static const int bufferSize;
	static const int maxWrite;
	bool vaild;
	int socketId;
public:
	Link();
	Link(int socketId);
	Link(const Link&) = delete;
	Link& operator=(const Link&) = delete;

	int getSocketId() const;
	void setSocketId(int socket);

	bool read(std::string& s) const;
	bool write(const std::string& s) const;

	~Link();
};

#endif