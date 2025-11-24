#include <stdio.h> // printf
#include <sys/socket.h> // socket 
#include <arpa/inet.h> // htons
#include <unistd.h> // close
#include <string.h> // strlen strtok

int main() {
    	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    	
    	struct sockaddr_in server_address;
    	server_address.sin_family = AF_INET;
    	server_address.sin_port = htons(8080);
    	server_address.sin_addr.s_addr = INADDR_ANY;
    	
    	bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address));
    	listen(server_fd, 10);
    	
    	printf("Server listening on port 8080...\n");
    	
	// client info
	struct sockaddr_in client_address;
	socklen_t client_addr_len = sizeof(client_address);
	int client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_addr_len);
	
	char client_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
	int client_port = ntohs(client_address.sin_port);
	
	printf("Client connected from: %s:%d\n", client_ip, client_port);
	// client info end

    	// http headres 
    	char buffer[1024] = {0};
    	int bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

    	if (bytes_read > 0) {
    	    //printf("\n=== Received HTTP Request ===\n%s\n", buffer);

    	    // Parse request line (first line)
    	    char method[16], path[256], version[16];
    	    sscanf(buffer, "%s %s %s", method, path, version);

    	    printf("Method: %s\n", method);
    	    printf("Path: %s\n", path);
    	    printf("Version: %s\n", version);

    	    // Look for specific headers
    	    char *user_agent = strstr(buffer, "User-Agent:");
    	    if (user_agent) {
    	        user_agent = strtok(user_agent, "\r\n");
    	        printf("User-Agent: %s\n", user_agent + 11); // Skip "User-Agent:"
    	    }

    	    char *host = strstr(buffer, "Host:");
    	    if (host) {
    	        host = strtok(host, "\r\n");
    	        printf("Host: %s\n", host + 6); // Skip "Host: "
    	    }
    	}
    	// http headres end
    
	char *response = 
	    "HTTP/1.1 200 OK\r\n"
	    "Content-Type: text/plain\r\n"
	    "Content-Length: 13\r\n"
	    "\r\n"
	    "hello kitty";
	
	send(client_fd, response, strlen(response), 0);

    	printf("Response sent...\n\n");

    	close(client_fd);
    	close(server_fd);
    	return 0;
}
