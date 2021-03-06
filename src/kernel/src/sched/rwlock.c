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
#ifndef GUARD_KERNEL_SRC_SCHED_RWLOCK_C
#define GUARD_KERNEL_SRC_SCHED_RWLOCK_C 1
#define _KOS_SOURCE 1

#include <hybrid/compiler.h>
#include <kos/types.h>
#include <hybrid/atomic.h>
#include <kernel/debug.h>
#include <kernel/sections.h>
#include <kernel/heap.h>
#include <kernel/bind.h>
#include <sched/task.h>
#include <sched/rwlock.h>
#include <sched/pertask-arith.h>
#include <except.h>
#include <assert.h>
#include <string.h>
#include <bits/poll.h>

#include "rwlock.h"

DECL_BEGIN

INTERN ATTR_PERTASK struct read_locks my_readlocks = {
    .rls_use = 0,
    .rls_cnt = 0,
    .rls_msk = CONFIG_TASK_STATIC_READLOCKS-1
};

DEFINE_PERTASK_INIT(readlocks_init);
DEFINE_PERTASK_FINI(readlocks_fini);
PRIVATE ATTR_USED void KCALL
readlocks_init(struct task *__restrict thread) {
 struct read_locks *locks;
 locks = &FORTASK(thread,my_readlocks);
 locks->rls_vec = locks->rls_sbuf;
}
PRIVATE ATTR_USED void KCALL
readlocks_fini(struct task *__restrict thread) {
 struct read_locks *locks;
 locks = &FORTASK(thread,my_readlocks);
 assertf(locks->rls_use == 0,
        "Thread died with read locks still held");
 if (locks->rls_vec != locks->rls_sbuf)
     kfree(locks->rls_vec);
}

/* Find an existing read-lock descriptor for `lock', or return NULL. */
INTERN ATTR_NOTHROW struct read_lock *KCALL
rwlock_find_readlock(struct rwlock *__restrict lock) {
 uintptr_t i,perturb;
 struct read_locks *locks;
 struct read_lock *lockdesc;
 locks = &PERTASK(my_readlocks);
 assert(locks->rls_use <= locks->rls_cnt);
 assert(locks->rls_cnt <= locks->rls_msk);
 perturb = i = RWLOCK_HASH(lock) & locks->rls_msk;
 for (;; i = ((i << 2) + i + perturb + 1),perturb >>= 5) {
  lockdesc = &locks->rls_vec[i & locks->rls_msk];
  if (lockdesc->rl_rwlock == lock)
      return lockdesc; /* Existing descriptor. */
  if (!lockdesc->rl_rwlock)
      break;
 }
 return NULL;
}

/* Return a read-lock descriptor for `lock', or allocate a new one. */
INTERN ATTR_RETNONNULL struct read_lock *KCALL
rwlock_get_readlock(struct rwlock *__restrict lock) {
 uintptr_t j,perturb;
 struct read_locks *locks;
 struct read_lock *lockdesc;
 struct read_lock *result;
 locks = &PERTASK(my_readlocks);
again:
 assert(locks->rls_use <= locks->rls_cnt);
 assert(locks->rls_cnt <= locks->rls_msk);
 result = NULL;
 perturb = j = RWLOCK_HASH(lock) & locks->rls_msk;
 for (;; j = ((j << 2) + j + perturb + 1),perturb >>= 5) {
  lockdesc = &locks->rls_vec[j & locks->rls_msk];
  if (lockdesc->rl_rwlock == lock)
      return lockdesc; /* Existing descriptor. */
  if (!lockdesc->rl_rwlock) {
   /* End of chain (if no dummy was found, use this entry). */
   if (!result) {
    if (locks->rls_cnt == locks->rls_msk) {
     struct read_lock *old_vector;
     struct read_lock *EXCEPT_VAR new_vector;
     size_t new_mask,i;
     /* Must re-hash the readlock hash-vector. */
     new_mask   = (locks->rls_msk << 1)|1;
     /* NOTE: Memory must be locked in-core, because otherwise the following loop can happen:
      *   #1: #PF
      *   #2: EXCEPTION
      *   #3: EXCEPTION: tryread(VM)  (To find the associated FDE)
      *   #4: FIND_READLOCK(VM)
      *   #5: #PF
      */
     new_vector = (struct read_lock *)kmalloc((new_mask+1)*
                                               sizeof(struct read_lock),
                                               GFP_SHARED|GFP_CALLOC|
                                               GFP_LOCKED);
     old_vector = locks->rls_vec;
     TRY {
      for (i = 0; i <= locks->rls_msk; ++i) {
       struct rwlock *heldlock = old_vector[i].rl_rwlock;
       if (!heldlock || heldlock == READLOCK_DUMMYLOCK) continue;
       perturb = j = RWLOCK_HASH(heldlock) & new_mask;
       for (;; j = ((j << 2) + j + perturb + 1),perturb >>= 5) {
        lockdesc = &new_vector[j & new_mask];
        if (lockdesc->rl_rwlock) continue;
        memcpy(lockdesc,&old_vector[i],sizeof(struct read_lock));
        break;
       }
      }
     } EXCEPT (EXCEPT_EXECUTE_HANDLER) {
      kfree(new_vector);
      error_rethrow();
     }
     locks->rls_cnt = locks->rls_use; /* Dismiss dummy entries. */
     locks->rls_vec = new_vector;
     locks->rls_msk = new_mask;
     if (old_vector != locks->rls_sbuf)
         kfree(old_vector);
     goto again;
    }
    ++locks->rls_cnt;
    result = lockdesc;
   }
   break;
  }
  if (lockdesc->rl_rwlock == READLOCK_DUMMYLOCK) {
   /* Re-use dummy locks. */
   if (!result)
        result = lockdesc;
  }
 }
 /* New descriptor. */
 assert(result->rl_rwlock == NULL ||
        result->rl_rwlock == READLOCK_DUMMYLOCK);
 result->rl_recursion = 0;
 result->rl_rwlock = lock;
 ++locks->rls_use;
 return result;
}

/* Return a read-lock descriptor for `lock', or allocate a new one. */
INTERN struct read_lock *KCALL
rwlock_get_readlock_nothrow(struct rwlock *__restrict lock) {
 uintptr_t j,perturb;
 struct read_locks *locks;
 struct read_lock *lockdesc;
 struct read_lock *result = NULL;
 locks = &PERTASK(my_readlocks);
 assert(locks->rls_use <= locks->rls_cnt);
 assert(locks->rls_cnt <= locks->rls_msk);
 perturb = j = RWLOCK_HASH(lock) & locks->rls_msk;
 for (;; j = ((j << 2) + j + perturb + 1),perturb >>= 5) {
  lockdesc = &locks->rls_vec[j & locks->rls_msk];
  if (lockdesc->rl_rwlock == lock)
      return lockdesc; /* Existing descriptor. */
  if (!lockdesc->rl_rwlock) {
   /* End of chain (if no dummy was found, use this entry). */
   if (!result) {
    if (locks->rls_cnt == locks->rls_msk)
        return NULL;
    result = lockdesc;
    ++locks->rls_cnt;
   }
   break;
  }
  if (lockdesc->rl_rwlock == READLOCK_DUMMYLOCK) {
   /* Re-use dummy locks. */
   if (!result)
        result = lockdesc;
  }
 }
 if (!result) goto done;
 /* New descriptor. */
 assert(result->rl_rwlock == NULL ||
        result->rl_rwlock == READLOCK_DUMMYLOCK);
 result->rl_recursion = 0;
 result->rl_rwlock = lock;
 ++locks->rls_use;
done:
 return result;
}

/* Delete the given rlock. */
INTERN void KCALL
rwlock_delete_readlock(struct read_lock *__restrict rlock) {
 assert(PERTASK_TEST(my_readlocks.rls_use));
 assert(PERTASK_TEST(my_readlocks.rls_cnt));
 assert(rlock >= PERTASK_GET(my_readlocks.rls_vec));
 assert(rlock <= PERTASK_GET(my_readlocks.rls_vec)+PERTASK_GET(my_readlocks.rls_msk));
 rlock->rl_rwlock = READLOCK_DUMMYLOCK;
 PERTASK_DEC(my_readlocks.rls_use);
}



PUBLIC ATTR_NOTHROW bool KCALL
rwlock_reading(struct rwlock *__restrict self) {
 struct read_lock *lock;
 if (self->rw_scnt == 0)
     return false;
 if (self->rw_mode == RWLOCK_MODE_FWRITING)
     return self->rw_xowner == THIS_TASK;
 lock = rwlock_find_readlock(self);
 assert(!lock || lock->rl_recursion != 0);
 return lock != NULL;
}

PUBLIC bool KCALL
__os_rwlock_tryread(struct rwlock *__restrict self) {
 struct read_lock *desc;
 u32 control_word;
#ifdef CONFIG_LOG_RWLOCKS
 debug_printf("[RWLOCK][%p] TRY_READ(%p)\n",THIS_TASK,self);
#endif
 if (self->rw_mode == RWLOCK_MODE_FWRITING) {
  if (self->rw_xowner == THIS_TASK) {
   /* Recursive read-after-write. */
   ++self->rw_xind;
   return true;
  }
  assertf(!rwlock_find_readlock(self),
          "You can't be holding read locks when another thread is writing\n"
          "THIS_TASK       = %p\n"
          "self->rw_xowner = %p\n"
          "self->rw_xind   = %I16u\n",
          THIS_TASK,self->rw_xowner,
          self->rw_xind);
  return false;
 }
 desc = rwlock_get_readlock(self);
 if (desc->rl_recursion) {
  /* Recursive read-lock. */
  assert(self->rw_mode != RWLOCK_MODE_FWRITING);
  assert(self->rw_scnt != 0);
#if 0 /* Thread that already have a read-lock can still acquire
       * more, even if the rw-lock is in upgrade mode. */
  if (self->rw_mode == RWLOCK_MODE_FUPGRADING)
      return false;
#endif
  ++desc->rl_recursion;
  return true;
 }
 do {
  control_word = ATOMIC_READ(self->rw_state);
  /* Only allow acquisition when the lock is in read-mode.
   * In upgrade or write-mode, we cannot allow more locks
   * to be acquired. */
  if (__RWLOCK_MODE(control_word) != RWLOCK_MODE_FREADING) {
   /* This also deals with upgrade-mode. */
   rwlock_delete_readlock(desc);
   return false;
  }
 } while (!ATOMIC_CMPXCH_WEAK(self->rw_state,control_word,
                              __RWLOCK_INCIND(control_word)));
 /* Initial read-lock. */
 desc->rl_recursion = 1;
 return true;
}

PUBLIC ATTR_NOTHROW bool KCALL
__os_rwlock_tryread_nothrow(struct rwlock *__restrict self) {
 struct read_lock *desc;
 u32 control_word;
#ifdef CONFIG_LOG_RWLOCKS
 debug_printf("[RWLOCK][%p] TRY_READ(%p)\n",THIS_TASK,self);
#endif
 if (self->rw_mode == RWLOCK_MODE_FWRITING) {
  if (self->rw_xowner == THIS_TASK) {
   /* Recursive read-after-write. */
   ++self->rw_xind;
   return true;
  }
  assertf(!rwlock_find_readlock(self),
          "You can't be holding read locks when another thread is writing");
  return false;
 }
 desc = rwlock_get_readlock_nothrow(self);
 if (!desc) return false; /* Failed to allocate more read locks. */
 if (desc->rl_recursion) {
  /* Recursive read-lock. */
  assert(self->rw_mode != RWLOCK_MODE_FWRITING);
  assert(self->rw_scnt != 0);
#if 0 /* Thread that already have a read-lock can still acquire
       * more, even if the rw-lock is in upgrade mode. */
  if (self->rw_mode == RWLOCK_MODE_FUPGRADING)
      return false;
#endif
  ++desc->rl_recursion;
  return true;
 }
 do {
  control_word = ATOMIC_READ(self->rw_state);
  /* Only allow acquisition when the lock is in read-mode.
   * In upgrade or write-mode, we cannot allow more locks
   * to be acquired. */
  if (__RWLOCK_MODE(control_word) != RWLOCK_MODE_FREADING) {
   /* This also deals with upgrade-mode. */
   rwlock_delete_readlock(desc);
   return false;
  }
 } while (!ATOMIC_CMPXCH_WEAK(self->rw_state,control_word,
                              __RWLOCK_INCIND(control_word)));
 /* Initial read-lock. */
 desc->rl_recursion = 1;
 return true;
}

PUBLIC bool KCALL
__os_rwlock_timedread(struct rwlock *__restrict self,
                      jtime_t abs_timeout) {
 struct read_lock *desc;
 u32 control_word;
#ifdef CONFIG_LOG_RWLOCKS
 debug_printf("[RWLOCK][%p] READ(%p)\n",THIS_TASK,self);
#endif
 assertf(!task_isconnected(),
         "You mustn't be connected when calling this function");
 if (self->rw_mode == RWLOCK_MODE_FWRITING) {
  if (self->rw_xowner == THIS_TASK) {
   /* Recursive read-after-write. */
   ++self->rw_xind;
   return true;
  }
  assertf(!rwlock_find_readlock(self),
          "You can't be holding read locks when another thread is writing");
  desc = rwlock_get_readlock(self);
 } else {
  desc = rwlock_get_readlock(self);
  if (desc->rl_recursion) {
   /* Recursive read-lock. */
   assert(self->rw_mode != RWLOCK_MODE_FWRITING);
   assert(self->rw_scnt != 0);
#if 0 /* Thread that already have a read-lock can still acquire
        * more, even if the rw-lock is in upgrade mode. */
   if (self->rw_mode == RWLOCK_MODE_FUPGRADING)
       return false;
#endif
   ++desc->rl_recursion;
   return true;
  }
 }
acquire_read_lock:
 assert(desc->rl_recursion == 0);
 do {
  control_word = ATOMIC_READ(self->rw_state);
  /* Only allow acquisition when the lock is in read-mode.
   * In upgrade or write-mode, we cannot allow more locks
   * to be acquired. */
  if (__RWLOCK_MODE(control_word) != RWLOCK_MODE_FREADING) {
   /* Connect to the chmode-signal. */
   task_connect(&self->rw_chmode);
   /* Wait for the lock to become available. */
   for (;;) {
    control_word = ATOMIC_READ(self->rw_state);
    /* Check if the lock switched to read-mode. */
    if unlikely(__RWLOCK_MODE(control_word) == RWLOCK_MODE_FREADING) {
     if (!ATOMIC_CMPXCH_WEAK(self->rw_state,control_word,
                             __RWLOCK_INCIND(control_word)))
          continue;
     /* It actually did! */
     task_disconnect();
     goto initial_lock;
    }
    break;
   }
   if (!task_waitfor(abs_timeout)) {
    /* Timeout... (delete the read-descriptor) */
    rwlock_delete_readlock(desc);
    return false;
   }
   goto acquire_read_lock;
  }
  /* Try to increment the read-indirection */
 } while (!ATOMIC_CMPXCH_WEAK(self->rw_state,control_word,
                              __RWLOCK_INCIND(control_word)));
initial_lock:
 /* Initial read-lock. */
 desc->rl_recursion = 1;
 return true;
}

PUBLIC ATTR_NOTHROW bool KCALL
__os_rwlock_trywrite(struct rwlock *__restrict self) {
 u32 control_word;
#ifdef CONFIG_LOG_RWLOCKS
 debug_printf("[RWLOCK][%p] TRY_WRITE(%p)\n",THIS_TASK,self);
#endif
again:
 control_word = ATOMIC_READ(self->rw_state);
 switch (__RWLOCK_MODE(control_word)) {

 case RWLOCK_MODE_FWRITING:
  if (self->rw_xowner == THIS_TASK) {
   /* Recursive read-after-write. */
   ++self->rw_xind;
   return true;
  }
  assertf(!rwlock_find_readlock(self),
          "You can't be holding read locks when another thread is writing");
 case RWLOCK_MODE_FUPGRADING:
  return false;

 default: break;
 }
 /* Read-mode. */
 if (__RWLOCK_IND(control_word) == 0) {
  /* Direct switch to write-mode. */
  assertf(!rwlock_find_readlock(self),"Without any read-locks, how can you be holding any?");
  if (!ATOMIC_CMPXCH(self->rw_state,control_word,
                     __RWLOCK_STATE(RWLOCK_MODE_FWRITING,1)))
       goto again;
got_lock:
  self->rw_xowner = THIS_TASK;
  return true;
 } else {
  bool caller_has_lock;
  assert(__RWLOCK_IND(control_word) >= 1);
  caller_has_lock = rwlock_find_readlock(self) != NULL;
  /* Must use upgrade-mode to acquire the lock. */
  if (caller_has_lock && __RWLOCK_IND(control_word) == 1) {
   /* We seem to be the only reader. - Try to do an atomic upgrade. */
   if (!ATOMIC_CMPXCH(self->rw_state,control_word,
                      __RWLOCK_STATE(RWLOCK_MODE_FWRITING,1)))
        goto again;
   goto got_lock;
  }
  return false;
 }
}

PUBLIC bool KCALL
__os_rwlock_timedwrite(struct rwlock *__restrict self,
                       jtime_t abs_timeout) {
#if 0
 return __os_rwlock_timedwrite_aggressive(self,abs_timeout);
#else
 u32 control_word;
 assertf(!task_isconnected(),
         "You mustn't be connected when calling this function");
#ifdef CONFIG_LOG_RWLOCKS
 debug_printf("[RWLOCK][%p] WRITE(%p,%p)\n",THIS_TASK,self,self->rw_state);
#endif
again:
 control_word = ATOMIC_READ(self->rw_state);
 switch (__RWLOCK_MODE(control_word)) {

 case RWLOCK_MODE_FWRITING:
  if (self->rw_xowner == THIS_TASK) {
   /* Recursive read-after-write. */
   ++self->rw_xind;
   return true;
  }
  assertf(!rwlock_find_readlock(self),
          "You can't be holding read locks when another thread is writing");
  /* Wait for the write-mode to finish. */
wait_for_endwrite:
  TASK_POLL_BEFORE_CONNECT({
   COMPILER_READ_BARRIER();
   if (self->rw_mode == RWLOCK_MODE_FREADING)
       goto again;
  });
  task_connect(&self->rw_chmode);
  COMPILER_READ_BARRIER();
  if (self->rw_mode == RWLOCK_MODE_FREADING) {
   task_disconnect();
   goto again;
  }
  /* Wait for the lock to become available. */
  if (!task_waitfor(abs_timeout))
       return false; /* Timeout... */
  goto again;

 case RWLOCK_MODE_FUPGRADING:
  /* Lock is in upgrade-mode. */
  if (rwlock_find_readlock(self)) {
   struct exception_info *info;
   /* The caller is holding read-locks, that must be removed to prevent
    * a deadlock situation. For this purposes, the `E_RETRY_RWLOCK'
    * exception exists. */
   info                 = error_info();
   info->e_error.e_code = E_RETRY_RWLOCK;
   info->e_error.e_flag = ERR_FNORMAL;
   memset(info->e_error.e_pointers,0,sizeof(info->e_error.e_pointers));
   /* Save the R/W-lock responsible. */
   info->e_error.e_retry_rwlock.e_rwlock_ptr = self;
   error_throw_current();
   __builtin_unreachable();
  }
  goto wait_for_endwrite;


 default:
  /* Read-mode. */
  if (__RWLOCK_IND(control_word) == 0) {
   /* Direct switch to write-mode. */
   assertf(!rwlock_find_readlock(self),"Without any read-locks, how can you be holding any?");
   if (!ATOMIC_CMPXCH(self->rw_state,control_word,
                      __RWLOCK_STATE(RWLOCK_MODE_FWRITING,1)))
        goto again;
got_lock:
   self->rw_xowner = THIS_TASK;
   return true;
  } else {
   bool caller_has_lock;
   assert(__RWLOCK_IND(control_word) >= 1);
   caller_has_lock = rwlock_find_readlock(self) != NULL;
   /* Must use upgrade-mode to acquire the lock. */
   if (caller_has_lock && __RWLOCK_IND(control_word) == 1) {
    /* We seem to be the only reader. - Try to do an atomic upgrade. */
    if (!ATOMIC_CMPXCH(self->rw_state,control_word,
                       __RWLOCK_STATE(RWLOCK_MODE_FWRITING,1)))
         goto again;
    goto got_lock;
   }
   /* Switch to upgrade-mode. */
   if (!ATOMIC_CMPXCH(self->rw_state,control_word,
                      __RWLOCK_STATE(RWLOCK_MODE_FUPGRADING,
                                     __RWLOCK_IND(control_word)-
                                    (caller_has_lock ? 1 : 0))))
        goto again;
   TRY {
wait_for_unshare:
    /* Wait for the end of all readers to be signaled. */
    TASK_POLL_BEFORE_CONNECT({
     if unlikely(ATOMIC_CMPXCH(self->rw_state,
                               __RWLOCK_STATE(RWLOCK_MODE_FUPGRADING,0),
                               __RWLOCK_STATE(RWLOCK_MODE_FWRITING,1)))
        goto got_lock;
    });
    /* Connect to the unshare-signal. */
    task_connect(&self->rw_unshare);
    /* Check if the upgrade already finished (unlikely) */
    if unlikely(ATOMIC_CMPXCH(self->rw_state,
                              __RWLOCK_STATE(RWLOCK_MODE_FUPGRADING,0),
                              __RWLOCK_STATE(RWLOCK_MODE_FWRITING,1))) {
     task_disconnect();
     goto got_lock;
    }
    if (!task_waitfor(abs_timeout)) {
     /* Our timeout expired. - Re-acquire the read-lock (if we had one),
      * switch back to read-mode, and return `false' */
     do {
      control_word = ATOMIC_READ(self->rw_state);
      assert(__RWLOCK_MODE(control_word) == RWLOCK_MODE_FUPGRADING);
     } while (!ATOMIC_CMPXCH_WEAK(self->rw_state,control_word,
                                  __RWLOCK_STATE(RWLOCK_MODE_FREADING,
                                                 __RWLOCK_IND(control_word)+
                                                (caller_has_lock ? 1 : 0))));
     return false;
    }
   } EXCEPT (EXCEPT_EXECUTE_HANDLER) {
    /* Something went wrong. - Re-acquire the read-lock
     * (if we had one), switch back to read-mode. */
    do {
     control_word = ATOMIC_READ(self->rw_state);
     assert(__RWLOCK_MODE(control_word) == RWLOCK_MODE_FUPGRADING);
    } while (!ATOMIC_CMPXCH_WEAK(self->rw_state,control_word,
                                 __RWLOCK_STATE(RWLOCK_MODE_FREADING,
                                                __RWLOCK_IND(control_word)+
                                               (caller_has_lock ? 1 : 0))));
    error_rethrow();
   }
   /* Signal received. - Check if the unshared is complete. */
   goto wait_for_unshare;
  }
  break;
 }
#endif
}


PRIVATE void KCALL kill_reader(struct rwlock *__restrict lock) {
 struct exception_info *info;
 /* Check if the calling thread has a read-lock on `lock',
  * and throw an `E_RETRY_RWLOCK' exception if it does. */
 if (!rwlock_find_readlock(lock))
      return;
#if 0
 debug_printf("KILL_READER: %u for %p\n",posix_gettid(),lock);
#endif
 /* Apparently we are using that lock in particular.
  * So as already mentioned, to deal with this we simply throw
  * an exception that'll cause the read-lock to be re-acquired. */
 info = error_info();
 memset(info->e_error.e_pointers,0,sizeof(info->e_error.e_pointers));
 info->e_error.e_code                      = E_RETRY_RWLOCK;
 info->e_error.e_flag                      = ERR_FNORMAL;
 info->e_error.e_retry_rwlock.e_rwlock_ptr = lock;
 error_throw_current();
}

PRIVATE bool KCALL
kill_rwlock_reader(struct task *__restrict thread,
                   struct rwlock *__restrict lock) {
 struct read_locks *locks;
 if (thread == THIS_TASK)
     goto done; /* Skip our own thread. */
 locks = &FORTASK(thread,my_readlocks);
 if (!locks->rls_use)
      goto done; /* This thread isn't using any R/W-locks */
 if (thread->t_segment.ts_xcurrent.e_error.e_code == E_RETRY_RWLOCK &&
     thread->t_segment.ts_xcurrent.e_error.e_retry_rwlock.e_rwlock_ptr == lock)
     goto done; /* This thread is already in the process of dropping this read-lock. */
 if (locks->rls_vec == locks->rls_sbuf) {
  unsigned int i;
  /* We can search the thread's static read-lock map. */
  for (i = 0; i < COMPILER_LENOF(locks->rls_sbuf); ++i) {
   /* Use an atomic read to reduce the chances of
    * reading an incomplete pointer in SMP mode. */
   if (ATOMIC_LOAD(locks->rls_sbuf[i].rl_rwlock) == lock)
       goto use_rpc;
  }
  /* Check again if the thread is still using its static buffer.
   * NOTE: Event if the thread temporarily used a dynamic
   *       buffer while we were checking its static buffer,
   *       we're trying to look for threads that were already
   *       using our R/W-lock when we changed the lock to
   *       upgrade-mode. So with that in mind, a thread that
   *       wasn't using our lock at that point won't show up
   *       here either. An those that were using it and didn't
   *       show up just now are of no interest to us, as those
   *       are the ones that already got rid of their locks
   *       to our R/W-lock.
   * This check is only required to ensure that the thread didn't
   * migrate its lock to our r/w-lock while we were checking its
   * set of used locks. */
  if (locks->rls_vec == locks->rls_sbuf)
      goto use_rpc;
 } else {
use_rpc:
  /* Send an RPC to the thread to check if it's using our lock. */
  task_queue_rpc(thread,(task_rpc_t)&kill_reader,lock,TASK_RPC_SYNC);
 }
done:
 return true;
}

PRIVATE void KCALL
rwlock_kill_readers(struct rwlock *__restrict self) {
 /* Enumerate all running threads to find (and kill)
  * those that are using our R/W-lock. */
 task_foreach_weak_running((bool(KCALL *)(struct task *__restrict,void*))&kill_rwlock_reader,self);
}


PUBLIC bool KCALL
__os_rwlock_timedwrite_aggressive(struct rwlock *__restrict self,
                                 jtime_t abs_timeout) {
 u32 control_word;
 assertf(!task_isconnected(),
         "You mustn't be connected when calling this function");
#ifdef CONFIG_LOG_RWLOCKS
 debug_printf("[RWLOCK][%p] WRITE(%p,%p)\n",THIS_TASK,self,self->rw_state);
#endif
again:
 control_word = ATOMIC_READ(self->rw_state);
 switch (__RWLOCK_MODE(control_word)) {

 case RWLOCK_MODE_FWRITING:
  if (self->rw_xowner == THIS_TASK) {
   /* Recursive read-after-write. */
   ++self->rw_xind;
   return true;
  }
  assertf(!rwlock_find_readlock(self),
          "You can't be holding read locks when another thread is writing");
  /* Wait for the write-mode to finish. */
wait_for_endwrite:
  TASK_POLL_BEFORE_CONNECT({
   COMPILER_READ_BARRIER();
   if (self->rw_mode == RWLOCK_MODE_FREADING)
       goto again;
  });
  task_connect(&self->rw_chmode);
  COMPILER_READ_BARRIER();
  if (self->rw_mode == RWLOCK_MODE_FREADING) {
   task_disconnect();
   goto again;
  }
  /* Wait for the lock to become available. */
  if (!task_waitfor(abs_timeout))
       return false; /* Timeout... */
  goto again;

 case RWLOCK_MODE_FUPGRADING:
  /* Lock is in upgrade-mode. */
  if (rwlock_find_readlock(self)) {
   struct exception_info *info;
   /* The caller is holding read-locks, that must be removed to prevent
    * a deadlock situation. For this purposes, the `E_RETRY_RWLOCK'
    * exception exists. */
   info                 = error_info();
   info->e_error.e_code = E_RETRY_RWLOCK;
   info->e_error.e_flag = ERR_FNORMAL;
   memset(info->e_error.e_pointers,0,sizeof(info->e_error.e_pointers));
   /* Save the R/W-lock responsible. */
   info->e_error.e_retry_rwlock.e_rwlock_ptr = self;
   error_throw_current();
   __builtin_unreachable();
  }
  goto wait_for_endwrite;


 default:
  /* Read-mode. */
  if (__RWLOCK_IND(control_word) == 0) {
   /* Direct switch to write-mode. */
   assertf(!rwlock_find_readlock(self),"Without any read-locks, how can you be holding any?");
   if (!ATOMIC_CMPXCH(self->rw_state,control_word,
                      __RWLOCK_STATE(RWLOCK_MODE_FWRITING,1)))
        goto again;
got_lock:
   self->rw_xowner = THIS_TASK;
   return true;
  } else {
   bool caller_has_lock;
   assert(__RWLOCK_IND(control_word) >= 1);
   caller_has_lock = rwlock_find_readlock(self) != NULL;
   /* Must use upgrade-mode to acquire the lock. */
   if (caller_has_lock && __RWLOCK_IND(control_word) == 1) {
    /* We seem to be the only reader. - Try to do an atomic upgrade. */
    if (!ATOMIC_CMPXCH(self->rw_state,control_word,
                       __RWLOCK_STATE(RWLOCK_MODE_FWRITING,1)))
         goto again;
    goto got_lock;
   }
   /* Switch to upgrade-mode. */
   if (!ATOMIC_CMPXCH(self->rw_state,control_word,
                      __RWLOCK_STATE(RWLOCK_MODE_FUPGRADING,
                                     __RWLOCK_IND(control_word)-
                                    (caller_has_lock ? 1 : 0))))
        goto again;
   TRY {
wait_for_unshare:
    /* Wait for the end of all readers to be signaled. */
    TASK_POLL_BEFORE_CONNECT({
     if unlikely(ATOMIC_CMPXCH(self->rw_state,
                               __RWLOCK_STATE(RWLOCK_MODE_FUPGRADING,0),
                               __RWLOCK_STATE(RWLOCK_MODE_FWRITING,1)))
        goto got_lock;
    });
    rwlock_kill_readers(self);
    TASK_POLL_BEFORE_CONNECT({
     if unlikely(ATOMIC_CMPXCH(self->rw_state,
                               __RWLOCK_STATE(RWLOCK_MODE_FUPGRADING,0),
                               __RWLOCK_STATE(RWLOCK_MODE_FWRITING,1)))
        goto got_lock;
    });

    /* Connect to the unshare-signal. */
    task_connect(&self->rw_unshare);
    /* Check if the upgrade already finished (unlikely) */
    if unlikely(ATOMIC_CMPXCH(self->rw_state,
                              __RWLOCK_STATE(RWLOCK_MODE_FUPGRADING,0),
                              __RWLOCK_STATE(RWLOCK_MODE_FWRITING,1))) {
     task_disconnect();
     goto got_lock;
    }
    rwlock_kill_readers(self);
    if unlikely(ATOMIC_CMPXCH(self->rw_state,
                              __RWLOCK_STATE(RWLOCK_MODE_FUPGRADING,0),
                              __RWLOCK_STATE(RWLOCK_MODE_FWRITING,1))) {
     task_disconnect();
     goto got_lock;
    }
    /* Wait for a while, assuming that readers behave as they should
     * and release their locks before entering a wait-state when trying to
     * reacquire them, but noticing that the lock is in UPGRADING mode.
     * NOTE: We use a small timeout so we can try again in case we missed something. */
    task_waitfor(jiffies + JIFFIES_FROM_MILLI(200));
   } EXCEPT (EXCEPT_EXECUTE_HANDLER) {
    /* Something went wrong. - Re-acquire the read-lock
     * (if we had one), switch back to read-mode. */
    do {
     control_word = ATOMIC_READ(self->rw_state);
     assert(__RWLOCK_MODE(control_word) == RWLOCK_MODE_FUPGRADING);
    } while (!ATOMIC_CMPXCH_WEAK(self->rw_state,control_word,
                                 __RWLOCK_STATE(RWLOCK_MODE_FREADING,
                                                __RWLOCK_IND(control_word)+
                                               (caller_has_lock ? 1 : 0))));
    error_rethrow();
   }
   /* Signal received. - Check if the unshared is complete. */
   goto wait_for_unshare;
  }
  break;
 }
}

PUBLIC bool KCALL
__os_rwlock_tryupgrade(struct rwlock *__restrict self) {
 /* Special handling to upgrade a non-shared
  * read-lock to an exclusive write-lock. */
 u32 control_word;
#ifdef CONFIG_LOG_RWLOCKS
 debug_printf("[RWLOCK][%p] TRY_UPGRADE(%p)\n",THIS_TASK,self);
#endif
again:
 control_word = ATOMIC_READ(self->rw_state);
 switch (__RWLOCK_MODE(control_word)) {

 case RWLOCK_MODE_FWRITING:
  assertf(self->rw_xowner == THIS_TASK,
          "You can't be holding read-locks on a write-mode R/W-lock that you don't own");
  assertf(self->rw_xind >= 2,
          "You're not upgrading a secondary read-lock, but your code "
          "contains a single `rwlock_write()', followed by `rwlock_upgrade()'");
  /* Nothing to do here. The previous read-lock already counted as
   * a write-lock because the R/W-lock is in recursive write-mode. */
  return true;
 case RWLOCK_MODE_FUPGRADING:
  return false;
 default: break;
 }
 /* Read-mode. */
 assertf(rwlock_find_readlock(self),"You're not holding any read-locks");
 assertf(__RWLOCK_IND(control_word) >= 1,
         "Inconsistent R/W-lock state: You're holding "
         "read-locks that the lock knows nothing about");
 if (__RWLOCK_IND(control_word) == 1) {
  /* We seem to be the only reader. - Try to do an atomic upgrade. */
  if (!ATOMIC_CMPXCH(self->rw_state,control_word,
                     __RWLOCK_STATE(RWLOCK_MODE_FWRITING,1)))
       goto again;
  /* Atomically upgraded to write-mode. */
  self->rw_xowner = THIS_TASK;
  /* TODO: Decrement the recursion counter of our read-lock, and delete
   *       it if that counter reaches ZERO(0) (it's an upgrade, so it
   *       replaces the last read-lock with a write-lock, meaning a
   *       non-recursive read-lock has to go away!) */
  return true;
 }
 return false;
}



PUBLIC bool KCALL
__os_rwlock_timedupgrade(struct rwlock *__restrict self,
                         jtime_t abs_timeout) {
 /* Special handling to upgrade a non-shared
  * read-lock to an exclusive write-lock. */
 u32 control_word;
#ifdef CONFIG_LOG_RWLOCKS
 debug_printf("[RWLOCK][%p] UPGRADE(%p)\n",THIS_TASK,self);
#endif
 assertf(!task_isconnected(),
         "You mustn't be connected when calling this function");
again:
 control_word = ATOMIC_READ(self->rw_state);
 switch (__RWLOCK_MODE(control_word)) {

 case RWLOCK_MODE_FWRITING:
  assertf(self->rw_xowner == THIS_TASK,
          "You can't be holding read-locks on a write-mode R/W-lock that you don't own");
  assertf(self->rw_xind >= 2,
          "You're not upgrading a secondary read-lock, but your code "
          "contains a single `rwlock_write()', followed by `rwlock_upgrade()'");
  /* Nothing to do here. The previous read-lock already counted as
   * a write-lock because the R/W-lock is in recursive write-mode. */
  return true;

 {
  struct exception_info *info;
 case RWLOCK_MODE_FUPGRADING:
  /* Lock is in upgrade-mode. */
  assertf(rwlock_find_readlock(self),"You're not holding any read-locks");
  /* The caller is holding read-locks, that must be removed to prevent
   * a deadlock situation. For this purposes, the `E_RETRY_RWLOCK'
   * exception exists. */
  info         = error_info();
  info->e_error.e_code = E_RETRY_RWLOCK;
  info->e_error.e_flag = ERR_FNORMAL;
  memset(info->e_error.e_pointers,0,sizeof(info->e_error.e_pointers));
  /* Save the R/W-lock responsible. */
  info->e_error.e_retry_rwlock.e_rwlock_ptr = self;
  error_throw_current();
  __builtin_unreachable();
 } break;

 {
  struct rwlock *EXCEPT_VAR xself;
 default:
  /* Read-mode. */
  assertf(rwlock_find_readlock(self),"You're not holding any read-locks");
  assertf(__RWLOCK_IND(control_word) >= 1,
          "Inconsistent R/W-lock state: You're holding "
          "read-locks that the lock knows nothing about");
  if (__RWLOCK_IND(control_word) == 1) {
   /* We seem to be the only reader. - Try to do an atomic upgrade. */
   if (!ATOMIC_CMPXCH(self->rw_state,control_word,
                      __RWLOCK_STATE(RWLOCK_MODE_FWRITING,1)))
        goto again;
got_lock:
   /* Atomically upgraded to write-mode. */
   self->rw_xowner = THIS_TASK;
   return true;
  }
  /* Switch to upgrade-mode. */
  if (!ATOMIC_CMPXCH(self->rw_state,control_word,
                     __RWLOCK_STATE(RWLOCK_MODE_FUPGRADING,
                                    __RWLOCK_IND(control_word)-1)))
       goto again;
  xself = self;
  TRY {
wait_for_unshare:
   /* Wait for the end of all readers to be signaled. */
   TASK_POLL_BEFORE_CONNECT({
    if unlikely(ATOMIC_CMPXCH(self->rw_state,
                              __RWLOCK_STATE(RWLOCK_MODE_FUPGRADING,0),
                              __RWLOCK_STATE(RWLOCK_MODE_FWRITING,1)))
       goto got_lock;
   });
   /* Connect to the unshare-signal. */
   task_connect(&self->rw_unshare);
   /* Check if the upgrade already finished (unlikely) */
   if unlikely(ATOMIC_CMPXCH(self->rw_state,
                             __RWLOCK_STATE(RWLOCK_MODE_FUPGRADING,0),
                             __RWLOCK_STATE(RWLOCK_MODE_FWRITING,1))) {
    task_disconnect();
    goto got_lock;
   }
   if (!task_waitfor(abs_timeout)) {
    /* Our timeout expired. - Re-acquire the read-lock,
     * switch back to read-mode, and return `false' */
    do {
     control_word = ATOMIC_READ(self->rw_state);
     assert(__RWLOCK_MODE(control_word) == RWLOCK_MODE_FUPGRADING);
    } while (!ATOMIC_CMPXCH_WEAK(self->rw_state,control_word,
                                 __RWLOCK_STATE(RWLOCK_MODE_FREADING,
                                                __RWLOCK_IND(control_word)+1)));
    return false;
   }
  } EXCEPT (EXCEPT_EXECUTE_HANDLER) {
   /* Something went wrong. - Re-acquire the read-lock and switch back to read-mode. */
   do {
    control_word = ATOMIC_READ(xself->rw_state);
    assert(__RWLOCK_MODE(control_word) == RWLOCK_MODE_FUPGRADING);
   } while (!ATOMIC_CMPXCH_WEAK(xself->rw_state,control_word,
                                __RWLOCK_STATE(RWLOCK_MODE_FREADING,
                                               __RWLOCK_IND(control_word)+1)));
   error_rethrow();
  }
  /* Signal received. - Check if the unshared is complete. */
  goto wait_for_unshare;
 } break;
 }
}



PUBLIC void KCALL
__os_rwlock_downgrade(struct rwlock *__restrict self) {
#ifdef CONFIG_LOG_RWLOCKS
 debug_printf("[RWLOCK][%p] DOWNGRADE(%p)\n",THIS_TASK,self);
#endif
 assertf(self->rw_mode == RWLOCK_MODE_FWRITING,
         "Lock isn't in write-mode");
 assertf(self->rw_xowner == THIS_TASK,
         "You're not the owner of this lock");
 assertf(self->rw_xind >= 1,
         "Inconsistent R/W-lock state");
 if (self->rw_xind == 1) {
  /* Increment the read-recursion for the lock. */
  rwlock_get_readlock(self)->rl_recursion += 1;
  /* Do the downgrade (keep the level#1 indirection as reader-counter). */
  ATOMIC_WRITE(self->rw_mode,RWLOCK_MODE_FREADING);
  /* Signal the downgrade. */
  sig_broadcast(&self->rw_chmode);
 } else {
  /* downgrading a recursive read-after-write lock is a no-op. */
 }
}

PUBLIC ATTR_NOTHROW void KCALL
__os_rwlock_endwrite(struct rwlock *__restrict self) {
#ifdef CONFIG_LOG_RWLOCKS
 debug_printf("[RWLOCK][%p] END_WRITE(%p)\n",THIS_TASK,self);
#endif
 assertf(self->rw_mode == RWLOCK_MODE_FWRITING,
         "Lock isn't in write-mode (%p:%p)",
         self,self->rw_state);
 assertf(self->rw_xowner == THIS_TASK,
         "You're not the owner of this lock");
 assertf(self->rw_xind >= 1,
         "Inconsistent R/W-lock state");
 if (self->rw_xind == 1) {
  struct read_lock *desc;
  /* Clear the owner-field. */
  ATOMIC_WRITE(self->rw_xowner,NULL);
  /* If our thread has older read-locks, restore them. */
  desc = rwlock_find_readlock(self);
  assert(!desc || desc->rl_rwlock == self);
  if (desc) {
   assert(desc->rl_recursion != 0);
   /* Downgrade to read-mode (keep the indirection of `1'). */
   assert(self->rw_scnt == 1);
   ATOMIC_WRITE(self->rw_mode,RWLOCK_MODE_FREADING);
  } else {
   /* Last lock. */
   ATOMIC_WRITE(self->rw_state,__RWLOCK_STATE(RWLOCK_MODE_FREADING,0));
  }
  /* Signal the unlock. */
  sig_broadcast(&self->rw_chmode);
 } else {
  /* Recursively stop writing. */
  --self->rw_xind;
 }
}

/* endread() and end() cannot be differenciated
 * without additional debug informations. */
DEFINE_PUBLIC_ALIAS(rwlock_endread,rwlock_end);

PUBLIC ATTR_NOTHROW bool KCALL
__os_rwlock_end(struct rwlock *__restrict self) {
#ifdef CONFIG_LOG_RWLOCKS
 debug_printf("[RWLOCK][%p] END(%p)\n",THIS_TASK,self);
#endif
 if (self->rw_mode == RWLOCK_MODE_FWRITING) {
  /* end-read after already writing */
  assertf(self->rw_xowner == THIS_TASK,
          "You're not the owner of this lock");
  assertf(self->rw_xind >= 1,
          "Inconsistent R/W-lock state");
  if (self->rw_xind == 1) {
   struct read_lock *desc;
   /* Clear the owner-field. */
   ATOMIC_WRITE(self->rw_xowner,NULL);
   /* If our thread has older read-locks, restore them. */
   desc = rwlock_find_readlock(self);
   if (desc) {
    assert(desc->rl_recursion != 0);
    /* Downgrade to read-mode (keep the indirection of `1'). */
    ATOMIC_WRITE(self->rw_mode,RWLOCK_MODE_FREADING);
   } else {
    /* Last lock. */
    ATOMIC_WRITE(self->rw_state,__RWLOCK_STATE(RWLOCK_MODE_FREADING,0));
   }
   /* Signal the unlock. */
   sig_broadcast(&self->rw_chmode);
  } else {
   /* Recursively stop writing. */
   --self->rw_xind;
  }
 } else {
  struct read_lock *desc;
  /* Drop a read-lock. */
  desc = rwlock_find_readlock(self);
  assertf(desc,
          "You're not holding any read-locks\n"
          "self                          = %p\n"
          "PERTASK(my_readlocks).rls_use = %Iu\n"
          "PERTASK(my_readlocks).rls_cnt = %Iu",
          self,
          PERTASK_GET(my_readlocks.rls_use),
          PERTASK_GET(my_readlocks.rls_cnt));
  assertf(self->rw_scnt >= 1,
          "Noone is holding read-locks");
  assert(desc->rl_recursion != 0);
  if (desc->rl_recursion == 1) {
   u32 control_word;
   struct exception_info *error;
   /* Last read-lock is being removed. (decrement the
    * reader-counter of the lock and delete the descriptor) */
   rwlock_delete_readlock(desc);
   do {
    control_word = ATOMIC_READ(self->rw_state);
    assert(__RWLOCK_MODE(control_word) != RWLOCK_MODE_FWRITING);
    assert(__RWLOCK_IND(control_word) != 0);
   } while (!ATOMIC_CMPXCH_WEAK(self->rw_state,control_word,
                                __RWLOCK_DECIND(control_word)));
   /* If the last reader went away, send the downgrade signal.
    * NOTE: Since the lock is now ready for exclusive locking,
    *       only signal a single thread to improve its chances
    *       of acquiring that lock. */
   if (__RWLOCK_IND(control_word) == 1)
       sig_send(&self->rw_unshare,1);

   /* Deal with parallel-upgrade exceptions. */
   error = error_info();
   if (error->e_error.e_code == E_RETRY_RWLOCK &&
       error->e_error.e_retry_rwlock.e_rwlock_ptr == self) {
    error->e_error.e_retry_rwlock.e_rwlock_ptr = NULL;
    /* Try to yield to the task that is waiting for the lock to become available. */
    task_yield();
    /* Get the caller to re-try acquiring this lock. */
    return true;
   }
  } else {
   /* Recursively release a read-lock, but don't signal the
    * R/W-lock because it's not the last one we're holding. */
   --desc->rl_recursion;
  }
 }
 return false;
}

PUBLIC unsigned int KCALL
rwlock_poll(struct rwlock *__restrict self, unsigned int mode) {
 unsigned int result; u32 state;
 /* Use ghost connections to prevent the deadlock
  * scenario described by `task_connect_ghost()' */
 task_connect_ghost(&self->rw_chmode);
 COMPILER_READ_BARRIER();
 /* Check the state of the R/W lock. */
 state = ATOMIC_READ(self->rw_state);
 if (__RWLOCK_MODE(state) == RWLOCK_MODE_FREADING) {
  result = POLLIN; /* Read locks are available. */
  if (__RWLOCK_IND(state) == 0 ||
     (__RWLOCK_IND(state) == 1 && rwlock_get_readlock(self)))
      result |= POLLOUT; /* No [other] readers -> Write lock is available. */
 } else if (self->rw_xowner == THIS_TASK) {
  /* Caller is holding the write-lock. */
  result = POLLIN|POLLOUT;
 } else {
  /* Another thread is writing/upgrading. */
  result = 0;
 }
 return result & mode;
}



DECL_END

#endif /* !GUARD_KERNEL_SRC_SCHED_RWLOCK_C */
