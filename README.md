#Sam
###About

Sam is a stack-based virtual machine written in standards-compliant C++. It's written in a way as to be entirely platform independent.

The basic package is two simple files: `bytecode.h`, the file that defines the various base instructions of the virtual machine. `vm.h` defines the virtual machine logic. This is a header-only library.

### Project Structure

```
vm.h                The main library.
bytecode.h          Supplementary file. Required by vm.h
doc/API.md          More detailed documentation regarding API.
doc/CHANGELOG.md    Current changelog.
doc/INSTALL.md      Build information.
sasm/*              Project files for the Sasm assembler and sasm-run to run assembled binaries.
samples/*           C++ sample files to demonstrate usage of the Sam API.
```