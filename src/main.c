#include <stdio.h>
#include <sys/socket.h>     // socket
#include <netinet/in.h>     // sockaddr
#include <unistd.h>         // close
#include <string.h>         // strlen
#include <arpa/inet.h>      // inet_ntop

#define PORT 8080

typedef struct {
    char method[16];
    char path[256];
    char version[16];
} Req_t;

int parse_req(int client_fd, Req_t* req); // parse http req

int main() 
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address, client_address;
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr = INADDR_ANY;

	bind(server_fd, (struct sockaddr*)&address, sizeof(address));
	listen(server_fd, 10);

    printf("Server running on port %d...\n", PORT);


    Req_t req = {0};
    char buffer[1024] = {0}; 

    while(1)
    {
	    socklen_t client_addr_length = sizeof(client_address);                      // address length of client 
	    int client_fd = accept(server_fd, (struct sockaddr*)&client_address, &client_addr_length);

        // Get client info
        char client_ip[INET_ADDRSTRLEN];
	    inet_ntop(AF_INET, &client_address.sin_addr, client_ip, sizeof(client_ip));      // get client ip
        int client_port = ntohs(client_address.sin_port);	                            // get client port 
        printf("client: %s:%d\n", client_ip, client_port);

        
        memset(buffer, 0, sizeof(buffer));  // Reset buffer for new request
        parse_req(client_fd, &req);         // get http req GET path Version
        printf("path: %s\n\n", req.path);   // log

        char *res = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 13\r\n"
            "\r\n"
            "Hello, World!";

        send(client_fd, res, strlen(res), 0);
        close(client_fd);
    };

    close(server_fd);

    return 0;
}

int parse_req(int client_fd, Req_t* req)
{

    // read request 
    int bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if(bytes_read <= 0) return -1;
    buffer[bytes_read] = '\0';

    // Parse method, path, and version
    char *saveptr;
    char *line = strtok_r(buffer, "\r\n", &saveptr);
    if (line == NULL) return -1;

    // parse first line: GET /path HTTP/1.1
    char *method  = strtok(line, " ");
    char *path    = strtok(NULL, " ");
    char *version = strtok(NULL, " ");

    // Copy to struct (safe bounds-checked)
    strncpy(req->method, method, sizeof(req->method) - 1);
    req->method[sizeof(req->method) - 1] = '\0';
    
    strncpy(req->path, path, sizeof(req->path) - 1);
    req->path[sizeof(req->path) - 1] = '\0';
    
    strncpy(req->version, version, sizeof(req->version) - 1);
    req->version[sizeof(req->version) - 1] = '\0';
    
    return 0;
}
