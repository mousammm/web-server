# C Web Server

A lightweight, single-threaded web server written in pure C that serves static files over HTTP.

![Demo](asset/demo.mp4)

## Features

- **HTTP/1.1 GET request handling** - Parses method, path, and protocol version
- **Static file serving** - Delivers HTML, CSS, JS, images, and other files
- **MIME type detection** - Automatically sets correct Content-Type headers
- **Client logging** - Logs client IP addresses and ports for each connection
- **Clean architecture** - Separated parsing, response handling, and utilities


## How to Use

### Compilation
```bash
make
```

### Running the Server
```bash
make run
```

## ⚠️ Current Limitations

- Single-threaded (handles one request at a time)
- Only supports GET method
- No directory listing
- No security features (basic implementation)
- No persistent connections (HTTP/1.0 style)

---
