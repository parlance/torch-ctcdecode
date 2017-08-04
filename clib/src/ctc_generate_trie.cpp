#include <iostream>
#include <iterator>
#include <string>

#include "ctc_labels.h"
#include "ctc_trie_node.h"
#include "lm/model.hh"
#include "utf8.h"

typedef lm::ngram::ProbingModel Model;

namespace pytorch {
namespace ctc {

lm::WordIndex GetWordIndex(const Model &model, const std::string &word) {
  return model.GetVocabulary().Index(word);
}

float ScoreWord(const Model &model, lm::WordIndex vocab) {
  Model::State in_state(model.NullContextState()), out_state;
  return model.FullScore(in_state, vocab, out_state).prob;
}

int generate_trie(Labels &labels, const char *lm_path, const char *vocab_path,
                  const char *trie_path) {
  lm::ngram::Config config;
  config.load_method = util::POPULATE_OR_READ;
  Model model(lm_path, config);

  TrieNode root(labels.GetSize());

  std::ifstream ifs;
  ifs.open(vocab_path, std::ifstream::in);
  if (!ifs.is_open()) {
    std::cerr << "unable to open vocabulary file " << vocab_path << std::endl;
    return -1;
  }

  std::ofstream ofs;
  ofs.open(trie_path);

  std::string word;
  while (ifs >> word) {
    lm::WordIndex vocab = GetWordIndex(model, word);
    std::wstring wide_word;
    utf8::utf8to32(word.begin(), word.end(), std::back_inserter(wide_word));
    root.Insert(wide_word.c_str(),
                [&labels](wchar_t c) { return labels.GetLabel(c); }, vocab,
                ScoreWord(model, vocab));
  }

  root.WriteToStream(ofs);
  ifs.close();
  ofs.close();
  return 0;
}

} // namespace ctc
} // namespace pytorch

using namespace pytorch::ctc;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <path_to_lm>"
              << " <path_to_labels>"
              << "< VOCAB" << std::endl;
    return 1;
  }

  const char *lm_path = argv[1];
  const char *labels_path = argv[2];

  lm::ngram::Config config;
  config.load_method = util::POPULATE_OR_READ;
  Model model(lm_path, config);

  Labels labels(labels_path);

  TrieNode root(labels.GetSize());

  std::string word;
  while (std::cin >> word) {
    lm::WordIndex vocab = GetWordIndex(model, word);
    float unigram_score = ScoreWord(model, vocab);
    std::wstring wide_word;
    utf8::utf8to32(word.begin(), word.end(), std::back_inserter(wide_word));
    root.Insert(wide_word.c_str(),
                [&labels](wchar_t c) {
                  return labels.GetLabel(c);
                },
                vocab, unigram_score);
  }
  root.WriteToStream(std::cout);
  return 0;
}
