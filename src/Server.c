#include "Server.h"
#include <stdio.h>
#include <time.h>

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

// CLIENT
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
    		HttpRequest request; // only need to fill http headers
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

// HTTP
void http_parse_request(const char* buffer, HttpRequest* request)
{
	// initialize request 
	memset(request, 0, sizeof(HttpRequest));

	// Parse request line 
	sscanf(buffer, "%15s %255s %15s", 
			request->method, request->path, request->version);

	// Parse headers
    	char* line = strtok((char*)buffer, "\r\n");
    	while (line != NULL) {
    	    if (strncasecmp(line, "User-Agent:", 11) == 0) {
    	        strncpy(request->user_agent, line + 12, sizeof(request->user_agent) - 1);
    	    }
    	    else if (strncasecmp(line, "Host:", 5) == 0) {
    	        strncpy(request->host, line + 6, sizeof(request->host) - 1);
    	    }
    	    else if (strncasecmp(line, "Content-Type:", 13) == 0) {
    	        strncpy(request->content_type, line + 14, sizeof(request->content_type) - 1);
    	    }
    	    else if (strncasecmp(line, "Content-Length:", 15) == 0) {
    	        request->content_length = atoi(line + 16);
    	    }
    	    line = strtok(NULL, "\r\n");
    	}
}

// send response to the client 
void http_send_response(Client* client, const char* body, const char* content_type) {
    char response[4096];
    snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        content_type, strlen(body), body); // body is actual data like html api file 
    
    send(client->fd, response, strlen(response), 0);

     // Full context available!
    printf("Sent %ld bytes to %s\n",
           strlen(response), client->ip);
}

