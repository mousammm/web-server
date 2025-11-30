// HTTP 
#include "Server.h"

// parse the req that client had given and put it in HttpRequest struct
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

// http_send_response() - more flexible
void http_send_response(Client* client, HttpResponse* response)
{
    char headers[1024];
    snprintf(headers, sizeof(headers),
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n",
        response->status_code, response->status_text,
        response->content_type, response->content_length);

    write(client->fd, headers, strlen(headers));
    write(client->fd, response->content, response->content_length);
    
    // Full context available!
    printf("\nHTTP_RESPONSE:");
    printf("%d %s %s %ld\n{{%s}}\n\n", response->status_code, response->status_text, response->content_type,  response->content_length, response->content);

}

void http_send_html(Client* client, const char* html)
{
    HttpResponse res = {
        .status_code = 200,
        .status_text = "OK", 
        .content_type = "text/html",
        .content = (char*)html,
        .content_length = strlen(html)
    };
    http_send_response(client, &res);
}

void http_send_text(Client* client, const char* text)
{
    HttpResponse res = {
        .status_code = 200,
        .status_text = "OK", 
        .content_type = "text/plain",
        .content = (char*)text,
        .content_length = strlen(text)
    };
    http_send_response(client, &res);
}

void http_send_json(Client* client, const char* json)
{
    HttpResponse res = {
        .status_code = 200,
        .status_text = "OK", 
        .content_type = "application/json",
        .content = (char*)json,
        .content_length = strlen(json)
    };
 
    http_send_response(client, &res);
}

void http_send_error(Client* client, int code, const char* message)
{
    char error_html[512];
    snprintf(error_html, sizeof(error_html),
        "<html><body><h1>Error %d</h1><p>%s</p></body></html>",
        code, message);

    HttpResponse res = {
        .status_code = code,
        .status_text = "Internal Server Error", // Use appropriate status text
        .content_type = "text/html",
        .content = error_html,
        .content_length = strlen(error_html)
    };

    http_send_response(client, &res); // Assuming this function exists
}
