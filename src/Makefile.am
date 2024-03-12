LDADD = -lhelper -lpopt -lm
AM_LDFLAGS = -L../lib
AM_CPPFLAGS = -I../lib -I../../../lib

bin_PROGRAMS = split
split_SOURCES = types.h project.h version.h gettext.h main.c
split_CPPFLAGS = -pipe -std=c89 -ansi -DAUTOMAKE=1 -DCONST=const -DLOCALEDIR=\"$(localedir)\" -I../lib -I../../../lib
