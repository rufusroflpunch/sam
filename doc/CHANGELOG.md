# CHANGELOG

## 0.2.2
### TBD

* Merged bytecode.h and vm.h so there is only one header now.
* Added API call stack_pop(). This does error checking before popping the stack to prevent
  segfaults.

## 0.2.1
### 8/4/14

* Added `uint peek()` function to the C++ API, which returns the current top value of the stack.
* Added unit tests. Can be built/run using make target 'run_tests'.
* Added a better build system for the samples, tests, assembler and runner, using CMake.

## 0.2
### 7/22/14

* Sam is now a header-only library! No need to link any files anymore. Just `#include <vm.h>` and you are good to go!
* Added an assembler under the folder sasm/ as well as an application to run Sam binaries. You can now write assembly text files instead of writing C++ to use the VM!
* New VM method: `uint get_ip()`. This method returns the value of the current instruction pointer. This is useful in implementing labels in the assembler (labels not yet implemented).
* SSTORE now pops the value from the stack that is stored. Previously, it only popped the address but left the value on top of the stack.

Coming Soon:

* Labels for jumping in sasm files.

## 0.1
### 7/19/14

* Initial public release
* Contains code for a standards-compliant C++ virtual machine, uses for generic purposes.
* Complete instruction set
* Saving and loading of binary executable files
* samples/ folder with example on API usage
