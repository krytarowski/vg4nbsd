
/*--------------------------------------------------------------------*/
/*--- Signal-related libc stuff.                    m_libcsignal.c ---*/
/*--------------------------------------------------------------------*/
 
/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2000-2005 Julian Seward 
      jseward@acm.org

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307, USA.

   The GNU General Public License is contained in the file COPYING.
*/

#include "pub_core_basics.h"
#include "pub_core_libcbase.h"
#include "pub_core_libcassert.h"
#include "pub_core_libcsignal.h"
#include "pub_core_syscall.h"
#include "vki_unistd.h"

/* sigemptyset, sigfullset, sigaddset and sigdelset return 0 on
   success and -1 on error.  */

Int VG_(sigfillset)( vki_sigset_t* set )
{
   Int i;
   if (set == NULL)
      return -1;
   for (i = 0; i < _VKI_NSIG_WORDS; i++)
      set->sig[i] = ~(UWord)0x0;
   return 0;
}

Int VG_(sigemptyset)( vki_sigset_t* set )
{
   Int i;
   if (set == NULL)
      return -1;
   for (i = 0; i < _VKI_NSIG_WORDS; i++)
      set->sig[i] = 0x0;
   return 0;
}

Bool VG_(isemptysigset)( const vki_sigset_t* set )
{
   Int i;
   vg_assert(set != NULL);
   for (i = 0; i < _VKI_NSIG_WORDS; i++)
      if (set->sig[i] != 0x0) return False;
   return True;
}

Bool VG_(isfullsigset)( const vki_sigset_t* set )
{
   Int i;
   vg_assert(set != NULL);
   for (i = 0; i < _VKI_NSIG_WORDS; i++)
      if (set->sig[i] != ~(UWord)0x0) return False;
   return True;
}

Bool VG_(iseqsigset)( const vki_sigset_t* set1, const vki_sigset_t* set2 )
{
   Int i;
   vg_assert(set1 != NULL && set2 != NULL);
   for (i = 0; i < _VKI_NSIG_WORDS; i++)
      if (set1->sig[i] != set2->sig[i]) return False;
   return True;
}

/* The functions above should be ok. however a seperate implementation
for some of the below functions is provided as netbsd seems to do them
differnetly. see /usr/include/sys/sigtypes.h */

#ifdef VGO_netbsdelf2 
#define __sigmask(n)		(1 << (((unsigned int)(n) - 1) & 31))
#define	__sigword(n)		(((unsigned int)(n) - 1) >> 5)

Int VG_(sigaddset)( vki_sigset_t* set, Int signum )
{

   if (set == NULL)
      return -1;
   if (signum < 1 || signum > _VKI_NSIG)
      return -1;
   // signum--; /* required? */
   set->sig[__sigword(signum)] |= __sigmask(signum) ;
   //   set->sig[signum / _VKI_NSIG_BPW] |= (1UL << (signum % _VKI_NSIG_BPW));
   return 0;
}

Int VG_(sigdelset)( vki_sigset_t* set, Int signum )
{
   if (set == NULL)
      return -1;
   if (signum < 1 || signum > _VKI_NSIG)
      return -1;
   // signum--;
   set->sig[__sigword(signum)] &= ~__sigmask(signum) ;      
   //set->sig[signum / _VKI_NSIG_BPW] &= ~(1UL << (signum % _VKI_NSIG_BPW));
   return 0;
}

Int VG_(sigismember) ( const vki_sigset_t* set, Int signum )
{
   if (set == NULL)
      return 0;
   if (signum < 1 || signum > _VKI_NSIG)
      return 0;
   //signum--;
   VG_(printf)("checking if sig is member signum = %d\n",signum);
   VG_(printf)("returning %d\n",((set->sig[__sigword(signum)] & __sigmask(signum)) != 0));
   return    ((set->sig[__sigword(signum)] & __sigmask(signum)) != 0);

}

#else  

Int VG_(sigaddset)( vki_sigset_t* set, Int signum )
{
   if (set == NULL)
      return -1;
   if (signum < 1 || signum > _VKI_NSIG)
      return -1;
   signum--;
   set->sig[signum / _VKI_NSIG_BPW] |= (1UL << (signum % _VKI_NSIG_BPW));
   return 0;
}

Int VG_(sigdelset)( vki_sigset_t* set, Int signum )
{
   if (set == NULL)
      return -1;
   if (signum < 1 || signum > _VKI_NSIG)
      return -1;
   signum--;
    set->sig[signum / _VKI_NSIG_BPW] &= ~(1UL << (signum % _VKI_NSIG_BPW));
   return 0;
}

Int VG_(sigismember) ( const vki_sigset_t* set, Int signum )
{
   if (set == NULL)
      return 0;
   if (signum < 1 || signum > _VKI_NSIG)
      return 0;
   signum--;
   if (1 & ((set->sig[signum / _VKI_NSIG_BPW]) >> (signum % _VKI_NSIG_BPW)))
      return 1;
   else
      return 0;
}

#endif /* Sigaddset etc functions */ 
/* Add all signals in src to dst. */
void VG_(sigaddset_from_set)( vki_sigset_t* dst, vki_sigset_t* src )
{
   Int i;
   vg_assert(dst != NULL && src != NULL);
   for (i = 0; i < _VKI_NSIG_WORDS; i++)
      dst->sig[i] |= src->sig[i];
}

/* Remove all signals in src from dst. */
void VG_(sigdelset_from_set)( vki_sigset_t* dst, vki_sigset_t* src )
{
   Int i;
   vg_assert(dst != NULL && src != NULL);
   for (i = 0; i < _VKI_NSIG_WORDS; i++)
      dst->sig[i] &= ~(src->sig[i]);
}


/* The functions sigaction, sigprocmask, sigpending and sigsuspend
   return 0 on success and -1 on error.  
*/
/* #if defined (VGP_x86_netbsdelf2) */
/* asm(  */
/* 	"do_sigprocmask_inner:\n" */
/* 	"movl    8(%esp),%ecx\n"            /\*  fetch new sigset pointer *\/ */
/* 	"testl   %ecx,%ecx\n"               /\*  check new sigset pointer *\/ */
/* 	"jnz     1f\n"                      /\*  if not null, indirect *\/ */
/* 	"movl    $1,4(%esp)\n "             /\*  SIG_BLOCK *\/ */
/* 	"jmp     2f\n" */
/* 	"1:movl    (%ecx),%ecx\n"             /\*  fetch indirect  ... *\/ */
/* 	"movl    %ecx,8(%esp)\n"             /\* to new mask arg *\/ */
/* 	"2:movl $48,%eax\n" /\*  move syscall no to eax  *\/ */
/* 	"int $0x80\n" */
/* 	"jae 3f\n" */
/* 	"movl $-1,%eax\n" */
/* 	"3:\n" */
/* 	"ret\n" */
/* 	); */
/* #endif  */
Int VG_(sigprocmask)( Int how, const vki_sigset_t* set, vki_sigset_t* oldset)
{
  VG_(debugLog)(1,"m_libcsignal","In sigprocmask how = %d\n",how);
#  if !defined(VGP_x86_netbsdelf2)
   SysRes res = VG_(do_syscall4)(__NR_rt_sigprocmask, 
                                 how, (UWord)set, (UWord)oldset, 
                                 _VKI_NSIG_WORDS * sizeof(UWord));
   return res.isError ? -1 : 0;
#else
   SysRes res = VG_(do_syscall3)(__NR___sigprocmask14, how,set,oldset);
/*    return do_sigprocmask_inner(how,set,oldset); */
   return res.isError ? -1 : 0;
#endif
}

/* Not sure if this is the right place for this (aka BIG HACK SIR) */
#if defined(VGP_x86_netbsdelf2)
#define __STRING(x) #x
#define STR(x) __STRING(x)
#define __NR_EXIT         STR(__NR_exit)
#define __NR_COMPAT___16_SIGRETURN14     STR(__NR_compat_16___sigreturn14)
#define __NR_SETCONTEXT STR(__NR_setcontext)
/* from /usr/netbsd-current/src/lib/libc/arch/i386/sys/_sigtramp2.S */
extern void * __vg_sigtramp_siginfo_2;
     asm(".text\n"
	 ".align 4\n"
	 ".globl __vg_sigtramp_siginfo_2\n"
	 ".type __vg_sigtramp_siginfo_2,@function\n"
	 "__vg_sigtramp_siginfo_2:\n"
	 "leal	12+128(%esp),%eax\n"	/* get pointer to ucontext */
	 "movl  %eax,4(%esp)\n"	/* put it in the argument slot */
	 /* fake return address already there */
	 "movl $308,%eax\n"    /* do setcontext */
	 "int $0x80\n"
	 "movl  %eax,4(%esp)\n"	/* error code */
	 "movl $1,%eax\n"
	 "int $0x80\n"
	 );

     /* from src/lib/libc/compat/arch/i386/sys/compat___sigtramp1. */
extern void * __vg_sigtramp_sigcontext_1;
asm(".text\n"      /* Start of _ENTRY, see include/i386/asm.h */
    ".align 4\n"   /* This is ALIGN_TEXT, defined 4 if __ELF, else 2 */
    ".globl __vg_sigtramp_sigcontext_1\n"
    ".type __vg_sigtramp_sigcontext_1,@function\n"
    "__vg_sigtramp_sigcontext_1:\n"
    "leal	12(%esp),%eax\n"	/* get pointer to sigcontext */
    "movl	%eax,4(%esp)\n" 	/* put it in the argument slot */
    /* fake return address already there */
    /* "movl $(" STR(__NR_compat_16___sigreturn14) "), %eax\n"  */
/*     "movl $"__NR_COMPAT___16_SIGRETURN14", %eax\n" */

    "movl $295,%eax\n"
    "int $0x80\n"
    "movl	%eax,4(%esp)\n"   	/* error code */
/*     "movl $"__NR_EXIT", %eax\n" */       		/* exit */
    "movl $1,%eax\n"
    "int $0x80\n"
    );
#endif

SysRes VG_(do_sigaction_sigtramp)(Int signum ,const struct  vki_sigaction * act, struct vki_sigaction * oldact ) 
{
  SysRes res;

  if (act == NULL) {
	   res = VG_(do_syscall5)(__NR___sigaction_sigtramp,
				  signum, (UWord)act, (UWord)oldact,
				  (UWord)NULL, 0);
   } else {

     VG_(debugLog)(2,"m_libcsignal","doing weird syscall\n");
	   res = VG_(do_syscall5)(__NR___sigaction_sigtramp,
				  signum, (UWord)act, (UWord)oldact,
				  (void * )__vg_sigtramp_siginfo_2, 2); /* legacy sigcontext trampoline */
/* 	   res = VG_(do_syscall5)(__NR___sigaction_sigtramp, */
/* 				  signum, (UWord)act, (UWord)oldact, */
/* 				  NULL, 0); */ /* legacy kernal trampoline */
   }
  return res;
}

/* we should consider implementing both the sigcontext and sigaction tramps */
Int VG_(sigaction) (Int signum, const struct vki_sigaction* act,  
                     struct vki_sigaction* oldact)
{
#  if !defined(VGP_x86_netbsdelf2)
   SysRes res = VG_(do_syscall4)(__NR_rt_sigaction,
                                 signum, (UWord)act, (UWord)oldact, 
                                 _VKI_NSIG_WORDS * sizeof(UWord));
   return res.isError ? -1 : 0;
#else
   /* From /usr/src/lib/libc/arch/i386/sys/__sigaction14_sigtramp */
   SysRes res;
   // res = VG_(do_sys_sigaction)(signum , act, oldact);
     res = VG_(do_sigaction_sigtramp) (signum, act, oldact);
   if(res.isError) 
     VG_(debugLog)(1,"m_libcsignal","Sigaction failed!!!\n");

   return res.isError ? -1 : 0;
#endif
}


Int VG_(sigaltstack)( const vki_stack_t* ss, vki_stack_t* oss )
{
#  if !defined(VGP_x86_netbsdelf2)
   SysRes res = VG_(do_syscall2)(__NR_sigaltstack, (UWord)ss, (UWord)oss);
   return res.isError ? -1 : 0;
#else
   I_die_here;
#endif
}

Int VG_(sigtimedwait)( const vki_sigset_t *set, vki_siginfo_t *info, 
                       const struct vki_timespec *timeout )
{
#  if !defined(VGP_x86_netbsdelf2)
   SysRes res = VG_(do_syscall4)(__NR_rt_sigtimedwait, (UWord)set, (UWord)info, 
                                 (UWord)timeout, sizeof(*set));
   return res.isError ? -1 : res.val;
#elif defined (VGP_x86_netbsdelf2)
   SysRes res = VG_(do_syscall3)(__NR___sigtimedwait, (UWord)set, (UWord)info, 
                                 (UWord)timeout/* , sizeof(*set) */);
   if (res.isError){
     VG_(printf)("Sigtimedwait fialed!!");
   }
     
   return res.isError ? -1 : res.val;
#endif
}
 
Int VG_(signal)(Int signum, void (*sighandler)(Int))
{
#  if !defined(VGP_x86_netbsdelf2)
   SysRes res;
   Int    n;
   struct vki_sigaction sa;
   sa.ksa_handler = sighandler;
   sa.sa_flags = VKI_SA_ONSTACK | VKI_SA_RESTART;
   sa.sa_restorer = NULL;
   n = VG_(sigemptyset)( &sa.sa_mask );
   vg_assert(n == 0);
   res = VG_(do_syscall4)(__NR_rt_sigaction, signum, (UWord)&sa, (UWord)NULL,
                           _VKI_NSIG_WORDS * sizeof(UWord));
   return res.isError ? -1 : 0;
#else
   I_die_here;
#endif
}


Int VG_(kill)( Int pid, Int signo )
{
   SysRes res = VG_(do_syscall2)(__NR_kill, pid, signo);
   return res.isError ? -1 : 0;
}


Int VG_(tkill)( ThreadId tid, Int signo )
{
#  if !defined(VGP_x86_netbsdelf2)
   SysRes res = VG_(mk_SysRes_Error)(VKI_ENOSYS);

#if 0
   /* This isn't right because the client may create a process
      structure with multiple thread groups */
   res = VG_(do_syscall3)(__NR_tgkill, VG_(getpid)(), tid, signo);
#endif

   res = VG_(do_syscall2)(__NR_tkill, tid, signo);

   if (res.isError && res.val == VKI_ENOSYS)
      res = VG_(do_syscall2)(__NR_kill, tid, signo);

   return res.isError ? -1 : 0;
#else
   I_die_here;
#endif
}

Int VG_(sigpending) ( vki_sigset_t* set )
{
#  if !defined(VGP_x86_netbsdelf2)

// Nb: AMD64/Linux doesn't have __NR_sigpending;  it only provides
// __NR_rt_sigpending.  This function will have to be abstracted in some
// way to account for this.  In the meantime, the easy option is to forget
// about it for AMD64 until it's needed.
#if defined(VGA_amd64)
   I_die_here;
#else
   SysRes res = VG_(do_syscall1)(__NR_sigpending, (UWord)set);
   return res.isError ? -1 : 0;
#endif

#else
   I_die_here;
#endif
}

/*--------------------------------------------------------------------*/
/*--- end                                                          ---*/
/*--------------------------------------------------------------------*/
