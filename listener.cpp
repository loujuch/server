#include "listener.hpp"

#include <cstdio>
#include <cstring>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <pthread.h>

Listener::Listener(const short port):status(false), listenSocket(0) {
	char buffer[6]="\0";
	sprintf(buffer, "%hd", port);
	struct addrinfo hint, *list, *p;
	memset(&hint, 0, sizeof(hint));
	hint.ai_socktype=SOCK_STREAM;
	hint.ai_flags=AI_PASSIVE|AI_ADDRCONFIG|AI_NUMERICSERV;
	int tmp=getaddrinfo(NULL, buffer, &hint, &list);
	if(tmp<0) {
		perror(gai_strerror(tmp));
		return;
	}
	int optval=1;
	for(p=list;p!=NULL;p=p->ai_next) {
		if((listenSocket=socket(p->ai_family, p->ai_socktype, p->ai_protocol)) <0)continue;
		setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int));
		if(bind(listenSocket, p->ai_addr, p->ai_addrlen)!=-1)break;
		close(listenSocket);
	}
	freeaddrinfo(list);
	if(p==NULL)return;
	if(::listen(listenSocket,1024)<0) {
		close(listenSocket);
		return;
	}
	status=true;
}

void Listener::listen(Event event) const {
	if(!status)return;
	socklen_t size;
	struct sockaddr_in otherSocket;
	while(1) {
		int other;
		other=accept(listenSocket, (struct sockaddr*)&other, &size);
		if(other!=-1) {
			void* tmp;
			memcpy(&tmp, &other, sizeof(int));
			pthread_t t;
			pthread_create(&t, NULL, event, tmp);
			pthread_detach(t);
		} else {
			perror("accept error!");
		}
	}
}

Listener::~Listener() {
	if(!status)return;
	close(listenSocket);
}