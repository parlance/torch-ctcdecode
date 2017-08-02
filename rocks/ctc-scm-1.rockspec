package = 'ctc'
version = 'scm-1'
source = {
  url = 'XXX',
}
description = {
  summary = 'XXX',
  detailed = 'XXX',
   homepage = 'XXX',
   license = 'DR',
}
dependencies = {
  'argcheck',
  'lua >= 5.1',
  'luaffi',
  'paths',
  'torch >= 7.0',
}
build = {
  type = "cmake",
  variables = {
    CMAKE_BUILD_TYPE = "Release",
    CMAKE_PREFIX_PATH="$(LUA_BINDIR)/..",
    CMAKE_INSTALL_PREFIX="$(PREFIX)",
  }
}
