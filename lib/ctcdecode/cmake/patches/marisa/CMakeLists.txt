cmake_minimum_required(VERSION 3.1.3 FATAL_ERROR)
project(Marisa LANGUAGES CXX)

include(GNUInstallDirs)

set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
set(LIB_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
set(CMAKEPACKAGE_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/cmake/marisa")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

include(CMakePackageConfigHelpers)

add_library(marisa
  "lib/marisa/agent.cc"
  "lib/marisa/keyset.cc"
  "lib/marisa/trie.cc"
  "lib/marisa/grimoire/io/mapper.cc"
  "lib/marisa/grimoire/io/reader.cc"
  "lib/marisa/grimoire/io/writer.cc"
  "lib/marisa/grimoire/trie/louds-trie.cc"
  "lib/marisa/grimoire/trie/tail.cc"
  "lib/marisa/grimoire/vector/bit-vector.cc"
)

target_include_directories(marisa PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>
  PRIVATE lib
)

set_target_properties(marisa PROPERTIES EXPORT_NAME Marisa)

install(TARGETS marisa
  EXPORT MarisaTargets
  ARCHIVE DESTINATION   ${LIB_INSTALL_DIR}
  LIBRARY DESTINATION   ${LIB_INSTALL_DIR}
  INCLUDES DESTINATION  ${INCLUDE_INSTALL_DIR}
)
install(DIRECTORY include/ DESTINATION ${INCLUDE_INSTALL_DIR}
  FILES_MATCHING PATTERN "*.h"
)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/MarisaConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/MarisaConfig.cmake
  INSTALL_DESTINATION ${CMAKEPACKAGE_INSTALL_DIR}
)

export(TARGETS marisa NAMESPACE Marisa:: FILE MarisaTargets.cmake)

export(PACKAGE Marisa)

install(EXPORT MarisaTargets DESTINATION ${CMAKEPACKAGE_INSTALL_DIR}
  NAMESPACE Marisa::
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/MarisaConfig.cmake
  DESTINATION ${CMAKEPACKAGE_INSTALL_DIR}
)
