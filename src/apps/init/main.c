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
#ifndef GUARD_APPS_INIT_MAIN_C
#define GUARD_APPS_INIT_MAIN_C 1
#define _ALL_SOURCE 1
//#define _EXCEPT_API 1
#define __EXPOSE_CPU_CONTEXT 1
#define __EXPOSE_STAT_STRUCTURES 1

#include <kos/sched/rwlock.h>
#include <parts/kos3/sys/wstat.h>
#include <parts/kos3/sys/ustat.h>
#include <parts/kos3/wstdlib.h>
#include <assert.h>
#include <parts/kos3/ustdlib.h>
#include <parts/kos3/except/uunistd.h>
#include <parts/kos3/except/wunistd.h>
#include <parts/kos3/except/unicode.h>
#include <parts/kos3/except/wio.h>
#include <parts/kos3/except/io.h>
#include <format-printer.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <kos/futex.h>
#include <hybrid/bit.h>
#include <wchar.h>
#include <uchar.h>
#include <stdlib.h>
#include <malloc.h>
#include <io.h>
#include <parts/kos2/uprocess.h>
#include <parts/kos2/unistd.h>
#include <parts/kos3/process.h>
#include <parts/dos/process.h>
#include <parts/kos3/malloc.h>
#include <parts/kos2/string.h>
#include <parts/kos3/string.h>
#include <parts/kos3/wstring.h>

#include <hybrid/compiler.h>
#include <sys/mman.h>
#include <kos/ushare.h>
#include <kos/intrin-arith.h>
#include <kos/fcntl.h>
#include <kos/kernctl.h>
#include <except.h>

#include <process.h>
#include <string.h>
#include <wchar.h>
#include <uchar.h>
#include <syslog.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/unistd.h>
#include <sys/ucontext.h>
#include <fcntl.h>
#include <signal.h>
#include <dirent.h>
#include <sched.h>
#include <sys/wait.h>
#include <kos/rpc.h>
#include <kos/thread.h>
#include <kos/environ.h>
#include <asm/cpu-flags.h>

DECL_BEGIN


#if 1

struct ushare_procinfo *map_procinfo(void) {
 struct mmap_info info;
 memset(&info,0,sizeof(info));
 info.mi_addr            = NULL;
 info.mi_prot            = PROT_READ|PROT_WRITE;
 info.mi_flags           = MAP_PRIVATE;
 info.mi_xflag           = XMAP_USHARE;
 info.mi_size            = USHARE_PROCINFO_FSIZE;
 info.mi_align           = PAGESIZE;
 info.mi_gap             = 0;
 info.mi_tag             = NULL;
 info.mi_ushare.mu_name  = USHARE_PROCINFO_FNAME;
 info.mi_ushare.mu_start = 0;
 return (struct ushare_procinfo *)xmmap(&info);
}

PRIVATE void
my_handler(int signo, siginfo_t *info, ucontext_t *context) {
 syslog(LOG_DEBUG,"my_handler(%d)\n",signo);
 syslog(LOG_DEBUG,"info    = %p\n",info);
 syslog(LOG_DEBUG,"context = %p\n",context);
}


PRIVATE ATTR_THREAD int thread_local_x = 42;
PRIVATE int LIBCCALL my_thread(void *arg) {
 syslog(LOG_DEBUG,"thread_local_x = %p:%d\n",&thread_local_x,thread_local_x);
 syslog(LOG_DEBUG,"my_thread() #1\n");
 pause();
 syslog(LOG_DEBUG,"my_thread() #2\n");
 pause();
 syslog(LOG_DEBUG,"my_thread() #3\n");
 pause();
 syslog(LOG_DEBUG,"my_thread() #4\n");
 syslog(LOG_DEBUG,"thread_local_x = %p:%d\n",&thread_local_x,thread_local_x);
 return 0;
}



PRIVATE unsigned int LIBCCALL my_rpc(void *arg) {
 syslog(LOG_DEBUG,"thread_local_x = %p:%d\n",&thread_local_x,thread_local_x);
 ++thread_local_x;
 syslog(LOG_DEBUG,"my_rpc(%p)\n",arg);
 /*asm("int3");*/
 return RPC_RETURN_RESTART;
}


void test_rpc(void) {
 pid_t child_pid;
 Xclone(&my_thread,CLONE_CHILDSTACK_AUTO,
        CLONE_NEW_THREAD |
        CLONE_CHILD_CLEARTID |
        CLONE_PARENT_SETTID,
        NULL,
       &child_pid);
 syslog(LOG_DEBUG,"#1\n");
 while (sched_yield());
 syslog(LOG_DEBUG,"#3\n");
//  syslog(LOG_DEBUG,"SCHEDULE FIRST RPC -- %d\n",sched_yield());
//  syslog(LOG_DEBUG,"SCHEDULE FIRST RPC -- %d\n",sched_yield());
 Xqueue_rpc(child_pid,&my_rpc,(void *)1,RPC_FSYNCHRONOUS|RPC_FWAITFOR);
 Xqueue_rpc(child_pid,&my_rpc,(void *)2,RPC_FSYNCHRONOUS|RPC_FWAITFOR);
 Xqueue_rpc(child_pid,&my_rpc,(void *)3,RPC_FSYNCHRONOUS|RPC_FWAITFOR);
 Xwaitpid(child_pid,NULL,WEXITED);
}


int main(int argc, char *argv[]) {
 struct sigaction act;
// kernctl(KERNEL_CONTROL_TRACE_SYSCALLS_ON);
 kernctl(KERNEL_CONTROL_DBG_DUMP_LEAKS);
 memset(&act,0,sizeof(struct sigaction));
 act.sa_sigaction = &my_handler;
 act.sa_flags     = SA_SIGINFO;
 Xsigaction(SIGCHLD,&act,NULL);

 syslog(LOG_DEBUG,"__current()->ts_self = %p\n",__current()->ts_self);
 syslog(LOG_DEBUG,"thread_local_x = %p:%d\n",&thread_local_x,thread_local_x);
 thread_local_x = 7;
 syslog(LOG_DEBUG,"thread_local_x = %p:%d\n",&thread_local_x,thread_local_x);

 test_rpc();

 assert(strcmp("foo","foo") == 0);
 assert(strcmp("foo","bar") != 0);
 assert(strcmp("foo","foobar") != 0);
 assert(strcmp("foobar","foo") != 0);
 assert(strncmp("foo","foo",2) == 0);
 assert(strncmp("foo","foo",3) == 0);
 assert(strncmp("foo","foo",4) == 0);
 assert(strncmp("foo","bar",2) != 0);
 assert(strncmp("foo","bar",3) != 0);
 assert(strncmp("foo","bar",4) != 0);
 assert(strncmp("foo","foobar",2) == 0);
 assert(strncmp("foo","foobar",3) == 0);
 assert(strncmp("foo","foobar",4) != 0);
 assert(strncmp("foo","foobar",5) != 0);
 assert(strncmp("foo","foobar",6) != 0);
 assert(strncmp("foo","foobar",7) != 0);
 assert(strverscmp("foo","foo") == 0);
 assert(strverscmp("foo1","foo1") == 0);
 assert(strverscmp("foo9","foo9") == 0);
 assert(strverscmp("foo000","foo00") < 0);
 assert(strverscmp("foo00","foo01") < 0);
 assert(strverscmp("foo01","foo010") < 0);
 assert(strverscmp("foo010","foo09") < 0);
 assert(strverscmp("foo09","foo0") < 0);
 assert(strverscmp("foo0","foo1") < 0);
 assert(strverscmp("foo1","foo9") < 0);
 assert(strverscmp("foo9","foo10") < 0);
#if 1
 pid_t cpid;
 if ((cpid = Xfork()) == 0) {
  kernctl(KERNEL_CONTROL_DBG_DUMP_LEAKS);
  syslog(LOG_DEBUG,"Child process (%u)\n",getpid());
  sched_yield();
  sched_yield();
  sched_yield();
  sched_yield();
  syslog(LOG_DEBUG,"Exit Child process\n");
  exit(0x12);
 }
 syslog(LOG_DEBUG,"cpid = %d\n",cpid);

 {
  union wait info;
  pid_t pid;
  //pause();
  //syslog(LOG_DEBUG,"pause() -> %s\n",strerror(errno));
  while ((pid = wait(&info)) < 0 && errno == EINTR)
      syslog(LOG_DEBUG,"wait() -> E_INTERRUPT\n");
  syslog(LOG_DEBUG,"wait() -> %d (%s)\n",pid,strerror(errno));
  syslog(LOG_DEBUG,"info.w_status = %x\n",info.w_status);
 }
 //exit(0);

#if 0
 syslog(LOG_DEBUG,"REALPATH_FPATH                        = %q\n",xrealpath("bin",NULL,0,REALPATH_FPATH));
 syslog(LOG_DEBUG,"REALPATH_FHEAD                        = %q\n",xrealpath("bin",NULL,0,REALPATH_FHEAD));
 syslog(LOG_DEBUG,"REALPATH_FTAIL                        = %q\n",xrealpath("bin",NULL,0,REALPATH_FTAIL));
 syslog(LOG_DEBUG,"REALPATH_FDRIVE                       = %q\n",xrealpath("bin",NULL,0,REALPATH_FDRIVE & ~REALPATH_FDOSPATH));
 syslog(LOG_DEBUG,"REALPATH_FDRIVEPATH                   = %q\n",xrealpath("bin",NULL,0,REALPATH_FDRIVEPATH & ~REALPATH_FDOSPATH));
 syslog(LOG_DEBUG,"REALPATH_FDOSPATH|REALPATH_FPATH      = %q\n",xrealpath("bin",NULL,0,REALPATH_FDOSPATH|REALPATH_FPATH));
 syslog(LOG_DEBUG,"REALPATH_FDOSPATH|REALPATH_FHEAD      = %q\n",xrealpath("bin",NULL,0,REALPATH_FDOSPATH|REALPATH_FHEAD));
 syslog(LOG_DEBUG,"REALPATH_FDOSPATH|REALPATH_FTAIL      = %q\n",xrealpath("bin",NULL,0,REALPATH_FDOSPATH|REALPATH_FTAIL));
 syslog(LOG_DEBUG,"REALPATH_FDOSPATH|REALPATH_FDRIVE     = %q\n",xrealpath("bin",NULL,0,REALPATH_FDOSPATH|REALPATH_FDRIVE));
 syslog(LOG_DEBUG,"REALPATH_FDOSPATH|REALPATH_FDRIVEPATH = %q\n",xrealpath("bin",NULL,0,REALPATH_FDOSPATH|REALPATH_FDRIVEPATH));
#endif

#if 0
 kernctl(KERNEL_CONTROL_DBG_DUMP_LEAKS);
 {
  struct ushare_procinfo *base;
  base = map_procinfo();
  syslog(LOG_DEBUG,"pi_tid              = %u\n",base->pi_tid);
  syslog(LOG_DEBUG,"pi_pid              = %u\n",base->pi_pid);
  syslog(LOG_DEBUG,"pi_ppid             = %u\n",base->pi_ppid);
  syslog(LOG_DEBUG,"pi_gpid             = %u\n",base->pi_gpid);
  syslog(LOG_DEBUG,"pi_sid              = %u\n",base->pi_sid);
  syslog(LOG_DEBUG,"pi_hz               = %u\n",base->pi_hz);
  syslog(LOG_DEBUG,"pi_time             = %u\n",base->pi_time);
  syslog(LOG_DEBUG,"pi_thread.t_start   = %u\n",base->pi_thread.t_start);
  syslog(LOG_DEBUG,"pi_thread.t_hswitch = %u\n",base->pi_thread.t_hswitch);
  syslog(LOG_DEBUG,"pi_thread.t_uswitch = %u\n",base->pi_thread.t_uswitch);
  syslog(LOG_DEBUG,"pi_thread.t_hyield  = %u\n",base->pi_thread.t_hyield);
  syslog(LOG_DEBUG,"pi_thread.t_uyield  = %u\n",base->pi_thread.t_uyield);
  syslog(LOG_DEBUG,"pi_thread.t_sleep   = %u\n",base->pi_thread.t_sleep);
  syslog(LOG_DEBUG,"pi_thread.t_xrpc    = %u\n",base->pi_thread.t_xrpc);
  syslog(LOG_DEBUG,"pi_thread.t_qrpc    = %u\n",base->pi_thread.t_qrpc);
 }
#endif
 kernctl(KERNEL_CONTROL_DBG_DUMP_LEAKS);

#if 0
 asm("movl $0x11223344, %%esi" : : : "esi");
 asm("movl $0x55667788, %%edi" : : : "edi");
 for (;;) Xfork();
#endif

// kernctl(KERNEL_CONTROL_TRACE_SYSCALLS_ON);
 kernctl(KERNEL_CONTROL_DBG_DUMP_LEAKS);

 /* Create and mount the /dev filesystem. */
 mkdir("/dev",0755);
 Xmount("devfs","/dev","devfs",0,NULL);
 {
  struct stat buf;
  /* Create a symbolic link for the default keyboard and mouse device. */
  /* */if (stat("/dev/ps2_kbda",&buf) == 0) Xsymlink("ps2_kbda","/dev/keyboard");
  else if (stat("/dev/ps2_kbdb",&buf) == 0) Xsymlink("ps2_kbdb","/dev/keyboard");
  /* */if (stat("/dev/ps2_mouseb",&buf) == 0) Xsymlink("ps2_mouseb","/dev/mouse");
  else if (stat("/dev/ps2_mousea",&buf) == 0) Xsymlink("ps2_mousea","/dev/mouse");
 }
#endif

 kernctl(KERNEL_CONTROL_INSMOD,"/mod/pe.mod",NULL);
 kernctl(KERNEL_CONTROL_INSMOD,"/mod/memdev.mod",NULL);
 kernctl(KERNEL_CONTROL_INSMOD,"/mod/unix-domain.mod",NULL);

 /* Just for testing: Load a kernel driver. */
 if (kernctl(KERNEL_CONTROL_INSMOD,"/mod/procfs.mod",NULL) >= 0) {
  mkdir("/proc",0755);
  mount("procfs","/proc","procfs",0,NULL);
 }

 if (kernctl(KERNEL_CONTROL_INSMOD,"/mod/ext2fs.mod",NULL) >= 0) {
  mount("/dev/hdc1","/mount",NULL,0,NULL);
 }


#if 1
 if (kernctl(KERNEL_CONTROL_INSMOD,"/mod/vga.mod",NULL) >= 0) {
  kernctl(KERNEL_CONTROL_INSMOD,"/mod/bochs-vbe.mod",NULL);
  Xexecl("/bin/wms","wms","/bin/wm","wm",(char *)NULL);
 }
#endif

#if 0
 if (fork() == 0)
     Xexecl("/bin/hybrid-demo.exe","hybrid-demo.exe",(char *)NULL);
#endif

 for (;;) {
  TRY Xexecl("/bin/terminal-vga",
             "terminal-vga",
             "/bin/busybox",
             "sh",
             "-i",
             (char *)NULL);
  CATCH_HANDLED (E_INTERRUPT) { }
  syslog(LOG_DEBUG,"\n\nCATCH INTERRUPT\n\n\n");
 }
}

#else

static volatile int x = 42;

_nsyscall4(int,SYS_openat,     sys_openat,fd_t,dfd,char const *,filename,oflag_t,flags,mode_t,mode);
_nsyscall4(size_t,SYS_xreaddir,sys_xreaddir,fd_t,fd,struct dirent *,buf,size_t,bufsize,int,mode);
_nsyscall1(int,SYS_close,      sys_close,fd_t,fd);
_nsyscall3(int,SYS_xsyslog,    sys_xsyslog,int,type,char const *,p,size_t,len);
_nsyscall1(int,SYS_exit,       sys_exit,int,exitcode);
_nsyscall0(pid_t,SYS_fork,     sys_fork);
_nsyscall0(pid_t,SYS_gettid,   sys_gettid);
_nsyscall0(int,SYS_sched_yield,sys_sched_yield);
_nsyscall1(int,SYS_exit_group, sys_exit_group,int,exitcode);
_nsyscall5(pid_t,SYS_clone,    sys_clone,
                struct cpu_context *,context,
                syscall_ulong_t,flags,pid_t *,parent_tidptr,
                void *,tls_val,pid_t *,child_tidptr);


__asm__(".text\n"
        "thread_entry:\n"
        "    pushl %edx\n"
        "    call *%ecx\n"
        "    movl  %eax, (%esp)\n"
        "    call  sys_exit\n");
INTDEF ATTR_NORETURN void FCALL
thread_entry(void *(*func)(void *arg), void *arg);


void sys_print(char const *str) {
 size_t len = 0;
 while (str[len]) ++len;
 sys_xsyslog(0,str,len);
}

void print_ptr(uintptr_t ptr) {
 char buf[9],*iter = COMPILER_ENDOF(buf);
 *--iter = '\0';
 while (iter != buf) {
  *--iter = (ptr & 0xf) >= 10 ? 'A' + ((ptr & 0xf)-10) : '0' + (ptr & 0xf);
  ptr >>= 4;
 }
 COMPILER_WRITE_BARRIER();
 sys_print(buf);
}


pid_t start_thread(void *(*func)(void *arg), void *arg) {
 struct cpu_context context;
 context.c_gpregs.gp_pdi  = 0;
 context.c_gpregs.gp_psi  = 0;
 context.c_gpregs.gp_pbp  = 0;
 context.c_gpregs.gp_psp  = (uintptr_t)CLONE_CHILDSTACK_AUTO;
 context.c_gpregs.gp_pbx  = 0;
 context.c_gpregs.gp_pdx  = (uintptr_t)arg;
 context.c_gpregs.gp_pcx  = (uintptr_t)func;
 context.c_gpregs.gp_pax  = 0;
 context.c_segments.sg_gs = 0;
 context.c_segments.sg_fs = 0;
#ifndef CONFIG_X86_FIXED_SEGMENTATION
 context.c_segments.sg_es = 0;
 context.c_segments.sg_ds = 0;
 context.c_cs             = 0;
 context.c_ss             = 0;
#endif /* !CONFIG_X86_FIXED_SEGMENTATION */
 context.c_pip            = (uintptr_t)&thread_entry;
 context.c_pflags         = 0;

 return sys_clone(&context,
                  CLONE_VM|
                  CLONE_THREAD|
                  CLONE_PARENT|
                  CLONE_FS|
                  CLONE_FILES|
                  CLONE_SIGHAND,
                  NULL,NULL,NULL);
}


void print_dir(int fd) {
 for (;;) {
  char buffer[2048]; size_t bufsize;
  struct dirent *iter = (struct dirent *)buffer;
  // Read as many entries as our buffer can fit
  sys_print("sys_xreaddir()\n");
  bufsize = sys_xreaddir(fd,iter,sizeof(buffer),READDIR_MULTIPLE);
  if (!bufsize) break; // End of directory
  if (bufsize > sizeof(buffer)) {
   sys_print("A directory entry is too larger for buffer\n");
   break;
  }
  // Process entries that were read.
  for (;;) {
   sys_print("Entry: ");
   sys_print(iter->d_name);
   sys_print("\n");
   iter = READDIR_MULTIPLE_GETNEXT(iter);
   if (!READDIR_MULTIPLE_ISVALID(iter,buffer,bufsize))
       break;
  }
 }
}

void *thread_main(void *arg) {
 sys_print("thread_main(): ");
 print_ptr(sys_gettid());
 sys_print("\n");
 return arg;
}


void _start(void) {
 sys_print("Hello world\n");
 x += 17;
 pid_t child_pid;
#if 1
 if (kernctl(KERNEL_CONTROL_DBG_DUMP_LEAKS))
     sys_exit(1);

 int fd = sys_openat(AT_FDCWD,"/bin",O_DIRECTORY,0);
 print_dir(fd);
 sys_close(fd);
 if (kernctl(KERNEL_CONTROL_DBG_DUMP_LEAKS))
     sys_exit(1);
#if 1
 for (;;) {
  unsigned int i;
  for (i = 0; i < 3; ++i)
  {
   child_pid = start_thread(thread_main,(void *)42);
   sys_print("After start_thread(): ");
   print_ptr(child_pid);
   sys_print("\n");
  }
//  for (int i = 0; i < 10000; ++i)
//      sys_sched_yield();
/*
  for (;;)
*/
//     {
//      if (kernctl(KERNEL_CONTROL_DBG_DUMP_LEAKS))
//          sys_exit(1);
//     }
 }
#else
 child_pid = start_thread(thread_main,(void *)42);
 child_pid = start_thread(thread_main,(void *)42);
 child_pid = start_thread(thread_main,(void *)42);
 child_pid = start_thread(thread_main,(void *)42);
 child_pid = start_thread(thread_main,(void *)42);
 child_pid = start_thread(thread_main,(void *)42);
 child_pid = start_thread(thread_main,(void *)42);
 child_pid = start_thread(thread_main,(void *)42);
 child_pid = start_thread(thread_main,(void *)42);
 child_pid = start_thread(thread_main,(void *)42);
 child_pid = start_thread(thread_main,(void *)42);
 sys_print("After start_thread(): ");
 print_ptr(child_pid);
 sys_print("\n");
#endif
#endif
 if ((child_pid = sys_fork()) == 0) {
  sys_print("Child process\n");
 } else {
  sys_print("Parent process ");
  print_ptr(child_pid);
  sys_print("\n");
 }
 sys_exit_group(0);
}

INTERN uintptr_t __stack_chk_guard = 0x1246Ab1f;
INTERN ATTR_NORETURN void __stack_chk_fail(void) {
 sys_print("__stack_chk_fail()\n");
 sys_exit_group(1);
}
#endif


DECL_END

#endif /* !GUARD_APPS_INIT_MAIN_C */
