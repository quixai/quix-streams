set(NAME FooBar_test)

file(GLOB _SRCS "src/*.[hc]pp")

add_executable(${NAME}
  src/main.cpp
  src/FooBar_UT.cpp
  )
# note: macOS is APPLE and also UNIX !
if(APPLE)
  set_target_properties(${NAME} PROPERTIES
    INSTALL_RPATH "@loader_path/../${CMAKE_INSTALL_LIBDIR}")
elseif(UNIX AND NOT APPLE)
  set_target_properties(${NAME} PROPERTIES
    INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
endif()
target_link_libraries(${NAME} PRIVATE Catch2 ${PROJECT_NAME}::FooBar)
add_test(NAME cxx_${NAME} COMMAND ${NAME})
