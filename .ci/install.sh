#!/usr/bin/env bash
set -x
set -e

function install-cmake() {
  # need CMake >= 3.14 (for using the newly swig built-in UseSWIG module)
  if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
    wget https://cmake.org/files/v3.16/cmake-3.16.2.tar.gz
    tar xzf cmake-3.16.2.tar.gz && rm cmake-3.16.2.tar.gz
    cd cmake-3.16.2 && ./bootstrap --prefix=/opt/cmake-3.16.2
    make -j 2
    sudo make install
    cd .. && rm -rf cmake-3.16.2
    export PATH=/opt/cmake-3.16.2/bin:$PATH
    command -v cmake
    cmake --version
  elif [ "$TRAVIS_OS_NAME" == "osx" ]; then
    cmake --version
  fi
}


eval "${MATRIX_EVAL}"

if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
  /usr/bin/x86_64-linux-gnu-ld --version
elif [ "$TRAVIS_OS_NAME" == "osx" ]; then
  brew update
fi
if [[ "$LANGUAGE" == "python" ]]; then
  install-python
elif [[ "$LANGUAGE" == "dotnet" ]]; then
  install-dotnet-sdk
elif [[ "$LANGUAGE" == "java" ]]; then
  install-java
fi