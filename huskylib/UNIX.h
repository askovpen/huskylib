/* $Id$
 *
 * HUSKYLIB: common defines, types and functions for HUSKY
 *
 * This is part of The HUSKY Fidonet Software project:
 * see http://husky.sourceforge.net for details
 *
 *
 * HUSKYLIB is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * HUSKYLIB is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; see file COPYING. If not, write to the
 * Free Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * See also http://www.gnu.org, license may be found here.
 */

/* Declarations for UNIX clones: Linux, FreeBSD, SUNOS (Solaris), MacOS etc. */

#ifndef HUSKY_UNIX_H
#define HUSKY_UNIX_H

#  define HUSKYEXT extern
#  define _stdc
#  define _intr
#  define _intcast
#  define _veccast
#  define _fast
#  define _loadds

#  define cdecl
#  define pascal
#  define near
#  undef  far
#  define far

#  define farread read
#  define farwrite write

#  if (defined(__APPLE__) && defined(__MACH__)) || defined(__NetBSD__) || defined(__FreeBSD__) || defined(_AIX) || defined(__SUN__) || defined(__LINUX__) || defined(__osf__) || defined(__hpux) || defined(__OpenBSD__) || defined(__CYGWIN__)
#    define mymkdir(a) mkdir((a), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)
#  else
#    define mymkdir(a) __mkdir((a), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)
#  endif

#  ifndef __SUN__
#    define tell(a) lseek((a),0,SEEK_CUR)
#  endif

#  ifndef stricmp
#    define stricmp strcasecmp
#  endif
#  ifndef strnicmp
#    define strnicmp strncasecmp
#  endif

#  if !defined(USG)
#  define HAS_SYS_PARAM_H
#  endif

#  if (defined(BSD) && (BSD >= 199103))
    /* now we can be sure we are on BSD 4.4 */
#  define HAS_SYS_MOUNT_H
#  endif
    /* we are not on any BSD-like OS */
    /* list other UNIX os'es without getfree mechanism here */
#  if defined( __svr4__ ) || defined( __SVR4 ) || defined (__linux__) && defined (__GLIBC__)
#  define HAS_SYS_STATVFS_H
#  endif

#  if defined (__LINUX__) && !defined(__GLIBC__)
#  define HAS_SYS_VFS_H
#  endif

#  include <fcntl.h>
#  ifndef O_BINARY
#   define O_BINARY 0 /* O_BINARY flag has no effect under UNIX */
#  endif

#  ifndef O_TEXT
#   define O_TEXT   0 /* O_TEXT flag has no effect under UNIX */
#  endif

#  ifndef SH_DENYNONE
#    define SH_DENYNONE 0
#  endif
#  ifndef SH_DENYNO
#    define SH_DENYNO 0
#  endif
#  ifndef SH_DENYALL
#    define SH_DENYALL 1
#  endif

#  define _XPENTRY

/* Other OS's may sleep with other functions */

#  if defined(__SUN__)
#    define mysleep(x) usleep(x*1000000l)
#    define sleep(x)   usleep(x*1000000l)
#    define HAS_sleep     1
#  elif defined(__BSD__) || defined(__CYGWIN__) || defined(__LINUX__) || defined(__APPLE__)
#    define mysleep(x) sleep(x)
#    define HAS_sleep     1
#  endif
#  ifndef __SUN__ /* SunOs 2.7 not have snprintf() and vsnprintf in libc */
                  /* If you known test for this - please report to developers */
#    define HAS_snprintf  1
#    define HAS_vsnprintf 1
#  endif

#  define HAS_SYSEXITS_H       1  /*  <sysexits.h> */
#  define HAS_UNISTD_H         1  /* <unistd.h> */
#  define HAS_PWD_H            1  /* <pwd.h> */
#  define HAS_GRP_H            1  /* may be used "#include <grp.h>" */
#  define HAS_SIGNAL_H         1  /* <signal.h> */
#  define HAS_SYS_WAIT_H       1  /* <sys/wait.h> */
#  define USE_STAT_MACROS

#  if defined(__LINUX__) || defined(__BSD__) || defined(__CYGWIN__) || defined(__APPLE__)
#    define HAS_mktime	/* <time.h> */
#    define HAS_strftime	/* <time.h> */
#    define HAS_DIRENT_H  /* <dirent.h> */
#  endif

#  if defined(__CYGWIN__)
#    define HAS_strupr	/* <string.h> from libc (newlib) */
#  endif

#  define HAS_popen_close  /* popen(); pclose() */


typedef unsigned bit;

typedef unsigned char byte;
typedef signed char sbyte;

typedef unsigned short word;
typedef signed short sword;

#ifdef __ALPHA__    /* add other 64 bit systems here */
typedef unsigned int dword;
typedef signed   int sdword;
#else             /* 32 and 16 bit machines */
typedef unsigned long dword;
typedef signed long sdword;
#endif

typedef signed short sshort;
/*typedef unsigned short ushort;*/

typedef signed long slong;
/*typedef unsigned long ulong;*/

#endif
