#ifndef Server_h
#define Server_h

#include <sys/socket.h>
#include <netinet/in.h> // u_long

struct Server
{
	int domain;
	int service;
	int protocol;
	u_long interface;
	int port;
	int backlog;
	struct sockaddr_in address;	
	int socket;
	//member fun for launching server
	void (*launch)(struct Server *server);
};

struct Server server_constructor(
		int domain,
	       	int service,
	       	int protocol,
	       	u_long interface,
	       	int port,
	       	int backlog,
		void (*launch)(struct Server *server)
		);

#endif 

