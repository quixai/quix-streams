| Linux | macOS | Windows |
|-------|-------|---------|
| [![Status][cpp_linux_svg]][cpp_linux_link] | [![Status][cpp_osx_svg]][cpp_osx_link] | [![Status][cpp_win_svg]][cpp_win_link] |


[cpp_linux_svg]: https://github.com/Mizux/cmake-swig/workflows/C++%20Linux%20CI/badge.svg?branch=master
[cpp_linux_link]: https://github.com/Mizux/cmake-swig/actions?query=workflow%3A"C%2B%2B+Linux+CI"
[cpp_osx_svg]: https://github.com/Mizux/cmake-swig/workflows/C++%20MacOS%20CI/badge.svg?branch=master
[cpp_osx_link]: https://github.com/Mizux/cmake-swig/actions?query=workflow%3A"C%2B%2B+MacOS+CI"
[cpp_win_svg]: https://github.com/Mizux/cmake-swig/workflows/C++%20Windows%20CI/badge.svg?branch=master
[cpp_win_link]: https://github.com/Mizux/cmake-swig/actions?query=workflow%3A"C%2B%2B+Windows+CI"


# C++ Status
* [x] GNU/Linux
* [x] MacOS
* [x] Windows

# Introduction
This is a complete example of how to create a Modern [CMake](https://cmake.org/) C++ Project.

This project should run on GNU/Linux, MacOS and Windows.

# Dependencies
To complexify a little, the CMake project is composed of three libraries (`Foo`, `Bar` and `FooBar`)
and one executable `FooBarApp` with the following dependencies:
```sh
CMakeSwig::Foo:
CMakeSwig::Bar:
CMakeSwig::FooBar: PUBLIC CMakeSwig::Foo PRIVATE CMakeSwig::Bar
CMakeSwig::FooBarApp: PRIVATE CMakeSwig::FooBar
```

# C++ Project Build
To build the C++ project, as usual:
```shell
cmake -S. -Bbuild
cmake --build build --target all -v
```

## Managing RPATH
Since we want to use the [CMAKE_BINARY_DIR](https://cmake.org/cmake/help/latest/variable/CMAKE_BINARY_DIR.html) to generate the wrapper package (e.g. python wheel package) as well as be able to test from the build directory.
We need to enable:
```cmake
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
```
And have a finely tailored rpath for each library.

For `Foo` and `Bar` which depend on nothing:
```cmake
set(CMAKE_INSTALL_RPATH "$ORIGIN")
```

For `FooBar` which depend on `Foo` and `Bar`:
```cmake
set(CMAKE_INSTALL_RPATH "$ORIGIN:$ORIGIN/../Foo:$ORIGIN/../Bar")
```

For `FooBarApp` which depend on `FooBar`:
```cmake
include(GNUInstallDirs)
...
set(CMAKE_INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}:$ORIGIN/../FooBar")
```
