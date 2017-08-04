cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
project(thctc CXX)

cmake_policy(SET CMP0042 NEW)
set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)

set(EP_PREFIX third_party)
set_directory_properties(PROPERTIES EP_PREFIX ${EP_PREFIX})
include(cmake/third_party/eigen.cmake)
include(cmake/third_party/utfcpp.cmake)
include(cmake/third_party/kenlm.cmake)

find_package(Torch REQUIRED)

get_target_property(KENLM_LIBRARY third_party::kenlm IMPORTED_LOCATION)
install(FILES ${KENLM_LIBRARY} DESTINATION ${Torch_INSTALL_LIB_SUBDIR})

set(${PROJECT_NAME}_SOURCES
  src/cpu_binding.cpp
  src/ctc/ctc_status.cpp)

add_library(${PROJECT_NAME}
  ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  TH
  third_party::kenlm
  third_party::utf8
  third_party::eigen3
)
target_include_directories(${PROJECT_NAME} PUBLIC src)

install(TARGETS ${PROJECT_NAME}
  EXPORT THCTC-exports
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
install(FILES src/cpu_binding.h DESTINATION include/${PROJECT_NAME})
