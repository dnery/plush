# Farst
An attempt at a job control shell in C.

## Progress
Job control is fully implemented. Functionality is still limited to a simple
fork/exec loop, tho', since we don't have proper command parsing yet... But
we're getting there.

## Building
There's a simple but well written Makefile accompanying this, so the process is
as usual. The code is C89 compliant, so there shouldn't be any compatibility
problems. We'll be moving to `autotools` soon.

## References:
These will be with us 'til the day we die.
 - **Posix Programmer's ManPages**
 - [die.net Linux Documentation](http://linux.die.net/)
 - [GNU Manuals: Implemeenting a Job Control Shell](http://www.gnu.org/software/libc/manual/html_node/Implementing-a-Shell.html#Implementing-a-Shell)
