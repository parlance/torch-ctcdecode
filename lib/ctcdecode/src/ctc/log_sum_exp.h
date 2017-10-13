#ifndef CTCDECODE_CTC_LOG_SUM_EXP_H_
#define CTCDECODE_CTC_LOG_SUM_EXP_H_

#include "ctc/constants.h"

namespace ctcdecode {
namespace ctc {

inline float LogSumExp(float log_prob_1, float log_prob_2) {
  // Always have 'b' be the smaller number to avoid the exponential from
  // blowing up.
  if (log_prob_1 == kLogZero && log_prob_2 == kLogZero) {
    return kLogZero;
  } else {
    return (log_prob_1 > log_prob_2)
               ? log_prob_1 + log1pf(expf(log_prob_2 - log_prob_1))
               : log_prob_2 + log1pf(expf(log_prob_1 - log_prob_2));
  }
}

}  // namespace ctc
}  // namespace ctcdecode

#endif  // CTCDECODE_CTC_LOG_SUM_EXP_H_
