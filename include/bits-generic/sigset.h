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
#ifndef _BITS_GENERIC_SIGSET_H
#define _BITS_GENERIC_SIGSET_H 1
#define _BITS_SIGSET_H 1

#include <__stdinc.h>
#include <hybrid/typecore.h>

__SYSDECL_BEGIN

/* __sig_atomic_t, __sigset_t, and related definitions.  Linux version.
   Copyright (C) 1991-2016 Free Software Foundation, Inc.
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

#ifndef __SIZEOF_SIGSET_T__
#define __SIZEOF_SIGSET_T__ 128 /* 1024/8 */
#endif /* !__SIZEOF_SIGSET_T__ */

#ifndef _SIGSET_H_types
#define _SIGSET_H_types 1
#define _SIGSET_NWORDS    (__SIZEOF_SIGSET_T__/__SIZEOF_POINTER__)
#ifdef __CC__
#ifndef ____sig_atomic_t_defined
#define ____sig_atomic_t_defined 1
typedef __SIG_ATOMIC_TYPE__ __sig_atomic_t;
#endif /* !____sig_atomic_t_defined */
typedef struct { __ULONGPTR_TYPE__ __val[_SIGSET_NWORDS]; } __sigset_t;
#ifdef __USE_KOS
#define __SIGSET_INIT_NONE    {{ [0 ... _SIGSET_NWORDS-1] = 0 }}
#define __SIGSET_INIT_FULL    {{ [0 ... _SIGSET_NWORDS-1] = (__ULONGPTR_TYPE__)-1 }}
#endif
#endif /* __CC__ */
#endif

#ifdef __CC__
#ifdef __DOCGEN__
#undef __sigmask
#endif
#ifdef __KERNEL__
#define __sigmask(sig) (((__ULONGPTR_TYPE__)1)<<((sig)%(8*sizeof(__ULONGPTR_TYPE__))))
#define __sigword(sig) ((sig)/(8*sizeof(__ULONGPTR_TYPE__)))
#else
#define __sigmask(sig) (((__ULONGPTR_TYPE__)1)<<(((sig)-1)%(8*sizeof(__ULONGPTR_TYPE__))))
#define __sigword(sig) (((sig)-1)/(8*sizeof(__ULONGPTR_TYPE__)))
#endif
#define __sigemptyset(set) \
  __XBLOCK({ int __cnt = _SIGSET_NWORDS; \
             __sigset_t *__set = (set); \
             while (--__cnt >= 0) __set->__val[__cnt] = 0; \
             __XRETURN 0; })
#define __sigfillset(set) \
  __XBLOCK({ int __cnt = _SIGSET_NWORDS; \
             __sigset_t *__set = (set); \
             while (--__cnt >= 0) __set->__val[__cnt] = ~0UL; \
             __XRETURN 0; })

#ifdef __USE_GNU
#define __sigisemptyset(set) \
  __XBLOCK({ int __cnt = _SIGSET_NWORDS; \
             const __sigset_t *__set = (set); \
             int __ret = __set->__val[--__cnt]; \
             while (!__ret && --__cnt >= 0) \
             __ret = __set->__val[__cnt]; \
             __XRETURN __ret == 0; })
#define __sigandset(dest,left,right) \
  __XBLOCK({ int __cnt = _SIGSET_NWORDS; \
             __sigset_t *__dest = (dest); \
             const __sigset_t *__left = (left); \
             const __sigset_t *__right = (right); \
             while (--__cnt >= 0) { \
               __dest->__val[__cnt] = (__left->__val[__cnt] & __right->__val[__cnt]); \
             }\
             __XRETURN 0; })
#define __sigorset(dest,left,right) \
  __XBLOCK({ int __cnt = _SIGSET_NWORDS; \
             __sigset_t *__dest = (dest); \
             const __sigset_t *__left = (left); \
             const __sigset_t *__right = (right); \
             while (--__cnt >= 0) { \
               __dest->__val[__cnt] = (__left->__val[__cnt] | __right->__val[__cnt]); \
             }\
             __XRETURN 0; })
#endif

#ifndef __KERNEL__
__LIBC int (__LIBCCALL __sigismember)(const __sigset_t *__set, int __sig);
__LIBC int (__LIBCCALL __sigaddset)(__sigset_t *__set, int __sig);
__LIBC int (__LIBCCALL __sigdelset)(__sigset_t *__set, int __sig);
#else /* !__KERNEL__ */
#define __SIGSETFN(name,body,const) \
__LOCAL int (__LIBCCALL name)(const __sigset_t *__set, int __sig) { \
 __ULONGPTR_TYPE__ __mask = __sigmask(__sig); \
 __ULONGPTR_TYPE__ __word = __sigword(__sig); \
 return body; \
}
__SIGSETFN(__sigismember,(__set->__val[__word] & __mask) ? 1 : 0,const)
__SIGSETFN(__sigaddset,((__set->__val[__word] |= __mask), 0),)
__SIGSETFN(__sigdelset,((__set->__val[__word] &= ~__mask), 0),)
#undef __SIGSETFN
#endif /* __KERNEL__ */
#endif /* __CC__ */

__SYSDECL_END

#endif /* !_BITS_GENERIC_SIGSET_H */
