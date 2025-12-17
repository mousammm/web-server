CC = gcc

BINDIR = build
SRCDIR = src
BINARY = server

INCS = -I./src/includes
LIBS = 

CFLAGS = -Wall -Wextra -g $(INCS)
LDLIBS = $(LIBS)

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(BINDIR)/%.o, $(SRCS))

all: $(BINDIR)/$(BINARY)

$(BINDIR):
	mkdir -p $(BINDIR)

# gcc flags -o server main.o lal.o -libs
$(BINDIR)/$(BINARY): $(OBJS)
	@echo "Compiling $< -> $@"
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDLIBS)

# gcc flags -c main.c -o main.o
$(BINDIR)/%.o: $(SRCDIR)/%.c | $(BINDIR)
	@echo "Compiling $< -> $@"
	$(CC) $(CFLAGS) -c $< -o $@

run: $(BINDIR)/$(BINARY)
	./$(BINDIR)/$(BINARY)

clean:
	rm -rf build/*

.PHONY: all run clean
