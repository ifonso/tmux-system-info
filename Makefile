CC = gcc
CFLAGS = -Wall -Werror -I src
SOURCES = main.c $(wildcard src/*.c)
OUTPUT = tmux_sysinfo

all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(OUTPUT)

clean:
	rm -f $(OUTPUT)