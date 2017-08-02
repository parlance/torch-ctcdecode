ExternalProject_Add(utfcpp
  URL https://github.com/nemtrif/utfcpp/archive/v2.3.5.tar.gz
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    -DCMAKE_VERBOSE_MAKEFILE=OFF
    -DCMAKE_BUILD_TYPE=Release
  INSTALL_COMMAND
    ${CMAKE_COMMAND} -E copy_directory
    <SOURCE_DIR>/source
    <INSTALL_DIR>/include
)
ExternalProject_Get_Property(utfcpp SOURCE_DIR INSTALL_DIR)
add_library(utf8 INTERFACE)
target_include_directories(utf8 INTERFACE
  $<BUILD_INTERFACE:${SOURCE_DIR}/source>
  $<INSTALL_INTERFACE:${INSTALL_DIR}/include>
)
add_dependencies(utf8 utfcpp)
add_library(third_party::utf8 ALIAS utf8)
