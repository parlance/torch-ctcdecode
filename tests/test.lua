require 'torch'
local ctcdecode = require 'ctcdecode'
local paths = require 'paths'
torch.setdefaulttensortype('torch.FloatTensor')

local test = torch.TestSuite()

local tester = torch.Tester()

function test.BeamSearchDecoder()
  local logProbabilities = torch.Tensor{
    {{0, 0, 0.6, 0.4}, {0, 0, 0.4, 0.6}},
    {{0, 0, 0.5, 0.5}, {0, 0, 0.5, 0.5}},
    {{0, 0, 0.4, 0.6}, {0, 0, 0.6, 0.4}},
    {{0, 0, 0.4, 0.6}, {0, 0, 0.6, 0.4}},
    {{0, 0, 0.4, 0.6}, {0, 0, 0.6, 0.4}}
  }:log()

  local sequenceLength = torch.IntTensor(logProbabilities:size(2))
  sequenceLength:fill(logProbabilities:size(1))

  local nBest = 3

  local decoder = ctcdecode.BeamSearchDecoder(
    logProbabilities:size(3),
    10 * nBest,
    ctcdecode.DefaultBeamScorer(),
    1,
    false
  )

  -- oof this is a long line
  local outputs, alignments, pathLen, scores = decoder:decode(logProbabilities, nBest, sequenceLength)


  local expected = {
    {
      torch.IntTensor{2, 3},
      torch.IntTensor{2, 3, 2},
      torch.IntTensor{3, 2, 3}
    },
    {
      torch.IntTensor{3, 2},
      torch.IntTensor{3, 2, 3},
      torch.IntTensor{2, 3, 2}
    }
  }

  for b = 1, outputs:size(1) do
    for p = 1, outputs:size(2) do
      -- extract the path of labels
      local path = outputs[{{b}, {p}, {1, pathLen[b][p]}}]:squeeze()
      tester:eq(path, expected[b][p])
    end
  end
end


function test.NGramDecoder()
  local logProbabilities = torch.Tensor{
    {{1, 0, 0, 0}},
    {{0, 0, 0.4, 0.6}},
    {{1, 0, 0, 0}},
    {{0, 1, 0, 0}},  -- force a space
    {{1, 0, 0, 0}},
    {{0, 0, 0.4, 0.6}},
    {{1, 0, 0, 0}},
    {{1, 0, 0, 0}},
  }:log()

  local sequenceLength = torch.IntTensor(logProbabilities:size(2))
  sequenceLength:fill(logProbabilities:size(1))

  local nBest = 1

  local labelMapFilename = paths.thisfile('data/label_map')
  local ngramModelFilename = paths.thisfile('data/test.arpa')

  local scorer = ctcdecode.NGramBeamScorer(labelMapFilename, ngramModelFilename)
  local decoder = ctcdecode.NGramDecoder(
    logProbabilities:size(3),
    10 * nBest,
    scorer,
    1,
    false
  )

  local outputs, alignments, pathLen, scores = decoder:decode(logProbabilities, nBest, sequenceLength)
  local path = outputs[{{1}, {1}, {1, pathLen[1][1]}}]:squeeze()
  -- 'B' is not in the vocab, so we should prefer 'A A'
  tester:eq(path, torch.IntTensor{2, 1, 2})
  print(outputs)
  print(alignments)
end


tester:add(test)
tester:run()
