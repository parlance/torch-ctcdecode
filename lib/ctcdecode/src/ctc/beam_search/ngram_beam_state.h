#ifndef CTCDECODE_CTC_BEAM_SEARCH_NGRAM_BEAM_STATE_H_
#define CTCDECODE_CTC_BEAM_SEARCH_NGRAM_BEAM_STATE_H_

#include <string>

#include <lm/model.hh>

namespace ctcdecode {
namespace ctc {
namespace beam_search {

struct NGramBeamState {
  float ngram_score;
  int num_words;
  std::string word_prefix;
  lm::ngram::State ngram_state;
};
}  // namespace beam_search
}  // namespace ctc
}  // namespace ctcdecode

#endif  // CTCDECODE_CTC_BEAM_SEARCH_NGRAM_BEAM_STATE_H_
