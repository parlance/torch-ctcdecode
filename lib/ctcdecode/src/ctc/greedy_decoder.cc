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

#include "ctc/greedy_decoder.h"

#include "util/status.h"
#include "util/unsigned_to_signed.h"

namespace ctcdecode {
namespace ctc {

util::Status GreedyDecoder::Decode(
    const BaseDecoder::SequenceLength &seq_len,
    const std::vector<BaseDecoder::Input> &inputs,
    std::vector<BaseDecoder::Output> *outputs,
    std::vector<BaseDecoder::Alignment> *alignments,
    BaseDecoder::ScoreOutput *scores) {
  const int batch_size = inputs[0].rows();
  if (outputs->empty() ||
      util::UnsignedToSigned<int>((*outputs)[0].size()) < batch_size) {
    return util::Status(
        util::error::INVALID_ARGUMENT,
        "outputs needs to be of size at least (1, batch_size).");
  }

  if (scores->rows() < batch_size || scores->cols() == 0) {
    return util::Status(util::error::INVALID_ARGUMENT,
                        "scores needs to be of size at least (batch_size, 1).");
  }

  // For each batch entry, identify the transitions
  for (int b = 0; b < batch_size; ++b) {
    int seq_len_b = seq_len[b];
    // Only writing to beam 0
    std::vector<int> &output_b = (*outputs)[0][b];
    std::vector<int> &alignment_b = (*alignments)[0][b];

    int prev_label = -1;
    (*scores)(b, 0) = 0.0;

    for (int t = 0; t < seq_len_b; ++t) {
      int max_label;
      (*scores)(b, 0) += -inputs[t].row(b).maxCoeff(&max_label);
      if (max_label != blank_label_ &&
          !(merge_repeated_ && max_label == prev_label)) {
        output_b.push_back(max_label);
        alignment_b.push_back(t);
      }
      prev_label = max_label;
    }
  }

  return util::Status::OK;
}

}  // namespace ctc
}  // namespace ctcdecode
