CC = gcc
CPP_FLAGS = -I. -Wall -std=c89
OBJS = utils.o parser.o shell.o main.o

%.o: %.c
	$(CC) -c $<

all: $(OBJS)
	$(CC) $(CPP_FLAGS) $^ -o shue

run:
	./shue

.PHONY: clean

clean:
	rm -rf *.o shue
