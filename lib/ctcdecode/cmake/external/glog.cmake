if (NOT __GLOG_INCLUDED)  # guard against multiple includes
  set(__GLOG_INCLUDED TRUE)

  ExternalProject_Add(glog
    DEPENDS gflags

    GIT_REPOSITORY  https://github.com/google/glog
    GIT_TAG         v0.3.5

    UPDATE_COMMAND ""

    CMAKE_ARGS
      -DBUILD_TESTING=NO
      -DBUILD_SHARED_LIBS:BOOL=NO
      -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
      -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
      -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=ON
      -DWITH_GFLAGS:BOOL=ON
      -Dgflags_DIR:PATH=<INSTALL_DIR>

    LOG_DOWNLOAD  YES
    LOG_INSTALL   YES
  )

  add_library(third_party::glog STATIC IMPORTED)
  add_dependencies(third_party::glog glog)
  ExternalProject_Get_Property(glog INSTALL_DIR)

  set_target_properties(third_party::glog PROPERTIES
    IMPORTED_LOCATION "${INSTALL_DIR}/lib/libglog${CMAKE_STATIC_LIBRARY_SUFFIX}"
    INTERFACE_INCLUDE_DIRECTORIES "${INSTALL_DIR}/include"
    INTERFACE_LINK_LIBRARIES
      "${CMAKE_THREAD_LIBS_INIT};${INSTALL_DIR}/lib/libgflags.a"
  )

  list(APPEND EXTERNAL_LINK_LIBRARIES third_party::glog)
endif()
