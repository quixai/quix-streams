set_property(SOURCE foobar.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE foobar.i PROPERTY COMPILE_OPTIONS
  -namespace ${CMAKE_SWIG_DOTNET}.FooBar
  -dllimport mizux-cmakeswig-native)
swig_add_library(dotnet_FooBar
  TYPE OBJECT
  LANGUAGE csharp
  OUTPUT_DIR ${PROJECT_BINARY_DIR}/dotnet/${PROJECT_NAME}/FooBar
  SOURCES	foobar.i)

set_target_properties(dotnet_FooBar PROPERTIES
  SWIG_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/dotnet
  SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON
  POSITION_INDEPENDENT_CODE ON)
target_link_libraries(dotnet_FooBar PRIVATE CMakeSwig::FooBar)
