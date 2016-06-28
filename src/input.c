#include "input.h"
#include "debug.h"

#include <stdlib.h>
#include <string.h>

size_t tokenize(const char *line, char **argv[])
{
        char *str;      /* Token string pointer */
        size_t i = 0;   /* Argument list iterator */

        str = (char *)line;

        /* Parse according to CMD_DELIM */
        (*argv)[i++] = strtok(str, CMD_DELIM);
        while ((i < CMD_MAXARGV) && ((*argv)[i++] = strtok(NULL, CMD_DELIM)));

        /* Return string copy */
        return --i;
}

char *read_line(FILE *stream)
{
        char *str = NULL;               /* String containing obtained line */
        char *aux = NULL;               /* Auxiliary pointer (for realloc) */
        size_t def = CMD_MAXCHARS;      /* Default line length */
        size_t len = 0;                 /* Current line length */
        int ch;                         /* fgetc return container */

        /* Default allocate nominal size */
        str = malloc(sizeof(*str) * def);

        while ((ch = fgetc(stream)) != EOF && ch != '\n') {
                str[len] = (char)ch;
                len++;

                if (len == def) {
                        aux = realloc(str, sizeof(*str) * (def += CMD_MAXCHARS));
                        check(aux != NULL);
                        str = aux;
                }
        }

        /* Append null character */
        str[len] = '\0';
        len++;

        /* Return safe area */
        return realloc(str, sizeof(*str) * len);
}

command_t *read_command()
{
        char *line;             /* Input line */
        char **argv;            /* Argument vector */
        size_t argc;            /* Argument vector size */
        size_t i = 0;           /* Argument vector iterator */
        command_t *command;     /* Allocated command */

prompt:
        fprintf(stdout, ">>> ");

        /* Read input, silly */
        if (strcmp((line = read_line(stdin)), "") == 0) {
                free(line);
                goto prompt;
        }

        /* Secure memory region */
        argv = malloc(sizeof(*argv) * CMD_MAXARGV);

        /* Get everything else */
        argc = tokenize(line, &argv);
        command = malloc(sizeof(*command));

        command->fg = 1;
        command->argc = argc;
        command->argv = argv;
        command->_line = line;

        /* Silly commands are silly */
        if (strcmp(argv[0], "exit") == 0) {
                delete_command(command);
                return NULL;
        }

        /* TODO FG/BG */

        return command;
}

void delete_command(command_t *command)
{
        free(command->_line);
        free(command->argv);
        free(command);
}
