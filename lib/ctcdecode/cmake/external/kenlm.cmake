if (NOT __KENLM_INCLUDED)  # guard against multiple includes
  set(__KENLM_INCLUDED TRUE)

  set(KENLM_MAX_ORDER 6 CACHE STRING "Max ngram order supported.")

  ExternalProject_Add(kenlm
    GIT_REPOSITORY  https://github.com/kpu/kenlm
    GIT_TAG         master

    UPDATE_COMMAND ""
    PATCH_COMMAND ${CMAKE_COMMAND}
      -Dkenlm_PATCH:PATH=${CMAKE_CURRENT_SOURCE_DIR}/cmake/patches/kenlm
      -Dkenlm_SOURCE=<SOURCE_DIR>
      -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/patches/kenlm/patch.cmake

    CMAKE_ARGS
      -DBUILD_SHARED_LIBS:BOOL=ON
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
      -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
      -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=True
      -DKENLM_MAX_ORDER:STRING=${KENLM_MAX_ORDER}

      LOG_DOWNLOAD  YES
      LOG_INSTALL   YES
  )

  add_library(third_party::kenlm SHARED IMPORTED)
  add_dependencies(third_party::kenlm kenlm)
  ExternalProject_Get_Property(kenlm INSTALL_DIR)
  set(KENLM_INCLUDE_DIR "${INSTALL_DIR}/include/kenlm")
  file(MAKE_DIRECTORY ${KENLM_INCLUDE_DIR})

  set_target_properties(third_party::kenlm PROPERTIES
    IMPORTED_LOCATION "${INSTALL_DIR}/lib/libkenlm${CMAKE_SHARED_LIBRARY_SUFFIX}"
    INTERFACE_COMPILE_DEFINITIONS KENLM_MAX_ORDER=${KENLM_MAX_ORDER}
    INTERFACE_INCLUDE_DIRECTORIES ${KENLM_INCLUDE_DIR}
  )

  list(APPEND EXTERNAL_LINK_LIBRARIES third_party::kenlm)

endif()
