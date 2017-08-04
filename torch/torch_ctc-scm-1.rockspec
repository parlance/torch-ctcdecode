package = 'torch_ctc'
version = 'scm-1'

source = {
  url = '',
}

description = {
  summary = '',
  license = '',
}

dependencies = {
  'argcheck',
  'lua >= 5.1',
  'luaffi',
  'utf8',
  'paths',
  'torch >= 7.0',
}

build = {
  type = "cmake",
  variables = {
    CMAKE_BUILD_TYPE = "Release",
    CMAKE_PREFIX_PATH = "$(LUA_BINDIR)/..",
    CMAKE_INSTALL_PREFIX = "$(PREFIX)",
  }
}
