set(RDKAFKA_BUILD_STATIC ON CACHE BOOL "")

ExternalProject_Add(${protobufExternal}
    GIT_REPOSITORY https://github.com/edenhill/librdkafka
    GIT_TAG           master
    URL "${protobufPackageUrl}"
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -H. -B_cmake_build
    BUILD_COMMAND ${CMAKE_COMMAND} --build _cmake_build -DRDKAFKA_BUILD_STATIC=1
    INSTALL_COMMAND ""
)
