#ifndef SHALALA_JOBS_H
#define SHALALA_JOBS_H

#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

/*
 * A process is a node in a pipeline
 */
typedef struct process_t {

        pid_t pid;                      /* Process ID */

        int status;                     /* Reported status */
        int suspended;                  /* True if suspended */
        int completed;                  /* True if completed */
        char **argv;                    /* Argument vector (exec) */

        struct process_t *next;         /* Next process in pipeline */

} process_t;

/*
 * A job manages this process pipeline
 */
typedef struct job_t {

        pid_t pgid;                     /* Process group ID */
        char *command;                  /* Command line string */
        process_t *first_proc;          /* First process in pipeline */

        int notified;                   /* True if notified about suspended job */
        int stdin, stdout, stderr;      /* Standard input/output/error channels */
        struct termios modes;           /* Saved terminal modes (what is this?) */

        struct job_t *next;             /* Next process array in pipeline */

} job_t;

/*
 * This holds the current head in the job array
 */
job_t *first_job = NULL;

/*
 * Find job element with certain pgid element
 */
static inline job_t *find(pid_t pgid)
{
        job_t *j = first_job;

        for (; j != NULL; j = j->next)
                if (j->pgid == pgid)
                        return j;

        return NULL;
}

/*
 * Returns true if all processes in a job are suspended
 */
static inline int is_suspended(job_t *job)
{
        process_t *p = job->first_proc;

        for (; p != NULL; p = p->next)
                if (!p->suspended)
                        return 0;

        return 1;
}

/*
 * Returns true if all processes in a job are completed
 */
static inline int is_completed(job_t *job)
{
        process_t *p = job->first_proc;

        for (; p != NULL; p = p->next)
                if (!p->completed)
                        return 0;

        return 1;
}

#endif /* ifndef SHALALA_JOBS_H */
