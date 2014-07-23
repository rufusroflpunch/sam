# INSTALL

### Main Library

No installation required for the main library. Just add the library folder to your projects include directories, then `#include "vm.h"`.

### Samples

This project uses CMake. To compile the samples under the samples folder, just run the appopriate cmake command to generate the project files.

For instance, to create makefiles:

1. `cmake -G "Unix Makefiles"`
2. `make`

This will compile the binaries.

### Sasm

The assembler (sasm) and VM interpreter (sasm-run) can be built the same as above with CMake.