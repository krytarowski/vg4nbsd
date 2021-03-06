noinst_SCRIPTS = filter_cpuid filter_stderr gen_insn_test.pl

CLEANFILES = $(addsuffix .c,$(INSN_TESTS))
INSN_TESTS = insn_basic insn_fpu insn_cmov insn_mmx insn_mmxext insn_sse insn_sse2

EXTRA_DIST = $(noinst_SCRIPTS) \
	badseg.stderr.exp badseg.stdout.exp badseg.vgtest \
	bt_everything.stderr.exp bt_everything.stdout.exp bt_everything.vgtest \
	bt_literal.stderr.exp bt_literal.stdout.exp bt_literal.vgtest \
	cpuid.stderr.exp cpuid.stdout.exp cpuid.vgtest \
	cmpxchg8b.stderr.exp cmpxchg8b.stdout.exp cmpxchg8b.vgtest \
	faultstatus.vgtest faultstatus.stderr.exp \
	fpu_lazy_eflags.stderr.exp fpu_lazy_eflags.stdout.exp \
	fpu_lazy_eflags.vgtest \
	fxtract.stdout.exp fxtract.stderr.exp fxtract.vgtest \
	getseg.stdout.exp getseg.stderr.exp getseg.vgtest \
	incdec_alt.stdout.exp incdec_alt.stderr.exp incdec_alt.vgtest \
	int.stderr.exp int.stdout.exp int.vgtest \
	$(addsuffix .stderr.exp,$(INSN_TESTS)) \
	$(addsuffix .stdout.exp,$(INSN_TESTS)) \
	$(addsuffix .vgtest,$(INSN_TESTS)) \
	lahf.stdout.exp lahf.stderr.exp lahf.vgtest \
	looper.stderr.exp looper.stdout.exp looper.vgtest \
	pushpopseg.stderr.exp pushpopseg.stdout.exp pushpopseg.vgtest \
	sbbmisc.stderr.exp sbbmisc.stdout.exp sbbmisc.vgtest \
	seg_override.stderr.exp seg_override.stdout.exp seg_override.vgtest \
	sigcontext.stdout.exp sigcontext.stderr.exp sigcontext.vgtest \
	smc1.stderr.exp smc1.stdout.exp smc1.vgtest \
	yield.stderr.exp yield.stdout.exp yield.vgtest

# cmpxchg8b faultstatus fxtract incdec_alt lahf looper sbbmisc seg_override 
# sigcontext smc1 yield
# The above are missing somehow... add them back when we find them again :)
check_PROGRAMS = \
	badseg bt_everything bt_literal cpuid \
	fpu_lazy_eflags \
	getseg $(INSN_TESTS) \
	int pushpopseg

AM_CFLAGS    = $(WERROR) -Winline -Wall -Wshadow \
		@FLAG_M32@ -g -I$(top_srcdir)/include \
		$(FLAG_MMMX) $(FLAG_MSSE)
AM_CXXFLAGS  = $(AM_CFLAGS)
AM_CCASFLAGS = @FLAG_M32@

# generic C ones
cpuid_SOURCES 		= cpuid_c.c cpuid_s.s
# fpu_lazy_eflags must use these flags -- the bug only occurred with them
fpu_lazy_eflags_CFLAGS	= @FLAG_M32@ -O2 -march=pentiumpro
fxtract_LDADD		= -lm
insn_basic_SOURCES	= insn_basic.def
insn_basic_LDADD	= -lm
insn_fpu_SOURCES	= insn_fpu.def
insn_fpu_LDADD		= -lm
insn_cmov_SOURCES	= insn_cmov.def
insn_cmov_LDADD		= -lm
insn_mmx_SOURCES	= insn_mmx.def
insn_mmx_LDADD		= -lm
insn_mmxext_SOURCES	= insn_mmxext.def
insn_mmxext_LDADD	= -lm
insn_sse_SOURCES	= insn_sse.def
insn_sse_LDADD		= -lm
insn_sse2_SOURCES	= insn_sse2.def
insn_sse2_LDADD		= -lm
yield_CFLAGS		= $(AM_CFLAGS) -D__$(VG_ARCH)__
yield_LDADD		= -lpthread

.def.c: $(srcdir)/gen_insn_test.pl
	$(PERL) $(srcdir)/gen_insn_test.pl < $< > $@
