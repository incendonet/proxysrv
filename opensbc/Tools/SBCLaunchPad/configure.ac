AC_INIT( SBCLaunchPad.h )

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
  AC_CHECK_FILE( ../../../opensipstack/OpenSIPStack.sln, ENABLE_OPENSIPSTACK=1)
  if test "${ENABLE_OPENSIPSTACK:-unset}" != "unset" ; then
    OPENSIPSTACKDIR="../../../opensipstack"
  fi
fi

if test "${ENABLE_OPENSIPSTACK:-unset}" = "unset" ; then
  AC_MSG_ERROR( must have OpenSIPStack istalled in the same directory level as opensbc or set it manually be defining OPENSIPSTACKDIR! )
  exit
fi

AC_SUBST(OPENSIPSTACKDIR)


dnl ########################################################################
dnl Header file

AC_CONFIG_FILES(Makefile)

dnl ########################################################################
dnl Output configured files

AC_OUTPUT()
