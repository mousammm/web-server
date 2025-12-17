#include <stdio.h>
#include <sys/socket.h>     // socket
#include <netinet/in.h>     // sockaddr
#include <unistd.h>         // close
#include <string.h>         // strlen
#include <arpa/inet.h>      // inet_ntop

#define PORT 8080

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address, client_address;
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr = INADDR_ANY;

	bind(server_fd, (struct sockaddr*)&address, sizeof(address));
	listen(server_fd, 10);

    printf("Server running on port %d...\n", PORT);


    while(1)
    {
	    socklen_t client_addr_length = sizeof(client_address);                      // address length of client 
	    int client_fd = accept(server_fd, (struct sockaddr*)&client_address, &client_addr_length);

        // Get client info
        char client_ip[INET_ADDRSTRLEN];
	    inet_ntop(AF_INET, &client_address.sin_addr, client_ip, sizeof(client_ip));      // get client ip
        int client_port = ntohs(client_address.sin_port);	                            // get client port 
        printf("client: %s:%d\n", client_ip, client_port);

        // get http req
        char buffer[256] = {0};         // GET /file.html ...
        recv(client_fd, buffer, 256, 0);

        // parse http request
        char* f = buffer + 5;   // file.html ...
        *strchr(f, ' ') = 0;    // file.html0...
        /* printf("path: %s\n\n", f); */
        printf("path: %s\n\n", buffer);

        
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
