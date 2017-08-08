#include "ctc/ctc_status.h"

#include <stdio.h>

#include <cassert>
#include <iostream>
#include <string>

namespace torch {
namespace ctc {

Status::Status(Code code, std::string msg) {
  assert(code != torch::ctc::OK);
  state_ = new State;
  state_->code = code;
  state_->msg = msg;
}

const std::string &Status::empty_string() {
  static std::string *empty = new std::string;
  return *empty;
}

std::string Status::ToString() const {
  if (state_ == nullptr) {
    return "OK";
  } else {
    char tmp[30];
    const char *type;
    switch (code()) {
    case torch::ctc::Code::CANCELLED:
      type = "Cancelled";
      break;
    case torch::ctc::Code::INVALID_ARGUMENT:
      type = "Invalid argument";
      break;
    case torch::ctc::Code::FAILED_PRECONDITION:
      type = "Failed precondition";
      break;
    case torch::ctc::Code::OUT_OF_RANGE:
      type = "Out of range";
      break;
    default:
      snprintf(tmp, sizeof(tmp), "Unknown code(%d)", static_cast<int>(code()));
      type = tmp;
      break;
    }
    std::string result(type);
    result += ": " + state_->msg;
    return result;
  }
}

std::ostream &operator<<(std::ostream &os, const Status &x) {
  os << x.ToString();
  return os;
}

namespace errors {
Status InvalidArgument(std::string msg) {
  return Status(Code::INVALID_ARGUMENT, msg);
}
Status FailedPrecondition(std::string msg) {
  return Status(Code::FAILED_PRECONDITION, msg);
}

} // namespace errors

} // namespace ctc
} // namespace torch
