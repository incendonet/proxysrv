


PREFIX=/usr/local
exec_prefix = $(PREFIX)
LIBDIR=${exec_prefix}/lib
export OPENSIPSTACKDIR=/home/speechbridge/src/ProxySrv/SBMediaSrv/opensipstack

INSTALL=/usr/bin/install -c

default :: optnoshared

all :: bothnoshared

include $(OPENSIPSTACKDIR)/make/opensipstack.mak

SUBDIRS := src/pwlib src



PREFIX=/usr/local
exec_prefix = ${PREFIX}
LIBDIR=${exec_prefix}/lib

ifndef PREFIX
PREFIX=/usr/local
endif


# standard build command
# all these targets are just passed to all subdirectories
$(subst tagbuild,,$(STANDARD_TARGETS)) ::
	set -e; $(foreach dir,$(SUBDIRS),$(MAKE) -C $(dir) $@;)



