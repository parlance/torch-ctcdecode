local ctc = require 'ctc'

local scorer = ctc.Scorer()
local decoder = ctc.BeamDecoder{
  scorer = scorer,
  labels = "\0 ABC",
  topPaths = 1,
  beamWidth = 10,
  blankIndex = 0,
  spaceIndex = 1,
  mergeRepeated = false
}

decoder = nil
scorer = nil
collectgarbage()
collectgarbage()
