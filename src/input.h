#ifndef FARST_INPUT_H
#define FARST_INPUT_H

#include "shell.h"

#include <stdio.h>

#define CMD_BG          "&"
#define CMD_PIPE        "|"
#define CMD_DELIM       " \n\t\r\a"
#define CMD_MAXARGV     256
#define CMD_MAXCHARS    512

/*
 * Returns true if line is empty.
 */
int is_null(const char *line);

/*
 * Reads a line from input stream.
 */
char *read_line(FILE *stream);

/*
 * Parse am input line, build pipeline and launch jobs.
 */
void try_pipeline(const char *line);

#endif /* ifndef FARST_INPUT_H */
