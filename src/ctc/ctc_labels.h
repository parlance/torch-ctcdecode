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

#ifndef CTC_CTC_LABELS_H_
#define CTC_CTC_LABELS_H_

#include <assert.h>

#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <unordered_map>

#include "utf8.h"

namespace torch {
namespace ctc {

class Labels {
public:
  ~Labels() { delete char_list; }

  Labels(const char *spec_file_path) {
    std::ifstream in(spec_file_path, std::ios::in);
    ReadFromFile(in);
    in.close();
  }

  Labels(const wchar_t *char_list, int size, int blank_index, int space_index)
      : size(size) {
    this->char_list = new wchar_t[size];
    for (int i = 0; i < size; i++) {
      this->char_list[i] = char_list[i];
      char_to_label[char_list[i]] = i;
    }
    this->blank_index = blank_index;
    this->space_index = space_index;
  }

  bool inline IsBlank(int label) const {
    // If label is not contained in this Label
    // it must be a blank label
    return label == blank_index;
  }

  bool inline IsSpace(int label) const { return label == space_index; }

  wchar_t GetCharacter(int label) const {
    assert(label < size);
    return char_list[label];
  }

  int GetLabel(wchar_t c) { return char_to_label[c]; }

  int GetSize() const { return size; }

private:
  int size;
  wchar_t *char_list;
  int blank_index;
  int space_index;
  std::unordered_map<wchar_t, int> char_to_label;

  void ReadFromFile(std::istream &is) {
    std::string all_symbols_utf8;
    std::getline(is, all_symbols_utf8);
    std::wstring all_symbols;
    utf8::utf8to32(all_symbols_utf8.begin(), all_symbols_utf8.end(),
                   std::back_inserter(all_symbols));
    size = all_symbols.size();
    char_list = new wchar_t[size];
    for (int i = 0; i < size; i++) {
      char_list[i] = all_symbols[i];
      char_to_label[char_list[i]] = i;
    }
  }
};

} // namespace ctc
} // namespace torch

#endif // CTC_CTC_LABELS_H_
