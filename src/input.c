#include "input.h"
#include "debug.h"

#include <stdlib.h>
#include <string.h>

int is_null(const char *line)
{
        char *i = (char *)line;
        for (; *i != '\0'; i++)
                if (*i != ' ')
                        return 0;

        return 1;
}

char *read_line(FILE *stream)
{
        char *str = NULL;               /* String containing obtained line */
        char *aux = NULL;               /* Auxiliary pointer (for realloc) */
        size_t def = CMD_MAXCHARS;      /* Default line length */
        size_t len = 0;                 /* Current line length */
        int ch;                         /* fgetc return container */

        /* Default allocate nominal size */
        str = malloc(sizeof(*str) * def);

        while ((ch = fgetc(stream)) != EOF && ch != '\n') {
                str[len] = (char)ch;
                len++;

                if (len == def) {
                        aux = realloc(str, sizeof(*str) * (def += CMD_MAXCHARS));
                        check(aux != NULL);
                        str = aux;
                }
        }

        /* Append null character */
        str[len] = '\0';
        len++;

        /* Return safe area */
        return realloc(str, sizeof(*str) * len);
}

void try_pipeline(const char *line)
{
        job_t *job;             /* Job iterator */
        char *ctok, *ccon;      /* Command token, command context */

        /* Copy string to safe memory area */
        char *str = malloc(strlen(line) + 1);
        strcpy(str, line);

        /**/
        ctok = strtok_r(str, CMD_BG, &ccon);
        while (ctok != NULL) {

                char *ptok, *pcon; /* Process token, process context*/

                job = create_job(shell_pgid);
                job->command = malloc(strlen(ctok) + 1);
                strcpy(job->command, ctok);

                /**/
                ptok = strtok_r(ctok, CMD_PIPE, &pcon);
                while (ptok != NULL) {

                        char *argv[CMD_MAXARGV], *acon; /* Argument vector, argument context */

                        /**/
                        int i = 0;
                        argv[i++] = strtok_r(ptok, CMD_DELIM, &acon);
                        while ((i < CMD_MAXARGV) && (argv[i++] = strtok_r(NULL, CMD_DELIM, &acon)));
                        create_process(job, argv);

                        ptok = strtok_r(NULL, CMD_PIPE, &pcon);
                }
                ctok = strtok_r(NULL, CMD_BG, &ccon);
        }

        /* Launch all created jobs */
        job = first_job;
        for (; job != NULL; job = job->next)
                if (job->next != NULL)
                        sh_launch_job(job, 0);
                else
                        sh_launch_job(job, 1);
        job_do_notify();

        /* Free safe memory area */
        free(str);
}
