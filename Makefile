all:
	gcc -o ./bin/server.out src/main.c

run:
	./bin/server.out

clean:
	rm ./bin/server.out
