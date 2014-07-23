# CHANGELOG

## 0.1
### 07/29/2014

* Initial public release
* Contains code for a standards-compliant C++ virtual machine, uses for generic purposes.
* Complete instruction set
* Saving and loading of binary executable files
* samples/ folder with example on API usage

## 0.2
### TBD

* Sam is now a header-only library! No need to link any files anymore. Just `#include <vm.h>` and you are good to go!
* Added an assembler under the folder sasm/ as well as an application to run Sam binaries. You can now write assembly text files instead of writing C++ to use the VM!
* New VM method: `uint get_ip()`. This method returns the value of the current instruction pointer. This is useful in implementing labels in the assembler.
* SSTORE now pops the value from the stack that is stored. Previously, it only popped the address but left the value on top of the stack.
