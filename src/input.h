#ifndef FARST_INPUT_H
#define FARST_INPUT_H

#include <stdio.h>

#define CMD_BG          "&"
#define CMD_PIPE        "|"
#define CMD_DELIM       " \n\t\r"
#define CMD_MAXARGV     256
#define CMD_MAXCHARS    2048

/* Simple comand type structure */
typedef struct command_t {

        int fg;
        int argc;
        char **argv;
        char *_line;

} command_t;

/*
 * Tokenize a string into a string array based on CMD_DELIM.
 */
size_t tokenize(const char *line, char **argv[]);

/*
 * Read a line (carefully) from input.
 */
char *read_line(FILE *stream);

/*
 * Read a fully parsed command.
 */
command_t *read_command();

/*
 * Delete allocated command struct.
 */
void delete_command(command_t *command);

#endif /* ifndef FARST_INPUT_H */
