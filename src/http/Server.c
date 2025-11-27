#include "server.h"

// Server creation and initialization
Server* server_create(int port) {
    Server* server = malloc(sizeof(Server));
    if (!server) return NULL;

    server->fd = socket(AF_INET, SOCK_STREAM, 0);
    server->port = port;
    server->is_running = 0;

    if (server->fd < 0) {
        free(server);
        return NULL;
    }

    // Set server address
    server->address.sin_family = AF_INET;
    server->address.sin_port = htons(port);
    server->address.sin_addr.s_addr = INADDR_ANY;

    return server;
}

void server_destroy(Server* server) {
    if (server) {
        if (server->fd >= 0) {
            close(server->fd);
        }
        free(server);
    }
}

int server_start(Server* server) {
    if (bind(server->fd, (struct sockaddr*)&server->address, sizeof(server->address)) < 0) {
        perror("Bind failed");
        return -1;
    }

    if (listen(server->fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        return -1;
    }

    server->is_running = 1;
    printf("Server started on port %d...\n", server->port);
    return 0;
}

void server_stop(Server* server) {
    server->is_running = 0;
}

ClientInfo* server_accept_client(Server* server) {
    if (!server->is_running) return NULL;

    ClientInfo* client = malloc(sizeof(ClientInfo));
    if (!client) return NULL;

    socklen_t client_len = sizeof(client->address);
    client->fd = accept(server->fd, (struct sockaddr*)&client->address, &client_len);
    
    if (client->fd < 0) {
        free(client);
        return NULL;
    }

    // Fill client information
    inet_ntop(AF_INET, &client->address.sin_addr, client->ip, INET_ADDRSTRLEN);
    client->port = ntohs(client->address.sin_port);
    
    // Get timestamp
    time_t now = time(NULL);
    strftime(client->timestamp, sizeof(client->timestamp), 
             "%Y-%m-%d %H:%M:%S", localtime(&now));

    return client;
}

void client_log_connection(ClientInfo* client) {
    printf("\n=== New Connection ===\n");
    printf("Time: %s\n", client->timestamp);
    printf("Client IP: %s\n", client->ip);
    printf("Client Port: %d\n", client->port);
}

void http_parse_request(const char* buffer, HttpRequest* request) {
    // Initialize request
    memset(request, 0, sizeof(HttpRequest));
    
    // Parse request line
    sscanf(buffer, "%15s %255s %15s", 
           request->method, request->path, request->version);
    
    // Parse headers
    char* line = strtok((char*)buffer, "\r\n");
    while (line != NULL) {
        if (strncasecmp(line, "User-Agent:", 11) == 0) {
            strncpy(request->user_agent, line + 12, sizeof(request->user_agent) - 1);
        }
        else if (strncasecmp(line, "Host:", 5) == 0) {
            strncpy(request->host, line + 6, sizeof(request->host) - 1);
        }
        else if (strncasecmp(line, "Content-Type:", 13) == 0) {
            strncpy(request->content_type, line + 14, sizeof(request->content_type) - 1);
        }
        else if (strncasecmp(line, "Content-Length:", 15) == 0) {
            request->content_length = atoi(line + 16);
        }
        line = strtok(NULL, "\r\n");
    }
}

void http_send_response(int client_fd, const char* body, const char* content_type) {
    char response[BUFFER_SIZE];
    snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        content_type, strlen(body), body);
    
    send(client_fd, response, strlen(response), 0);
}

void client_handle_request(ClientInfo* client) {
    // Log the connection
    client_log_connection(client);

    // Read HTTP request
    char buffer[BUFFER_SIZE] = {0};
    int bytes_read = recv(client->fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_read > 0) {
        // Parse the request
        HttpRequest request;
        http_parse_request(buffer, &request);

        printf("Request: %s %s\n", request.method, request.path);
        if (strlen(request.user_agent) > 0) {
            printf("User-Agent: %s\n", request.user_agent);
        }

        // Route handling
        if (strcmp(request.path, "/") == 0 || strcmp(request.path, "/home") == 0) {
            char response_body[256];
            snprintf(response_body, sizeof(response_body),
                    "<html><body><h1>Welcome Home!</h1><p>Client: %s</p></body></html>",
                    client->ip);
            http_send_response(client->fd, response_body, "text/html");
        }
        else if (strcmp(request.path, "/hello") == 0) {
            http_send_response(client->fd, "Hello World!", "text/plain");
        }
        else if (strcmp(request.path, "/time") == 0) {
            char time_response[128];
            snprintf(time_response, sizeof(time_response), 
                    "Server Time: %s\nClient IP: %s", 
                    client->timestamp, client->ip);
            http_send_response(client->fd, time_response, "text/plain");
        }
        else if (strcmp(request.path, "/info") == 0) {
            char info_response[512];
            snprintf(info_response, sizeof(info_response),
                    "Client Information:\n"
                    "IP: %s\n"
                    "Port: %d\n"
                    "Time: %s\n"
                    "User-Agent: %s",
                    client->ip, client->port, client->timestamp, request.user_agent);
            http_send_response(client->fd, info_response, "text/plain");
        }
        else {
            http_send_response(client->fd, "404 - Page Not Found", "text/plain");
        }

        printf("Response sent...\n\n");
    }
}

void client_destroy(ClientInfo* client) {
    if (client) {
        if (client->fd >= 0) {
            close(client->fd);
        }
        free(client);
    }
}
