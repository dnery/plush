/* runcmd.c - Execute a command as a subprocess.

   Copyright (c) 2014, Francisco José Monaco <moanco@icmc.usp.br>

   This file is part of POSIXeg

   POSIXeg is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.


   Code written by Henrique F. M. Freitas (https://github.com/Henrique1792)
 */

#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

#include "runcmd.h"
#include "debug.h"

#define CHK_NULL(value) ((value) == NULL ? 1 : 0)
/*
 * Executes 'command' in a subprocess. Information on the subprocess execution
 * is stored in 'result' after its completion, and can be inspected with the
 * aid of macros made available for this purpose. Argument 'io' is a pointer
 * to an integer vector where the first, second and third positions store
 * file descriptors to where standard input, output and error, respective,
 * shall be redirected; if NULL, no redirection is performed. On
 * success, returns subprocess' pid; on error, returns 0.
 */

int runcmd (const char *command, int *result, const int *io){
        int pid, status;
        int aux, i, tmp_result;
        int exec_ok[2], bkp_fd[3];
        char *args[RCMD_MAXARGS], *p, *cmd;

        tmp_result = 0;

        /* Parse arguments to obtain an argv vector. */

        cmd = malloc ((strlen (command)+1) * sizeof(char));
        sysfail (!cmd, -1);
        p = strcpy (cmd, command);

        i=0;
        args[i++] = strtok (cmd, RCMD_DELIM);
        while ((i<RCMD_MAXARGS) && (args[i++] = strtok (NULL, RCMD_DELIM)));
        i--;

        /* Create a subprocess. */
        sysfail(pipe(exec_ok)<0,-1);
        pid = fork();
        sysfail (pid<0, -1);

        if (pid>0){ /* Caller process (parent). */
                close(exec_ok[1]);
                aux = wait (&status);

                sysfail (aux<0, -1);

                if(!(CHK_NULL(io))){
                        /*Recover values from bkp*/
                        dup2(bkp_fd[0],STDIN_FILENO);
                        dup2(bkp_fd[1],STDOUT_FILENO);
                        dup2(bkp_fd[2],STDERR_FILENO);
                        close(bkp_fd[0]);
                        close(bkp_fd[1]);
                        close(bkp_fd[2]);
                        close(io[0]);
                        close(io[1]);
                        close(io[2]);
                }

                /* Collect termination mode. */
                if (WIFEXITED(status)){
                        tmp_result |= NORMTERM;
                        tmp_result |= (WEXITSTATUS(status)&RETSTATUS);

                }
                if(read(exec_ok[0],NULL,1)==0)
                        tmp_result |=EXECOK;
        }
        else{ /*Child Procces.*/
                if(!(CHK_NULL(io))){
                        /*Old fd backup*/
                        dup2(STDIN_FILENO,bkp_fd[0]);
                        dup2(STDOUT_FILENO,bkp_fd[1]);
                        dup2(STDERR_FILENO,bkp_fd[2]);

                        /*Change default descriptors*/
                        dup2(io[0],STDIN_FILENO);
                        dup2(io[1],STDOUT_FILENO);
                        dup2(io[2],STDERR_FILENO);
                        close(io[0]);
                        close(io[1]);
                        close(io[2]);
                }
                aux = execvp (args[0], args);
                if(!(CHK_NULL(io))){
                        /*Recover values from bkp*/
                        dup2(bkp_fd[0],STDIN_FILENO);
                        dup2(bkp_fd[1],STDOUT_FILENO);
                        dup2(bkp_fd[2],STDERR_FILENO);
                        close(bkp_fd[0]);
                        close(bkp_fd[1]);
                        close(bkp_fd[2]);
                }
                write(exec_ok[1],"False!",1);
                close(exec_ok[1]);
                close(exec_ok[0]);
                free (cmd);
                exit (EXECFAILSTATUS);
        }

        if (result)    *result = tmp_result;

        free (p);
        return pid; /* Only parent reaches this point. */
}

/*
 * If function runcmd is called in non-blocking mode, then function
 * pointed by rcmd_onexit is asynchronously evoked upon the subprocess
 * termination. If this variable points to NULL, no action is performed.
 */

void (*runcmd_onexit)(void) = NULL;
