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
#ifndef _SCHED_H
#define _SCHED_H 1

#include "__stdinc.h"
#include <bits/sched.h>
#include <bits/types.h>
#include <features.h>
#include <hybrid/timespec.h>

#define sched_priority	__sched_priority

__SYSDECL_BEGIN

#ifdef __USE_GNU

#ifdef __CC__
typedef __cpu_set_t    cpu_set_t;
#endif /* __CC__ */

#ifdef __USE_KOS
#define CPU_SETNONE                 __CPU_SETNONE
#define CPU_SETALL                  __CPU_SETALL
#define CPU_SETONE(i)               __CPU_SETONE(i)
#define CPU_ISEMPTY(cpusetp)        __CPU_ISEMPTY_S(sizeof(cpu_set_t),cpusetp)
#define CPU_FILL(cpusetp)           __CPU_FILL_S(sizeof(cpu_set_t),cpusetp)
#define CPU_FILL_S(setsize,cpusetp) __CPU_FILL_S(setsize,cpusetp)
#endif
#define CPU_SETSIZE   __CPU_SETSIZE

#define CPU_SET(cpu,cpusetp)                      __CPU_SET_S(cpu,sizeof(cpu_set_t),cpusetp)
#define CPU_CLR(cpu,cpusetp)                      __CPU_CLR_S(cpu,sizeof(cpu_set_t),cpusetp)
#define CPU_ISSET(cpu,cpusetp)                  __CPU_ISSET_S(cpu,sizeof(cpu_set_t),cpusetp)
#define CPU_ZERO(cpusetp)                        __CPU_ZERO_S(sizeof(cpu_set_t),cpusetp)
#define CPU_COUNT(cpusetp)                      __CPU_COUNT_S(sizeof(cpu_set_t),cpusetp)
#define CPU_SET_S(cpu,setsize,cpusetp)            __CPU_SET_S(cpu,setsize,cpusetp)
#define CPU_CLR_S(cpu,setsize,cpusetp)            __CPU_CLR_S(cpu,setsize,cpusetp)
#define CPU_ISSET_S(cpu,setsize,cpusetp)        __CPU_ISSET_S(cpu,setsize, cpusetp)
#define CPU_ZERO_S(setsize,cpusetp)              __CPU_ZERO_S(setsize,cpusetp)
#define CPU_COUNT_S(setsize,cpusetp)            __CPU_COUNT_S(setsize,cpusetp)
#define CPU_EQUAL(cpusetp1,cpusetp2)            __CPU_EQUAL_S(sizeof(cpu_set_t),cpusetp1,cpusetp2)
#define CPU_EQUAL_S(setsize,cpusetp1,cpusetp2)  __CPU_EQUAL_S(setsize,cpusetp1,cpusetp2)
#define CPU_AND(destset,srcset1,srcset2)           __CPU_OP_S(sizeof(cpu_set_t),destset,srcset1,srcset2,&)
#define CPU_OR(destset,srcset1,srcset2)            __CPU_OP_S(sizeof(cpu_set_t),destset,srcset1,srcset2,|)
#define CPU_XOR(destset,srcset1,srcset2)           __CPU_OP_S(sizeof(cpu_set_t),destset,srcset1,srcset2,^)
#define CPU_AND_S(setsize,destset,srcset1,srcset2) __CPU_OP_S(setsize,destset,srcset1,srcset2,&)
#define CPU_OR_S(setsize,destset,srcset1,srcset2)  __CPU_OP_S(setsize,destset,srcset1,srcset2,|)
#define CPU_XOR_S(setsize,destset,srcset1,srcset2) __CPU_OP_S(setsize,destset,srcset1,srcset2,^)
#define CPU_ALLOC_SIZE(count)                __CPU_ALLOC_SIZE(count)
#define CPU_ALLOC(count)                          __CPU_ALLOC(count)
#define CPU_FREE(cpuset)                           __CPU_FREE(cpuset)

#ifdef __CC__
#ifndef __KERNEL__
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,sched_setparam,(__pid_t __pid, struct sched_param const *__param),(__pid,__param))
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,sched_getparam,(__pid_t __pid, struct sched_param *__param),(__pid,__param))
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,sched_setscheduler,(__pid_t __pid, int __policy, struct sched_param const *__param),(__pid,__policy,__param))
__REDIRECT_EXCEPT(__LIBC,__PORT_NODOS,int,__LIBCCALL,sched_getscheduler,(__pid_t __pid),(__pid))

/* @return: 1: Another thread was executed prior to the function returning.
 *             The thread may not necessarily be apart of the calling process.
 * @return: 0: The function returned immediately when no other thread was executed. */
__REDIRECT_EXCEPT(__LIBC,__PORT_NODOS,int,__LIBCCALL,sched_yield,(void),())

__LIBC __PORT_NODOS int (__LIBCCALL sched_get_priority_max)(int __algorithm);
__LIBC __PORT_NODOS int (__LIBCCALL sched_get_priority_min)(int __algorithm);
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,sched_setaffinity,(__pid_t __pid, __size_t __cpusetsize, cpu_set_t const *__cpuset),(__pid,__cpusetsize,__cpuset))
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,sched_getaffinity,(__pid_t __pid, __size_t __cpusetsize, cpu_set_t *__cpuset),(__pid,__cpusetsize,__cpuset))
__REDIRECT_EXCEPT_TM64_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,sched_rr_get_interval,(__pid_t __pid, struct timespec *__t),(__pid,__t))
#ifdef __USE_TIME64
__REDIRECT_EXCEPT_XVOID(__LIBC,__PORT_NODOS,int,__LIBCCALL,sched_rr_get_interval64,(__pid_t __pid, struct __timespec64 *__t),(__pid,__t))
#endif /* __USE_TIME64 */
#endif /* !__KERNEL__ */
#endif /* __CC__ */

#endif /* __USE_GNU */

__SYSDECL_END

#ifdef __USE_KOS3
#include "parts/kos3/sched.h"
#endif
#ifdef __USE_EXCEPT
#include "parts/kos3/except/sched.h"
#endif

#endif /* !_SCHED_H */
