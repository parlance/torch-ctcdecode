include(ExternalProject)

ExternalProject_Add(eigen
  URL           http://bitbucket.org/eigen/eigen/get/3.3.4.tar.gz
  CMAKE_ARGS    -DCMAKE_BUILD_TYPE=Release
                -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
                -DCMAKE_VERBOSE_MAKEFILE=NO
                -DEIGEN_BUILD_PKGCONFIG=NO
  LOG_DOWNLOAD  YES
  LOG_CONFIGURE YES
  LOG_BUILD     YES
  LOG_INSTALL   YES
)

add_library(eigen3 INTERFACE)
add_dependencies(eigen3 eigen)
ExternalProject_Get_Property(eigen SOURCE_DIR)
target_include_directories(eigen3 INTERFACE
  $<BUILD_INTERFACE:${SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/eigen3>
)

add_library(third_party::eigen3 ALIAS eigen3)

# export(TARGETS eigen3 APPEND FILE ${PROJECT_NAME}-exports.cmake)
