if (NOT __MARISA_INCLUDED)  # guard against multiple includes
  set(__MARISA_INCLUDED TRUE)

  ExternalProject_Add(marisa
    GIT_REPOSITORY  https://github.com/s-yata/marisa-trie
    GIT_TAG         master

    UPDATE_COMMAND ""
    PATCH_COMMAND ${CMAKE_COMMAND}
      -Dmarisa_PATCH:PATH=${CMAKE_CURRENT_SOURCE_DIR}/cmake/patches/marisa
      -Dmarisa_SOURCE=<SOURCE_DIR>
      -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/patches/marisa/patch.cmake

    CMAKE_ARGS
      -DBUILD_SHARED_LIBS:BOOL=ON
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
      -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
      -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=True

      LOG_DOWNLOAD  YES
      LOG_INSTALL   YES
  )

  add_library(third_party::marisa SHARED IMPORTED)
  add_dependencies(third_party::marisa marisa)
  ExternalProject_Get_Property(marisa INSTALL_DIR)
  set(MARISA_INCLUDE_DIR "${INSTALL_DIR}/include")
  file(MAKE_DIRECTORY ${MARISA_INCLUDE_DIR})

  set_target_properties(third_party::marisa PROPERTIES
    IMPORTED_LOCATION "${INSTALL_DIR}/lib/libmarisa${CMAKE_SHARED_LIBRARY_SUFFIX}"
    INTERFACE_COMPILE_DEFINITIONS MARISA_MAX_ORDER=${MARISA_MAX_ORDER}
    INTERFACE_INCLUDE_DIRECTORIES ${MARISA_INCLUDE_DIR}
  )

  list(APPEND EXTERNAL_LINK_LIBRARIES third_party::marisa)

endif()
