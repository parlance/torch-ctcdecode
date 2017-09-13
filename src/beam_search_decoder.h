#ifndef CTCDECODE_SRC_BEAM_SEARCH_DECODER_H_
#define CTCDECODE_SRC_BEAM_SEARCH_DECODER_H_

#include <TH/TH.h>

#include <ctc/beam_search_decoder.h>

using ctcdecode::ctc::BeamSearchDecoder;

extern "C" {

void *BeamSearchDecoder_new(int num_labels, int beam_width,
                            BeamSearchDecoder<>::DefaultBeamScorer *scorer,
                            int blank_label, bool merge_repeated);

void BeamSearchDecoder_delete(BeamSearchDecoder<> *self);

void BeamSearchDecoder_Decode(BeamSearchDecoder<> *self,
                              THFloatTensor *inputs_tensor,
                              THIntTensor *seq_len_tensor,
                              THIntTensor *outputs_tensor,
                              THIntTensor *alignments_tensor,
                              THIntTensor *path_len_tensor,
                              THFloatTensor *scores_tensor);

void BeamSearchDecoder_SetLabelSelectionSize(BeamSearchDecoder<> *self,
                                             int label_selection_size);

void BeamSearchDecoder_SetLabelSelectionMargin(BeamSearchDecoder<> *self,
                                               float label_selection_margin);
}  // extern "C"

#endif  // CTCDECODE_SRC_BEAM_SEARCH_DECODER_H_
