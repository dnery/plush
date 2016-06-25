#ifndef FARST_SHELL_H
#define FARST_SHELL_H

#include "jobs.h"

/* == GLOBALS =============================================================== */

pid_t shell_pgid;               /* Shell process group ID */
int shell_terminal;             /* Shell fileNo descriptor */
int shell_interactive;          /* True if shell is interactive */
struct termios shell_modes;     /* Save shell terminal modes (what is this?) */

/* == API FUNCTIONS ========================================================= */

/*
 * Will spawn shell interactively or not at all.
 */
void sh_init();

/*
 * Will launch a complete job pipeline.
 */
void sh_launch_job(job_t *job, int foreground);

/*
 * Will launch a single process.
 *
 * TODO Redirection/descriptor duplication
 *
 *     -> http://bit.ly/28OupvE
 */
void sh_launch_process(process_t *p, pid_t pgid, int in, int out, int err,
                int foreground);

/*
 * Continue a suspended job.
 */
void sh_continue(job_t *job, int foreground);

/*
 * Place job in foreground, giving it control of the terminal.
 */
void put_in_foreground(job_t *job, int cont);

/*
 * Place job in background, without blocking the shell.
 */
void put_in_background(job_t *job, int cont);

#endif /* ifndef FARST_SHELL_H */
