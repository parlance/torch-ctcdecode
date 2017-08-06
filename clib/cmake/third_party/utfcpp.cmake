include(ExternalProject)

ExternalProject_Add(utfcpp
  URL https://github.com/nemtrif/utfcpp/archive/v2.3.5.tar.gz

  PATCH_COMMAND   ${CMAKE_COMMAND} -E copy_if_different
                  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/third_party/utfcpp.CMakeLists.txt
                  <SOURCE_DIR>/CMakeLists.txt

  CMAKE_ARGS  -DCMAKE_BUILD_TYPE=Release
              -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
              -DCMAKE_VERBOSE_MAKEFILE=NO

  INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
                  <SOURCE_DIR>/source
                  <INSTALL_DIR>/include/utf8
)

add_library(utf8 INTERFACE)
add_dependencies(utf8 utfcpp)
ExternalProject_Get_Property(utfcpp SOURCE_DIR)
target_include_directories(utf8 INTERFACE
  $<BUILD_INTERFACE:${SOURCE_DIR}/source>
  $<INSTALL_INTERFACE:include/utf8>
)

add_library(third_party::utf8 ALIAS utf8)
