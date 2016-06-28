#include "shell.h"
#include "input.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int main(int argc, char *argv[])
{
        char *input = NULL;
        process_t *p;
        job_t *j;

        sh_init();

        while (1) {
                fprintf(stdout, ">>> ");

                if (!is_null(input = read_line(stdin))) {

                        if (strcmp(input, "exit") == 0)
                                break;

                        try_pipeline(input);
                        free(input);
                }
        }

        for (j = first_job; j != NULL; j = j->next) {

                for (p = j->first_process; p != NULL; p = p->next)
                        kill(p->pid, SIGHUP);

                kill(-j->pgid, SIGHUP);
        }

        free(input);
        return 0;
}
