cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
project(thctc CXX)

# Required for the libthctc backend.
cmake_policy(SET CMP0042 NEW)
set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)

# Third party dependencies
set(EP_PREFIX third_party)
set_directory_properties(PROPERTIES EP_PREFIX ${EP_PREFIX})
include(cmake/third_party/utfcpp.cmake)
include(cmake/third_party/eigen.cmake)
include(cmake/third_party/kenlm.cmake)

find_package(Torch REQUIRED)

# Install libkenlm.{so,dylib} to the Torch library directory
get_target_property(KENLM_LIBRARY third_party::kenlm IMPORTED_LOCATION)
install(FILES ${KENLM_LIBRARY} DESTINATION ${Torch_INSTALL_LIB_SUBDIR})

# Generate our ffi.lua file.
file(STRINGS lib${PROJECT_NAME}/${PROJECT_NAME}.h ${PROJECT_NAME}_h
  NEWLINE_CONSUME
)
string(REPLACE "#include \"TH.h\""
  "" ${PROJECT_NAME}_h
  ${${PROJECT_NAME}_h}
)
configure_file(${PROJECT_NAME}/ffi.lua.in ${PROJECT_NAME}/ffi.lua @ONLY)

# Set C/C++ sources.
set(${PROJECT_NAME}_SOURCES
  lib${PROJECT_NAME}/${PROJECT_NAME}.cpp
  lib${PROJECT_NAME}/ctc/ctc_status.cpp
)

# Set Lua sources, including the generated ffi.lua file.
set(${PROJECT_NAME}_LUA_SOURCES
  ${PROJECT_NAME}/env.lua
  ${PROJECT_NAME}/init.lua
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/ffi.lua
)

# Add the package.
add_torch_package(${PROJECT_NAME}
  "${${PROJECT_NAME}_SOURCES}" "${${PROJECT_NAME}_LUA_SOURCES}"
)

# Take care of dependencies.
target_link_libraries(${PROJECT_NAME}
  TH
  third_party::kenlm
  third_party::utf8
  third_party::eigen3
)
target_include_directories(${PROJECT_NAME} PRIVATE lib${PROJECT_NAME})
