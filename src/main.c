#include <stdio.h>
#include <sys/socket.h>     // socket
#include <netinet/in.h>     // sockaddr
#include <unistd.h>         // close
#include <string.h>         // strlen

#define PORT 8080
		    
int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr = INADDR_ANY;

	bind(server_fd, (struct sockaddr*)&address, sizeof(address));
	listen(server_fd, 10);

    int client_fd = accept(server_fd, NULL, NULL); // null: no need client info

    char *res = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 13\r\n"
        "\r\n"
        "Hello, World!";

    send(client_fd, res, strlen(res), 0);

    close(client_fd);
    close(server_fd);

    return 0;
}
