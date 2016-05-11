CC = gcc
CPP_FLAGS = -I. -Wall -std=c89 -ansi -pedantic
OBJS = utils.o parser.o shell.o main.o

%.o: %.c
	$(CC) -c $<

all: $(OBJS)
	$(CC) $(CPP_FLAGS) $^ -o shue

test:
	./shue

.PHONY: clean

clean:
	rm -rf *.o shue
