# torch-ctcdecode

`torch-ctcdecode` is an implementation of Connectionist Temporal Classification (CTC) decoding for Torch. C++ code is borrowed liberally from TensorFlow.

## Requirements

- C++11 compiler (tested with gcc 4.8.4 and Apple LLVM version 8.0.0)
- CMake 3.1 or later
- Torch7

## Installation

```bash
git clone https://github.com/

#### CMake External Projects

All other 3rd party C++ libraries will be downloaded, built, linked, and installed (if necessary) by CMake. Libraries are statically linked unless they are LGPL, in which case they are dynamically linked.

CMake will download the following libraries:

- Eigen3
- GFlags
- GLog
- KenLM (LGPL, dynamically linked)
- Marisa (LGPL)

See the `lib/ctcdecode/cmake/` directory for more details.
