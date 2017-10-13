cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(torch-ctcdecode)

# ---[[ RPATH
set(CMAKE_MACOSX_RPATH ON)

# ---[[ C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# ---[[ Position Independent Code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ---[[ Verbose warning
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

find_package(Torch REQUIRED)

# ---[[ Builds libctcdecode target
add_subdirectory(lib/ctcdecode)


set(CTCDECODE_SOURCES
  "src/default_beam_scorer.cc"
  "src/beam_search_decoder.cc"
  "src/ngram_beam_scorer.cc"
  "src/ngram_decoder.cc"
)

set(CTCDECODE_LUA_SOURCES
  "lua/env.lua"
  "lua/init.lua"
  "lua/DefaultBeamScorer.lua"
  "lua/BeamSearchDecoder.lua"
  "lua/NGramBeamScorer.lua"
  "lua/NGramDecoder.lua"
)

include_directories(ctcdecode/src)

add_torch_package(ctcdecode
  "${CTCDECODE_SOURCES}"
  "${CTCDECODE_LUA_SOURCES}"
)
target_link_libraries(ctcdecode libctcdecode TH)
