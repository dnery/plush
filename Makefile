# Automated flags
CC = gcc
#CFLAGS = --ansi -D_POSIX_C_SOURCE=20011L -Werror --pedantic-errors -O0 -g
CFLAGS = --ansi -D_POSIX_C_SOURCE=20011L -Werror --pedantic-errors -O0 -g -ftrapv -fsanitize=address

# Local variables
SRCDIR = ./src/
BINDIR = ./bin/
OBJ = jobs.o input.o shell.o main.o

# Path-resolved objects
RESOBJ = $(addprefix $(BINDIR), $(OBJ))

all: $(BINDIR) $(RESOBJ)
	$(CC) $(CFLAGS) -o plush $(RESOBJ)

#1. Build path is constructed
$(BINDIR):
	mkdir -p $(BINDIR)

#2. Objects are put into build path
$(RESOBJ): $(BINDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: run clean

run:
	./plush

clean:
	rm -f plush $(RESOBJ)
