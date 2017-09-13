#ifndef CTCDECODE_CTC_LABEL_MAP_H_
#define CTCDECODE_CTC_LABEL_MAP_H_

#include <string>
#include <vector>

namespace ctcdecode {
namespace ctc {

class LabelMap {
 public:
  LabelMap(const char *filename);

  bool IsSpace(int label) const { return tokens_.at(label) == " "; }

  std::string GetToken(int label) const { return tokens_.at(label); }

 private:
  std::vector<std::string> tokens_;
};

}  // namespace ctc
}  // namespace ctcdecode

#endif  // CTCDECODE_CTC_LABEL_MAP_H_
