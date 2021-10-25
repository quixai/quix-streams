cmake_minimum_required(VERSION 3.5)
project(Sample VERSION 1.0.0 LANGUAGES CXX)

include(CTest)
find_package(CMakeSwig REQUIRED)

add_executable(sample main.cpp)
#target_compile_features(sample PUBLIC cxx_std_11)
set_target_properties(sample PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  VERSION ${PROJECT_VERSION})
target_link_libraries(sample PRIVATE CMakeSwig::FooBar)

if(BUILD_TESTING)
  add_test(NAME sample_UT COMMAND sample)
endif()

include(GNUInstallDirs)
install(TARGETS sample
  EXPORT SampleTargets
  DESTINATION ${CMAKE_INSTALL_BINDIR})
