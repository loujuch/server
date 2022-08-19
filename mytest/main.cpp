#include <cstring>
#include <cstdio>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "../link.hpp"

#include <thread>
#include <fstream>
#include <iostream>

Link l;

bool finish=false;

int open_clientfd(const char*hostname,const char*port) {
        int fd=0;
        struct addrinfo hint,*p,*list;
        memset(&hint,0,sizeof(struct addrinfo));
        hint.ai_socktype=SOCK_STREAM;
        hint.ai_flags=AI_NUMERICSERV|AI_ADDRCONFIG;
        fd=getaddrinfo(hostname,port,&hint,&list);
        if(fd<0) {
                printf("%s\n",gai_strerror(fd));
                return -1;
        }
        for(p=list;p!=NULL;p=p->ai_next) {
                if((fd=socket(p->ai_family,p->ai_socktype,p->ai_protocol))<0)continue;
                if(connect(fd,p->ai_addr,p->ai_addrlen)!=-1)break;
                close(fd);
        }
        freeaddrinfo(list);
        if(p==NULL)return -1;
        else return fd;
}

void mr(const char* filename) {
	std::ofstream f(filename, std::ios::app);
	while(!finish) {
		std::string s;
		l.read(s);
		std::cout<<"will read: "<<s<<' '<<(finish?"true":"false")<<std::endl;
		f<<s<<"\n";
	}
	f.close();
}

void mw() {
	static const int BUFFERSIZE=1024*8;
	char* s=new char[BUFFERSIZE];
	while(!finish) {
		scanf("%s", s);
		if(!strcmp(s, "quit")) {
			std::cout<<"mw quit\n";
			finish=true;
		}
		printf("will send: %s, %s\n", s, finish?"true":"false");
		l.write(s);
	}
	delete[]s;
}

int main(int argc, char* argv[]) {
	if(argc!=2) {
		printf("%d\n", argc);
		return 1;
	}
	int fd=open_clientfd("101.34.216.93", "8080");
	// int fd=open_clientfd("127.0.0.1", "8080");
	if(fd==-1) {
		perror("fd error");
		return -1;
	}
	l.setSocketId(fd);
	printf("link\n");
	std::thread tr(mr, argv[1]);
	std::thread tw(mw);
	printf("wait\n");
	tr.join();
	tw.join();
	printf("end\n");
	return 0;
}
