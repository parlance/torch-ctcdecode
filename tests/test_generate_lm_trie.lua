local ctc = require 'ctc'

local tests = torch.TestSuite()
local tester = torch.Tester()

function tests.generateLMTrie()

end


tester:add(tests)
tester:run()
