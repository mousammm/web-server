#include "Server.h"

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

