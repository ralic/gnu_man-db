/* Copyright (C) 1992 Free Software Foundation, Inc.
This file is part of the GNU C Library.

The GNU C Library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The GNU C Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with the GNU C Library; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.  */

/* This file implements the `flock' function in terms of the POSIX.1 `fcntl'
   locking mechanism.  In 4BSD, these are two incompatible locking mechanisms,
   perhaps with different semantics?  */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "flock.h"
#include <errno.h>

#if HAVE_UNISTD_H
#  include <unistd.h>
#endif
#if HAVE_FCNTL_H
#  include <fcntl.h>
#endif
#if HAVE_SYS_FILE_H
#  include <sys/file.h>
#endif

/* Apply or remove an advisory lock, according to OPERATION,
   on the file FD refers to.  */
int
flock (int fd, int operation)
{
  struct flock lbuf;

  switch (operation & ~LOCK_NB)
    {
    case LOCK_SH:
      lbuf.l_type = F_RDLCK;
      break;
    case LOCK_EX:
      lbuf.l_type = F_WRLCK;
      break;
    case LOCK_UN:
      lbuf.l_type = F_UNLCK;
      break;
    default:
      errno = EINVAL;
      return -1;
    }

  lbuf.l_whence = SEEK_SET;
  lbuf.l_start = lbuf.l_len = 0L; /* Lock the whole file.  */

  return fcntl (fd, (operation & LOCK_NB) ? F_SETLK : F_SETLKW, &lbuf);
}
