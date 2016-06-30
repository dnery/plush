/* test-001.c - Program to test runcmd function

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
#include <fcntl.h>

#include "runcmd.h"
#include "debug.h"
#include "t.h"
#include "testutils.h"

/* This program performs a series of tests and return the number of errors.*/

int main (int argc, char **argv)
{
        /* Test cases. */

        char cmd1[] = "./t1" ;                          /* Exits T_NOARG_EXIT */
        char cmd2[] = "./t1 " strfy(T_SOMEVALUE) ;      /* Exits T_SOMEVALUE */
        char cmd3[] = "./t1 " strfy(T_SEGFAULT) ;       /* Causes segfault */
        char cmd4[] = "./nosuchfile";                   /* Causes exec failure */
        char cmd5[] = "./t1 " strfy(EXECFAILSTATUS);    /* Exits EXECFAILSTATUS */

        int result, pid, rpid, nerrors;
        FILE *fp;

        test_set = "T1";

        nerrors = 0;

        /* Disable standard streams if not redirecting. */

        sysfatal ((noio[0] = open ("/dev/null", O_WRONLY)) <0);
        sysfatal ((noio[1] = open ("/dev/null", O_WRONLY)) <0);
        sysfatal ((noio[2] = open ("/dev/null", O_WRONLY)) <0);

        /* Check  */

        pid = try_runcmd (cmd3, &result, NULL);   /* Normal, success. */
        /* printf ("  Checking...\n"); */

        nerrors += check ("abnormal termination is correctly reported",
                        (!IS_NORMTERM(result)) &&
                        (EXITSTATUS(result) == 0));

        /* Check */

        pid = try_runcmd (cmd1, &result, NULL);   /* Normal, success. */
        /* printf ("  Checking...\n"); */

        fp = fopen ("t1.log", "r");
        sysfatal (!fp);
        fscanf (fp, "%d", &rpid);

        nerrors += check ("normal termination is correctly reported",
                        IS_NORMTERM(result));

        nerrors += check ("pid is correctly returned on success",
                        (IS_NORMTERM(result)) &&
                        (pid == rpid));

        nerrors += check ("exit status is correctly reported on sucess",
                        (IS_NORMTERM(result)) &&
                        (EXITSTATUS(result) == 10));

        nerrors += check ("exec success if correctly reported",
                        (IS_NORMTERM(result)) &&
                        (IS_EXECOK(result)));

        fclose (fp);
        unlink ("t1.log");

        /* Check  */

        pid = try_runcmd (cmd2, &result, NULL);   /* Normal, success. */
        /* printf ("  Checking...\n"); */

        nerrors += check ("command line arguments are correctly read", EXITSTATUS(result) == 11);

        /* Check */

        pid = try_runcmd (cmd4, &result, NULL);   /* Normal, success. */
        /* printf ("  Checking...\n"); */

        nerrors += check ("exec failure is correctly reported (a)",
                        (!IS_EXECOK(result)) &&
                        (EXITSTATUS(result) == EXECFAILSTATUS));

        /* Check */

        pid = try_runcmd (cmd5, &result, NULL);   /* Normal, success. */
        /* printf ("  Checking...\n"); */

        nerrors += check ("exec failure is not reported on sucess (b)",
                        (IS_EXECOK(result)) &&
                        (EXITSTATUS(result) == EXECFAILSTATUS));

        /* Check. */

        return nerrors;
}
