// CLIENT
#include "Server.h"

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

void client_logs(Client* client)
{
    printf("\n\n=== New Connection ===\n");
    printf("Client IP: %s\n", client->ip);
    printf("Client Port: %d\n", client->port);
    printf("Time: %s\n\n", client->timestamp);
}

void client_handle_request(Client* client){
	// Log client ip port time
	client_logs(client);

	char buffer[4096] = {0};
    	int bytes_read = recv(client->fd, buffer, sizeof(buffer) - 1, 0);// Read HTTP request from client socket into buffer

    	if (bytes_read > 0) {
    		//buffer[bytes_read] = '\0';  // Null-terminate the string
    		//printf("Raw HTTP Request:\n%s\n", buffer);

    		// Parse the buffer containing HTTP request
    		HttpRequest request; // only need to fill http headers struct
        	http_parse_request(buffer, &request);  // return noting 

    		// Now you have structured data
    		printf("HTTP_REQUEST:");
    		printf("%s-%s-%s\n\n", request.method, request.path, request.version );

		// Route handlaing 
		router_handle_request(client, &request);

    	} // if end

        printf("Response sent...\n\n");
}
