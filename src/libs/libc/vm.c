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
#ifndef GUARD_LIBS_LIBC_VM_C
#define GUARD_LIBS_LIBC_VM_C 1

#include "libc.h"
#include "vm.h"
#include "system.h"
#include "errno.h"
#include "unistd.h"

#include <hybrid/align.h>
#include <hybrid/sync/atomic-rwlock.h>
#include <hybrid/section.h>
#include <sys/mman.h>
#include <errno.h>

DECL_BEGIN

EXPORT(mmap64,libc_mmap64);
INTERN void *LIBCCALL
libc_mmap64(void *addr, size_t len, int prot,
            int flags, fd_t fd, pos64_t offset) {
#if __SIZEOF_SYSCALL_LONG__ < 8
 if (offset > (syscall_ulong_t)-1 && fd >= 0) {
  struct mmap_info_v1 info;
  info.mi_prot          = prot;
  info.mi_flags         = flags;
  info.mi_xflag         = XMAP_FINDAUTO;
  info.mi_addr          = addr;
  info.mi_size          = len;
  info.mi_align         = PAGESIZE;
  info.mi_gap           = PAGESIZE*16;
  info.mi_virt.mv_file  = fd;
  info.mi_virt.mv_begin = 0;
  info.mi_virt.mv_off   = offset;
  info.mi_virt.mv_len   = CEIL_ALIGN(len,PAGESIZE);
  info.mi_virt.mv_fill  = 0;
  info.mi_virt.mv_guard = PAGESIZE;
  info.mi_virt.mv_funds = MMAP_VIRT_MAXFUNDS;
  return libc_xmmap1(&info);
 }
#endif
 return libc_mmap(addr,len,prot,flags,fd,(syscall_ulong_t)offset);
}

EXPORT(msync,libc_msync);
INTERN int LIBCCALL
libc_msync(void *addr, size_t len, int flags) {
 libc_seterrno(ENOSYS);
 return -1;
}

EXPORT(mlock,libc_mlock);
INTERN int LIBCCALL
libc_mlock(void const *addr, size_t len) {
 libc_seterrno(ENOSYS);
 return -1;
}

EXPORT(munlock,libc_munlock);
INTERN int LIBCCALL
libc_munlock(void const *addr, size_t len) {
 libc_seterrno(ENOSYS);
 return -1;
}

EXPORT(mlockall,libc_mlockall);
INTERN int LIBCCALL
libc_mlockall(int flags) {
 libc_seterrno(ENOSYS);
 return -1;
}


EXPORT(munlockall,libc_munlockall);
INTERN int LIBCCALL
libc_munlockall(void) {
 libc_seterrno(ENOSYS); return -1;
}

EXPORT(madvise,libc_madvise);
INTERN int LIBCCALL
libc_madvise(void *addr, size_t len, int advice) {
 libc_seterrno(ENOSYS);
 return -1;
}

EXPORT(mincore,libc_mincore);
INTERN int LIBCCALL
libc_mincore(void *start, size_t len, unsigned char *vec) {
 libc_seterrno(ENOSYS);
 return -1;
}

EXPORT(posix_madvise,libc_posix_madvise);
INTERN int LIBCCALL
libc_posix_madvise(void *addr, size_t len, int advice) {
 libc_seterrno(ENOSYS);
 return -1;
}

EXPORT(remap_file_pages,libc_remap_file_pages);
INTERN int LIBCCALL
libc_remap_file_pages(void *start, size_t size, int prot, size_t pgoff, int flags) {
 libc_seterrno(ENOSYS);
 return -1;
}

PRIVATE ATTR_SECTION(".rodata.str.rare")
char const dev_shm[] = "::/dev/shm";

CRT_RARE_DATA int shm_fd = -1;
CRT_RARE int LIBCCALL libc_shm_getfd(void) {
 fd_t fd,old_fd;
 if (shm_fd >= 0) return shm_fd;
 fd = libc_open(dev_shm,O_RDONLY|O_DIRECTORY);
 if (fd >= 0 && fd < 4096) {
  /* Use some high number to not take up (sometimes) more valuable lower numbers. */
  old_fd = libc_fcntl(fd,F_DUPFD_CLOEXEC,4096);
  if (old_fd >= 0) libc_close(fd),fd = old_fd;
  old_fd = ATOMIC_CMPXCH_VAL(shm_fd,-1,fd);
  if unlikely(old_fd >= 0) libc_close(fd),fd = old_fd;
 }
 return fd;
}

EXPORT(__KSYM(shm_open),libc_shm_open);
CRT_RARE int LIBCCALL
libc_shm_open(char const *name, oflag_t oflag, mode_t mode) {
 int shm_fd;
 if unlikely(!name) { libc_seterrno(EINVAL); return -1; }
 while (*name == '/' || (*name == '\\' &&
       ((oflag&O_DOSPATH) ? !LIBC_DOSMODE_DISABLED()
                          :  LIBC_DOSMODE_ENABLED())))
       ++name;
 if ((shm_fd = libc_shm_getfd()) < 0) return -1;
 /* NOTE: Append `O_NOFOLLOW' to go along `AT_SYMLINK_NOFOLLOW' in shm_unlink(). */
 return libc_openat(shm_fd,name,oflag|O_NOFOLLOW,mode);
}

EXPORT(__DSYM(shm_open),libc_dos_shm_open);
CRT_DOS_EXT int LIBCCALL
libc_dos_shm_open(char const *name, oflag_t oflag, mode_t mode) {
 return libc_shm_open(name,oflag|O_DOSPATH,mode);
}

EXPORT(__KSYM(shm_unlink),libc_shm_unlink);
CRT_RARE int LIBCCALL
libc_shm_unlink(char const *name) {
 int shm_fd;
 if unlikely(!name) { libc_seterrno(EINVAL); return -1; }
 while (*name == '/' ||
       (*name == '\\' && LIBC_DOSMODE_ENABLED()))
       ++name;
 if ((shm_fd = libc_shm_getfd()) < 0) return -1;
 return libc_unlinkat(shm_fd,name,AT_SYMLINK_NOFOLLOW);
}

EXPORT(__DSYM(shm_unlink),libc_dos_shm_unlink);
CRT_DOS_EXT int LIBCCALL
libc_dos_shm_unlink(char const *name) {
 int shm_fd;
 if unlikely(!name) { libc_seterrno(EINVAL); return -1; }
 while (*name == '/' ||
       (*name == '\\' && !LIBC_DOSMODE_DISABLED()))
       ++name;
 if ((shm_fd = libc_shm_getfd()) < 0) return -1;
 return libc_unlinkat(shm_fd,name,AT_SYMLINK_NOFOLLOW|AT_DOSPATH);
}



EXPORT(xmmap1,libc_xmmap1);
INTERN void *LIBCCALL
libc_xmmap1(struct mmap_info const *data) {
 return Esys_xmmap(MMAP_INFO_CURRENT,data);
}

extern byte_t _end[]; /* Automatically defined by the linker (end of '.bss'). */
PRIVATE byte_t *brk_curr = NULL;
PRIVATE DEFINE_ATOMIC_RWLOCK(brk_lock);

PRIVATE int LIBCCALL do_brk(void *addr) {
 byte_t *real_oldbrk,*real_newbrk;
 if ((real_oldbrk = brk_curr) == NULL)
      real_oldbrk = (byte_t *)CEIL_ALIGN((uintptr_t)_end,PAGESIZE);
 else real_oldbrk = (byte_t *)CEIL_ALIGN((uintptr_t)real_oldbrk,PAGESIZE);
 real_newbrk = (byte_t *)CEIL_ALIGN((uintptr_t)addr,PAGESIZE);
 if (real_newbrk < real_oldbrk) {
  /* Release memory */
  if unlikely(libc_munmap(real_newbrk,real_oldbrk-real_newbrk) == -1)
     return -1;
 } else if (real_newbrk > real_oldbrk) {
  void *map_result;
  /* Allocate more memory */
  map_result = libc_mmap(real_oldbrk,real_newbrk-real_oldbrk,
                         PROT_READ|PROT_WRITE,
                         MAP_FIXED|MAP_ANONYMOUS,-1,0);
  if unlikely(map_result == MAP_FAILED) return -1;
  assertf(map_result == real_oldbrk,"%p != %p",map_result,real_oldbrk);
 }
 brk_curr = (byte_t *)addr;
 return 0;
}

EXPORT(brk,libc_brk);
INTERN int LIBCCALL libc_brk(void *addr) {
 int result;
 atomic_rwlock_write(&brk_lock);
 result = do_brk(addr);
 atomic_rwlock_endwrite(&brk_lock);
 return result;
}

EXPORT(sbrk,libc_sbrk);
INTERN void *LIBCCALL libc_sbrk(intptr_t delta) {
 byte_t *result;
 atomic_rwlock_write(&brk_lock);
 if ((result = brk_curr) == NULL)
      result = (byte_t *)CEIL_ALIGN((uintptr_t)_end,PAGESIZE);
 if (do_brk(result+delta) != 0) result = (byte_t *)-1;
 atomic_rwlock_endwrite(&brk_lock);
 return result;
}





EXPORT(Xmmap64,libc_Xmmap64);
CRT_EXCEPT void *LIBCCALL
libc_Xmmap64(void *addr, size_t len, int prot,
             int flags, fd_t fd, pos64_t offset) {
#if __SIZEOF_SYSCALL_LONG__ < 8
 if (offset > (syscall_ulong_t)-1 && fd >= 0) {
  struct mmap_info_v1 info;
  info.mi_prot          = prot;
  info.mi_flags         = flags;
  info.mi_xflag         = XMAP_FINDAUTO;
  info.mi_addr          = addr;
  info.mi_size          = len;
  info.mi_align         = PAGESIZE;
  info.mi_gap           = PAGESIZE*16;
  info.mi_virt.mv_file  = fd;
  info.mi_virt.mv_begin = 0;
  info.mi_virt.mv_off   = offset;
  info.mi_virt.mv_len   = CEIL_ALIGN(len,PAGESIZE);
  info.mi_virt.mv_fill  = 0;
  info.mi_virt.mv_guard = PAGESIZE;
  info.mi_virt.mv_funds = MMAP_VIRT_MAXFUNDS;
  return libc_Xxmmap1(&info);
 }
#endif
 return Xsys_mmap(addr,len,prot,flags,fd,(syscall_ulong_t)offset);
}

#if !defined(CONFIG_LIBCCALL_IS_CDECL) || \
    !defined(CONFIG_VAARGS_IS_HIDDEN_ARGS)
EXPORT(Xmremap,libc_Xmremap);
CRT_EXCEPT void *ATTR_CDECL
libc_Xmremap(void *addr, size_t old_len, size_t new_len, int flags, ...) {
 void *COMPILER_IGNORE_UNINITIALIZED(result);
 va_list __EXCEPTVAR_VALIST args;
 va_start(args,flags);
 __TRY_VALIST {
  result = Xsys_mremap(addr,old_len,new_len,flags,va_arg(args,void *));
 } __FINALLY_VALIST {
  va_end(args);
 }
 return result;
}
#endif

EXPORT(Xmsync,libc_Xmsync);
CRT_EXCEPT void LIBCCALL
libc_Xmsync(void *addr, size_t len, int flags) {
 libc_error_throw(E_NOT_IMPLEMENTED);
}

EXPORT(Xmlock,libc_Xmlock);
CRT_EXCEPT void LIBCCALL
libc_Xmlock(void const *addr, size_t len) {
 libc_error_throw(E_NOT_IMPLEMENTED);
}

EXPORT(Xmunlock,libc_Xmunlock);
CRT_EXCEPT void LIBCCALL
libc_Xmunlock(void const *addr, size_t len) {
 libc_error_throw(E_NOT_IMPLEMENTED);
}

EXPORT(Xmlockall,libc_Xmlockall);
CRT_EXCEPT void LIBCCALL
libc_Xmlockall(int flags) {
 libc_error_throw(E_NOT_IMPLEMENTED);
}

EXPORT(Xmunlockall,libc_Xmunlockall);
CRT_EXCEPT void LIBCCALL
libc_Xmunlockall(void) {
 libc_error_throw(E_NOT_IMPLEMENTED);
}

EXPORT(Xmadvise,libc_Xmadvise);
CRT_EXCEPT void LIBCCALL
libc_Xmadvise(void *addr, size_t len, int advice) {
 libc_error_throw(E_NOT_IMPLEMENTED);
}

EXPORT(Xmincore,libc_Xmincore);
CRT_EXCEPT void LIBCCALL
libc_Xmincore(void *start, size_t len, unsigned char *vec) {
 libc_error_throw(E_NOT_IMPLEMENTED);
}

EXPORT(Xposix_madvise,libc_Xposix_madvise);
CRT_EXCEPT void LIBCCALL
libc_Xposix_madvise(void *addr, size_t len, int advice) {
 libc_error_throw(E_NOT_IMPLEMENTED);
}

EXPORT(Xremap_file_pages,libc_Xremap_file_pages);
CRT_EXCEPT void LIBCCALL
libc_Xremap_file_pages(void *start, size_t size, int prot, size_t pgoff, int flags) {
 libc_error_throw(E_NOT_IMPLEMENTED);
}

CRT_EXCEPT int LIBCCALL libc_Xshm_getfd(void) {
 fd_t fd,old_fd;
 if (shm_fd >= 0) return shm_fd;
 fd = libc_Xopen(dev_shm,O_RDONLY|O_DIRECTORY);
 if (fd >= 0 && fd < 4096) {
  /* Use some high number to not take up (sometimes) more valuable lower numbers. */
  old_fd = libc_fcntl(fd,F_DUPFD_CLOEXEC,4096);
  if (old_fd >= 0) libc_close(fd),fd = old_fd;
  old_fd = ATOMIC_CMPXCH_VAL(shm_fd,-1,fd);
  if unlikely(old_fd >= 0) libc_close(fd),fd = old_fd;
 }
 return fd;
}

EXPORT(Xshm_open,libc_Xshm_open);
CRT_EXCEPT int LIBCCALL
libc_Xshm_open(char const *name, oflag_t oflag, mode_t mode) {
 if unlikely(!name)
    error_throw(E_INVALID_ARGUMENT);
 while (*name == '/' || (*name == '\\' &&
       ((oflag&O_DOSPATH) ? !LIBC_DOSMODE_DISABLED()
                          :  LIBC_DOSMODE_ENABLED())))
       ++name;
 /* NOTE: Append `O_NOFOLLOW' to go along `AT_SYMLINK_NOFOLLOW' in shm_unlink(). */
 return libc_Xopenat(libc_Xshm_getfd(),
                     name,
                     oflag|O_NOFOLLOW,
                     mode);
}

EXPORT(Xshm_unlink,libc_Xshm_unlink);
CRT_EXCEPT void LIBCCALL
libc_Xshm_unlink(char const *name) {
 if unlikely(!name)
    error_throw(E_INVALID_ARGUMENT);
 while (*name == '/' ||
       (*name == '\\' && LIBC_DOSMODE_ENABLED()))
       ++name;
 libc_Xunlinkat(libc_Xshm_getfd(),
                name,
                AT_SYMLINK_NOFOLLOW);
}

EXPORT(Xxmmap1,libc_Xxmmap1);
CRT_EXCEPT void *LIBCCALL
libc_Xxmmap1(struct mmap_info const *data) {
 return Xsys_xmmap(MMAP_INFO_CURRENT,data);
}

CRT_EXCEPT void LIBCCALL do_Xbrk(void *addr) {
 byte_t *real_oldbrk,*real_newbrk;
 if ((real_oldbrk = brk_curr) == NULL)
      real_oldbrk = (byte_t *)CEIL_ALIGN((uintptr_t)_end,PAGESIZE);
 else real_oldbrk = (byte_t *)CEIL_ALIGN((uintptr_t)real_oldbrk,PAGESIZE);
 real_newbrk = (byte_t *)CEIL_ALIGN((uintptr_t)addr,PAGESIZE);
 if (real_newbrk < real_oldbrk) {
  /* Release memory */
  libc_munmap(real_newbrk,real_oldbrk-real_newbrk);
 } else if (real_newbrk > real_oldbrk) {
  void *map_result;
  /* Allocate more memory */
  map_result = libc_Xmmap(real_oldbrk,real_newbrk-real_oldbrk,
                          PROT_READ|PROT_WRITE,
                          MAP_FIXED|MAP_ANONYMOUS,-1,0);
  assertf(map_result == real_oldbrk,
          "%p != %p",map_result,real_oldbrk);
 }
 brk_curr = (byte_t *)addr;
}

EXPORT(Xbrk,libc_Xbrk);
CRT_EXCEPT void LIBCCALL libc_Xbrk(void *addr) {
 atomic_rwlock_write(&brk_lock);
 LIBC_TRY {
  do_brk(addr);
 } LIBC_FINALLY {
  atomic_rwlock_endwrite(&brk_lock);
 }
}

EXPORT(Xsbrk,libc_Xsbrk);
CRT_EXCEPT void *LIBCCALL libc_Xsbrk(intptr_t delta) {
 byte_t *COMPILER_IGNORE_UNINITIALIZED(result);
 atomic_rwlock_write(&brk_lock);
 LIBC_TRY {
  if ((result = brk_curr) == NULL)
       result = (byte_t *)CEIL_ALIGN((uintptr_t)_end,PAGESIZE);
  do_Xbrk(result+delta);
 } LIBC_FINALLY {
  atomic_rwlock_endwrite(&brk_lock);
 }
 return result;
}

/* GLibc aliases */
EXPORT(__sbrk,libc_sbrk);

DECL_END

#endif /* !GUARD_LIBS_LIBC_VM_C */
