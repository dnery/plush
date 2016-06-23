#ifndef FARST_SHELL_H
#define FARST_SHELL_H

#include "jobs.h"

/*
 * Global shell attributes
 */
pid_t shell_pgid;               /* Shell process group ID */
int shell_terminal;             /* Shell fileNo descriptor */
int shell_interactive;          /* True if shell is interactive */
struct termios shell_modes;     /* Save shell terminal modes (what is this?) */

/*
 * Will spawn shell interactively or not at all
 */
void sh_init();

/*
 * Will launch a job in foreground or background
 */
void sh_launchj(job_t *job, int foreground);

/*
 * Will launch a process in foreground or background
 *
 * TODO Redirection/descriptor duplication
 *
 *     -> http://bit.ly/28OupvE
 */
void sh_launchp(process_t *p, pid_t pgid, int in, int out, int err,
                int foreground);

#endif /* ifndef FARST_SHELL_H */
