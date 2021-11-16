if(NOT MY_PROJECT_PROTOBUF_VERSION)
    set(MY_PROJECT_PROTOBUF_VERSION 3.19.1)
endif()

option(MY_PROJECT_SHARED_LIBS "Build proto modules as shared libraries" OFF)

message(STATUS "Google Protocol Buffers version: ${MY_PROJECT_PROTOBUF_VERSION}")

if(MY_PROJECT_SHARED_LIBS)
    message(STATUS "Building proto modules as SHARED libraries")
else()
    message(STATUS "Building proto modules as STATIC libraries")
endif()

include(ExternalProject)

# Downloads and builds a given protobuf version, generating a protobuf target
# with the include dir and binaries imported
macro(configure_protobuf VERSION)
    set(protobufPackage "protobuf-cpp-${VERSION}.zip")
    set(protobufPackageUrl "https://github.com/google/protobuf/releases/download/v${VERSION}/${protobufPackage}")
    set(protobufExternal protobuf-external)

    IF (WIN32) 
        ExternalProject_Add(${protobufExternal}
            URL "${protobufPackageUrl}"
            CONFIGURE_COMMAND 
                ${CMAKE_COMMAND} -Dprotobuf_BUILD_EXAMPLES=OFF -DCMAKE_BUILD_TYPE=Release <SOURCE_DIR>/cmake
            BUILD_COMMAND ${MAKE}
            INSTALL_COMMAND ""
        )
    ELSE()
        ExternalProject_Add(${protobufExternal}
            URL "${protobufPackageUrl}"
            CONFIGURE_COMMAND
                <SOURCE_DIR>/configure --disable-build-tests --prefix=<INSTALL_DIR>
            BUILD_COMMAND ${MAKE}
        )
    ENDIF()


    IF (WIN32) 

        set(protobufBinaryDir  "${CMAKE_BINARY_DIR}/lib/protobuf/${protobufExternal}-prefix/src/protobuf-external-build/Release")        
        set(protobufLibraryDir "${CMAKE_BINARY_DIR}/lib/protobuf/${protobufExternal}-prefix/src/protobuf-external-build/Release")
        set(protobufCompiler   "${protobufBinaryDir}/protoc.exe")
 
        set(protobufLibrary    "${protobufLibraryDir}/libprotobuf-lite.lib")

        set(protobufIncludeDir "${CMAKE_BINARY_DIR}/lib/protobuf/${protobufExternal}-prefix/src/protobuf-external/src")
    ELSE()

        set(protobufBinaryDir  "${CMAKE_BINARY_DIR}/lib/protobuf/${protobufExternal}-prefix/bin")
        set(protobufLibraryDir "${CMAKE_BINARY_DIR}/lib/protobuf/${protobufExternal}-prefix/lib")
        set(protobufCompiler   "${protobufBinaryDir}/protoc")
 
        set(protobufLibrary    "${protobufLibraryDir}/libprotobuf.a")

        set(protobufIncludeDir "${CMAKE_BINARY_DIR}/lib/protobuf/protobuf-external-prefix/include")
    ENDIF()

    if(MY_PROJECT_SHARED_LIBS)
        set(libType SHARED)
    else()
        set(libType STATIC)
    endif()



    add_library(protobuf_imported ${libType} IMPORTED)
    add_dependencies(protobuf_imported ${protobufExternal})

    set_target_properties(protobuf_imported PROPERTIES
        IMPORTED_LOCATION "${protobufLibrary}"
    )

    add_library(protobuf INTERFACE)
    target_include_directories(protobuf INTERFACE "${protobufIncludeDir}")
    target_link_libraries(protobuf INTERFACE protobuf_imported)

endmacro()

function(protobuf_generate_cpp LIBRARY)
    set(PROTOS ${ARGN})

    if(MY_PROJECT_SHARED_LIBS)
        set(libType SHARED)
    else()
        set(libType)
    endif()

    add_library(${LIBRARY} ${libType} ${SOURCES} ${HEADERS})
    target_compile_options(${LIBRARY} PRIVATE -std=c++11)
    target_link_libraries(${LIBRARY} PUBLIC protobuf)
    target_include_directories(${LIBRARY} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
endfunction()



configure_protobuf(${MY_PROJECT_PROTOBUF_VERSION})
