/* test-003.c - Program to test runcmd function

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
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>

#include "runcmd.h"
#include "debug.h"
#include "t.h"
#include "testutils.h"

/* This program performs a series of tests and return the number of errors.*/

int main (int argc, char **argv)
{
        int nerrors, result, result2, i;
        /* char buffer[T_TOKENSIZE]; */
        /* sigset_t sigset; */
        struct sigaction act;
        int io[3];

        char cmd1[] = "./t2 " strfy (T_CHECKFDS);
        char cmd2[] = "./nosuchfile";
        /* char cmd3[] = "./t1 &"; */


        io[0]=0; io[1]=1; io[2]=2;

        test_set = "T3";
        nerrors = 0;

        /* Check for messy descriptors. */

        try_runcmd (cmd1, &result, io);

        /* printf ("Checking...\n"); */

        nerrors += check ("lib didn't mess with child's descriptors",
                        EXITSTATUS(result) == T_CHECKFDS);


        /* Check if everything works if all signals are default. */

        sysfatal (!memset (&act, 0, sizeof (struct sigaction)));
        act.sa_handler = SIG_DFL;
        for (i=1; i<32; i++)
        {
                sigaction (i, &act, NULL);
        }

        try_runcmd (cmd2, &result2, io);

        /* printf ("Checking...\n"); */

        nerrors += check ("lib does not depend on caller's signal handlers",
                        !IS_EXECOK(result2));

        return nerrors;
}


/* Check if signal is restored. */

/* unlink (T_TEMPFILE); */

/* runcmd_onexit = go_on; */

/* try_runcmd (cmd1, &result, io); */

/* while(!freetogo) */
/*   sleep(1); */

/* fp = fopen (T_TEMPFILE, "r"); */
/* buffer[0] = '\0'; */
/* if (fp) */
/*   fgets (buffer, T_TOKENSIZE+1, fp); */

/* nerrors += check ("subprocess handlers are not messed", */
/*                  !strcmp (buffer, T_READTHIS)); */
