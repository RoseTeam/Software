MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/extras/roxml \
-I$(top_builddir)/src/extras/roxml \
-DPACKAGE_BIN_DIR=\"$(bindir)\" \
-DPACKAGE_LIB_DIR=\"$(libdir)\" \
-DPACKAGE_DATA_DIR=\"$(pkgdatadir)\"

includes_HEADERS = roxml.h
includesdir = $(includedir)/Software

noinst_LTLIBRARIES = libroxml.la
libroxml_la_SOURCES = \
roxml-defines.h \
roxml-doxy.h \
roxml.h \
roxml-internal.h \
roxml-parse-engine.h \
roxml-types.h \
roxml_win32_native.h \
roxml.c \
roxml-internal.c \
roxml-parse-engine.c

#libroxml_la_LDFLAGS = -no-undefined @lt_enable_auto_import@ -version-info @version_info@ @release_info@
