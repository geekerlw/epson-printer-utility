                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ()
AC_INIT(PrinterUtility, 1.0.2, epson@localdomain)
AM_INIT_AUTOMAKE


AC_CONFIG_SRCDIR(PrinterUtility/Main.cpp)
AC_CONFIG_HEADER([config.h])

EPS_PREFIX=epson-printer
AC_SUBST(EPS_PREFIX)

PACKAGE_NAME=$EPS_PREFIX-utility
AC_SUBST(PACKAGE_NAME)

#CC=/opt/lsb/bin/lsbcc
CC=/opt/lsb/bin/lsbc++
CXX=/opt/lsb/bin/lsbc++

# EPS_INSTALL_DIR=/opt/$PACKAGE_NAME
# AC_SUBST(EPS_INSTALL_DIR)

# EPS_BIN_DIR=$EPS_INSTALL_DIR/bin
# AC_SUBST(EPS_BIN_DIR)

# EPS_CONFIG_DIR=$EPS_INSTALL_DIR/config
# AC_SUBST(EPS_CONFIG_DIR)

# EPS_LIB_DIR=$EPS_INSTALL_DIR/lib
# AC_SUBST(EPS_LIB_DIR)

# EPS_RESOURCE_DIR=$EPS_INSTALL_DIR/resource
# AC_SUBST(EPS_RESOURCE_DIR)

EPS_RULES_DIR=/ect/udev/rules.d
AC_SUBST(EPS_SHELL_DIR)

ENDIF=endif
AC_SUBST(ENDIF)

ELSE=else
AC_SUBST(ELSE)

LIBARCH=lib
BUILD_MACHINE_ARCH=`uname -m`
if test "x$BUILD_MACHINE_ARCH" = "xx86_64"; then
	LIBARCH=lib64
fi
AC_SUBST(LIBARCH)

AM_CONDITIONAL([ARCH_CON], [test "x$BUILD_MACHINE_ARCH" = "xx86_64"])

AH_TOP(
#undef LINUX_API
#undef BSD_API
#undef DARWIN_API

#undef HAVE_OLD_DEV_USB_USB_H

#undef HAVE_LIMITS_H
#undef HAVE_UNISTD_H
#undef HAVE_VALUES_H

#undef LIBUSB_MAJOR_VERSION
#undef LIBUSB_MINOR_VERSION
#undef LIBUSB_MICRO_VERSION
#undef LIBUSB_INTERFACE_AGE
#undef LIBUSB_BINARY_AGE
)

# Save this value here, since automake will set cflags later
cflags_set=${CFLAGS+set}

AM_INIT_AUTOMAKE([gnu nostdinc subdir-objects 1.7.6 --warnings=all])

LIBUSB_VERSION=$VERSION

#
# The following version number definitions apply to libusb
# as a whole, so if changes occoured in any of them, they are all
# treated with the same interface and binary age.
#
# Making releases:
#   LIBUSB_MICRO_VERSION += 1;
#   LIBUSB_INTERFACE_AGE += 1;
#   LIBUSB_BINARY_AGE += 1;
# if any functions have been added, set LIBUSB_INTERFACE_AGE to 0.
# if backwards compatibility has been broken,
# set LIBUSB_BINARY_AGE _and_ LIBUSB_INTERFACE_AGE to 0.
#
LIBUSB_MAJOR_VERSION=0
LIBUSB_MINOR_VERSION=1
LIBUSB_MICRO_VERSION=12
LIBUSB_INTERFACE_AGE=4
LIBUSB_BINARY_AGE=8


AC_SUBST(LIBUSB_MAJOR_VERSION)
AC_SUBST(LIBUSB_MINOR_VERSION)
AC_SUBST(LIBUSB_MICRO_VERSION)
AC_SUBST(LIBUSB_VERSION)
AC_SUBST(LIBUSB_INTERFACE_AGE)
AC_SUBST(LIBUSB_BINARY_AGE)

AC_DEFINE_UNQUOTED(LIBUSB_MAJOR_VERSION, $LIBUSB_MAJOR_VERSION)
AC_DEFINE_UNQUOTED(LIBUSB_MINOR_VERSION, $LIBUSB_MINOR_VERSION)
AC_DEFINE_UNQUOTED(LIBUSB_MICRO_VERSION, $LIBUSB_MICRO_VERSION)
AC_DEFINE_UNQUOTED(LIBUSB_INTERFACE_AGE, $LIBUSB_INTERFACE_AGE)
AC_DEFINE_UNQUOTED(LIBUSB_BINARY_AGE, $LIBUSB_BINARY_AGE)

# libtool versioning
# JR: Add support for MICRO_VERSION to be alpha-numeric. This will make
# lt_current use the numeric part of MICRO_VERSION

LIBUSB_MICRO_VERSION_NUM=`echo $LIBUSB_MICRO_VERSION | sed 's/[[a-zA-Z]]//g'`
LT_RELEASE=$LIBUSB_MAJOR_VERSION.$LIBUSB_MINOR_VERSION
LT_CURRENT=`expr $LIBUSB_MICRO_VERSION_NUM - $LIBUSB_INTERFACE_AGE`
LT_REVISION=$LIBUSB_INTERFACE_AGE
LT_AGE=`expr $LIBUSB_BINARY_AGE - $LIBUSB_INTERFACE_AGE`
AC_SUBST(LT_RELEASE)
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

dnl Initialize libtool
AM_PROG_LIBTOOL

dnl This has already been dragged in by AM_PROG_LIBTOOL
dnl AC_CANONICAL_HOST

LINUX_API=0
DARWIN_API=0
BSD_API=0

AC_MSG_CHECKING(for what USB OS support)
case $host in
  *-linux*)
    AC_DEFINE(LINUX_API, 1)
    AC_DEFINE(BSD_API, 0)
    AC_DEFINE(DARWIN_API, 0)
    LINUX_API=1
    os_support=linux
    AC_MSG_RESULT(Linux)
    OSLIBS=""
    ;;
  *-freebsd*|*-kfreebsd*-gnu|*-openbsd*|*-netbsd*)
    AC_DEFINE(BSD_API, 1)
    AC_DEFINE(LINUX_API, 0)
    AC_DEFINE(DARWIN_API, 0)
    BSD_API=1
    os_support=bsd
    AC_MSG_RESULT(FreeBSD, OpenBSD and/or NetBSD)
    OSLIBS=""
    ;;
  *-darwin*)
    AC_DEFINE(DARWIN_API, 1) 
    AC_DEFINE(LINUX_API, 0)
    AC_DEFINE(BSD_API, 0)
    DARWIN_API=1
    os_support=darwin
    AC_MSG_RESULT(Darwin and/or MacOS 10)
    OSLIBS="-Wl,-framework -Wl,IOKit -Wl,-framework -Wl,CoreFoundation -Wl,-prebind"
    ;;
  *)
    AC_MSG_RESULT(unknown operating system)
    AC_MSG_ERROR(libusb does not support compiling for $host)
    ;;
esac

AC_SUBST(DARWIN_API)
AC_SUBST(LINUX_API)
AC_SUBST(BSD_API)

AM_CONDITIONAL(LINUX_API, test "$os_support" = "linux")
AM_CONDITIONAL(BSD_API, test "$os_support" = "bsd")
AM_CONDITIONAL(DARWIN_API, test "$os_support" = "darwin")

AC_SUBST(OSLIBS)

if test `expr $LIBUSB_MINOR_VERSION \% 2` = 1 ; then
  debug_default=yes
else
  debug_default=minimum
fi

dnl declare --enable-* args and collect ac_help strings
AC_ARG_ENABLE(debug, [  --enable-debug=[no/minimum/yes] turn on debugging [default=$debug_default]],,enable_debug=$debug_default)

if test "x$enable_debug" = "xyes"; then
  test "$cflags_set" = set || CFLAGS="$CFLAGS -g"
fi

# Checks for programs.
AC_PROG_CXX
AC_LANG_CPLUSPLUS
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL

changequote(,)dnl
if test "x$GCC" = "xyes"; then
  case " $CFLAGS " in
  *[\ \	]-Wall[\ \	]*) ;;
  *) CFLAGS="$CFLAGS -Wall" ;;
  esac
fi
changequote([,])dnl

# Checks for header files.
AC_HEADER_STDC

# Checks for library functions.
AC_FUNC_VPRINTF

AC_C_CONST

dnl for bytesex stuff
AC_C_BIGENDIAN

if test "$ac_cv_c_bigendian" = "yes"; then
  BIGENDIAN="1"
else
  BIGENDIAN="0"
fi
AC_SUBST(BIGENDIAN)

dnl header file checks
AC_CHECK_HEADERS(limits.h, AC_DEFINE_UNQUOTED(HAVE_LIMITS_H))
AC_CHECK_HEADERS(unistd.h, AC_DEFINE_UNQUOTED(HAVE_UNISTD_H))
AC_CHECK_HEADERS(values.h, AC_DEFINE_UNQUOTED(HAVE_VALUES_H))

# Check for some functions
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_TYPE_SIGNAL
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([ \
    floor \
    memmove \
    memset \
    nl_langinfo \
    select \
    setlocale \
    strdup \
    strerror \
    ])

AC_CHECK_FUNCS(memmove)

if test "$os_support" = "bsd"; then
  AC_MSG_CHECKING(if dev/usb/usb.h uses new naming convention)
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <dev/usb/usb.h>]], [[
       int a = ((struct usb_ctl_request *)0L)->ucr_addr
     ]])],[AC_MSG_RESULT(yes)],[AC_MSG_RESULT(no)
    AC_DEFINE_UNQUOTED(HAVE_OLD_DEV_USB_USB_H)])
fi

#pkg-config: pkg-config is a tool to simplify checks for libraries and versions.
pkgconfig_msg="no (http://www.epson.co.jp)"
try_pkgconfig=true
have_pkgconfig=false
AC_ARG_WITH(pkgconfig, [  --without-pkgconfig     don"t use pkg-config],[
  if test x$withval = xno; then
    try_pkgconfig=false
    pkgconfig_msg="no (not requested)"
  fi])
if $try_pkgconfig; then
  AC_PATH_PROG(PKG_CONFIG,pkg-config)
  if test -n "${PKG_CONFIG}"; then
    have_pkgconfig=true
    pkgconfig_msg="yes"
  fi
fi
# guessing directory to install *.pc into
pkgconfigdir="${libdir}/pkgconfig"
AC_SUBST(pkgconfigdir)

# Check if the user wants to enable documentation
AC_MSG_CHECKING(if we should build the documentation)
AC_ARG_ENABLE(build-docs, [  --disable-build-docs    turn off building documentation],
	[ test "$enableval" != no && build_docs=yes || build_docs=no ],
	build_docs=auto)
AC_MSG_RESULT($build_docs)

if test "$build_docs" != "no"; then
  AC_CHECK_PROG(JADE, jade, jade)
  if test  -z "$JADE"; then
    AC_CHECK_PROG(JADE, openjade, openjade)
  fi
fi

dnl Not quite ready for primetime --jerdfelt
dnl AC_CHECK_PROG(DVIPS, dvips, dvips, false)
dnl if test "$DVIPS" = "false"; then
dnl   unset BUILD_DOCS
dnl fi

if test "$build_docs" != "no" -a -z "$JADE"; then
  AC_MSG_WARN(Documentation build components not installed. sgml docs will not be built)
  build_docs="no"
fi

AM_CONDITIONAL(BUILD_DOCS, test "$build_docs" != "no")

AC_CONFIG_FILES([
		Makefile
		PrinterUtility/Makefile
		PrinterUtility/Languages/Makefile
		EPSCommonLib/Makefile
])
AC_OUTPUT
