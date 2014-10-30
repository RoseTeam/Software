MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/extras \
-I$(top_builddir)/src/extras \
-I$(top_srcdir)/src/actuators \
-I$(top_builddir)/src/actuators \
-I$(top_srcdir)/src/propulsion \
-I$(top_builddir)/src/propulsion \
-I$(top_srcdir)/src/strategy \
-I$(top_builddir)/src/strategy \
-DPACKAGE_BIN_DIR=\"$(bindir)\" \
-DPACKAGE_LIB_DIR=\"$(libdir)\" \
-DPACKAGE_DATA_DIR=\"$(pkgdatadir)\" \
-DLOG_ENABLE=0

includes_HEADERS = strategy.h
includesdir = $(includedir)/Software

lib_LTLIBRARIES = libstrategy.la
libstrategy_la_SOURCES = \
strategy.h \
avoid.h \
strategy.c \
avoid.c

libpropulsion_la_LIBADD = ../extras/libextras.la ../actuators/libactuators.la ../propulsion/libpropulsion.la
#libstrategy_la_LDFLAGS = -no-undefined @lt_enable_auto_import@ -version-info @version_info@ @release_info@
