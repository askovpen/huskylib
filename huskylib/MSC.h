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

/* Microsoft C or Microsoft QuickC for MS-DOS or OS/2 */

#ifndef HUSKY_MSC_H
#define HUSKY_MSC_H

#  ifndef __LITTLE_ENDIAN__
#    define __LITTLE_ENDIAN__
#  endif

#  define _stdc cdecl
#  define _intr cdecl interrupt far
#  define _intcast void (_intr *)()
#  define _veccast _intcast

#  if _MSC_VER >= 600
#    define _fast _fastcall
#  else
#    define _fast pascal
#  endif

#  define farmalloc(n)    _fmalloc(n)
#  define farfree(p)      _ffree(p)
#  define farrealloc(p,n) _frealloc(p,n)

#  if _MSC_VER >= 600
#    define farcalloc(a,b) _fcalloc(a,b)
#  else
     void far *farcalloc(int n, int m);
#  endif

#  ifdef __OS2__
    /* just don't use 16 bit OS/2, we doubt that it still works */
#    define farread read
#    define farwrite write
#    define _XPENTRY pascal far
#    define mysleep(x) DosSleep(1000L*(x))
#    define sleep(x) DosSleep(1000L*(x))
#    define HAS_sleep     1
#  else
#    define _XPENTRY
#  endif

#  define HAS_MALLOC_H        /* use "#include <malloc.h>" for malloc() etc. */
#  define HAS_IO_H     1  /* may use "#include <io.h> */
#  define HAS_SHARE_H  1  /* may use "#include <share.h> */
#  define HAS_PROCESS_H   /* may use "#include <process.h> */

#  define USE_STAT_MACROS


typedef unsigned bit;

typedef unsigned char byte;
typedef signed char sbyte;

typedef unsigned short word;
typedef signed short sword;

typedef unsigned long dword;
typedef signed long sdword;

typedef signed short sshort;
typedef unsigned short ushort;

typedef signed long slong;
typedef unsigned long ulong;

#endif
