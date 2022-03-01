set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp *.pb.cc)

set(SOURCES ${SOURCES})

include_directories(${protobufIncludeDir})

IF (WIN32) 
    set(PROTOBUF_USE_DLLS OFF)
    set(protobufIncludeDir "${CMAKE_BINARY_DIR}/lib/protobuf/protobuf-external-prefix/src/protobuf-external/src")
ELSE()
    set(protobufIncludeDir "${CMAKE_BINARY_DIR}/lib/protobuf/protobuf-external-prefix/include")
ENDIF()


message(STATUS "PROTOBUF INCLUDE DIR ${protobufIncludeDir}")
message(STATUS ${protobufIncludeDir})


# add_executable(${BINARY}_run ${SOURCES})
# target_include_directories(${BINARY}_run INTERFACE "${protobufIncludeDir}")
# target_link_libraries(${BINARY}_run ${Protobuf_LIBRARIES}) 




add_library(${BINARY}_lib STATIC ${SOURCES})

# set(LINK_LIBRARIES "rdkafka++ ${Protobuf_LIBRARIES} protobuf")
# IF (WIN32) 
#     set(LINK_LIBRARIES "${LINK_LIBRARIES} pthread")
# ENDIF()

IF (WIN32) 
    target_link_libraries(${BINARY}_lib PUBLIC rdkafka++ ${Protobuf_LIBRARIES} protobuf) 
    # target_link_libraries(${BINARY}_lib PUBLIC ws2_32.lib secur32.lib crypt32.lib legacy_stdio_definitions.lib User32.lib rdkafka++ ${Protobuf_LIBRARIES} protobuf) 
ELSE()
#  -lws2_32 -lsecur32 -lcrypt32
    target_link_libraries(${BINARY}_lib PUBLIC rdkafka++ ${Protobuf_LIBRARIES} pthread protobuf) 
ENDIF()
    # target_link_libraries(${BINARY}_lib PUBLIC rdkafka++ ${Protobuf_LIBRARIES} pthread protobuf) 
# ENDIF()
target_include_directories(${BINARY}_lib INTERFACE "${protobufIncludeDir}")

