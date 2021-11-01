# Quix streaming SDK module

[![Build Status](https://dev.azure.com/quix-analytics/Quix%20streams/_apis/build/status/quixai.quix-streams?branchName=main)](https://dev.azure.com/quix-analytics/Quix%20streams/_build/latest?definitionId=51&branchName=main)

## Overview

This module is the c++ implementation of the next gen Quix SDK library for [Quix.ai](https://quix.ai/).

## Linux development

### Install prerequisities

```sh
    sudo apt-get install build-essential cmake
```

### How to build?

run this command inside project root folder:

```sh
    mkdir build && cd build && cmake .. && make -j4
```

### How to test?

run this command inside project root folder:

```sh
     ./build/test/QuixStreaming_tst  
```

## Windows development


### Install prerequisities

We do recommend using the Visual Studio Community C++ toolchains. Please follow instructions on this page [https://docs.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=msvc-160](https://docs.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=msvc-160). 

### How to build?

run this command inside project root folder:

```cmd
    Mkdir build && cd build && ( cmake .. && cmake --build . --config Release & cd .. )
```

### How to test?

run this command inside project root folder:

```cmd
    build\test\Release\QuixStreaming_tst.exe
```


## Development

### IDE choice

The project is currently set up to work with Visual Studio Code IDE. It leverages usage of the C++ and CMake ( both from Microsoft ) extensions from the Visual Studio Extensions library.

Integrations with other IDEs are welcome to be accepted in PR !


### CI pipeline

We use Azure DevOps CI pipeline for running our unit tests.

Please folow on this link [https://dev.azure.com/quix-analytics/Quix%20streams/_build?definitionId=51](https://dev.azure.com/quix-analytics/Quix%20streams/_build?definitionId=51).


### Naming conventions

Our naming conventions are inspired by 

[https://manual.gromacs.org/documentation/5.1-current/dev-manual/naming.html](https://manual.gromacs.org/documentation/5.1-current/dev-manual/naming.html).

