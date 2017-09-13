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
=============================================================================*/
#ifndef CTCDECODE_CTC_GREEDY_DECODER_H_
#define CTCDECODE_CTC_GREEDY_DECODER_H_

#include <vector>

#include "ctc/base_decoder.h"
#include "util/status.h"

namespace ctcdecode {
namespace ctc {

// GreedyDecoder is an implementation of the simple best path decoding
// algorithm, selecting at each timestep the most likely class at each timestep.
class GreedyDecoder : public BaseDecoder {
 public:
  GreedyDecoder(int num_labels, int blank_label, bool merge_repeated)
      : BaseDecoder(num_labels, blank_label, merge_repeated) {}

  util::Status Decode(const BaseDecoder::SequenceLength &seq_len,
                      const std::vector<BaseDecoder::Input> &inputs,
                      std::vector<BaseDecoder::Output> *outputs,
                      std::vector<BaseDecoder::Alignment> *alignments,
                      BaseDecoder::ScoreOutput *scores) override;
};

}  // namespace ctc
}  // namespace ctcdecode

#endif
