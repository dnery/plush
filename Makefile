# Automated flags
CC = gcc
CFLAGS = --ansi -D_POSIX_C_SOURCE=20011L -Werror --pedantic-errors -O0 -g

# Local variables
SRCDIR = ./src/
BINDIR = ./build/
OBJ = jobs.o shell.o main.o

# Path-resolved objects
RESOBJ = $(addprefix $(BINDIR), $(OBJ))

all: $(RESOBJ)
	$(CC) $(CFLAGS) -o farst $(RESOBJ)

$(RESOBJ): $(BINDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: run clean

run:
	./farst

clean:
	rm -f farst $(RESOBJ)
