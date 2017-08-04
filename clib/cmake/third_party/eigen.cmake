include(ExternalProject)

ExternalProject_Add(eigen
  URL           http://bitbucket.org/eigen/eigen/get/3.3.4.tar.gz
  CMAKE_ARGS    -DCMAKE_BUILD_TYPE=Release
                -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
                -DCMAKE_VERBOSE_MAKEFILE=OFF
  LOG_DOWNLOAD  YES
  LOG_CONFIGURE YES
  LOG_BUILD     YES
  LOG_INSTALL   YES
)
ExternalProject_Get_Property(eigen SOURCE_DIR INSTALL_DIR)
add_library(eigen3 INTERFACE)
target_include_directories(eigen3 INTERFACE ${SOURCE_DIR})
add_dependencies(eigen3 eigen)
add_library(third_party::eigen3 ALIAS eigen3)
