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

#ifndef __HUSKYLIB_H__
#define __HUSKYLIB_H__

#include <stdio.h>
#include <time.h>

#include "compiler.h"
#include "cvtdate.h"

#ifdef __DOS__
/* flushasm.asm for DOS, redefined for known implementations in flush.c */
void pascal far flush_handle2(int fd);
#endif

void _fast flush_handle(FILE * fp);
HUSKYEXT int _fast fexist(const char *filename);
HUSKYEXT long _fast fsize(const char *filename);
HUSKYEXT int _fast direxist(const char *directory);
void _fast tdelay(int);
HUSKYEXT int _fast setfsize(int fd, long size);
#ifdef __LITTLE_ENDIAN__

#define put_dword(ptr, val)	(*(dword *)(ptr) = (val))
#define put_word(ptr, val)	(*(word *)(ptr) = (val))
#define get_dword(ptr)		(*(dword *)(ptr))
#define get_word(ptr)		(*(word *)(ptr))

#else

HUSKYEXT void put_word(byte *ptr, word value);      /* memory.c */
HUSKYEXT void put_dword(byte *ptr, dword value);    /* memory.c */
/*
 *  get_dword
 *
 *  Reads in a 4 byte word that is stored in little endian (Intel) notation
 *  and converts it to the local representation n an architecture-
 *  independent manner
 */

#define get_dword(ptr)            \
   ((dword)((unsigned char)(ptr)[0]) |           \
    (((dword)((unsigned char)(ptr)[1])) << 8)  | \
    (((dword)((unsigned char)(ptr)[2])) << 16) | \
    (((dword)((unsigned char)(ptr)[3])) << 24))  \

/*
 *  get_word
 *
 *  Reads in a 2 byte word that is stored in little endian (Intel) notation
 *  and converts it to the local representation in an architecture-
 *  independent manner
 */

#define get_word(ptr)         \
    ((word)((unsigned char)(ptr)[0]) |         \
     (((word)((unsigned char)(ptr)[1])) << 8 ))

#endif /* __LITTLE_ENDIAN__ */

HUSKYEXT int  _createDirectoryTree(const char *pathName);
/*DOC
  Input:  a pointer to a \0 terminated string
  Output: 0 if successfull, 1 else
  FZ:     pathName is a correct directory name
          createDirectoryTree creates the directory and all parental directories
          if they do not exist.

  was taken from hpt\fcommon
*/

#endif