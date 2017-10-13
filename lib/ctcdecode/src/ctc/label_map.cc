#include "ctc/label_map.h"

#include <fstream>

namespace ctcdecode {
namespace ctc {

LabelMap::LabelMap(const char *filename) {
  std::ifstream file(filename);
  std::string token;
  while (std::getline(file, token)) {
    tokens_.push_back(token);
  }
}

}  // namespace ctc
}  // namespace ctcdecode
