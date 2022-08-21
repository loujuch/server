#ifndef _BUFFER_HPP_
#define _BUFFER_HPP_

#include <string>

class Buffer {
	std::string s;
public:
	Buffer();

	void addInt32(int n);
	void addString(const std::string& s);
	void addHead();

	const std::string& data() const;
	int size() const;
};

#endif