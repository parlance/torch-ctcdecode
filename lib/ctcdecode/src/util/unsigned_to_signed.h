#ifndef CTCDECODE_UTIL_SIZE_TO_INT_H_
#define CTCDECODE_UTIL_SIZE_TO_INT_H_

#include <limits>
#include <stdexcept>

// Adapted from a Pluralsight blog post.
// https://www.pluralsight.com/blog/software-development/convert-unsigned-int-to-signed-int

namespace ctcdecode {
namespace util {

template <typename To, typename From>
inline To UnsignedToSigned(From value) {
  if (value > static_cast<From>((std::numeric_limits<To>::max)())) {
    // TODO Better exception error. Maybe use typeid(...).name() ?
    throw std::overflow_error(
        "cannot safely convert the value from unsigned to signed");
  }
  return static_cast<To>(value);
}

}  // namespace util
}  // namespace ctcdecode

#endif  // CTCDECODE_UTIL_SIZE_TO_INT_H_
