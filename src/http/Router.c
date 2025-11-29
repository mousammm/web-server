#include "Server.h"

const char* get_content_type(const char* filename);
void serve_static_file(Client* client, const char* filename);

void router_handle_request(Client* client, HttpRequest* request)
{
	printf("Routing: %s %s \n", request->method, request->path);

	// simple routing 
		if (strcmp(request->path, "/home") == 0) {
    	    		http_send_html(client, "<h1>Welcome</h1><p>Home page</p>/hello /time /info");
    		}
		else if (strcmp(request->path, "/json") == 0 ) {
        	    //http_send_response(client, "[{\"id\": 1, \"name\": \"John\"}]", "application/json");
        	}
		else if (strcmp(request->path, "/hola") == 0) {
    		    printf("Trying to serve file\n");
		    serve_static_file(client, "index.html");
		}
		else if (strchr(request->path, '.') != NULL) {
       			 printf("File request detected: %s\n", request->path);
       			 // Remove the leading '/' and serve the file
       			 serve_static_file(client, request->path + 1);
    		}

        	else if (strcmp(request->path, "/hello") == 0) {
    	    		http_send_html(client, "<h1>Hello</h1>");
        	}
        	else if (strcmp(request->path, "/time") == 0) {
    	    		http_send_html(client, "<h1>time</h1>");
        	}
        	else if (strcmp(request->path, "/info") == 0) {
    	    		http_send_html(client, "<h1>info</h1>");
       		}
        	else {
    	    		http_send_html(client, "<h1>404 page not found!</h1>");
        	}

}


void serve_static_file(Client* client, const char* filename) {
    printf("Trying to serve file: %s\n", filename);

    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("File not found: %s\n", filename);
        //http_send_error(client, 404, "File Not Found");
        return;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* content = malloc(size + 1);
    fread(content, 1, size, file);
    content[size] = '\0';  // Null terminate for text files
    fclose(file);

    printf("File content (%ld bytes):\n%s\n", size, content);

    const char* content_type = get_content_type(filename);
    printf("Content-Type: %s\n", content_type);

    // Actually send the response based on content type
    if (strcmp(content_type, "text/html") == 0) {
        http_send_html(client, content);
    }
    else if (strcmp(content_type, "text/plain") == 0) {
        //http_send_text(client, content);
    }
    else if (strcmp(content_type, "application/json") == 0) {
        //http_send_json(client, content);
    }
    else {
        // For binary files (images, etc.) use the low-level function
        //http_send_response(client, content, content_type, size);
    }

    free(content);
}

// needed when servering static files 
const char* get_content_type(const char* filename) {
    // Get file extension
    const char* dot = strrchr(filename, '.');
    if (!dot) return "application/octet-stream";

    // Compare extensions
    if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
        return "text/html";
    if (strcmp(dot, ".txt") == 0)
        return "text/plain";
    if (strcmp(dot, ".css") == 0)
        return "text/css";
    if (strcmp(dot, ".js") == 0)
        return "application/javascript";
    if (strcmp(dot, ".json") == 0)
        return "application/json";
    if (strcmp(dot, ".png") == 0)
        return "image/png";
    if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
        return "image/jpeg";
    if (strcmp(dot, ".gif") == 0)
        return "image/gif";
    if (strcmp(dot, ".pdf") == 0)
        return "application/pdf";

    // Default for unknown types
    return "application/octet-stream";
}
