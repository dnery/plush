#ifndef SHALALA_SHELL_H
#define SHALALA_SHELL_H

#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

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

#endif /* ifndef SHALALA_SHELL_H */
