set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp *.pb.cc)

set(SOURCES ${SOURCES})

include_directories(${protobufIncludeDir})

set(protobufIncludeDir "${CMAKE_BINARY_DIR}/lib/protobuf/protobuf-external-prefix/include")


message(STATUS "PROTOBUF INCLUDE DIR ${protobufIncludeDir}")
message(STATUS ${protobufIncludeDir})


# add_executable(${BINARY}_run ${SOURCES})
# target_include_directories(${BINARY}_run INTERFACE "${protobufIncludeDir}")
# target_link_libraries(${BINARY}_run ${Protobuf_LIBRARIES}) 




add_library(${BINARY}_lib STATIC ${SOURCES})
target_link_libraries(${BINARY}_lib ${Protobuf_LIBRARIES} protobuf) 
target_include_directories(${BINARY}_lib INTERFACE "${protobufIncludeDir}")

