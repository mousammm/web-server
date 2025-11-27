CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = webserver
SOURCES = test.c Server.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run
