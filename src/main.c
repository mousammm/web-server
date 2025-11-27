#include "Server.h" // printf
#include <stdio.h> // printf
		    
int main() {
    	Server* server = server_create(8000);
	int server_running = server_start(server);
	Client* client = server_accept_client(server); // uses is_runnign accept connection 

        client_info(client);

	char *response =
        	"HTTP/1.1 200 OK\r\n"
        	"Content-Type: text/plain\r\n"
        	"Content-Length: 13\r\n"
        	"\r\n"
        	"Hello, World!";

	send(client->fd, response, strlen(response), 0);

	printf("Response sent...\n\n");

    	close(client->fd);
    	close(server->fd);
	server_stop(server); // set is running = 0 
    	return 0;
}
