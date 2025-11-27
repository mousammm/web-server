#include "Server.h"
#include <stdio.h>
#include <time.h>

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

Client* server_accept_client(Server* server)
{
	if(!server->is_running )
	{
		printf("Server is not running...\n");
		return NULL;
	}

	printf("Waiting for incomming connection...\n");

	Client* client = malloc(sizeof(Client));

	socklen_t client_addr_length = sizeof(client->address);
	client->fd = accept(server->fd, (struct sockaddr*)&client->address, &client_addr_length);
	                                      // client info                  // only accept pointer
	// Fill client infomaton
	inet_ntop(AF_INET, &client->address.sin_addr, client->ip, INET_ADDRSTRLEN); // get client ip
        client->port = ntohs(client->address.sin_port);	 // get client port 
         						 
        time_t now = time(NULL); // get timestamp
	strftime(client->timestamp, sizeof(client->timestamp), 
			"%Y-%m-%d %H:%M:%S", localtime(&now));
		
	return client;
}

void client_info(Client* client) {
    printf("\n\n=== New Connection ===\n");
    printf("Client IP: %s\n", client->ip);
    printf("Client Port: %d\n", client->port);
    printf("Time: %s\n\n", client->timestamp);
}


