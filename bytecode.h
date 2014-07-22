#ifndef BYTECODE_H
#define BYTECODE_H
#include "vm.h"

#define SAM_BYTECODE_VER 1                      // This is the current version of the bytecode. If any ordering changes are made, or
// opcodes are added, this should be increased.

namespace Sam
{
enum Bytecode
{
  PUSH = 1,
  POP,
  ADD,
  SUB,
  MUL,
  DIV,
  MOD,
  INC,
  DEC,
  JGE,
  JGT,
  JLE,
  JLT,
  JEQ,
  JMP,
  OUT,
  IN,
  DBG,
  STORE,
  LOAD,
  SSTORE,
  SLOAD,
  HALT
};
}

#endif // BYTECODE_H
