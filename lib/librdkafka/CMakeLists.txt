
IF (WIN32) 

    # # add_library(rdkafka_imported STATIC IMPORTED)
    # # # add_dependencies(rdkafka_imported ${protobufExternal})
    # # # set_target_properties(rdkafka_imported PROPERTIES
    # # #     IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/win/rdkafka++.dll"
    # # # )
    # # # set_target_properties(rdkafka_imported PROPERTIES
    # # #     IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/win/rdkafka++.lib"
    # # # )

    # # set_target_properties(rdkafka_imported PROPERTIES
    # #     IMPORTED_IMPLIB "./win/rdkafka++.lib"
    # # )

    # # add_library(rdkafka_imported SHARED IMPORTED)
    # # SET_TARGET_PROPERTIES(rdkafka_imported PROPERTIES
    # #     # IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/win/rdkafka++.lib"
    # #     IMPORTED_LINK_INTERFACE_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/win/rdkafka++.lib"
    # # )



    # # add_library(rdkafka++ INTERFACE)
    # # target_include_directories(rdkafka++ INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/repo/src")
    # # target_include_directories(rdkafka++ INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/repo/src-cpp")
    # # target_link_libraries(rdkafka++ INTERFACE rdkafka_imported)

    # add_library(rdkafka2++ STATIC IMPORTED)
    # # target_include_directories(rdkafka2++ INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/repo/src")
    # # target_include_directories(rdkafka2++ INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/repo/src-cpp")
    # set_target_properties(rdkafka2++ PROPERTIES
    #                 INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/repo/src-cpp"
    #               )
    # set_property(TARGET rdkafka2++ PROPERTY
    #                 IMPORTED_IMPLIB  "${CMAKE_CURRENT_SOURCE_DIR}\\win\\rdkafka-abcd++.lib")
    # set_property(TARGET rdkafka2++ PROPERTY
    #                 IMPORTED_LOCATION  "${CMAKE_CURRENT_SOURCE_DIR}\\win\\rdkafka-abcd++.dll")
    # add_library(rdkafka++ STATIC "${CMAKE_CURRENT_SOURCE_DIR}/win/rdkafka++.lib")
    # set_target_properties(rdkafka++ PROPERTIES LINKER_LANGUAGE CXX)
    # target_include_directories(rdkafka++ PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/repo/src")
    # target_include_directories(rdkafka++ PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/repo/src-cpp")

    # find_library(FOO_LIBRARY NAMES rdkafka++)
    # add_library(rdkafka++ UNKNOWN IMPORTED)
    # set_property(TARGET rdkafka++ PROPERTY IMPORTED_LOCATION "${FOO_LIBRARY}")
    # add_executable(myexe src1.c src2.c)
    # target_link_libraries(myexe foo)
    

    set(RDKAFKA_BUILD_EXAMPLES OFF)
    set(RDKAFKA_BUILD_TESTS OFF)
    # set(RDKAFKA_BUILD_STATIC ON CACHE BOOL "")
    # set(RDKAFKA_BUILD_STATIC ON CACHE BOOL "")
    set(RDKAFKA_BUILD_STATIC ON)

    add_subdirectory(repo)

    # set_property(TARGET rdkafka PROPERTY
    #     MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    # set_property(TARGET rdkafka++ PROPERTY
    #     MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

ELSE()
    set(RDKAFKA_BUILD_EXAMPLES OFF)
    set(RDKAFKA_BUILD_TESTS OFF)
    set(RDKAFKA_BUILD_STATIC ON CACHE BOOL "")

    add_subdirectory(repo)
ENDIF()
