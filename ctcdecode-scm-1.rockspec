package = 'ctcdecode'
version = 'scm-1'

source = {
  url = '',
}

description = {
  summary = '',
  license = '',
}

dependencies = {
  'argcheck >= 1.0',
  'lua >= 5.1',
  'luaffi >= 1.0',
  'paths >= 1.0',
  'torch >= 7.0',
}

-- build = {
  -- type = "cmake",
  -- variables = {
    -- CMAKE_BUILD_TYPE = "Release",
    -- CMAKE_INSTALL_PREFIX = "$(PREFIX)",
    -- CMAKE_PREFIX_PATH = "$(LUA_BINDIR)/..",
  -- }
-- }


build = {
  type = 'command',
  build_command = [[
  cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$(LUA_BINDIR)/.." -DCMAKE_INSTALL_PREFIX="$(PREFIX)" && $(MAKE) -Cbuild -j$(getconf _NPROCESSORS_ONLN)
    ]],
  install_command = '$(MAKE) -Cbuild install'
}
