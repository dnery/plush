#include <stdio.h>
#include <stdlib.h>
#include "shell.h"

int main(int args, char **argv){
    int exit = 0;

    startRoutines(int args, char **argv);
    while(exit == 0){
        exit = shellLoop();
    }
    exitRoutines();
    return exit;
}
