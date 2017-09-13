#include "ctc/beam_search/ngram_beam_scorer.h"

namespace ctcdecode {
namespace ctc {
namespace beam_search {

float NGramBeamScorer::ScoreNewWord(const lm::ngram::State& from_ngram_state,
                                    const std::string& new_word,
                                    lm::ngram::State* to_ngram_state) const {
  return ngram_model_->BaseScore(&from_ngram_state,
                                 ngram_model_->BaseVocabulary().Index(new_word),
                                 to_ngram_state);
}

}  // namespace beam_search
}  // namespace ctc
}  // namespace ctcdecode
