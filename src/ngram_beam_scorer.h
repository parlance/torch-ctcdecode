#ifndef CTCDECODE_SRC_PREFIX_BEAM_SCORER_H_
#define CTCDECODE_SRC_PREFIX_BEAM_SCORER_H_

#include <ctc/beam_search/ngram_beam_scorer.h>

using ctcdecode::ctc::beam_search::NGramBeamScorer;

extern "C" {

void *NGramBeamScorer_new(const char *label_map_filename,
                          const char *ngram_model_filename);

void NGramBeamScorer_delete(NGramBeamScorer *self);

void NGramBeamScorer_SetNGramModelWeight(NGramBeamScorer *self,
                                         float ngram_model_weight);

void NGramBeamScorer_SetWordInsertionWeight(NGramBeamScorer *self,
                                            float word_insertion_weight);

}  // extern "C"

#endif  // CTCDECODE_SRC_PREFIX_BEAM_SCORER_H_
