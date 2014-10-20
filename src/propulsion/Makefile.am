MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/extras \
-I$(top_builddir)/src/extras \
-I$(top_srcdir)/src/propulsion \
-I$(top_builddir)/src/propulsion \
-DPACKAGE_BIN_DIR=\"$(bindir)\" \
-DPACKAGE_LIB_DIR=\"$(libdir)\" \
-DPACKAGE_DATA_DIR=\"$(pkgdatadir)\"

includes_HEADERS = propulsion.h
includesdir = $(includedir)/Software

lib_LTLIBRARIES = libpropulsion.la
libpropulsion_la_SOURCES = \
propulsion.h \
move.h \
propulsion.c \
move.c

libpropulsion_la_LIBADD = ../extras/libextras.la
#libpropulsion_la_LDFLAGS = -no-undefined @lt_enable_auto_import@ -version-info @version_info@ @release_info@
