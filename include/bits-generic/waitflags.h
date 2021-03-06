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
#ifndef _BITS_GENERIC_WAITFLAGS_H
#define _BITS_GENERIC_WAITFLAGS_H 1
#define _BITS_WAITFLAGS_H 1

#include <__stdinc.h>
#include <features.h>

__SYSDECL_BEGIN

/* NOTE: This file based on the GLIBC header of the same name: */
/* Definitions of flag bits for `waitpid' et al.
   Copyright (C) 1992-2016 Free Software Foundation, Inc.
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

/* Bits in the third argument to `waitpid'. */
#define WNOHANG     1 /* Don't block waiting. */
#define WUNTRACED   2 /* Report status of stopped children. */

/* Bits in the fourth argument to `waitid'. */
#define WSTOPPED    2 /* Report stopped child (same as WUNTRACED). */
#define WEXITED     4 /* Report dead child. */
#define WCONTINUED  8 /* Report continued child. */
#if defined(__USE_KOS3) && __KOS_VERSION__ >= 300
#define WONLYTHREADS 0x8000 /* Only wait for children created within the current
                             * process, and with the `CLONE_THREAD' flag set. */
#endif
#define WNOWAIT     0x01000000 /* Don't reap, just poll status. */
#ifdef __KOS__
#define WNOREAP     WNOWAIT    /* Don't reap, just poll status (doesn't this name make more sense than `WNOWAIT'?). */
#endif

#if defined(__KOS__) && __KOS_VERSION__ >= 300
#define __WNOTHREAD __COMPILER_DEPRECATED_EXPR_("`__WNOTHREAD' is not supposed in KOS",0x20000000)
#define __WALL      __COMPILER_DEPRECATED_EXPR_("`__WALL' is not supposed in KOS",     0x40000000)
#define __WCLONE    __COMPILER_DEPRECATED_EXPR_("`__WCLONE' is not supposed in KOS",   0x80000000)
#else
#define __WNOTHREAD 0x20000000 /* Don't wait on children of other threads in this group */
#define __WALL      0x40000000 /* Wait for any child. */
#define __WCLONE    0x80000000 /* Wait for cloned process. */
#endif

/* The following values are used by the `waitid' function. */
#if defined(__USE_XOPEN) || defined(__USE_XOPEN2K8)
#ifndef __ENUM_IDTYPE_T
#define __ENUM_IDTYPE_T 1
#   undef P_ALL
#   undef P_PID
#   undef P_PGID
#ifdef __COMPILER_PREFERR_ENUMS
typedef enum {
    P_ALL  = 0, /* Wait for any child. */
    P_PID  = 1, /* Wait for specified process. */
    P_PGID = 2  /* Wait for members of process group. */
} idtype_t;
#   define P_ALL   P_ALL
#   define P_PID   P_PID
#   define P_PGID  P_PGID
#else /* __COMPILER_PREFERR_ENUMS */
#ifdef __CC__
typedef int idtype_t;
#endif /* __CC__ */
#   define P_ALL  0 /* Wait for any child. */
#   define P_PID  1 /* Wait for specified process. */
#   define P_PGID 2 /* Wait for members of process group. */
#endif /* !__COMPILER_PREFERR_ENUMS */
#endif /* !__ENUM_IDTYPE_T */
#endif /* ... */

__SYSDECL_END

#endif /* !_BITS_GENERIC_WAITFLAGS_H */
