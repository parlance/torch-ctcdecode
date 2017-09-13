cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(libctcdecode)

# ---[[ RPATH
set(CMAKE_MACOSX_RPATH ON)

# ---[[ C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# ---[[ Position Independent Code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ---[[ Dependencies
include(cmake/dependencies.cmake)

add_library(libctcdecode
  "src/util/status.cc"
  "src/ctc/label_map.cc"
  "src/ctc/greedy_decoder.cc"
  "src/ctc/beam_search/ngram_beam_scorer.cc"
  # "src/ctc/kenlm_beam_search_decoder.cc"
)
set_target_properties(libctcdecode PROPERTIES OUTPUT_NAME ctcdecode)
target_include_directories(libctcdecode PUBLIC src)

target_link_libraries(libctcdecode ${EXTERNAL_LINK_LIBRARIES})

include(CTest)
enable_testing()

# add_executable(labels_test
  # "src/ctc/labels_test.cc"
# )
# target_link_libraries(labels_test libctcdecode)
# add_test(test_labels
  # labels_test "${CMAKE_CURRENT_SOURCE_DIR}/share/test.labels"
# )

# add_executable(beam_probability_test
  # "src/ctc/beam_search/beam_probability_test.cc"
# )
# target_link_libraries(beam_probability_test libctcdecode)
# add_test(test_beam_probability beam_probability_test)

# add_executable(ngram_beam_state_test
  # "src/ctc/beam_search/ngram_beam_state_test.cc"
# )
# target_link_libraries(ngram_beam_state_test libctcdecode)
# add_test(test_ngram_beam_state
  # ngram_beam_state_test "${CMAKE_CURRENT_SOURCE_DIR}/share/test.arpa.mmap"
# )
#
# add_executable(greedy_decoder_test
#   "src/ctc/greedy_decoder_test.cc"
# )
# target_link_libraries(greedy_decoder_test libctcdecode)
# add_test(test_greedy_decoder greedy_decoder_test)
#
# add_executable(ngram_beam_scorer_test
#   "src/ctc/beam_search/ngram_beam_scorer_test.cc"
# )
# target_link_libraries(ngram_beam_scorer_test libctcdecode)
# add_test(test_ngram_beam_scorer
#   ngram_beam_scorer_test
#   "${CMAKE_CURRENT_SOURCE_DIR}/share/test.arpa.mmap"
#   "${CMAKE_CURRENT_SOURCE_DIR}/share/test.labels"
# )
