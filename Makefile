SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
EXE = a.out
CC = gcc
CFLAGS = -Wall -g -O0 -pedantic -Wextra -DLACH_DEBUG=1
LDLIBS =

.PHONY: build clean rebuild run

build: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f $(EXE) $(OBJS)

rebuild: clean build

run: $(EXE)
	./$(EXE)
