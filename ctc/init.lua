require 'torch'
local argcheck = require 'argcheck'
local ffi = require 'ffi'

local ctc = require 'ctc.env'
local libctc = require 'ctc.ffi'


local BaseBeamDecoder = torch.class('ctc.BaseBeamDecoder', ctc)

BaseBeamDecoder.__init = argcheck{
  {name = 'self', type = 'ctc.BaseBeamDecoder'},
  {name = 'labels', type = 'string'},
  {name = 'topPaths', type = 'number', default = 1},
  {name = 'beamWidth', type = 'number', default = 10},
  {name = 'blankIndex', type = 'number', default = 0},
  {name = 'spaceIndex', type = 'number', default = 1},
  {name = 'mergeRepeated', type = 'boolean', default = false},
  call =
  function(self, labels, topPaths, beamWidth, blankIndex, spaceIndex, mergeRepeated)
    self._labels = labels
    self._topPaths = topPaths
    self._beamWidth = beamWidth
    self._blankIndex = blankIndex
    self._spaceIndex = spaceIndex
    self._mergeRepeated = mergeRepeated
    self._numClasses = #labels
    self._decoder = nil

    if blankIndex < 0 or blankIndex >= self._numClasses then
      error('blankIndex must be within numClasses')
    end

    if topPaths < 1 or topPaths > beamWidth then
      error('topPaths must be greater than 1 and at most beamWidth')
    end
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
      self._decoder,
      self._decoderType,
      logProbs:cdata(),
      seqLen:cdata(),
      output:cdata(),
      scores:cdata(),
      outSeqLen:cdata()
    )

    return output, scores, outSeqLen
  end
}


local BaseScorer = torch.class('ctc.BaseScorer', ctc)

function BaseScorer:__init()
  self._scorerType = 0
  self._scorer = nil
end


function BaseScorer:getScorerType()
  return self._scorerType
end


function BaseScorer:getScorer()
  return self._scorer
end


local Scorer, super = torch.class('ctc.Scorer', 'ctc.BaseScorer', ctc)

-- XXX(will.frey) Will this be garbage collected?
function Scorer:__init()
  super.__init(self)
  self._scorer = libctc.get_base_scorer()
  ffi.gc(self._scorer, libctc.free_base_scorer)
end


local BeamDecoder, super = torch.class('ctc.BeamDecoder', 'ctc.BaseBeamDecoder', ctc)

BeamDecoder.__init = argcheck{
  {name = 'self', type = 'ctc.BeamDecoder'},
  {name = 'scorer', type = 'ctc.BaseScorer'},
  {name = 'labels', type = 'string'},
  {name = 'topPaths', type = 'number', default = 1},
  {name = 'beamWidth', type = 'number', default = 10},
  {name = 'blankIndex', type = 'number', default = 0},
  {name = 'spaceIndex', type = 'number', default = 1},
  {name = 'mergeRepeated', type = 'boolean', default = false},
  call =
  function(self, scorer, labels, topPaths, beamWidth, blankIndex, spaceIndex, mergeRepeated)
    super.__init(self, labels, topPaths, beamWidth, blankIndex, spaceIndex, mergeRepeated)
    local mergeInt = mergeRepeated and 1 or 0
    self._scorer = scorer
    self._decoderType = scorer:getScorerType()
    self._decoder = libctc.get_ctc_beam_decoder(
      self._numClasses,
      self._topPaths,
      self._beamWidth,
      self._blankIndex,
      mergeInt,
      self._scorer:getScorer(),
      self._decoderType
    )
  end
}

--
-- local KenLMScorer, super = torch.class('ctc.KenLMScorer', 'ctc.BaseScorer', ctc)
--
-- KenLMScorer = argcheck{
--   {name = 'self', type = 'ctc.KenLMScorer'},
--   {name = 'labels', type = 'string'},
--   {name = 'lmPath', type = 'string', check = paths.filep},
--   {name = 'triePath', type = 'string', check = paths.filep},
--   {name = 'blankIndex', type = 'number', default = 0},
--   {name = 'spaceIndex', type = 'number', default = 1},
--   call =
--   function(self, labels, lmPath, triePath, blankIndex, spaceIndex)
--     super.__init()
--     if libctc.kenlm_enabled() ~= 1 then
--       error('libctc not compiled with KenLM support!')
--     end
--     self._scorerType = 1
--     self._scorer = libctc.get_kenlm_scorer(
--       labels,
--       #labels,
--       spaceIndex,
--       blankIndex,
--       lmPath,
--       triePath
--     )
--   end
-- }
--
-- KenLMScorer.setLMWeight = argcheck{
--   {name = 'self', type = 'ctc.KenLMScorer'},
--   {name = 'weight', type = 'number'},
--   call =
--   function(self, weight)
--     libctc.set_kenlm_scorer_lm_weight(self._scorer, weight)
--   end
-- }
--
--
-- KenLMScorer.setWordWeight = argcheck{
--   {name = 'self', type = 'ctc.KenLMScorer'},
--   {name = 'weight', type = 'number'},
--   call =
--   function(self, weight)
--     libctc.set_kenlm_scorer_wc_weight(self._scorer, weight)
--   end
-- }
--
--
-- KenLMScorer.setValidWordWeight = argcheck{
--   {name = 'self', type = 'ctc.KenLMScorer'},
--   {name = 'weight', type = 'number'},
--   call =
--   function(self, weight)
--     libctc.set_kenlm_scorer_vwc_weight(self._scorer, weight)
--   end
-- }


ctc.generateLMTrie = argcheck{
  {name = 'vocabPath', type = 'string', check = paths.filep},
  {name = 'lmPath', type = 'string', check = paths.filep},
  {name = 'outputPath', type = 'string'},
  {name = 'labels', type = 'string'},
  {name = 'blankIndex', type = 'number', default = 0},
  {name = 'spaceIndex', type = 'number', default = 1},
  call =
  function(vocabPath, lmPath, outputPath, labels, blankIndex, spaceIndex)
    if libctc.kenlm_enabled() ~= 1 then
      error('libctc not compiled with KenLM support!')
    end
    local result = libctc.generate_lm_trie(
      ffi.cast('wchar_t *', labels),
      tonumber(#labels),
      blankIndex,
      spaceIndex,
      kenlmPath,
      dictionaryPath,
      outputPath
    )

    if result ~= 0 then
      error('error encounted generating trie')
    end

  end
}



return ctc
