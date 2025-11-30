#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h> // socket 
#include <arpa/inet.h> // htons 
#include <unistd.h> // close
#include <string.h> // strlen 
#include <strings.h> // strcasecmp
#include <stdlib.h> // malloc
#include <stdio.h>
#include <time.h>

// Server struct
typedef struct {
    int fd;
    int port;
    int is_running;
    struct sockaddr_in address;
} Server;

typedef struct {
    int fd;
    struct sockaddr_in address;
    char ip[INET_ADDRSTRLEN];  // Store IP as string
    int port;                  // Store port
    char timestamp[20];        // Store connection time
} Client;

// HTTP Request from client
typedef struct {
    char method[16]; // GET POST PUT etc
    char path[256];  // localhost:8080/path
    char version[16];
    char user_agent[256];
    char host[128];
    char content_type[128]; // Empty for get request 
    int content_length;     // 0 for get request
} HttpRequest;

// http response from server
typedef struct {
    int status_code;
    const char* status_text;
    const char* content_type;  // From SERVER (for response)
    const char* content;       // From SERVER
    size_t content_length;     // From SERVER (for response)
} HttpResponse;

// server
Server* server_create(int port); // set the TCP ipv4
int server_start(Server* server); // bind listen
void server_stop(Server* server); // set is_running = 0

// client 
Client* server_accept_client(Server* server); // is_running > 0 then accept & get client info
void client_logs(Client* client); // get client info ip port time
void client_handle_request(Client* client); 
// client_logs >
// parse http request with client>fd buffer > 
// route handeling

// http
void http_parse_request(const char* buffer, HttpRequest* request);
void http_send_response_new(Client* client, HttpResponse* response);
void http_send_html(Client* client, const char* html); // new 
void http_send_text(Client* client, const char* text);
void http_send_json(Client* client, const char* json);
void http_send_error(Client* client, int code, const char* message);
						       
// Routing 
void router_handle_request(Client* client, HttpRequest* request);

#endif
