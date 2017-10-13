/* Copyright 2015 The TensorFlow Authors. All Rights Reserved.

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

#ifndef CTCDECODE_UTIL_MACROS_H_
#define CTCDECODE_UTIL_MACROS_H_

namespace ctcdecode {

// A macro to disallow the copy constructor and operator= functions.
// This is usually placed in the private: declarations for a class.
#define CTCDECODE_DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName &) = delete;               \
  void operator=(const TypeName &) = delete

}  // namespace ctcdecode
#endif  // CTCDECODE_UTIL_MACROS_H_
