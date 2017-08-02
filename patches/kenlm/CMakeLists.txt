cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
project(kenlm)

set(KENLM_MAX_ORDER 6 CACHE STRING "Max order ngram supported.")

add_library(kenlm SHARED
  lm/bhiksha.cc
  lm/binary_format.cc
  lm/config.cc
  lm/lm_exception.cc
  lm/model.cc
  lm/quantize.cc
  lm/read_arpa.cc
  lm/search_hashed.cc
  lm/search_trie.cc
  lm/sizes.cc
  lm/trie.cc
  lm/trie_sort.cc
  lm/value_build.cc
  lm/virtual_interface.cc
  lm/vocab.cc
  # KenLM util sources
  util/bit_packing.cc
  util/ersatz_progress.cc
  util/exception.cc
  util/file.cc
  util/file_piece.cc
  util/float_to_string.cc
  util/integer_to_string.cc
  util/mmap.cc
  util/murmur_hash.cc
  util/parallel_read.cc
  util/pool.cc
  util/read_compressed.cc
  util/scoped.cc
  util/spaces.cc
  util/string_piece.cc
  util/usage.cc
  # KenLM double-conversion sources
  util/double-conversion/bignum-dtoa.cc
  util/double-conversion/bignum.cc
  util/double-conversion/cached-powers.cc
  util/double-conversion/diy-fp.cc
  util/double-conversion/double-conversion.cc
  util/double-conversion/fast-dtoa.cc
  util/double-conversion/fixed-dtoa.cc
  util/double-conversion/strtod.cc
)

target_include_directories(kenlm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(kenlm PUBLIC -DKENLM_MAX_ORDER=${KENLM_MAX_ORDER})

install(TARGETS kenlm DESTINATION lib)
install(DIRECTORY include DESTINATION .)
