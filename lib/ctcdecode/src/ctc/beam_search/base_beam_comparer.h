#ifndef CTCDECODE_CTC_BEAM_SEARCH_BASE_BEAM_COMPARER_H_
#define CTCDECODE_CTC_BEAM_SEARCH_BASE_BEAM_COMPARER_H_

#include "ctc/beam_search/beam_entry.h"

namespace ctcdecode {
namespace ctc {
namespace beam_search {

template <typename BeamState>
class BaseBeamComparer {
 public:
  virtual ~BaseBeamComparer() {}

  virtual bool operator()(const BeamEntry<BeamState> *lhs,
                          const BeamEntry<BeamState> *rhs) const {
    return lhs->new_prob.total > rhs->new_prob.total;
  }
};

}  // namespace beam_search
}  // namespace ctc
}  // namespace ctcdecode

#endif  // CTCDECODE_CTC_BEAM_SEARCH_BASE_BEAM_COMPARER_H_
