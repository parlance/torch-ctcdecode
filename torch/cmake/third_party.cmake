set(EP_PREFIX third_party)
set_directory_properties(PROPERTIES EP_PREFIX ${EP_PREFIX})

include(cmake/third_party/utfcpp.cmake)
include(cmake/third_party/eigen.cmake)
include(cmake/third_party/kenlm.cmake)

get_target_property(KENLM_LIBRARY third_party::kenlm IMPORTED_LOCATION)

install(FILES ${KENLM_LIBRARY} DESTINATION ${Torch_INSTALL_LIB_SUBDIR})
