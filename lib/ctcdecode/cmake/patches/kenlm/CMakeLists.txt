cmake_minimum_required(VERSION 3.1.3 FATAL_ERROR)
project(KenLM LANGUAGES CXX)

include(GNUInstallDirs)

set(KENLM_MAX_ORDER 6 CACHE STRING "Maximum supported ngram order.")
set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}/kenlm")
set(LIB_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
set(CMAKEPACKAGE_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/cmake/kenlm")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

include(CMakePackageConfigHelpers)

add_library(kenlm
  "lm/bhiksha.cc"
	"lm/binary_format.cc"
	"lm/config.cc"
	"lm/lm_exception.cc"
	"lm/model.cc"
	"lm/quantize.cc"
	"lm/read_arpa.cc"
	"lm/search_hashed.cc"
	"lm/search_trie.cc"
	"lm/sizes.cc"
	"lm/trie.cc"
	"lm/trie_sort.cc"
	"lm/value_build.cc"
	"lm/virtual_interface.cc"
	"lm/vocab.cc"
  "util/bit_packing.cc"
  "util/ersatz_progress.cc"
  "util/exception.cc"
  "util/file_piece.cc"
  "util/file.cc"
  "util/float_to_string.cc"
  "util/integer_to_string.cc"
  "util/mmap.cc"
  "util/murmur_hash.cc"
  "util/parallel_read.cc"
  "util/pool.cc"
  "util/read_compressed.cc"
  "util/scoped.cc"
  "util/spaces.cc"
  "util/string_piece.cc"
  "util/usage.cc"
  "util/double-conversion/bignum-dtoa.cc"
  "util/double-conversion/bignum.cc"
  "util/double-conversion/cached-powers.cc"
  "util/double-conversion/diy-fp.cc"
  "util/double-conversion/double-conversion.cc"
  "util/double-conversion/fast-dtoa.cc"
  "util/double-conversion/fixed-dtoa.cc"
  "util/double-conversion/strtod.cc"
)

target_compile_definitions(kenlm PUBLIC "KENLM_MAX_ORDER=${KENLM_MAX_ORDER}")
target_include_directories(kenlm PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>
)

set_target_properties(kenlm PROPERTIES EXPORT_NAME KenLM)

install(TARGETS kenlm
  EXPORT KenLMTargets
  ARCHIVE DESTINATION   ${LIB_INSTALL_DIR}
  LIBRARY DESTINATION   ${LIB_INSTALL_DIR}
  INCLUDES DESTINATION  ${INCLUDE_INSTALL_DIR}
)
install(DIRECTORY lm util DESTINATION ${INCLUDE_INSTALL_DIR}
  FILES_MATCHING PATTERN "*.hh"
  PATTERN "lm/builder" EXCLUDE
  PATTERN "lm/common" EXCLUDE
  PATTERN "lm/filter" EXCLUDE
  PATTERN "lm/interpolate" EXCLUDE
  PATTERN "lm/wrappers" EXCLUDE
  PATTERN "util/double-conversion" EXCLUDE
  PATTERN "util/stream" EXCLUDE
)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/KenLMConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/KenLMConfig.cmake
  INSTALL_DESTINATION ${CMAKEPACKAGE_INSTALL_DIR}
)

export(TARGETS kenlm NAMESPACE KenLM:: FILE KenLMTargets.cmake)

export(PACKAGE KenLM)

install(EXPORT KenLMTargets DESTINATION ${CMAKEPACKAGE_INSTALL_DIR}
  NAMESPACE KenLM::
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/KenLMConfig.cmake
  DESTINATION ${CMAKEPACKAGE_INSTALL_DIR}
)
