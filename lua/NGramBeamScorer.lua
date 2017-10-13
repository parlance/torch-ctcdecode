local ctcdecode = require 'ctcdecode.env'
local argcheck = require 'argcheck'
local ffi = require 'ffi'
local paths = require 'paths'

-- ***** NGramBeamScorer *****

ffi.cdef[[
typedef struct NGramBeamScorer NGramBeamScorer;

NGramBeamScorer *NGramBeamScorer_new(const char *ngram_model_filename,
                                     const char *labels_filename);

void NGramBeamScorer_delete(NGramBeamScorer *cdata);

void NGramBeamScorer_SetNGramModelWeight(NGramBeamScorer *cdata,
                                         float ngram_model_weight);

void NGramBeamScorer_SetWordInsertionWeight(NGramBeamScorer *cdata,
                                            float word_insertion_weight);
]]

local C = ctcdecode.C

local NGramBeamScorer, super = torch.class(
  'ctcdecode.NGramBeamScorer', ctcdecode
)

NGramBeamScorer.__init = argcheck{
  {name = 'self', type = 'ctcdecode.NGramBeamScorer'},
  {name = 'labelsFilename', type = 'string', check = paths.filep},
  {name = 'ngramModelFilename', type = 'string', check = paths.filep},
  call = function(self, labelsFilename, ngramModelFilename)
    self._cdata = ffi.gc(
      C.NGramBeamScorer_new(labelsFilename, ngramModelFilename),
      C.NGramBeamScorer_delete
    )
  end
}

NGramBeamScorer.cdata = argcheck{
  {name = 'self', type = 'ctcdecode.NGramBeamScorer'},
  call =
  function(self)
    return self._cdata
  end
}

NGramBeamScorer.setNGramModelWeight = argcheck{
  {name = 'self', type = 'ctcdecode.NGramBeamScorer'},
  {name = 'ngramModelWeight', type = 'number'},
  call =
  function(self, ngramModelWeight)
    C.NGramBeamScorer_SetNGramModelWeight(
      self:cdata(),
      ngramModelWeight
    )
  end
}

NGramBeamScorer.setWordInsertionWeight = argcheck{
  {name = 'self', type = 'ctcdecode.NGramBeamScorer'},
  {name = 'wordInsertionWeight', type = 'number'},
  call =
  function(self, wordInsertionWeight)
    C.NGramBeamScorer_SetWordInsertionWeight(
      self:cdata(),
      wordInsertionWeight
    )
  end
}

-- Aliases for backwards compatibility (sort of...)

NGramBeamScorer.setAlpha = NGramBeamScorer.setNGramModelWeight

NGramBeamScorer.setBeta = NGramBeamScorer.setWordInsertionWeight
