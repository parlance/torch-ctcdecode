#ifndef CTCDECODE_SRC_NGRAM_DECODER_H_
#define CTCDECODE_SRC_NGRAM_DECODER_H_

#include <TH/TH.h>

#include <ctc/beam_search/base_beam_scorer.h>
#include <ctc/beam_search_decoder.h>

#include "src/ngram_beam_scorer.h"

using ctcdecode::ctc::beam_search::NGramBeamState;
using ctcdecode::ctc::beam_search::BaseBeamScorer;
using ctcdecode::ctc::BeamSearchDecoder;

extern "C" {

void *NGramDecoder_new(int num_labels, int beam_width,
                       BaseBeamScorer<NGramBeamState> *scorer, int blank_label,
                       bool merge_repeated);

void NGramDecoder_delete(BeamSearchDecoder<NGramBeamState> *self);

void NGramDecoder_Decode(BeamSearchDecoder<NGramBeamState> *self,
                         THFloatTensor *inputs_tensor,
                         THIntTensor *seq_len_tensor,
                         THIntTensor *outputs_tensor,
                         THIntTensor *alignments_tensor,
                         THIntTensor *path_len_tensor,
                         THFloatTensor *scores_tensor);

void NGramDecoder_SetLabelSelectionSize(BeamSearchDecoder<NGramBeamState> *self,
                                        int label_selection_size);

void NGramDecoder_SetLabelSelectionMargin(
    BeamSearchDecoder<NGramBeamState> *self, float label_selection_margin);

}  // extern "C"

#endif  // CTCDECODE_SRC_NGRAM_DECODER_H_
