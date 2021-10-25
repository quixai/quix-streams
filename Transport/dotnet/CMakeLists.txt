set_property(SOURCE transport.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE transport.i PROPERTY COMPILE_OPTIONS
  -namespace ${CMAKE_SWIG_DOTNET}.Transport
  -dllimport mizux-cmakeswig-native)
swig_add_library(dotnet_Transport
  TYPE OBJECT
  LANGUAGE csharp
  OUTPUT_DIR ${PROJECT_BINARY_DIR}/dotnet/${PROJECT_NAME}/Transport
  SOURCES	transport.i)

set_target_properties(dotnet_Transport PROPERTIES
  SWIG_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/dotnet
  SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON
  POSITION_INDEPENDENT_CODE ON)
target_link_libraries(dotnet_Transport PRIVATE CMakeSwig::Transport)
