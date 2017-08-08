cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
project(ctc CXX)

# Build configuration.
cmake_policy(SET CMP0042 NEW)
set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)

find_package(Torch REQUIRED)

# Download + build third party dependencies.
set(EP_PREFIX third_party)
set_directory_properties(PROPERTIES EP_PREFIX ${EP_PREFIX})
include(cmake/third_party/kenlm.cmake)
include(cmake/third_party/utfcpp.cmake)
include(cmake/third_party/eigen.cmake)

# KenLM is LGPL, so we must install it as a standalone shared library.
get_target_property(KENLM_LIBRARY third_party::kenlm IMPORTED_LOCATION)
install(FILES ${KENLM_LIBRARY} DESTINATION ${Torch_INSTALL_LIB_SUBDIR})


# Generate our ffi.lua file.
file(STRINGS src/init.h init_h NEWLINE_CONSUME)
# Replace the required #include "TH.h".
# TODO Is there a better way to handle this?
string(REPLACE "#include <TH/TH.h>" "" init_h ${init_h})
configure_file(${PROJECT_NAME}/ffi.lua.in ${PROJECT_NAME}/ffi.lua @ONLY)


set(${PROJECT_NAME}_SOURCES src/init.cpp src/ctc/ctc_status.cpp)

set(${PROJECT_NAME}_LUA_SOURCES
  ${PROJECT_NAME}/env.lua
  ${PROJECT_NAME}/init.lua
  ${PROJECT_NAME}/scorer.lua
  ${PROJECT_NAME}/decoder.lua
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/ffi.lua
)

add_torch_package(${PROJECT_NAME}
  "${${PROJECT_NAME}_SOURCES}"  # compiled sources
  "${${PROJECT_NAME}_LUA_SOURCES}"  # lua sources
)

# Take care of dependencies.
target_link_libraries(${PROJECT_NAME}
  TH
  third_party::kenlm
  third_party::utf8
  third_party::eigen3
)

target_include_directories(${PROJECT_NAME} PRIVATE src)
