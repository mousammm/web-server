#include "Server.h"
#include <stdio.h>
		    
int main() {
    
    Server* server = server_create(8000);
	Client* client = server_accept_client(server); // uses is_runnign accept connection 

	// log client ip timestamp time 
	// read http headers in buffer | and log them like GET PATH VERSION CONTENT TYPE LENGTH
	// route handling
	client_handle_request(client);

    close(client->fd);
    close(server->fd);
	//server_stop(server); // set is running = 0 
    return 0;
}
