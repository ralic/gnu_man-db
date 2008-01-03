# DO NOT EDIT! GENERATED AUTOMATICALLY!
# Copyright (C) 2004-2007 Free Software Foundation, Inc.
#
# This file is free software, distributed under the terms of the GNU
# General Public License.  As a special exception to the GNU General
# Public License, this file may be distributed as part of a program
# that contains a configuration script generated by Autoconf, under
# the same distribution terms as the rest of that program.
#
# Generated by gnulib-tool.
#
# This file represents the compiled summary of the specification in
# gnulib-cache.m4. It lists the computed macro invocations that need
# to be invoked from configure.ac.
# In projects using CVS, this file can be treated like other built files.


# This macro should be invoked from ./configure.ac, in the section
# "Checks for programs", right after AC_PROG_CC, and certainly before
# any checks for libraries, header files, types and library functions.
AC_DEFUN([gl_EARLY],
[
  m4_pattern_forbid([^gl_[A-Z]])dnl the gnulib macro namespace
  m4_pattern_allow([^gl_ES$])dnl a valid locale name
  m4_pattern_allow([^gl_LIBOBJS$])dnl a variable
  m4_pattern_allow([^gl_LTLIBOBJS$])dnl a variable
  AC_REQUIRE([AC_PROG_RANLIB])
  AC_REQUIRE([AC_GNU_SOURCE])
  AC_REQUIRE([gl_USE_SYSTEM_EXTENSIONS])
  AC_REQUIRE([gl_LOCK_EARLY])
  dnl Some compilers (e.g., AIX 5.3 cc) need to be in c99 mode
  dnl for the builtin va_copy to work.  With Autoconf 2.60 or later,
  dnl AC_PROG_CC_STDC arranges for this.  With older Autoconf AC_PROG_CC_STDC
  dnl shouldn't hurt, though installers are on their own to set c99 mode.
  AC_REQUIRE([AC_PROG_CC_STDC])
])

# This macro should be invoked from ./configure.ac, in the section
# "Check for header files, types and library functions".
AC_DEFUN([gl_INIT],
[
  m4_pushdef([AC_LIBOBJ], m4_defn([gl_LIBOBJ]))
  m4_pushdef([AC_REPLACE_FUNCS], m4_defn([gl_REPLACE_FUNCS]))
  m4_pushdef([AC_LIBSOURCES], m4_defn([gl_LIBSOURCES]))
  AM_CONDITIONAL([GL_COND_LIBTOOL], [false])
  gl_cond_libtool=false
  gl_libdeps=
  gl_ltlibdeps=
  gl_source_base='gnulib/lib'
  gl_FUNC_ALLOCA
  gl_ARGP
  gl_FUNC_ATEXIT
  AC_FUNC_CANONICALIZE_FILE_NAME
  gl_MODULE_INDICATOR([canonicalize])
  gl_CANONICALIZE_LGPL
  gl_FUNC_CHDIR_LONG
  gl_FUNC_CHOWN
  gl_UNISTD_MODULE_INDICATOR([chown])
  gl_CHECK_TYPE_STRUCT_DIRENT_D_INO
  gl_CHECK_TYPE_STRUCT_DIRENT_D_TYPE
  gl_FUNC_DIRFD
  gl_DIRNAME
  gl_DOUBLE_SLASH_ROOT
  gl_FUNC_DUP2
  gl_UNISTD_MODULE_INDICATOR([dup2])
  gl_ERROR
  m4_ifdef([AM_XGETTEXT_OPTION],
    [AM_XGETTEXT_OPTION([--flag=error:3:c-format])
     AM_XGETTEXT_OPTION([--flag=error_at_line:5:c-format])])
  gl_EXITFAIL
  gl_FUNC_FCHDIR
  gl_UNISTD_MODULE_INDICATOR([fchdir])
  gl_FCNTL_H
  gl_FCNTL_SAFER
  gl_MODULE_INDICATOR([fcntl-safer])
  gl_FILE_NAME_CONCAT
  gl_FLOAT_H
  # No macro. You should also use one of fnmatch-posix or fnmatch-gnu.
  gl_FUNC_FNMATCH_GNU
  gl_FUNC_GETCWD
  gl_UNISTD_MODULE_INDICATOR([getcwd])
  gl_GETLOGIN_R
  gl_UNISTD_MODULE_INDICATOR([getlogin_r])
  gl_GETOPT
  AC_SUBST([LIBINTL])
  AC_SUBST([LTLIBINTL])
  gl_FUNC_GETTIMEOFDAY
  gl_GLOB
  gl_HASH
  gl_INLINE
  gl_FUNC_LCHOWN
  gl_UNISTD_MODULE_INDICATOR([lchown])
  gl_IGNORE_UNUSED_LIBRARIES
  gl_LOCALCHARSET
  gl_LOCK
  gl_FUNC_LSTAT
  AC_FUNC_MALLOC
  AC_DEFINE([GNULIB_MALLOC_GNU], 1, [Define to indicate the 'malloc' module.])
  gl_FUNC_MALLOC_POSIX
  gl_STDLIB_MODULE_INDICATOR([malloc-posix])
  gl_MALLOCA
  gl_FUNC_MEMCMP
  gl_FUNC_MEMPCPY
  gl_STRING_MODULE_INDICATOR([mempcpy])
  gl_FUNC_MEMRCHR
  gl_STRING_MODULE_INDICATOR([memrchr])
  gl_MINMAX
  gt_FUNC_MKDTEMP
  gl_STDLIB_MODULE_INDICATOR([mkdtemp])
  gl_FUNC_MKSTEMP
  gl_STDLIB_MODULE_INDICATOR([mkstemp])
  gl_FUNC_OPENAT
  gl_PATHMAX
  gl_FUNC_READLINK
  gl_UNISTD_MODULE_INDICATOR([readlink])
  gl_REGEX
  gl_FUNC_RENAME
  gl_SAME
  gl_SAVE_CWD
  gl_FUNC_SETENV
  gl_FUNC_UNSETENV
  gl_SIZE_MAX
  gl_FUNC_SLEEP
  gl_UNISTD_MODULE_INDICATOR([sleep])
  gt_TYPE_SSIZE_T
  gl_STDARG_H
  AM_STDBOOL_H
  gl_STDINT_H
  gl_STDIO_H
  gl_STDLIB_H
  gl_STRCASE
  gl_FUNC_STRCHRNUL
  gl_STRING_MODULE_INDICATOR([strchrnul])
  gl_FUNC_STRCSPN
  gl_FUNC_STRDUP
  gl_STRING_MODULE_INDICATOR([strdup])
  gl_FUNC_STRERROR
  gl_STRING_MODULE_INDICATOR([strerror])
  gl_HEADER_STRING_H
  gl_HEADER_STRINGS_H
  gl_FUNC_STRNDUP
  gl_STRING_MODULE_INDICATOR([strndup])
  gl_FUNC_STRNLEN
  gl_STRING_MODULE_INDICATOR([strnlen])
  gl_FUNC_STRPBRK
  gl_STRING_MODULE_INDICATOR([strpbrk])
  gl_FUNC_STRSEP
  gl_STRING_MODULE_INDICATOR([strsep])
  gl_HEADER_SYS_SELECT
  AC_PROG_MKDIR_P
  gl_HEADER_SYS_SOCKET
  AC_PROG_MKDIR_P
  gl_HEADER_SYS_STAT_H
  AC_PROG_MKDIR_P
  gl_HEADER_SYS_TIME_H
  AC_PROG_MKDIR_P
  gl_SYSEXITS
  gl_FUNC_GEN_TEMPNAME
  gl_UNISTD_H
  gl_UNISTD_SAFER
  gl_FUNC_VASNPRINTF
  gl_FUNC_VASPRINTF
  gl_STDIO_MODULE_INDICATOR([vasprintf])
  m4_ifdef([AM_XGETTEXT_OPTION],
    [AM_XGETTEXT_OPTION([--flag=asprintf:2:c-format])
     AM_XGETTEXT_OPTION([--flag=vasprintf:2:c-format])])
  gl_FUNC_VSNPRINTF
  gl_STDIO_MODULE_INDICATOR([vsnprintf])
  gl_WCHAR_H
  gl_WCTYPE_H
  gl_XALLOC
  gl_XGETCWD
  gl_XSIZE
  gl_XSTRNDUP
  gl_XVASPRINTF
  m4_ifdef([AM_XGETTEXT_OPTION],
    [AM_XGETTEXT_OPTION([--flag=xasprintf:1:c-format])])
  LIBGNU_LIBDEPS="$gl_libdeps"
  AC_SUBST([LIBGNU_LIBDEPS])
  LIBGNU_LTLIBDEPS="$gl_ltlibdeps"
  AC_SUBST([LIBGNU_LTLIBDEPS])
  m4_popdef([AC_LIBSOURCES])
  m4_popdef([AC_REPLACE_FUNCS])
  m4_popdef([AC_LIBOBJ])
  AC_CONFIG_COMMANDS_PRE([
    gl_libobjs=
    gl_ltlibobjs=
    if test -n "$gl_LIBOBJS"; then
      # Remove the extension.
      sed_drop_objext='s/\.o$//;s/\.obj$//'
      for i in `for i in $gl_LIBOBJS; do echo "$i"; done | sed "$sed_drop_objext" | sort | uniq`; do
        gl_libobjs="$gl_libobjs $i.$ac_objext"
        gl_ltlibobjs="$gl_ltlibobjs $i.lo"
      done
    fi
    AC_SUBST([gl_LIBOBJS], [$gl_libobjs])
    AC_SUBST([gl_LTLIBOBJS], [$gl_ltlibobjs])
  ])
])

# Like AC_LIBOBJ, except that the module name goes
# into gl_LIBOBJS instead of into LIBOBJS.
AC_DEFUN([gl_LIBOBJ], [
  AS_LITERAL_IF([$1], [gl_LIBSOURCES([$1.c])])dnl
  gl_LIBOBJS="$gl_LIBOBJS $1.$ac_objext"
])

# Like AC_REPLACE_FUNCS, except that the module name goes
# into gl_LIBOBJS instead of into LIBOBJS.
AC_DEFUN([gl_REPLACE_FUNCS], [
  m4_foreach_w([gl_NAME], [$1], [AC_LIBSOURCES(gl_NAME[.c])])dnl
  AC_CHECK_FUNCS([$1], , [gl_LIBOBJ($ac_func)])
])

# Like AC_LIBSOURCES, except the directory where the source file is
# expected is derived from the gnulib-tool parametrization,
# and alloca is special cased (for the alloca-opt module).
# We could also entirely rely on EXTRA_lib..._SOURCES.
AC_DEFUN([gl_LIBSOURCES], [
  m4_foreach([_gl_NAME], [$1], [
    m4_if(_gl_NAME, [alloca.c], [], [
      m4_syscmd([test -r gnulib/lib/]_gl_NAME[ || test ! -d gnulib/lib])dnl
      m4_if(m4_sysval, [0], [],
        [AC_FATAL([missing gnulib/lib/]_gl_NAME)])
    ])
  ])
])

# This macro records the list of files which have been installed by
# gnulib-tool and may be removed by future gnulib-tool invocations.
AC_DEFUN([gl_FILE_LIST], [
  build-aux/config.rpath
  build-aux/link-warning.h
  lib/alloca.c
  lib/alloca.in.h
  lib/areadlink-with-size.c
  lib/areadlink.h
  lib/argp-ba.c
  lib/argp-eexst.c
  lib/argp-fmtstream.c
  lib/argp-fmtstream.h
  lib/argp-fs-xinl.c
  lib/argp-help.c
  lib/argp-namefrob.h
  lib/argp-parse.c
  lib/argp-pin.c
  lib/argp-pv.c
  lib/argp-pvh.c
  lib/argp-xinl.c
  lib/argp.h
  lib/asnprintf.c
  lib/asprintf.c
  lib/at-func.c
  lib/atexit.c
  lib/basename.c
  lib/canonicalize-lgpl.c
  lib/canonicalize.c
  lib/canonicalize.h
  lib/chdir-long.c
  lib/chdir-long.h
  lib/chown.c
  lib/config.charset
  lib/creat-safer.c
  lib/dirent.in.h
  lib/dirfd.c
  lib/dirfd.h
  lib/dirname.c
  lib/dirname.h
  lib/dup-safer.c
  lib/dup2.c
  lib/error.c
  lib/error.h
  lib/exitfail.c
  lib/exitfail.h
  lib/fchdir.c
  lib/fchmodat.c
  lib/fchown-stub.c
  lib/fchownat.c
  lib/fcntl--.h
  lib/fcntl-safer.h
  lib/fcntl.in.h
  lib/fd-safer.c
  lib/file-set.c
  lib/file-set.h
  lib/filenamecat.c
  lib/filenamecat.h
  lib/float+.h
  lib/float.in.h
  lib/fnmatch.c
  lib/fnmatch.in.h
  lib/fnmatch_loop.c
  lib/fstatat.c
  lib/getcwd.c
  lib/getlogin_r.c
  lib/getopt.c
  lib/getopt.in.h
  lib/getopt1.c
  lib/getopt_int.h
  lib/gettext.h
  lib/gettimeofday.c
  lib/glob-libc.h
  lib/glob.c
  lib/glob.in.h
  lib/hash-pjw.c
  lib/hash-pjw.h
  lib/hash-triple.c
  lib/hash-triple.h
  lib/hash.c
  lib/hash.h
  lib/intprops.h
  lib/lchown.c
  lib/localcharset.c
  lib/localcharset.h
  lib/lock.c
  lib/lock.h
  lib/lstat.c
  lib/lstat.h
  lib/malloc.c
  lib/malloca.c
  lib/malloca.h
  lib/malloca.valgrind
  lib/memcmp.c
  lib/mempcpy.c
  lib/memrchr.c
  lib/minmax.h
  lib/mkdirat.c
  lib/mkdtemp.c
  lib/mkstemp.c
  lib/open-safer.c
  lib/openat-die.c
  lib/openat-priv.h
  lib/openat-proc.c
  lib/openat.c
  lib/openat.h
  lib/pathmax.h
  lib/pipe-safer.c
  lib/printf-args.c
  lib/printf-args.h
  lib/printf-parse.c
  lib/printf-parse.h
  lib/readlink.c
  lib/ref-add.sin
  lib/ref-del.sin
  lib/regcomp.c
  lib/regex.c
  lib/regex.h
  lib/regex_internal.c
  lib/regex_internal.h
  lib/regexec.c
  lib/rename.c
  lib/same-inode.h
  lib/same.c
  lib/same.h
  lib/save-cwd.c
  lib/save-cwd.h
  lib/setenv.c
  lib/setenv.h
  lib/size_max.h
  lib/sleep.c
  lib/stdbool.in.h
  lib/stdint.in.h
  lib/stdio.in.h
  lib/stdlib.in.h
  lib/strcasecmp.c
  lib/strchrnul.c
  lib/strcspn.c
  lib/strdup.c
  lib/strerror.c
  lib/string.in.h
  lib/strings.in.h
  lib/stripslash.c
  lib/strncasecmp.c
  lib/strndup.c
  lib/strnlen.c
  lib/strpbrk.c
  lib/strsep.c
  lib/sys_select.in.h
  lib/sys_socket.in.h
  lib/sys_stat.in.h
  lib/sys_time.in.h
  lib/sysexits.in.h
  lib/tempname.c
  lib/tempname.h
  lib/unistd--.h
  lib/unistd-safer.h
  lib/unistd.in.h
  lib/unsetenv.c
  lib/vasnprintf.c
  lib/vasnprintf.h
  lib/vasprintf.c
  lib/vsnprintf.c
  lib/wchar.in.h
  lib/wctype.in.h
  lib/xalloc-die.c
  lib/xalloc.h
  lib/xasprintf.c
  lib/xgetcwd.c
  lib/xgetcwd.h
  lib/xmalloc.c
  lib/xsize.h
  lib/xstrndup.c
  lib/xstrndup.h
  lib/xvasprintf.c
  lib/xvasprintf.h
  m4/absolute-header.m4
  m4/alloca.m4
  m4/argp.m4
  m4/atexit.m4
  m4/canonicalize-lgpl.m4
  m4/canonicalize.m4
  m4/chdir-long.m4
  m4/chown.m4
  m4/codeset.m4
  m4/d-ino.m4
  m4/d-type.m4
  m4/dirfd.m4
  m4/dirname.m4
  m4/dos.m4
  m4/double-slash-root.m4
  m4/dup2.m4
  m4/eealloc.m4
  m4/eoverflow.m4
  m4/error.m4
  m4/exitfail.m4
  m4/extensions.m4
  m4/fchdir.m4
  m4/fcntl-safer.m4
  m4/fcntl_h.m4
  m4/filenamecat.m4
  m4/float_h.m4
  m4/fnmatch.m4
  m4/getcwd-abort-bug.m4
  m4/getcwd-path-max.m4
  m4/getcwd.m4
  m4/getlogin_r.m4
  m4/getopt.m4
  m4/gettimeofday.m4
  m4/glibc21.m4
  m4/glob.m4
  m4/gnulib-common.m4
  m4/hash.m4
  m4/include_next.m4
  m4/inline.m4
  m4/intmax_t.m4
  m4/inttypes_h.m4
  m4/lchown.m4
  m4/lib-ignore.m4
  m4/lib-ld.m4
  m4/lib-link.m4
  m4/lib-prefix.m4
  m4/localcharset.m4
  m4/lock.m4
  m4/longlong.m4
  m4/lstat.m4
  m4/malloc.m4
  m4/malloca.m4
  m4/mbstate_t.m4
  m4/memcmp.m4
  m4/mempcpy.m4
  m4/memrchr.m4
  m4/minmax.m4
  m4/mkdtemp.m4
  m4/mkstemp.m4
  m4/onceonly_2_57.m4
  m4/openat.m4
  m4/pathmax.m4
  m4/readlink.m4
  m4/regex.m4
  m4/rename.m4
  m4/same.m4
  m4/save-cwd.m4
  m4/setenv.m4
  m4/size_max.m4
  m4/sleep.m4
  m4/sockpfaf.m4
  m4/ssize_t.m4
  m4/stdarg.m4
  m4/stdbool.m4
  m4/stdint.m4
  m4/stdint_h.m4
  m4/stdio_h.m4
  m4/stdlib_h.m4
  m4/strcase.m4
  m4/strchrnul.m4
  m4/strcspn.m4
  m4/strdup.m4
  m4/strerror.m4
  m4/string_h.m4
  m4/strings_h.m4
  m4/strndup.m4
  m4/strnlen.m4
  m4/strpbrk.m4
  m4/strsep.m4
  m4/sys_select_h.m4
  m4/sys_socket_h.m4
  m4/sys_stat_h.m4
  m4/sys_time_h.m4
  m4/sysexits.m4
  m4/tempname.m4
  m4/unistd-safer.m4
  m4/unistd_h.m4
  m4/vasnprintf.m4
  m4/vasprintf.m4
  m4/vsnprintf.m4
  m4/wchar.m4
  m4/wchar_t.m4
  m4/wctype.m4
  m4/wint_t.m4
  m4/xalloc.m4
  m4/xgetcwd.m4
  m4/xsize.m4
  m4/xstrndup.m4
  m4/xvasprintf.m4
])
