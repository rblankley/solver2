# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.64)
AC_INIT(solver, 2.0.0, rblankley@woh.rr.com)

AC_CONFIG_AUX_DIR([.])
AM_INIT_AUTOMAKE

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADER([config.h])

# Doxygen support
DX_DOXYGEN_FEATURE(ON)
DX_HTML_FEATURE(ON)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN(solver, Doxyfile, doc/html)

# use the C++ compiler for the following checks
AC_LANG([C++])
AX_CXX_COMPILE_STDCXX_17([noext], [optional])

if test $HAVE_CXX17 = 1; then
	AC_MSG_NOTICE([Found c++17 support])
else
	AX_CXX_COMPILE_STDCXX_14([noext], [optional])

	if test $HAVE_CXX14 = 1; then
		AC_MSG_NOTICE([Found c++14 support])
	else
		AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])
	fi
fi

# distribute additional compiler and linker flags among Makefiles
# --> set and change these variables instead of CXXFLAGS or LDFLAGS (for user only)
AC_SUBST([AM_CXXFLAGS])
AC_SUBST([AM_LDFLAGS])

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.
ACX_PTHREAD

# Checks for header files.
AC_CHECK_HEADERS([array])
AC_CHECK_HEADERS([cassert])
AC_CHECK_HEADERS([chrono])
AC_CHECK_HEADERS([condition_variable])
AC_CHECK_HEADERS([cstdarg])
AC_CHECK_HEADERS([cstdio])
AC_CHECK_HEADERS([ctime])
AC_CHECK_HEADERS([future])
AC_CHECK_HEADERS([list])
AC_CHECK_HEADERS([locale])
AC_CHECK_HEADERS([map])
AC_CHECK_HEADERS([memory])
AC_CHECK_HEADERS([mutex])
AC_CHECK_HEADERS([new])
AC_CHECK_HEADERS([iostream])
AC_CHECK_HEADERS([shared_mutex])
AC_CHECK_HEADERS([sstream])
AC_CHECK_HEADERS([stack])
AC_CHECK_HEADERS([stdexcept])
AC_CHECK_HEADERS([string])
AC_CHECK_HEADERS([thread])
AC_CHECK_HEADERS([vector])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# creating top level and subdirectory Makefiles:
AC_CONFIG_FILES([
	Makefile
	src/Makefile
])

AC_OUTPUT

