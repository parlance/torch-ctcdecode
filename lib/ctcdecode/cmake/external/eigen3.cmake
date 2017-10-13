if (NOT __EIGEN3_INCLUDED)  # guard against multiple includes
  set(__EIGEN3_INCLUDED TRUE)

  ExternalProject_Add(eigen3
    URL http://bitbucket.org/eigen/eigen/get/3.3.4.tar.gz

    UPDATE_COMMAND ""

    CMAKE_ARGS
      -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
      -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
      -DEIGEN_BUILD_PKGCONFIG:BOOL=False
      -DCMAKEPACKAGE_INSTALL_DIR:PATH=share/cmake/eigen3

    LOG_DOWNLOAD  YES
    LOG_CONFIGURE YES
    LOG_INSTALL   YES
  )

  add_library(third_party::eigen3 INTERFACE IMPORTED)
  add_dependencies(third_party::eigen3 eigen3)
  ExternalProject_Get_Property(eigen3 INSTALL_DIR)
  set(EIGEN3_INCLUDE_DIR "${INSTALL_DIR}/include/eigen3")
  file(MAKE_DIRECTORY ${EIGEN3_INCLUDE_DIR})
  set_target_properties(third_party::eigen3 PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${EIGEN3_INCLUDE_DIR}
  )

  list(APPEND EXTERNAL_LINK_LIBRARIES third_party::eigen3)

endif()
