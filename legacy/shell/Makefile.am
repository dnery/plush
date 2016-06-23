
## Reminder for versioning
##
## Versioning: CURRENT:REVISION:AGE
## CURRENT The latest interface implemented.
## REVISION The implementation number of CURRENT (read: number of bugs fixed...)
## AGE The number of interfaces implemented, minus one.
## The library supports all interfaces between CURRENT − AGE and CURRENT.
##
## If you have 
## not changed the interface (bug fixes)         CURRENT : REVISION+1 : AGE
## augmented the interface (new functions)       CURRENT+1 : 0 : AGE+1
## broken old interface (e.g. removed functions) CURRENT+1 : 0 : 0



MYLIB_VERSIONING = 0:0:0 

lib_LTLIBRARIES = libshell.la

libshell_la_SOURCES = utils.c parser.c shell.c
libshell_la_LDFLAGS = -version-info $(MYLIB_VERSIONING)

include_HEADERS = utils.h parser.h shell.h


bin_PROGRAMS = Shue
Shue_SOURCES = main.c
Shue_LDADD = libshell.la

