cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
cmake_policy(SET CMP0042 NEW)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(ExternalProject)
set(EP_PREFIX third_party)
set_directory_properties(PROPERTIES EP_PREFIX ${EP_PREFIX})
include(third_party/eigen)
include(third_party/utfcpp)
include(third_party/kenlm)

find_package(Torch REQUIRED)

file(STRINGS src/cpu_binding.h cpu_binding NEWLINE_CONSUME)
set(ffi ctc/ffi.lua)
file(WRITE ${ffi} "local ffi = require 'ffi'\n\n")
file(APPEND ${ffi} "ffi.cdef[[\n")
file(APPEND ${ffi} ${cpu_binding})
file(APPEND ${ffi} "\n]]\n\n")
file(APPEND ${ffi} "return ffi.load(package.searchpath('libctc', package.cpath))")

set(src "src/cpu_binding.cpp" "src/util/status.cpp")
file(GLOB_RECURSE luasrc "ctc/*.lua")
add_torch_package(ctc "${src}" "${luasrc}")
target_link_libraries(ctc PUBLIC
  TH
  third_party::kenlm
  third_party::eigen3
  third_party::utf8
)
target_compile_definitions(ctc PUBLIC -DINCLUDE_KENLM)
get_target_property(KENLM_LIBRARY third_party::kenlm IMPORTED_LOCATION)
install(FILES ${KENLM_LIBRARY}
  DESTINATION ${Torch_INSTALL_LIB_SUBDIR}
)
