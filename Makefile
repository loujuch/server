main: *.h *.cpp
	g++ *.h *.cpp -lpthread -lsqlite3 -o main -std=c++98 -g
