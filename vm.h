#ifndef VM_H
#define VM_H

#include <vector>
#include <stack>
#include <string>

#include "bytecode.h"


typedef unsigned int uint;

namespace Sam
{
class VM
{
public:
  VM();
  
  enum ErrorState {
    ERR_NONE = 1,
    ERR_INVALID_INS,
    ERR_OPEN_FILE,
    ERR_BYTECODE_VER,
    ERR_INT_SIZE,
    ERR_READ_FAIL
  } error_state;

  void execute();                               // Execute the entire code vector
  
  bool load(std::string filename);
  bool save(std::string filename);
  void clear();
  void reset();
  uint get_ip();

  // Instructions
  void push(uint val);
  void pop();
  void add();
  void sub();
  void mul();
  void div();
  void mod();
  void inc();
  void dec();
  void jge(uint val, uint addr);		// Jump if >=
  void jgt(uint val, uint addr);		// Jump if >
  void jle(uint val, uint addr);		// Jump if <=
  void jlt(uint val, uint addr);		// Jump if <
  void jeq(uint val, uint addr);		// Jump if ==
  void jmp(uint val);
  void out();
  void in(uint val, uint addr);
  void dbg();
  void store(uint addr);
  void load(uint addr);
  void sstore();
  void sload();
  void halt();

  bool trace; // Trace output

private:
  bool cycle();                                                 // Execute one CPU cycle
  void vec_to_mem(std::vector<uint> str, uint size, uint addr);      // Store an int vector in program memory at the given address
  void alloc(uint addr);                                        // Allocate new memory up to and including 'addr'
  std::vector<uint> string_to_int(std::string conv);                      // This is a convenience method that convert a C++ string in a vector
                                                                // of packed integers.

  std::vector<uint> code;            // Bytecode to run
  std::vector<uint> memory;          // Program memory
  std::stack<uint> mn_stack;         // This is a stack-based VM

  uint ip;
};
}

#endif // VM_H
