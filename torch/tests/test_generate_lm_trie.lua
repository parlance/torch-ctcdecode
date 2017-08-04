require 'paths'

local ctc = require 'ctc'

local tests = torch.TestSuite()
local tester = torch.Tester()

function tests.generateLMTrie()
  local labels = "| ABCDEFGHIJKLMNOPQRSTUVWXYZ'"
  ctc.generateLMTrie{
    labels = labels,
    vocabPath = paths.thisfile('file.vocab'),
    lmPath = paths.thisfile('file.probing.bin'),
    outputPath = paths.thisfile('file.2.trie'),
    blankIndex = 0,
    spaceIndex = 1
  }
end

tests.generateLMTrie()

--
-- tester:add(tests)
-- tester:run()
