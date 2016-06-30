# Plush
An attempt at a simple POSIX.1-2001 compliant job control shell in C. Cute name
because cute names are always welcome.

## Progress
 - [x] Fork/exec loop
 - [ ] IO redirection (w/ pipes)
 - [ ] Non-blocking execution (&)
 - [ ] Built-in functionality (cd, output eval, etc)

## Building
There's a simple but well written Makefile accompanying this, so the process is
as usual. The code is C89 compliant, so there shouldn't be any compatibility
problems. I'll consider moving to autotools soon.
