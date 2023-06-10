CC = gcc -g
CFLAGS = -I./include
LDFLAGS = -pthread
SOURCES = $(wildcard ./src/*.c)

EXECUTABLE = myshell

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f $(EXECUTABLE)

