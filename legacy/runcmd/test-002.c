/* test-002.c - Program to test runcmd function

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
#include <pthread.h>
#include <sys/stat.h>
#include <signal.h>

#include <runcmd.h>
#include <debug.h>

#include "t.h"
#include "testutils.h"

int pipefd[2];
int onexit_called = 0;

/* This function is executed in a new thread in order to write to
   subprocess standard input. */

void * writepipe (void *arg)
{
  int rs=0;
  do 
    rs += write (pipefd[1], "abc", T_TOKENSIZE);
  while ( (rs>0) && (rs<T_TOKENSIZE) );

  sysfatal (rs<0);
  return NULL;
}

/* Used as SIGCHLD handler. */

void onexit (void)
{
  onexit_called = 1;
}

/* Used as SIGALRM handler. */

int expired = 0;
void giveup (int signum)
{
  expired = 1;
}

/* This program performs a series of tests and return the number of errors.*/

int main (int argc, char **argv)
{
  int nbytes;
  
  /* Test cases. */

  char cmd1[] = "./t1 " strfy(T_MAKEIO);  /* Exits EXECFAILSTATUS */
  char cmd2[] = "./t1 &";
  char cmd3[] = "./t1 " strfy(T_WRITEFIFO) " &"; /* Write to fifo. */

  int result, pid, nerrors, rs, redirok, io[3], i, fd, wasnonblock;
  pthread_t thread;
  char buffer[4];
  struct sigaction act;

  test_set = "T2";
  nerrors = 0;

  /* Disable standard streams if not redirecting. */

  sysfatal ((noio[0] = open ("/dev/null", O_WRONLY)) <0);
  sysfatal ((noio[1] = open ("/dev/null", O_WRONLY)) <0);
  sysfatal ((noio[2] = open ("/dev/null", O_WRONLY)) <0);

  /* Check  redirection

     Redirect subprocess' stdandard input and output to a pipe.
     Write a predefined watchword and wait for a specific countersign.
     If subprocess does not read from the pipe or does not write back,
     assume test failure. */

  /* Create the pipe. */

  rs = pipe (pipefd);
  sysfatal (rs<0);

  /* Use a new thread to write to the pipe so that caller never blocks.
     This is probably unecessary but we do it anyway. */

  pthread_create (&thread, NULL, writepipe, NULL);

  io[0] = pipefd[0];		/* Redirect standard input.  */
  io[1] = pipefd[1];		/* Redirect standard output. */
  io[2] = 2;	                /* Dont touch standard error.*/

  /* Call runcmd. */

  pid = try_runcmd (cmd1, &result, io);   

  /* printf ("  Checking...\n"); */

  /* Read subprocess reply. If subprocess is not reading from the pipe
     (redirected standard input) it will block and timeout without having
     received the watchword, thereby sending an incorrect ack. A correct
     countersign is sent, otherwise. */

  buffer[0]='\0';		
  nbytes = read (pipefd[0], buffer, T_TOKENSIZE);
  sysfatal (nbytes<0);
  buffer[3]=0;

  /* Check countersign.*/

  if (!strcmp(buffer,T_READTHIS)) /* Subprocess answered correctly. */
    redirok=1;
  else				   /* Subprocess must have timed-out. */
    redirok=0;

  nerrors += check ("whether stdin/stdout are redirected correctly", redirok);

  /* Check IS_NONBLOCK and runcmd_onexit.

     Set up a SIGCHLD handler andm ake a nonblocking call. Check IS_NONBLOCK.
     Check if the handler has been called. 
 */

  runcmd_onexit = onexit;
  pid = try_runcmd (cmd2, &result, NULL);   

  /* printf ("Checking...\n"); */

  nerrors += check ("whether nonblock is correctly reported", 
		    IS_NONBLOCK(result));

  for (i=-0; (i<3) && (!onexit_called); i++) /* Wait a resonable time. */
    sleep(1);

  nerrors += check ("whether runcmd_onexit is called when requested", 
		    onexit_called);
 


  /* Check parallel execution. 
     
     Try to communicate with subprocess through a named pipe (FIFO).
     Send a watchword and checks the countersign. If client is not running in
     parallel, caller will block. Use timeouts to detect this case. */

  /* Create a fifo. */

  unlink (T_FIFONAME);
  rs = mkfifo (T_FIFONAME, 0600);
  sysfatal (rs<0);

  buffer[0]='\0';		
  /* io[0]=0; io[1]=1; io[2]=2; */

  /* Set a time out alarm. */

  rs = sigaction (SIGALRM, NULL, &act);
  sysfatal (rs<0);
  act.sa_handler = giveup;
  rs = sigaction (SIGALRM, &act, NULL);
  sysfatal (rs<0);

  /* Make a nonblocking call. Probe probram will try to open the
     fifo to read from it. If subprocess is not running in parallel,
     it will block since caller itself has not opened the fifo as yet,
     caise the caller to block in a deadlock. Only time out releases
     caller. */

  expired = 0;
  alarm (TIMEOUT);
  pid = try_runcmd (cmd3, &result, io);
  alarm (0);

  /* Now caller opens the fifo and reads from it. Had a faulty (nonparallel)
     subprocess been stuck, it would not proceed and write into the fifo. */

  fd = open (T_FIFONAME, O_RDONLY);
  sysfatal (fd<0);

  /* Caller may read from fifo now. */

  nbytes = read (fd, buffer, T_TOKENSIZE);
  sysfatal (nbytes<0);
  buffer[nbytes]='\0';

  /* Check the countersigh. */

  wasnonblock = 0;
  if (!strcmp(buffer, T_READTHIS))
    wasnonblock=1;
  
  /* Result is ok if countersigh is correct and caller has not timedout. */

  /* printf ("Checking...\n"); */

  nerrors += check ("nonblock mode execs in parallel", 
  		    (!expired) && (wasnonblock));
  /* Cleanup. */

  unlink (T_FIFONAME);

  pid = pid;			/* Avoid gcc complaints. */


  return nerrors;

}
