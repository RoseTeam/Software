SUBDIRS = extras actuators propulsion strategy

MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-DPACKAGE_BIN_DIR=\"$(bindir)\" \
-DPACKAGE_LIB_DIR=\"$(libdir)\" \
-DPACKAGE_DATA_DIR=\"$(datadir)\" \
-I$(top_srcdir)/src/extras \
-I$(top_srcdir)/src/actuators \
-I$(top_srcdir)/src/propulsion \
-I$(top_srcdir)/src/strategy \
-I$(top_builddir)/src/extras \
-I$(top_builddir)/src/actuators \
-I$(top_builddir)/src/propulsion \
-I$(top_builddir)/src/strategy 


bin_PROGRAMS = robot

robot_SOURCES = main.c

robot_LDADD = \
$(top_builddir)/src/extras/libextras.la \
$(top_builddir)/src/actuators/libactuators.la \
$(top_builddir)/src/propulsion/libpropulsion.la \
$(top_builddir)/src/strategy/libstrategy.la
