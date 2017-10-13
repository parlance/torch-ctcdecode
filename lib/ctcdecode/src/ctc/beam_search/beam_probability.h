#ifndef CTCDECODE_CTC_BEAM_SEARCH_BEAM_PROBABILITY_H_
#define CTCDECODE_CTC_BEAM_SEARCH_BEAM_PROBABILITY_H_

#include "ctc/constants.h"

namespace ctcdecode {
namespace ctc {
namespace beam_search {

struct BeamProbability {
  void Reset() {
    total = kLogZero;
    blank = kLogZero;
    label = kLogZero;
  }

  float total = kLogZero;
  float blank = kLogZero;
  float label = kLogZero;
};

}  // namespace beam_search
}  // namespace ctc
}  // namespace ctcdecode

#endif  // CTCDECODE_CTC_BEAM_SEARCH_BEAM_PROBABILITY_H_
