local ctcdecode = {}

local ffi = require 'ffi'
ctcdecode.C = ffi.load(package.searchpath('libctcdecode', package.cpath))

return ctcdecode
