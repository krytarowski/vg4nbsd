
AUTOMAKE_OPTIONS = foreign 1.6 dist-bzip2

include $(top_srcdir)/Makefile.all.am 

## addrcheck must come after memcheck, for mac_*.o
TOOLS =		none \
		memcheck

##		addrcheck \
##		memcheck
# Temporary: we want to compile Helgrind, but not regtest it.
# And we want to include Addrcheck in the distro, but not compile/test it.
# Put docs last because building the HTML is slow and we want to get
# everything else working before we try it.
#SUBDIRS = include coregrind . tests auxprogs $(TOOLS) helgrind docs
SUBDIRS = include coregrind . $(TOOLS) 
# For VG4nbsd , we are not compiling auxprogs and tests for now  or docs

DIST_SUBDIRS  = $(SUBDIRS) addrcheck

SUPP_FILES = \
	glibc-2.2.supp glibc-2.3.supp glibc-2.4.supp \
	xfree-3.supp xfree-4.supp

dist_val_DATA = $(SUPP_FILES) default.supp

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = valgrind.pc

incincdir = $(includedir)/valgrind
nodist_incinc_HEADERS = $(VEX_PUBLIC_HDRS)

BUILT_SOURCES  = default.supp valgrind.pc
CLEANFILES     = 
DISTCLEANFILES = default.supp

if VG_X86_LINUX
BUILT_SOURCES += valt_load_address_x86_linux.lds
CLEANFILES += valt_load_address_x86_linux.lds
endif
if VG_AMD64_LINUX
BUILT_SOURCES += valt_load_address_amd64_linux.lds
CLEANFILES += valt_load_address_amd64_linux.lds
endif
if VG_PPC32_LINUX
BUILT_SOURCES += valt_load_address_ppc32_linux.lds
CLEANFILES += valt_load_address_ppc32_linux.lds
endif
if VG_X86_NETBSD
BUILT_SOURCES += valt_load_address_x86_netbsd.lds
CLEANFILES += valt_load_address_x86_netbsd.lds
# XXX sjamaan: We're using valt_load_address.lds now
#BUILT_SOURCES += valt_load_address_x86_netbsd.lds
#CLEANFILES += valt_load_address_x86_netbsd.lds
endif

default.supp: $(SUPP_FILES)

## Preprend @PERL@ because tests/vg_regtest isn't executable
regtest: check
	@PERL@ tests/vg_regtest $(TOOLS)

EXTRA_DIST = \
	ACKNOWLEDGEMENTS \
	README_DEVELOPERS \
	README_PACKAGERS \
	README_MISSING_SYSCALL_OR_IOCTL \
	valgrind.spec.in valgrind.pc.in \
	Makefile.all.am Makefile.tool.am Makefile.core.am \
	Makefile.tool-inplace.am \
	$(VEX_PRIMARY_SOURCES)

install-exec-hook:
	$(mkinstalldirs) $(DESTDIR)$(valdir)

all-local:
	mkdir -p $(inplacedir)
	rm -f $(addprefix $(inplacedir)/,default.supp $(SUPP_FILES))
	ln -s ../default.supp $(inplacedir)
	ln -s $(addprefix ../$(top_srcdir)/,$(SUPP_FILES)) $(inplacedir)

distclean-local:
	rm -rf $(inplacedir)

# These list the bits of vex we need to install

VEX_PUBLIC_HDRS = \
	@VEX_DIR@/pub/libvex_guest_amd64.h \
	@VEX_DIR@/pub/libvex_ir.h \
	@VEX_DIR@/pub/libvex_guest_ppc32.h \
	@VEX_DIR@/pub/libvex_basictypes.h \
	@VEX_DIR@/pub/libvex_guest_offsets.h \
	@VEX_DIR@/pub/libvex_emwarn.h \
	@VEX_DIR@/pub/libvex.h \
	@VEX_DIR@/pub/libvex_trc_values.h \
	@VEX_DIR@/pub/libvex_guest_arm.h \
	@VEX_DIR@/pub/libvex_guest_x86.h

# These list the bits of vex we need to copy into the tarball

VEX_PRIMARY_SOURCES = \
	VEX/HACKING.README \
	VEX/LICENSE.README \
	VEX/LICENSE.GPL \
	VEX/Makefile \
	VEX/auxprogs/genoffsets.c \
	VEX/pub/libvex_guest_amd64.h \
	VEX/pub/libvex_ir.h \
	VEX/pub/libvex_guest_ppc32.h \
	VEX/pub/libvex_basictypes.h \
	VEX/pub/libvex_guest_offsets.h \
	VEX/pub/libvex_emwarn.h \
	VEX/pub/libvex.h \
	VEX/pub/libvex_trc_values.h \
	VEX/pub/libvex_guest_arm.h \
	VEX/pub/libvex_guest_x86.h \
	VEX/priv/ir/irmatch.c \
	VEX/priv/ir/irmatch.h \
	VEX/priv/ir/irdefs.c \
	VEX/priv/ir/iropt.c \
	VEX/priv/ir/iropt.h \
	VEX/priv/host-ppc32/isel.c \
	VEX/priv/host-ppc32/hdefs.c \
	VEX/priv/host-ppc32/hdefs.h \
	VEX/priv/main/vex_svnversion.h \
	VEX/priv/main/vex_globals.c \
	VEX/priv/main/vex_globals.h \
	VEX/priv/main/vex_main.c \
	VEX/priv/main/vex_util.c \
	VEX/priv/main/vex_util.h \
	VEX/priv/guest-arm/ghelpers.c \
	VEX/priv/guest-arm/gdefs.h \
	VEX/priv/guest-arm/toIR.c \
	VEX/priv/guest-x86/ghelpers.c \
	VEX/priv/guest-x86/gdefs.h \
	VEX/priv/guest-x86/toIR.c \
	VEX/priv/guest-generic/g_generic_x87.c \
	VEX/priv/guest-generic/g_generic_x87.h \
	VEX/priv/guest-generic/bb_to_IR.c \
	VEX/priv/guest-generic/bb_to_IR.h \
	VEX/priv/host-arm/isel.c \
	VEX/priv/host-arm/hdefs.c \
	VEX/priv/host-arm/hdefs.h \
	VEX/priv/host-x86/isel.c \
	VEX/priv/host-x86/hdefs.c \
	VEX/priv/host-x86/hdefs.h \
	VEX/priv/guest-amd64/ghelpers.c \
	VEX/priv/guest-amd64/gdefs.h \
	VEX/priv/guest-amd64/toIR.c \
	VEX/priv/guest-ppc32/ghelpers.c \
	VEX/priv/guest-ppc32/gdefs.h \
	VEX/priv/guest-ppc32/toIR.c \
	VEX/priv/host-generic/reg_alloc2.c \
	VEX/priv/host-generic/h_generic_regs.c \
	VEX/priv/host-generic/h_generic_regs.h \
	VEX/priv/host-generic/h_generic_simd64.c \
	VEX/priv/host-generic/h_generic_simd64.h \
	VEX/priv/host-amd64/isel.c \
	VEX/priv/host-amd64/hdefs.c \
	VEX/priv/host-amd64/hdefs.h


# Generate a linker script for linking the binaries.  This is the
# standard gcc linker script, except hacked so that an alternative
# load address can be specified by (1) asking gcc to use this script
# (-Wl,-T,valt_load_address.lds) and (2) setting the symbol
# valt_load_address to the required value
# (-Wl,-defsym,valt_load_address=0x70000000).
#
# Extract ld's default linker script and hack it to our needs.
# First we cut everything above and below the "=====..." lines at the top
# and bottom.
# Then we have to replace the load address with "valt_load_address".
# The line to replace in has one of the following two forms:
#
#   . = 0x08048000 + SIZEOF_HEADERS;
#
# or
#   PROVIDE (__executable_start = 0x08048000); . = 0x08048000 + SIZEOF_HEADERS;
#
# So we search for the line with a hex value "+ SIZEOF_HEADERS", and replace
# all the hex values in that line with "valt_load_address".
#valt_load_address_x86_linux.lds: Makefile
#	$(CC) @FLAG_M32@ -Wl,--verbose -nostdlib 2>&1 | sed \
#		-e '1,/^=====\+$$/d' \
#		-e '/^=====\+$$/d' \
#		-e '/\. = 0x[0-9A-Fa-f]\+ + SIZEOF_HEADERS/s/0x[0-9A-Fa-f]\+/valt_load_address/g' > $@ \
#	|| rm -f $@
#
#valt_load_address_amd64_linux.lds: Makefile
#	$(CC) -m64 -Wl,--verbose -nostdlib 2>&1 | sed \
#		-e '1,/^=====\+$$/d' \
#		-e '/^=====\+$$/d' \
#		-e '/\. = 0x[0-9A-Fa-f]\+ + SIZEOF_HEADERS/s/0x[0-9A-Fa-f]\+/valt_load_address/g' > $@ \
#	|| rm -f $@
#
#valt_load_address_ppc32_linux.lds: Makefile
#	$(CC) -Wl,--verbose -nostdlib 2>&1 | sed \
#		-e '1,/^=====\+$$/d' \
#		-e '/^=====\+$$/d' \
#		-e '/\. = 0x[0-9A-Fa-f]\+ + SIZEOF_HEADERS/s/0x[0-9A-Fa-f]\+/valt_load_address/g' > $@ \
#	|| rm -f $@
#
valt_load_address_x86_netbsd.lds: Makefile
	$(CC) @FLAG_M32@ -Wl,--verbose -nostdlib 2>&1 | awk \
        '!skipped && $$0 !~ /^=====+$$/ { next } \
	/^=====+$$/ { skipped = 1; next } \
	/\. = 0x[0-9A-Fa-f]+ \+ SIZEOF_HEADERS/ { gsub(/0x[0-9A-Fa-f]+/, "valt_load_address"); } \
	{ print }' > $@ \
	|| rm -f $@

#valt_load_address_x86_netbsd.lds: Makefile
#	$(CC) @FLAG_M32@ -Wl,--verbose -nostdlib 2>&1 | sed \
#		-e '1,/^=====\+$$/d' \
#		-e '/^=====\+$$/d' \
#		-e '/\. = 0x[0-9A-Fa-f]\+ + SIZEOF_HEADERS/s/0x[0-9A-Fa-f]\+/valt_load_address/g' > $@ \
#	|| rm -f $@
