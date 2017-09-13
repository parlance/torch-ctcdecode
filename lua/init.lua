local ctcdecode = require 'ctcdecode.env'

-- There's a lot of boilerplate code going on.
-- Too much nasty copy + paste.
-- TODO: Is there a way to clean this up?
-- If I could treat the ffi C library as a table,
-- lookups would be easy. But that doesn't seem to work.
-- https://github.com/Xamla/torch-pcl may be a good place to look
-- for inspiration.

require 'ctcdecode.DefaultBeamScorer'
require 'ctcdecode.BeamSearchDecoder'
require 'ctcdecode.NGramBeamScorer'
require 'ctcdecode.NGramDecoder'

return ctcdecode
