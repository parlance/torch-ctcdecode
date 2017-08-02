include(ExternalProject)

set(KENLM_MAX_ORDER 6 CACHE STRING "Max ngram order supported.")

ExternalProject_Add(kenlm
  URL http://kheafield.com/code/kenlm.tar.gz
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/patches/kenlm/CMakeLists.txt
    <SOURCE_DIR>/CMakeLists.txt
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    -DCMAKE_CXX_STANDARD=11
  CMAKE_CACHE_ARGS
    -DCXX_STANDARD_REQUIRED:BOOL=ON
)

ExternalProject_Get_Property(kenlm SOURCE_DIR BINARY_DIR)
add_library(third_party::kenlm SHARED IMPORTED)
set_target_properties(third_party::kenlm PROPERTIES
  IMPORTED_LOCATION ${BINARY_DIR}/libkenlm${CMAKE_SHARED_LIBRARY_SUFFIX}
  INTERFACE_INCLUDE_DIRECTORIES ${SOURCE_DIR}
  INTERFACE_COMPILE_DEFINITIONS KENLM_MAX_ORDER=${KENLM_MAX_ORDER}
)
add_dependencies(third_party::kenlm kenlm)
