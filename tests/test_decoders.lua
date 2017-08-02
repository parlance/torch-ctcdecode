require 'torch'

local ctc = require 'ctc'

local tests = torch.TestSuite()
local tester = torch.Tester()

-- Smoke test
function tests.simpleDecode()
  local aa = torch.FloatTensor{
    {{0.0, 1.0}},
    {{0.0, 1.0}},
    {{1.0, 0.0}},
    {{0.0, 1.0}},
    {{0.0, 1.0}},
  }:log()
  local seqLen = torch.IntTensor{aa:size(1)}
  local labels = '\0A'
  local scorer = ctc.Scorer()

  local cases = {
    {true, torch.IntTensor{1}},
    {false, torch.IntTensor{1, 1}},
  }
  for _, case in ipairs(cases) do
    local mergeRepeated, expected = unpack(case)
    local decoder = ctc.BeamDecoder{
      scorer = scorer,
      labels = labels,
      topPaths = 1,
      beamWidth = 1,
      blankIndex = 0,
      spaceIndex = 1,
      mergeRepeated = mergeRepeated,
    }
    local labels, _, lengths = decoder:decode(aa, seqLen)
    tester:eq(labels[1][1][{{1, lengths[1][1]}}], expected)
  end
end

tester:add(tests)
tester:run()
