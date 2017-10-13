# torch-ctcdecode

`torch-ctcdecode` is an implementation of Connectionist Temporal Classification (CTC) decoding for Torch. C++ code is borrowed liberally from TensorFlow.

## Requirements

- C++11 compiler (tested with gcc 4.8.4 and Apple LLVM version 8.0.0)
- CMake 3.1 or later
- Torch7

### CMake3 Installation

#### Ubuntu

```bash
$ sudo apt-get install cmake3
```

#### OSX

```bash
$ brew install cmake
```

## Installation

```bash
$ git clone https://github.com/willfrey/torch-ctcdecode
$ cd torch-ctcdecode
$ luarocks make
```

### Note About Third Party Libraries

The underlying C++ library utilizes a number of third party projects. These projects are downloaded, built, linked, and installed (if necessary) by CMake. Libraries are statically linked unless they are LGPL, in which case they are dynamically linked.

CMake will download the following libraries:

- Eigen3
- GFlags
- GLog
- KenLM (LGPL)

See the `lib/ctcdecode/cmake/` directory for more details.

# Using `torch-ctcdecode`

```lua
local ctcdecode = require 'ctcdecode'
```

See `tests/test.lua` for some examples. More documentation to come.
