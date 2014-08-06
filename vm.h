#ifndef VM_H
#define VM_H

#include <vector>
#include <stack>
#include <string>
#include <iostream>
#include <fstream>

#define SAM_BYTECODE_VER 1 // This is the current version of the bytecode. If any ordering changes are made, or
                           // opcodes are added, this should be increased.

#define SAM_MAJOR_VER 0    // This represents the current version of the Sam VM.
#define SAM_MINOR_VER 2
#define SAM_REVISION 1

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

class VM
{
public:
  VM();

  enum ErrorState
  {
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
  uint peek();

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
  void jge(uint val, uint addr);                // Jump if >=
  void jgt(uint val, uint addr);                // Jump if >
  void jle(uint val, uint addr);                // Jump if <=
  void jlt(uint val, uint addr);                // Jump if <
  void jeq(uint val, uint addr);                // Jump if ==
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
  void vec_to_mem(std::vector<uint> str, uint size, uint addr); // Store an int vector in program memory at the given address
  void alloc(uint addr);                                        // Allocate new memory up to and including 'addr'
  std::vector<uint> string_to_int(std::string conv);            // This is a convenience method that convert a C++ string in a vector
                                                                // of packed integers.

  std::vector<uint> code;            // Bytecode to run
  std::vector<uint> memory;          // Program memory
  std::stack<uint> mn_stack;         // This is a stack-based VM

  uint ip;
};


VM::VM()
{
  ip = 0;
  trace = false;
  error_state = ERR_NONE;
}

void VM::clear()
{
  ip = 0;
  error_state = ERR_NONE;

  // Clear the mn_stack
  while(!mn_stack.empty()) mn_stack.pop();

  // Clear the code and memory
  memory.clear();
  code.clear();
}

void VM::reset()
{
  ip = 0;
  error_state = ERR_NONE;

  // Clear the mn_stack
  while(!mn_stack.empty()) mn_stack.pop();

  memory.clear();
}

uint VM::get_ip()
{
  return ip;
}

// Peek at the top of the stack without popping it
uint VM::peek()
{
  return mn_stack.top();
}

bool VM::cycle()
{
  uint opcode = code[ip];
  uint val = 0;
  uint addr = 0;
  ip++;

  if(trace)
  {
    std::cout << '\n' << (ip - 1) << "\t: " << opcode << "\tStack: ";

    if(!mn_stack.empty()) std::cout << mn_stack.top();
    else std::cout << "N/A";

    std::cout << "\tOut: ";
  }

  switch(opcode)
  {
  case PUSH:
    mn_stack.push(code[ip]);
    ip++;
    break;

  case POP:
    mn_stack.pop();
    break;

  case ADD:
    val = mn_stack.top();
    mn_stack.pop();
    val += mn_stack.top();
    mn_stack.pop();
    mn_stack.push(val);
    break;

  case SUB:
    val = mn_stack.top();
    mn_stack.pop();
    val -= mn_stack.top();
    mn_stack.pop();
    mn_stack.push(val);
    break;

  case MUL:
    val = mn_stack.top();
    mn_stack.pop();
    val *= mn_stack.top();
    mn_stack.pop();
    mn_stack.push(val);
    break;

  case DIV:
    val = mn_stack.top();
    mn_stack.pop();
    val /= mn_stack.top();
    mn_stack.pop();
    mn_stack.push(val);
    break;

  case MOD:
    val = mn_stack.top();
    mn_stack.pop();
    val %= mn_stack.top();
    mn_stack.pop();
    mn_stack.push(val);
    break;

  case INC:
    val = mn_stack.top();
    mn_stack.pop();
    val++;
    mn_stack.push(val);
    break;

  case DEC:
    val = mn_stack.top();
    mn_stack.pop();
    val--;
    mn_stack.push(val);
    break;

  case JGE:
    val = code[ip];		// The value to compare to
    ip++;
    addr = code[ip];		// The address to jump to if true
    ip++;			// Next opcode for next round
    if(mn_stack.top() >= val) ip = addr;	// If true, jump to the following address.
    break;

  case JGT:
    val = code[ip];
    ip++;
    addr = code[ip];
    ip++;
    if(mn_stack.top() > val) ip = addr;
    break;

  case JLE:
    val = code[ip];
    ip++;
    addr = code[ip];
    ip++;
    if(mn_stack.top() <= val) ip = addr;
    break;

  case JLT:
    val = code[ip];
    ip++;
    addr = code[ip];
    ip++;
    if(mn_stack.top() < val) ip = addr;
    break;

  case JEQ:
    val = code[ip];
    ip++;
    addr = code[ip];
    ip++;
    if(mn_stack.top() == val) ip = addr;
    break;

  case JMP:
    ip = code[ip];
    break;

  case OUT:
  {
    uint uint_chars = mn_stack.top();
    for(int shift = sizeof(uint) * 8 - 8; shift >= 0; shift -= 8)
    {
      char ascii = (char)(uint_chars >> shift);
      // Since a null character (0x00) signals the end of a string in C++, this will short circuit the output of standard out.
      // If it's a null character, don't output that character.
      if(ascii) std::cout << ascii;
    }
    break;
  }

  case IN:
  {
    std::string str;
    getline(std::cin, str);
    int val = code[ip];
    ip++;
    addr = code[ip];
    vec_to_mem(string_to_int(str), val, addr);
    break;
  }

  case DBG:
    std::cout << std::hex <<  mn_stack.top() << std::dec << std::endl;
    break;

  case STORE:
    addr = code[ip];
    ip++;
    alloc(addr); // Make sure there is enough memory
    memory[addr] = mn_stack.top();
    mn_stack.pop();
    break;

  case LOAD:
    addr = code[ip];
    ip++;
    mn_stack.push(memory[addr]);
    break;

  case SSTORE:
    addr = mn_stack.top();
    mn_stack.pop();
    val = mn_stack.top();
    mn_stack.pop();
    alloc(addr);
    memory[addr] = val;
    break;

  case SLOAD:
    addr = mn_stack.top();
    mn_stack.pop();
    mn_stack.push(memory[addr]);
    break;

  case HALT:
    return false;
  }

  return true;
}

void VM::execute()
{
  bool cyc = true;
  while(ip < code.size() && cyc == true)
    cyc = cycle();
}

void VM::push(uint val)
{
  code.push_back(PUSH);
  code.push_back(val);
}

void VM::pop()
{
  code.push_back(POP);
}

void VM::add()
{
  code.push_back(ADD);
}

void VM::sub()
{
  code.push_back(SUB);
}

void VM::mul()
{
  code.push_back(MUL);
}

void VM::div()
{
  code.push_back(DIV);
}

void VM::mod()
{
  code.push_back(MOD);
}

void VM::inc()
{
  code.push_back(INC);
}

void VM::dec()
{
  code.push_back(DEC);
}

void VM::jge(uint val, uint addr)
{
  code.push_back(JGE);
  code.push_back(val);
  code.push_back(addr);
}

void VM::jgt(uint val, uint addr)
{
  code.push_back(JGT);
  code.push_back(val);
  code.push_back(addr);
}

void VM::jle(uint val, uint addr)
{
  code.push_back(JLE);
  code.push_back(val);
  code.push_back(addr);
}

void VM::jlt(uint val, uint addr)
{
  code.push_back(JLT);
  code.push_back(val);
  code.push_back(addr);
}

void VM::jeq(uint val, uint addr)
{
  code.push_back(JEQ);
  code.push_back(val);
  code.push_back(addr);
}

void VM::jmp(uint addr)
{
  code.push_back(JMP);
  code.push_back(addr);
}

void VM::out()
{
  code.push_back(OUT);
}

void VM::in(uint val, uint addr)
{
  code.push_back(IN);
  code.push_back(val);
  code.push_back(addr);
}

void VM::dbg()
{
  code.push_back(DBG);
}

void VM::store(uint addr)
{
  code.push_back(STORE);
  code.push_back(addr);
}

void VM::load(uint addr)
{
  code.push_back(LOAD);
  code.push_back(addr);
}

void VM::sstore()
{
  code.push_back(SSTORE);
}

void VM::sload()
{
  code.push_back(SLOAD);
}

void VM::halt()
{
  code.push_back(HALT);
}

/*
 * This is a convenience method that is used to turn a std C++ string into a vector of packaged integers.
 * All instructions and memory points in the virutal are represented by 32-bit integers. However, a string
 * is an array of 8-bit integers, or "chars". In order to save space and prevent an entire 32-bit integer
 * from being used only by an 8-bit number, this convenience function will pack up to four chars into a
 * single integer, and return the vector representing the array of integers.
 *
 * These individual integers can be outputed with the OUT instruction.
*/
std::vector<uint> VM::string_to_int(std::string conv)
{
  int shift = sizeof(uint) * 8;
  std::string::iterator it = conv.begin();
  std::vector<uint> returner;
  uint int_chars = 0;

  while(shift > 0 && it != conv.end())
  {
    shift -= 8;
    int_chars = int_chars | (*it << shift);

    it++;

    if(shift == 0 || it == conv.end())          // If it reaches the end of the integer or the end of the string
    {
      shift = sizeof(uint) * 8;                 // Reset for next integer
      returner.push_back(int_chars);            // It must be returned eventually;
      int_chars = 0;
    }
  }

  return returner;
}

void VM::vec_to_mem(std::vector<uint> str, uint size, uint addr)
{
  alloc(addr + size);       // Allow an extra integer for the null integer at the end to delimit the string
  memory[addr + size] = 0;  // Null integer
  for(int i = 0; i < size && i < str.size(); i++)
  {
    memory[addr + i] = str[i];
  }
}

void VM::alloc(uint addr)
{
  if(memory.size() <= addr) memory.resize(addr + 1);  // Resize the array to include the memory address
}

bool VM::save(std::string filename)
{
  std::ofstream outfile;
  outfile.open(filename);

  if(!outfile.is_open())
  {
    error_state = ERR_OPEN_FILE;
    return false;
  }                                             // Exit early on error

  // The header
  outfile.put((char)SAM_BYTECODE_VER);          // The first byte is the bytecode version
  outfile.put((char)sizeof(uint));              // The second byte is the uint size (platform-stuff)
  for(int i = 0; i < 16; i++) outfile.put(0);   // 16 bytes of empty space reserved for future header/file additions

  //Cycle through all integers
  for(auto it = code.begin(); it != code.end(); it++)
  {
    // Cycle through every character in the integer
    for(int shift = sizeof(uint) * 8 - 8; shift >= 0; shift -= 8)
    {
      outfile.put((char)(*it >> shift));
    }
  }

  outfile.close();
  return true;
}

bool VM::load(std::string filename)
{
  std::ifstream infile;
  infile.open(filename);

  if(!infile.is_open())
  {
    error_state = ERR_OPEN_FILE;
    return false;
  }

  // Read the header
  if(infile.get() != SAM_BYTECODE_VER)          // Exit for incorrect bytecode version
  {
    error_state = ERR_BYTECODE_VER;
    return false;
  }
  if(infile.get() != sizeof(uint))              // Incorrect int size
  {
    error_state = ERR_INT_SIZE;
    return false;
  }
  for(int i = 0; i < 16; i++) infile.get();     // 16 dummy bytes reserved for later used

  // Read the code
  while(!infile.eof())
  {
    uint new_int = 0;

    for(int shift = sizeof(uint) * 8 - 8; shift >= 0; shift -= 8)
    {
      unsigned char read_char = infile.get();
      if(!infile.fail())
      {
        new_int |= ((uint)(read_char) << shift);
      }
    }

    code.push_back(new_int);
  }

  return true;
}
}

#endif
