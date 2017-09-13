#ifndef CTCDECODE_SRC_DEFAULT_BEAM_SCORER_H_
#define CTCDECODE_SRC_DEFAULT_BEAM_SCORER_H_

#include <ctc/beam_search_decoder.h>

using ctcdecode::ctc::BeamSearchDecoder;

extern "C" {

void *DefaultBeamScorer_new();

void DefaultBeamScorer_delete(BeamSearchDecoder<>::DefaultBeamScorer *self);

}  // extern "C"

#endif  // CTCDECODE_SRC_DEFAULT_BEAM_SCORER_H_
