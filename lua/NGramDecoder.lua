local ctcdecode = require 'ctcdecode.env'
local argcheck = require 'argcheck'
local ffi = require 'ffi'

ffi.cdef[[
typedef struct NGramBeamScorer NGramBeamScorer;
typedef struct NGramDecoder NGramDecoder;

typedef struct NGramBeamScorer NGramBeamScorer;
typedef struct NGramDecoder NGramDecoder;

void *NGramDecoder_new(int num_labels, int beam_width, NGramBeamScorer *scorer,
                       int blank_label, bool merge_repeated);

void NGramDecoder_delete(NGramDecoder *cdata);

void NGramDecoder_Decode(NGramDecoder *cdata, THFloatTensor *inputs_tensor,
                         THIntTensor *seq_len_tensor,
                         THIntTensor *outputs_tensor,
                         THIntTensor *alignments_tensor,
                         THIntTensor *path_len_tensor,
                         THFloatTensor *scores_tensor);

void NGramDecoder_SetLabelSelectionSize(NGramDecoder *cdata,
                                        int label_selection_size);

void NGramDecoder_SetLabelSelectionMargin(NGramDecoder *cdata,
                                          float label_selection_margin);
]]


local C = ctcdecode.C

local NGramDecoder = torch.class('ctcdecode.NGramDecoder', ctcdecode)

NGramDecoder.__init = argcheck{
  {name = 'self', type = 'ctcdecode.NGramDecoder'},
  {name = 'numClasses', type = 'number'},
  {name = 'beamWidth', type = 'number'},
  {name = 'scorer', type = 'ctcdecode.NGramBeamScorer'},
  {name = 'blankLabel', type = 'number'},
  {name = 'mergeRepeated', type = 'boolean', default = false},
  call =
  function(self, numClasses, beamWidth, scorer, blankLabel, mergeRepeated)
    self._scorer = scorer -- avoid having scorer garbage collected prematurely
    self._cdata = ffi.gc(
      C.NGramDecoder_new(
        numClasses,
        beamWidth,
        self._scorer:cdata(),
        blankLabel - 1,
        mergeRepeated
      ),
      C.NGramDecoder_delete
    )
  end
}

NGramDecoder.cdata = argcheck{
  {name = 'self', type = 'ctcdecode.NGramDecoder'},
  call =
  function(self)
    return self._cdata
  end
}

NGramDecoder.decode = argcheck{
  {name = 'self', type = 'ctcdecode.NGramDecoder'},
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

    C.NGramDecoder_Decode(
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


NGramDecoder.setLabelSelectionSize = argcheck{
  {name = 'self', type = 'ctcdecode.NGramDecoder'},
  {name = 'labelSelectionSize', type = 'number'},
  call =
  function(self, labelSelectionSize)
    C.NGramDecoder_SetLabelSelectionSize(
      self:cdata(),
      labelSelectionSize
    )
  end
}

NGramDecoder.setLabelSelectionMargin = argcheck{
  {name = 'self', type = 'ctcdecode.NGramDecoder'},
  {name = 'labelSelectionMargin', type = 'number'},
  call =
  function(self, labelSelectionMargin)
    C.NGramDecoder_SetLabelSelectionMargin(
      self:cdata(),
      labelSelectionMargin
    )
  end
}
