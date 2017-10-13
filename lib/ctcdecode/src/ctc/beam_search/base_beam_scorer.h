#ifndef CTCDECODE_CTC_BEAM_SEARCH_BASE_BEAM_SCORER_H_
#define CTCDECODE_CTC_BEAM_SEARCH_BASE_BEAM_SCORER_H_

namespace ctcdecode {
namespace ctc {
namespace beam_search {

template <typename BeamState>
class BaseBeamScorer {
 public:
  virtual ~BaseBeamScorer() {}

  // State initialization.
  virtual void InitializeState(BeamState *root) const {
    (void)root;  // unused
  }

  // ExpandState is called when expanding a beam to one of its children.
  // Called at most once per child beam. In the simplest case, no state
  // expansion is done.
  virtual void ExpandState(const BeamState &from_state, int from_label,
                           BeamState *to_state, int to_label) const {
    (void)from_state;  // unused
    (void)from_label;  // unused
    (void)to_state;    // unused
    (void)to_label;    // unused
  }

  // ExpandStateEnd is called after decoding has finished. Its purpose is to
  // allow a final scoring of the beam in its current state, before resorting
  // and retrieving the TopN requested candidates. Called at most once per
  // beam.
  virtual void ExpandStateEnd(BeamState *state) const {
    (void)state;  // unused
  }

  // GetStateExpansionScore should be an inexpensive method to retrieve the
  // (cached) expansion score computed within ExpandState. The score is
  // multiplied (log-addition) with the input score at the current step from
  // the network.
  //
  // The score returned should be a log-probability. In the simplest case,
  // as there's no state expansion logic, the expansion score is zero.
  virtual float GetStateExpansionScore(const BeamState &state,
                                       float previous_score) const {
    (void)state;  // unused
    return previous_score;
  }

  // GetStateEndExpansionScore should be an inexpensive method to retrieve
  // the (cached) expansion score computed within ExpandStateEnd. The score
  // is multiplied (log-addition) with the final probability of the beam.
  //
  // The score returned should be a log-probability.
  virtual float GetStateEndExpansionScore(const BeamState &state) const {
    (void)state;  // unused
    return 0;
  }
};

}  // namespace beam_search
}  // namespace ctc
}  // namespace ctcdecode

#endif  // CTCDECODE_CTC_BEAM_SEARCH_BASE_BEAM_SCORER_H_
