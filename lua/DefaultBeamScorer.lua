local ctcdecode = require 'ctcdecode.env'
local argcheck = require 'argcheck'
local ffi = require 'ffi'

-- ***** DefaultBeamScorer *****

ffi.cdef[[
typedef struct DefaultBeamScorer DefaultBeamScorer;

DefaultBeamScorer *DefaultBeamScorer_new();

void DefaultBeamScorer_delete(DefaultBeamScorer* cdata);
]]

local C = ctcdecode.C

local DefaultBeamScorer = torch.class('ctcdecode.DefaultBeamScorer', ctcdecode)

DefaultBeamScorer.__init = argcheck{
  {name = 'self', type = 'ctcdecode.DefaultBeamScorer'},
  call =
  function(self)
    self._cdata = ffi.gc(
      C.DefaultBeamScorer_new(),
      C.DefaultBeamScorer_delete
    )
  end
}

DefaultBeamScorer.cdata = argcheck{
  {name = 'self', type = 'ctcdecode.DefaultBeamScorer'},
  call =
  function(self)
    return self._cdata
  end
}
