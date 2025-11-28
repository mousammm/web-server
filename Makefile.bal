all:
	gcc -o ./bin/server.out src/main.c src/Server.c src/Http.c

run:
	./bin/server.out

clean:
	rm ./bin/server.out


