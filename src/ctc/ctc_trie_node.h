/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.
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

#ifndef CTC_CTC_TRIE_NODE_H_
#define CTC_CTC_TRIE_NODE_H_

#include <functional>
#include <iostream>
#include <istream>
#include <limits>

#include "lm/model.hh"

namespace torch {
namespace ctc {

class TrieNode {
public:
  TrieNode(int vocab_size)
      : vocab_size(vocab_size), prefix_count(0), min_score_word(0),
        min_unigram_score(std::numeric_limits<float>::max()) {
    children = new TrieNode *[vocab_size]();
  }

  ~TrieNode() {
    for (int i = 0; i < vocab_size; i++) {
      delete children[i];
    }
    delete children;
  }

  void WriteToStream(std::ostream &os) {
    WriteNode(os);
    for (int i = 0; i < vocab_size; i++) {
      if (children[i] == nullptr) {
        os << -1 << std::endl;
      } else {
        // Recursive call
        children[i]->WriteToStream(os);
      }
    }
  }

  static void ReadFromStream(std::istream &is, TrieNode *&obj, int vocab_size) {
    int prefix_count;
    is >> prefix_count;

    if (prefix_count == -1) {
      // This is an undefined child
      obj = nullptr;
      return;
    }

    obj = new TrieNode(vocab_size);
    obj->ReadNode(is, prefix_count);
    for (int i = 0; i < vocab_size; i++) {
      // Recursive call
      ReadFromStream(is, obj->children[i], vocab_size);
    }
  }

  void Insert(const wchar_t *word, std::function<int(wchar_t)> translator,
              lm::WordIndex lm_word, float unigram_score) {
    wchar_t word_character = *word;
    prefix_count++;
    if (unigram_score < min_unigram_score) {
      min_unigram_score = unigram_score;
      min_score_word = lm_word;
    }
    if (word_character != '\0') {
      int vocab_index = translator(word_character);
      TrieNode *child = children[vocab_index];
      if (child == nullptr)
        child = children[vocab_index] = new TrieNode(vocab_size);
      child->Insert(word + 1, translator, lm_word, unigram_score);
    }
  }

  int GetFrequency() { return prefix_count; }

  lm::WordIndex GetMinScoreWordIndex() { return min_score_word; }

  float GetMinUnigramScore() { return min_unigram_score; }

  TrieNode *GetChildAt(int vocab_index) { return children[vocab_index]; }

private:
  int vocab_size;
  int prefix_count;
  lm::WordIndex min_score_word;
  float min_unigram_score;
  TrieNode **children;

  void WriteNode(std::ostream &os) const {
    os << prefix_count << std::endl;
    os << min_score_word << std::endl;
    os << min_unigram_score << std::endl;
  }

  void ReadNode(std::istream &is, int first_input) {
    prefix_count = first_input;
    is >> min_score_word;
    is >> min_unigram_score;
  }
};

} // namespace ctc
} // namespace torch

#endif // CTC_CTC_TRIE_NODE_H_
