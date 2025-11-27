#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h> // socket 
#include <arpa/inet.h> // htons 
#include <unistd.h> // close
#include <string.h> // strlen 
#include <stdlib.h> // malloc

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

// Function declarations
Server* server_create(int port); // set the TCP ipv4
int server_start(Server* server); // bind listen
void server_stop(Server* server); // set is_running = 0

Client* server_accept_client(Server* server); // is_running > 0 then accept & get client info
void client_info(Client* client); // get client info ip port time



#endif
