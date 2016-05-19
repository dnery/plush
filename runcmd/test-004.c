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
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <signal.h>

#include <runcmd.h>
#include <debug.h>
#include <signal.h>

#include "t.h"
#include "testutils.h"

void dummy (void)
{
  /* printf ("---> %d %d\n", IS_NORMTERM(runcmd_result), EXITSTATUS(runcmd_result)); */
}


/* This program performs a series of tests and return the number of errors.*/

int main (int argc, char **argv)
{
  int nerrors, result, i; 
  struct sigaction act;
  int messedsigs;

  char cmd1[] = "./t1 &";

  test_set = "T4";
  nerrors = 0;

  /* Check if lib hasn't messed with caller's signal configs. */

  runcmd_onexit = dummy;

  try_runcmd (cmd1, &result, NULL);
  sleep (1);

  messedsigs = 0;
  for (i=1; i<32; i++)
    {
      sigaction (i, NULL, &act);
      if (act.sa_handler != SIG_DFL)
  	messedsigs += 1;
    }
  
  nerrors += check ("lib didn't mess with caller's signal handlers",
  		    messedsigs == 0);
  
  return nerrors;

}

