#ifndef CTCDECODE_CTC_BEAM_SEARCH_BEAM_ENTRY_H_
#define CTCDECODE_CTC_BEAM_SEARCH_BEAM_ENTRY_H_

#include <cstddef>
#include <vector>

#include <glog/logging.h>

#include "ctc/beam_search/beam_probability.h"
#include "ctc/constants.h"
#include "util/macros.h"

namespace ctcdecode {
namespace ctc {
namespace beam_search {

// TODO Refactor BeamEntry to be a class.
// Under Google's C++ style guide, this really should be a class I think?
// It's like a really involved struct, since we're using the members directly.
template <typename BeamState>
struct BeamEntry {
  BeamEntry() : parent(nullptr), timestep(-1), label(-1) {}

  BeamEntry(BeamEntry<BeamState> *parent, int timestep, int label,
            int num_labels, int blank_label);

  bool Active() const { return new_prob.total != kLogZero; }

  bool HasChildren() const { return !children.empty(); }

  void PopulateChildren(int timestep, int num_labels, int blank_label);

  std::vector<BeamEntry<BeamState>> *Children() {
    CHECK(HasChildren());
    return &children;
  }

  const std::vector<BeamEntry<BeamState>> *Children() const {
    CHECK(HasChildren());
    return &children;
  }

  std::vector<int> LabelSequence(bool merge_repeated) const;

  std::vector<int> TimeAlignment(bool merge_repeated) const;

  BeamEntry<BeamState> *parent;
  int timestep;
  int label;
  std::vector<BeamEntry<BeamState>> children;
  BeamProbability old_prob;
  BeamProbability new_prob;
  BeamState state;

 private:
  CTCDECODE_DISALLOW_COPY_AND_ASSIGN(BeamEntry<BeamState>);
};

template <typename BeamState>
BeamEntry<BeamState>::BeamEntry(BeamEntry<BeamState> *parent, int timestep,
                                int label, int num_labels, int blank_label)
    : parent(parent), timestep(timestep), label(label) {
  PopulateChildren(timestep, num_labels, blank_label);
}

template <typename BeamState>
void BeamEntry<BeamState>::PopulateChildren(int timestep, int num_labels,
                                            int blank_label) {
  CHECK(!HasChildren());
  children = std::vector<BeamEntry<BeamState>>(num_labels - 1);
  int label = 0;
  for (auto &child : children) {
    if (label == blank_label) {
      ++label;  // skip the blank label
    }
    child.parent = this;
    child.label = label;
    child.timestep = timestep;
    ++label;
  }
}

template <typename BeamState>
std::vector<int> BeamEntry<BeamState>::LabelSequence(
    bool merge_repeated) const {
  std::vector<int> result;
  int prev_label = -1;
  const BeamEntry<BeamState> *current = this;
  while (current->parent != nullptr) {
    if (!merge_repeated || current->label != prev_label) {
      result.push_back(current->label);
    }
    prev_label = current->label;
    current = current->parent;
  }
  std::reverse(result.begin(), result.end());
  return result;
}

template <typename BeamState>
std::vector<int> BeamEntry<BeamState>::TimeAlignment(
    bool merge_repeated) const {
  std::vector<int> result;
  const BeamEntry<BeamState> *current = this;
  while (current->parent != nullptr) {
    if (!merge_repeated) {
      result.push_back(current->timestep);
    }
    current = current->parent;
  }
  std::reverse(result.begin(), result.end());
  return result;
}

}  // namespace beam_search
}  // namespace ctc
}  // namespace ctcdecode

#endif  // CTCDECODE_CTC_BEAM_SEARCH_BEAM_ENTRY_H_
