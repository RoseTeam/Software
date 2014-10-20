MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/extras \
-I$(top_builddir)/src/extras \
-DPACKAGE_BIN_DIR=\"$(bindir)\" \
-DPACKAGE_LIB_DIR=\"$(libdir)\" \
-DPACKAGE_DATA_DIR=\"$(pkgdatadir)\"

includes_HEADERS = log.h
includesdir = $(includedir)/Software

lib_LTLIBRARIES = libextras.la

libextras_la_SOURCES = \
action.h \
log.h \
timer.h \
action.c \
log.c \
timer.c

#libextras_la_LIBADD = @EXTRAS_LIBS@
libextras_la_LDFLAGS = -lrt
