# INSTALL

### Main Library

No installation required for the main library. Just add the library folder to your projects include directories, then `#include <vm.h>`.

### Samples, Assembler (sasm) and runner (sasm-run)

This project uses CMake. To build, follow these instructions:

1. `$ mkdir build` from man Sam folder.
2. `$ cd build`
3. `$ cmake ..` to create the setting appropriate build files. This example assumes you will be using Makefiles.
4. `$ make run_tests` to run the tests.
5. `$ make sasm_full` to build sasm and sasm-run.
6. `$ make samples` to build the samples.

