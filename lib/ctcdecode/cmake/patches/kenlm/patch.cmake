message(STATUS "Patching kenlm")

file(COPY ${kenlm_PATCH}/ DESTINATION ${kenlm_SOURCE})
file(COPY ${kenlm_PATCH}/CMakeLists.txt
  DESTINATION ${kenlm_SOURCE}
)
file(COPY ${kenlm_PATCH}/cmake/KenLMConfig.cmake.in
  DESTINATION ${kenlm_SOURCE}/cmake
)
