#include <clocale>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "ctc_labels.h"
#include "utf8.h"

using pytorch::ctc::Labels;

int main() {
  std::setlocale(LC_ALL, "en_US.UTF-8");
  std::wcout.imbue(std::locale("en_US.UTF-8"));

  const char *s = "\x7a\xc3\x9f\xe6\xb0\xb4\xf0\x9f\x8d\x8c";
  size_t sz = 10;

  std::wstring ws;
  utf8::utf8to32(s, s + sz, std::back_inserter(ws));
  std::wcout << ws << std::endl;
  std::cout << ws.size() << std::endl;

}
