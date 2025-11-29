# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = bin/server

# Directories
SRC_DIR = src
HTTP_DIR = $(SRC_DIR)/http
BIN_DIR = bin

# Source files - main.c is in src/, others are in src/http/
SRCS = $(SRC_DIR)/main.c $(HTTP_DIR)/Server.c $(HTTP_DIR)/Http.c $(HTTP_DIR)/Client.c $(HTTP_DIR)/Router.c

# Object files (in bin directory)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)

# Header files
HEADERS = $(HTTP_DIR)/Server.h

# Create bin directory structure if it doesn't exist
$(shell mkdir -p $(BIN_DIR))

# Default target
all: $(TARGET)

# Create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile .c files to .o files in bin directory
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(BIN_DIR)

# Rebuild everything
rebuild: clean all

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run
