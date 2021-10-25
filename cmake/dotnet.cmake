if(NOT BUILD_DOTNET)
  return()
endif()

# Use latest UseSWIG module
cmake_minimum_required(VERSION 3.14)

if(NOT TARGET CMakeSwig::FooBar)
  message(FATAL_ERROR ".Net: missing FooBar TARGET")
endif()

# Will need swig
set(CMAKE_SWIG_FLAGS)
find_package(SWIG REQUIRED)
include(UseSWIG)

if(UNIX AND NOT APPLE)
  list(APPEND CMAKE_SWIG_FLAGS "-DSWIGWORDSIZE64")
endif()

# Find dotnet
find_program(DOTNET_EXECUTABLE dotnet)
if(NOT DOTNET_EXECUTABLE)
  message(FATAL_ERROR "Check for dotnet Program: not found")
else()
  message(STATUS "Found dotnet Program: ${DOTNET_EXECUTABLE}")
endif()

# Create the native library
add_library(mizux-cmakeswig-native SHARED "")
set_target_properties(mizux-cmakeswig-native PROPERTIES
  PREFIX ""
  POSITION_INDEPENDENT_CODE ON)
# note: macOS is APPLE and also UNIX !
if(APPLE)
  set_target_properties(mizux-cmakeswig-native PROPERTIES INSTALL_RPATH "@loader_path")
elseif(UNIX)
  set_target_properties(mizux-cmakeswig-native PROPERTIES INSTALL_RPATH "$ORIGIN")
endif()

# Swig wrap all libraries
set(CMAKE_SWIG_DOTNET Mizux.CMakeSwig)
foreach(SUBPROJECT IN ITEMS Transport FooBar)
  add_subdirectory(${SUBPROJECT}/dotnet)
  target_link_libraries(mizux-cmakeswig-native PRIVATE dotnet_${SUBPROJECT})
endforeach()

############################
##  .Net Runtime Package  ##
############################
file(COPY
  ${PROJECT_SOURCE_DIR}/dotnet/logo.png
  DESTINATION dotnet)
file(COPY
  ${PROJECT_SOURCE_DIR}/dotnet/Directory.Build.props
  DESTINATION dotnet)

if(APPLE)
  set(RUNTIME_IDENTIFIER osx-x64)
elseif(UNIX)
  set(RUNTIME_IDENTIFIER linux-x64)
elseif(WIN32)
  set(RUNTIME_IDENTIFIER win-x64)
else()
  message(FATAL_ERROR "Unsupported system !")
endif()
set(CMAKE_SWIG_DOTNET_NATIVE ${CMAKE_SWIG_DOTNET}.runtime.${RUNTIME_IDENTIFIER})

file(GENERATE OUTPUT dotnet/$<CONFIG>/replace_runtime.cmake
  CONTENT
  "FILE(READ ${PROJECT_SOURCE_DIR}/dotnet/${CMAKE_SWIG_DOTNET}.runtime.csproj.in input)
  STRING(REPLACE \"@PROJECT_VERSION@\" \"${PROJECT_VERSION}\" input \"\${input}\")
  STRING(REPLACE \"@RUNTIME_IDENTIFIER@\" \"${RUNTIME_IDENTIFIER}\" input \"\${input}\")
  STRING(REPLACE \"@CMAKE_SWIG_DOTNET@\" \"${CMAKE_SWIG_DOTNET}\" input \"\${input}\")
  STRING(REPLACE \"@CMAKE_SWIG_DOTNET_NATIVE@\" \"${CMAKE_SWIG_DOTNET_NATIVE}\" input \"\${input}\")
  STRING(REPLACE \"@Transport@\" \"$<TARGET_FILE:Transport>\" input \"\${input}\")
  STRING(REPLACE \"@FooBar@\" \"$<TARGET_FILE:FooBar>\" input \"\${input}\")
  STRING(REPLACE \"@native@\" \"$<TARGET_FILE:mizux-cmakeswig-native>\" input \"\${input}\")
  FILE(WRITE ${CMAKE_SWIG_DOTNET_NATIVE}/${CMAKE_SWIG_DOTNET_NATIVE}.csproj \"\${input}\")"
  )

add_custom_command(
  OUTPUT dotnet/${CMAKE_SWIG_DOTNET_NATIVE}/${CMAKE_SWIG_DOTNET_NATIVE}.csproj
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SWIG_DOTNET_NATIVE}
  COMMAND ${CMAKE_COMMAND} -P ./$<CONFIG>/replace_runtime.cmake
  WORKING_DIRECTORY dotnet
  )

add_custom_target(dotnet_native
  DEPENDS
  mizux-cmakeswig-native
  dotnet/${CMAKE_SWIG_DOTNET_NATIVE}/${CMAKE_SWIG_DOTNET_NATIVE}.csproj
  COMMAND ${CMAKE_COMMAND} -E make_directory packages
  COMMAND ${DOTNET_EXECUTABLE} build -c Release /p:Platform=x64 ${CMAKE_SWIG_DOTNET_NATIVE}/${CMAKE_SWIG_DOTNET_NATIVE}.csproj
  COMMAND ${DOTNET_EXECUTABLE} pack -c Release ${CMAKE_SWIG_DOTNET_NATIVE}/${CMAKE_SWIG_DOTNET_NATIVE}.csproj
  WORKING_DIRECTORY dotnet
  )

# Pure .Net Package
file(GENERATE OUTPUT dotnet/$<CONFIG>/replace.cmake
  CONTENT
  "FILE(READ ${PROJECT_SOURCE_DIR}/dotnet/${CMAKE_SWIG_DOTNET}.csproj.in input)
  STRING(REPLACE \"@PROJECT_VERSION@\" \"${PROJECT_VERSION}\" input \"\${input}\")
  STRING(REPLACE \"@CMAKE_SWIG_DOTNET@\" \"${CMAKE_SWIG_DOTNET}\" input \"\${input}\")
  STRING(REPLACE \"@DOTNET_PACKAGES_DIR@\" \"${PROJECT_BINARY_DIR}/dotnet/packages\" input \"\${input}\")
  FILE(WRITE ${CMAKE_SWIG_DOTNET}/${CMAKE_SWIG_DOTNET}.csproj \"\${input}\")"
  )

add_custom_command(
  OUTPUT dotnet/${CMAKE_SWIG_DOTNET}/${CMAKE_SWIG_DOTNET}.csproj
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SWIG_DOTNET}
  COMMAND ${CMAKE_COMMAND} -P ./$<CONFIG>/replace.cmake
  WORKING_DIRECTORY dotnet
  )

add_custom_target(dotnet_package ALL
  DEPENDS
  dotnet_native
  dotnet/${CMAKE_SWIG_DOTNET}/${CMAKE_SWIG_DOTNET}.csproj
  COMMAND ${DOTNET_EXECUTABLE} build -c Release /p:Platform=x64 ${CMAKE_SWIG_DOTNET}/${CMAKE_SWIG_DOTNET}.csproj
  COMMAND ${DOTNET_EXECUTABLE} pack -c Release ${CMAKE_SWIG_DOTNET}/${CMAKE_SWIG_DOTNET}.csproj
  WORKING_DIRECTORY dotnet
  )

