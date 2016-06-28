#include "jobs.h"
#include "debug.h"

#include <sys/wait.h>

process_t *create_process(job_t *job, char **argv)
{
        process_t *p = NULL; /* Allocated process */
        process_t *k = NULL; /* Process iterator */

        /* Allocate p and zer0 it out */
        if ((p = malloc(sizeof(process_t))) == NULL)
                return NULL;

        p->suspended = 0;
        p->completed = 0;
        p->status = -1;
        p->argv = argv;
        p->next = NULL;
        p->pid = -1;

        /* insert p at the end of process queue */
        if (job->first_process == NULL) {
                job->first_process = p;
        } else {
                for (k = job->first_process; k->next != NULL; k = k->next);
                k->next = p;
        }

        /* return successfully */
        return p;
}

void delete_process(process_t *process)
{
        free(process);
}

job_t *create_job(pid_t pgid)
{
        job_t *j; /* Allocated job */
        job_t *k; /* Job iterator */

        /* Allocate j and zer0 it out */
        if((j = malloc(sizeof(job_t))) == NULL)
                return NULL;

        j->pgid = getpid();
        setpgid(j->pgid, j->pgid);

        j->stdin = STDIN_FILENO;
        j->stdout = STDOUT_FILENO;
        j->stderr = STDERR_FILENO;

        j->command = NULL;
        j->first_process = NULL;
        j->notified = 0;
        j->next = NULL;

        /* insert j at the end of the job queue */
        if (first_job == NULL) {
                first_job = j;
        } else {
                for (k = first_job; k->next != NULL; k = k->next);
                k->next = j;
        }

        /* return successfully */
        return j;
}

void delete_job(job_t *job)
{
        process_t *p = job->first_process;
        while (p != NULL) {
                process_t *n = p->next;
                delete_process(p);
                p = n;
        }
        free(job->command);
        free(job);
}

job_t *find_job(pid_t pgid)
{
        job_t *j = first_job;
        for (; j != NULL; j = j->next)
                if (j->pgid == pgid)
                        return j;

        return NULL;
}

int is_suspended(job_t *job)
{
        process_t *p = job->first_process;
        for (; p != NULL; p = p->next)
                if (!p->suspended && !p->completed)
                        return 0;

        return 1;
}

int is_completed(job_t *job)
{
        process_t *p = job->first_process;
        for (; p != NULL; p = p->next)
                if (!p->completed)
                        return 0;

        return 1;
}

int process_update_status(pid_t pid, int status)
{
        if (pid > 0) {

                /* For all existing jobs... */
                job_t *j = first_job;
                for (; j != NULL; j = j->next) {

                        /* For all existing processes... */
                        process_t *p = j->first_process;
                        for (; p != NULL; p = p->next) {

                                /* Once the pid is found, te status is set */
                                if (p->pid == pid) {
                                        p->status = status;

                                        /*
                                         * Process is either stopped or terminated.
                                         *
                                         * WIFSTOPPED
                                         *      -> non-zero if child process is stopped.
                                         *
                                         * WIFSIGNALED
                                         *      -> non-zero if child process terminated upon miss-handling a signal.
                                         *
                                         * WIFEXITED
                                         *      -> non-zero if child process exited normally (exit/_exit).
                                         */
                                        if (WIFSTOPPED(status)) {
                                                p->suspended = 1;
                                                fprintf(stderr, "Process %d stopped by signal %d.\n", (int)pid, WSTOPSIG(p->status));
                                        } else {
                                                p->completed = 1;

                                                if (WIFEXITED(status))
                                                        fprintf(stderr, "Process %d done with status %d.\n", (int)pid, WEXITSTATUS(p->status));

                                                if (WIFSIGNALED(status))
                                                        fprintf(stderr, "Process %d terminated by signal %d.\n", (int)pid, WTERMSIG(p->status));
                                        }

                                        return 0;
                                }
                        }
                }
                /* BAAAD */
                warn("Worst case return...\n");
                return -1;
        } else if (pid == 0 || errno == ECHILD) {
                /* BAAAD */
                warn("No child processes.\n");
                return -1;
        } else {
                /* BAAAD */
                warn("Weird waipid exit.\n");
                return -1;
        }
}

void job_update_status()
{
        int status;     /* Status retrieved from waitpid */
        pid_t pid;      /* PID retrieved from waitpid */

        do {
                pid = waitpid(-1, &status, WUNTRACED|WNOHANG);
        } while (!process_update_status(pid, status));
}

void job_wait_blocked(job_t *job)
{
        int status;     /* Status retrieved from waitpid */
        pid_t pid;      /* PID retrieved from waitpid */

        do {
                pid = waitpid(-1, &status, WUNTRACED);
        } while (!process_update_status(pid, status)
                        && !is_suspended(job)
                        && !is_completed(job));
}

void job_do_notify()
{
        job_t *j = first_job;   /* Job iterator */
        job_t *nextj = NULL;    /* Job iterator */
        job_t *lastj = NULL;    /* Job iterator */

        job_update_status();

        for (; j != NULL; j = nextj) {
                nextj = j->next;

                if (is_completed(j)) {
                        /* Upon completion, notify user & delete job */
                        format_job_info(j, "completed");

                        if (lastj)
                                lastj->next = nextj;
                        else
                                first_job = nextj;

                        delete_job(j);
                } else if (is_suspended(j) && !j->notified) {
                        /* Otherwise, notify user and mark jobs */
                        format_job_info(j, "suspended");
                        j->notified = 1;
                        lastj = j;
                } else {
                        /* Silently process running jobs */
                        lastj = j;
                }
        }
}
