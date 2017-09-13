#ifndef CTCDECODE_CTC_LABEL_MAP_H_
#define CTCDECODE_CTC_LABEL_MAP_H_

#include <string>
#include <vector>

namespace ctcdecode {
namespace ctc {

class LabelMap {
 public:
   // The file should have one token (string) per line.
   // The space token should be represented by a single whitespace.
   // Beware that many text editors strip trailing whitespace, so a single
   // space character on a line might get removed, causing problems.
  LabelMap(const char *filename);

  bool IsSpace(int label) const { return tokens_.at(label) == "<space>"; }

  std::string GetToken(int label) const { return tokens_.at(label); }

 private:
  std::vector<std::string> tokens_;
};

}  // namespace ctc
}  // namespace ctcdecode

#endif  // CTCDECODE_CTC_LABEL_MAP_H_
