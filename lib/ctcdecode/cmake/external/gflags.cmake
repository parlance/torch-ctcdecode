if (NOT __GFLAGS_INCLUDED)  # guard against multiple includes
  set(__GFLAGS_INCLUDED TRUE)

  ExternalProject_Add(gflags

    GIT_REPOSITORY  https://github.com/gflags/gflags
    GIT_TAG         v2.2.1

    UPDATE_COMMAND ""

    CMAKE_ARGS
      -DBUILD_PACKAGING:BOOL=OFF
      -DBUILD_SHARED_LIBS:BOOL=OFF
      -DBUILD_STATIC_LIBS:BOOL=ON
      -DBUILD_TESTING:BOOL=OFF
      -DBUILD_gflags_nothreads_LIB:BOOL=OFF
      -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
      -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
      -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=ON
      -DINSTALL_HEADERS:BOOL=ON

    LOG_DOWNLOAD  YES
    LOG_INSTALL   YES
  )

  add_library(third_party::gflags STATIC IMPORTED)
  add_dependencies(third_party::gflags gflags)
  ExternalProject_Get_Property(gflags INSTALL_DIR)
  find_package(Threads REQUIRED)
  set_target_properties(third_party::gflags PROPERTIES
    IMPORTED_LOCATION "${INSTALL_DIR}/lib/libgflags${CMAKE_STATIC_LIBRARY_SUFFIX}"
    INTERFACE_INCLUDE_DIRECTORIES "${INSTALL_DIR}/include"
    INTERFACE_LINK_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}"
  )

  list(APPEND EXTERNAL_LINK_LIBRARIES third_party::gflags)

endif()
