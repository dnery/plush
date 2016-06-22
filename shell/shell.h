#ifndef SHELL_H
#define SHELL_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int startRoutines(int args, char **argv);
int shellLoop();
int exitRoutines();

#endif
