/*
 * locking.c
 *
 * This file implements record locking for platforms where the lock(),
 * unlock() and/or sopen() functions are not available. It also
 * implements the waitlock() routine, which is the same as lock(), but
 * waits until the lock can be applied.
 *
 * EMX GCC on OS/2:
 *   lock, unlock: implemented as OS/2 API calls
 *   sopen: provided by the EMX
 *
 * EMX GCC on Windows 32 bit using RSXNT:
 *   lock, unlock: implemented as Win32 API calls
 *   sopen: provided by the RSX RTL
 *
 * UNIX:
 *   lock, unlock: implemented as calls to fcntl
 *   sopen: implemented as open with subsequent shared lock
 *
 * OTHER:
 *   lock, unlock and sopen provieded by the CRTL
 *   waitlock defined as a loop calling lock and then sleep
 *
 * Written by Tobias Ernst @ 2:2476/418, released to the public domain.
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "../huskylib/compiler.h"
/* !!! Don't include locking.h into this file !!! */

#ifdef HAS_DIRECT_H
#  include <direct.h>
#endif

#ifdef HAS_UNISTD_H
#  include <unistd.h>
#endif

#ifdef HAS_IO_H
#  include <io.h>
#endif

#ifdef HAS_DOS_H
#  include <dos.h>
#endif

#include "../huskylib/huskylib.h"

#ifdef __DJGPP__

#include <dpmi.h>

sword far pascal shareloaded(void)
{
    __dpmi_regs r;
    r.x.ax = 0x1000;
    __dpmi_int(0x2f, &r);
    return (r.h.al == 0xff);
}
#endif

/*#if defined (__WATCOMC__OS2__) || defined(__EMX__) || defined(__IBMC__OS2__)*/
#if defined (__OS2__)

#include <os2.h>

int lock(int handle, long ofs, long length)
{
    FILELOCK urange, lrange;
    APIRET apiret;

    lrange.lOffset = ofs;
    lrange.lRange = length;
    urange.lRange = urange.lOffset = 0;

    if ((apiret = DosSetFileLocks((HFILE)handle, &urange, &lrange, 0, 0)) != 0)
    {
        return -1;
    }

    return 0;
}

int unlock(int handle, long ofs, long length)
{
    FILELOCK urange, lrange;
    APIRET apiret;

    urange.lOffset = ofs;
    urange.lRange = length;
    lrange.lRange = lrange.lOffset = 0;

    if ((apiret = DosSetFileLocks((HFILE)handle, &urange, &lrange, 0, 0)) != 0)
    {
        return -1;
    }
    return 0;
}

int waitlock(int handle, long ofs, long length)
{
    FILELOCK urange, lrange;
    APIRET apiret;

    lrange.lOffset = ofs;
    lrange.lRange = length;
    urange.lRange = urange.lOffset = 0;

    while ((apiret = DosSetFileLocks((HFILE)handle, &urange, &lrange, 60000, 0)) != 0);

    return 0;
}

int waitlock2(int handle, long ofs, long length, long t)
{
    FILELOCK urange, lrange;

    lrange.lOffset = ofs;
    lrange.lRange = length;
    urange.lRange = urange.lOffset = 0;

    return DosSetFileLocks((HFILE)handle, &urange, &lrange, t*1000l, 0);
}


#elif defined(__RSXNT__)

#include <windows.h>
#include <emx/syscalls.h>

#ifndef F_GETOSFD
#define F_GETOSFD 6
#endif

int waitlock(int handle, long ofs, long length)
{
    int nt_handle = __fcntl(handle, F_GETOSFD, 0);

    if (nt_handle < 0)
    {
        return -1;
    }
    while (LockFile(nt_handle, (DWORD)ofs, 0L, (DWORD)length, 0L) == FALSE)
    {
        sleep(1);
    }

    return 0;
}

/*
 *  THERE SHOULD BE A BETTER WAY TO MAKE A TIMED LOCK.
 */

int waitlock2(int handle, long ofs, long length, long t)
{
    int forever = 0;
    int rc;

    if (t==0)
      forever = 1;

    t *= 10;
    while ((rc = lock(handle, ofs, length)) == -1 && (t > 0 || forever))
    {
        tdelay(100);
        t--;
    }

    return rc;
}

int lock(int handle, long ofs, long length)
{
    int nt_handle = __fcntl(handle, F_GETOSFD, 0);

    if (nt_handle < 0 ||
        LockFile((DWORD)nt_handle, (DWORD)ofs, 0L, (DWORD)length, 0L) == FALSE)
    {
        return -1;
    }
    return 0;
}

int unlock(int handle, long ofs, long length)
{
    int nt_handle = __fcntl(handle, F_GETOSFD, 0);

    if (nt_handle < 0 ||
        UnlockFile((DWORD)nt_handle, (DWORD)ofs, 0L, (DWORD)length,
                   0L) == FALSE)
    {
        return -1;
    }
    return 0;
}

#elif defined(__MINGW32__) || defined(__MSVC__)

int waitlock(int handle, long ofs, long length)
{
    long offset = tell(handle);

    if (offset == -1)
        return -1;

    lseek(handle, ofs, SEEK_SET);
    _locking(handle, 1, length);
    lseek(handle, offset, SEEK_SET);

    return 0;
}

/*
 * THERE SHOULD BE A BETTER WAY TO MAKE A TIMED LOCK !!!!
 */
int waitlock2(int handle, long ofs, long length, long t)
{
    int forever = 0;
    int rc;

    if (t==0)
      forever = 1;

    t *= 10;
    while ((rc = lock(handle, ofs, length)) == -1 && (t > 0 || forever))
    {
        tdelay(100);
        t--;
    }

    return rc;
}


int lock(int handle, long ofs, long length)
{
    long offset = tell(handle);
    int r;

    if (offset == -1)
        return -1;


    lseek(handle, ofs, SEEK_SET);
    r = _locking(handle, 2, length);
    lseek(handle, offset, SEEK_SET);

    if  (r)
       return -1;

    return 0;
}

int unlock(int handle, long ofs, long length)
{
    long offset = tell(handle);

    if (offset == -1)
        return -1;

    lseek(handle, ofs, SEEK_SET);
    _locking(handle, 0, length);
    lseek(handle, offset, SEEK_SET);

    return 0;
}

/* This #if-#elif-#else-#endif branch doing never!!!
#elif defined(__MSVC__)

#include <io.h>
#include <stdio.h>

int waitlock(int handle, long ofs, long length)
{
    while (lock(handle, ofs, length) == -1)
    {
        tdelay(100);
    }
    return 0;
}

int waitlock2(int handle, long ofs, long length, long t)
{
    int forever = 0;
    int rc;

    if (t==0)
      forever = 1;

    t *= 10;
    while ((rc = lock(handle, ofs, length)) == -1 && (t > 0 || forever))
    {
        tdelay(100);
        t--;
    }

    return rc;
}

int lock(int handle, long ofs, long length)
{
    long offset = tell(handle);
    int r;

    if (offset == -1)
        return -1;

    lseek(handle, ofs, SEEK_SET);
    r = locking(handle, 2, length);
    lseek(handle, offset, SEEK_SET);

    if  (r)
       return -1;

    return 0;
}

int unlock(int handle, long ofs, long length)
{
    long offset = tell(handle);
    int r;

    if (offset == -1)
        return -1;

    lseek(handle, ofs, SEEK_SET);
    r = locking(handle, 0, length);
    lseek(handle, offset, SEEK_SET);

    if (r)
        return -1;
    return 0;
}

*/

#elif defined(__BEOS__)

int lock(int handle, long ofs, long length)
{
	return 0;
}

int waitlock(int handle, long ofs, long length)
{
	return 0;
}

int waitlock2(int handle, long ofs, long length, long t)
{
	return 0;
}

int unlock(int handle, long ofs, long length)
{
	return 0;
}

#ifndef HAS_sopen
int sopen(const char *name, int oflag, int ishared, int mode)
{
    int fd = open(name, oflag, mode);
    return fd;
}
#endif

#elif defined(__UNIX__) && !defined(__BEOS__)

static struct flock* file_lock(short type, long ofs, long length, struct flock *ret)
{
    ret->l_type = type;
    ret->l_start = ofs;
    ret->l_whence = SEEK_SET;
    ret->l_len = length;
    ret->l_pid = getpid();
    return ret;
}

int lock(int handle, long ofs, long length)
{
    struct flock fl;
    return fcntl(handle, F_SETLK, file_lock(F_WRLCK, ofs, length, &fl));
}

int waitlock(int handle, long ofs, long length)
{
    struct flock fl;
    return fcntl(handle, F_SETLKW, file_lock(F_WRLCK, ofs, length, &fl));
}

/*
 * waitlock2() wait <t> seconds for a lock
 */

int waitlock2(int handle, long ofs, long length, long t)
{
	int rc;
	struct flock fl;
	
	alarm(t);
	rc = fcntl(handle, F_SETLKW, file_lock(F_WRLCK, ofs, length, &fl));
	alarm(0);
	
	return rc;
}


int unlock(int handle, long ofs, long length)
{
    struct flock fl;
    return fcntl(handle, F_SETLK, file_lock(F_UNLCK, ofs, length, &fl));
}

#ifndef HAS_sopen
int sopen(const char *name, int oflag, int ishared, int mode)
{
    int fd = open(name, oflag, mode);

    /*
     * I removed this code, 'cause there is no more need for it (i hope so)
     */
/*
#ifndef NO_LOCKING
    struct flock fl;
    if (fd != -1 && fcntl(fd, F_SETLK,
            file_lock((ishared == SH_DENYNONE) ? F_RDLCK : F_WRLCK, 0, 0, &fl)))

    {
        close(fd);
        return -1;
    }
#endif
*/
    return fd;
}
#endif

#else

#ifdef __OS2__
#define INCL_DOSDATETIME
#include <os2.h>
#endif

int waitlock(int handle, long ofs, long length)
{
    while (lock(handle, ofs, length) == -1)
    {
        tdelay(100);
    }
    return 0;
}

int waitlock2(int handle, long ofs, long length, long t)
{
    int forever = 0;
    int rc;

    if (t==0)
      forever = 1;

    t *= 10;

    while ((rc = lock(handle, ofs, length)) == -1 && (t > 0 || forever))
    {
        tdelay(100);
        t--;
    }

    return rc;
}


#endif
