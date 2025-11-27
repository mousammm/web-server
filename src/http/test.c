#include "server.h"

int main() {
    // Create and start server
    Server* server = server_create(PORT);
    if (!server) {
        fprintf(stderr, "Failed to create server\n");
        return 1;
    }

    if (server_start(server) < 0) {
        server_destroy(server);
        return 1;
    }

    // Main server loop
    while (server->is_running) {
        ClientInfo* client = server_accept_client(server);
        if (client) {
            client_handle_request(client);
            client_destroy(client);
        }
    }

    // Cleanup
    server_destroy(server);
    return 0;
}
