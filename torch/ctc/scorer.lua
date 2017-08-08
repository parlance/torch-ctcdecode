local ctc = require 'ctc.env'
local libctc = require 'ctc.ffi'

local argcheck = require 'argcheck'
local doc = require 'argcheck.doc'
local ffi = require 'ffi'

local BaseScorer = torch.class('ctc.BaseScorer', ctc)

BaseScorer.__init = argcheck{
  {name = 'self', type = 'ctc.BaseScorer'},
  call = function(self)
    self._type = 0
    self._cdata = libctc.get_base_scorer()
    ffi.gc(self._cdata, libctc.free_base_scorer)
  end
}

BaseScorer.type = argcheck{
  {name = 'self', type = 'ctc.BaseScorer'},
  call = function(self)
    return self._type
  end
}


BaseScorer.cdata = argcheck{
  {name = 'self', type = 'ctc.BaseScorer'},
  call = function(self)
    return self._cdata
  end
}


local Scorer, super = torch.class('ctc.Scorer', 'ctc.BaseScorer', ctc)
