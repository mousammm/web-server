#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define MAX_CLIENTS 10

// Client information struct
typedef struct {
    int fd;
    struct sockaddr_in address;
    char ip[INET_ADDRSTRLEN];
    int port;
    char timestamp[64];
} ClientInfo;

// HTTP Request struct
typedef struct {
    char method[16];
    char path[256];
    char version[16];
    char user_agent[256];
    char host[128];
    char content_type[128];
    int content_length;
} HttpRequest;

// Server struct
typedef struct {
    int fd;
    struct sockaddr_in address;
    int port;
    int is_running;
} Server;

// Function declarations
Server* server_create(int port); // done 
void server_destroy(Server* server);
int server_start(Server* server); // done 
void server_stop(Server* server); // done 
ClientInfo* server_accept_client(Server* server); // done 
void client_handle_request(ClientInfo* client);
void client_destroy(ClientInfo* client);
void http_parse_request(const char* buffer, HttpRequest* request);
void http_send_response(int client_fd, const char* body, const char* content_type);

#endif
