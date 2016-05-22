/* t2.c - Test program 1

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
#include <fcntl.h>


#include <debug.h>
#include "t.h"

int  called;
void chld_handler (int signum)
{
  called=1;
}

int main (int argc, char **argv)
{
  int i, val, open_max;

  /* Unless otherwise specified by further rules this programs returns
     the value it receives as its first argument converted to an integer. If no
     arguments are given, the program returns the default value T_NOARG.*/

  if (argc > 1)
    val = atoi (argv[1]);
  else
    val = T_NOARG;


  /* Check if subprocess has more descriptors than expected. */

  if (val == T_CHECKFDS)
    {
#ifdef OPEN_MAX
      open_max = OPEN_MAX;
#else
      open_max = sysconf (_SC_OPEN_MAX);
#endif

      for (i=3; i < open_max; i++)
	{
	  if ( (fcntl(i, F_GETFD) != -1) || (errno != EBADF))
	    {
	      val=T_EXTRAFDS;
	      break;
	    }
	}
      
    }

  return val;

}

  /* Check if child handlers have not been blocked. This is unlikely to
     be useful since exec resets signal dispositions. */

  /* if (val == T_CHLDSIG) */
  /*   { */
  /*     /\* Set SIGCHLD handler. *\/ */
  /*     called=0; */
  /*     rs = sigaction (SIGCHLD, NULL, &act); */
  /*     sysfatal(rs<0); */
  /*     act.sa_handler = chld_handler; */
  /*     rs = sigaction (SIGCHLD, &act, NULL); */
  /*     sysfatal(rs<0); */

  /*     /\* Create grandson subprocess. *\/ */
  /*     pid = fork(); */
  /*     sysfatal (pid<0); */

  /*     /\* Grandson does nothing and exits. *\/ */
  /*     if (pid==0) */
  /* 	exit(T_NEVER); */

  /*     /\* This process waits for grandson. *\/ */
  /*     wait(&status);	 */
      
  /*     if (called) */
  /* 	{ */
  /* 	  fp = fopen (T_TEMPFILE, "w"); */
  /* 	  sysfatal (!fp); */
  /* 	  rs = fputs(T_READTHIS,fp); */
  /* 	  sysfatal (rs == EOF); */
  /* 	  fclose (fp); */
  /* 	} */
  /*   } */


  /* This is always true. */

  /* if (val == T_CHLDSIG) */
  /*   { */
  /*     /\* Check SIGCHLD handler. *\/       */
     
  /*     rs = sigaction (SIGCHLD, NULL, &act); */
  /*     sysfatal(rs<0); */
      
  /*     if (act.sa_handler == SIG_IGN) */
  /* 	{ */
  /* 	  fp = fopen (T_TEMPFILE, "w"); */
  /* 	  sysfatal (!fp); */
  /* 	  rs = fputs(T_READTHIS,fp); */
  /* 	  sysfatal (rs == EOF); */
  /* 	  fclose (fp); */
  /* 	} */
  /*   } */
