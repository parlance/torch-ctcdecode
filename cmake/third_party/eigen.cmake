ExternalProject_Add(eigen
  URL http://bitbucket.org/eigen/eigen/get/3.3.4.tar.gz
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    -DCMAKE_VERBOSE_MAKEFILE=OFF
    -DCMAKE_BUILD_TYPE=Release
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
)
ExternalProject_Get_Property(eigen SOURCE_DIR INSTALL_DIR)
add_library(eigen3 INTERFACE)
target_include_directories(eigen3 INTERFACE
  $<BUILD_INTERFACE:${SOURCE_DIR}>
  $<INSTALL_INTERFACE:${INSTALL_DIR}/include/eigen3
)
add_dependencies(eigen3 eigen)
add_library(third_party::eigen3 ALIAS eigen3)
