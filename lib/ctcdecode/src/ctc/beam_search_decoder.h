/* Copyright 2016 The TensorFlow Authors. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#ifndef CTCDECODE_CTC_BEAM_SEARCH_DECODER_H_
#define CTCDECODE_CTC_BEAM_SEARCH_DECODER_H_

#include <functional>
#include <memory>
#include <vector>

#include <Eigen/Core>

#include "ctc/base_decoder.h"
#include "ctc/beam_search/base_beam_comparer.h"
#include "ctc/beam_search/base_beam_scorer.h"
#include "ctc/beam_search/beam_entry.h"
#include "ctc/beam_search/beam_probability.h"
#include "ctc/beam_search/empty_beam_state.h"
#include "ctc/constants.h"
#include "ctc/log_sum_exp.h"
#include "gtl/top_n.h"
#include "util/macros.h"
#include "util/unsigned_to_signed.h"

namespace ctcdecode {
namespace ctc {

template <typename BeamState = beam_search::EmptyBeamState,
          template <typename> class BeamComparer =
              beam_search::BaseBeamComparer>
class BeamSearchDecoder : public BaseDecoder {
  // Beam Search
  //
  // Example (GravesTh Fig. 7.5):
  //         a    -
  //  P = [ 0.3  0.7 ]  t = 0
  //      [ 0.4  0.6 ]  t = 1
  //
  // Then P(l = -) = P(--) = 0.7 * 0.6 = 0.42
  //      P(l = a) = P(a-) + P(aa) + P(-a) = 0.3*0.4 + ... = 0.58
  //
  // In this case, Best Path decoding is suboptimal.
  //
  // For Beam Search, we use the following main recurrence relations:
  //
  // Relation 1:
  // ---------------------------------------------------------- Eq. 1
  //      P(l=abcd @ t=7) = P(l=abc  @ t=6) * P(d @ 7)
  //                      + P(l=abcd @ t=6) * (P(d @ 7) + P(- @ 7))
  // where P(l=? @ t=7), ? = a, ab, abc, abcd are all stored and
  // updated recursively in the beam entry.
  //
  // Relation 2:
  // ---------------------------------------------------------- Eq. 2
  //      P(l=abc? @ t=3) = P(l=abc @ t=2) * P(? @ 3)
  // for ? in a, b, d, ..., (not including c or the blank index),
  // and the recurrence starts from the beam entry for P(l=abc @ t=2).
  //
  // For this case, the length of the new sequence equals t+1 (t
  // starts at 0).  This special case can be calculated as:
  //   P(l=abc? @ t=3) = P(a @ 0)*P(b @ 1)*P(c @ 2)*P(? @ 3)
  // but we calculate it recursively for speed purposes.

  // The beam search decoder is constructed specifying the beam_width
  // (number of candidates to keep at each decoding timestep) and a
  // beam scorer (used for custom scoring, for example enabling the use of
  // a language model). The ownership of the scorer remains with the caller.
 public:
  using DefaultBeamScorer = beam_search::BaseBeamScorer<BeamState>;

  BeamSearchDecoder(int num_labels, int beam_width,
                    beam_search::BaseBeamScorer<BeamState> *beam_scorer,
                    int blank_label, bool merge_repeated = false);

  ~BeamSearchDecoder() override {}

  util::Status Decode(const BaseDecoder::SequenceLength &seq_len,
                      const std::vector<BaseDecoder::Input> &inputs,
                      std::vector<BaseDecoder::Output> *outputs,
                      std::vector<BaseDecoder::Alignment> *alignments,
                      BaseDecoder::ScoreOutput *scores) override;

  template <typename Derived>  // template argument deduction?
  void Step(int timestep, const Eigen::EigenBase<Derived> &raw_input);

  beam_search::BaseBeamScorer<BeamState> *GetBeamScorer() const {
    return beam_scorer_;
  }

  int LabelSelectionSize() const { return label_selection_size_; }

  void SetLabelSelectionSize(int label_selection_size) {
    label_selection_size_ = label_selection_size;
  }

  float LabelSelectionMargin() const { return label_selection_margin_; }

  void SetLabelSelectionMargin(float label_selection_margin) {
    label_selection_margin_ = label_selection_margin;
  }

  void SetLabelSelectionParameters(int label_selection_size,
                                   float label_selection_margin) {
    SetLabelSelectionSize(label_selection_size);
    SetLabelSelectionMargin(label_selection_margin);
  }

  void Reset();

  util::Status TopPaths(int n, BaseDecoder::Output *paths,
                        BaseDecoder::Alignment *alignment,
                        std::vector<float> *log_probs,
                        bool merge_repeated) const;

 private:
  int beam_width_;
  // Label selection is designed to avoid possibly very expensive scorer
  // calls, by pruning the hypotheses based on the input alone. Label
  // selection size controls how many items in each beam are passed through
  // to the beam scorer. Only items with top N input scores are considered.
  // Label selection margin controls the difference between minimal input
  // score (versus the best scoring label) for an item to be passed to the
  // beam scorer. This margin is expressed in terms of log-probability.
  // Default is to do no label selection.
  // For more detail: https://research.google.com/pubs/pub44823.html
  int label_selection_size_ = 0;       // zero means unlimited
  float label_selection_margin_ = -1;  // -1 means unlimited.
  gtl::TopN<beam_search::BeamEntry<BeamState> *, BeamComparer<BeamState>>
      leaves_;
  std::unique_ptr<beam_search::BeamEntry<BeamState>> beam_root_;
  beam_search::BaseBeamScorer<BeamState> *beam_scorer_;

  CTCDECODE_DISALLOW_COPY_AND_ASSIGN(BeamSearchDecoder);
};

// XXX Should the implementations go in a .tcc file?

template <typename BeamState, template <typename> class BeamComparer>
BeamSearchDecoder<BeamState, BeamComparer>::BeamSearchDecoder(
    int num_labels, int beam_width,
    beam_search::BaseBeamScorer<BeamState> *beam_scorer, int blank_label,
    bool merge_repeated)
    : BaseDecoder(num_labels, blank_label, merge_repeated),
      beam_width_(beam_width),
      leaves_(beam_width),
      beam_scorer_(CHECK_NOTNULL(beam_scorer)) {
  Reset();
}

template <typename BeamState, template <typename> class BeamComparer>
util::Status BeamSearchDecoder<BeamState, BeamComparer>::Decode(
    const BaseDecoder::SequenceLength &seq_len,
    const std::vector<BaseDecoder::Input> &inputs,
    std::vector<BaseDecoder::Output> *outputs,
    std::vector<BaseDecoder::Alignment> *alignments,
    BaseDecoder::ScoreOutput *scores) {
  // Storage for top paths.
  BaseDecoder::Output beams;
  BaseDecoder::Alignment beam_alignments;
  std::vector<float> beam_log_probabilities;

  const Eigen::Index batch_size = inputs[0].rows();

  if (std::any_of(outputs->begin(), outputs->end(),
                  [batch_size](const BaseDecoder::Output &output) -> bool {
                    return (Eigen::Index)output.size() < batch_size;
                  })) {
    return util::Status(
        util::error::INVALID_ARGUMENT,
        "outputs needs to be of size at least (top_n, batch_size).");
  }

  const int top_n = outputs->size();

  if (scores->rows() < batch_size || scores->cols() < top_n) {
    return util::Status(
        util::error::INVALID_ARGUMENT,
        "scores needs to be of size at least (batch_size, top_n).");
  }

  for (Eigen::Index b = 0; b < batch_size; ++b) {
    int seq_len_b = seq_len[b];
    Reset();
    for (Eigen::Index t = 0; t < seq_len_b; ++t) {
      // Pass log-probabilities for this example + time.
      Step(t, inputs[t].row(b));
    }  // for (Eigen::Index t ...

    // O(n * log(n))
    std::unique_ptr<std::vector<beam_search::BeamEntry<BeamState> *>> branches(
        leaves_.Extract());
    leaves_.Reset();
    for (int i = 0; i < util::UnsignedToSigned<int>(branches->size()); ++i) {
      beam_search::BeamEntry<BeamState> *entry = (*branches)[i];
      beam_scorer_->ExpandStateEnd(&entry->state);
      entry->new_prob.total +=
          beam_scorer_->GetStateEndExpansionScore(entry->state);
      leaves_.push(entry);
    }

    util::Status status = TopPaths(top_n, &beams, &beam_alignments,
                                   &beam_log_probabilities, merge_repeated_);

    if (!status.ok()) {
      return status;
    }

    CHECK_EQ(top_n, beam_log_probabilities.size());
    CHECK_EQ(beams.size(), beam_log_probabilities.size());
    CHECK_EQ(beams.size(), beam_alignments.size());

    for (int i = 0; i < top_n; ++i) {
      // Copy output to the correct beam + batch
      (*outputs)[i][b].swap(beams[i]);
      (*alignments)[i][b].swap(beam_alignments[i]);
      (*scores)(b, i) = -beam_log_probabilities[i];
    }
  }  // for (Eigen::Index b...
  return util::Status::OK;
}

template <typename BeamState, template <typename> class BeamComparer>
template <typename Derived>
void BeamSearchDecoder<BeamState, BeamComparer>::Step(
    int timestep, const Eigen::EigenBase<Derived> &raw_input) {
  Eigen::ArrayXf input = raw_input;
  // Remove the max for stability when performing log-prob calculations.
  input -= input.maxCoeff();

  // Minimum allowed input value for label selection:
  float label_selection_input_min = kLogZero;
  if (label_selection_size_ > 0 && label_selection_size_ < input.size()) {
    std::vector<float> input_copy(input.data(), input.data() + input.size());
    std::nth_element(input_copy.begin(),
                     input_copy.begin() + label_selection_size_ - 1,
                     input_copy.end(), [](float a, float b) { return a > b; });
    label_selection_input_min = input_copy[label_selection_size_ - 1];
  }
  if (label_selection_margin_ >= 0) {
    // max element is 0, per normalization above
    label_selection_input_min =
        std::max(label_selection_input_min, -label_selection_margin_);
  }

  // Extract the beams sorted in decreasing new probability
  CHECK_EQ(num_labels_, input.size());

  std::unique_ptr<std::vector<beam_search::BeamEntry<BeamState> *>> branches(
      leaves_.Extract());
  leaves_.Reset();

  for (beam_search::BeamEntry<BeamState> *branch : *branches) {
    // P(.. @ t) becomes the new P(.. @ t-1)
    branch->old_prob = branch->new_prob;
  }

  for (auto *branch : *branches) {
    if (branch->parent != nullptr) {  // if not the root
      if (branch->parent->Active()) {
        // If last two sequence characters are identical:
        //   Plabel(l=acc @ t=6) = (Plabel(l=acc @ t=5)
        //                          + Pblank(l=ac @ t=5))
        // else:
        //   Plabel(l=abc @ t=6) = (Plabel(l=abc @ t=5)
        //                          + P(l=ab @ t=5))
        float previous = (branch->label == branch->parent->label)
                             ? branch->parent->old_prob.blank
                             : branch->parent->old_prob.total;
        branch->new_prob.label = LogSumExp(
            branch->new_prob.label,
            beam_scorer_->GetStateExpansionScore(branch->state, previous));
      }
      // Plabel(l=abc @ t=6) *= P(c @ 6)
      branch->new_prob.label += input(branch->label);
    }
    // Pblank(l=abc @ t=6) = P(l=abc @ t=5) * P(- @ 6)
    branch->new_prob.blank = branch->old_prob.total + input(blank_label_);
    // P(l=abc @ t=6) = Plabel(l=abc @ t=6) + Pblank(l=abc @ t=6)
    branch->new_prob.total =
        LogSumExp(branch->new_prob.blank, branch->new_prob.label);

    // Push the entry back to the top paths list.
    // Note, this will always fill leaves back up in sorted order.
    leaves_.push(branch);
  }

  // we need to resort branches in descending old_prob order.

  // branches is in descending old_prob order because it was
  // originally in descending new_prob order and we copied new_prob to
  // old_prob.

  // Grow new leaves
  for (auto *branch : *branches) {
    // A new leaf (represented by its BeamProbability) is a candidate
    // iff its total probability is nonzero and either the beam list
    // isn't full, or the lowest probability entry in the beam has a
    // lower probability than the leaf.
    auto is_candidate = [this](const beam_search::BeamProbability &prob) {
      return (prob.total > kLogZero &&
              (util::UnsignedToSigned<int>(leaves_.size()) < beam_width_ ||
               prob.total > leaves_.peek_bottom()->new_prob.total));
    };

    if (!is_candidate(branch->old_prob)) {
      continue;
    }

    if (!branch->HasChildren()) {
      branch->PopulateChildren(timestep, num_labels_, blank_label_);
    }

    for (auto &child : *branch->Children()) {
      if (!child.Active()) {
        // Perform label selection: if input for this label looks very
        // unpromising, never evaluate it with a scorer.
        if (input(child.label) < label_selection_input_min) {
          continue;
        }
        //   Pblank(l=abcd @ t=6) = 0
        child.new_prob.blank = kLogZero;
        // If new child label is identical to beam label:
        //   Plabel(l=abcc @ t=6) = Pblank(l=abc @ t=5) * P(c @ 6)
        // Otherwise:
        //   Plabel(l=abcd @ t=6) = P(l=abc @ t=5) * P(d @ 6)
        beam_scorer_->ExpandState(branch->state, branch->label, &child.state,
                                  child.label);
        float previous = (child.label == branch->label)
                             ? branch->old_prob.blank
                             : branch->old_prob.total;
        child.new_prob.label =
            input(child.label) +
            beam_scorer_->GetStateExpansionScore(child.state, previous);
        // P(l=abcd @ t=6) = Plabel(l=abcd @ t=6)
        child.new_prob.total = child.new_prob.label;

        if (is_candidate(child.new_prob)) {
          // Before adding the new node to the beam, check if the beam
          // is already at maximum width.
          if (util::UnsignedToSigned<int>(leaves_.size()) == beam_width_) {
            // Bottom is no longer in the beam search.  Reset
            // its probability; signal it's no longer in the beam search.
            beam_search::BeamEntry<BeamState> *bottom = leaves_.peek_bottom();
            bottom->new_prob.Reset();
          }
          leaves_.push(&child);
        } else {
          // Deactivate child (signal it's not in the beam)
          child.old_prob.Reset();
          child.new_prob.Reset();
        }
      }  // if (!child.Active()) ...
    }    // for (BeamEntry<BeamState>& child in children...
  }      // for (BeamEntry<BeamState>* branch...
}

// Reset the beam search
template <typename BeamState, template <typename> class BeamComparer>
void BeamSearchDecoder<BeamState, BeamComparer>::Reset() {
  leaves_.Reset();

  // This beam root, and all of its children, will be in memory until
  // the next reset.
  beam_root_.reset(new beam_search::BeamEntry<BeamState>(
      nullptr, 0, -1, num_labels_, blank_label_));
  beam_root_->new_prob.total = 0.0;  // ln(1)
  beam_root_->new_prob.blank = 0.0;  // ln(1)

  // Add the root as the initial leaf.
  leaves_.push(beam_root_.get());

  // Call initialize state on the root object.
  beam_scorer_->InitializeState(&beam_root_->state);
}

template <typename BeamState, template <typename> class BeamComparer>
util::Status BeamSearchDecoder<BeamState, BeamComparer>::TopPaths(
    int n, BaseDecoder::Output *paths, BaseDecoder::Alignment *alignment,
    std::vector<float> *log_probs, bool merge_repeated) const {
  CHECK_NOTNULL(paths)->clear();
  CHECK_NOTNULL(alignment)->clear();
  CHECK_NOTNULL(log_probs)->clear();

  if (n > beam_width_) {
    return util::Status(util::error::INVALID_ARGUMENT,
                        "requested more paths than the beam width.");
  }
  if (n > util::UnsignedToSigned<int>(leaves_.size())) {
    return util::Status(util::error::INVALID_ARGUMENT,
                        "Less leaves in the beam search than requested.");
  }

  gtl::TopN<beam_search::BeamEntry<BeamState> *, BeamComparer<BeamState>>
      top_branches(n);

  // O(beam_width_ * log(n)), space complexity is O(n)
  for (auto it = leaves_.unsorted_begin(); it != leaves_.unsorted_end(); ++it) {
    top_branches.push(*it);
  }
  // O(n * log(n))
  std::unique_ptr<std::vector<beam_search::BeamEntry<BeamState> *>> branches(
      top_branches.Extract());

  for (int i = 0; i < n; ++i) {
    beam_search::BeamEntry<BeamState> *entry((*branches)[i]);
    paths->push_back(entry->LabelSequence(merge_repeated));
    alignment->push_back(entry->TimeAlignment(merge_repeated));
    log_probs->push_back(entry->new_prob.total);
  }
  return util::Status::OK;
}

}  // namespace ctc
}  // namespace ctcdecode

#endif  // CTCDECODE_CTC_BEAM_SEARCH_DECODER_H_
