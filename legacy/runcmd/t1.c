/* t1.c - Test program 1

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
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include <debug.h>
#include "t.h"

char buffer[T_TOKENSIZE];


void giveup(int signun)
{
  fputs (buffer,stdout);
}

int main (int argc, char **argv)
{
  FILE *fp;
  int pid, val, rs, fd;
  struct sigaction act;
  void *rp;

  /* Unless otherwise specified by further rules this programs returns
     the value it receives as its first argument converted to an integer. If no
     arguments are given, the program returns the default value T_NOARG.*/

  if (argc > 1)
    val = atoi (argv[1]);
  else
    {
      val = T_NOARG;
      
      /* Save pid into a file. */
  
      pid = getpid();
      fp = fopen ("t1.log", "w");
      sysfatal (!fp);
      fprintf (fp, "%d\n", pid);
      fclose(fp);
    }      

  /* Cause a segmentation fault.*/

  if (val == T_SEGFAULT)
    * ((int *) 0x0)  = 0;
  
  /* Read from stdin, write to stdout. */

  if (val == T_MAKEIO)
    {

      /* In order not to block the caller, give up if nothing is
	 read from standard input. */

      rp = memset(&act, 0, sizeof(struct sigaction));
      sysfatal (!rp);
      act.sa_handler = giveup;		   
      rs = sigaction(SIGALRM, &act, NULL); 
      sysfatal (rs<0);

      alarm (TIMEOUT);
      fgets (buffer, T_TOKENSIZE+1, stdin);
      alarm (0);

      /* If a coorect watchword was received, send the countersing. If
	 this point has been reach due to a timeout, tough, send a bad ack.*/

      if (!strcmp(buffer,T_WRITETHIS))
	{
	  fputs (T_READTHIS,stdout);
	  val = T_IO;
	}
      else
	  fputs (buffer,stdout);
	
    }

  /* Read from and write to a fifo. */

  if (val == T_WRITEFIFO)
    {
      fd = open(T_FIFONAME, O_WRONLY);
      sysfatal (fd<0);
      write (fd, T_READTHIS, T_TOKENSIZE);
      close(fd);
    }

  return val;

}
