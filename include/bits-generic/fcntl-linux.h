/* Copyright (c) 2018 Griefer@Work                                            *
 *                                                                            *
 * This software is provided 'as-is', without any express or implied          *
 * warranty. In no event will the authors be held liable for any damages      *
 * arising from the use of this software.                                     *
 *                                                                            *
 * Permission is granted to anyone to use this software for any purpose,      *
 * including commercial applications, and to alter it and redistribute it     *
 * freely, subject to the following restrictions:                             *
 *                                                                            *
 * 1. The origin of this software must not be misrepresented; you must not    *
 *    claim that you wrote the original software. If you use this software    *
 *    in a product, an acknowledgement in the product documentation would be  *
 *    appreciated but is not required.                                        *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 */
#ifndef _BITS_GENERIC_FCNTL_LINUX_H
#define _BITS_GENERIC_FCNTL_LINUX_H 1
#define _BITS_FCNTL_LINUX_H 1

#include <__stdinc.h>
#include <hybrid/typecore.h>
#include <bits/types.h>
#include <features.h>

#ifdef __USE_GNU
//#include <bits/uio.h>
#endif /* __USE_GNU */

__SYSDECL_BEGIN

/* Open flags that are universal (binary-wise) */
#define O_ACCMODE     0x003
#define O_RDONLY      0x000
#define O_WRONLY      0x001
#define O_RDWR        0x002
/*      O_RDWR        0x003 // Implemented as an alias! */
#define O_TRUNC       0x200 /* This one was probably just a coincidence... */

/* Flags with common binary compatibility between DOS and UNIX. */
#define __DOS_O_COMMON  (O_TRUNC|O_ACCMODE)



/* DOS open flag values. */
#define __DOS_O_APPEND       0x00008
#define __DOS_O_RANDOM       0x00010
#define __DOS_O_SEQUENTIAL   0x00020 /* Ignored */
#define __DOS_O_TEMPORARY    0x00040 /* Same as O_TMPFILE */
#define __DOS_O_NOINHERIT    0x00080 /* Same as O_CLOEXEC */
#define __DOS_O_CREAT        0x00100
#define __DOS_O_TRUNC        0x00200
#define __DOS_O_EXCL         0x00400
#define __DOS_O_SHORT_LIVED  0x01000 /* Ignored */
#define __DOS_O_OBTAIN_DIR   0x02000 /* Same as O_DIRECTORY (Not quite, but effectively the same...) */
#define __DOS_O_TEXT         0x04000 /* Ignored */
#define __DOS_O_BINARY       0x08000 /* Ignored */
#define __DOS_O_WTEXT        0x10000 /* Ignored */
#define __DOS_O_U16TEXT      0x20000 /* Ignored */
#define __DOS_O_U8TEXT       0x40000 /* Ignored */
#define __DOS_O_RAW          __DOS_O_BINARY

#ifdef __USE_DOS
/* DOS name aliases */
#   define _O_RDONLY       O_RDONLY
#   define _O_WRONLY       O_WRONLY
#   define _O_RDWR         O_RDWR
#   define _O_APPEND       O_APPEND
#   define _O_BINARY       O_BINARY
#   define _O_CREAT        O_CREAT
#   define _O_EXCL         O_EXCL
#   define _O_RANDOM       O_RANDOM
#   define _O_RAW          O_RAW
#   define _O_SEQUENTIAL   O_SEQUENTIAL
#   define _O_TEXT         O_TEXT
#   define _O_TRUNC        O_TRUNC
#   define _O_NOINHERIT    __O_CLOEXEC
#   define _O_TEMPORARY    __O_TMPFILE
#   define _O_OBTAIN_DIR   __O_DIRECTORY
#   define O_NOINHERIT     __O_CLOEXEC
#   define O_TEMPORARY     __O_TMPFILE
#endif /* __USE_DOS */

#ifdef __USE_DOSFS
#undef __O_DIRECTORY
#undef O_APPEND
#undef O_CREAT
#undef O_EXCL
#undef __O_CLOEXEC
#undef __O_TMPFILE
#undef O_NOCTTY
#undef O_NONBLOCK
#undef O_SYNC
#undef O_ASYNC
#undef __O_DSYNC
#undef __O_DIRECT
#undef __O_LARGEFILE
#undef __O_NOFOLLOW
#undef __O_NOATIME
#   define __O_DIRECTORY   __DOS_O_OBTAIN_DIR
#   define O_APPEND        __DOS_O_APPEND
#   define O_CREAT         __DOS_O_CREAT
#   define O_EXCL          __DOS_O_EXCL
#   define __O_CLOEXEC     __DOS_O_NOINHERIT
#   define __O_TMPFILE     __DOS_O_TEMPORARY
/* Unix flags not supported on DOS. */
#   define O_NOCTTY        0
#   define O_NONBLOCK      0
#   define O_SYNC          0
#   define O_ASYNC         0
#   define __O_DSYNC       0
#   define __O_DIRECT      0
#   define __O_LARGEFILE   0
#   define __O_NOFOLLOW    0
#   define __O_NOATIME     0
#ifdef __USE_DOS
/* DOS extension flags. */
#   define O_RAW           __DOS_O_RAW
#   define O_TEXT          __DOS_O_TEXT
#   define O_BINARY        __DOS_O_BINARY
#   define O_SEQUENTIAL    __DOS_O_SEQUENTIAL
#   define O_RANDOM        __DOS_O_RANDOM
#   define _O_SHORT_LIVED  __DOS_O_SHORT_LIVED
#   define _O_U16TEXT      __DOS_O_U16TEXT
#   define _O_U8TEXT       __DOS_O_U8TEXT
#   define _O_WTEXT        __DOS_O_WTEXT
#endif /* __USE_DOS */
#else /* __USE_DOSFS */
#ifndef O_CREAT
#   define O_CREAT        0x40
#endif /* !O_CREAT */
#ifndef O_EXCL
#   define O_EXCL         0x80
#endif /* !O_EXCL */
#ifndef O_NOCTTY
#   define O_NOCTTY       0x100
#endif /* !O_NOCTTY */
#ifndef O_APPEND
#   define O_APPEND       0x400
#endif /* !O_APPEND */
#ifndef O_NONBLOCK
#   define O_NONBLOCK     0x800
#endif /* !O_NONBLOCK */
#ifndef O_SYNC
#   define O_SYNC         0x1000
#endif /* !O_SYNC */
#ifndef __O_DSYNC
#   define __O_DSYNC      0x1000
#endif /* !__O_DSYNC */
#ifndef O_ASYNC
#   define O_ASYNC        0x2000
#endif /* !O_ASYNC */
#ifndef __O_DIRECT
#   define __O_DIRECT     0x4000
#endif /* !__O_DIRECT */
#ifndef __O_LARGEFILE
#   define __O_LARGEFILE  0x8000
#endif /* !__O_LARGEFILE */
#ifndef __O_DIRECTORY
#   define __O_DIRECTORY  0x10000
#endif /* !__O_DIRECTORY */
#ifndef __O_NOFOLLOW
#   define __O_NOFOLLOW   0x20000
#endif /* !__O_NOFOLLOW */
#ifndef __O_NOATIME
#   define __O_NOATIME    0x40000
#endif /* !__O_NOATIME */
#ifndef __O_CLOEXEC
#   define __O_CLOEXEC    0x80000
#endif /* !__O_CLOEXEC */
#ifndef __O_PATH
#   define __O_PATH       0x200000
#endif /* !__O_PATH */
#ifndef __O_TMPFILE
#   define __O_TMPFILE   (0x400000|__O_DIRECTORY)
#endif /* !__O_TMPFILE */

#ifdef __USE_DOS
/* DOS names for some misc./ignored flags. */
#   define O_RAW           0
#   define O_TEXT          0
#   define O_BINARY        0
#   define O_SEQUENTIAL    0
#   define O_RANDOM        0
#   define _O_SHORT_LIVED  0
#   define _O_U16TEXT      0
#   define _O_U8TEXT       0
#   define _O_WTEXT        0
#endif /* __USE_DOS */
#endif /* !__USE_DOSFS */

#ifdef __CRT_KOS
#ifndef __O_CLOFORK
#   define __O_CLOFORK   0x0100000
#endif /* !__O_CLOFORK */
#ifndef __O_SYMLINK
#   define __O_SYMLINK   0x2000000 /* Open a symlink itself, rather than dereferencing it.
                                    * NOTE: When combined with O_EXCL, throw an `ERROR_FS_NOT_A_SYMLINK'
                                    *       if the file isn't a symbolic link.
                                    * NOTE: When used alongside `O_NOFOLLOW', throw an `E_INVALID_ARGUMENT' */
#endif /* !__O_SYMLINK */
#ifndef __O_DOSPATH
#   define __O_DOSPATH   0x4000000 /* Interpret '\\' as '/', and ignore casing during path resolution.
                                    * Additionally, recognize DOS mounting points, and interpret leading
                                    * slashes as relative to the closest DOS mounting point. (s.a.: `AT_DOSPATH') */
#endif /* !__O_DOSPATH */
#else /* __CRT_KOS */
#   undef __O_CLOFORK
#   undef __O_DOSPATH
#   undef __O_SYMLINK
#   define __O_CLOFORK   0
#   define __O_DOSPATH   0
#   define __O_SYMLINK   0
#endif /* !__CRT_KOS */

/* Open anything directly (as best as possible). (file, directory or symlink)
 * KOS: Open a file, directory or symlink directly.
 * DOS: Open a file or directory directly.
 * GLC: Open a file or directory directly.
 */
#ifdef __CRT_KOS
#ifdef __USE_DOSFS
#   define __O_ANYTHING  (__O_SYMLINK|__DOS_O_OBTAIN_DIR)
#else
#   define __O_ANYTHING   __O_SYMLINK
#endif
#elif defined(__USE_DOSFS)
#   define __O_ANYTHING   __DOS_O_OBTAIN_DIR
#else
#   define __O_ANYTHING   0
#endif
#ifdef __USE_KOS
#   define O_ANYTHING   __O_ANYTHING
#endif


#ifndef O_NDELAY
#define O_NDELAY       O_NONBLOCK
#endif /* !O_NDELAY */
#ifndef O_FSYNC
#define O_FSYNC        O_SYNC
#endif /* !O_FSYNC */

#ifdef __USE_LARGEFILE64
#   define O_LARGEFILE  __O_LARGEFILE
#endif
#ifdef __USE_XOPEN2K8
#   define O_DIRECTORY  __O_DIRECTORY
#   define O_NOFOLLOW   __O_NOFOLLOW
#   define O_CLOEXEC    __O_CLOEXEC
#endif
#if defined(__CRT_KOS) && defined(__USE_KOS)
#   define O_CLOFORK    __O_CLOFORK
#   define O_DOSPATH    __O_DOSPATH
#   define O_SYMLINK    __O_SYMLINK
#endif
#ifdef __USE_GNU
#   define O_DIRECT     __O_DIRECT
#   define O_NOATIME    __O_NOATIME
#   define O_PATH       __O_PATH
#   define O_TMPFILE    __O_TMPFILE
#endif
#if defined(__USE_POSIX199309) || defined(__USE_UNIX98)
#   define O_DSYNC      __O_DSYNC
#   define O_RSYNC      O_SYNC
#endif

#ifndef __DOS_COMPAT__
#ifndef __USE_FILE_OFFSET64
#   define F_GETLK     5 /* Get record locking info. */
#   define F_SETLK     6 /* Set record locking info (non-blocking). */
#   define F_SETLKW    7 /* Set record locking info (blocking).    */
#else
#   define F_GETLK     F_GETLK64  /* Get record locking info. */
#   define F_SETLK     F_SETLK64  /* Set record locking info (non-blocking).*/
#   define F_SETLKW    F_SETLKW64 /* Set record locking info (blocking). */
#endif
#ifndef F_GETLK64
#if !defined(__KERNEL__) && __SIZEOF_KERNEL_OFF_T__ == __SIZEOF_OFF64_T__
#   define F_GETLK64   5  /* Get record locking info. */
#   define F_SETLK64   6  /* Set record locking info (non-blocking). */
#   define F_SETLKW64  7  /* Set record locking info (blocking).    */
#else
#   define F_GETLK64   12 /* Get record locking info. */
#   define F_SETLK64   13 /* Set record locking info (non-blocking). */
#   define F_SETLKW64  14 /* Set record locking info (blocking). */
#endif
#endif
#ifdef __USE_GNU
#   define F_OFD_GETLK  36
#   define F_OFD_SETLK  37
#   define F_OFD_SETLKW 38
#endif

#define F_DUPFD        0 /* Duplicate file descriptor. */
#define F_GETFD        1 /* Get file descriptor flags. */
#define F_SETFD        2 /* Set file descriptor flags. */
#define F_GETFL        3 /* Get file status flags. */
#define F_SETFL        4 /* Set file status flags. */

#ifndef __F_SETOWN
#   define __F_SETOWN  8
#   define __F_GETOWN  9
#endif

#if defined(__USE_UNIX98) || defined(__USE_XOPEN2K8)
#   define F_SETOWN    __F_SETOWN /* Get owner (process receiving SIGIO). */
#   define F_GETOWN    __F_GETOWN /* Set owner (process receiving SIGIO). */
#endif

#ifndef __F_SETSIG
#   define __F_SETSIG    10 /* Set number of signal to be sent. */
#   define __F_GETSIG    11 /* Get number of signal to be sent. */
#endif
#ifndef __F_SETOWN_EX
#   define __F_SETOWN_EX 15 /* Get owner (thread receiving SIGIO). */
#   define __F_GETOWN_EX 16 /* Set owner (thread receiving SIGIO). */
#endif

#ifdef __USE_GNU
#   define F_SETSIG      __F_SETSIG    /* Set number of signal to be sent. */
#   define F_GETSIG      __F_GETSIG    /* Get number of signal to be sent. */
#   define F_SETOWN_EX   __F_SETOWN_EX /* Get owner (thread receiving SIGIO). */
#   define F_GETOWN_EX   __F_GETOWN_EX /* Set owner (thread receiving SIGIO). */
#endif

#ifdef __USE_GNU
#   define F_SETLEASE   1024 /* Set a lease. */
#   define F_GETLEASE   1025 /* Enquire what lease is active. */
#   define F_NOTIFY     1026 /* Request notifications on a directory. */
#   define F_SETPIPE_SZ 1031 /* Set pipe page size array. */
#   define F_GETPIPE_SZ 1032 /* Get pipe page size array. */
#endif
#ifdef __USE_XOPEN2K8
#   define F_DUPFD_CLOEXEC 1030 /* Duplicate file descriptor with close-on-exit set. */
#endif
#ifdef __USE_KOS
#   define F_SETFL_XCH  5163 /* Same as 'F_SETFL', but return the old set of flags instead of `-EOK' upon success. */
#endif

#define FD_CLOEXEC   0x01 /* FLAG: Close the descriptor on `exec()'. */
#ifdef __USE_KOS
#define FD_CLOFORK   0x02 /* FLAG: Close the descriptor during unsharing after `fork()' (Similar to `PROT_LOOSE' for memory). */
#endif /* __USE_KOS */

#ifndef F_RDLCK
#   define F_RDLCK 0 /* Read lock. */
#   define F_WRLCK 1 /* Write lock. */
#   define F_UNLCK 2 /* Remove lock. */
#endif

#ifndef F_EXLCK
#   define F_EXLCK 4
#   define F_SHLCK 8
#endif

#ifdef __USE_MISC
#   define LOCK_SH 1 /* Shared lock. */
#   define LOCK_EX 2 /* Exclusive lock. */
#   define LOCK_NB 4 /* Or'd with one of the above to prevent blocking. */
#   define LOCK_UN 8 /* Remove lock. */
#endif

#ifdef __USE_GNU
#   define LOCK_MAND  32  /* This is a mandatory flock: */
#   define LOCK_READ  64  /* ... which allows concurrent read operations. */
#   define LOCK_WRITE 128 /* ... which allows concurrent write operations. */
#   define LOCK_RW    192 /* ... Which allows concurrent read & write operations. */
#endif

#ifdef __USE_GNU
#   define DN_ACCESS    0x00000001 /* File accessed. */
#   define DN_MODIFY    0x00000002 /* File modified. */
#   define DN_CREATE    0x00000004 /* File created. */
#   define DN_DELETE    0x00000008 /* File removed. */
#   define DN_RENAME    0x00000010 /* File renamed. */
#   define DN_ATTRIB    0x00000020 /* File changed attributes. */
#   define DN_MULTISHOT 0x80000000 /* Don't remove notifier. */
#endif


#ifdef __USE_GNU
#ifdef __CC__
enum __pid_type {
    F_OWNER_TID  = 0, /* Kernel thread. */
    F_OWNER_PID  = 1, /* Process. */
    F_OWNER_PGRP = 2, /* Process group. */
    F_OWNER_GID  = 2  /* Alternative, obsolete name. */
};
#ifdef __COMPILER_HAVE_PRAGMA_PUSHMACRO
#pragma push_macro("type")
#pragma push_macro("pid")
#endif /* __COMPILER_HAVE_PRAGMA_PUSHMACRO */
#undef type
#undef pid
struct f_owner_ex {
    enum __pid_type type; /* Owner type of ID. */
    __pid_t         pid;  /* ID of owner. */
};
#ifdef __COMPILER_HAVE_PRAGMA_PUSHMACRO
#pragma pop_macro("pid")
#pragma pop_macro("type")
#endif /* __COMPILER_HAVE_PRAGMA_PUSHMACRO */
#endif /* __CC__ */
#endif

#ifdef __USE_MISC
#   define FAPPEND   O_APPEND
#   define FFSYNC    O_FSYNC
#   define FASYNC    O_ASYNC
#   define FNONBLOCK O_NONBLOCK
#   define FNDELAY   O_NDELAY
#endif
#ifndef __POSIX_FADV_DONTNEED
#   define __POSIX_FADV_DONTNEED 4
#   define __POSIX_FADV_NOREUSE  5
#endif
#ifdef __USE_XOPEN2K
#   define POSIX_FADV_NORMAL     0 /* No further special treatment. */
#   define POSIX_FADV_RANDOM     1 /* Expect random page references. */
#   define POSIX_FADV_SEQUENTIAL 2 /* Expect sequential page references. */
#   define POSIX_FADV_WILLNEED   3 /* Will need these pages. */
#   define POSIX_FADV_DONTNEED   __POSIX_FADV_DONTNEED /* Don't need these pages. */
#   define POSIX_FADV_NOREUSE    __POSIX_FADV_NOREUSE  /* Data will be accessed once. */
#endif

#ifdef __USE_GNU
#define SYNC_FILE_RANGE_WAIT_BEFORE 1 /* Wait upon writeout of all pages in the range before performing the write. */
#define SYNC_FILE_RANGE_WRITE       2 /* Initiate writeout of all those dirty pages in the range which are not presently under writeback. */
#define SYNC_FILE_RANGE_WAIT_AFTER  4 /* Wait upon writeout of all pages in the range after performing the write. */

#define SPLICE_F_MOVE               1 /* Move pages instead of copying. */
#define SPLICE_F_NONBLOCK           2 /* Don't block on the pipe splicing (but we may still block on the fd we splice from/to). */
#define SPLICE_F_MORE               4 /* Expect more data. */
#define SPLICE_F_GIFT               8 /* Pages passed in are a gift. */

#define FALLOC_FL_KEEP_SIZE         1 /* Don't extend size of file even if offset + len is greater than file size. */
#define FALLOC_FL_PUNCH_HOLE        2 /* Create a hole in the file. */
#define FALLOC_FL_COLLAPSE_RANGE    8 /* Remove a range of a file without leaving a hole. */
#define FALLOC_FL_ZERO_RANGE       16 /* Convert a range of a file to zeros. */

#ifdef __CC__
#ifdef __COMPILER_HAVE_PRAGMA_PUSHMACRO
#pragma push_macro("handle_bytes")
#pragma push_macro("handle_type")
#endif /* __COMPILER_HAVE_PRAGMA_PUSHMACRO */
#undef handle_bytes
#undef handle_type
struct file_handle {
    unsigned int  handle_bytes;
    int           handle_type;
    __empty_arr(unsigned char,f_handle); /* File identifier. */
};
#define MAX_HANDLE_SZ    128
#ifdef __COMPILER_HAVE_PRAGMA_PUSHMACRO
#pragma pop_macro("handle_type")
#pragma pop_macro("handle_bytes")
#endif /* __COMPILER_HAVE_PRAGMA_PUSHMACRO */
#endif /* __CC__ */
#endif

#ifdef __USE_ATFILE
#   define AT_FDCWD              (-100)/* Special value used to indicate the *at functions should use the current working directory. */
#   define AT_SYMLINK_NOFOLLOW  0x0100 /* If the last path component is a symlink, don't follow it. */
#   define AT_REMOVEDIR         0x0200 /* Remove directory instead of unlinking file. */
#   define AT_EACCESS           0x0200 /* Test access permitted for effective IDs, not real IDs. */
#   define AT_SYMLINK_FOLLOW    0x0400 /* If the last path component is a symlink, follow it. (WARNING: Only used by `linkat(2)') */
#ifdef __USE_GNU
#   define AT_NO_AUTOMOUNT      0x0800 /* Suppress terminal automount traversal. */
#   define AT_EMPTY_PATH        0x1000 /* Allow empty relative pathname. */
#endif /* __USE_GNU */

#if defined(__USE_KOS) || defined(__KERNEL__)
#if __KOS_VERSION__ >= 300
#   define AT_SYMLINK_REGULAR   0x2000 /* Treat symbolic links similar to like regular files and throw an `ERROR_FS_TOO_MANY_LINKS' error during the first encounter. */
#   define AT_CHANGE_CTIME      0x0200 /* For use with `utimensat' and friends: Take `struct timespec[3]', where the 3rd entry
                                        * (when not equal to `UTIME_OMIT') is used to override the file creation timestamp.
                                        * NOTE: Passing this flag when the 3rd timespec isn't set to `UTIME_OMIT' requires
                                        *       that the calling thread be holding the `CAP_SYS_TIME' permission and be the
                                        *       owner of the file in question, or to be holding the `CAP_FOWNER' permission.
                                        *       Less permissions are required when `UTIME_NOW' is passed, in which case
                                        *      `CAP_SYS_TIME' is not required (similarly how having `CAP_FOWNER', or being
                                        *       the owner would allow you to copy the file (updating the timestamp), then
                                        *       replacing the original)
                                        * NOTE: This flag is used to implement full compatibility with NT's SetFileTime function.
                                        */
#   define AT_READLINK_REQSIZE  0x0200 /* For use with `freadlinkat' and friends.
                                        * Rather than following unix semantics and returning the amount of
                                        * copied bytes with no indication of whether or not everything was
                                        * copied, return the ~required~ buffer size (including a terminating
                                        * NUL-character that normally wouldn't be copied either) to user-space
                                        * (which may be more than the specified buffer size, which should be
                                        * reallocated to fit in successive calls)
                                        * Additionally, as already mentioned, a trailing NUL-character is
                                        * appended to the link text, ensuring that a valid C-style string
                                        * can be read so long as the provided buffer was of sufficient size. */
#   define AT_REMOVEREG        0x00200 /* Explicitly allow removing anything that unlink() removes. (Default;
                                        * Set in addition to `AT_REMOVEDIR' to implement `remove()' semantics
                                        * as an atomic kernel operation, removing any race condition that the
                                        * alternative of `if (unlink(x) && errno == EISDIR) rmdir(x)' would
                                        * introduce). */
#else /* __KOS_VERSION__ >= 300 */
#   define AT_REMOVEREG        0x40000 /* ... */
#ifdef __KERNEL__
#   define AT_REMOVEMNT        0x80000 /* Used internally by the kernel: Delete a mounting point. (Userspace must use `unmount()') */
#endif /* __KERNEL__ */
#endif /* __KOS_VERSION__ < 300 */
#   define AT_DOSPATH         0x100000 /* Interpret '\\' as '/', and ignore casing during path resolution.
                                        * Additionally, recognize `<LETTER>:'-style drive prefixes, as well
                                        * as make use of per-drive working directories, as well as drive-relative
                                        * path names that start with a leading slash.
                                        * Basically, when set: perform the system call in DOS-compatibility mode.
                                        * HINT: This flag can be specified with the `fsmode()' system call. */

/* Same as `AT_FDCWD' but sets the filesystem root
 * (using this, you can `chroot()' with 'dup2()'!) */
#define AT_FDROOT        (-101)

/* Special, symbolic file numbers. 
 * These descriptors cannot be overwritten,
 * and their meaning is context-sensible. */
#define AT_THIS_TASK     (-180)
#define AT_THIS_MMAN     AT_THIS_TASK /* DEPRECATED */
#define AT_THIS_STACK    AT_THIS_TASK /* DEPRECATED */

#if __KOS_VERSION__ >= 300
/* DOS Drive root / current-working paths.
 * These are special file descriptors that can be written to using `dup2()',
 * where they expect to receive either a FILE or PATH descriptor.
 * Once set, these paths describe the root-paths of DOS's drives:
 * >> // Force DOS filesystem semantics for all filesystem operations.
 * >> struct fsmask mask = {0,0};
 * >> mask = fsmode(mask);
 * >> mask.fm_mask &= ~AT_DOSPATH;
 * >> mask.fm_mode |=  AT_DOSPATH;
 * >> fsmode(mask);
 * >>
 * >> // Mount some filesystem locations as DOS drives.
 * >> dup2(open("/"),AT_FDDRIVE_ROOT('C'));
 * >> dup2(open("/bin"),AT_FDDRIVE_ROOT('D'));
 * >> dup2(open("/home"),AT_FDDRIVE_ROOT('E'));
 * >>
 * >> // Open a file on a dos-path.
 * >> open("D:\\ls"); // open("/bin/ls")
 * >>
 * >> // Explicitly set the current-working directory of a specific drive.
 * >> // NOTE: Requires that the opened path be reachable from `AT_FDDRIVE_ROOT('E')'
 * >> //       If it isn't, an `ERROR_FS_CROSSDEVICE_LINK' error is thrown.
 * >> dup2(open("/home/me/Downloads"),AT_FDDRIVE_CWD('E'));
 * >>
 * >> chdir("C:\\bin"); // AT_FDCWD = "/bin"; AT_FDDRIVE_CWD('C') = AT_FDCWD;
 * >> chdir("E:");      // AT_FDCWD = AT_FDDRIVE_CWD('E'); (`/home/me/Downloads')
 * >> chdir("E:\\");    // AT_FDCWD = "/home"; AT_FDDRIVE_CWD('E') = AT_FDCWD;
 * >> chdir("C:");      // AT_FDCWD = AT_FDDRIVE_CWD('C'); (`/bin')
 * >> chdir("D:\\");    // AT_FDCWD = "/bin"; AT_FDDRIVE_CWD('D') = AT_FDCWD;
 * >> chdir("..");      // AT_FDCWD = "/bin"; AT_FDDRIVE_CWD('D') = AT_FDCWD; (Don't escape the current drive)
 * >>
 * NOTES:
 *   - Setting the root-path of a drive will not update the drive working directories of processes sharing VFS.
 *     With that in mind, you should not re-mount a DOS drive as a sub-directory of where
 *     it was located before then, unless you first unshared your VFS (using `CLONE_NEWNS').
 *     Allowing a program's current working directory to be located outside the associated
 *     drive causes weak undefined behavior. In KOS this means:
 *       - `xfdname()' will not produce the correct path
 *       - Relative path names still operate on the old directory
 *       - Navigating into parent directories is no longer restricted (except by `chroot()')
 *       - Manually navigating into a portion of the new drives mounting point fixes the problem.
 *   - DOS drive mounting points are beneath UNIX mounting points, in that they don't
 *     actually mount a superblock to some filesystem location, but rather mirror a
 *     filesystem path, meaning that DOS drives can be thought of as the equivalent
 *     of the `dfd' argument of `*at' system calls:
 *     >> open("C:\\foo.txt"); // Same as `openat(AT_FDDRIVE_ROOT('C'),"foo.txt")'
 *     If `mount()' is used to create a new (real) mounting point, it will appear
 *     in all DOS drives that are mounting the location where the mount happened,
 *     or any location further up the tree immediately (it's the same data object
 *     internally; DOS drives are merely aliasing regular paths; aka. `O_PATH')
 *   - To ~unmount~ (quote-unquote) a DOS drive, simply close() the associated root file descriptor:
 *     >> // Unmount `C:' (also closes the associated FDCWD descriptor)
 *     >> close(AT_FDDRIVE_ROOT('C'));
 *   - Mounting, or unmounting dos drives requires the same privileges as the `mount()' and `umount()' system calls.
 *   - Closing an `AT_FDDRIVE_CWD()' descriptor will revert it to match the root path of the associated drive.
 *     >> close(AT_FDDRIVE_CWD('C'));
 *     >> // Same as:
 *     >> dup2(AT_FDDRIVE_ROOT('C'),AT_FDDRIVE_CWD('C'));
 *   - Symbolic links are evaluated the same as any user-provided path.
 *     If a symbolic link starts with a '/' or '\\' character, the remainder
 *     of its text is relative to the first DOS drive mounting point
 *     encountered while walking up the chain of parent directories.
 */
#   define AT_FDDRIVE_CWD(drivechar)  ((-350)+((drivechar)-'A'))
#   define AT_FDDRIVE_ROOT(drivechar) ((-300)+((drivechar)-'A'))
#   define AT_DOS_DRIVEMIN            'A'
#   define AT_DOS_DRIVEMAX            'Z'
#endif
#endif /* __USE_KOS */
#endif /* __USE_ATFILE */
#endif /* !__DOS_COMPAT__ */

#ifdef __USE_GNU
#ifdef __CC__
#ifdef __DOS_COMPAT__
/* Ignore these as no-ops under DOS. */
__LOCAL __ssize_t (__LIBCCALL readahead)(__fd_t __UNUSED(__fd), __off64_t __UNUSED(__offset), __size_t __count) { return (__size_t)__count; }
__LOCAL int (__LIBCCALL sync_file_range)(__fd_t __UNUSED(__fd), __off64_t __UNUSED(__offset), __off64_t __UNUSED(__count), unsigned int __UNUSED(__flags)) { return 0; }
__LOCAL int (__LIBCCALL fallocate)(__fd_t __UNUSED(__fd), __fd_t __UNUSED(__mode), __off_t __UNUSED(__offset), __off_t __UNUSED(__len)) { return 0; }
#ifdef __USE_LARGEFILE64
__LOCAL int (__LIBCCALL fallocate64)(__fd_t __UNUSED(__fd), __fd_t __UNUSED(__mode), __off64_t __UNUSED(__offset), __off64_t __UNUSED(__len)) { return 0; }
#endif /* __USE_LARGEFILE64 */
#else /* __DOS_COMPAT__ */
struct iovec;
__LIBC __ssize_t (__LIBCCALL readahead)(__fd_t __fd, __off64_t __offset, __size_t __count);
__LIBC int (__LIBCCALL sync_file_range)(__fd_t __fd, __off64_t __offset, __off64_t __count, unsigned int __flags);
__LIBC __PORT_NODOS __ssize_t (__LIBCCALL vmsplice)(__fd_t __fdout, struct iovec const *__iov, __size_t __count, unsigned int __flags);
__LIBC __PORT_NODOS __ssize_t (__LIBCCALL splice)(__fd_t __fdin, __off64_t *__offin, __fd_t __fdout, __off64_t *__offout, __size_t __len, unsigned int __flags);
__LIBC __PORT_NODOS __ssize_t (__LIBCCALL tee)(__fd_t __fdin, __fd_t __fdout, __size_t __len, unsigned int __flags);
__LIBC __PORT_NODOS int (__LIBCCALL name_to_handle_at)(__fd_t __dfd, char const *__name, struct file_handle *__handle, int *__mnt_id, int __flags);
__LIBC __PORT_NODOS int (__LIBCCALL open_by_handle_at)(__fd_t __mountdirfd, struct file_handle *__handle, int __flags);
__REDIRECT_FS64(__LIBC,,int,__LIBCCALL,fallocate,(__fd_t __fd, int __mode, __FS_TYPE(off) __offset, __FS_TYPE(off) __len),(__fd,__mode,__offset,__len))
#ifdef __USE_LARGEFILE64
__LIBC int (__LIBCCALL fallocate64)(__fd_t __fd, int __mode, __off64_t __offset, __off64_t __len);
#endif /* __USE_LARGEFILE64 */
#endif /* !__DOS_COMPAT__ */
#endif /* __CC__ */
#endif /* __USE_GNU */

__SYSDECL_END

#endif /* !_BITS_GENERIC_FCNTL_LINUX_H */
