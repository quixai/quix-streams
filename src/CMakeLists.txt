set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp *.pb.cc)

set(SOURCES ${SOURCES})

include(FindProtobuf)
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})


add_executable(${BINARY}_run ${SOURCES})
target_link_libraries(${BINARY}_run ${Protobuf_LIBRARIES}) 




add_library(${BINARY}_lib STATIC ${SOURCES})
target_link_libraries(${BINARY}_lib ${Protobuf_LIBRARIES}) 

