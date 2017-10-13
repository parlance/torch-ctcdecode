#include "src/ngram_beam_scorer.h"

extern "C" {

void *NGramBeamScorer_new(const char *label_map_filename,
                          const char *ngram_model_filename) {
  return static_cast<void *>(
      new NGramBeamScorer(label_map_filename, ngram_model_filename));
}

void NGramBeamScorer_delete(NGramBeamScorer *self) { delete self; }

void NGramBeamScorer_SetNGramModelWeight(NGramBeamScorer *self,
                                         float ngram_model_weight) {
  self->SetNGramModelWeight(ngram_model_weight);
}

void NGramBeamScorer_SetWordInsertionWeight(NGramBeamScorer *self,
                                            float word_insertion_weight) {
  self->SetWordInsertionWeight(word_insertion_weight);
}

}  // extern "C"
