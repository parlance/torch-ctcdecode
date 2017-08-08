local ctc = require 'ctc.env'
local libctc = require 'ctc.ffi'

local argcheck = require 'argcheck'
local doc = require 'argcheck.doc'
local ffi = require 'ffi'


local BaseBeamDecoder = torch.class('ctc.BaseBeamDecoder', ctc)

BaseBeamDecoder.__init = argcheck{
  {name = 'self', type = 'ctc.BaseBeamDecoder'},
  {name = 'scorer', type = 'ctc.BaseScorer', default = ctc.Scorer()},
  {name = 'labels', type = 'string'},
  {name = 'topPaths', type = 'number', default = 1},
  {name = 'beamWidth', type = 'number', default = 10},
  {name = 'blankIndex', type = 'number', default = 0},
  {name = 'spaceIndex', type = 'number', default = 1},
  {name = 'mergeRepeated', type = 'boolean', default = false},
  call =
  function(self, scorer, labels, topPaths, beamWidth, blankIndex, spaceIndex, mergeRepeated)
    self._scorer = scorer
    self._labels = labels
    self._topPaths = topPaths
    self._beamWidth = beamWidth
    self._blankIndex = blankIndex
    self._spaceIndex = spaceIndex
    self._mergeRepeated = mergeRepeated
    self._numClasses = #labels
    self._type = scorer:type()
    self._cdata = libctc.get_ctc_beam_decoder(
      self._numClasses,
      self._topPaths,
      self._beamWidth,
      self._blankIndex,
      self._mergeRepeated,
      self._scorer:cdata(),
      self._type
    )
    ffi.gc(self._cdata, libctc.free_ctc_beam_decoder)

    if blankIndex < 0 or blankIndex >= self._numClasses then
      error('blankIndex must be within numClasses')
    end

    if topPaths < 1 or topPaths > beamWidth then
      error('topPaths must be greater than 1 and at most beamWidth')
    end
  end
}


BaseBeamDecoder.cdata = argcheck{
  {name = 'self', type = 'ctc.BaseBeamDecoder'},
  call = function(self)
    return self._decoder
  end
}


BaseBeamDecoder.decode = argcheck{
  {name = 'self', type = 'ctc.BaseBeamDecoder'},
  {name = 'logProbs', type = 'torch.FloatTensor'},
  {name = 'seqLen', type = 'torch.IntTensor', opt = true},
  call =
  function(self, logProbs, seqLen)
    local logProbsSize = logProbs:size()
    local maxSeqLen = logProbs:size(1)
    local batchSize = logProbs:size(2)
    local numClasses = logProbs:size(3)

    if seqLen ~= nil and batchSize ~= seqLen:size(1) then
      error('seqLen shape must be a (batchSize) tensor or nil')
    end

    local seqLen = seqLen or torch.IntTensor(batchSize):fill(maxSeqLen)
    local output = torch.IntTensor(self._topPaths, batchSize, maxSeqLen)
    local scores = torch.FloatTensor(self._topPaths, batchSize)
    local outSeqLen = torch.IntTensor(self._topPaths, batchSize)

    local result = libctc.ctc_beam_decode(
      self:cdata(),
      self._type,
      logProbs:cdata(),
      seqLen:cdata(),
      output:cdata(),
      scores:cdata(),
      outSeqLen:cdata()
    )

    return output, scores, outSeqLen
  end
}


local BeamDecoder, super = torch.class('ctc.BeamDecoder', 'ctc.BaseBeamDecoder', ctc)
