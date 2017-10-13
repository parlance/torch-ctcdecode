local ctcdecode = require 'ctcdecode.env'
local argcheck = require 'argcheck'
local ffi = require 'ffi'

ffi.cdef[[
typedef struct DefaultBeamScorer DefaultBeamScorer;
typedef struct BeamSearchDecoder BeamSearchDecoder;

void *BeamSearchDecoder_new(int num_labels, int beam_width,
                            DefaultBeamScorer *scorer, int blank_label,
                            bool merge_repeated);

void BeamSearchDecoder_delete(BeamSearchDecoder *cdata);

void BeamSearchDecoder_Decode(BeamSearchDecoder *cdata,
                              THFloatTensor *inputs_tensor,
                              THIntTensor *seq_len_tensor,
                              THIntTensor *outputs_tensor,
                              THIntTensor *alignments_tensor,
                              THIntTensor *path_len_tensor,
                              THFloatTensor *scores_tensor);

void BeamSearchDecoder_SetLabelSelectionSize(BeamSearchDecoder *cdata,
                                             int label_selection_size);

void BeamSearchDecoder_SetLabelSelectionMargin(BeamSearchDecoder *cdata,
                                               float label_selection_margin);
]]


local C = ctcdecode.C

local BeamSearchDecoder = torch.class(
  'ctcdecode.BeamSearchDecoder', ctcdecode
)

-- TODO: Maybe register all of the C function calls so that we can reuse
-- the Lua-side implementations for these methods?

BeamSearchDecoder.__init = argcheck{
  {name = 'self', type = 'ctcdecode.BeamSearchDecoder'},
  {name = 'numClasses', type = 'number'},
  {name = 'beamWidth', type = 'number'},
  {name = 'scorer', type = 'ctcdecode.DefaultBeamScorer'},
  {name = 'blankLabel', type = 'number'},
  {name = 'mergeRepeated', type = 'boolean', default = false},
  call =
  function(self, numClasses, beamWidth, scorer, blankLabel, mergeRepeated)
    self._scorer = scorer -- avoid having scorer garbage collected prematurely
    self._cdata = ffi.gc(
      C.BeamSearchDecoder_new(
        numClasses,
        beamWidth,
        self._scorer:cdata(),
        blankLabel - 1,
        mergeRepeated
      ),
      C.BeamSearchDecoder_delete
    )
  end
}

BeamSearchDecoder.cdata = argcheck{
  {name = 'self', type = 'ctcdecode.BeamSearchDecoder'},
  call =
  function(self)
    return self._cdata
  end
}

BeamSearchDecoder.decode = argcheck{
  {name = 'self', type = 'ctcdecode.BeamSearchDecoder'},
  {name = 'inputs', type = 'torch.FloatTensor'},
  {name = 'topPaths', type = 'number'},
  {name = 'seqLen', type = 'torch.IntTensor', opt = true},
  call =
  function(self, inputs, topPaths, seqLen)
    local maxSeqLen = inputs:size(1)
    local batchSize = inputs:size(2)

    local seqLen = seqLen or torch.IntTensor(batchSize):fill(maxSeqLen)

    local outputs = torch.IntTensor(batchSize, topPaths, maxSeqLen):zero()
    local scores = torch.FloatTensor(batchSize, topPaths):zero()
    local alignments = torch.IntTensor(batchSize, topPaths, maxSeqLen):zero()
    local pathLen = torch.IntTensor(batchSize, topPaths):zero()

    C.BeamSearchDecoder_Decode(
      self:cdata(),
      inputs:cdata(),
      seqLen:cdata(),
      outputs:cdata(),
      alignments:cdata(),
      pathLen:cdata(),
      scores:cdata()
    )
    return outputs, alignments, pathLen, scores
  end
}


BeamSearchDecoder.setLabelSelectionSize = argcheck{
  {name = 'self', type = 'ctcdecode.BeamSearchDecoder'},
  {name = 'labelSelectionSize', type = 'number'},
  call =
  function(self, labelSelectionSize)
    C.BeamSearchDecoder_SetLabelSelectionSize(
      self:cdata(),
      labelSelectionSize
    )
  end
}

BeamSearchDecoder.setLabelSelectionMargin = argcheck{
  {name = 'self', type = 'ctcdecode.BeamSearchDecoder'},
  {name = 'labelSelectionMargin', type = 'number'},
  call =
  function(self, labelSelectionMargin)
    C.BeamSearchDecoder_SetLabelSelectionMargin(
      self:cdata(),
      labelSelectionMargin
    )
  end
}
