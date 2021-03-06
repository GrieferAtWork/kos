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
#ifndef _UNISTD_H
#define _UNISTD_H 1

#include "__stdinc.h"
#include <features.h>
#include <bits/types.h>
#include <bits/confname.h>
#include <bits/posix_opt.h>
#include <parts/redirect-exec.h>
#ifdef __CRT_GLC
#include <asm/unistd.h>
#endif /* __CRT_GLC */
#if defined(__USE_UNIX98) || defined(__USE_XOPEN2K)
#include <bits/environments.h>
#endif

__SYSDECL_BEGIN

/* Disclaimer: Code below is based off of /usr/include/unistd.h, yet has been _heavily_ modified. */

/* Copyright (C) 1991-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#ifdef __USE_XOPEN2K8
#   define _POSIX_VERSION    200809L
#elif defined(__USE_XOPEN2K)
#   define _POSIX_VERSION    200112L
#elif defined(__USE_POSIX199506)
#   define _POSIX_VERSION    199506L
#elif defined(__USE_POSIX199309)
#   define _POSIX_VERSION    199309L
#else
#   define _POSIX_VERSION    199009L
#endif
#ifdef __USE_XOPEN2K8
#   define __POSIX2_THIS_VERSION    200809L
#elif defined(__USE_XOPEN2K)
#   define __POSIX2_THIS_VERSION    200112L
#elif defined(__USE_POSIX199506)
#   define __POSIX2_THIS_VERSION    199506L
#else
#   define __POSIX2_THIS_VERSION    199209L
#endif

#define _POSIX2_VERSION   __POSIX2_THIS_VERSION
#define _POSIX2_C_VERSION __POSIX2_THIS_VERSION
#define _POSIX2_C_BIND    __POSIX2_THIS_VERSION
#define _POSIX2_C_DEV     __POSIX2_THIS_VERSION
#define _POSIX2_SW_DEV    __POSIX2_THIS_VERSION
#define _POSIX2_LOCALEDEF __POSIX2_THIS_VERSION

#ifdef __USE_XOPEN2K8
#   define _XOPEN_VERSION    700
#elif defined(__USE_XOPEN2K)
#   define _XOPEN_VERSION    600
#elif defined(__USE_UNIX98)
#   define _XOPEN_VERSION    500
#else
#   define _XOPEN_VERSION    4
#endif
#define _XOPEN_XCU_VERSION   4
#define _XOPEN_XPG2          1
#define _XOPEN_XPG3          1
#define _XOPEN_XPG4          1
#define _XOPEN_UNIX          1
#define _XOPEN_CRYPT         1
#define _XOPEN_ENH_I18N      1
#define _XOPEN_LEGACY        1

#define STDIN_FILENO  0 /* Standard input.  */
#define STDOUT_FILENO 1 /* Standard output.  */
#define STDERR_FILENO 2 /* Standard error output.  */

#ifndef __ssize_t_defined
#define __ssize_t_defined 1
typedef __ssize_t ssize_t;
#endif /* !__ssize_t_defined */

#ifndef __size_t_defined
#define __size_t_defined 1
typedef __SIZE_TYPE__ size_t;
#endif /* !__size_t_defined */

#ifndef NULL
#define NULL __NULLPTR
#endif /* !NULL */

#if defined(__USE_XOPEN) || defined(__USE_XOPEN2K)
#ifndef __gid_t_defined
#define __gid_t_defined 1
typedef __gid_t gid_t;
#endif /* !__gid_t_defined */
#ifndef __uid_t_defined
#define __uid_t_defined 1
typedef __uid_t uid_t;
#endif /* !__uid_t_defined */
#ifndef __off_t_defined
#define __off_t_defined
typedef __typedef_off_t off_t;
#endif /* !__off_t_defined */
#ifndef __useconds_t_defined
#define __useconds_t_defined 1
typedef __useconds_t useconds_t;
#endif /* !__useconds_t_defined */
#ifndef __pid_t_defined
#define __pid_t_defined 1
typedef __pid_t pid_t;
#endif /* !__pid_t_defined */
#ifdef __USE_LARGEFILE64
#ifndef __off64_t_defined
#define __off64_t_defined 1
typedef __off64_t off64_t;
#endif /* !__off64_t_defined */
#endif /* __USE_LARGEFILE64 */
#endif /* __USE_XOPEN || __USE_XOPEN2K */

#if defined(__USE_XOPEN_EXTENDED) || \
    defined(__USE_XOPEN2K)
#ifndef __intptr_t_defined
#define __intptr_t_defined 1
typedef __intptr_t intptr_t;
#endif /* !__intptr_t_defined */
#endif

#if defined(__USE_MISC) || defined(__USE_XOPEN)
#ifndef __socklen_t_defined
#define __socklen_t_defined 1
typedef __socklen_t socklen_t;
#endif /* !__socklen_t_defined */
#endif

#define F_OK 0 /* Test for existence.  */
#define X_OK 1 /* Test for execute permission.  */
#define W_OK 2 /* Test for write permission.  */
#define R_OK 4 /* Test for read permission.  */

#ifndef SEEK_SET
#   define SEEK_SET  0 /* Seek from beginning of file.  */
#   define SEEK_CUR  1 /* Seek from current position.  */
#   define SEEK_END  2 /* Seek from end of file.  */
#ifdef __USE_GNU
#   define SEEK_DATA 3 /* Seek to next data.  */
#   define SEEK_HOLE 4 /* Seek to next hole.  */
#endif /* __USE_GNU */
#endif

#ifdef __USE_MISC
#ifndef L_SET
#   define L_SET  SEEK_SET
#   define L_CURR SEEK_CUR
#   define L_INCR SEEK_CUR
#   define L_XTND SEEK_END
#endif /* !L_SET */
#endif


/* Argument types used by exec() and spawn() functions. */
#ifndef __TARGV
#ifdef __USE_DOS
#   define __TARGV  char const *const *___argv
#   define __TENVP  char const *const *___envp
#else
#   define __TARGV  char *const ___argv[__restrict_arr]
#   define __TENVP  char *const ___envp[__restrict_arr]
#endif
#endif /* !__TARGV */


#ifndef __KERNEL__

#ifndef ____environ_defined
#define ____environ_defined 1
#ifndef __NO_ASMNAME
#undef __environ
#ifdef __DOS_COMPAT__
__LIBC char **__environ __ASMNAME("_environ");
#else /* __DOS_COMPAT__ */
__LIBC char **__environ __ASMNAME("environ");
#endif /* !__DOS_COMPAT__ */
#else /* __NO_ASMNAME */
#ifdef __DOS_COMPAT__
#undef _environ
__LIBC char **_environ;
#define __environ _environ
#else /* __DOS_COMPAT__ */
#undef environ
__LIBC char **environ;
#define __environ environ
#endif /* !__DOS_COMPAT__ */
#endif /* !__NO_ASMNAME */
#endif /* !____environ_defined */

#ifndef __execl_defined
#define __execl_defined 1

/* >> execv(3), execve(3), execvp(3), execl(3), execle(3), execlp(3)
 * Replace the calling process with the application image referred to by `PATH' / `FILE'
 * and execute it's `main()' method, passing the given `ARGV', and setting `environ' to `ENVP' */
__REDIRECT_EXCEPT_UFSDPA_XVOID(__LIBC,__XATTR_NORETURN __NONNULL((1,2)),int,__LIBCCALL,execv,(char const *__restrict __path, __TARGV),(__path,___argv))
__REDIRECT_EXCEPT_UFSDPA_XVOID(__LIBC,__XATTR_NORETURN __NONNULL((1,2,3)),int,__LIBCCALL,execve,(char const *__restrict __path, __TARGV, __TENVP),(__path,___argv,___envp))
__REDIRECT_EXCEPT_UFSDPA_XVOID(__LIBC,__XATTR_NORETURN __NONNULL((1,2)),int,__LIBCCALL,execvp,(char const *__restrict __file, __TARGV),(__path,___argv))
__XREDIRECT_EXCEPT_UFSDPA_XVOID(__LIBC,__XATTR_NORETURN __NONNULL((1)) __ATTR_SENTINEL,int,__ATTR_CDECL,execl,(char const *__restrict __path, char const *__args, ...),__REDIRECT_EXECL(char,execv,__path,__args))
__XREDIRECT_EXCEPT_UFSDPA_XVOID(__LIBC,__XATTR_NORETURN __NONNULL((1)) __ATTR_SENTINEL_O(1),int,__ATTR_CDECL,execle,(char const *__restrict __path, char const *__args, ...),__REDIRECT_EXECLE(char,execve,__path,__args))
__XREDIRECT_EXCEPT_UFSDPA_XVOID(__LIBC,__XATTR_NORETURN __NONNULL((1)) __ATTR_SENTINEL,int,__ATTR_CDECL,execlp,(char const *__restrict __file, char const *__args, ...),__REDIRECT_EXECL(char,execvp,__path,__args))

#ifdef __USE_EXCEPT
/* >> execv(3), execve(3), execvp(3), execl(3), execle(3), execlp(3)
 * Replace the calling process with the application image referred to by `PATH' / `FILE'
 * and execute it's `main()' method, passing the given `ARGV', and setting `environ' to `ENVP' */
__LIBC __ATTR_NORETURN __NONNULL((1,2)) void (__LIBCCALL Xexecv)(char const *__restrict __path, __TARGV);
__LIBC __ATTR_NORETURN __NONNULL((1,2,3)) void (__LIBCCALL Xexecve)(char const *__restrict __path, __TARGV, __TENVP);
__LIBC __ATTR_NORETURN __NONNULL((1,2)) void (__LIBCCALL Xexecvp)(char const *__restrict __file, __TARGV);
__LIBC __ATTR_NORETURN __NONNULL((1)) __ATTR_SENTINEL void (__ATTR_CDECL Xexecl)(char const *__restrict __path, char const *__args, ...);
__LIBC __ATTR_NORETURN __NONNULL((1)) __ATTR_SENTINEL_O(1) void (__ATTR_CDECL Xexecle)(char const *__restrict __path, char const *__args, ...);
__LIBC __ATTR_NORETURN __NONNULL((1)) __ATTR_SENTINEL void (__ATTR_CDECL Xexeclp)(char const *__restrict __file, char const *__args, ...);
#endif /* __USE_EXCEPT */
#endif /* !__execl_defined */

#ifndef __getpid_defined
#define __getpid_defined 1
/* >> getpid(2)
 * Return the PID of the calling process (that is the TID of the calling thread group's leader)
 * THIS_THREAD->LEADER->PID */
__REDIRECT_DPA(__LIBC,__WUNUSED,__pid_t,__LIBCCALL,getpid,(void),())
#endif /* !__getpid_defined */

#ifdef __DOS_COMPAT__
__REDIRECT(__LIBC,__WUNUSED_SUGGESTED __NONNULL((1)),int,__LIBCCALL,__dos_pipe,(__fd_t __pipedes[2], __UINT32_TYPE__ __pipesize, int __textmode),_pipe,(__pipedes,__pipesize,__textmode))
/* >> pipe(2)
 * Create a new pair of connected pipes ([0] = reader, [1] = writer) */
__LOCAL __WUNUSED_SUGGESTED __NONNULL((1)) int (__LIBCCALL pipe)(__fd_t __pipedes[2]) { return __dos_pipe(__pipedes,4096,0x8000); /*O_BINARY*/ }
#else /* __DOS_COMPAT__ */
/* >> pipe(2)
 * Create a new pair of connected pipes ([0] = reader, [1] = writer) */
__REDIRECT_EXCEPT_XVOID(__LIBC,__WUNUSED_SUGGESTED __NONNULL((1)),int,__LIBCCALL,pipe,(__fd_t __pipedes[2]),(__pipedes))
#endif /* !__DOS_COMPAT__ */

#ifndef __sleep_defined
#define __sleep_defined 1
#ifdef __DOS_COMPAT__
__REDIRECT_VOID(__LIBC,,__LIBCCALL,__dos_sleep,(__UINT32_TYPE__ __duration),_sleep,(__duration))

/* >> sleep(3)
 * Sleep for up to `SECONDS' seconds. */
__LOCAL unsigned int (__LIBCCALL sleep)(unsigned int __seconds) { __dos_sleep(__seconds); return 0; }
#else /* __DOS_COMPAT__ */
/* >> sleep(3)
 * Sleep for up to `SECONDS' seconds. */
__LIBC unsigned int (__LIBCCALL sleep)(unsigned int __seconds);
#endif /* !__DOS_COMPAT__ */
#endif /* !__sleep_defined */

#ifdef __DOS_COMPAT__
/* >> fsync(2)
 * Synchronize a file (including its descriptor which contains timestamps, and its size),
 * meaning that changes to its data and/or descriptor are written to disk. */
__REDIRECT(__LIBC,,int,__LIBCCALL,fsync,(__fd_t __fd),_commit,(__fd))
#else /* __DOS_COMPAT__ */
/* >> fsync(2)
 * Synchronize a file (including its descriptor which contains timestamps, and its size),
 * meaning that changes to its data and/or descriptor are written to disk. */
__REDIRECT_EXCEPT_XVOID(__LIBC,,int,__LIBCCALL,fsync,(__fd_t __fd),(__fd))
#endif /* !__DOS_COMPAT__ */

#ifdef __CRT_GLC
/* >> getppid(2)
 * Return the PID of the calling process's parent.
 * (That is the TID of the leader of the parent of the calling thread's leader)
 * THIS_THREAD->LEADER->PARENT->LEADER->PID */
__LIBC __PORT_NODOS __WUNUSED __pid_t (__LIBCCALL getppid)(void);

/* >> getpgrp(2)
 * Return the ID of the calling process's process group.
 * (That is the TID of the leader of the process group of the calling thread's leader)
 * THIS_THREAD->LEADER->GROUP_LEADER->PID */
__LIBC __PORT_NODOS __WUNUSED __pid_t (__LIBCCALL getpgrp)(void);

/* >> getpgid(2)
 * Return the ID of the process group associated with `PID's process.
 * (That is the TID of the leader of the process group of `PID's leader)
 * THREAD[PID]->LEADER->GROUP_LEADER->PID
 * When `PID' is ZERO(0), use `gettid()' for it instead. */
__REDIRECT_EXCEPT_(__LIBC,__PORT_NODOS __WUNUSED,__pid_t,__LIBCCALL,__getpgid,(__pid_t __pid),getpgid,(__pid))

/* >> setpgid(2)
 * Change the ID of the process group associated with `PID's process.
 * (That is the TID of the leader of the process group of `PID's leader)
 * THREAD[PID]->LEADER->GROUP_LEADER = THREAD[PGID]
 * When `PID' is ZERO(0), use `gettid()' for it instead.
 * When `PGID' is ZERO(0), use `PID' (after it was substituted) for instead. */
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,setpgid,(__pid_t __pid, __pid_t __pgid),(__pid,__pgid))

/* >> setsid()
 * Make the calling thread's process the leader of its associated
 * process group, before also making it its own session leader.
 * Then return the TID of that new session leader, which is also the PID of the calling process.
 * THIS_THREAD->LEADER->GROUP_LEADER                 = THIS_THREAD->LEADER;
 * THIS_THREAD->LEADER->GROUP_LEADER->SESSION_LEADER = THIS_THREAD->LEADER->GROUP_LEADER;
 * return THIS_THREAD->LEADER->PID; */
__LIBC __PORT_NODOS __pid_t (__LIBCCALL setsid)(void);

/* ... */
__LIBC __PORT_NODOS __WUNUSED __uid_t (__LIBCCALL getuid)(void);

/* ... */
__LIBC __PORT_NODOS __WUNUSED __uid_t (__LIBCCALL geteuid)(void);

/* ... */
__LIBC __PORT_NODOS __WUNUSED __gid_t (__LIBCCALL getgid)(void);

/* ... */
__LIBC __PORT_NODOS __WUNUSED __gid_t (__LIBCCALL getegid)(void);

/* ... */
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,getgroups,(int __size, __gid_t __list[]),(__size,__list))

/* ... */
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,setuid,(__uid_t __uid),(__uid))

/* ... */
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,setgid,(__gid_t __gid),(__gid))

/* >> fork(2)
 * Clone the calling thread into a second process and return twice, once
 * in the parent process where this function returns the (non-zero) PID
 * of the forked child process, and a second time in the child process
 * itself, where ZERO(0) is returned.
 * The child then usually proceeds by calling `exec(2)' to replace its
 * application image with that of another program that the original
 * parent can then `wait(2)' for. */
__REDIRECT_EXCEPT(__LIBC,__PORT_NODOS __WUNUSED,__pid_t,__LIBCCALL,fork,(void),())

/* >> alarm(2)
 * @return: 0 : No alarm was scheduled before.
 * @return: * : The number of seconds yet to pass before a previous alarm would have elapsed.
 * Schedule an to deliver a `SIGALRM' after letting `seconds' elapse.
 * You may pass ZERO(0) for SECONDS to disable a previously scheduled alarm. */
__LIBC __PORT_NODOS unsigned int (__LIBCCALL alarm)(unsigned int __seconds);

/* >> pause(2)
 * Suspend execution until the delivery of a POSIX_SIGNAL */
__REDIRECT_EXCEPT_XVOID(__LIBC,__XATTR_NORETURN __PORT_NODOS,int,__LIBCCALL,pause,(void),())

/* >> fpathconf(2)
 * @param: NAME: One of `_PC_*' from <bits/confname.h>
 * Return a path configuration value associated with `NAME' for `FD' */
__REDIRECT_EXCEPT(__LIBC,__PORT_NODOS __WUNUSED,long int,__LIBCCALL,fpathconf,(__fd_t __fd, int __name),(__fd,__name))

/* >> ttyname(3), ttyname_r(3)
 * Return the name of a TTY given its file descriptor. */
__LIBC __XATTR_RETNONNULL __PORT_NODOS __WUNUSED char *(__LIBCCALL ttyname)(__fd_t __fd);
__LIBC __PORT_NODOS __NONNULL((2)) int (__LIBCCALL ttyname_r)(__fd_t __fd, char *__buf, size_t __buflen);

/* >> tcgetpgrp(2)
 * Return the foreground process group of a given TTY file descriptor. */
__REDIRECT_EXCEPT(__LIBC,__PORT_NODOS __WUNUSED,__pid_t,__LIBCCALL,tcgetpgrp,(__fd_t __fd),(__fd))

/* >> tcsetpgrp(2)
 * Set the foreground process group of a given TTY file descriptor. */
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,tcsetpgrp,(__fd_t __fd, __pid_t __pgrp_id),(__fd,__pgrp_id))

/* ... */
__REDIRECT_EXCEPT(__LIBC,__XATTR_RETNONNULL __PORT_NODOS __WUNUSED,char *,__LIBCCALL,getlogin,(void),())

/* >> chown(2)
 * Change the ownership of a given `FILE' to `GROUP:OWNER'. */
__REDIRECT_EXCEPT_UFS_XVOID(__LIBC,__PORT_NODOS __NONNULL((1)),int,__LIBCCALL,chown,(char const *__file, __uid_t __owner, __gid_t __group),(__file,__owner,__group))

/* >> fpathconf(2)
 * @param: NAME: One of `_PC_*' from <bits/confname.h>
 * Return a path configuration value associated with `NAME' for `PATH' */
__REDIRECT_EXCEPT_UFS(__LIBC,__PORT_NODOS __NONNULL((1)),long int,__LIBCCALL,pathconf,(char const *__path, int __name),(__path,__name))

/* >> link(2)
 * Create a hard link from `FROM', leading to `TO' */
__REDIRECT_EXCEPT_UFS_XVOID(__LIBC,__PORT_NODOS __NONNULL((1,2)),int,__LIBCCALL,link,(char const *__from, char const *__to),(__from,__to))
#endif /* __CRT_GLC */

#ifndef ___exit_defined
#define ___exit_defined 1
/* >> _exit(2)
 * Exit the calling process without executing `atexit(3)' callbacks. */
__LIBC __ATTR_NORETURN void (__LIBCCALL _exit)(int __status);
#endif /* !___exit_defined */

#ifndef __read_defined
#define __read_defined 1
#if __SIZEOF_SIZE_T__ <= 4
/* >> read(2)
 * Read data from a given file descriptor `FD' and return the number of bytes read.
 * A return value of ZERO(0) is indicative of EOF. */
__REDIRECT_EXCEPT_DPA(__LIBC,__WUNUSED_SUGGESTED __NONNULL((2)),__EXCEPT_SELECT(size_t,ssize_t),__LIBCCALL,
                      read,(__fd_t __fd, void *__dstbuf, size_t __dstbufsize),(__fd,__dstbuf,__dstbufsize))
#elif defined(__DOS_COMPAT__)
/* >> read(2)
 * Read data from a given file descriptor `FD' and return the number of bytes read.
 * A return value of ZERO(0) is indicative of EOF. */
__REDIRECT(__LIBC,__WUNUSED_SUGGESTED __NONNULL((2)),__INT32_TYPE__,__LIBCCALL,__read32,(__fd_t __fd, void *__dstbuf, __UINT32_TYPE__ __n_bytes),_read,(__fd,__dstbuf,__n_bytes))
__LOCAL __WUNUSED_SUGGESTED __NONNULL((2)) ssize_t (__LIBCCALL read)(__fd_t __fd, void *__dstbuf, size_t __n_bytes) { return __read32(__fd,__dstbuf,__n_bytes > (size_t)(__UINT32_TYPE__)-1 ? (__UINT32_TYPE__)-1 : (__UINT32_TYPE__)__n_bytes); }
#else
/* >> read(2)
 * Read data from a given file descriptor `FD' and return the number of bytes read.
 * A return value of ZERO(0) is indicative of EOF. */
__REDIRECT_EXCEPT(__LIBC,__WUNUSED_SUGGESTED __NONNULL((2)),__EXCEPT_SELECT(size_t,ssize_t),__LIBCCALL,
                  read,(__fd_t __fd, void *__dstbuf, size_t __dstbufsize),(__fd,__dstbuf,__dstbufsize))
#endif
#endif /* !__read_defined */

#ifndef __write_defined
#define __write_defined 1
#if __SIZEOF_SIZE_T__ <= 4
/* >> write(2)
 * Write data to a given file descriptor `FD' and return the number of bytes written. */
__REDIRECT_EXCEPT_DPA(__LIBC,__NONNULL((2)),__EXCEPT_SELECT(size_t,ssize_t),__LIBCCALL,
                      write,(__fd_t __fd, void *__buf, size_t __bufsize),(__fd,__buf,__bufsize))
#elif defined(__DOS_COMPAT__)
/* >> write(2)
 * Write data to a given file descriptor `FD' and return the number of bytes written. */
__REDIRECT(__LIBC,__NONNULL((2)),__INT32_TYPE__,__LIBCCALL,__write32,(__fd_t __fd, void *__buf, __UINT32_TYPE__ __n_bytes),_write,(__fd,__buf,__n_bytes))
__LOCAL __NONNULL((2)) ssize_t (__LIBCCALL write)(__fd_t __fd, void *__buf, size_t __n_bytes) { return __write32(__fd,__buf,__n_bytes > (size_t)(__UINT32_TYPE__)-1 ? (__UINT32_TYPE__)-1 : (__UINT32_TYPE__)__n_bytes); }
#else
/* >> write(2)
 * Write data to a given file descriptor `FD' and return the number of bytes written. */
__REDIRECT_EXCEPT(__LIBC,__NONNULL((2)),__EXCEPT_SELECT(size_t,ssize_t),__LIBCCALL,
                  write,(__fd_t __fd, void *__buf, size_t __bufsize),(__fd,__buf,__bufsize))
#endif
#endif /* !__write_defined */

#ifndef __lseek_defined
#define __lseek_defined 1
#ifdef __DOS_COMPAT__
#ifdef __USE_FILE_OFFSET64
/* >> lseek(2)
 * Change the position of the file read/write pointer within a file referred to by `FD'. */
__REDIRECT(__LIBC,,__off64_t,__LIBCCALL,lseek,(__fd_t __fd, __off64_t __offset, int __whence),_lseeki64,(__fd,__offset,__whence))
#else /* __USE_FILE_OFFSET64 */
/* >> lseek(2)
 * Change the position of the file read/write pointer within a file referred to by `FD'. */
__REDIRECT_DPA(__LIBC,,__off32_t,__LIBCCALL,lseek,(__fd_t __fd, __off32_t __offset, int __whence),(__fd,__offset,__whence))
#endif /* !__USE_FILE_OFFSET64 */
#else /* __DOS_COMPAT__ */
/* >> lseek(2)
 * Change the position of the file read/write pointer within a file referred to by `FD'. */
__REDIRECT_EXCEPT_FS64(__LIBC,,__EXCEPT_SELECT(__FS_TYPE(pos),__FS_TYPE(off)),__LIBCCALL,lseek,(__fd_t __fd, __FS_TYPE(off) __offset, int __whence),(__fd,__offset,__whence))
#endif /* !__DOS_COMPAT__ */
#endif /* !__lseek_defined */

#ifndef __isatty_defined
#define __isatty_defined 1
/* >> isatty(2)
 * @return: 1: Is a tty
 * @return: 0: Not a tty
 * Check if the given file handle `FD' refers to a TTY. */
__REDIRECT_DPA(__LIBC,__WUNUSED,int,__LIBCCALL,isatty,(__fd_t __fd),(__fd))
#endif /* !__isatty_defined */

#ifndef __dup2_defined
#define __dup2_defined 1
/* >> dup2(2)
 * @return: NEWFD: Returns the new handle upon success.
 * Duplicate a file referred to by `OLDFD' into `NEWFD' */
__REDIRECT_EXCEPT_DPA(__LIBC,,__fd_t,__LIBCCALL,dup2,(__fd_t __oldfd, __fd_t __newfd),(__oldfd,__newfd))
#endif /* !__dup2_defined */

#ifndef __dup_defined
#define __dup_defined 1
/* >> dup(2)
 * @return: * : Returns the new handle upon success.
 * Duplicate a file referred to by `FD' and return its duplicated handle number. */
__REDIRECT_EXCEPT_DPA(__LIBC,__WUNUSED,__fd_t,__LIBCCALL,dup,(__fd_t __fd),(__fd))
#endif /* !__dup_defined */

#ifndef __close_defined
#define __close_defined 1
/* >> close(2)
 * Close a file handle. */
__REDIRECT_DPA(__LIBC,__CLEANUP,int,__LIBCCALL,close,(__fd_t __fd),(__fd))
#endif /* !__close_defined */

#ifndef __access_defined
#define __access_defined 1
/* >> access(2)
 * @param: TYPE: Set of `X_OK|W_OK|R_OK'
 * Test for access to the specified file `NAME', testing for `TYPE' */
__REDIRECT_EXCEPT_UFSDPA_XVOID(__LIBC,__IF_NUSE_EXCEPT(__WUNUSED) __NONNULL((1)),int,__LIBCCALL,access,(char const *__name, int __type),(__name,__type))
#endif /* !__access_defined */

#ifndef __chdir_defined
#define __chdir_defined 1
/* >> chdir(2)
 * Change the current working directory to `PATH' */
__REDIRECT_EXCEPT_UFSDPA_XVOID(__LIBC,__NONNULL((1)),int,__LIBCCALL,chdir,(char const *__path),(__path))
#endif /* !__chdir_defined */

#ifndef __getcwd_defined
#define __getcwd_defined 1
/* >> getcwd(2)
 * Return the path of the current working directory, relative to the filesystem root set by `chdir(2)' */
__REDIRECT_EXCEPT_UFSDPA(__LIBC,__XATTR_RETNONNULL __WUNUSED_SUGGESTED,char *,__LIBCCALL,getcwd,(char *__buf, size_t __size),(__buf,__size))
#endif /* !__getcwd_defined */

#ifndef __unlink_defined
#define __unlink_defined 1
/* >> unlink(2)
 * Remove a file, symbolic link, device or FIFO referred to by `NAME' */
__REDIRECT_EXCEPT_UFSDPA_XVOID(__LIBC,__NONNULL((1)),int,__LIBCCALL,unlink,(char const *__name),(__name))
#endif /* !__unlink_defined */

#ifndef __rmdir_defined
#define __rmdir_defined 1
/* >> rmdir(2)
 * Remove a directory referred to by `PATH' */
__REDIRECT_EXCEPT_UFSDPA_XVOID(__LIBC,__NONNULL((1)),int,__LIBCCALL,rmdir,(char const *__path),(__path))
#endif /* !__rmdir_defined */

#ifdef __USE_GNU
#ifdef __DOS_COMPAT__
/* >> euidaccess(2), eaccess(2)
 * @param: TYPE: Set of `X_OK|W_OK|R_OK'
 * Test for access to the specified file `NAME', testing for `TYPE', using the effective filesystem ids */
__REDIRECT(__LIBC,__WUNUSED __NONNULL((1)),int,__LIBCCALL,euidaccess,(char const *__name, int __type),_access,(__name,__type))
__REDIRECT(__LIBC,__WUNUSED __NONNULL((1)),int,__LIBCCALL,eaccess,(char const *__name, int __type),_access,(__name,__type))
#else /* __DOS_COMPAT__ */
/* >> euidaccess(2), eaccess(2)
 * @param: TYPE: Set of `X_OK|W_OK|R_OK'
 * Test for access to the specified file `NAME', testing for `TYPE', using the effective filesystem ids */
__REDIRECT_EXCEPT_UFS_XVOID_(__LIBC,__IF_NUSE_EXCEPT(__WUNUSED) __NONNULL((1)),int,__LIBCCALL,euidaccess,(char const *__name, int __type),eaccess,(__name,__type))
__REDIRECT_EXCEPT_UFS_XVOID(__LIBC,__IF_NUSE_EXCEPT(__WUNUSED) __NONNULL((1)),int,__LIBCCALL,eaccess,(char const *__name, int __type),(__name,__type))
#endif /* !__DOS_COMPAT__ */
#endif /* __USE_GNU */

#ifdef __USE_ATFILE
#ifdef __CRT_GLC
/* >> faccessat(2)
 * @param: TYPE: Set of `X_OK|W_OK|R_OK'
 * Test for access to the specified file `DFD:FILE', testing for `TYPE' */
__REDIRECT_EXCEPT_UFS_XVOID(__LIBC,__PORT_NODOS_ALT(access) __NONNULL((2)),int,__LIBCCALL,faccessat,(__fd_t __dfd, char const *__file, int __type, __atflag_t __flags),(__dfd,__file,__type,__flag))

/* >> fchownat(2)
 * Change the ownership of a given `DFD:FILE' to `GROUP:OWNER'. */
__REDIRECT_EXCEPT_UFS_XVOID(__LIBC,__PORT_NODOS __NONNULL((2)),int,__LIBCCALL,fchownat,(__fd_t __dfd, char const *__file, __uid_t __owner, __gid_t __group, __atflag_t __flags),(__dfd,__file,__owner,__group))

/* >> linkat(2)
 * Create a hard link from `FROMFD:FROM', leading to `TOFD:TO' */
__REDIRECT_EXCEPT_UFS_XVOID(__LIBC,__PORT_NODOS __NONNULL((2,4)),int,__LIBCCALL,linkat,(__fd_t __fromfd, char const *__from, __fd_t __tofd, char const *__to, __atflag_t __flags),(__fromfd,__from,__tofd,__to,__flags))

/* >> symlink(3)
 * Create a new symbolic link loaded with `FROM' as link
 * text, at the filesystem location referred to by `TOFD:TO'. */
__REDIRECT_EXCEPT_UFS_XVOID(__LIBC,__PORT_NODOS __NONNULL((1,3)),int,__LIBCCALL,symlinkat,(char const *__from, __fd_t __tofd, char const *__to),(__from,__tofd,__to))

/* >> readlinkat(3)
 * Read the text of a symbolic link under `DFD:PATH' into the provided buffer.
 * WARNING: This function is badly designed and will neither append a trailing
 *          NUL-character to the buffer, nor will it return the required buffer
 *          size. Instead, it will return the written size, and the caller must
 *          keep on over allocating until the function indicates that it didn't
 *          make use of the buffer in its entirety.
 * When targeting KOS, consider using `freadlinkat(2)' with `AT_READLINK_REQSIZE'. */
__REDIRECT_EXCEPT_UFS(__LIBC,__PORT_NODOS __NONNULL((2,3)),__EXCEPT_SELECT(size_t,ssize_t),__LIBCCALL,readlinkat,(__fd_t __dfd, char const *__restrict __path, char *__restrict __buf, size_t __buflen),(__dfd,__path,__buf,__buflen))

/* >> unlinkat(2)
 * Remove a file, symbolic link, device or FIFO referred to by `DFD:NAME' */
__REDIRECT_EXCEPT_UFS_XVOID(__LIBC,__PORT_NODOS_ALT(unlink) __NONNULL((2)),int,__LIBCCALL,unlinkat,(__fd_t __dfd, char const *__name, __atflag_t __flags),(__dfd,__name,__flag))
#endif /* __CRT_GLC */
#endif /* __USE_ATFILE */

#ifdef __USE_LARGEFILE64
#ifdef __DOS_COMPAT__
/* >> lseek64(2)
 * Change the position of the file read/write pointer within a file referred to by `FD'. */
__REDIRECT(__LIBC,,__off64_t,__LIBCCALL,lseek64,(__fd_t __fd, __off64_t __offset, int __whence),_lseeki64,(__fd,__offset,__whence))
#else /* __DOS_COMPAT__ */
/* >> lseek64(2)
 * Change the position of the file read/write pointer within a file referred to by `FD'. */
__REDIRECT_EXCEPT(__LIBC,,__EXCEPT_SELECT(__pos64_t,__off64_t),__LIBCCALL,lseek64,(__fd_t __fd, __off64_t __offset, int __whence),(__fd,__offset,__whence))
#endif /* !__DOS_COMPAT__ */
#endif /* __USE_LARGEFILE64 */

#if defined(__USE_UNIX98) || defined(__USE_XOPEN2K8)
#ifdef __USE_KOS
#define __PIO_OFFSET     __FS_TYPE(off)
#define __PIO_OFFSET64   __off64_t
#else
#define __PIO_OFFSET     __FS_TYPE(pos)
#define __PIO_OFFSET64   __pos64_t
#endif
#ifdef __DOS_COMPAT__
/* It may not be quick, and it may not be SMP-safe, but it'll still do the job! */
#define __DEFINE_PREADWRITE(off_t,lseek,readwrite) \
{ ssize_t __result; off_t __oldpos; \
  if ((__oldpos = lseek(__fd,0,SEEK_CUR)) < 0) return -1; \
  if (lseek(__fd,__offset,SEEK_SET) < 0) return -1; \
  __result = readwrite(__fd,__buf,__n_bytes); \
  lseek(__fd,__oldpos,SEEK_SET); \
  return __result; \
}
/* >> pread(2), pwrite(2)
 * Read/write data to/from a file at a specific offset. */
__LOCAL __WUNUSED_SUGGESTED ssize_t (__LIBCCALL pread)(__fd_t __fd, void *__buf, size_t __n_bytes, __PIO_OFFSET __offset) __DEFINE_PREADWRITE(__FS_TYPE(off),lseek,read)
__LOCAL ssize_t (__LIBCCALL pwrite)(__fd_t __fd, void const *__buf, size_t __n_bytes, __PIO_OFFSET __offset) __DEFINE_PREADWRITE(__FS_TYPE(off),lseek,write)

#ifdef __USE_LARGEFILE64
/* >> pread(2), pwrite(2)
 * Read/write data to/from a file at a specific offset. */
__LOCAL __WUNUSED_SUGGESTED ssize_t (__LIBCCALL pread64)(__fd_t __fd, void *__buf, size_t __n_bytes, __PIO_OFFSET64 __offset) __DEFINE_PREADWRITE(__off64_t,lseek64,read)
__LOCAL ssize_t (__LIBCCALL pwrite64)(__fd_t __fd, void const *__buf, size_t __n_bytes, __PIO_OFFSET64 __offset) __DEFINE_PREADWRITE(__off64_t,lseek64,write)
#endif /* __USE_LARGEFILE64 */
#undef __DEFINE_PREADWRITE
#else /* __DOS_COMPAT__ */
/* >> pread(2), pwrite(2)
 * Read/write data to/from a file at a specific offset. */
__REDIRECT_EXCEPT_FS64(__LIBC,__WUNUSED_SUGGESTED,__EXCEPT_SELECT(size_t,ssize_t),__LIBCCALL,pread,(__fd_t __fd, void *__buf, size_t __n_bytes, __PIO_OFFSET __offset),(__fd,__buf,__n_bytes,__offset))
__REDIRECT_EXCEPT_FS64(__LIBC,,__EXCEPT_SELECT(size_t,ssize_t),__LIBCCALL,pwrite,(__fd_t __fd, void const *__buf, size_t __n_bytes, __PIO_OFFSET __offset),(__fd,__buf,__n_bytes,__offset))

#ifdef __USE_LARGEFILE64
/* >> pread(2), pwrite(2)
 * Read/write data to/from a file at a specific offset. */
__REDIRECT_EXCEPT(__LIBC,__WUNUSED_SUGGESTED,__EXCEPT_SELECT(size_t,ssize_t),__LIBCCALL,pread64,(__fd_t __fd, void *__buf, size_t __n_bytes, __PIO_OFFSET64 __offset),(__fd,__buf,__n_bytes,__offset))
__REDIRECT_EXCEPT(__LIBC,,__EXCEPT_SELECT(size_t,ssize_t),__LIBCCALL,pwrite64,(__fd_t __fd, void const *__buf, size_t __n_bytes, __PIO_OFFSET64 __offset),(__fd,__buf,__n_bytes,__offset))
#endif /* __USE_LARGEFILE64 */
#endif /* !__DOS_COMPAT__ */
#undef __PIO_OFFSET64
#undef __PIO_OFFSET
#endif /* __USE_UNIX98 || __USE_XOPEN2K8 */

#ifdef __USE_GNU
#ifndef __environ_defined
#define __environ_defined 1
#undef environ
#ifdef __DOS_COMPAT__
/* Try to maintain binary compatibility with DOS.
 * Note though, that LIBC exports `environ' and `__p__environ' in DOS and KOS mode. */
__LIBC __WUNUSED char ***__NOTHROW((__LIBCCALL __p__environ)(void));
#define environ  (*__p__environ())
#else /* __DOS_COMPAT__ */
#ifdef __PE__
__LIBC char **environ __ASMNAME("_environ");
#else /* __PE__ */
__LIBC char **environ;
#endif /* !__PE__ */
#if defined(__PE__) || defined(__USE_KOS)
/* Always defining `environ' as a macro is a KOS extension.
 * Otherwise, we mimic PE behavior which always defines `environ' as a macro. */
#define environ  environ
#endif /* __PE__ || __USE_KOS */
#endif /* !__DOS_COMPAT__ */
#endif /* !__environ_defined */

#ifdef __DOS_COMPAT__
__LOCAL int (__LIBCCALL pipe2)(__fd_t __pipedes[2], __oflag_t __flags) { return __dos_pipe(__pipedes,4096,0x8000|__flags); }
__LOCAL __fd_t (__LIBCCALL dup3)(__fd_t __oldfd, __fd_t __newfd, __oflag_t __UNUSED(__flags)) { return __newfd != __oldfd ? dup2(__oldfd,__newfd) : -1; }
__LOCAL __WUNUSED char *(__LIBCCALL get_current_dir_name)(void) { return getcwd(NULL,0); }
__LOCAL int (__LIBCCALL syncfs)(__fd_t __UNUSED(__fd)) {}
#else /* __DOS_COMPAT__ */
__REDIRECT_EXCEPT_XVOID(__LIBC,,int,__LIBCCALL,pipe2,(__fd_t __pipedes[2], __oflag_t __flags),(__pipedes,__flags))
__REDIRECT_EXCEPT(__LIBC,,__fd_t,__LIBCCALL,dup3,(__fd_t __oldfd, __fd_t __newfd, __oflag_t __flags),(__oldfd,__newfd,__flags))
__REDIRECT_EXCEPT_UFS(__LIBC,__WUNUSED,char *,__LIBCCALL,get_current_dir_name,(void),())
__REDIRECT_EXCEPT_XVOID(__LIBC,,int,__LIBCCALL,syncfs,(__fd_t __fd),(__fd))
#endif /* !__DOS_COMPAT__ */
#ifdef __CRT_GLC
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,group_member,(__gid_t __gid),(__gid))
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,getresuid,(__uid_t *__ruid, __uid_t *__euid, __uid_t *__suid),(__ruid,__euid,__suid))
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,getresgid,(__gid_t *__rgid, __gid_t *__egid, __gid_t *__sgid),(__rgid,__egid,__sgid))
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,setresuid,(__uid_t __ruid, __uid_t __euid, __uid_t __suid),(__ruid,__euid,__suid))
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,setresgid,(__gid_t __rgid, __gid_t __egid, __gid_t __sgid),(__rgid,__egid,__sgid))
#endif /* __CRT_GLC */
#endif /* __USE_GNU */

#if (defined(__USE_XOPEN_EXTENDED) && !defined(__USE_XOPEN2K8)) || \
     defined(__USE_MISC)
#ifdef __DOS_COMPAT__
/* Hidden function exported by DOS that allows for millisecond precision. */
#ifndef ____dos_sleep_mili_defined
#define ____dos_sleep_mili_defined 1
__REDIRECT_VOID(__LIBC,,__LIBCCALL,__dos_sleep_mili,(__UINT32_TYPE__ __msecs),__crtSleep,(__msecs))
#endif /* !____dos_sleep_mili_defined */
__LOCAL int (__LIBCCALL usleep)(__useconds_t __useconds) { return __dos_sleep_mili(__useconds/1000l /*USEC_PER_MSEC*/); }
__LOCAL __NONNULL((1)) __ATTR_DEPRECATED("Use getcwd()") char *(__LIBCCALL getwd)(char *__buf) { return getcwd(__buf,(size_t)-1); }
#else /* __DOS_COMPAT__ */
__LIBC int (__LIBCCALL usleep)(__useconds_t __useconds);
__REDIRECT_UFS(__LIBC,__NONNULL((1)) __ATTR_DEPRECATED("Use getcwd()"),char *,__LIBCCALL,getwd,(char *__buf),(__buf))
#endif /* !__DOS_COMPAT__ */
#ifdef __CRT_GLC
__LIBC __PORT_NODOS __useconds_t (__LIBCCALL ualarm)(__useconds_t __value, __useconds_t __interval);

#if !defined(__NO_ASMNAME) || !defined(__USE_EXCEPT_API)
/* >> vfork(2)
 * Same as `fork(2)', but suspend the calling process until the child
 * process either calls `exit(2)' or one of the many `exec(2)' functions. */
__REDIRECT_EXCEPT(__LIBC,__ATTR_RETURNS_TWICE __PORT_NODOS __WUNUSED,__pid_t,__LIBCCALL,vfork,(void),())
#else
#ifndef __Xvfork_defined
#define __Xvfork_defined 1
/* >> vfork(2)
 * Same as `fork(2)', but suspend the calling process until the child
 * process either calls `exit(2)' or one of the many `exec(2)' functions. */
__LIBC __ATTR_RETURNS_TWICE __PORT_NODOS __WUNUSED __pid_t (__LIBCCALL Xvfork)(void);
#endif /* !__Xvfork_defined */

/* >> vfork(2)
 * Same as `fork(2)', but suspend the calling process until the child
 * process either calls `exit(2)' or one of the many `exec(2)' functions. */
#define vfork()     Xvfork()
#endif
#endif /* __CRT_GLC */
#endif

#if defined(__USE_XOPEN_EXTENDED) || defined(__USE_XOPEN2K8)
#ifdef __USE_KOS
#define __TRUNCATE_OFF_T    __FS_TYPE(pos)
#define __TRUNCATE_OFF64_T  __pos64_t
#else
#define __TRUNCATE_OFF_T    __FS_TYPE(off)
#define __TRUNCATE_OFF64_T  __off64_t
#endif
#ifdef __CRT_GLC
/* >> fchown(2)
 * Change the ownership of a given `FD' to `GROUP:OWNER'. */
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,fchown,(__fd_t __fd, __uid_t __owner, __gid_t __group),(__fd,__owner,__group))

/* >> chdir(2)
 * Change the current working directory to `PATH' */
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS_ALT(chdir),int,__LIBCCALL,fchdir,(__fd_t __fd),(__fd))

/* >> getpgid(2)
 * Return the ID of the process group associated with `PID's process.
 * (That is the TID of the leader of the process group of `PID's leader)
 * THREAD[PID]->LEADER->GROUP_LEADER->PID
 * When `PID' is ZERO(0), use `gettid()' for it instead. */
__REDIRECT_EXCEPT(__LIBC,__PORT_NODOS __WUNUSED,__pid_t,__LIBCCALL,getpgid,(__pid_t __pid),(__pid))

/* >> getsid(2)
 * Return the ID of the session which a process `PID' is apart of.
 * return THREAD[PID]->LEADER->GROUP_LEADER->SESSION_LEADER->PID; */
__REDIRECT_EXCEPT(__LIBC,__PORT_NODOS __WUNUSED,__pid_t,__LIBCCALL,getsid,(__pid_t __pid),(__pid))

/* >> lchown(2)
 * Change the ownership of a given `FILE' to `GROUP:OWNER',
 * but don't reference it if that file is a symbolic link. */
__REDIRECT_EXCEPT_UFS_XVOID(__LIBC,__PORT_NODOS __NONNULL((1)),int,__LIBCCALL,lchown,(char const *__file, __uid_t __owner, __gid_t __group),(__file,__owner,__group))

/* >> truncate(2), truncate64(2)
 * Truncate the given file `FILE' to a length of `LENGTH' */
__REDIRECT_EXCEPT_UFS64_XVOID(__LIBC,__NONNULL((1)),int,__LIBCCALL,truncate,(char const *__file, __TRUNCATE_OFF_T __length),(__file,__length))

#ifdef __USE_LARGEFILE64
/* >> truncate(2), truncate64(2)
 * Truncate the given file `FILE' to a length of `LENGTH' */
__REDIRECT_EXCEPT_UFS_XVOID(__LIBC,__NONNULL((1)),int,__LIBCCALL,truncate64,(char const *__file, __TRUNCATE_OFF64_T __length),(__file,__length))
#endif /* __USE_LARGEFILE64 */

#else /* __CRT_GLC */
/* >> truncate(2), truncate64(2)
 * Truncate the given file `FILE' to a length of `LENGTH' */
__LOCAL __NONNULL((1)) int (__LIBCCALL truncate)(char const *__UNUSED(__file), __TRUNCATE_OFF_T __UNUSED(__length)) { return 0; /* TODO: open()+ftruncate(); */ }

#ifdef __USE_LARGEFILE64
/* >> truncate(2), truncate64(2)
 * Truncate the given file `FILE' to a length of `LENGTH' */
__LOCAL __NONNULL((1)) int (__LIBCCALL truncate64)(char const *__UNUSED(__file), __TRUNCATE_OFF64_T __UNUSED(__length)) { return 0; /* TODO: open()+ftruncate64(); */ }
#endif /* __USE_LARGEFILE64 */
#endif /* !__CRT_GLC */
#undef __TRUNCATE_OFF_T
#undef __TRUNCATE_OFF64_T
#endif /* __USE_XOPEN_EXTENDED || __USE_XOPEN2K8 */

#ifdef __USE_XOPEN2K8
#ifdef __CRT_GLC
#ifndef __fexecve_defined
#define __fexecve_defined 1

/* >> fexecve(2)
 * Replace the calling process with the application image referred to by `FD' and
 * execute it's `main()' method, passing the given `ARGV', and setting `environ' to `ENVP' */
__REDIRECT_EXCEPT_XVOID(__LIBC,__XATTR_NORETURN __PORT_NODOS_ALT(execve) __NONNULL((2,3)),int,__LIBCCALL,fexecve,(__fd_t __fd, __TARGV, __TENVP),(__fd,___argv,___envp))

#ifdef __USE_EXCEPT
/* >> fexecve(2)
 * Replace the calling process with the application image referred to by `FD' and
 * execute it's `main()' method, passing the given `ARGV', and setting `environ' to `ENVP' */
__LIBC __PORT_KOSONLY __ATTR_NORETURN __NONNULL((2,3)) void (__LIBCCALL Xfexecve)(__fd_t __fd, __TARGV, __TENVP);
#endif /* __USE_EXCEPT */
#endif /* !__fexecve_defined */
#endif /* __CRT_GLC */
#endif /* __USE_XOPEN2K8 */

#ifdef __USE_GNU
#ifndef __execvpe_defined
#define __execvpe_defined 1

/* >> execvpe(3)
 * Replace the calling process with the application image referred to by `FILE'
 * and execute it's `main()' method, passing the given `ARGV', and setting `environ' to `ENVP' */
__REDIRECT_EXCEPT_UFSDPA_XVOID(__LIBC,__XATTR_NORETURN __NONNULL((1,2,3)),int,__LIBCCALL,execvpe,(char const *__file, __TARGV, __TENVP),(__file,___argv,___envp))

#ifdef __USE_EXCEPT
/* >> execvpe(3)
 * Replace the calling process with the application image referred to by `FILE'
 * and execute it's `main()' method, passing the given `ARGV', and setting `environ' to `ENVP' */
__LIBC __PORT_KOSONLY __ATTR_NORETURN __NONNULL((1,2,3)) void (__LIBCCALL Xexecvpe)(char const *__file, __TARGV, __TENVP);
#endif /* __USE_EXCEPT */
#endif /* !__execvpe_defined */
#endif /* __USE_GNU */

#if defined(__USE_MISC) || defined(__USE_XOPEN)
#ifdef __DOS_COMPAT__
/* It should be sufficient to emulate this is a no-op. */
__LOCAL __WUNUSED int (__LIBCCALL nice)(int __UNUSED(__inc)) { return 0; }
#else /* __DOS_COMPAT__ */
__REDIRECT_EXCEPT(__LIBC,__WUNUSED,int,__LIBCCALL,nice,(int __inc),(__inc))
#endif /* !__DOS_COMPAT__ */
#endif /* __USE_MISC || __USE_XOPEN */

#ifdef __USE_POSIX2
#ifdef __CRT_GLC
__REDIRECT_EXCEPT(__LIBC,__PORT_NODOS __WUNUSED_SUGGESTED,size_t,__LIBCCALL,
                  confstr,(int __name, char *__buf, size_t __buflen),(__name,__buf,__buflen))

#ifdef __COMPILER_HAVE_PRAGMA_PUSHMACRO
#pragma push_macro("optarg")
#pragma push_macro("optind")
#pragma push_macro("opterr")
#pragma push_macro("optopt")
#endif

#undef optarg
#undef optind
#undef opterr
#undef optopt
__LIBC char *optarg;
__LIBC int optind;
__LIBC int opterr;
__LIBC int optopt;

#ifdef __COMPILER_HAVE_PRAGMA_PUSHMACRO
#pragma pop_macro("optopt")
#pragma pop_macro("opterr")
#pragma pop_macro("optind")
#pragma pop_macro("optarg")
#endif

#ifndef __getopt_defined
#define __getopt_defined 1
#if defined(__USE_POSIX2) && !defined(__USE_POSIX_IMPLICITLY) && !defined(__USE_GNU)
__REDIRECT(__LIBC,__PORT_NODOS __WUNUSED,int,__LIBCCALL,getopt,
          (int ___argc, char *const *___argv, char const *__shortopts),
           __posix_getopt,(___argc,___argv,__shortopts))
#else /* ... */
__LIBC __PORT_NODOS int (__LIBCCALL getopt)(int ___argc, char *const *___argv, char const *__shortopts);
#endif /* !... */
#endif /* !__getopt_defined */
#endif /* __CRT_GLC */
#endif /* __USE_POSIX2 */

#if defined(__USE_MISC) || defined(__USE_XOPEN_EXTENDED)
#ifdef __DOS_COMPAT__
__LOCAL void (__LIBCCALL sync)(void) {}
#if defined(__USE_MISC) || !defined(__USE_XOPEN2K)
#if defined(__PAGESIZE)
__LOCAL __WUNUSED __ATTR_CONST int (__LIBCCALL getpagesize)(void) { return __PAGESIZE; }
#elif defined(PAGE_SIZE)
__LOCAL __WUNUSED __ATTR_CONST int (__LIBCCALL getpagesize)(void) { return PAGE_SIZE; }
#elif defined(PAGESIZE)
__LOCAL __WUNUSED __ATTR_CONST int (__LIBCCALL getpagesize)(void) { return PAGESIZE; }
#elif defined(__i386__) || defined(__x86_64__)
__LOCAL __WUNUSED __ATTR_CONST int (__LIBCCALL getpagesize)(void) { return 4096; }
#else
#error "ERROR: PAGESIZE not known for this arch"
#endif
#endif /* __USE_MISC || !__USE_XOPEN2K */
#else /* __DOS_COMPAT__ */

/* >> sync(2)
 * Synchronize all disk operations of all mounted file systems and flush
 * unwritten buffers down to the hardware layer, ensuring that modifications
 * made become visible on the underlying, persistent media. */
__LIBC void (__LIBCCALL sync)(void);

/* >> setpgrp(3)
 * Move the calling process into its own process group.
 * Equivalent to `setpgid(0,0)' */
__REDIRECT_EXCEPT(__LIBC,__PORT_NODOS,__pid_t,__LIBCCALL,setpgrp,(void),())

/* ... */
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,setreuid,(__uid_t __ruid, __uid_t __euid),(__ruid,__euid))

/* ... */
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,setregid,(__gid_t __rgid, __gid_t __egid),(__rgid,__egid))

/* ... */
__REDIRECT_EXCEPT(__LIBC,__PORT_NODOS __WUNUSED,long int,__LIBCCALL,gethostid,(void),())

#if defined(__USE_MISC) || !defined(__USE_XOPEN2K)
/* >> getpagesize(3)
 * Return the size of a PAGE (in bytes) */
__LIBC __ATTR_CONST __WUNUSED int (__LIBCCALL getpagesize)(void) ;

/* ... */
__LIBC __PORT_NODOS __WUNUSED int (__LIBCCALL getdtablesize)(void);
#endif /* __USE_MISC || !__USE_XOPEN2K */
#endif /* !__DOS_COMPAT__ */
#endif /* __USE_MISC || __USE_XOPEN_EXTENDED */

#ifdef __CRT_GLC
#ifdef __USE_XOPEN2K
/* ... */
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,seteuid,(__uid_t __uid),(__uid))

/* ... */
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,setegid,(__gid_t __gid),(__gid))
#endif /* __USE_XOPEN2K */

#if defined(__USE_MISC) || \
   (defined(__USE_XOPEN_EXTENDED) && !defined(__USE_UNIX98))
/* ... */
__REDIRECT_EXCEPT(__LIBC,__PORT_NODOS __WUNUSED,int,__LIBCCALL,ttyslot,(void),())
#endif

#if defined(__USE_XOPEN_EXTENDED) || defined(__USE_XOPEN2K)
/* >> symlink(3)
 * Create a new symbolic link loaded with `FROM' as link
 * text, at the filesystem location referred to by `TO'.
 * Same as `symlinkat(FROM,AT_FDCWD,TO)' */
__REDIRECT_EXCEPT_UFS(__LIBC,__PORT_NODOS __NONNULL((1,2)),int,__LIBCCALL,symlink,(char const *__from, char const *__to),(__from,__to))

/* >> readlink(3)
 * Read the text of a symbolic link under `PATH' into the provided buffer.
 * Same as `readlinkat(AT_FDCWD,PATH,BUF,BUFLEN)'
 * WARNING: This function is badly designed and will neither append a trailing
 *          NUL-character to the buffer, nor will it return the required buffer
 *          size. Instead, it will return the written size, and the caller must
 *          keep on over allocating until the function indicates that it didn't
 *          make use of the buffer in its entirety.
 * When targeting KOS, consider using `freadlinkat(2)' with `AT_READLINK_REQSIZE'. */
__REDIRECT_EXCEPT_UFS(__LIBC,__PORT_NODOS __NONNULL((1,2)),__EXCEPT_SELECT(size_t,ssize_t),__LIBCCALL,readlink,(char const *__restrict __path, char *__restrict __buf, size_t __buflen),(__path,__buf,__buflen))
#endif /* __USE_XOPEN_EXTENDED || __USE_XOPEN2K */

#if defined(__USE_REENTRANT) || defined(__USE_POSIX199506)
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS __WUNUSED_SUGGESTED __NONNULL((1)),int,__LIBCCALL,getlogin_r,(char *__name, size_t __name_len),(__name,__name_len))
#endif /* __USE_REENTRANT || __USE_POSIX199506 */

#if defined(__USE_UNIX98) || defined(__USE_XOPEN2K)
/* >> gethostname(3)
 * Return the name assigned to the hosting machine, as set by `sethostname(2)' */
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS __WUNUSED_SUGGESTED __NONNULL((1)),int,__LIBCCALL,gethostname,(char *__name, size_t __buflen),(__name,__buflen))
#endif /* __USE_UNIX98 || __USE_XOPEN2K */

#ifdef __USE_MISC
/* ... */
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS __NONNULL((1)),int,__LIBCCALL,setlogin,(char const *__name),(__name))

/* >> sethostname(2)
 * Set the name of the hosting machine */
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS __NONNULL((1)),int,__LIBCCALL,sethostname,(char const *__name, size_t __len),(__name,__len))

/* ... */
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,sethostid,(long int __id),(__id))

/* >> getdomainname(3)
 * Return the name assigned to the hosting machine's domain, as set by `setdomainname(2)' */
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS __WUNUSED_SUGGESTED __NONNULL((1)),int,__LIBCCALL,getdomainname,(char *__name, size_t __buflen),(__name,__buflen))

/* >> setdomainname(2)
 * Set the name of the hosting machine's domain */
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS __NONNULL((1)),int,__LIBCCALL,setdomainname,(char const *__name, size_t __len),(__name,__len))

/* ... */
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,vhangup,(void),())

/* ... */
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS __NONNULL((1)),int,__LIBCCALL,profil,(unsigned short int *__sample_buffer, size_t __size, size_t __offset, unsigned int __scale),(__sample_buffer,__size,__offset,__scale))

/* ... */
__LIBC __PORT_NODOS __WUNUSED char *(__LIBCCALL getusershell)(void);

/* ... */
__LIBC __PORT_NODOS void (__LIBCCALL endusershell)(void);

/* ... */
__LIBC __PORT_NODOS void (__LIBCCALL setusershell)(void);

/* ... */
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,daemon,(int __nochdir, int __noclose),(__nochdir,__noclose))

/* ... */
__REDIRECT_EXCEPT_UFS_XVOID(__LIBC,__PORT_NODOS __NONNULL((1)),int,__LIBCCALL,revoke,(char const *__file),(__file))

/* ... */
__REDIRECT_EXCEPT_UFS_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,acct,(char const *__name),(__name))

#if defined(__USE_KOS) && defined(__CRT_KOS)
/* Execute a system call, returning in both EAX and EDX */
#ifdef __NO_ASMNAME
#ifdef __USE_EXCEPT_API
#ifndef __Xsyscall_defined
#define __Xsyscall_defined 1

/* >> syscall(2)
 * Execute a system call `SYSNO', passing arguments found in varargs during invocation. */
__LIBC __PORT_NODOS long long int (__ATTR_CDECL Xsyscall)(long int __sysno, ...);
#endif /* !__Xsyscall_defined */

/* >> syscall(2)
 * Execute a system call `SYSNO', passing arguments found in varargs during invocation. */
#define lsyscall(...)             (Xsyscall)(__VA_ARGS__)

/* >> syscall(2)
 * Execute a system call `SYSNO', passing arguments found in varargs during invocation. */
#define syscall(...)   ((long int)(Xsyscall)(__VA_ARGS__))
#else /* __USE_EXCEPT_API */

/* >> syscall(2)
 * Execute a system call `SYSNO', passing arguments found in varargs during invocation. */
__LIBC __PORT_NODOS long long int (__ATTR_CDECL syscall)(long int __sysno, ...);

/* >> syscall(2)
 * Execute a system call `SYSNO', passing arguments found in varargs during invocation. */
#define lsyscall(...)             (syscall)(__VA_ARGS__)

/* >> syscall(2)
 * Execute a system call `SYSNO', passing arguments found in varargs during invocation. */
#define syscall(...)   ((long int)(syscall)(__VA_ARGS__))
#endif /* !__USE_EXCEPT_API */
#elif defined(__USE_EXCEPT_API)

/* >> syscall(2)
 * Execute a system call `SYSNO', passing arguments found in varargs during invocation. */
__LIBC __PORT_NODOS long int (__ATTR_CDECL syscall)(long int __sysno, ...) __ASMNAME("Xsyscall");

/* >> syscall(2)
 * Execute a system call `SYSNO', passing arguments found in varargs during invocation. */
__LIBC __PORT_KOSONLY long long int (__ATTR_CDECL lsyscall)(long int __sysno, ...) __ASMNAME("Xsyscall");
#else

/* >> syscall(2)
 * Execute a system call `SYSNO', passing arguments found in varargs during invocation. */
__LIBC __PORT_NODOS long int (__ATTR_CDECL syscall)(long int __sysno, ...);

/* >> syscall(2)
 * Execute a system call `SYSNO', passing arguments found in varargs during invocation. */
__LIBC __PORT_KOSONLY long long int (__ATTR_CDECL lsyscall)(long int __sysno, ...) __ASMNAME("syscall");
#endif /* !__NO_ASMNAME */
#else /* __USE_KOS && __CRT_KOS */

/* >> syscall(2)
 * Execute a system call `SYSNO', passing arguments found in varargs during invocation. */
__LIBC __PORT_NODOS long int (__ATTR_CDECL syscall)(long int __sysno, ...);
#endif /* !__USE_KOS || !__CRT_KOS */
#endif /* __USE_MISC */

#if defined(__USE_MISC) || \
   (defined(__USE_XOPEN) && !defined(__USE_XOPEN2K))
/* >> chroot(2)
 * Change the root directory of the calling `CLONE_FS' group of threads
 * (usually the process) to a path that was previously address by `PATH'. */
__REDIRECT_EXCEPT_UFS_XVOID(__LIBC,__PORT_NODOS __NONNULL((1)),int,__LIBCCALL,chroot,(char const *__restrict __path),(__path))

/* ... */
__REDIRECT_EXCEPT(__LIBC,__PORT_NODOS __WUNUSED __NONNULL((1)),char *,__LIBCCALL,getpass,(char const *__restrict __prompt),(__prompt))
#endif
#endif /* __CRT_GLC */

#if defined(__USE_POSIX199309) || defined(__USE_XOPEN_EXTENDED) || defined(__USE_XOPEN2K)
#ifdef __DOS_COMPAT__
#ifdef __USE_FILE_OFFSET64
/* >> ftruncate(2), ftruncate64(2)
 * Truncate the given file `FD' to a length of `LENGTH' */
__REDIRECT(__LIBC,,int,__LIBCCALL,ftruncate,(__fd_t __fd, __FS_TYPE(off) __length),_chsize_s,(__fd,__length))
#else /* __USE_FILE_OFFSET64 */
/* >> ftruncate(2), ftruncate64(2)
 * Truncate the given file `FD' to a length of `LENGTH' */
__REDIRECT(__LIBC,,int,__LIBCCALL,ftruncate,(__fd_t __fd, __FS_TYPE(off) __length),_chsize,(__fd,__length))
#endif /* !__USE_FILE_OFFSET64 */

#ifdef __USE_LARGEFILE64
/* >> ftruncate(2), ftruncate64(2)
 * Truncate the given file `FD' to a length of `LENGTH' */
__REDIRECT(__LIBC,,int,__LIBCCALL,ftruncate64,(__fd_t __fd, __off64_t __length),_chsize_s,(__fd,__length))
#endif /* __USE_LARGEFILE64 */
#else /* __DOS_COMPAT__ */
/* >> ftruncate(2), ftruncate64(2)
 * Truncate the given file `FD' to a length of `LENGTH' */
__REDIRECT_EXCEPT_FS64_XVOID(__LIBC,,int,__LIBCCALL,ftruncate,(__fd_t __fd, __FS_TYPE(off) __length),(__fd,__length))

#ifdef __USE_LARGEFILE64
/* >> ftruncate(2), ftruncate64(2)
 * Truncate the given file `FD' to a length of `LENGTH' */
__REDIRECT_EXCEPT_XVOID(__LIBC,,int,__LIBCCALL,ftruncate64,(__fd_t __fd, __off64_t __length),(__fd,__length))
#endif /* __USE_LARGEFILE64 */
#endif /* !__DOS_COMPAT__ */
#endif /* __USE_POSIX199309 || __USE_XOPEN_EXTENDED || __USE_XOPEN2K */

#ifdef __CRT_GLC
#if (defined(__USE_XOPEN_EXTENDED) && !defined(__USE_XOPEN2K)) || \
     defined(__USE_MISC)
/* >> brk(2), sbrk(2)
 * Change the program break, allowing for a rudimentry implementation of a heap.
 * It is recommended to use the much more advanced functions found in <sys/mman.h> instead. */
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS_ALT(free),int,__LIBCCALL,brk,(void *__addr),(__addr))
__REDIRECT_EXCEPT(__LIBC,__PORT_NODOS_ALT(malloc),void *,__LIBCCALL,sbrk,(intptr_t __delta),(__delta))
#endif
#endif /* __CRT_GLC */

#if defined(__USE_POSIX199309) || defined(__USE_UNIX98)
#ifdef __DOS_COMPAT__
/* >> fdatasync(2)
 * Synchronize only the data of a file (not its descriptor which contains timestamps, and its size),
 * meaning that changes are written to disk. */
__REDIRECT(__LIBC,,int,__LIBCCALL,fdatasync,(__fd_t __fd),_commit,(__fd))
#else
/* >> fdatasync(2)
 * Synchronize only the data of a file (not its descriptor which contains timestamps, and its size),
 * meaning that changes are written to disk. */
__REDIRECT_EXCEPT_XVOID(__LIBC,,int,__LIBCCALL,fdatasync,(__fd_t __fd),(__fd))
#endif
#endif /* __USE_POSIX199309 || __USE_UNIX98 */

#ifdef __USE_XOPEN
#ifdef __CRT_GLC
/* ... */
__LIBC __PORT_NODOS __NONNULL((1,2)) char *(__LIBCCALL crypt)(char const *__key, char const *__salt);
/* ... */
__LIBC __PORT_NODOS __NONNULL((1)) void (__LIBCCALL encrypt)(char *__glibc_block, int __edflag);
#endif /* __CRT_GLC */

#ifndef __swab_defined
#define __swab_defined 1
#if __SIZEOF_INT__ == __SIZEOF_SIZE_T__ && !defined(__DOS_COMPAT__)
__LIBC __NONNULL((1,2)) void (__LIBCCALL swab)(void const *__restrict __from, void *__restrict __to, int __n_bytes);
#else /* __SIZEOF_INT__ == __SIZEOF_SIZE_T__ */
__REDIRECT_VOID(__LIBC,__NONNULL((1,2)),__LIBCCALL,swab,(void const *__restrict __from, void *__restrict __to, int __n_bytes),_swab,(__from,__to,__n_bytes))
#endif /* __SIZEOF_INT__ != __SIZEOF_SIZE_T__ */
#endif /* !__swab_defined */
#endif /* __USE_XOPEN */

#if defined(_ALL_SOURCE) || \
   (defined(__USE_XOPEN) && !defined(__USE_XOPEN2K))
#ifdef __CRT_GLC
#ifndef __ctermid_defined
#define __ctermid_defined 1
/* ... */
__LIBC __PORT_NODOS __WUNUSED_SUGGESTED char *(__LIBCCALL ctermid)(char *__s);
#endif /* !__ctermid_defined */
#endif /* __USE_XOPEN && !__USE_XOPEN2K */

/* >> sysconf(2)
 * @param: NAME: One of `_SC_*' from <bits/confname.h>
 * Return a system configuration value `NAME' */
__REDIRECT_EXCEPT(__LIBC,__PORT_NODOS __WUNUSED,long int,__LIBCCALL,sysconf,(int __name),(__name))
#endif /* __CRT_GLC */
#endif /* !__KERNEL__ */


#if !defined(F_LOCK) && (defined(__USE_MISC) || defined(__USE_XOPEN_EXTENDED))
#   define F_ULOCK 0 /* Unlock a previously locked region. */
#   define F_LOCK  1 /* Lock a region for exclusive use. */
#   define F_TLOCK 2 /* Test and lock a region for exclusive use. */
#   define F_TEST  3 /* Test a region for other processes locks. */
#ifndef __KERNEL__
#if defined(__DOS_COMPAT__) || (defined(__PE__) && !defined(__USE_FILE_OFFSET64))
#ifdef __USE_FILE_OFFSET64
__REDIRECT(__LIBC,,int,__LIBCCALL,__lockf32,(__fd_t __fd, int __cmd, __off32_t __len),_locking,(__fd,__cmd,__len))
__LOCAL int (__LIBCCALL lockf)(__fd_t __fd, int __cmd, __off64_t __len) { return __lockf32(__fd,__cmd,(__off32_t)__len); }
#else /* __USE_FILE_OFFSET64 */
__REDIRECT(__LIBC,,int,__LIBCCALL,lockf,(__fd_t __fd, int __cmd, __FS_TYPE(off) __len),_locking,(__fd,__cmd,__len))
#endif /* !__USE_FILE_OFFSET64 */
#else /* ... */
__REDIRECT_EXCEPT_FS64_XVOID(__LIBC,,int,__LIBCCALL,lockf,(__fd_t __fd, int __cmd, __FS_TYPE(off) __len),(__fd,__cmd,__len))
#endif /* !... */
#ifdef __USE_LARGEFILE64
#ifdef __CRT_GLC
__REDIRECT_EXCEPT_XVOID(__LIBC,,int,__LIBCCALL,lockf64,(__fd_t __fd, int __cmd, __off64_t __len),(__fd,__cmd,__len))
#else /* __CRT_GLC */
__LOCAL int (__LIBCCALL lockf64)(__fd_t __fd, int __cmd, __off64_t __len) { return lockf(__fd,__cmd,(__FS_TYPE(off))__len); }
#endif /* !__CRT_GLC */
#endif /* __USE_LARGEFILE64 */
#endif /* !__KERNEL__ */
#endif /* ... */

#ifdef __USE_GNU
#ifdef __COMPILER_HAVE_TYPEOF
#define TEMP_FAILURE_RETRY(expression) \
 __XBLOCK({ __typeof__(expression) __result; \
            do __result = (expression); \
            while (__result == -1L && errno == EINTR); \
            __XRETURN __result; }))
#elif defined(__COMPILER_HAVE_AUTOTYPE)
#define TEMP_FAILURE_RETRY(expression) \
 __XBLOCK({ __auto_type __result; \
            do __result = (expression); \
            while (__result == -1L && errno == EINTR); \
            __XRETURN __result; }))
#else
#define TEMP_FAILURE_RETRY(expression) \
 __XBLOCK({ long int __result; \
            do __result = (long int)(expression); \
            while (__result == -1L && errno == EINTR); \
            __XRETURN __result; }))
#endif
#endif

__SYSDECL_END

#ifdef __USE_KOS
#ifndef _PARTS_KOS2_UNISTD_H
#include "parts/kos2/unistd.h"
#endif
#ifdef _WCHAR_H
#ifndef _PARTS_KOS2_WUNISTD_H
#include "parts/kos2/wunistd.h"
#endif
#endif
#endif /* __USE_KOS */

#ifdef __USE_UTF
#ifdef _UCHAR_H
#ifndef _PARTS_KOS2_UUNISTD_H
#include "parts/kos2/uunistd.h"
#endif
#endif
#endif /* __USE_UTF */

#ifdef __USE_KOS3
#ifndef _PARTS_KOS3_UNISTD_H
#include "parts/kos3/unistd.h"
#endif
#ifdef _WCHAR_H
#ifndef _PARTS_KOS3_WUNISTD_H
#include "parts/kos3/wunistd.h"
#endif
#endif
#ifdef _UCHAR_H
#ifndef _PARTS_KOS3_UUNISTD_H
#include "parts/kos3/uunistd.h"
#endif
#endif
#endif /* __USE_KOS3 */

#ifdef __USE_EXCEPT
#include "parts/kos3/except/unistd.h"
#endif /* __USE_EXCEPT */

#endif /* !_UNISTD_H */
