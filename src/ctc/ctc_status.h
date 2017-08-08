#include <string>

#ifndef CTC_CTC_STATUS_H_
#define CTC_CTC_STATUS_H_

namespace torch {
namespace ctc {

enum Code {
  OK,
  CANCELLED,
  INVALID_ARGUMENT,
  FAILED_PRECONDITION,
  OUT_OF_RANGE
};

class Status {
public:
  Status() : state_(NULL) {}
  ~Status() { delete state_; }

  Status(torch::ctc::Code code, std::string msg);

  /// Copy the specified status.
  Status(const Status &s);
  void operator=(const Status &s);

  static Status OK() { return Status(); }

  bool ok() const { return (state_ == NULL); }

  torch::ctc::Code code() const { return ok() ? torch::ctc::OK : state_->code; }

  const std::string &error_message() const {
    return ok() ? empty_string() : state_->msg;
  }

  bool operator==(const Status &x) const;
  bool operator!=(const Status &x) const;
  std::string ToString() const;

private:
  static const std::string &empty_string();
  struct State {
    torch::ctc::Code code;
    std::string msg;
  };
  // OK status has a `NULL` state_.  Otherwise, `state_` points to
  // a `State` structure containing the error code and message(s)
  State *state_;
};

inline Status::Status(const Status &s)
    : state_((s.state_ == NULL) ? NULL : new State(*s.state_)) {}

inline bool Status::operator==(const Status &x) const {
  return (this->state_ == x.state_) || (ToString() == x.ToString());
}

inline bool Status::operator!=(const Status &x) const { return !(*this == x); }

/// @ingroup core
std::ostream &operator<<(std::ostream &os, const Status &x);

namespace errors {

Status InvalidArgument(std::string msg);
Status FailedPrecondition(std::string msg);

} // namespace errors

} // namespace ctc
} // namespace torch

#endif // CTC_CTC_STATUS_H_
