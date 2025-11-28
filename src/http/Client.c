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

void client_logs(Client* client) {
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
    		printf("Method: %s\nPath: %s\nVersion: %s\nUser_Agent: %s\nHost: %s\nContent_type: %s\nContent_Length: %d\n\n", 
			    request.method, request.path, request.version,
			    request.user_agent, request.host, 
			    request.content_type,
			    request.content_length
			    );

		// Route handlaing 
        	if (strcmp(request.path, "/") == 0 || strcmp(request.path, "/home") == 0) {
        	    char response_body[256];
        	    snprintf(response_body, sizeof(response_body),
        	            "<html><body><h1>Welcome Home!</h1><p>Client: %s</p></body></html>",
        	            client->ip);
        	    http_send_response(client, response_body, "text/html");
        	}
		else if (strcmp(request.path, "/json") == 0 ) {
        	    http_send_response(client, "[{\"id\": 1, \"name\": \"John\"}]", "application/json");
        	}
        	else if (strcmp(request.path, "/hello") == 0) {
        	    http_send_response(client, "Hello World!", "text/plain");
        	}
        	else if (strcmp(request.path, "/time") == 0) {
        	    char time_response[128];
        	    snprintf(time_response, sizeof(time_response), 
        	            "Server Time: %s\nClient IP: %s", 
        	            client->timestamp, client->ip);
        	    http_send_response(client, time_response, "text/plain");
        	}
        	else if (strcmp(request.path, "/info") == 0) {
        	    char info_response[512];
        	    snprintf(info_response, sizeof(info_response),
        	            "Client Information:\n"
        	            "IP: %s\n"
        	            "Port: %d\n"
        	            "Time: %s\n"
        	            "User-Agent: %s",
        	            client->ip, client->port, client->timestamp, request.user_agent);
        	    http_send_response(client, info_response, "text/plain");
        	}
        	else {
        	    http_send_response(client, "404 - Page Not Found", "text/plain");
        	}

    	} // if end

        printf("Response sent...\n\n");
}

