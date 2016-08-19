AC_INIT( OpenSBC.h )

AC_PROG_CXX
AC_LANG(C++)
AC_HEADER_STDC

dnl ########################################################################
dnl Look for OPENSIPSTACK
dnl MSWIN_DISPLAY     opensipstack,OpenSIPStack
dnl MSWIN_CHECK_FILE  opensipstack,OpenSIPStack.sln,ENABLE_OPENSIPSTACK=1
dnl MSWIN_DIR_SYMBOL  opensipstack,OPENSIPSTACKDIR
dnl MSWIN_CHECK_VERSION opensipstack,Version.h,1,1,3

dnl ########################################################################
dnl look for opensipstack, use a preference order of explicit OpenSIPStack, external directory

if test "${OPENSIPSTACKDIR:-unset}" != "unset" ; then
  AC_CHECK_FILE(${OPENSIPSTACKDIR}/OpenSIPStack.sln, ENABLE_OPENSIPSTACK=1)
fi

if test "${ENABLE_OPENSIPSTACK:-unset}" = "unset" ; then
  AC_CHECK_FILE( ../../OpenSIPStack.sln, ENABLE_OPENSIPSTACK=1)
  if test "${ENABLE_OPENSIPSTACK:-unset}" != "unset" ; then
    OPENSIPSTACKDIR="../../"
  fi
fi

if test "${ENABLE_OPENSIPSTACK:-unset}" = "unset" ; then
  AC_CHECK_FILE( ../opensipstack/OpenSIPStack.sln, ENABLE_OPENSIPSTACK=1)
  if test "${ENABLE_OPENSIPSTACK:-unset}" != "unset" ; then
    OPENSIPSTACKDIR="../opensipstack"
  fi
fi

if test "${ENABLE_OPENSIPSTACK:-unset}" = "unset" ; then
  AC_MSG_ERROR( must have OpenSIPStack! )
  exit
fi

AC_SUBST(OPENSIPSTACKDIR)

dnl ########################################################################
dnl Look for RTBEClient
dnl MSWIN_DISPLAY     rtbeclient,RTBEClient
dnl MSWIN_CHECK_FILE  rtbeclient,RTBESession.h,HAS_RTBECLIENT=1
dnl MSWIN_DIR_SYMBOL  rtbeclient,RTBECLIENTDIR
dnl MSWIN_CHECK_VERSION rtbeclient,version.h,1,1,1

dnl ########################################################################

dnl ########################################################################
dnl look for rtbeclient, use a preference order of explicit RTBEClient, external directory
if test "${RTBECLIENTDIR:-unset}" != "unset" ; then
  AC_CHECK_FILE(${RTBECLIENTDIR}/RTBESession.h, ENABLE_RTBECLIENT=1)
fi

if test "${ENABLE_RTBECLIENT:-unset}" = "unset" ; then
  AC_CHECK_FILE(RTBESession.h, ENABLE_RTBECLIENT=1)
fi

if test "${ENABLE_RTBECLIENT:-unset}" = "unset" ; then
  echo "RTBEClient support DISABLED..."
  HAS_RTBECLIENT=0
else
  echo "RTBEClient support ENABLED..."
  AC_DEFINE(HAS_RTBECLIENT, 1, "RTBEClient found")
  HAS_RTBECLIENT=1
fi

AC_SUBST(RTBECLIENTDIR)
AC_SUBST(HAS_RTBECLIENT)

dnl ########################################################################
dnl Header file

AC_CONFIG_FILES(Makefile)
AC_CONFIG_HEADERS(sbcbuildopts.h)

dnl ########################################################################
dnl Output configured files

AC_OUTPUT()
