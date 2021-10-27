#!/usr/bin/env bash
set -x
set -e

###########################
## DOWNLOAD DEPENDENCIES ##
###########################

mkdir lib && mkdir lib/googletest
git clone https://github.com/google/googletest.git lib/googletest

#################
##  CONFIGURE  ##
#################
echo "CONFIGURING"

mkdir -p build

if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
  export PATH=/opt/cmake-3.16.2/bin:$PATH
fi

cmake --version

LDFLAGS=-v cmake -S. -Bbuild


#############
##  BUILD  ##
#############
echo "BUILDING"
cmake --build build --target all -- VERBOSE=1

############
##  TEST  ##
############
echo "TESTING"
cmake --build build --target test
# vim: set tw=0 ts=2 sw=2 expandtab: