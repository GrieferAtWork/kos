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
#ifndef __GUARD_HYBRID_ATOMIC_H
#define __GUARD_HYBRID_ATOMIC_H 1

#include "compiler.h"
#include "__atomic.h"
#include "host.h"

DECL_BEGIN

#ifdef __CC__

#ifndef __std_memory_order_defined
#define __std_memory_order_defined 1
__NAMESPACE_STD_BEGIN
typedef enum {
    memory_order_relaxed = __ATOMIC_RELAXED,
    memory_order_consume = __ATOMIC_CONSUME,
    memory_order_acquire = __ATOMIC_ACQUIRE,
    memory_order_release = __ATOMIC_RELEASE,
    memory_order_acq_rel = __ATOMIC_ACQ_REL,
    memory_order_seq_cst = __ATOMIC_SEQ_CST
} memory_order;
__NAMESPACE_STD_END
#endif
#ifndef __memory_order_defined
#define __memory_order_defined 1
__NAMESPACE_STD_USING(memory_order_relaxed)
__NAMESPACE_STD_USING(memory_order_consume)
__NAMESPACE_STD_USING(memory_order_acquire)
__NAMESPACE_STD_USING(memory_order_release)
__NAMESPACE_STD_USING(memory_order_acq_rel)
__NAMESPACE_STD_USING(memory_order_seq_cst)
__NAMESPACE_STD_USING(memory_order)
#endif /* !__memory_order_defined */

#define OATOMIC_LOAD(x,order)                          __hybrid_atomic_load(x,order)
#define OATOMIC_STORE(x,v,order)                       __hybrid_atomic_store(x,v,order)
#define OATOMIC_XCH(x,v,order)                         __hybrid_atomic_xch(x,v,order)
#define OATOMIC_CMPXCH(x,oldv,newv,succ,fail)          __hybrid_atomic_cmpxch(x,oldv,newv,succ,fail)
#define OATOMIC_CMPXCH_WEAK(x,oldv,newv,succ,fail)     __hybrid_atomic_cmpxch_weak(x,oldv,newv,succ,fail)
#define OATOMIC_CMPXCH_VAL(x,oldv,newv,succ,fail)      __hybrid_atomic_cmpxch_val(x,oldv,newv,succ,fail)
#define OATOMIC_CMPXCH_VAL_WEAK(x,oldv,newv,succ,fail) __hybrid_atomic_cmpxch_val_weak(x,oldv,newv,succ,fail)
#define OATOMIC_ADDFETCH(x,v,order)                    __hybrid_atomic_addfetch(x,v,order)
#define OATOMIC_SUBFETCH(x,v,order)                    __hybrid_atomic_subfetch(x,v,order)
#define OATOMIC_ANDFETCH(x,v,order)                    __hybrid_atomic_andfetch(x,v,order)
#define OATOMIC_ORFETCH(x,v,order)                     __hybrid_atomic_orfetch(x,v,order)
#define OATOMIC_XORFETCH(x,v,order)                    __hybrid_atomic_xorfetch(x,v,order)
#define OATOMIC_NANDFETCH(x,v,order)                   __hybrid_atomic_nandfetch(x,v,order)
#define OATOMIC_FETCHADD(x,v,order)                    __hybrid_atomic_fetchadd(x,v,order)
#define OATOMIC_FETCHSUB(x,v,order)                    __hybrid_atomic_fetchsub(x,v,order)
#define OATOMIC_FETCHAND(x,v,order)                    __hybrid_atomic_fetchand(x,v,order)
#define OATOMIC_FETCHOR(x,v,order)                     __hybrid_atomic_fetchor(x,v,order)
#define OATOMIC_FETCHXOR(x,v,order)                    __hybrid_atomic_fetchxor(x,v,order)
#define OATOMIC_FETCHNAND(x,v,order)                   __hybrid_atomic_fetchnand(x,v,order)
#define OATOMIC_INCFETCH(x,order)                      __hybrid_atomic_incfetch(x,order)
#define OATOMIC_DECFETCH(x,order)                      __hybrid_atomic_decfetch(x,order)
#define OATOMIC_FETCHINC(x,order)                      __hybrid_atomic_fetchinc(x,order)
#define OATOMIC_FETCHDEC(x,order)                      __hybrid_atomic_fetchdec(x,order)

#define ATOMIC_LOAD(x)                      OATOMIC_LOAD(x,__ATOMIC_SEQ_CST)
#define ATOMIC_STORE(x,v)                   OATOMIC_STORE(x,v,__ATOMIC_SEQ_CST)
#define ATOMIC_XCH(x,v)                     OATOMIC_XCH(x,v,__ATOMIC_SEQ_CST)
#define ATOMIC_CMPXCH(x,oldv,newv)          OATOMIC_CMPXCH(x,oldv,newv,__ATOMIC_SEQ_CST,__ATOMIC_SEQ_CST)
#define ATOMIC_CMPXCH_WEAK(x,oldv,newv)     OATOMIC_CMPXCH_WEAK(x,oldv,newv,__ATOMIC_SEQ_CST,__ATOMIC_SEQ_CST)
#define ATOMIC_CMPXCH_VAL(x,oldv,newv)      OATOMIC_CMPXCH_VAL(x,oldv,newv,__ATOMIC_SEQ_CST,__ATOMIC_SEQ_CST)
#define ATOMIC_CMPXCH_VAL_WEAK(x,oldv,newv) OATOMIC_CMPXCH_VAL_WEAK(x,oldv,newv,__ATOMIC_SEQ_CST,__ATOMIC_SEQ_CST)
#define ATOMIC_ADDFETCH(x,v)                OATOMIC_ADDFETCH(x,v,__ATOMIC_SEQ_CST)
#define ATOMIC_SUBFETCH(x,v)                OATOMIC_SUBFETCH(x,v,__ATOMIC_SEQ_CST)
#define ATOMIC_ANDFETCH(x,v)                OATOMIC_ANDFETCH(x,v,__ATOMIC_SEQ_CST)
#define ATOMIC_ORFETCH(x,v)                 OATOMIC_ORFETCH(x,v,__ATOMIC_SEQ_CST)
#define ATOMIC_XORFETCH(x,v)                OATOMIC_XORFETCH(x,v,__ATOMIC_SEQ_CST)
#define ATOMIC_NANDFETCH(x,v)               OATOMIC_NANDFETCH(x,v,__ATOMIC_SEQ_CST)
#define ATOMIC_FETCHADD(x,v)                OATOMIC_FETCHADD(x,v,__ATOMIC_SEQ_CST)
#define ATOMIC_FETCHSUB(x,v)                OATOMIC_FETCHSUB(x,v,__ATOMIC_SEQ_CST)
#define ATOMIC_FETCHAND(x,v)                OATOMIC_FETCHAND(x,v,__ATOMIC_SEQ_CST)
#define ATOMIC_FETCHOR(x,v)                 OATOMIC_FETCHOR(x,v,__ATOMIC_SEQ_CST)
#define ATOMIC_FETCHXOR(x,v)                OATOMIC_FETCHXOR(x,v,__ATOMIC_SEQ_CST)
#define ATOMIC_FETCHNAND(x,v)               OATOMIC_FETCHNAND(x,v,__ATOMIC_SEQ_CST)
#define ATOMIC_INCFETCH(x)                  OATOMIC_INCFETCH(x,__ATOMIC_SEQ_CST)
#define ATOMIC_DECFETCH(x)                  OATOMIC_DECFETCH(x,__ATOMIC_SEQ_CST)
#define ATOMIC_FETCHINC(x)                  OATOMIC_FETCHINC(x,__ATOMIC_SEQ_CST)
#define ATOMIC_FETCHDEC(x)                  OATOMIC_FETCHDEC(x,__ATOMIC_SEQ_CST)


#if (!defined(__NO_XBLOCK) & defined(__COMPILER_HAVE_TYPEOF)) && \
    ((defined(__KERNEL__) && defined(CONFIG_NO_SMP)) || \
     (defined(__i386__) || defined(__x86_64__)))
/*  On platforms with UMA, compiler barriers are sufficient to
 *  ensure that single-direction memory accesses are atomic
 * (UMA == Uniform Memory Access, meaning that any memory write access
 *  is guarantied to become visible to every cpu sooner or later, so
 *  all we need to do is force the compiler to not omit memory accesses
 *  by caching variables in registers, which can easily be done using
 *  read/write barriers)
 *  XXX: OMP has ways to force write-back/read-through for only
 *       specific variables. - Our barriers however force it for all.
 *       Maybe figure out a way to only clobber certain variables? */
#define ATOMIC_READ(x) \
 XBLOCK({ __typeof__(x) __temp = (x); \
          COMPILER_READ_BARRIER(); \
          XRETURN __temp; })
#define ATOMIC_WRITE(x,v) ((x) = (v),COMPILER_WRITE_BARRIER())
#else
#define ATOMIC_READ(x)     OATOMIC_LOAD(x,__ATOMIC_ACQUIRE)
#define ATOMIC_WRITE(x,v)  OATOMIC_STORE(x,v,__ATOMIC_RELEASE)
#endif

#define ATOMIC_INCIFNONZERO(x) \
 XBLOCK({ register __typeof__(x) __temp; \
          do { __temp = ATOMIC_READ(x); \
               if (!__temp) break; \
          } while (!ATOMIC_CMPXCH_WEAK(x,__temp,__temp+1)); \
          XRETURN __temp != 0; \
 })
#define ATOMIC_DECIFNOTONE(x) \
 XBLOCK({ register __typeof__(x) __temp; \
          do { __temp = ATOMIC_READ(x); \
               if (__temp == 1) break; \
          } while (!ATOMIC_CMPXCH_WEAK(x,__temp,__temp-1)); \
          XRETURN __temp != 1; \
 })
#define ATOMIC_DECIFNOTZERO(x) \
 XBLOCK({ register __typeof__(x) __temp; \
          do { __temp = ATOMIC_READ(x); \
               if (__temp == 0) break; \
          } while (!ATOMIC_CMPXCH_WEAK(x,__temp,__temp-1)); \
          XRETURN __temp != 0; \
 })
#define ATOMIC_DECIFONE(x) \
 XBLOCK({ register __typeof__(x) __temp; \
          do { __temp = ATOMIC_READ(x); \
               if (__temp != 1) break; \
          } while (!ATOMIC_CMPXCH_WEAK(x,__temp,__temp-1)); \
          XRETURN __temp == 1; \
 })

#endif /* __CC__ */

DECL_END

#endif /* !__GUARD_HYBRID_ATOMIC_H */
