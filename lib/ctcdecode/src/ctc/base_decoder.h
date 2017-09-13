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

#ifndef CTCDECODE_CTC_BASE_DECODER_H_
#define CTCDECODE_CTC_BASE_DECODER_H_

#include <vector>

#include <Eigen/Core>

#include "util/status.h"

namespace ctcdecode {
namespace ctc {

namespace detail {

typedef Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>
    RowMajorMatrixXf;

}  // namespace detail

// The BaseDecoder is an abstract interface to be implemented when providing a
// decoding method on the timestep output of a RNN trained with CTC loss.
class BaseDecoder {
 public:
  typedef Eigen::Map<const Eigen::ArrayXi> SequenceLength;
  typedef Eigen::Map<const detail::RowMajorMatrixXf> Input;
  typedef std::vector<std::vector<int>> Output;
  typedef std::vector<std::vector<int>> Alignment;
  typedef Eigen::Map<detail::RowMajorMatrixXf> ScoreOutput;

  BaseDecoder(int num_labels, int blank_label, bool merge_repeated)
      : num_labels_(num_labels),
        blank_label_(blank_label),
        merge_repeated_(merge_repeated) {}

  virtual ~BaseDecoder() {}

  // Dimensionality of the inputs/outputs is expected to be:
  //  - seq_len[b]:         b = 0 to batch_size_
  //  - inputs[t].rows(b):  t = 0 to timesteps; b = 0 t batch_size_
  //  - outputs.size()      specifies the number of beams to be returned.
  //  - scores(b, i):       b = 0 to batch_size; i = 0 to output.size()

  // TODO Refactor so that the Decode member function takes pointers to raw
  // data. We can create our Eigen::Map internally, instead of forcing our
  // poor user to do it when they interface.
  virtual util::Status Decode(const SequenceLength &seq_len,
                              const std::vector<Input> &inputs,
                              std::vector<Output> *outputs,
                              std::vector<Alignment> *alignments,
                              ScoreOutput *scores) = 0;

 protected:
  int num_labels_;
  int blank_label_;
  bool merge_repeated_;
};

}  // namespace ctc
}  // namespace ctcdecode

#endif  // CTCDECODE_CTC_DECODER_H_
