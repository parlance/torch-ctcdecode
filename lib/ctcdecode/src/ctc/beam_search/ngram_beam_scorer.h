#ifndef CTCDECODE_CTC_BEAM_SEARCH_NGRAM_BEAM_SCORER_H_
#define CTCDECODE_CTC_BEAM_SEARCH_NGRAM_BEAM_SCORER_H_

#include <iostream>
#include <memory>
#include <string>

#include <lm/model.hh>

#include "ctc/beam_search/base_beam_scorer.h"
#include "ctc/beam_search/ngram_beam_state.h"
#include "ctc/label_map.h"

namespace ctcdecode {
namespace ctc {
namespace beam_search {

class NGramBeamScorer : public BaseBeamScorer<NGramBeamState> {
 public:
  NGramBeamScorer(const char* label_map_filename,
                  const char* ngram_model_filename)
      : label_map_(LabelMap(label_map_filename)),
        ngram_model_(lm::ngram::LoadVirtual(ngram_model_filename)) {}

  void InitializeState(NGramBeamState* root) const override {
    root->ngram_score = 0;
    root->num_words = 0;
    root->word_prefix.clear();
    ngram_model_->NullContextWrite(&root->ngram_state);
  }

  void ExpandState(const NGramBeamState& from_state, int from_label,
                   NGramBeamState* to_state, int to_label) const override {
    (void)from_label;  // unused

    // Copy state information.
    (*to_state) = from_state;

    if (label_map_.IsSpace(to_label)) {
      // Only increment
      if (!from_state.word_prefix.empty()) {
        ++to_state->num_words;
      }

      // NOTE: This will also update to_state->ngram_state;
      to_state->ngram_score =
          ScoreNewWord(from_state.ngram_state, from_state.word_prefix,
                       &to_state->ngram_state);
      to_state->word_prefix.clear();  // just to be safe

    } else {
      to_state->ngram_score = 0;
      to_state->word_prefix.append(label_map_.GetToken(to_label));
    }
  }

  void ExpandStateEnd(NGramBeamState* state) const override {
    if (!state->word_prefix.empty()) {
      ++state->num_words;
    }

    lm::ngram::State to_ngram_state;
    state->ngram_score =
        ScoreNewWord(state->ngram_state, state->word_prefix, &to_ngram_state);
    state->ngram_state = to_ngram_state;  // explicitly update the ngram state
    state->word_prefix.clear();
  }

  float GetStateExpansionScore(const NGramBeamState& state,
                               float previous_score) const override {
    return previous_score + ngram_model_weight_ * state.ngram_score +
           word_insertion_weight_ * state.num_words;
  }

  float GetStateEndExpansionScore(const NGramBeamState& state) const override {
    return ngram_model_weight_ * state.ngram_score +
           word_insertion_weight_ * state.num_words;
  }

  // Mutators
  void SetNGramModelWeight(float ngram_model_weight) {
    ngram_model_weight_ = ngram_model_weight;
  }

  void SetWordInsertionWeight(float word_insertion_weight) {
    word_insertion_weight_ = word_insertion_weight;
  }

 private:
  // This is a wrapper around KenLM `lm::base::Model::BaseScore`.
  float ScoreNewWord(const lm::ngram::State& from_ngram_state,
                     const std::string& new_word,
                     lm::ngram::State* to_ngram_state) const;

  const LabelMap label_map_;
  std::unique_ptr<lm::base::Model> ngram_model_;
  float ngram_model_weight_ = 1.0;
  float word_insertion_weight_ = 0.0;
};

}  // namespace beam_search
}  // namespace ctc
}  // namespace ctcdecode

#endif  // CTCDECODE_CTC_BEAM_SEARCH_KENLM_BEAM_SCORER_H_
