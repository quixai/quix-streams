add_executable(FooBarApp "")
target_sources(FooBarApp
  PRIVATE
    src/main.cpp)
target_compile_features(FooBarApp PUBLIC cxx_std_11)
set_target_properties(FooBarApp PROPERTIES
  VERSION ${PROJECT_VERSION})
# note: macOS is APPLE and also UNIX !
if(APPLE)
  set_target_properties(FooBarApp PROPERTIES
    INSTALL_RPATH "@loader_path/../${CMAKE_INSTALL_LIBDIR}")
elseif(UNIX AND NOT APPLE)
  set_target_properties(FooBarApp PROPERTIES
    INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
endif()
target_link_libraries(FooBarApp PRIVATE ${PROJECT_NAME}::FooBar)
add_executable(${PROJECT_NAME}::FooBarApp ALIAS FooBarApp)

if(BUILD_TESTING)
  add_test(NAME cxx_FooBarApp_test COMMAND FooBarApp)
endif()

# Install
include(GNUInstallDirs)
install(TARGETS FooBarApp
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
