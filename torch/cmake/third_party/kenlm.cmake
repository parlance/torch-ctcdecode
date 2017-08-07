include(ExternalProject)

set(KENLM_MAX_ORDER 6 CACHE STRING "Max ngram order supported.")

ExternalProject_Add(kenlm
  # URL http://kheafield.com/code/kenlm.tar.gz
  GIT_REPOSITORY  https://github.com/kpu/kenlm
  GIT_TAG         master

  PATCH_COMMAND   ${CMAKE_COMMAND} -E copy_if_different
                  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/third_party/kenlm.CMakeLists.txt
                  <SOURCE_DIR>/CMakeLists.txt

  CMAKE_ARGS      -DBUILD_SHARED_LIBS=YES
                  -DCMAKE_BUILD_TYPE=Release
                  -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
                  -DCMAKE_VERBOSE_MAKEFILE=NO
)

add_library(third_party::kenlm SHARED IMPORTED)
add_dependencies(third_party::kenlm kenlm)
ExternalProject_Get_Property(kenlm SOURCE_DIR BINARY_DIR)
set_target_properties(third_party::kenlm PROPERTIES
  IMPORTED_LOCATION ${BINARY_DIR}/libkenlm${CMAKE_SHARED_LIBRARY_SUFFIX}
  INTERFACE_INCLUDE_DIRECTORIES ${SOURCE_DIR}
  INTERFACE_COMPILE_DEFINITIONS KENLM_MAX_ORDER=${KENLM_MAX_ORDER}
)
