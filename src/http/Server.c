#include "Server.h"

// SERVER
Server* server_create(int port)
{
	Server* server = malloc(sizeof(Server)); 

	server->fd = socket(AF_INET, SOCK_STREAM, 0);
	server->port = port;
	server->is_running = 0;
	
	server->address.sin_family = AF_INET;
	server->address.sin_port = htons(port);
	server->address.sin_family = INADDR_ANY;

	return server;
}

int server_start(Server* server)
{
	bind(server->fd, (struct sockaddr*)&server->address, sizeof(server->address));
	listen(server->fd, 10);
	server->is_running = 1;
	printf("Server is running on port %d..\n", server->port);

	return 0;
}

void server_stop(Server* server)
{
	server->is_running = 0;
}

