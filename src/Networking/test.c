#include <stdio.h>
#include <string.h>
#include <unistd.h> // read close
#include "Server.h"

void launch(struct Server *server)
{
	char buffer[30000];
	printf("3");
	while(1)
	{
		printf("===== WAITING CONNECTION =====");
		printf("4");
		int address_length = sizeof(server->address);
		int new_socket = accept(server->socket, (struct sockaddr*)&server->address, 
				(socklen_t *)&address_length);
		read(new_socket, buffer, 30000);
		printf("Buffer %s\n", buffer);
		char *hello = "HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2008 12:28:53 GMT\nServer: Appache/2.2.14 (win32)\nLast-Modified: Wed, 22 Jul 2009  19:22:12 GMT\nContent-Length: 88\nContent-Type: text/html\nConnection: Closed\n<html><body><h1>Hellow world</h1></Body></Html>";
		write(new_socket, hello, strlen(hello));
		close(new_socket);
	}
}

int main()
{
	printf("1");
	struct Server server = server_constructor(
			AF_INET, 
			SOCK_STREAM, 
			0,
			INADDR_ANY,
			80,
			10,
			launch
			);
	printf("2");
	server.launch(&server);

	return 0;
}
