include(ExternalProject)
include(GNUInstallDirs)

set(EP_PREFIX "third_party")
set_directory_properties(PROPERTIES EP_PREFIX ${EP_PREFIX})

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${EP_PREFIX}/include)

# ---[[ Eigen3
include(cmake/external/eigen3.cmake)

# ---[[ Google-gflags
include(cmake/external/gflags.cmake)

# ---[[ Google-glog
include(cmake/external/glog.cmake)

# ---[[ KenLM
include(cmake/external/kenlm.cmake)
# KenLM is LGPL, so we want to link dynamically.
# Thus, we need to install the shared library object.
get_target_property(KENLM_LIBRARY third_party::kenlm IMPORTED_LOCATION)
install(FILES ${KENLM_LIBRARY}
  DESTINATION ${CMAKE_PREFIX_PATH}/${CMAKE_INSTALL_LIBDIR}
)

# Marisa is LGPL also.
# include(cmake/external/marisa.cmake)
# get_target_property(MARISA_LIBRARY third_party::marisa IMPORTED_LOCATION)
# install(FILES ${MARISA_LIBRARY}
  # DESTINATION ${CMAKE_PREFIX_PATH}/${CMAKE_INSTALL_LIBDIR}
# )
