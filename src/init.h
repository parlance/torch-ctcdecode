#include <TH/TH.h>

typedef enum { CTC, CTC_KENLM } DecodeType;

/* scorers */
int kenlm_enabled();
void *get_kenlm_scorer(const wchar_t *label_str, int labels_size,
                       int space_index, int blank_index, const char *lm_path,
                       const char *trie_path);
void set_kenlm_scorer_lm_weight(void *scorer, float weight);
void set_kenlm_scorer_wc_weight(void *scorer, float weight);
void set_kenlm_scorer_vwc_weight(void *scorer, float weight);

void *get_base_scorer();

void free_base_scorer(void *scorer);

/* decoders */
void *get_ctc_beam_decoder(int num_classes, int top_paths, int beam_width,
                           int blank_index, int merge_repeated, void *scorer,
                           DecodeType type);

void *free_ctc_beam_decoder(void *decoder);


/* run decoding */
int ctc_beam_decode(void *decoder, DecodeType type, THFloatTensor *probs,
                    THIntTensor *seq_len, THIntTensor *output,
                    THFloatTensor *scores, THIntTensor *th_out_len);

/* utilities */
int generate_lm_trie(const char *labels, int size, int blank_index,
                     int space_index, const char *lm_path,
                     const char *dictionary_path, const char *output_path);