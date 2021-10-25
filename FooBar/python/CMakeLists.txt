set_property(SOURCE foobar.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE foobar.i PROPERTY SWIG_MODULE_NAME pyFooBar)
swig_add_library(pyFooBar
  LANGUAGE python
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/python/${PYTHON_PROJECT}/FooBar
  SOURCES foobar.i)
set_property(TARGET pyFooBar PROPERTY SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON)
target_include_directories(pyFooBar
  PRIVATE
  ../include
  ${Python_INCLUDE_DIRS}
  )
# note: macOS is APPLE and also UNIX !
if(APPLE)
  set_target_properties(pyFooBar PROPERTIES
    SUFFIX ".so"
    INSTALL_RPATH "@loader_path;@loader_path/../../${PYTHON_PROJECT}/.libs"
    )
  set_property(TARGET pyFooBar APPEND PROPERTY
    LINK_FLAGS "-flat_namespace -undefined suppress"
    )
elseif(UNIX)
  set_target_properties(pyFooBar PROPERTIES
    INSTALL_RPATH "$ORIGIN:$ORIGIN/../../${PYTHON_PROJECT}/.libs"
    )
endif()
target_link_libraries(pyFooBar PRIVATE FooBar)

# Variable PYTHON_LIBRARIES can contains keyword `optimized`
# which won't be interpreted inside a generator expression.
# i.e. we can't use: $<$<PLATFORM_ID:Windows>:${PYTHON_LIBRARIES}>
# see: https://cmake.org/cmake/help/git-stage/command/target_link_libraries.html#command:target_link_libraries
if(MSVC)
  target_link_libraries(pyFooBar	PRIVATE	${Python_LIBRARIES})
endif()
