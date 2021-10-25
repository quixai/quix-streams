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

function install-swig() {
  if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
    # apt-get only have swig 2.0.11
    # Need SWIG >= 3.0.8
    cd /tmp/
    wget https://github.com/swig/swig/archive/rel-4.0.1.tar.gz
    tar zxf rel-4.0.1.tar.gz
    cd swig-rel-4.0.1
    ./autogen.sh
    ./configure --prefix=/usr
    make -j 2
    sudo make install
  elif [ "$TRAVIS_OS_NAME" == "osx" ]; then
    brew install swig
  fi
}

function install-python(){
  if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
    # work around https://github.com/travis-ci/travis-ci/issues/8363
    pyenv global 3.7
    python --version
    python -m pip --version
    python -m pip install virtualenv wheel six
  elif [ "$TRAVIS_OS_NAME" == "osx" ]; then
    python3 -m pip install virtualenv wheel six
  fi
}

function install-dotnet-sdk(){
  if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
    # see https://docs.microsoft.com/en-us/dotnet/core/install/linux-package-manager-ubuntu-1804
    sudo apt-get install -yq apt-transport-https
    wget -q https://packages.microsoft.com/config/ubuntu/18.04/packages-microsoft-prod.deb
    sudo dpkg -i packages-microsoft-prod.deb
    sudo apt-get update -qq
    sudo apt-get install -yq dotnet-sdk-3.1
  elif [ "$TRAVIS_OS_NAME" == "osx" ]; then
    #brew tap homebrew/cask-cask
    brew cask install dotnet-sdk
    dotnet --info
  fi
}

function install-java(){
  if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
    java -version
  elif [ "$TRAVIS_OS_NAME" == "osx" ]; then
    brew cask install java
    java -version
  fi
}

eval "${MATRIX_EVAL}"

if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
  /usr/bin/x86_64-linux-gnu-ld --version
elif [ "$TRAVIS_OS_NAME" == "osx" ]; then
  brew update
fi

install-cmake
if [[ "$LANGUAGE" != "cpp" ]]; then
  install-swig
fi

if [[ "$LANGUAGE" == "python" ]]; then
  install-python
elif [[ "$LANGUAGE" == "dotnet" ]]; then
  install-dotnet-sdk
elif [[ "$LANGUAGE" == "java" ]]; then
  install-java
fi
# vim: set tw=0 ts=2 sw=2 expandtab:
