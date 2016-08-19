

PWLIBDIR	= /home/speechbridge/src/ProxySrv/SBMediaSrv/opensipstack
OPALDIR         = /home/speechbridge/src/ProxySrv/SBMediaSrv/opensipstack
SQLITEDIR       = 
CPPSQLITEDIR    = 
OPENSIPSTACKDIR	= /home/speechbridge/src/ProxySrv/SBMediaSrv/opensipstack
STDCCFLAGS	+=  -D_REENTRANT -I/home/speechbridge/src/ProxySrv/SBMediaSrv/opensipstack/include
LDFLAGS		+= 
LDLIBS		+= 
ENDLDLIBS	:=  -lpthread -lexpat -lresolv -ldl $(ENDLDLIBS)

################################################################################

ifdef LIBRARY_MAKEFILE
include $(PWLIBDIR)/make/unix.mak
else
ifeq ($(NOTRACE), 1)
OBJDIR_SUFFIX := n
endif # NOTRACE
include $(PWLIBDIR)/make/ptlib.mak
endif # LIBRARY_MAKEFILE

ifeq ($(NOTRACE), 1)
STDCCFLAGS += -DPASN_NOPRINTON -DPASN_LEANANDMEAN
OSS_SUFFIX = n
else
STDCCFLAGS += -DPTRACING
RCFLAGS	   += -DPTRACING
OSS_SUFFIX = $(OBJ_SUFFIX)
endif # NOTRACE

################################################################################

HAS_SQLITE = 0

ifeq ($(HAS_SQLITE), 1)

SQLITE_SRCDIR = $(SQLITEDIR)/src
SQLITE_INCDIR = $(SQLITEDIR)/
SQLITE_LIBDIR = $(SQLITEDIR)/.libs

LIBDIRS += $(SQLITE_LIBDIR)

LDFLAGS	    += -L$(SQLITE_LIBDIR)
LDLIBS	    += -lsqlite3
SYSLIBS     += -lsqlite3

STDCCFLAGS  += -I$(SQLITE_INCDIR)

endif

################################################################################

HAS_CPPSQLITE = 

ifeq ($(HAS_CPPSQLITE), 1)

CPPSQLITE_SRCDIR = $(CPPSQLITEDIR)
CPPSQLITE_INCDIR = $(CPPSQLITEDIR)

STDCCFLAGS  += -I$(CPPSQLITE_INCDIR)

endif

################################################################################

HAS_OPAL = 1

ifeq ($(HAS_OPAL), 1)

OPAL_SRCDIR = $(OPALDIR)/src
OPAL_INCDIR = $(OPALDIR)/include
OPAL_SIP          = 0
OPAL_H323         = 0
OPAL_IAX2         = 0
OPAL_SYSTEM_SPEEX = 0
STDCCFLAGS  += -I$(OPAL_INCDIR)

endif

################################################################################

LIBDIRS += $(OPENSIPSTACKDIR)
OSS_SRCDIR = $(OPENSIPSTACKDIR)/src
ifdef PREFIX
OSS_INCDIR = $(PREFIX)/include/opensipstack
else
OSS_INCDIR = $(OPENSIPSTACKDIR)/include
endif # PREFIX
ifndef OSS_LIBDIR
OSS_LIBDIR = $(OPENSIPSTACKDIR)/lib
endif # OSS_LIBDIR


OSS_BASE  = opensipstack_$(PLATFORM_TYPE)_$(OSS_SUFFIX)$(LIB_TYPE)
OSS_FILE  = lib$(OSS_BASE).$(LIB_SUFFIX)

LDFLAGS	    += -L$(OSS_LIBDIR)
LDLIBS	    := -l$(OSS_BASE) $(LDLIBS)
STDCCFLAGS  += -I$(OSS_INCDIR)


$(TARGET) :	$(OSS_LIBDIR)/$(OSS_FILE)

ifndef LIBRARY_MAKEFILE
ifdef DEBUG
$(OSS_LIBDIR)/$(OSS_FILE):
	$(MAKE) -C $(OSS_SRCDIR) debug
else
$(OSS_LIBDIR)/$(OSS_FILE):
	$(MAKE) -C $(OSS_SRCDIR) opt
endif # DEBUG
endif # LIBRARY_MAKEFILE

# End of file

