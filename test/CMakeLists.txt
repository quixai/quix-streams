set(BINARY ${CMAKE_PROJECT_NAME}_tst)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${TEST_SOURCES})

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

IF (WIN32) 
    set(protobufIncludeDir "${CMAKE_BINARY_DIR}/lib/protobuf/protobuf-external-prefix/src/protobuf-external/src")
ELSE()
    set(protobufIncludeDir "${CMAKE_BINARY_DIR}/lib/protobuf/protobuf-external-prefix/include")
ENDIF()

################################
# GTest
################################


### Googletest 1.8.0 has deprecation warning in Windows - see https://github.com/google/googletest/issues/1111
IF (WIN32) 
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} /D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING")
ENDIF()

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.8.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)



################################
# Tests
################################

add_executable(${BINARY} ${TEST_SOURCES})

add_test(NAME ${BINARY} COMMAND ${BINARY})


target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest gmock protobuf rdkafka++)
target_include_directories(${BINARY} INTERFACE "${protobufIncludeDir}")
