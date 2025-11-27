all:
	gcc -o ./bin/server.out src/main.c src/Server.c

run:
	./bin/server.out

clean:
	rm ./bin/server.out
