message(STATUS "Patching marisa-trie")

file(COPY ${marisa_PATCH}/ DESTINATION ${marisa_SOURCE})
file(COPY ${marisa_PATCH}/CMakeLists.txt
  DESTINATION ${marisa_SOURCE}
)
file(COPY ${marisa_PATCH}/cmake/MarisaConfig.cmake.in
  DESTINATION ${marisa_SOURCE}/cmake
)
