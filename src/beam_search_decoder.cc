#include "src/beam_search_decoder.h"

extern "C" {

void *BeamSearchDecoder_new(int num_labels, int beam_width,
                            BeamSearchDecoder<>::DefaultBeamScorer *scorer,
                            int blank_label, bool merge_repeated) {
  return static_cast<void *>(new BeamSearchDecoder<>(
      num_labels, beam_width, scorer, blank_label, merge_repeated));
}

void BeamSearchDecoder_delete(BeamSearchDecoder<> *self) { delete self; }

// TODO Refactor. It's doing too much.
// TODO Add comments.
void BeamSearchDecoder_Decode(BeamSearchDecoder<> *self,
                              THFloatTensor *inputs_tensor,
                              THIntTensor *seq_len_tensor,
                              THIntTensor *outputs_tensor,
                              THIntTensor *alignments_tensor,
                              THIntTensor *path_len_tensor,
                              THFloatTensor *scores_tensor) {
  THIntTensor_zero(outputs_tensor);
  THIntTensor_zero(alignments_tensor);
  THFloatTensor_zero(scores_tensor);
  THIntTensor_zero(path_len_tensor);

  const int timesteps = THFloatTensor_size(inputs_tensor, 0);
  const int batch_size = THFloatTensor_size(inputs_tensor, 1);
  const int num_labels = THFloatTensor_size(inputs_tensor, 2);
  const int top_paths = THIntTensor_size(outputs_tensor, 1);

  BeamSearchDecoder<>::SequenceLength seq_len(
      THIntTensor_data(seq_len_tensor), THIntTensor_size(seq_len_tensor, 0));

  std::vector<BeamSearchDecoder<>::Input> inputs;
  inputs.reserve(timesteps);
  for (int t = 0; t < timesteps; ++t) {
    inputs.emplace_back(
        THFloatTensor_data(inputs_tensor) +
            ptrdiff_t(t * THFloatTensor_stride(inputs_tensor, 0)),
        batch_size, num_labels);
  }

  std::vector<BeamSearchDecoder<>::Output> outputs(top_paths);
  for (auto &output : outputs) {
    output.resize(batch_size);
  }

  std::vector<BeamSearchDecoder<>::Alignment> alignments(top_paths);
  for (auto &alignment : alignments) {
    alignment.resize(batch_size);
  }

  BeamSearchDecoder<>::ScoreOutput scores(THFloatTensor_data(scores_tensor),
                                          batch_size, top_paths);

  ctcdecode::util::Status status =
      self->Decode(seq_len, inputs, &outputs, &alignments, &scores);
  if (!status.ok()) {
    return;
  }

  for (int p = 0; p < top_paths; ++p) {
    for (int b = 0; b < batch_size; ++b) {
      auto const &path = outputs[p][b];
      int *output_dest = THIntTensor_data(outputs_tensor) +
                         ptrdiff_t(b * THIntTensor_stride(outputs_tensor, 0) +
                                   p * THIntTensor_stride(outputs_tensor, 1));
      std::memcpy(output_dest, path.data(), path.size() * sizeof(int));

      // copy alignment info
      auto const &alignment = alignments[p][b];
      int *alignment_dest =
          THIntTensor_data(alignments_tensor) +
          ptrdiff_t(b * THIntTensor_stride(alignments_tensor, 0) +
                    p * THIntTensor_stride(alignments_tensor, 1));
      std::memcpy(alignment_dest, alignment.data(),
                  alignment.size() * sizeof(int));

      THIntTensor_set2d(path_len_tensor, b, p, path.size());
    }
  }
}

void BeamSearchDecoder_SetLabelSelectionSize(BeamSearchDecoder<> *self,
                                             int label_selection_size) {
  self->SetLabelSelectionSize(label_selection_size);
}

void BeamSearchDecoder_SetLabelSelectionMargin(BeamSearchDecoder<> *self,
                                               float label_selection_margin) {
  self->SetLabelSelectionMargin(label_selection_margin);
}

}  // extern "C"
