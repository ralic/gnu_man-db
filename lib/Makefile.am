## Process this file with automake to produce Makefile.in
##
## Copyright (C) 1994, 1995 Graeme Wilford.
## Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008 Colin Watson.
##
## This file is part of man-db.
##
## man-db is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## man-db is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with man-db; if not, write to the Free Software Foundation,
## Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

pkglib_LTLIBRARIES = libman.la
dist_noinst_DATA = README

AM_CFLAGS = $(WARN_CFLAGS)

libman_la_CPPFLAGS = \
	-I$(top_builddir)/include \
	-I$(top_srcdir)/gnulib/lib \
	-I$(top_builddir)/gnulib/lib \
	-DLOCALEDIR=\"$(localedir)\" \
	$(libpipeline_CFLAGS)

libman_la_SOURCES = \
	appendstr.c \
	cleanup.c \
	cleanup.h \
	debug.c \
	decompress.c \
	decompress.h \
	encodings.c \
	encodings.h \
	hashtable.c \
	hashtable.h \
	linelength.c \
	linelength.h \
	lower.c \
	lower.h \
	orderfiles.c \
	orderfiles.h \
	pathsearch.c \
	pathsearch.h \
	security.c \
	security.h \
	tempfile.c \
	util.c \
	wordfnmatch.c \
	wordfnmatch.h \
	xchown.c \
	xchown.h \
	xregcomp.c \
	xregcomp.h

libman_la_LIBADD = ../gnulib/lib/libgnu.la $(LTLIBOBJS) \
	$(LIBCOMPRESS) @LTLIBINTL@

libman_la_LDFLAGS = \
	-avoid-version -release $(VERSION) -rpath $(pkglibdir) \
	-no-undefined \
	$(libpipeline_LIBS)
