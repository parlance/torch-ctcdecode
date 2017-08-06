cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
project(torch_ctc)

find_package(Torch REQUIRED)

add_subdirectory(clib)
# add_library(thctc STATIC IMPORTED)
# set_target_properties(thctc PROPERTY
  # IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../clib/build/libthctc.a
  # INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT}
#
# file(STRINGS src/cpu_binding.h cpu_binding NEWLINE_CONSUME)
# set(ffi ctc/ffi.lua)
# file(WRITE ${ffi} "local ffi = require 'ffi'\n\n")
# file(APPEND ${ffi} "ffi.cdef[[\n")
# file(APPEND ${ffi} ${cpu_binding})
# file(APPEND ${ffi} "\n]]\n\n")
# file(APPEND ${ffi} "return ffi.load(package.searchpath('libctc', package.cpath))")
#
# set(src src/cpu_binding.cpp src/status.cpp)
# file(GLOB_RECURSE luasrc ctc/*.lua)
# add_torch_package(ctc "${src}" "${luasrc}")
