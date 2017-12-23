#!/bin/sh
# Run this to set up the build system: configure, makefiles, etc.
set -e

echo "Updating build configuration files, please wait...."

ACLOCAL_FLAGS="-I m4"
autoreconf -vfi
