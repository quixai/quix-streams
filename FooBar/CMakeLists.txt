add_library(FooBar "")

target_sources(FooBar
  PRIVATE
    include/foobar/FooBar.hpp
    src/FooBar.cpp)
target_include_directories(FooBar
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
target_compile_features(FooBar PUBLIC cxx_std_11)
set_target_properties(FooBar PROPERTIES
  VERSION ${PROJECT_VERSION}
  PUBLIC_HEADER include/foobar/FooBar.hpp)
# note: macOS is APPLE and also UNIX !
if(APPLE)
  set_target_properties(FooBar PROPERTIES
    INSTALL_RPATH "@loader_path")
elseif(UNIX)
  set_target_properties(FooBar PROPERTIES
    INSTALL_RPATH "$ORIGIN")
endif()
target_link_libraries(FooBar
  PUBLIC
  ${PROJECT_NAME}::Transport
  )
add_library(${PROJECT_NAME}::FooBar ALIAS FooBar)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()

# Install
include(GNUInstallDirs)
install(TARGETS FooBar
  EXPORT ${PROJECT_NAME}Targets
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/foobar
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  #RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
