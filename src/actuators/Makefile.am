MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/extras \
-I$(top_builddir)/src/extras \
-I$(top_srcdir)/src/actuators \
-I$(top_builddir)/src/actuators \
-DPACKAGE_BIN_DIR=\"$(bindir)\" \
-DPACKAGE_LIB_DIR=\"$(libdir)\" \
-DPACKAGE_DATA_DIR=\"$(pkgdatadir)\" \
-DLOG_ENABLE=0

includes_HEADERS = actuators.h
includesdir = $(includedir)/Software

lib_LTLIBRARIES = libactuators.la
libactuators_la_SOURCES = \
actuators.h \
arm.h \
actuators.c \
arm.c

libactuators_la_LIBADD = ../extras/libextras.la
#libactuators_la_LDFLAGS = -no-undefined @lt_enable_auto_import@ -version-info @version_info@ @release_info@
