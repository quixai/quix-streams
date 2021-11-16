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
        # ExternalProject_Add(${protobufExternal}
        #     PREFIX cmake
        #     GIT_REPOSITORY "https://github.com/google/protobuf.git"
        #     GIT_TAG "v2.6.1"
        #     CONFIGURE_COMMAND "devenv ${CMAKE_CURRENT_BINARY_DIR}\\protobuf\\src\\vsprojects\\protobuf.sln /Upgrade"
        #     BUILD_COMMAND "msbuild ${CMAKE_CURRENT_BINARY_DIR}\\protobuf\\src\\vsprojects\\protobuf.sln"
        #     INSTALL_COMMAND " "
        #     TEST_COMMAND " "
        # )

        ExternalProject_Add(${protobufExternal}
            URL "${protobufPackageUrl}"
            # SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake"
            # SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake" --build . --config Release
            # CONFIGURE_COMMAND
            #     ${CMAKE_COMMAND} --build cmake --config Release -DBUILD_TESTING=OFF ..
            # CONFIGURE_COMMAND
            #     cd cmake && ${CMAKE_COMMAND} -DBUILD_TESTING=OFF .
            # CONFIGURE_COMMAND
            #     ${CMAKE_COMMAND} --build cmake -DBUILD_TESTING=OFF
            # CONFIGURE_COMMAND 
            #     ${CMAKE_COMMAND} -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_BUILD_SHARED_LIBS=ON -DBUILD_SHARED_LIBS=ON -DPROTOBUF_USE_DLLS=ON  -Dprotobuf_BUILD_EXAMPLES=OFF -DMSVC_STATIC_RUNTIME=ON <SOURCE_DIR>/cmake
            CONFIGURE_COMMAND 
                ${CMAKE_COMMAND} -Dprotobuf_BUILD_EXAMPLES=OFF -Dprotobuf_BUILD_SHARED_LIBS=ON -DCMAKE_CXX_FLAGS_RELEASE="/Ob0 /Og /Oi /Os /O /Ot /Oy" -Dprotobuf_MSVC_STATIC_RUNTIME=ON -DCMAKE_BUILD_TYPE=Debug <SOURCE_DIR>/cmake
            # BUILD_COMMAND cd cmake && ${MAKE}
            BUILD_COMMAND ${MAKE}
            INSTALL_COMMAND ""
            # BUILD_COMMAND
            #     ${CMAKE_COMMAND} --build <SOURCE_DIR>/cmake
        )
    ELSE()
        ExternalProject_Add(${protobufExternal}
            URL "${protobufPackageUrl}"
            CONFIGURE_COMMAND
                <SOURCE_DIR>/configure -DDEBUG --disable-build-tests --prefix=<INSTALL_DIR>
            BUILD_COMMAND ${MAKE}
        )
    ENDIF()


    IF (WIN32) 

#        set(protobufBinaryDir  "${CMAKE_BINARY_DIR}/lib/protobuf/${protobufExternal}-prefix/src/protobuf-external-build/Release")
        set(protobufBinaryDir "C:\\Users\\Ales-QUIX\\Downloads\\libprotobuf-static-3.19.1-h7755175_0.tar\\Library\\lib")
        set(protobufLibraryDir "C:\\Users\\Ales-QUIX\\Downloads\\libprotobuf-static-3.19.1-h7755175_0.tar\\Library\\lib")
#        set(protobufBinaryDir  "C:\\Users\\Ales-QUIX\\Downloads\\protobuf-cpp-3.19.1\ (1)\\protobuf-3.19.1\\cmake\\build\\release\\Release")
#        set(protobufLibraryDir "C:\\Users\\Ales-QUIX\\Downloads\\protobuf-cpp-3.19.1\ (1)\\protobuf-3.19.1\\cmake\\build\\release\\Release")
        
 #       set(protobufLibraryDir "${CMAKE_BINARY_DIR}/lib/protobuf/${protobufExternal}-prefix/src/protobuf-external-build/Release")
        set(protobufCompiler   "${protobufBinaryDir}/protoc.exe")
 
        if(MY_PROJECT_SHARED_LIBS)
            set(protobufLibrary "${protobufLibraryDir}/libprotobuf.so")
        else()
            set(protobufLibrary "${protobufLibraryDir}/libprotobuf-lite.lib")
        endif()
        set(protobufLibraryLite "${protobufLibraryDir}/libprotobuf-lite.lib")
        set(protobufLibraryProtoc "${protobufLibraryDir}/libprotoc.lib")

        set(protobufIncludeDir "${CMAKE_BINARY_DIR}/lib/protobuf/${protobufExternal}-prefix/src/protobuf-external/src")
    ELSE()

        set(protobufBinaryDir  "${CMAKE_BINARY_DIR}/lib/protobuf/${protobufExternal}-prefix/bin")
        set(protobufLibraryDir "${CMAKE_BINARY_DIR}/lib/protobuf/${protobufExternal}-prefix/lib")
        set(protobufCompiler   "${protobufBinaryDir}/protoc")
 
        if(MY_PROJECT_SHARED_LIBS)
            set(protobufLibrary "${protobufLibraryDir}/libprotobuf.so")
        else()
            set(protobufLibrary "${protobufLibraryDir}/libprotobuf.a")
        endif()

        set(protobufIncludeDir "${CMAKE_BINARY_DIR}/lib/protobuf/protobuf-external-prefix/include")
    ENDIF()

    # set(protobufIncludeDir "${CMAKE_BINARY_DIR}/lib/protobuf/${protobufExternal}-prefix/include")

    if(MY_PROJECT_SHARED_LIBS)
        set(libType SHARED)
    else()
        set(libType STATIC)
    endif()

#    add_library(protobuf ${libType} IMPORTED)
#    add_dependencies(protobuf ${protobufExternal})
#    target_include_directories(protobuf INTERFACE "${protobufIncludeDir}")
#    set_target_properties(protobuf PROPERTIES
#        IMPORTED_LOCATION "${protobufLibrary}"
#    )


    add_library(protobuf_imported ${libType} IMPORTED)
    add_dependencies(protobuf_imported ${protobufExternal})

    IF (WIN32) 
        set_target_properties(protobuf_imported PROPERTIES
            IMPORTED_LOCATION "${protobufLibrary}"
        )
    ELSE()
        set_target_properties(protobuf_imported PROPERTIES
            IMPORTED_LOCATION "${protobufLibrary}"
        )
    ENDIF()

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



# protobuf_generate_cpp(MyProtobuf)


configure_protobuf(${MY_PROJECT_PROTOBUF_VERSION})

message(STATUS "2 PROTOBUF INCLUDE DIR ${protobufIncludeDir}")
message(STATUS ${protobufIncludeDir})



# set(LIBRARY myprotobuf)

# if(MY_PROJECT_SHARED_LIBS)
#     set(libType SHARED)
# else()
#     set(libType)
# endif()

# add_library(${LIBRARY} ${libType})
# target_compile_options(${LIBRARY} PRIVATE -std=c++11)
# target_link_libraries(${LIBRARY} PUBLIC protobuf)
# target_include_directories(protobuf PUBLIC ${CMAKE_CURRENT_BINARY_DIR})