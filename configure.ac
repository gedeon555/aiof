dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(aiof, 1.0)

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CXX
AM_PROG_CC_STDC
AC_HEADER_STDC

PKG_CHECK_MODULES(DEPS, gtk+-2.0 >= 2.2 glib-2.0 >= 2.2 libglade-2.0 >= 2.6)
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)

AM_PROG_LIBTOOL

AC_OUTPUT([
Makefile
src/Makefile

])
