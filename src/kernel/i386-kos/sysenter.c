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
#ifndef GUARD_KERNEL_I386_KOS_SYSENTER_C
#define GUARD_KERNEL_I386_KOS_SYSENTER_C 1
#define _KOS_SOURCE 1

#include <hybrid/compiler.h>
#include <i386-kos/syscall.h>
#include <i386-kos/gdt.h>
#include <i386-kos/cpuid.h>
#include <i386-kos/tss.h>
#include <kernel/debug.h>
#include <hybrid/section.h>
#include <asm/cpu-flags.h>
#include <syscall.h>
#include <kos/ushare.h>
#include <string.h>
#include <kos/intrin.h>

DECL_BEGIN

STATIC_ASSERT(TASK_USERCTX_REGS_FINT80    == X86_INTERRUPT_GUARD_FREG_INT80);
STATIC_ASSERT(TASK_USERCTX_REGS_FSYSENTER == X86_INTERRUPT_GUARD_FREG_SYSENTER);
STATIC_ASSERT(TASK_USERCTX_REGS_FPF       == X86_INTERRUPT_GUARD_FREG_PF);


INTERN ATTR_SECTION(".rodata.hot") u16 const x86_interrupt_mode_select[16] = {
    [(X86_INTERRUPT_GUARD_FINTERRUPT) >> 8]                                 = TASK_USERCTX_TYPE_INTR_INTERRUPT,
    [(X86_INTERRUPT_GUARD_FSYSCALL|X86_INTERRUPT_GUARD_FREG_INT80) >> 8]    = TASK_USERCTX_TYPE_INTR_SYSCALL|TASK_USERCTX_REGS_FINT80,
    [(X86_INTERRUPT_GUARD_FSYSCALL|X86_INTERRUPT_GUARD_FREG_PF) >> 8]       = TASK_USERCTX_TYPE_INTR_SYSCALL|TASK_USERCTX_REGS_FPF,
    [(X86_INTERRUPT_GUARD_FSYSCALL|X86_INTERRUPT_GUARD_FREG_SYSENTER) >> 8] = TASK_USERCTX_TYPE_INTR_SYSCALL|TASK_USERCTX_REGS_FSYSENTER,
};


/* The `IA32_SYSENTER_CS' register requires this layout. */
STATIC_ASSERT(X86_SEG_HOST_DATA == X86_SEG_HOST_CODE+1);
STATIC_ASSERT(X86_SEG_USER_CODE == X86_SEG_HOST_CODE+2);
STATIC_ASSERT(X86_SEG_USER_DATA == X86_SEG_HOST_CODE+3);

INTDEF byte_t x86_ushare_sysenter[];
INTDEF byte_t x86_fast_sysenter[];
INTDEF byte_t x86_fast_sysenter_size[];
INTERN byte_t *x86_sysenter_ushare_base = x86_ushare_sysenter;

#ifdef __x86_64__
INTDEF byte_t x86_ushare_syscall[];
INTDEF byte_t x86_fast_syscall[];
INTDEF byte_t x86_fast_syscall_size[];
INTERN byte_t *x86_syscall_ushare_base = x86_ushare_syscall;
#endif


#ifndef __x86_64__
INTDEF byte_t x86_sysexit_fixup_1[];
#endif


INTDEF void ASMCALL sysenter_kernel_entry(void);
#ifdef __x86_64__
INTDEF void ASMCALL syscall_kernel_entry(void);
#endif


INTERN ATTR_FREETEXT void KCALL x86_initialize_sysenter(void) {
 struct cpu_cpuid *feat = (struct cpu_cpuid *)&CPU_FEATURES;
 if (!(feat->ci_1d & CPUID_1D_SEP)) {
  /* Not available. */
#ifndef __x86_64__
  if (THIS_CPU == &_boot_cpu) {
   x86_sysexit_fixup_1[0] = 0xcf; /* iret */
  }
#endif
 } else {
  debug_printf(FREESTR("[X86] Enable SYSENTER\n"));
  /* Write sysenter-specific MSRs */
  __wrmsr(IA32_SYSENTER_CS,X86_KERNEL_CS);
#ifdef __x86_64__
  __wrmsr(IA32_SYSENTER_ESP,(uintptr_t)&PERCPU(x86_cputss).t_rsp0);
#else
  __wrmsr(IA32_SYSENTER_ESP,(uintptr_t)&PERCPU(x86_cputss).t_esp0);
#endif
  __wrmsr(IA32_SYSENTER_EIP,(uintptr_t)&sysenter_kernel_entry);

  if (THIS_CPU == &_boot_cpu) {
   /* The boot CPU is responsible for re-writing the ushare
    * segment containing the system call entry points. */
   memcpy(x86_ushare_sysenter,x86_fast_sysenter,
         (size_t)x86_fast_sysenter_size);
   x86_sysenter_ushare_base = x86_fast_sysenter;
  }
 }
#ifdef __x86_64__
 if (!(feat->ci_80000001d & CPUID_80000001D_SYSCALL)) {
  /* Not available. */
 } else {
  debug_printf(FREESTR("[X86] Enable SYSCALL\n"));
  /* Write sysenter-specific MSRs */
  __wrmsr(IA32_STAR, (u64)X86_KERNEL_CS << 32);
  __wrmsr(IA32_LSTAR,(uintptr_t)&syscall_kernel_entry);
  __wrmsr(IA32_FMASK,EFLAGS_IF|EFLAGS_DF);


  if (THIS_CPU == &_boot_cpu) {
   /* The boot CPU is responsible for re-writing the ushare
    * segment containing the system call entry points. */
   memcpy(x86_ushare_syscall,x86_fast_syscall,
         (size_t)x86_fast_syscall_size);
   x86_syscall_ushare_base = x86_fast_syscall;
  }
 }
#endif
}

DECL_END

#endif /* !GUARD_KERNEL_I386_KOS_SYSENTER_C */
