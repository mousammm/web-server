# NAME

Something like nginix but from scratch, in C!

## TODOs:

[ ] find a name 

[ ] make a simple http sever that server html,etc;

[ ] Listens on specified port (default: 9090)

[ ] Serves files from specified directory

[ ] Root response 

    [ ] 1 Simple Welcome

    [ ] 2 Simple Directory Listing

    [ ] "" > cmd line arg for root html file

[ ] HTTP Features > handle text

    [ ] GET / HTTP/1.1          → serve index.html or welcome

    [ ] GET /style.css HTTP/1.1 → serve CSS file

    [ ] GET /image.jpg HTTP/1.1 → serve binary file

    [ ] HTTP Methods:

    ✅ GET (primary focus)

    ❌ POST, PUT, DELETE (v2+)

[ ] Response Codes:

    [ ] 200 OK

    [ ] 404 Not Found

    [ ] 500 Internal Server Error

[ ] Basic logging (printf to console)

[ ] SIGINT handling for clean shutdown

[ ] MIME type detection
