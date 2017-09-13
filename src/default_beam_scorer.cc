#include "src/default_beam_scorer.h"

using ctcdecode::ctc::BeamSearchDecoder;

extern "C" {

void *DefaultBeamScorer_new() {
  return static_cast<void *>(new BeamSearchDecoder<>::DefaultBeamScorer);
}

void DefaultBeamScorer_delete(BeamSearchDecoder<>::DefaultBeamScorer *self) {
  delete self;
}

}  // extern "C"
