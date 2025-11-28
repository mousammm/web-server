# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = bin/server

# Directories
SRC_DIR = src
BIN_DIR = bin

# Source files
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/Server.c $(SRC_DIR)/Http.c $(SRC_DIR)/Client.c

# Object files (in bin directory, keep same structure)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)

# Header files
HEADERS = $(SRC_DIR)/Server.h

# Create bin directory structure if it doesn't exist
$(shell mkdir -p $(BIN_DIR))

# Default target
all: $(TARGET)

# Create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile .c files to .o files in bin directory
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)  # Create subdirectories in bin if needed
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
